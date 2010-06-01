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
	Main :
-----------------------------------------------------------------------------*/

void LogCallBack(void *p) 
{
	uint msg_coloring[LOG_MSG_MAX];
	char *msg_prefix[LOG_MSG_MAX];
	
	for (uint i=0; i< LOG_MSG_MAX; i++) {
		msg_coloring[ i ]	= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		msg_prefix[ i ]		= "";
	}
	
	msg_coloring[ LOG_MSG_INFO		]	=	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	msg_coloring[ LOG_MSG_WARNING	]	=	FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
	msg_coloring[ LOG_MSG_ERROR		]	=	FOREGROUND_INTENSITY | FOREGROUND_RED;
	msg_coloring[ LOG_MSG_FATAL		]	=	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
	msg_coloring[ LOG_MSG_DEBUG		]	=	FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
	msg_coloring[ LOG_MSG_DIMMED	]	=	FOREGROUND_INTENSITY;

	msg_prefix[ LOG_MSG_INFO		]	=	"       ";
	msg_prefix[ LOG_MSG_WARNING		]	=	"WARNING";
	msg_prefix[ LOG_MSG_ERROR		]	=	"ERROR  ";
	msg_prefix[ LOG_MSG_FATAL		]	=	"FATAL  ";
	msg_prefix[ LOG_MSG_DEBUG		]	=	"DEBUG  ";
	msg_prefix[ LOG_MSG_DIMMED		]	=	"LUA    ";

	HANDLE hcon	=	GetStdHandle(STD_OUTPUT_HANDLE);


	ELogMessage_s	msg;
	Log()->GetLogLastMessage(msg);
	
	//	set appropriate color :
	SetConsoleTextAttribute(hcon, msg_coloring[ msg.msg_type ]);
	
	printf("%s : %s\r\n", msg_prefix[msg.msg_type], msg.message);

	//	restore color :
	SetConsoleTextAttribute(hcon, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}



static bool can_quit = false;

int quit(lua_State *L) {
	can_quit	=	true;
	return 0;
}


//
//	NVisInit
//
DLL_EXPORT void NavyInit( void )
{
	Log()->SetWriteCB(LogCallBack, NULL);

	InitCoreSubsystems();
	
	Linker()->GetConfig()->LoadConfig();
	
	lua_State *L = Linker()->GetShell()->Lua();
	
	lua_register(L, "quit", quit);
	
	Linker()->LinkDLLGeometryEngine("geometry_engine.dll");
	Linker()->LinkDLLSciVis("scivis.dll");
	
	Linker()->GetInputSystem()->SetTargetWindow( Linker()->GetSciVis()->GetWindowDescriptor() );
}


//
//	NVisShutdown
//
DLL_EXPORT void NavyShutdown( void )
{
	Linker()->LinkSciVis(NULL);
	Linker()->LinkGeometryEngine(NULL);
	
	Linker()->GetConfig()->SaveConfig();

	ShutdownCoreSubsystems();
}


//
//	NVisFrame
//
DLL_EXPORT void NavyFrame( uint dtime )
{
	IPxSciVis	nvis	=	Linker()->GetSciVis();
	
	const char *command = va("if SciVisFrame then SciVisFrame(%g); end", 0.001*(float)dtime);
	
	nvis->RenderSnapshot( command );
	
	Linker()->GetInputSystem()->SetInputMode(IN_KB_SCAN);
	Linker()->GetInputSystem()->ProcessInput();
}


//
//	NVisCommand
//
DLL_EXPORT void NVisCommand( const char *cmd )
{
	LOGF("command : %s", cmd);

	IPxSciVis	nvis	=	Linker()->GetSciVis();
	nvis->RenderSnapshot( cmd );
}										 


BOOL WINAPI ConsoleHandleRoutine(DWORD dwCtrlType)
{
	if (dwCtrlType==CTRL_CLOSE_EVENT) {
		printf("\r\n");
		printf("**** IMMEDIATE TERMINATION ****\r\n");
		printf("\r\n");
		
		TerminateProcess(0, 0);
		return TRUE;
	}
	
	return FALSE;
}



//
//	main
//
int main(int argc, const char **argv)
{
	uint	old_time	=	System()->Milliseconds();
	uint	time		=	System()->Milliseconds();
	MSG		msg = {0};

	SetConsoleCtrlHandler( ConsoleHandleRoutine, TRUE );

	try {
							  
		NavyInit();

		while ( WM_QUIT != msg.message )
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )		
			{			
				TranslateMessage( &msg );		
				DispatchMessage( &msg );				
			}		
			else			
			{
				time = System()->Milliseconds();
				
				NavyFrame(time - old_time);

				if (can_quit) {
					break;
				}
				
				old_time = time;
			}

		}

		NavyShutdown();
		
	} catch (exception &e) {
		FATAL(e.what());
	}
	
}


