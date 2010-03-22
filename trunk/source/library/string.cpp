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

#include "library.h"


/*-----------------------------------------------------------------------------
	Wild Cards :
-----------------------------------------------------------------------------*/

//
//	WildMatch
//	http://xoomer.alice.it/acantato/dev/wildcard/wildmatch.html
//
bool WildMatch(const char *filter, const char *str) 
{
	switch (*filter) {
		case '\0':
			return !*str;
		case '*' :
			return WildMatch(filter+1, str) || *str && WildMatch(filter, str+1);
		case '?' :
			return *str && (*str != '.') && WildMatch(filter+1, str+1);
		default  :
			return (*str == *filter) && WildMatch(filter+1, str+1);
			//return (mapCaseTable[*str] == mapCaseTable[*filter]) && WildMatch(filter+1, str+1);
	}
}

//
//	MatchFilter()
//	- function was swiped from Doom3 SDK,
//	  but, probably does not work properly...
//
bool MatchFilter(const char *filter, const char *name) 
{
	string buf;
	int i, found, index;

	while(*filter) {
		// "*"
		if (*filter == '*') {
			filter++;
			buf.clear();
			for (i = 0; *filter; i++) {
				if ( *filter == '*' || *filter == '?' || (*filter == '[' && *(filter+1) != '[') ) {
					break;
				}
				buf += *filter;
				if ( *filter == '[' ) {
					filter++;
				}
				filter++;
			}
			if ( buf.length() ) {
				index = strstr(name, buf.c_str()) - name;
				if ( index == -1 ) {
					return false;
				}
				name += index + buf.length();
			}
		}
		// "?"
		else if (*filter == '?') {
			filter++;
			name++;
		}
		// "[..]"
		else if (*filter == '[') {
			if ( *(filter+1) == '[' ) {
				if ( *name != '[' ) {
					return false;
				}
				filter += 2;
				name++;
			}
			else {
				filter++;
				found = false;
				while(*filter && !found) {
					if (*filter == ']' && *(filter+1) != ']') {
						break;
					}
					if (*(filter+1) == '-' && *(filter+2) && (*(filter+2) != ']' || *(filter+3) == ']')) {
						if (*name >= *filter && *name <= *(filter+2)) {
							found = true;
						}
						filter += 3;
					}
					else {
						if (*filter == *name) {
							found = true;
						}
						filter++;
					}
				}
				if (!found) {
					return false;
				}
				while(*filter) {
					if ( *filter == ']' && *(filter+1) != ']' ) {
						break;
					}
					filter++;
				}
				filter++;
				name++;
			}
		}
		else {
			if (*filter != *name) {
				return false;
			}
			filter++;
			name++;
		}
	}
	return true;
}

/*-----------------------------------------------------------------------------
	String manipulation :
-----------------------------------------------------------------------------*/

//
//	RemoveExtension
//
string RemoveExtension(const string &path)
{
	string temp = path;
	for (int i=path.length(); i>=0; i--) {
		if (path[i]=='.') {
			temp.resize(i);
		}
	}
	return temp;
}


//
//	BackSlashesToForwardSlashes
//
void BackSlashesToForwardSlashes(char *str)
{
	uint len = strlen(str);
	for (uint i=0; i<len; i++) {
		if (str[i]=='\\') {
			str[i]='/';
		}
	}
}


//
//	DecolorizeString
//	- removes coloring escape characters, rules :
//		string -> string
//		^1A^2B -> AB
//		^^A    -> ^A
//		^xA    -> X
//
string DecolorizeString( const string &str )
{
	string result;
	result.reserve(str.size());
	
	bool skip_next = false;

	for (uint i=0; i<str.length(); i++) {
		if (skip_next) {
			skip_next = false;
			continue;
		}
		
		if (str[i]=='^') {
			skip_next = true;
			continue;
		}

		result.push_back(str[i]);
	}
	
	return result;
}


//
//	TrimString
//
string	TrimString	( const string &sample )
{
	const char *scan = sample.c_str();
	
	while (*scan) {
		if (*scan==' ' || *scan=='\t' || *scan=='\r' || *scan=='\n' ) {
			scan++;
		} else {
			break;
		}
	}

	string result = scan;	

	while (result.length()>0) {
		char ch = result[ result.length()-1 ];
		if (ch==' ' || ch=='\t' || ch=='\r' || ch=='\n') {
			result.resize(result.length()-1);
		} else {
			break;
		}
	}
	
	return result;
}


