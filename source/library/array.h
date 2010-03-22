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


template <class Type> class EArray {
	public:
		EArray	( void );
		EArray	( const EArray &other );
		~EArray	( void );
		
		
		uint		size		( void ) const;
		void		resize		( uint new_length );
		void		resize		( uint new_length, Type a );
		Type		operator[]	( uint n ) const;
		Type		&operator[]	( uint n );
		EArray		&operator=	( const EArray &other );
		void		clear		( void );

	private:
		uint data_size;
		Type *data;
	};


template<class Type> EArray<Type>::EArray(void)
{
	data = NULL;
	data_size = 0;
}

template<class Type> EArray<Type>::~EArray(void)
{
	clear();
}


template<class Type> EArray<Type>::EArray(const EArray &other)
{
	*this = other;
}


template<class Type> uint EArray<Type>::size(void) const
{
	return data_size;
}


template<class Type> void EArray<Type>::resize(uint new_size)
{
	if (data_size == new_size) {
		return;
	}

	
	if (data_size==0) {
		delete[] data;
		data = NULL;
		data_size = 0;
	}


	Type *temp = data;
	data = new Type[new_size];
	
	for (uint i=0; i<min(data_size, new_size); i++) {
		data[i] = temp[i];
	}
	
	delete[] temp;

	data_size = new_size;
}


template<class Type> void EArray<Type>::resize(uint new_size, Type a)
{
	if (data_size == new_size) {
		return;
	}

	
	if (data_size==0) {
		delete[] data;
		data = NULL;
		data_size = 0;
	}


	Type *temp = data;
	data = new Type[new_size];
	
	for (uint i=0; i<min(data_size, new_size); i++) {
		data[i] = temp[i];
	}
	
	if (new_size > data_size) {
		for (uint i=data_size; i<new_size; i++) {
			data[i] = a;
		}
	}
	
	delete[] temp;

	data_size = new_size;
}


template<class Type> EArray<Type> &EArray<Type>::operator =(const EArray<Type> &other)
{
	delete[] data;
	
	if (other.data_size==0) {
		return *this;
	}

	data_size	=	other.data_size;
	data = new Type[data_size];
	
	for (uint i=0; i<data_size; i++) {
		data[i] = other.data[i];
	}
	
	return *this;
}


template<class Type> Type EArray<Type>::operator [](uint n) const
{
	ASSERT(data_size);
	ASSERT(n<data_size);
	return data[n];
}


template<class Type> Type &EArray<Type>::operator [](uint n)
{
	ASSERT(data_size);
	ASSERT(n<data_size);
	return data[n];
}


template<class Type> void EArray<Type>::clear(void)
{
	data_size = 0;
	delete[] data;
	data = NULL;
}
