/*
	The MIT License

	Copyright (c) 2010

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


class EAttributeList {
	public:
						EAttributeList	( void );
						~EAttributeList	( void );
		const char		*GetValue		( const char *key, const char *def_value ) const;
		void			SetValue		( const char *key, const char *value );
		void			ClearParams		( void ) { pairs.clear(); }
		void			AppendParams	( const EAttributeList *other );
		
		uint			GetParamNum		( void ) const;
		const char		*GetValueByID	( uint index ) const;
		const char		*GetKeyByID		( uint index ) const;
		
	protected:
		struct pair_s {
			string	key;
			string	value;
		};
		const pair_s	*FindPair	( const char *key ) const;
		vector<pair_s>	pairs;
	};