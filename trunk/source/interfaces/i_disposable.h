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
// i_disposable.h - 


#pragma once


/*-----------------------------------------------------------------------------
	IDisposable
	
	Base object for reference counted objects.
	See an example: http://www.codeproject.com/KB/COM/simplecomserver.aspx
-----------------------------------------------------------------------------*/

class CORE_API IDisposable {
	public:
						IDisposable	( void ) : ref_count(0), handle(0) { }
		virtual			~IDisposable( void ) { /**/ }

		//
		//	AddRef()
		//
		virtual int		AddRef		( void ) const { 
			return InterlockedIncrement(&ref_count);
		}

		//
		//	Dispose()
		//			
		virtual int		Dispose		( void ) const { 
			ASSERT(ref_count>0);
			if (InterlockedDecrement(&ref_count)==0) {
				delete this; 
				return 0;
			}
			return ref_count;
		}
		
		//
		//	GetRefCount()
		//	
		virtual int		GetRefCount	( void ) const { 
			return ref_count; 
		}
		
		virtual void	SetHandle	( uint h ) { handle = h; }
		virtual uint	Handle		( void ) const { return handle; }

		////
		////	AddWeakRef()
		////
		//void	AddWeakRef	( weak<IDisposable> *weak_ref ) {
		//	ASSERT(weak_ref==this);
		//#if 0
		//	ref_list.push_back( weak_ref );
		//#else
		//	list< weak<IDisposable>* >::iterator li;

		//	li = find( ref_list.begin(), ref_list.end(), weak_ref );
		//	
		//	if (li==ref_list.end()) {
		//		ref_list.push_back( weak_ref );
		//	} else {
		//		SIGNAL("Attempting to add weak reference twice");
		//	}
		//#endif
		//}
		//
		////
		////	RemoveWeakRef()
		////
		//void	RemoveWeakRef	( weak<IDisposable> *weak_ref ) {
		//	ASSERT(weak_ref==this);

		//	list< weak<IDisposable>* >::iterator li;
		//	
		//	li = find( ref_list.begin(), ref_list.end(), weak_ref );
		//	
		//	if (li==ref_list.end()) {
		//		SIGNAL("Attempting to remove weak reference, which was not added");
		//	} else {
		//		ref_list.erase( li );
		//	}
		//}
		
	private:
		uint	handle;	
		mutable volatile long int ref_count;
	};
	

