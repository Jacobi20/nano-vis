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
	Function-like template class for converting variable arguments to string
	
	Using this template is quite similar to using old function 'va',
	but this template provides thread- and stack- safe functionality
	
	Example:
		RAISE_EXCPETION(va("file '%s' not found", file->GetName()));
		
	Do not use this template as argument for another 'va' or 'printf' without
	calling method 'c_str()'.
	Compiler does not call 'operator const char*' (because operator '...' 
	does not provide any type information) and access violation or another 
	error will occur.
	
	Also, DO NOT return result result of va
	
	Wrong:
		printf("%s", va("%d", 4));
	Correct:
		printf("%s", va("%d", 4).c_str());
-----------------------------------------------------------------------------*/

template <uint size> class EVa {
	public:
		explicit EVa	( const char *frmt, ... ) {
			va_list	argptr;
			va_start (argptr, frmt);
			vsnprintf (buffer, size-1, frmt, argptr);
			va_end (argptr);
		}
		
		~EVa	( void ) {
		}
		
		const char *c_str(void) const {
			return buffer;
		}
		
		operator const char* () const {
			return buffer;
		}
	private:
		char buffer[size];
		//	copying and assigning are forbidded :
		EVa<size> &operator = (EVa<size> &other) {}
		EVa<size> ( EVa<size> &other ) {}
	};


typedef EVa<512> va;


