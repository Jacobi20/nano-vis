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
// i_core.h - 


#pragma once

/*-----------------------------------------------------------------------------
	CORE
-----------------------------------------------------------------------------*/

class	ILog;
class	ILinker;
class	ISystem;

class	IDisposable;
class		IConfig;
class		IShell;
class		IFileSystem;
class		INetworkSystem;
class		IRenderSystem;
class		ISoundSystem;
class		IInputSystem;
class		IGame;
class		IUserInterface;
class		IGeometryEngine;
class		IPhysEngine;
class		INanoVis;

#include "../core/hard_ref.hpp"
//#include "../core/weak_ref.hpp"

DLL_EXPORT	ILinker			*Linker			( void );
DLL_EXPORT	ISystem			*System			( void );
DLL_EXPORT	ILog			*Log			( void );

DLL_EXPORT	void			InitCoreSubsystems		( void );
DLL_EXPORT	void			ShutdownCoreSubsystems	( void );		


DLL_EXPORT	void		Fatal		( const char *message );
DLL_EXPORT	const char	*Version	( void );


DLL_EXPORT  IShell*				CreateShell         ( void );
DLL_EXPORT  IConfig*			CreateConfig        ( void );
DLL_EXPORT  IFileSystem*		CreateFileSystem    ( void );
DLL_EXPORT  INetworkSystem*		CreateNetworkSystem ( void );
DLL_EXPORT  IRenderSystem*		CreateRenderSystem  ( void );
DLL_EXPORT  ISoundSystem*		CreateSoundSystem   ( void );
DLL_EXPORT  IInputSystem*		CreateInputSystem   ( void );
DLL_EXPORT  IGame*				CreateGame          ( void );
DLL_EXPORT  IUserInterface*		CreateUserInterface ( void );
DLL_EXPORT	IGeometryEngine*	CreateGeometryEngine( void );	
DLL_EXPORT	IPhysEngine*		CreatePhysEngine	( void );
DLL_EXPORT	INanoVis*			CreateNanoVis		( void );


typedef hard_ref<IShell				>	IPxShell			;
typedef hard_ref<IConfig			>	IPxConfig			;
typedef hard_ref<IFileSystem		>	IPxFileSystem		;
typedef hard_ref<INetworkSystem		>	IPxNetworkSystem	;
typedef hard_ref<IRenderSystem		>	IPxRenderSystem		;
typedef hard_ref<ISoundSystem		>	IPxSoundSystem		;
typedef hard_ref<IInputSystem		>	IPxInputSystem		;
typedef hard_ref<IGame				>	IPxGame				;
typedef hard_ref<IUserInterface		>	IPxUserInterface	;
typedef hard_ref<IGeometryEngine	>	IPxGeometryEngine	;
typedef hard_ref<IPhysEngine		>	IPxPhysEngine		;
typedef hard_ref<INanoVis			>	IPxNanoVis			;


/*-----------------------------------------------------------------------------
	Linker :
-----------------------------------------------------------------------------*/

class ILinker { 
	public:
		virtual						~ILinker			( void ) {}
		
		virtual	IPxShell			GetShell			( void ) = 0;
		virtual	IPxConfig			GetConfig			( void ) = 0;
		virtual	IPxFileSystem		GetFileSystem		( void ) = 0;
		virtual IPxNetworkSystem	GetNetworkSystem	( void ) = 0;
		virtual	IPxRenderSystem 	GetRenderSystem		( void ) = 0;
		virtual	IPxSoundSystem		GetSoundSystem		( void ) = 0;
		virtual IPxInputSystem		GetInputSystem		( void ) = 0;
		virtual	IPxGame				GetGame				( void ) = 0;
		virtual IPxUserInterface 	GetUserInterface	( void ) = 0;
		virtual IPxGeometryEngine	GetGeometryEngine	( void ) = 0;
		virtual IPxPhysEngine		GetPhysEngine		( void ) = 0;
		virtual IPxNanoVis			GetNanoVis			( void ) = 0;
					
		virtual void				LinkShell			( IShell *shell ) = 0;
		virtual void				LinkConfig			( IConfig *cfg ) = 0;
		virtual void				LinkFileSystem		( IFileSystem *fs ) = 0;
		virtual void				LinkNetworkSystem	( INetworkSystem *ns ) = 0;
		virtual void				LinkRenderSystem	( IRenderSystem *rs ) = 0;
		virtual void				LinkSoundSystem		( ISoundSystem *ss ) = 0;
		virtual void				LinkInputSystem		( IInputSystem *is ) = 0;
		virtual void				LinkGame			( IGame	*game ) = 0;
		virtual void				LinkUserInterface	( IUserInterface *ui ) = 0;
		virtual void				LinkGeometryEngine	( IGeometryEngine *ge ) = 0;
		virtual void				LinkPhysEngine		( IPhysEngine *pe ) = 0;
		virtual void				LinkNanoVis			( INanoVis *nvis ) = 0;

		virtual void				LinkDLLShell			( const char *dllname ) = 0;
		virtual void				LinkDLLConfig			( const char *dllname ) = 0;
		virtual void				LinkDLLFileSystem		( const char *dllname ) = 0;
		virtual void				LinkDLLNetworkSystem	( const char *dllname ) = 0;
		virtual void				LinkDLLRenderSystem		( const char *dllname ) = 0;
		virtual void				LinkDLLSoundSystem		( const char *dllname ) = 0;
		virtual void				LinkDLLInputSystem		( const char *dllname ) = 0;
		virtual void				LinkDLLGame				( const char *dllname ) = 0;
		virtual void				LinkDLLUserInterface	( const char *dllname ) = 0;
		virtual void				LinkDLLGeometryEngine	( const char *dllname ) = 0;
		virtual void				LinkDLLPhysEngine		( const char *dllname ) = 0;
		virtual void				LinkDLLNanoVis			( const char *dllname ) = 0;
	};					


	
	
	
	