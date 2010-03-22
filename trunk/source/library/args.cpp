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

EArgs &EArgs::operator =(const EArgs &other)
{
	Reset();

	argc	=	other.argc;
	cursor	=	other.cursor;
	memcpy(cmdline, other.cmdline, ARG_MAX_CHARS);
	
	for (int i=0; i<argc; i++)
	{
		argv[i] = &cmdline[0] + (other.argv[i] - &other.cmdline[0] );
	}
	
	return *this;
}


//
//	EArgs::Reset()
//
void EArgs::Reset(void)
{
	argc = 0;
	cursor = 0;
	for (int i=0; i<ARG_MAX_ARGS; i++) argv[i] = "";
	memset(cmdline, '\0', ARG_MAX_CHARS);
}


//
//	EArgs::Argñ()
//
int EArgs::Argc(void) const
{
	return argc;
}


//
//	EArgs::Argv()
//
const char *EArgs::Argv(int n) const
{
	if (n>=ARG_MAX_ARGS || n<0) return "";
	return argv[n];
}


//
//	EArgs::Argv()
//
const char **EArgs::Argv() const
{
	return (const char**)argv;
}


//
//	EArgs::CheckParam()
//	returns the position (1 to argc-1) in the program's argument list
//	where the given parameter apears, or 0 if not present
//
bool EArgs::CheckParam( const char *p, int *index/* =NULL */ ) const
{
	for (int i=0; i<Argc(); i++) {
		if ( _strcmpi(Argv(i), p)==0 ) {
			if (index) {
				*index = i;
			}
			return true;
		}
	}
	return false;
}


//
//	EArgs::Tokenize()
//
void EArgs::Tokenize(const char *str)
{
	if (!str) Reset();

	if ( strlen(str)>=ARG_MAX_CHARS )
		RAISE_EXCEPTION(va("string exceeded %d chars", ARG_MAX_CHARS-1));

	Reset();
	strcpy(cmdline, str);
	char	*scan = &cmdline[0];
	bool	quotes = false;

	for (int i=0; i<ARG_MAX_ARGS; i++)
	{
		quotes = false;
		while (*scan==' ' || *scan=='\t' || *scan=='\r' || *scan=='\n') scan++;		// skip spaces
		if (*scan=='\"'){ quotes = true; scan++; }		// check & skip first quote
		if (*scan=='\0'){ break; }						// fix bug with last empty argument.
		argv[i] = &scan[0];
		argc = i+1;

		if (quotes)	{
			while (*scan!='\"' && *scan!='\0') scan++;
			if (*scan=='\0') break;
			*scan = '\0';
			scan++;
			continue;
		}
		if (!quotes) {
			while (*scan!=' ' && *scan!='\t' && *scan!='\r' && *scan!='\n' && *scan!='\0') scan++;
			if (*scan=='\0') break;
			*scan = '\0';
			scan++;
			continue;
		}
	}

 	if (quotes)	{
 		RAISE_EXCEPTION("unmatched quotes");
 	}
}


//
//	EArgs::NewArg
//
void EArgs::NewArg(void)
{
	PushChar('\0');
	
	argc++;
	cmdline[cursor] ='\0';
	argv[argc-1] = &cmdline[cursor];
	
	if (argc>=ARG_MAX_ARGS) {
		RAISE_EXCEPTION(va("NewArg() exceeded %d args", ARG_MAX_ARGS));
	}
}

//
//	EArgs::PushChar
//
void EArgs::PushChar(char ch)
{
	cmdline[cursor] = ch;
	cursor++;
	cmdline[cursor] = '\0';
	
	if (cursor >= ARG_MAX_CHARS-1) {
		RAISE_EXCEPTION(va("PushChar() exceeded %d chars", ARG_MAX_CHARS));
	}
}


//
//	EArgs::NewArg
//
void EArgs::NewArg(const char *str)
{
	NewArg();
	uint len = strlen(str);
	for (uint i=0; i<len; i++) {
		PushChar(str[i]);
	}
}