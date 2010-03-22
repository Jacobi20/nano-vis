//
//	ARRAY.H
//
//	descr : dynamic array template
//

#pragma once

/*
===============================================================================

	TArray	- dynamic array with granularity
	
	[d][d][d][d][d][d][d][d][d][d][d][d][d][.][.][.]	
	[  granule ][  granule ][  granule ][  granule ]

===============================================================================
*/
template<class type>	
inline int	EArraySortCriteria	( const type *a, const type *b )
{
	if (*a > *b)	return +1;
	if (*a < *b)	return -1;
	return 0;
}

template<class type>
inline bool EArrayEqualCriteria( const type *a, const type *b )
{
	if (*a==*b) return 1;
	return 0;
}



template <class type, uint granularity=8> class	TArray
	{
	private:

	typedef		int		(*cmp_t)	( const type *a, const type *b );
	typedef		bool	(*equal_t)	( const type *a, const type *b );
	
	typedef		TArray<type,granularity>	array_type;

		uint		size;			//	number of allocated elements.
		uint		num;			//	number of elements.
		type		*data;			
		
	public:
					TArray		( void );
					TArray		( const array_type &array );
		explicit	TArray		( uint newnum );
		explicit	TArray		( uint newnum, const type &val );
		explicit	TArray		( uint newnum, const type *array );
					~TArray		( void );
					
		array_type	&operator = ( const array_type &array );
		const type	&operator[]	( int index ) const;
		type		&operator[]	( int index );
		const type	*Ptr		( void ) const;
		type		*Ptr		( void );	
		
		uint		Size		( void ) const;
		uint		Num			( void ) const;

		void		Clear		( void );					
		void		Resize		( uint newsize );					
		void		SetNum		( uint newnum, bool resize=true );	
		void		Insert		( uint index, const type &val );
		void		Remove		( uint index );
		void		Append		( const type &val );
		void		RemoveLast	( void );
		int			IndexOf		( const type *ptr ) const;
		
		int			Find		( const type &sample, equal_t equal = EArrayEqualCriteria<type> ) const;
		void		Sort		( cmp_t compare = EArraySortCriteria<type> );
		void		SortRange	( uint begin, uint end, cmp_t compare = EArraySortCriteria<type> );

		/*
		template<class equal_criteria>
		int		FindCriteria	( const type &sample );
		template<class sort_criteria>
		void	SortCriteria	( bool reverse );
		*/
	};



//
//	TArray::TArray()
//	
template<class type, uint granularity>
TArray<type,granularity>::TArray( void )
{
	ASSERT(IsPowerOfTwo(granularity));
	size = num = 0;
	data = NULL;
}


template<class type, uint granularity>
TArray<type, granularity>::TArray( const array_type &array )
{
	ASSERT(IsPowerOfTwo(granularity));
	size = num = 0;
	data = NULL;
	*this = array;	// ???
}


template<class type, uint granularity>
TArray<type,granularity>::TArray( uint newnum )
{
	ASSERT(IsPowerOfTwo(granularity));
	size = num = 0;
	data = NULL;
	SetNum(newnum);
}


template<class type, uint granularity>
TArray<type,granularity>::TArray( uint newnum, const type &val )
{
	ASSERT(IsPowerOfTwo(granularity));
	size = num = 0;
	data = NULL;
	SetNum(newnum);
	for (uint i=0; i<Num(); i++)	data[i] = val;
}


template<class type, uint granularity>
TArray<type,granularity>::TArray( uint newnum, const type *array )
{
	ASSERT(IsPowerOfTwo(granularity));
	size = num = 0;
	data = NULL;
	SetNum(newnum);
	for (uint i=0; i<Num(); i++)	data[i] = array[i];
}


template<class type, uint granularity>
TArray<type,granularity>::~TArray( void )
{
	Clear();
}



//
//	TArray::operator=()
//
template<class type, uint granularity>
TArray<type,granularity> &TArray<type,granularity>::operator = (const array_type &array)
{
	if (this==&array) return *this;
	
	SetNum(array.num, true);
	
	for (uint i=0; i<array.num; i++)	
		data[i] = array.data[i];
		
	return *this;
}


//
//	TArray::Size()
//
template <class type, uint granularity>
uint	TArray<type,granularity>::Size( void ) const
{
	return size;
}


