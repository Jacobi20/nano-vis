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
// win_system.h - windows system stuff

#pragma once



class	EWinDLL : public IDLL {
	public:
								EWinDLL			( const char *dll_name );
		virtual					~EWinDLL		( void );
		virtual	void*			GetProcAddr		( const char *func_name );
	protected:
		HMODULE	h_dll;
		string	name;
	};


class	EWinSystem : public ISystem
	{
	public:
								EWinSystem		( void );
		virtual					~EWinSystem		( void );
								
		virtual const char		*GetCmdLine		( void );
														
		virtual	uint			Milliseconds	( void );
		virtual	systemTime_t	GetGMTTime		( void );
		virtual	systemTime_t	GetLocalTime	( void );
		
		virtual	void			Quit			( uint code );
		virtual void			Terminate		( uint code );
		
		virtual IPxDLL			LoadDLL			( const char *dll_name );
		
		virtual thread_t		SpawnThread		( threadFunc_f func, void *param );
		virtual void			KillThread		( thread_t thread );
		virtual void			Sleep			( uint msec );
		
	protected:
		uint	init_msec;
	};