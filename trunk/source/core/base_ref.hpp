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

/*-----------------------------------------------------------------------------
	Base pointer :
-----------------------------------------------------------------------------*/

template<typename T>
class base_ptr {
	public:
	
		//
		//	Constructors / Destructor :
		//	
		base_ptr( void ) {
		}
		
		base_ptr( T *p ) : ptr(p) {
		}
		
		base_ptr( const base_ptr &other ) : ptr(other.ptr) {
		}
		
		~base_ptr( void ) {
		}

		//
		//	Access functions and operators :
		//
		T *operator -> ( void ) {
			if (!ptr) {	
				SIGNAL(va("Hard ref '%s' is not initialized", typeid(T).name()));
				RAISE_EXCEPTION(va("Proxy object '%s' is not initialized", typeid(T).name()));	
			}
			return ptr;
		}
	
		const T *operator -> ( void ) const {
			if (!ptr) {	
				SIGNAL(va("Hard ref '%s' is not initialized", typeid(T).name()));
				RAISE_EXCEPTION(va("Proxy object '%s' is not initialized", typeid(T).name()));	
			}
			return ptr;
		}
		
		T *Get ( void ) {
			return ptr;
		}

		const T *Get ( void ) const {
			return ptr;
		}
		
		//
		//	Static and dynamic casting :
		//
		template <typename T1> T1 *To	(void) {
			return static_cast<T1*>(ptr);
		}

		template <typename T1> T1 *As	(void) {
			return dynamic_cast<T1*>(ptr);
		}

		template <typename T1> const T1 *To	(void) const {
			return static_cast<const T1*>(ptr);
		}

		template <typename T1> const T1 *As	(void) const {
			return dynamic_cast<const T1*>(ptr);
		}

		//
		//	Comparison operator :
		//
		bool operator! (void) const {
			return (ptr==0);
		}
		
		bool operator == ( const base_ptr &other ) const {
			return (ptr==other.ptr);
		}
		
		bool operator == ( const T *other ) const {
			return (ptr==other);
		}
		
		//	NOTE : See A. Alexandrescu, "Modern C++ Design", p 199:
		class IfPtrCheck { void operator delete (void*); };
			
		operator IfPtrCheck* (void) const {
			if (!ptr) return NULL;
			static IfPtrCheck check;
			return &check;
		}
	
	protected:
		T	*ptr;
	};
	