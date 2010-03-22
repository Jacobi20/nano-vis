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
// i_system.h - 
//NOTE must be removed


struct	systemTime_t {
		word	year,	month,	day_of_week,	day;
		word	hour,	minute,	sec,	msec;
	};

#define SYS_CALL	__stdcall

typedef	uint	thread_t;
typedef uint	(SYS_CALL* threadFunc_f)	( void *param );

class IDLL : public IDisposable { 
	public:
		virtual					~IDLL			( void ) {};
		virtual	void*			GetProcAddr		( const char *func_name ) = 0;
	};


typedef hard_ref<IDLL>	IPxDLL;


class ISystem { 
	public:
		virtual					~ISystem		( void ) {};

		virtual const char		*GetCmdLine		( void ) = 0;			//	program arguments
																	
		virtual	uint			Milliseconds	( void ) = 0;			//	retrieves the time elapsed since game started
		virtual	systemTime_t	GetGMTTime		( void ) = 0;			//	retrievse the GMT system time
		virtual	systemTime_t	GetLocalTime	( void ) = 0;			//	retrievse the local system time
		
		virtual	void			Quit			( uint code ) = 0;		//	normal quit
		virtual void			Terminate		( uint code ) = 0;		//	immediate quit
		
		virtual IPxDLL			LoadDLL			( const char *dll_name ) = 0;
		
		virtual thread_t		SpawnThread		( threadFunc_f func, void *param ) = 0;
		virtual void			KillThread		( thread_t thread ) = 0;
		virtual void			Sleep			( uint msec ) = 0;
	};
	
