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
// win_inputsys.cpp - mouse and keyboard stuff


#include "../core/core.h"
#include "win_inputsys.h"
#include "win_key_list.h"


/*-----------------------------------------------------------------------------
	EXPORT Stuff
-----------------------------------------------------------------------------*/

DLL_EXPORT IInputSystem *CreateInputSystem(void)	{	return new EWinInputSystem();	}

/*-----------------------------------------------------------------------------
	Input stuff
-----------------------------------------------------------------------------*/

//
//	EWinInputSystem::EWinInputSystem
//
EWinInputSystem::EWinInputSystem(void)
{
	shell	=	Linker()->GetShell();
	fs		=	Linker()->GetFileSystem();

	LOG_INIT("Input system");
	
	
	for (int i=0; i<256; i++)
	{
		bindlist[i].name		=	OS_KEY_LIST[i];
		bindlist[i].is_pressed	=	false;
		bindlist[i].bind_cmd	=	"";
	}
	
	
	shell->AddCommand("bind",		(EShellCmd_f)Bind_f,		this);
	shell->AddCommand("unbind",		(EShellCmd_f)Unbind_f,		this);
	shell->AddCommand("unbindAll",	(EShellCmd_f)UnbindAll_f,	this);
	shell->AddCommand("listBinds",	(EShellCmd_f)ListBinds_f,	this);
	shell->AddCommand("listKeys",	(EShellCmd_f)ListKeys_f,	this);
	
	shell->DefineCFunc("SetMouseFunc",	SetMouseFunc);
}


//
//	EWinInputSystem::~EWinInputSystem
//
EWinInputSystem::~EWinInputSystem(void)
{
	LOG_SHUTDOWN("Input system");

	shell	=	NULL;
	fs		=	NULL;
	
	state.mouse_func	=	"";
	
	SetTargetWindow(NULL);

	ClipCursor( NULL );
	while( ShowCursor(TRUE) < 0 ) ;
}


//
//	EWinInputSystem::SetTargetWindow
//
void EWinInputSystem::SetTargetWindow(void *hwnd)
{
	state.target_hwnd	=	hwnd;
}


//
//	EWinInputSystem::SetInputMode
//
void EWinInputSystem::SetInputMode(uint flags)
{
	state.input_mode = flags;
}


//
//	EWinInputSystem::IsTargetWndActive
//
bool EWinInputSystem::IsTargetWndActive(void)
{
	HWND	hWnd	=	(HWND)state.target_hwnd;
	
	if (GetActiveWindow()==hWnd) {
		return true;
	} else {
		return false;
	}
}


//
//	EWinInputSystem::ProcessInput
//
void EWinInputSystem::ProcessInput(void)
{
	bool	enable_scan = false;
	bool	clip_cursor	= false;
	bool	relative	= false;
	bool	hide_cursor	= false;

	if (state.input_mode & IN_KB_SCAN		) {	enable_scan = true;		}	
	if (state.input_mode & IN_MOUSE_CLIP	) { clip_cursor	= true;		}
	if (state.input_mode & IN_MOUSE_HIDE	) { hide_cursor	= true;		}
	if (state.input_mode & IN_MOUSE_RELATIVE) { relative	= true;		}

	if (!IsTargetWndActive()) {
		enable_scan	= false;
		clip_cursor	= false;
		relative	= false;
		hide_cursor	= false;
	}
	
	ScanKeyboard(enable_scan);

	HandleMouse(hide_cursor, clip_cursor, relative);
}


//
//	EWinInputSystem::GetMouseInput
//
void EWinInputSystem::GetMouseInput(mouseInput_s &mi)
{
	mi = state.mouse;
}