//
//	ExtractDirectory
//
string ExtractDirectory( const string &path )
{
	uint slash_pos  = path.rfind('/');
	uint rslash_pos = path.rfind('\\');
	
	slash_pos = max(rslash_pos, slash_pos);
	
	return path.substr(0, slash_pos);
}


/*-----------------------------------------------------------------------------
	"atoi"-like functions :
-----------------------------------------------------------------------------*/

EQuat atoq(const char *str) {
	EQuat q(0,0,0,0);
	sscanf(str, "%f%f%f%f", &q.x, &q.y, &q.z, &q.w);
	return q;
}

EVec4 atov4(const char *str) {
	EVec4 v(0,0,0,0);
	sscanf(str, "%f%f%f%f", &v.x, &v.y, &v.z, &v.w);
	return v;
}

EVec3 atov3(const char *str) {
	EVec3 v(0,0,0);
	sscanf(str, "%f%f%f", &v.x, &v.y, &v.z);
	return v;
}

EVec2 atov2(const char *str) {
	EVec2 v(0,0);
	sscanf(str, "%f%f", &v.x, &v.y);
	return v;
}


bool streq( const char *a, const char *b )
{
	int i = strcmp(a, b);
	return (i==0);
}


//
//	ParseFloat3
//
void ParseFloat3(const char *str, float &a, float &b, float &c)
{
	a = b = c = 0;
	EArgs	args(str);
	a = (float)atof(args.Argv(0));
	b = (float)atof(args.Argv(1));
	c = (float)atof(args.Argv(2));
}

//
//	ParseFloat4
//
void ParseFloat4(const char *str, float &a, float &b, float &c, float &d)
{
	a = b = c = d = 0;
	EArgs	args(str);
	a = (float)atof(args.Argv(0));
	b = (float)atof(args.Argv(1));
	c = (float)atof(args.Argv(2));
	d = (float)atof(args.Argv(3));
}


//
//	va()
//	deprecated, use template EVa
//
#if 0
char *va(const char *frmt, ...)
{
	static uint hits = 0;
	static char text[8][32000];
	
	char	*ptext = text[(hits++)%8];
	
	va_list	argptr;

	va_start (argptr, frmt);
	vsnprintf (ptext, 31999, frmt, argptr);
	va_end (argptr);

	return ptext;
}
#endif


//
//	common_parti()
//	copy to "s" common part of "s" & "sample"
//	example:	"wgl_mode" & "wgl_fullscr" -> "wgl_"
//
void common_parti(char *s, const char *sample)
{
	int i;
	for (i=0; s[i], sample[i]; i++)
	{	
		if ( tolower(s[i])!=tolower(sample[i]) )
		{
			s[i]='\0';
			return;
		}
	}
}

/*-----------------------------------------------------------------------------
	Text validating 	
-----------------------------------------------------------------------------*/
bool IsAlpha( char ch )
{
	if ('a'<=ch && ch<'z') return true;
	if ('A'<=ch && ch<'Z') return true;
	if (ch=='_') return true;
	return false;
}

bool IsDigit(char ch)
{
	if ('0'<=ch && ch<='9') return true;
	return false;
}


/*-----------------------------------------------------------------------------
	HASHING
-----------------------------------------------------------------------------*/

unsigned int StupidHash(const char *str)
{
	int hash = 7;
	for ( ; *str; str++) hash += tolower(*str);
	return (hash & 0x7FFFFFFF);
}

unsigned int RSHash(const char *str)
{
	unsigned int b    = 378551;
	unsigned int a    = 63689;		 
	unsigned int hash = 0;			 

	for( ; *str; str++)	{
		hash = hash*a + tolower(*str);
		a = a*b;
	}   

	return (hash & 0x7FFFFFFF);
}

unsigned int JSHash(const char* str)
{
	unsigned int hash = 1315423911;

	for( ; *str; str++)
			hash ^= ((hash << 5) + tolower(*str) + (hash >> 2));
	return (hash & 0x7FFFFFFF);
}


