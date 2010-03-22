//
//	HASH.H
//

#pragma once

#include "./library.h"



#define		HASH_EMPTY		0
#define		HASH_DELETED	((void*)(-1))


template<class Type>
class ETHashInfoClass	{
	public:
	static void	PrintEmpty	( ehandle_t hkey )	{ PRINTF("Empty   : %d\r\n", hkey); }
	static void	PrintDeleted( ehandle_t	hkey )	{ PRINTF("Deleted : %d\r\n", hkey); }
	static void	PrintEngaged( ehandle_t	hkey, const char *key, const Type &value ) 
												{ PRINTF("Engaged : %d\r\n", hkey); }
};



template<class Type, uint size, bool case_sensitivity>
class	ETHashTable	
	{
	private:
		struct	hashEntry_s {
			EStr	key;
			Type	value;
		};


		static ehandle_t	Hash	( const char *key, uint n );
	
	private:
		hashEntry_s	*table[size];
		uint		hashmask;
		uint		num_entries;

	public:
						ETHashTable		( void );
						~ETHashTable	( void );
					
		ehandle_t		Insert			( const char *key, const Type &val );
		ehandle_t		Search			( const char *key );
		void			Delete			( const char *key );
		void			DeleteAll		( void );
		
		Type			&GetValue		( ehandle_t hkey );		// returns reference to item, or raises exception
		Type			*GetValuePtr	( ehandle_t	hkey );		// returns pointer to item or NULL
		const char		*GetKey			( ehandle_t	hkey );		// returns key of item
		
		bool			IsEmpty			( ehandle_t hkey )	{ return ( table[ hkey & hashmask ]==HASH_EMPTY ); }
		bool			IsDeleted		( ehandle_t hkey )	{ return ( table[ hkey & hashmask ]==HASH_DELETED ); }
		bool			IsEngaged		( ehandle_t hkey )	{ return ( !IsEmpty(hkey) && !IsDeleted(hkey) ); }
		
		uint			Engaged			( void ) { return num_entries; }				// number of entries
		float			Density			( void ) { return (float)num_entries/size; }	// number of entries / size


		template<class Info> void	PrintHashedValues	( void );
		template<class Info> void	PrintSortedValues	( void );
	};
	


	
template<class Type, uint size, bool case_sensitivity>
INLINE ETHashTable<Type, size, case_sensitivity>::ETHashTable(void)
{
	if ( !is_power_of_two(size) )	FATAL("Size of table must be power of two!");
	
	hashmask	=	size-1;
	num_entries	=	0;
	
	for (int i=0; i<size; i++) table[i] = HASH_EMPTY;
}


template<class Type, uint size, bool case_sensitivity>
INLINE ETHashTable<Type, size, case_sensitivity>::~ETHashTable(void) 
{	
	DeleteAll();
}

template<class Type, uint size, bool case_sensitivity>
INLINE ehandle_t ETHashTable<Type, size, case_sensitivity>::Hash(const char *key, uint n) 
{
	uint h = ( StupidHash(key)+n ) & (size-1);
	if (h==0) h++;
	return h;
}


//	
//	ETHashTable<>::Insert()
//	exceptions:
//		- when specified item is already defined
//		- when hash-table overflows
//
template<class Type, uint size, bool case_sensitivity>
INLINE ehandle_t ETHashTable<Type, size, case_sensitivity>::Insert( const	char *key, const Type &value )
{
	if ( Search(key) ) RAISE_EXCEPTION(va("Entry with key \"%s\" is already added", key));
	ehandle_t h;
	for (int i=0; i<size; i++)
	{
		h = Hash(key, i);
		if ( table[h]==HASH_EMPTY || table[h]==HASH_DELETED )
		{
			table[h] = new hashEntry_s;
			table[h]->key	= key;
			table[h]->value	= value;
			num_entries++;
			return h;
		}
	}
	RAISE_EXCEPTION("Hash-table overflow");
	return 0;
}


//
//	ETHashTable<>::Search()
//	returns handle of found item or NULL
//
template<class Type, uint size, bool case_sensitivity>
INLINE ehandle_t ETHashTable<Type, size, case_sensitivity>::Search( const char *key )
{
	ehandle_t h;
	for (int i=0; i<size; i++) 
	{
		h = Hash(key, i);
		if ( table[h]==HASH_EMPTY )		break;
		if ( table[h]==HASH_DELETED )	continue;
		if (case_sensitivity)	{	if ( table[h]->key.Cmp(key) ==0) return h; }	// case-sensitive compare
						else	{	if ( table[h]->key.ICmp(key) ==0) return h; }	// case-insensitive comapar
	}
	return NULL;	
}