//
//	EWinInputSystem::ScanKeyboard
//
void EWinInputSystem::ScanKeyboard( bool enable_scan )
{
	unsigned char keys[256];
	char	cmd[1024];
	GetKeyboardState(keys);
	
	for (int i=0; i<256; i++)
	{
		//	skip unassigned keys :
		if (!bindlist[i].name) continue;
		if (bindlist[i].bind_cmd.length()==0) continue;
		
		strcpy(cmd, bindlist[i].bind_cmd.c_str());
		
		if ((keys[i] & 0x80) && !bindlist[i].is_pressed && enable_scan)
		{
			shell->ExecuteString(va("%s;", cmd));
			bindlist[i].is_pressed = true;
		}
		
		if (  ((!(keys[i] & 0x80)) || !enable_scan) && bindlist[i].is_pressed)
		{
			bindlist[i].is_pressed = false;
			if (cmd[0]=='_') 
			{
				shell->ExecuteString(va("%s;", &cmd[1]));
			}
		}
	}
	
}


//
//	EWinInputSystem::HandleMouse
//
void EWinInputSystem::HandleMouse( bool hide_cursor, bool clip_cursor, bool relative )
{
	HWND	hWnd	=	(HWND)state.target_hwnd;

	if ( hide_cursor ) {
		while( ShowCursor(FALSE) >= 0 ) ;
	} else {
		while( ShowCursor(TRUE) < 0 ) ;
	}

	RECT	rect;
	WINDOWINFO	info;
	GetWindowRect(hWnd, &rect);
	GetWindowInfo(hWnd, &info);

	//	TODO DHG : Is following stuff handle all kind of windows???
	if (info.dwStyle & WS_POPUP) {
	} else {
		rect.left	+= ( GetSystemMetrics(SM_CXFIXEDFRAME) );
		rect.right	-= ( GetSystemMetrics(SM_CXFIXEDFRAME) );
		rect.top	+= ( GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYCAPTION) );
		rect.bottom -= ( GetSystemMetrics(SM_CYFIXEDFRAME) );
	}
	int x = rect.left;
	int y = rect.top;

	POINT	pnt;
	GetCursorPos(&pnt);
	int cx	= (rect.left + rect.right)/2;
	int cy	= (rect.top  + rect.bottom)/2;

	if (relative) {
		state.mouse.x		= cx - x;	// at the center of the window
		state.mouse.y		= cy - y;
		state.mouse.dx		= pnt.x - cx;	// 
		state.mouse.dy		= pnt.y - cy;
		if (!state.mouse_func.empty()) {
			shell->ExecuteString(va("%s ( %d, %d);", state.mouse_func.c_str(), state.mouse.dx, state.mouse.dy));
		}
	} else {
		state.mouse.x		= pnt.x - x;
		state.mouse.y		= pnt.y - y;
		state.mouse.dx		= 0;
		state.mouse.dy		= 0;
	}
	
	state.mouse.left	=	(0 != (GetKeyState(VK_LBUTTON) & 0x80));
	state.mouse.middle	=	(0 != (GetKeyState(VK_MBUTTON) & 0x80));
	state.mouse.right	=	(0 != (GetKeyState(VK_RBUTTON) & 0x80));

	if (relative) {
		SetCursorPos(cx, cy);
	}
	if (clip_cursor) {
		ClipCursor(&rect);
	} else {
		ClipCursor(NULL);
	}
}


//
//	EWinInputSystem::SaveKeyBindings
//
void EWinInputSystem::SaveKeyBindings( IPxFile file )
{
	file->Printf("\r\n-- key bindings --\r\n\r\n");
	file->Printf("unbindAll()\r\n");
	
	for (int i=0; i<256; i++)
	{
		if (bindlist[i].name) { 
			if (bindlist[i].bind_cmd.length()==0) {
				continue;
			}
			
			file->Printf("bind (\"%s\", \"%s\");\r\n", bindlist[i].name, bindlist[i].bind_cmd.c_str());
		}
	}
}


//
//	EWinInputSystem::GetKeyByCmd
//	
uint EWinInputSystem::GetKeyByCmd( const char *cmd )
{	
	for(int i = 0; i < 256; i++) {		
		if ( strcmpi(bindlist[i].bind_cmd.c_str(), cmd) ==0 ) {
			return i;
		}
	}
	return 0;
}


/*-----------------------------------------------------------------------------
	Stuff...
-----------------------------------------------------------------------------*/

