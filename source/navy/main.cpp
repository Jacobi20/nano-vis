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

#include <Windows.h>
#include "../core/core.h"


/*-----------------------------------------------------------------------------
	Log callback :
-----------------------------------------------------------------------------*/

class ELogCB : public ILogCB {
	public:
		virtual void	Callback	( ELogMessage_s &message ) 
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

			ELogMessage_s	msg	=	message;
			
			//	set appropriate color :
			SetConsoleTextAttribute(hcon, msg_coloring[ msg.msg_type ]);
			
			printf("%s : %s\r\n", msg_prefix[msg.msg_type], msg.message);

			//	restore color :
			SetConsoleTextAttribute(hcon, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	};


/*-----------------------------------------------------------------------------
	Application :
-----------------------------------------------------------------------------*/

#include "..\scivis\sci_int.h"
ISciVis	*CreateSciVis	( void );

class EApp : public ICoreApp {
	public:
		virtual void	Init		( const EArgs &args );
		virtual void	Frame		( uint dtime );
		virtual void	Shutdown	( void );
	protected:
		hard_ref<ISciVis>	sci_vis;
	};



void EApp::Init( const EArgs &args )
{
	Config()->LoadConfig();

	sci_vis	=	CreateSciVis();	
	
	InputSystem()->SetTargetWindow( sci_vis->GetWindowDescriptor() );
}


void EApp::Frame( uint dtime )
{
	const char *command = va("if SciVisFrame then SciVisFrame(%g); end", 0.001*(float)dtime);
	
	sci_vis->RenderSnapshot( command );
	
	InputSystem()->SetInputMode(IN_KB_SCAN);
	InputSystem()->ProcessInput();
}


void EApp::Shutdown( void )
{
	sci_vis	=	NULL;
	
	Config()->SaveConfig();
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



/*-----------------------------------------------------------------------------
	main :
-----------------------------------------------------------------------------*/

//
//	main
//
int main(int argc, const char **argv)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(656);

	SetConsoleCtrlHandler( ConsoleHandleRoutine, TRUE );
	
	Log()->SetWriteCB( new ELogCB() );
	CoreRunApp( new EApp() );
}


