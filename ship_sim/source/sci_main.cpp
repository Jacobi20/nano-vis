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

IWaving	*create_waving(lua_State *L, int idx);

//
//	ESciVis::ESciVis
//
ESciVis::ESciVis( void )
{
	sci_vis	=	this;

	global_simulation_time	=	0;

	LOG_SPLIT("SciVis initialization");
	
	lua_State *L = CoreLua();
	
	rs()->InstallForward();
	
	waving	=	create_waving(NULL, 0);
	
	//	register ship API :
	ELuaShip::Register(L);
	RegisterAPI();
	
	CoreExecuteString("dofile('run.lua')");
	
	LOG_SPLIT("");
}


//
//	ESciVis::~ESciVis
//
ESciVis::~ESciVis( void )
{
	LOG_SPLIT("SciVis shutting down");
	
	waving	=	NULL;

	//	unregister ship API :
	lua_State *L = CoreLua();
	ELuaShip::Unregister(L);
	
	//	ShutdownPhysX();
	
	LOG_SPLIT("");
}


//
//	ESciVis::Frame
//
void ESciVis::Frame(uint dtime)
{
	dtime = 30;	//	~1/60

	uint w, h;
	rs()->GetScreenSize(w, h);
	float aspect = (float)w / (float)h;

	InputSystem()->SetInputMode( IN_KB_SCAN );
	InputSystem()->ProcessInput();
	
	//FramePhysX( dtime / 1000.0f );
	
	phys()->RunSimulation(dtime);

	if (rs()->BeginFrame()) {

		if (GetFRScene()) {
		
			CoreExecuteString( va("if sci_frame then sci_frame(%f); end;", dtime/1000.0f) );

		
			
			float zn = VIEW_NEAR;
			float zf = VIEW_FAR;		
			float tf = tanf(vmath::rad(view.fov/2));
			
			GetFRScene()->SetProjection( zn, zf, zf * tf * aspect, zf * tf );
			GetFRScene()->SetView( view.position, view.orient );
			
			view.frustum.SetFrustum( zf * tf * aspect, zf * tf, zn, zf );
			view.frustum.SetPosition( view.position );
			view.frustum.SetOrient( view.orient );
			
			
			GetFRScene()->SetDirectLight( EVec4(1,1,1,1), EVec4(0,0,0,1), 1, 1);
			
			rs()->GetDVScene()->SetView( view.position, view.orient );
			rs()->GetDVScene()->SetProjection( zn, zf, zf * tf * aspect, zf * tf );
			
			
			rs()->GetDVScene()->DrawArrow( EVec4(0,0,1,1), EVec4(1,0,0,0), 1.0f, EVec4(1,0,0,1 ) );
			rs()->GetDVScene()->DrawArrow( EVec4(0,0,1,1), EVec4(0,1,0,0), 1.0f, EVec4(0,1,0,1 ) );
			rs()->GetDVScene()->DrawArrow( EVec4(0,0,1,1), EVec4(0,0,1,0), 1.0f, EVec4(0,0,1,1 ) );
			
			waving->Update( dtime / 1000.0f, view.position, view.orient );
			
			phys()->DebugRender( rs()->GetDVScene() );
		
			rs()->RenderFrame();
		}
		
		rs()->EndFrame();
	}
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