//
//	TArray::Num()
//
template <class type, uint granularity>
uint	TArray<type,granularity>::Num( void ) const
{
	return num;
}


//
//	TArray::Clear()
//	delete all stored data
//
template <class type, uint granularity>
void	TArray<type,granularity>::Clear( void )
{
	delete[] data;
	size = num = 0;
	data = NULL;
}


//
//	TArray::Resize()
//
template<class type, uint granularity>
void	TArray<type,granularity>::Resize( uint newsize )
{
	uint gran_new_size = Granulate(newsize, granularity);

	if ( size==gran_new_size ) 
		return;

	if ( gran_new_size==0 ) {
		Clear();
		return;
	}

	if ( num > gran_new_size )	
		num = gran_new_size;

	type *olddata	=	data;
	
	size = gran_new_size;
	data = new type[size];

	for (uint i=0; i<num; i++) 
		data[i] = olddata[i];
	// FIXME : may be 'memcpy()'?

	delete[] olddata;
}


//
//	TArray::SetNum()
//
template<class type, uint granularity>
void	TArray<type,granularity>::SetNum( uint newnum, bool resize )
{
	if ( (newnum > size) || resize )	Resize(newnum);
	num = newnum;
}


//
//	ÅArray::Ptr()
//
template<class type, uint granularity>
type	*TArray<type,granularity>::Ptr( void ) 
{
	return data;
}


//
//	ÅArray::Ptr()
//
template<class type, uint granularity>
const type	*TArray<type,granularity>::Ptr( void ) const 
{
	return data;
}


//
//	TArray::operator[]
//
template<class type, uint granularity>
const type	&TArray<type,granularity>::operator [](int index) const 
{
	ASSERT( index>=0 && index<(int)num );	
	return data[index];
}


//
//	TArray::operator[]
//
template<class type, uint granularity>
type	&TArray<type,granularity>::operator [](int index) 
{
	ASSERT( index>=0 && index<(int)num );	
	return data[index];
}


//
//	TArray::Insert()
//
template<class type, uint granularity>
void TArray<type,granularity>::Insert(uint index, const type &val)
{
	// NOTE : index can be equal to 'num'!
	ASSERT( index>=0 && index<=num );	
	
	uint oldnum = num;
	SetNum(num+1);

	for (uint i=index; i<oldnum; i++)
		data[i+1] = data[i];

	data[index] = val;
}


//
//	TArray::Delete()
//
template<class type, uint granularity>
void TArray<type,granularity>::Remove(uint index)
{
	ASSERT( index>=0 && index<num );	

	uint oldnum = num;

	for (uint i=index; i<oldnum-1; i++)
		data[i] = data[i+1];

	SetNum(num-1);
}


//
//	TArray::Append()
//
template<class type, uint granularity>
void TArray<type,granularity>::Append(const type &val)
{
	Insert(num, val);
}


//
//	TArray::RemoveLast()
//
template<class type, uint granularity>
void TArray<type,granularity>::RemoveLast(void)
{
	Remove(num-1);
}


//
//	TArray::Find()
//
template<class type, uint granularity>
int TArray<type,granularity>::Find(const type &sample, equal_t equal) const
{
	for (int i=0; i<(int)num; i++)
		if ( equal(&data[i], &sample) ) 
			return i;
	
	return -1;	// not found
}


//
//	TArray::Sort()
//
template<class type, uint granularity>
void TArray<type,granularity>::Sort( cmp_t compare )
{
	if (!data) return;
	qsort(data, num, sizeof(type), (int (__cdecl*)(const void*,const void*))compare);
}


//
//	TArray::SortRange()
//
template<class type, uint granularity>
void TArray<type,granularity>::SortRange( uint a, uint b, cmp_t compare )
{
	if (!data) return;
	
	if (a<0)	a = 0;
	if (a>num)	a = num;
	if (b<0)	b = 0;
	if (b>num)	b = num;
	if (a>b) return;
	int length = b-a;
	
	qsort( data+a, length, sizeof(type), (int (__cdecl*)(const void*,const void*))compare);
}


//
//	
//
template<class type, uint granularity>
int TArray<type,granularity>::IndexOf(const type *ptr) const
{
	if ( ptr<data )			return -1;
	if ( ptr>=data+num )	return -1;
	//if ( reinterpret_cast<uint>(ptr) % sizeof(type)!=0 ) return -1;
	
	return (ptr-data);
}







