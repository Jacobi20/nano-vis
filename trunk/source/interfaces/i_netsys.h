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
// i_netsys.h - 


#pragma once

const uint SV_MAXCLIENTS	=	256;


class	ITcpListener;
class	ITcpClient;
class	IUdpClient;
class	INetworkSystem;


typedef hard_ref<ITcpListener	>	IPxTcpListener		;
typedef hard_ref<ITcpClient		>	IPxTcpClient		;
typedef hard_ref<IUdpClient		>	IPxUdpClient		;
typedef hard_ref<INetworkSystem	>	IPxNetworkSystem	;



class	INetworkSystem : public IDisposable {
	public:
		virtual	IPxTcpListener	CreateTCPListener	( uint port ) = 0;
		virtual	IPxTcpClient	CreateTCPClient		( void ) = 0;
		virtual	IPxUdpClient	CreateUPDClient		( void ) = 0;
	};


class	ITcpListener : public IDisposable {
	public:
		virtual	IPxTcpClient	Accept		( void ) = 0;
	};


class	ITcpClient : public IDisposable {
	public:
		virtual	void			Connect			( const char *address, uint port ) = 0;
		virtual	uint			Send			( const void *data, uint size ) = 0;
		virtual	uint			Recv			( void *data, uint size ) = 0;
		virtual bool			IsConnected		( void ) const = 0;
		virtual void			SetBlockMode	( bool enable ) = 0;
	};
	

class	IUdpClient : public IDisposable {
	public:
		virtual	void			SetEndPoint		( const char *address, uint port ) = 0;
		virtual	uint			Send			( const void *data, uint size ) = 0;
		virtual	uint			Recv			( void *data, uint size ) = 0;
		virtual void			SetBlockMode	( bool enable ) = 0;
	};
	
	