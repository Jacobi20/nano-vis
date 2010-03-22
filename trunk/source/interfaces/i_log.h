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
// i_log.h - 

#pragma once


/*-----------------------------------------------------------------------------
	Log 
-----------------------------------------------------------------------------*/

const uint LOG_MAX_LINES	=	4096;
const uint LOG_LINE_LEN		=	256;

typedef void (*ELogWriteCB_f)(void *self);


enum ELogMsgType_t {
		LOG_MSG_INFO = 0,		//	simple message
		LOG_MSG_WARNING,		//	something went wrong, but system have been recovered
		LOG_MSG_ERROR,			//	something went wrong, but system still alive 
		LOG_MSG_FATAL,			//	something went wrong and system will down shortly
		LOG_MSG_DEBUG,			//	debug message
		LOG_MSG_ACTION,			//	(?) message about some process (loading, compiling, etc) which still not marked
		LOG_MSG_ACTION_OK,		//	(?) message about some process marked with OK
		LOG_MSG_ACTION_FAILED,	//	(?) message about some process marked with FAILED
		LOG_MSG_DIMMED,			//	some dimmed messages (like script output)
		LOG_MSG_MAX,
	};


struct ELogMessage_s {
		ELogMsgType_t	msg_type;
		char			message[LOG_LINE_LEN];
		const char	*	src_file;
		uint			src_line;
	};


class	ILog {
	public:
		virtual	uint	LogMessage			( ELogMsgType_t msg_type, const char *src_file, uint src_line, const char *frmt, ... ) = 0;
		virtual void	SetWriteCB			( ELogWriteCB_f write_cb, void *self ) = 0;
		virtual void	GetLogMessage		( uint line, ELogMessage_s &message ) const = 0;
		virtual void	GetLogLastMessage	( ELogMessage_s &message ) const = 0;
		virtual uint	GetLogMessageNum	( void ) const = 0;
		virtual void	Clear				( void ) = 0;
		virtual void	LogSplit			( const char *headline, char fill_char='-' ) = 0;
	};
	
