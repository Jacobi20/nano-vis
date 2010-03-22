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
// i_proxy.hpp - 


#pragma once


#include <crtdbg.h>
#include "baseptr.hpp"
#include "hard.hpp"

/*-----------------------------------------------------------------------------
	Disposing proxy template :
-----------------------------------------------------------------------------*/

template<typename T>
class weak : public base_ptr<T> {
	public:
	
		//
		//	Constructors / Destructor :
		//	
		weak( void ) {
			ptr		= NULL;
			next	= NULL;
		}
		
		weak( T *p ) : base_ptr(p) {
			if (ptr) {
				ptr->AddWeakRef(this);
			}
		}

		weak( hard_ref<T> p ) : base_ptr(p.Get()) {
			if (ptr) {
				ptr->AddWeakRef(this);
			}
		}

		template<typename P>		
		weak( const weak<P> &other ) : ptr(other.ptr) {
			if (ptr) {
				ptr->AddWeakRef(this);
			}
		}
		
		~weak( void ) {
			if (ptr) {
				ptr->RemoveWeakRef(this);
				ptr = NULL;
			}
		}
		
		//
		//	Assignment operator :
		//		
		weak &operator = ( const weak &other ) {
		}
		
		weak &operator = ( T *p ) {
		}

	public:	//	TODO : make protected
		weak<T>	*next;			
	};