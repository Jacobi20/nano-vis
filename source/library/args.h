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


class EArgs	
	{
	public:
					EArgs		( void ) { Reset(); }
					EArgs		( const char *str ) { Reset(); Tokenize(str); }
					EArgs		( const EArgs &other ) { *this = other; }
					~EArgs		( void ) { /**/ }

		void		Reset		( void );
		int			Argc		( void ) const;
		const char	*Argv		( int n ) const;
		const char	**Argv		( void ) const;
		void		Tokenize	( const char *str ); 
		bool		CheckParam	( const char *p, int *index=NULL ) const;
		
		void		NewArg		( void );
		void		NewArg		( const char *str );
		void		PushChar	( char ch );
		EArgs		&operator=	( const EArgs &other );
		
	private:
		static const int	ARG_MAX_ARGS	= 64;
		static const int	ARG_MAX_CHARS	= 2048;
		
		int		argc;
		int		cursor;
		char	*argv[ARG_MAX_ARGS];
		char	cmdline[ARG_MAX_CHARS];
	};

