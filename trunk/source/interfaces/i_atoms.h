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

#pragma once


/*-----------------------------------------------------------------------------
	Atom table :
	AddAtom() never returns zero
	DeleteAtom() can get zero as param
-----------------------------------------------------------------------------*/

const uint ATOM_SIZE	=	256;

class	IAtomTable {
	public:
		virtual uint		Add			( const char *str ) = 0;
		virtual uint		Add			( uint atom ) = 0;
		virtual void		Remove		( uint atom ) = 0;
		virtual const char	*GetName	( uint atom ) const = 0;
	};
	
	
DLL_EXPORT	IAtomTable	*Atoms	( void );

	
class	EName  {
	public:
		EName	( void ) {
			atom = Atoms()->Add("");
			string = Atoms()->GetName(atom);
		}
	
		EName ( const char *str ) {	
			atom = Atoms()->Add(str);
			string = Atoms()->GetName(atom);
		}
		
		EName	( const EName &other ) {
			atom = other.atom;
			Atoms()->Add(atom);
			string = Atoms()->GetName(atom);
		}
		
		~EName ( void ) {
			Atoms()->Remove( atom );
			atom = 0;
		}
		
		EName &operator = ( const EName &other ) {
			Atoms()->Remove( atom );
			atom = Atoms()->Add(other.atom);
			string = Atoms()->GetName(atom);
			return *this;
		}
		
		EName &operator = ( const char *str ) {
			Atoms()->Remove( atom );
			atom = Atoms()->Add(str);
			string = Atoms()->GetName(atom);
			return *this;
		}
		
		bool operator == ( const EName &other ) const {
			return atom==other.atom;
		}
		
		bool operator != ( const EName &other ) const {
			return atom!=other.atom;
		}
		
		const char *Name ( void ) const {
			return Atoms()->GetName( atom );
		} 
		
	protected:
		uint		atom;
		const char *string;
	};