//
//	EWinInputSystem::ListBinds_f
//
void EWinInputSystem::ListBinds_f(EWinInputSystem *self, int argc, const char **argv)
{
	uint num = 0;

	for (int i=0; i<256; i++)
	{
		if (self->bindlist[i].name) {
			if (self->bindlist[i].bind_cmd.length()==0) continue;
			
			LOGF("  %-12s = %s\r\n", self->bindlist[i].name, self->bindlist[i].bind_cmd.c_str());
			num++;
		}
	}
	LOGF("%d keys are bounded\r\n", num);
}


//
//	EWinInputSystem::ListKeys_f
//
void EWinInputSystem::ListKeys_f(EWinInputSystem *self, int argc, const char **argv)
{
	uint num = 0;
	
	LOGF("\r\n");
	for (int i=0; i<256; i++)
	{
		if (self->bindlist[i].name) {
			LOGF("  0x%02X : %s\r\n", i, self->bindlist[i].name);
		}
	}

	//for (int i=0; i<256; i++)
	//{
	//	if (self->bindlist[i].name) {
	//		LOGF("#define KEY_%-20s 0x%X\r\n", self->bindlist[i].name, i);
	//	}
	//}

	//LOGF("const char *KEY_LIST[256] = {\r\n");
	//for (int i=0; i<256; i++)
	//{
	//	if (i && i%16==0) LOGF("\r\n", i);

	//	LOGF("  /* 0x%02X */  ", i);
	//	
	//	if (self->bindlist[i].name) {
	//		LOGF("\"%s\"", self->bindlist[i].name);
	//	} else {
	//		LOGF("NULL", self->bindlist[i].name);
	//	}

	//	LOGF(",\r\n");
	//}
	//LOGF("};\r\n");
}


//
//	EWinInputSystem::Bind_f
//
void EWinInputSystem::Bind_f(EWinInputSystem *self, int argc, const char **argv)
{
	if (argc!=3) {
		LOGF("usage : bind <keyname> <cmd>\r\n");
		return;
	}
	
	uint key	=	self->GetKeyCode(argv[1]);
	if (key==0) RAISE_EXCEPTION(va("unknown key \"%s\"", argv[1]));
	
	self->bindlist[key].bind_cmd	=	argv[2];
}


//
//	EWinInputSystem::Unbind_f
//
void EWinInputSystem::Unbind_f(EWinInputSystem *self, int argc, const char **argv)
{
	if (argc!=2) {
		LOGF("usage : unbind <keyname>\r\n");
		return;
	}
	
	uint key	=	self->GetKeyCode(argv[1]);
	if (key==0) RAISE_EXCEPTION(va("unknown key \"%s\"", argv[1]));
	
	self->bindlist[key].bind_cmd	=	"";
}


//
//	EWinInputSystem::UnbindAll_f
//
void EWinInputSystem::UnbindAll_f(EWinInputSystem *self, int argc, const char **argv)
{
	for (uint i=0; i<256; i++) self->bindlist[i].bind_cmd = "";
}


//
//	EWinInputSystem::GetKeyCode
//
uint EWinInputSystem::GetKeyCode(const char *name)
{
	for (uint i=0; i<256; i++)
	{
		if ( bindlist[i].name ) {
			if ( strcmpi(bindlist[i].name, name)==0) return i;
		}
	}
	
	return 0;
}


//
//	EWinInputSystem::SetMouseFunc
//
int EWinInputSystem::SetMouseFunc( lua_State *L )
{
	int n = lua_gettop(L);
	
	EWinInputSystem	*self = Linker()->GetInputSystem().To<EWinInputSystem>();
	
	if (n!=1) {
		LOGF("usage : SetMouseFunc(<string: function name>) -- set handler");
		LOGF("        SetMouseFunc(nil)                     -- reset handler");
		return 0;
	}

	if (lua_isnil(L, 1)) {
		self->state.mouse_func	=	"";
	}
	
	if (lua_isstring(L, 1)) {
		self->state.mouse_func = lua_tostring(L, 1);
	}
	
	return 0;
}


//
//	EWinInputSystem::SetKeyFunc
//
int EWinInputSystem::SetKeyFunc( lua_State *L )
{
	return 0;
}










