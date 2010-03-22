/*
    The MIT License

    Copyright (c) 2010 IFMO/GameDev Studio

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "core_local.h"


/*-----------------------------------------------------------------------------
	EAtomTable declaration :
-----------------------------------------------------------------------------*/

const uint HASH_SIZE	=	16384;


struct EAtomEntry_s {
		int		ref_count;			//	ref counter, -1 means that entry was deleted, is used for open addressing
		char	key[ATOM_SIZE];		//	hash key
	};


class	EAtomTable : public IAtomTable {
	public:
							EAtomTable	( void );
							~EAtomTable	( void );
		virtual uint		Add			( const char *str );
		virtual uint		Add			( uint atom );
		virtual void		Remove		( uint atom );
		virtual const char	*GetName	( uint atom ) const;
	protected:
		float	GetLoad	( void ) const;
	
		uint	Find	( const char *str );
		uint	Hash	( const char *str, uint i );
		EAtomEntry_s	table[HASH_SIZE];
	};


/*-----------------------------------------------------------------------------
	Export stuff
-----------------------------------------------------------------------------*/

DLL_EXPORT IAtomTable *Atoms(void) {
	static EAtomTable	atoms_static_imp;
	return &atoms_static_imp;
}

/*-----------------------------------------------------------------------------
	EAtomTable implementation
-----------------------------------------------------------------------------*/

EAtomTable::EAtomTable(void)
{
	for (uint i=0; i<HASH_SIZE; i++) {	
		table[i].ref_count = 0;
	}
}


EAtomTable::~EAtomTable(void)
{
}


uint EAtomTable::Hash(const char *str, uint i)
{
	//	function will not return 0.
	return ((JSHash(str) + i * RSHash(str)) % (HASH_SIZE-1)) + 1;	
}


uint EAtomTable::Add(const char *str)
{
	for (uint i=0; i<HASH_SIZE; i++) {
		uint h = Hash(str, i);
		
		if (table[h].ref_count>0) {
			if (strcmp(table[h].key, str)==0) {
				table[h].ref_count++;
				return h;
			}
		}
		
		if (table[h].ref_count<=0) {
			table[h].ref_count	=	1;
			strcpy(table[h].key, str);
			//_CrtDbgReport(_CRT_WARN, 0, 0, NULL, va("ATOM ADD: %s: %f\r\n", str, GetLoad()));
			return h;
		}
	}
	
	FATAL("Atom table overflow");
	return 0;
}


uint EAtomTable::Add(uint atom)
{
	uint h = atom;
	if (table[h].ref_count>0) {
		table[h].ref_count++;
		return h;
	}
	return 0;
}


void EAtomTable::Remove(uint atom)
{
	uint h = atom;
	if (h) {
		if (table[h].ref_count>0) {
		
			table[h].ref_count--;
			
			if (table[h].ref_count==0) {
				table[h].ref_count = -1;
				//_CrtDbgReport(_CRT_WARN, 0, 0, NULL, va("ATOM REM: %s\r\n", table[h].key));
			}
		}
	}
}


const char *EAtomTable::GetName(uint atom) const
{
	if (table[atom].ref_count>0) {
		return table[atom].key;
	}
	SIGNAL(va("Wrong atom 0x%X", atom));
	RAISE_EXCEPTION(va("Wrong atom 0x%X", atom));
	return NULL;
}


float EAtomTable::GetLoad( void ) const
{
	float num = 0;
	for (uint i=0; i<HASH_SIZE; i++) {
		if (table[i].ref_count>0) {
			num += 1.0f;
		}
	}	
	
	return num / HASH_SIZE;
}