//
//	ETHashTable<>::Delete()
//	true -	specified item has been deleted
//	false - specified item not found
//
template<class Type, uint size, bool case_sensitivity>
INLINE void ETHashTable<Type, size, case_sensitivity>::Delete( const char *key )
{
	uint h = Search(key);
	if (!h) RAISE_EXCEPTION(va("item \"%s\" does not exist", key));

	delete table[h];
	table[h] = (hashEntry_s*)HASH_DELETED;
	num_entries--;
}


//
//	ETHashTable<>::DeleteAll()
//
template<class Type, uint size, bool case_sensitivity>
INLINE void ETHashTable<Type, size, case_sensitivity>::DeleteAll( void )
{
	for (int i=0; i<size; i++) 
		if (table[i]!=HASH_EMPTY && table[i]!=HASH_DELETED)	{
			delete table[i];
			table[i] = HASH_EMPTY; 
		}
	num_entries = 0;
}


//
//	ETHashTable<Type, size, case_sensitivity>::GetValue()
//
template<class Type, uint size, bool case_sensitivity>
INLINE Type &ETHashTable<Type, size, case_sensitivity>::GetValue( ehandle_t hkey ) 
{
	if ( !IsEngaged(hkey) ) RAISE_EXCEPTION("wrong hash-key");
	return ( table[ hkey & hashmask ]->value );
}


//
//	ETHashTable<Type, size, case_sensitivity>::GetValuePtr()
//
template<class Type, uint size, bool case_sensitivity>
INLINE Type *ETHashTable<Type, size, case_sensitivity>::GetValuePtr( ehandle_t hkey ) 
{
	if ( !IsEngaged(hkey) ) RAISE_EXCEPTION("wrong hash-key");
	return &( table[ hkey & hashmask ]->value );
}


//
//	ETHashTable<Type, size, case_sensitivity>::GetKey()
//
template<class Type, uint size, bool case_sensitivity>
INLINE const char *ETHashTable<Type, size, case_sensitivity>::GetKey( ehandle_t hkey ) 
{
	if ( !IsEngaged(hkey) ) RAISE_EXCEPTION("wrong hash-key");
	return table[ hkey & hashmask ]->key.c_str();
}


//
//	ETHashTable<Type, size, case_sensitivity>::PrintHashedValues<>()
//
template<class Type, uint size, bool case_sensitivity>		// class template
template <class Info>										// function template
INLINE void ETHashTable<Type, size, case_sensitivity>::PrintHashedValues( void )
{
	for (int i=0; i<size; i++) {
		if ( IsEmpty(i) )	Info::PrintEmpty(i);
		if ( IsDeleted(i) )	Info::PrintDeleted(i);
		if ( IsEngaged(i) )	Info::PrintEngaged(i, table[i]->key.c_str(), table[i]->value);
	}
}


//
//	ETHashTable<Type, size, case_sensitivity>::PrintSortedValues<>()
//
template<class Type, uint size, bool case_sensitivity>		// class template
template <class Info>										// function template
INLINE void ETHashTable<Type, size, case_sensitivity>::PrintSortedValues( void )
{
	uint	num	=	num_entries;
	const char	*list[size];
	uint i,j;
	
	for (i=0, j=0; i<size; i++)	{
		if (!IsEngaged(i)) continue;
		
		list[j] = table[i]->key.c_str();
		j++;
	}
	
	if (case_sensitivity)	ETFBubbleSort<const char*, EAPCharCmp>(list, num);
					else	ETFBubbleSort<const char*, EAPCharICmp>(list, num);
				
	//for (i=0; i<num; i++) PRINTF("%s\r\n", list[i]);	// DEBUG...
	
	for (i=0; i<num; i++) {	
		ehandle_t	hkey;
		ASSERT( hkey = Search(list[i]) );
		Type *val;
		ASSERT( val = GetValuePtr(hkey) );
		
		Info::PrintEngaged(hkey, list[i], *val);
	}
}


















