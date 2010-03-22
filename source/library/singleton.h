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
	Static Singleton	- also known as Meyers`s singleton
-----------------------------------------------------------------------------*/

template <class Type>
class	EStaticSingleton
	{
	public:
		static	Type	*Instance	( void );
	};
	
//
//	EStaticSingleton<Type>::Instance	
//
template <class Type>
Type *EStaticSingleton<Type>::Instance(void)
{
	static	Type	inst;
	return	&inst;
}



/*-----------------------------------------------------------------------------
	Undying Singleton	(destructor will not be called)
-----------------------------------------------------------------------------*/

template <class Type>
class	EUndyingSingleton
	{
	public:
		static	Type	*Instance	(void);
	};
	


//
//	EStaticSingleton<Type>::Instance	
//
template <class Type>
Type *EUndyingSingleton<Type>::Instance(void)
{
	static char	buffer[sizeof(Type)];
	static Type	*inst = NULL;
	
	if (!inst)
	{	
		new (buffer) Type;
		inst =	reinterpret_cast<Type*>(buffer);
	}
	
	return inst;
}












