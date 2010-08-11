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


#include "sci_local.h"


DLL_EXPORT IGame	*CreateGame	( void ) {	return new ESciVis(); }

ESciVis	*sci_vis = NULL;
ESciVis *self_ref<ESciVis>::self = NULL;

/*-----------------------------------------------------------------------------
	Nano vis :
-----------------------------------------------------------------------------*/

//
//	ESciVis::ESciVis
//
ESciVis::ESciVis( void )
{
	sci_vis	=	this;

	global_simulation_time	=	0;

	LOG_SPLIT("SciVis initialization");
	
	lua_State *L = CoreLua();
	
	InitPhysX();
	
	//	register ship API :
	ELuaShip::Register(L);
	RegisterAPI();
	
	LOG_SPLIT("");
}


//
//	ESciVis::~ESciVis
//
ESciVis::~ESciVis( void )
{
	LOG_SPLIT("SciVis shutting down");

	//	unregister ship API :
	lua_State *L = CoreLua();
	ELuaShip::Unregister(L);

	ShutdownPhysX();
	
	LOG_SPLIT("");
}


//
//	ESciVis::Frame
//
void ESciVis::Frame(uint dtime)
{
	//	TODO :
}


//
//	ESciVis::AddShip
//
void ESciVis::AddShip( IPxShip ship )
{
	ships.push_back( ship );
}


//
//	ESciVis::RemoveShip
//
void ESciVis::RemoveShip( IPxShip ship )
{
	vector<IPxShip>::iterator	vi;
	vi = find( ships.begin(), ships.end(), ship );
	
	if (vi!=ships.end()) {
		ships.erase( vi );
	}
}
