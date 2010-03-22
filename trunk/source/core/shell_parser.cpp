/*
	The MIT License

	Copyright (c) 2008 IFMO/GameDev Studio

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
// shell_parser.cpp - shell command pareser, refer "shell_parser_fsm.vsd"


#include "..\global.h"	
#include "shell.h"	


EShellParser::EShellParser(EShell *shell)
{
	this->shell	=	shell;
	Reset();
}


EShellParser::~EShellParser(void)
{
}


//
//	EShellParser::Reset
//	- resets parser
//
void EShellParser::Reset(void)
{	
	parsed_cmd.Reset();
}

/*-----------------------------------------------------------------------------
	Parsing stuff :
	
-----------------------------------------------------------------------------*/

//
//	EShellParser::ParseText
//	- parses text, throws exception if an error occurred
//
void EShellParser::ParseText(const char *text)
{
	Reset();

	//	read and preprocess text :	
	const uint BUF_SIZE = 32*1024;
	char buffer[BUF_SIZE];

	if (strlen(text) > BUF_SIZE) RAISE_EXCEPTION(va("text size exceded %d bytes", BUF_SIZE));
	strcpy(buffer, text);
	
	RemoveComments(buffer);

	const char *scan = &buffer[0];

	//	prepare arguments :	
	
	//	launch FSM :
	uint state	=	SHELL_FSM_BEGIN_COMMAND_PARSING;
	
	static	bool check_fsm_transitions = false;

	try {	
		while(state!=SHELL_FSM_END )
		{
			if (state==SHELL_FSM_END) break;

			if (check_fsm_transitions) {
				LOGF("[%2d] \"%c\" (0x%02X) -> ", state, *scan, *scan);
			}
			
			if (state==SHELL_FSM_BEGIN_COMMAND_PARSING)	{	state	=	TF_BeginCommandParsing	(*scan); } else
			if (state==SHELL_FSM_PARSE_COMMAND_NAME)	{	state	=	TF_ParseCommandName		(*scan); } else
			if (state==SHELL_FSM_BEGIN_ARGUMENT_PARSING){	state	=	TF_BeginArgumentParsing	(*scan); } else
			if (state==SHELL_FSM_PARSE_STRING)			{	state	=	TF_ParseString			(*scan); } else
			if (state==SHELL_FSM_PARSE_IDENT)			{	state	=	TF_ParseIdent			(*scan); } else
			if (state==SHELL_FSM_PARSE_ESCAPE_SEQUENCE)	{	state	=	TF_ParseEscapeSequence	(*scan); }

			if (check_fsm_transitions) {
				LOGF("[%2d]\r\n", state);
			}
			
			scan++;
		}
	} catch (exception &e)
	{
		uint line = GetTextLine(buffer, scan);
		string what = va("(%d) ", line);
		what += e.what();
		
		throw exception(what.c_str());
	}
}


uint EShellParser::GetTextLine(const char *text, const char *ptr)
{
	const char *begin = text;
	const char *scan = ptr;
	
	ASSERT(begin<=scan);
	
	if (begin==scan) {
		return 1;
	}
	
	scan--;	// move back to previous char
	
	uint line = 1;
	
	for ( ; scan!=begin; scan--) {
		if (*scan=='\n') line++;
	}
	
	return line;
}

void EShellParser::ExceptionWrongChar(char ch)
{
	if (isprint(ch)) {
		RAISE_EXCEPTION(va("Unexpected character 0x%02X '%c'", ch, ch));
	} else {
		RAISE_EXCEPTION(va("Unexpected character 0x%02X", ch));
	}
}


void EShellParser::NewCommand(void)
{
	parsed_cmd.Reset();	//	reset argument list
}


void EShellParser::NewArgument(void)
{
	parsed_cmd.NewArg();
}


void EShellParser::PushChar(char ch)
{
	parsed_cmd.PushChar(ch);
}


void EShellParser::ExecCommand(void)
{
	shell->CallCommand(parsed_cmd);
}

/*-----------------------------------------------------------------------------
	Shell parse FSM trasition functions
-----------------------------------------------------------------------------*/

uint EShellParser::TF_BeginCommandParsing(char ch)
{
	if (IsSpace(ch))		return SHELL_FSM_BEGIN_COMMAND_PARSING;
	if (IsDelimiter(ch))	return SHELL_FSM_BEGIN_COMMAND_PARSING;
	
	if (IsNull(ch)) return SHELL_FSM_END;
	
	if (IsIdentChar(ch)) {
		NewCommand();
		NewArgument();
		PushChar(ch);
		return SHELL_FSM_PARSE_COMMAND_NAME;
	}

	ExceptionWrongChar(ch);
	return SHELL_FSM_END;
}


uint EShellParser::TF_ParseCommandName(char ch)
{
	if (IsIdentChar(ch)) {
		PushChar(ch);
		return SHELL_FSM_PARSE_COMMAND_NAME;
	}
	
	if (IsDelimiter(ch)) {
		ExecCommand();
		return SHELL_FSM_BEGIN_COMMAND_PARSING;
	}
	
	if (IsSpace(ch)) {
		return SHELL_FSM_BEGIN_ARGUMENT_PARSING;
	}

	ExceptionWrongChar(ch);
	return SHELL_FSM_END;
}


