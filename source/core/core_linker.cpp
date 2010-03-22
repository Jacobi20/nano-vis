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


/*-----------------------------------------------------------------------------
	Export stuff :
-----------------------------------------------------------------------------*/

ELinker linker_imp_static;

ILinker *Linker(void) 
{
	return &linker_imp_static;
}


/*-----------------------------------------------------------------------------
	Linker main stuff :
-----------------------------------------------------------------------------*/

ELinker::ELinker(void)
{
}


ELinker::~ELinker(void)
{
}



/*-----------------------------------------------------------------------------
	Subsystem linking stuff :
-----------------------------------------------------------------------------*/
//	{Linker\(\)\-\>:i}:b*\(:b*\&{:i}:b*\)	- pattern to replace functions

void	ELinker::LinkShell			( IShell *shell )		{	api.shell	=	shell;		api.dll_shell	=	NULL;	}
void	ELinker::LinkConfig			( IConfig *cfg )		{	api.cfg		=	cfg;		api.dll_cfg		=	NULL;	}
void	ELinker::LinkFileSystem		( IFileSystem *fs )		{	api.fs		=	fs;			api.dll_fs		=	NULL;	}
void	ELinker::LinkNetworkSystem	( INetworkSystem *ns )	{	}
void	ELinker::LinkRenderSystem	( IRenderSystem *rs )	{	api.rs		=	rs;			api.dll_rs		=	NULL;	}
void	ELinker::LinkSoundSystem	( ISoundSystem *ss )	{	api.ss		=	ss;			api.dll_ss		=	NULL;	}
void	ELinker::LinkInputSystem	( IInputSystem *is )	{	api.is		=	is;			api.dll_is		=	NULL;	}
void	ELinker::LinkGame			( IGame *game )			{	api.game	=	game;		api.dll_game	=	NULL;	}
void	ELinker::LinkUserInterface	( IUserInterface *ui )	{	api.ui		=	ui;			api.dll_ui		=	NULL;	}
void	ELinker::LinkGeometryEngine	( IGeometryEngine *ge )	{	api.ge		=	ge;			api.dll_ge		=	NULL;	}
void	ELinker::LinkPhysEngine		( IPhysEngine *pe )		{	api.pe		=	pe;			api.dll_pe		=	NULL;	}
void	ELinker::LinkNanoVis		( INanoVis *nvis )		{	api.nvis	=	nvis;		api.dll_nvis	=	NULL;	}


template <typename Interface>	
hard_ref<Interface>	Link(IPxDLL &dll, const char *dllname, const char *funcname) 
{
	typedef Interface *(*create_f)(void);
	dll = System()->LoadDLL(dllname);
	create_f	create	=	(create_f)dll->GetProcAddr(funcname);
	return create();
}


void	ELinker::LinkDLLShell			( const char *dllname )	{	api.shell	=	Link<IShell			>(api.dll_shell	,	dllname, "CreateShell"			);	}
void	ELinker::LinkDLLConfig			( const char *dllname )	{	api.cfg		=	Link<IConfig		>(api.dll_cfg	,	dllname, "CreateConfig"			);	}
void	ELinker::LinkDLLFileSystem		( const char *dllname )	{	api.fs		=	Link<IFileSystem	>(api.dll_fs	,	dllname, "CreateFileSystem"		);	}
void	ELinker::LinkDLLNetworkSystem	( const char *dllname )	{	}															
void	ELinker::LinkDLLRenderSystem	( const char *dllname )	{	api.rs		=	Link<IRenderSystem	>(api.dll_rs	,	dllname, "CreateRenderSystem"	);	}
void	ELinker::LinkDLLSoundSystem		( const char *dllname )	{	api.ss		=	Link<ISoundSystem	>(api.dll_ss	,	dllname, "CreateSoundSystem"	);	}
void	ELinker::LinkDLLInputSystem		( const char *dllname )	{	api.is		=	Link<IInputSystem	>(api.dll_is	,	dllname, "CreateInputSystem"	);	}
void	ELinker::LinkDLLGame			( const char *dllname )	{	api.game	=	Link<IGame			>(api.dll_game	,	dllname, "CreateGame"			);	}
void	ELinker::LinkDLLUserInterface	( const char *dllname )	{	api.ui		=	Link<IUserInterface	>(api.dll_ui	,	dllname, "CreateUserInterface"	);	}
void	ELinker::LinkDLLGeometryEngine	( const char *dllname )	{	api.ge		=	Link<IGeometryEngine>(api.dll_ge	,	dllname, "CreateGeometryEngine"	);	}
void	ELinker::LinkDLLPhysEngine		( const char *dllname )	{	api.pe		=	Link<IPhysEngine	>(api.dll_pe	,	dllname, "CreatePhysEngine"		);	}
void	ELinker::LinkDLLNanoVis			( const char *dllname )	{	api.nvis	=	Link<INanoVis		>(api.dll_nvis	,	dllname, "CreateNanoVis"		);	}



IPxShell			ELinker::GetShell			( void )	{	return	api.shell;	}
IPxConfig			ELinker::GetConfig			( void )	{	return	api.cfg;	}
IPxFileSystem		ELinker::GetFileSystem		( void )	{	return	api.fs;		}
IPxNetworkSystem	ELinker::GetNetworkSystem	( void )	{	return	0;			}
IPxRenderSystem 	ELinker::GetRenderSystem	( void )	{	return	api.rs;		}
IPxSoundSystem		ELinker::GetSoundSystem		( void )	{	return	api.ss;		}
IPxInputSystem		ELinker::GetInputSystem		( void )	{	return	api.is;		}
IPxGame				ELinker::GetGame			( void )	{	return	api.game;	}
IPxUserInterface 	ELinker::GetUserInterface	( void )	{	return	api.ui;		}
IPxGeometryEngine	ELinker::GetGeometryEngine	( void )	{	return	api.ge;		}
IPxPhysEngine		ELinker::GetPhysEngine		( void )	{	return	api.pe;		}
IPxNanoVis			ELinker::GetNanoVis			( void )	{	return	api.nvis;	}


