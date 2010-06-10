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

/*-----------------------------------------------------------------------------
	Naive ship simulator :
-----------------------------------------------------------------------------*/

const float	SHIP_LENGTH		=	105;		//	m
const float	SHIP_WIDTH		=	6;			//	m
const float	SHIP_HEIGHT		=	7;			//	m
const float	SHIP_MASS		=	2000000;	//	kg
const float SHIP_CM_OFFSET	=  -0.7f;
const float WATER_DENSITY	=	1000;		//	kg/m^3
const float SHIP_CX			=	0.2f;



class EShipNaive : public IShip {
	public:
						EShipNaive	( lua_State *L, int idx );
						~EShipNaive	( void );
					
		virtual void	Simulate	( float dtime, IPxWaving waving );
		virtual void	GetPose		( EVec4 &position, EVec4 &orient );
		virtual void	Render		( ERendEnv_s *rend_env );
		
	protected:
		IPxTriMesh		mesh_vis;
		IPxTriMesh		mesh_flow;
		ID3DXMesh		*d3d_mesh_vis;
		ID3DXEffect		*shader_fx;
		
		NxActor			*ship_body;
	};
	

IShip * create_naive_ship( lua_State *L, int idx ) {
	return new EShipNaive(L, idx);
}	

/*-----------------------------------------------------------------------------
	Naive ship simulator :
-----------------------------------------------------------------------------*/

EShipNaive::EShipNaive( lua_State *L, int idx )
{
	mesh_vis		=	sci_vis->LoadMesh("../scidata/uboat.esx", "|boat1");
	mesh_flow		=	sci_vis->LoadMesh("../scidata/uboat.esx", "|flowsurf");
	d3d_mesh_vis	=	sci_vis->CreateMesh( mesh_vis );
	shader_fx		=	sci_vis->CompileEffect( "../scidata/shader.fx" );

	EQuat		q	=	QuatRotationAxis( deg2rad(20), EVec3(1,1,1));
	EVec4		p	=	EVec4(0,0,20,1);
	ship_body		=	sci_vis->CreatePhysBox( SHIP_LENGTH, SHIP_WIDTH, SHIP_HEIGHT, p, q, SHIP_MASS);
}


EShipNaive::~EShipNaive( void )
{
	sci_vis->GetNxScene()->releaseActor( *ship_body );
	SAFE_RELEASE( d3d_mesh_vis );
	SAFE_RELEASE( shader_fx );
}


void EShipNaive::Simulate( float dtime, IPxWaving waving )
{

}


void EShipNaive::GetPose( EVec4 &position, EVec4 &orient )
{

}

void EShipNaive::Render( ERendEnv_s *rend_env )
{
	NxQuat	nxq	=	ship_body->getGlobalOrientationQuat();
	NxVec3	nxv	=	ship_body->getGlobalPosition();
	EQuat	q	=	EQuat(nxq.x, nxq.y, nxq.z, nxq.w);
	EVec4	p	=	EVec4(nxv.x, nxv.y, nxv.z, 1);
	
	EMatrix4 world	=	QuatToMatrix(q) * Matrix4Translate(p);
	

	HRCALL( shader_fx->SetMatrix("matrix_world",	&D3DXMATRIX( world.Ptr() ) ) );
	HRCALL( shader_fx->SetMatrix("matrix_view",		&D3DXMATRIX( rend_env->matrix_view.Ptr() ) ) );
	HRCALL( shader_fx->SetMatrix("matrix_proj",		&D3DXMATRIX( rend_env->matrix_proj.Ptr() ) ) );

	//
	//	draw ship :
	//
	uint n;
	HRCALL( shader_fx->SetTechnique("solid_body") );
	HRCALL( shader_fx->Begin(&n, 0) );
	
	for (uint pass=0; pass<n; pass++) {
	
		HRCALL( shader_fx->BeginPass(pass) );
	
		d3d_mesh_vis->DrawSubset(0);
		
		HRCALL( shader_fx->EndPass() );
	}

	HRCALL( shader_fx->End() );
}

	