uint EShellParser::TF_BeginArgumentParsing(char ch)
{
	if (IsSpace(ch)) {
		return SHELL_FSM_BEGIN_ARGUMENT_PARSING;
	}

	if (IsIdentChar(ch)) {
		NewArgument();
		PushChar(ch);
		return SHELL_FSM_PARSE_IDENT;
	}
	
	if (IsQuote(ch)) {
		NewArgument();
		return SHELL_FSM_PARSE_STRING;
	}
	
	if (IsDelimiter(ch)) {
		ExecCommand();
		return SHELL_FSM_BEGIN_COMMAND_PARSING;
	}
	
	ExceptionWrongChar(ch);
	return SHELL_FSM_END;
}


uint EShellParser::TF_ParseEscapeSequence(char ch)
{
	if (ch=='"') {
		PushChar(ch);
		return SHELL_FSM_PARSE_STRING;
	}
	if (ch=='\\') {
		PushChar(ch);
		return SHELL_FSM_PARSE_STRING;
	}
	
	RAISE_EXCEPTION(va("Unknown escape sequence \"\\%c\"", ch));
}


uint EShellParser::TF_ParseString(char ch)
{
	if (IsBackslash(ch)) {
		return SHELL_FSM_PARSE_ESCAPE_SEQUENCE;
	}

	if (IsQuote(ch)) {
		return SHELL_FSM_BEGIN_ARGUMENT_PARSING;
	}
	
	if (IsStringChar(ch)) {
		PushChar(ch);
		return SHELL_FSM_PARSE_STRING;
	}

	if (ch=='\r' || ch=='\n') {
		RAISE_EXCEPTION(va("new line in the string", ch));
	}
	
	ExceptionWrongChar(ch);
	return SHELL_FSM_END;
}


uint EShellParser::TF_ParseIdent(char ch)
{
	if (IsIdentChar(ch)) {
		PushChar(ch);
		return SHELL_FSM_PARSE_IDENT;
	}

	if (IsSpace(ch)) {
		return SHELL_FSM_BEGIN_ARGUMENT_PARSING;
	}

	if (IsDelimiter(ch)) {
		ExecCommand();
		return SHELL_FSM_BEGIN_COMMAND_PARSING;
	}

	ExceptionWrongChar(ch);
	return SHELL_FSM_END;
}



/*-----------------------------------------------------------------------------
	Character classification :
-----------------------------------------------------------------------------*/

bool EShellParser::IsSpace(char ch) const
{
	return (ch==' ' || ch=='\r' || ch=='\t') ;
}

bool EShellParser::IsDelimiter(char ch) const
{
	return (ch==';' || ch=='\n');
}

bool EShellParser::IsQuote(char ch) const
{
	return (ch=='\"');
}

bool EShellParser::IsNull(char ch) const
{
	return (ch=='\0');
}

bool EShellParser::IsIdentChar(char ch) const
{
	char chars[] = "+-.:_^$";
	// "+" is used for key_down commands
	// "-" is used for key_up commands
	// "." is used for floating point numbers
	// ":" can be used for name-spaces
	// "_" is commonly used character
	// "_" is used for text coloring
	// "$" is used for indirect adressing
	
	if ('a'<=ch && ch<='z') return true;
	if ('A'<=ch && ch<='Z') return true;
	if ('0'<=ch && ch<='9') return true;
	
	for (char *scan=chars; *scan; scan++) {
		if (ch==*scan) return true;
	}
	
	return false;
}

bool EShellParser::IsStringChar(char ch) const
{
	char chars[] = " \t`~!@#$%^&*()_+-=\\|[]{};':<>,./?";
	
	if (IsIdentChar(ch)) return true;

	for (char *scan=chars; *scan; scan++) {
		if (ch==*scan) return true;
	}
	
	return false;
}

bool EShellParser::IsBackslash(char ch) const
{
	return ( ch=='\\' );
}


string EShellParser::AddEscapeSequences(const char *str) const
{
	string result;
	uint len = (uint)strlen(str);
	
	for (uint i=0; i<len; i++) {
		if (str[i]=='\\' || str[i]=='"') {
			result.push_back('\\');
		}
	
		result.push_back(str[i]);
	}
	
	return result;
}


/*-----------------------------------------------------------------------------
	Comment removing
-----------------------------------------------------------------------------*/
//
//	EShellParser::RemoveComments
//	- removes comments in a given text buffer
//
void EShellParser::RemoveComments(char *text)
{
	char *read, *write;
	read = write = text;
	
	bool	comment	=	false;
	bool	string	=	false;
	
	while(1)	
	{
		if (*read=='\0') {
			*write = *read;
			break;
		}

		if (*read=='"') {
			string = !string;
		}
		
		if (!string && *read=='/' && read[1]=='/') {
			comment = true;
		}
		
		if (*read=='\n') { 
			string = false; 
			comment = false; 
		}
		
		if (!comment) {
			*write = *read;
			write++;		
		}
		
		read++;
	}
}


