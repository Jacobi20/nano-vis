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
// win_inputsys.h - mouse and keyboard stuff


#pragma once


struct	bind_t
	{
		const char	*name;
		string	bind_cmd;
		string	key_func;
		string	key_func_arg;
		bool	is_pressed;
	};


struct EInputState_s {
		EInputState_s	( void ) { target_hwnd = NULL; input_mode = 0; }
		~EInputState_s	( void ) {  }
		void			*target_hwnd;
		uint			input_mode;
		queue<char>		keys;
		queue<char>		chars;
		mouseInput_s	mouse;
		string			mouse_func;
	};
	

class	EWinInputSystem : public IInputSystem
	{
	public:
							EWinInputSystem		( void );
							~EWinInputSystem	( void );
		virtual void		SetTargetWindow		( void *hwnd );				
		
		virtual void		ProcessInput		( void );					
		virtual void		GetMouseInput		( mouseInput_s &mi );
		virtual char		GetKbChar			( void ) { return 0; }
		virtual char		GetKbKey			( void ) { return 0; }					
		
		virtual	void		SaveKeyBindings		( IPxFile file );
		virtual	uint		GetKeyByCmd			( const char *cmd );		
		virtual void		SetInputMode		( uint flags );				
	
	private:
		IPxFileSystem	fs;
		IPxShell		shell;
	
		void		InitKeyNames		( void );
		uint		GetKeyCode			( const char *name );
		
		void		ScanKeyboard		( bool enable_scan );
		void		HandleMouse			( bool show_cursor, bool clip, bool relative );
		bool		IsTargetWndActive	( void );

		bind_t	bindlist[256];
		
		EInputState_s	state;


		void			ProcessInput(void *wnd, bool enable_scan, bool relative_mmove, bool clip_cursor, bool show_cursor, mouseInput_s &mouse);
		
		static void		ListBinds_f		( EWinInputSystem *self, int argc, const char **argv);
		static void 	ListKeys_f		( EWinInputSystem *self, int argc, const char **argv);
		static void		Bind_f			( EWinInputSystem *self, int argc, const char **argv);
		static void		Unbind_f		( EWinInputSystem *self, int argc, const char **argv);
		static void		UnbindAll_f		( EWinInputSystem *self, int argc, const char **argv);
		
		static int		SetMouseFunc	( lua_State *L );
		static int		SetKeyFunc		( lua_State *L );
	};	