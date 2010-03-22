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


void	common_parti( char *s, const char *sample );
EQuat	atoq		( const char *str );
EVec4	atov4		( const char *str );
EVec3	atov3		( const char *str );
EVec2	atov2		( const char *str );

bool	streq		( const char *a, const char *b );

unsigned int	StupidHash(const char *str);
unsigned int	RSHash(const char *str);
unsigned int	JSHash(const char* str);

bool	WildMatch	( const char *filter, const char *str );
bool	MatchFilter	( const char *filter, const char *name );
bool	IsAlpha		( char ch );
bool	IsDigit		( char ch );



void ParseFloat3(const char *str, float &a, float &b, float &c);
void ParseFloat4(const char *str, float &a, float &b, float &c, float &d);
string	TrimString	( const string &s );
string	DecolorizeString( const string &str );
string	RemoveExtension( const string &path );
string	ExtractDirectory( const string &path );

/*-----------------------------------------------------------------------------
	Addition to STL
-----------------------------------------------------------------------------*/
using namespace std;

struct ENocase {
		bool	operator() ( const string &a, const string &b ) const;
	};

struct EIHash {
		size_t	operator() ( const string &s ) const;
	};


inline bool ENocase::operator () ( const string &a, const string &b ) const
{
	string::const_iterator	p	=	a.begin();
	string::const_iterator	q	=	b.begin();
	
	while ( p!=a.end() && q!=b.end() && toupper(*p)==toupper(*q) )
	{
		p++; 
		q++;
	}
	
	if (p==a.end()) return q!=b.end();
	if (q==b.end()) return false;
	
	return toupper(*p)<toupper(*q);
}


inline size_t EIHash::operator () ( const string &s ) const
{
	return StupidHash(s.c_str());
}


/*-----------------------------------------------------------------------------
	Text function
-----------------------------------------------------------------------------*/








