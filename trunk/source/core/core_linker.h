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


#pragma once


/*-----------------------------------------------------------------------------
	Linker stuff :
-----------------------------------------------------------------------------*/

struct EApi_t {
		IPxDLL	dll_shell;	IPxShell			shell;	
		IPxDLL	dll_cfg;	IPxConfig			cfg;	
		IPxDLL	dll_fs;		IPxFileSystem		fs;		
		IPxDLL	dll_ns;		IPxNetworkSystem	ns;		
		IPxDLL	dll_rs;		IPxRenderSystem		rs;		
		IPxDLL	dll_ss;		IPxSoundSystem		ss;		
		IPxDLL	dll_is;		IPxInputSystem		is;		
		IPxDLL	dll_game;	IPxGame				game;	
		IPxDLL	dll_ui;		IPxUserInterface	ui;		
		IPxDLL	dll_ge;		IPxGeometryEngine	ge;		
		IPxDLL	dll_pe;		IPxPhysEngine		pe;
		IPxDLL	dll_nvis;	IPxNanoVis			nvis;
	};


class ELinker : public ILinker { 
	public:
									ELinker				( void );
		virtual						~ELinker			( void );

		virtual	IPxShell			GetShell			( void );
		virtual	IPxConfig			GetConfig			( void );
		virtual	IPxFileSystem		GetFileSystem		( void );
		virtual IPxNetworkSystem	GetNetworkSystem	( void );
		virtual	IPxRenderSystem 	GetRenderSystem		( void );
		virtual	IPxSoundSystem		GetSoundSystem		( void );
		virtual IPxInputSystem		GetInputSystem		( void );
		virtual	IPxGame				GetGame				( void );
		virtual IPxUserInterface 	GetUserInterface	( void );
		virtual IPxGeometryEngine	GetGeometryEngine	( void );
		virtual IPxPhysEngine		GetPhysEngine		( void );
		virtual IPxNanoVis			GetNanoVis			( void );
					
		virtual void				LinkShell			( IShell *shell );
		virtual void				LinkConfig			( IConfig *cfg );
		virtual void				LinkFileSystem		( IFileSystem *fs );
		virtual void				LinkNetworkSystem	( INetworkSystem *ns );
		virtual void				LinkRenderSystem	( IRenderSystem *rs );
		virtual void				LinkSoundSystem		( ISoundSystem *ss );
		virtual void				LinkInputSystem		( IInputSystem *is );
		virtual void				LinkGame			( IGame	*game );
		virtual void				LinkUserInterface	( IUserInterface *ui );
		virtual void				LinkGeometryEngine	( IGeometryEngine *ge );
		virtual void				LinkPhysEngine		( IPhysEngine *pe );
		virtual void				LinkNanoVis			( INanoVis *nvis );

		virtual void				LinkDLLShell			( const char *dllname );
		virtual void				LinkDLLConfig			( const char *dllname );
		virtual void				LinkDLLFileSystem		( const char *dllname );
		virtual void				LinkDLLNetworkSystem	( const char *dllname );
		virtual void				LinkDLLRenderSystem		( const char *dllname );
		virtual void				LinkDLLSoundSystem		( const char *dllname );
		virtual void				LinkDLLInputSystem		( const char *dllname );
		virtual void				LinkDLLGame				( const char *dllname );
		virtual void				LinkDLLUserInterface	( const char *dllname );
		virtual void				LinkDLLGeometryEngine	( const char *dllname );
		virtual void				LinkDLLPhysEngine		( const char *dllname );
		virtual void				LinkDLLNanoVis			( const char *dllname );
	
	protected:
		EApi_t	api;
	};
	

