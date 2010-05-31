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

#include "../core/core.h"

/*-----------------------------------------------------------------------------
	Linker stuff :
-----------------------------------------------------------------------------*/

void LogCallBack(void *p) 
{
	uint msg_coloring[LOG_MSG_MAX];
	
	for (uint i=0; i< LOG_MSG_MAX; i++) {
		msg_coloring[ i ] = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	}
	
	msg_coloring[ LOG_MSG_INFO		]	=	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	msg_coloring[ LOG_MSG_WARNING	]	=	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
	msg_coloring[ LOG_MSG_ERROR		]	=	FOREGROUND_INTENSITY | FOREGROUND_RED;
	msg_coloring[ LOG_MSG_FATAL		]	=	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
	msg_coloring[ LOG_MSG_DEBUG		]	=	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
	msg_coloring[ LOG_MSG_DIMMED	]	=	FOREGROUND_INTENSITY;

	HANDLE hcon	=	GetStdHandle(STD_OUTPUT_HANDLE);


	ELogMessage_s	msg;
	Log()->GetLogLastMessage(msg);
	
	//	set appropriate color :
	SetConsoleTextAttribute(hcon, msg_coloring[ msg.msg_type ]);
	
	printf("%s\r\n", msg.message);

	//	restore color :
	SetConsoleTextAttribute(hcon, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


int main(int argc, const char **argv) 
{
	Log()->SetWriteCB( LogCallBack, NULL );

	InitCoreSubsystems();
	
	
	
	ShutdownCoreSubsystems();

	getch();
	
}