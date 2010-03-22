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


#include "core_local.h"

#include <crtdbg.h>




struct ELogLine_s {
		ELogMsgType_t	msg_type;
		char			message[LOG_LINE_LEN];
		const char	*	src_file;
		uint			src_line;
		uint			result;
	};


class ELog : public ILog {
	public:
						ELog			( void );
						
		virtual	uint	LogMessage			( ELogMsgType_t msg_type, const char *src_file, uint src_line, const char *frmt, ... );
		
		virtual void	SetWriteCB			( ELogWriteCB_f write_cb, void *self );
		virtual void	GetLogMessage		( uint line, ELogMessage_s &message ) const;
		virtual void	GetLogLastMessage	( ELogMessage_s &message ) const;
		virtual uint	GetLogMessageNum	( void ) const;
		virtual void	Clear				( void );
		virtual void	LogSplit			( const char *headline, char fill_char );
		
	protected:
		ELogWriteCB_f	write_cb;
		void			*write_cb_self;
		
		ELogLine_s	log_lines[LOG_MAX_LINES];
		uint		current_line;
		uint		total_lines;
	};


/*-----------------------------------------------------------------------------
	Log export stuff :
-----------------------------------------------------------------------------*/	

ELog log_static_imp;
	
ILog *Log(void) 
{
	return &log_static_imp;
}


/*-----------------------------------------------------------------------------
	Log implementation :
-----------------------------------------------------------------------------*/	

//
//	DecolorizeString
//	TODO : move to 'library'.
//
void DecolorizeString(const char *input, char *output) 
{
	for ( ; *input; *input++, *output++ ) {
		if (*input=='^') {
			input++;
			if (*input>='0' && *input<='9') {
				input++;
			}
		}
		*output = *input;
	}
	*output = '\0';
}


//
//	ELog::ELog
//
ELog::ELog(void)
{
	SetWriteCB(NULL, NULL);
	Clear();
}


//
//	ELog::Clear
//
void ELog::Clear(void) 
{
	memset(log_lines,	0, sizeof(log_lines));
	current_line = 0;
	total_lines = 0;
}


//
//	ELog::WriteLine
//	
uint ELog::LogMessage( ELogMsgType_t msg_type, const char *src_file, uint src_line, const char *frmt, ... )
{
	uint msg_id = 0;

	char message[512];
	va_list	argptr;
	va_start (argptr, frmt);
	vsnprintf (message, 512-1, frmt, argptr);
	va_end (argptr);
	
	uint len = strlen(message);
	for (uint i=0; i<len; i++) {	
		switch (message[i]) {
			case '\r' :
			case '\n' :
				message[i] = '\0';
		}
	}
	

	memset( log_lines[ current_line ].message, 0, LOG_LINE_LEN );
	memcpy( log_lines[ current_line ].message, message, min(strlen(message), LOG_LINE_LEN-1) );
	log_lines[ current_line ].msg_type	=	msg_type;
	log_lines[ current_line ].src_file	=	src_file;
	log_lines[ current_line ].src_line	=	src_line;
	
	msg_id = current_line;
	
	current_line++;
	current_line %= LOG_MAX_LINES;
	total_lines++;
	
	if (write_cb) {
		write_cb( write_cb_self );
	}
	
	#ifdef _DEBUG
		systemTime_t t = System()->GetLocalTime();
		uint h  = t.hour;
		uint m  = t.minute;
		uint s  = t.sec;
		uint ms = t.msec;
		_CrtDbgReport(_CRT_WARN, 0, 0, NULL, va("[%02d:%02d:%02d.%03d] %s\r\n", h, m, s, ms, message));
	#endif

	return msg_id;
}


//
//	ELog::SetWriteCB
//
void ELog::SetWriteCB(ELogWriteCB_f write_cb, void *self)
{
	this->write_cb		= write_cb;
	this->write_cb_self	= self;
}


//
//	ELog::GetLine
//
void ELog::GetLogMessage( uint line, ELogMessage_s &message ) const
{
	if (line>=GetLogMessageNum()) {
		strcpy(message.message, "");
		message.src_file	=	"";
		message.src_line	=	0;
		message.msg_type	=	LOG_MSG_INFO;
		return;
	}

	uint n = (total_lines - line - 1) % LOG_MAX_LINES;

	memcpy( &message, &log_lines[ n ], sizeof(ELogMessage_s) );
}


void ELog::GetLogLastMessage( ELogMessage_s &message ) const
{
	memcpy( &message, &log_lines[ current_line-1 ], sizeof(ELogMessage_s) );
}


//
//	ELog::GetLineNum
//
uint ELog::GetLogMessageNum( void ) const
{
	return min(total_lines, LOG_MAX_LINES);
}



//
//	ELog::LogSplit
//
void ELog::LogSplit( const char *headline, char fill_char )
{
	if (strlen(headline)>0) {
		LogMessage(LOG_MSG_INFO, "", 0, "");
	}

	const int len = 71;
	char split[len];
	memset(split, fill_char, len);
	split[len-3] = '\r';
	split[len-2] = '\n';
	split[len-1] = '\0';

	char insert[len] = "";
	if (strlen(headline)!=0) {
		sprintf(insert, " %s ", headline);
	}
	
	memcpy(split+4, insert, strlen(insert));
	
	LogMessage(LOG_MSG_INFO, "", 0, "%s", split);;

	if (strlen(headline)==0) {
		LogMessage(LOG_MSG_INFO, "", 0, "");
	}
}