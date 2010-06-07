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
#include "linear/linear.h"
#include "waves/waves.h"
#include "waves/newave.h"


/*-----------------------------------------------------------------------------
	Nano vis :
-----------------------------------------------------------------------------*/

#define SHIP_FS_PATH		"../scidata/uboat.esx"
#define SHIP_H_PATH			"|boat1"
#define SHADER_FX			"../scidata/shader.fx"

const float	SHIP_LENGTH		=	105;		//	m
const float	SHIP_WIDTH		=	6;			//	m
const float	SHIP_HEIGHT		=	7;			//	m
const float	SHIP_MASS		=	2000000;	//	kg
const float SHIP_CM_OFFSET	=  -0.7f;
const float WATER_DENSITY	=	1000;		//	kg/m^3
const float SHIP_CX			=	0.2f;


//
//	ESciVis::InitRender
//
void ESciVis::InitRender( void )
{
	LOG_INIT("Scientific renderer");

	//	read file :	
	mesh_ship	=	LoadMesh(SHIP_FS_PATH, SHIP_H_PATH);
	mesh_sea	=	LoadMesh(SHIP_FS_PATH, "|sea2d");
	mesh_flow	=	LoadMesh(SHIP_FS_PATH, "|flowsurf");
	
	ship		=	CreateMesh( mesh_ship );
	sea			=	CreateMesh( mesh_sea );
	
	shader_fx	=	CompileEffect(SHADER_FX);
	
	change_papams_to_base_reson();
	
	EQuat		q	=	QuatRotationAxis( deg2rad(0), EVec3(1,1,1));
	ship_body		=	CreatePhysBox( SHIP_LENGTH, SHIP_WIDTH, SHIP_HEIGHT, EVec4(-100,0,10,1), q, SHIP_MASS);
	ship_body->setLinearVelocity( NxVec3(50,0,0) );
	
	Xi	=	90;
}


//
//	ESciVis::ShutdownRender
//
void ESciVis::ShutdownRender( void )
{
	LOG_SHUTDOWN("Scientific renderer");

	SAFE_RELEASE(ship);
	SAFE_RELEASE(shader_fx);
}


//
//	ESciVis::SCI_RenderView
//
int ESciVis::SCI_RenderView( lua_State * L )
{
	ESciVis *self = Linker()->GetSciVis().As<ESciVis>();
	
	self->RenderView( L );
	
	return 0;
}


//
//
//
int ESciVis::SCI_ReloadShaders( lua_State * L )
{
	ESciVis *self = Linker()->GetSciVis().As<ESciVis>();
	
	self->shader_fx	=	self->CompileEffect(SHADER_FX);
	
	return 0;
}



float WaveFormZ(float x, float t) {
	x*=3;
	t/=4;
	float S=0.0;
	for(int i=0; i<NN; i++) {
		float W	=	Wmin + i * dW;
		float K	=	W * W / g;//g==9.81f
		S += A_2D[i] * cos(K*x - W*t + Fi_2D[i]);		
	}
	return 0.0 * S;
}

float WaveFormX(float x, float t) {
	x*=3;
	t/=4;
	float S=0.0;
	for(int i=0; i<NN; i++) {
		float W	=	Wmin + i * dW;
		float K	=	W * W / g;//g==9.81f
		S += A_2D[i] * sin(K*x - W*t + Fi_2D[i]);		
	}
	return 0.0 * S;
}

float wave_pos2D_x(float x)
{
	return WaveFormZ(x, iterat);
}

EVec3 wave_vel2D_x(float x)
{
	float t  = iterat;
	float dt = 0.01f;
	float dz = WaveFormZ(x, t+dt) - WaveFormZ(x, t);
	float dx = WaveFormX(x, t+dt) - WaveFormX(x, t);
	
	return EVec3(dx/dt, 0, dz/dt);
}


//
//
//
float FEM_StaticWaveForce( EVec4 pos, float szx, float szy, float szz, float wave_height ) 
{
	float wh = wave_height;		//	wave height

	float frac = (wh - (pos.z - 0.5f*szz)) / szz;
	frac = Clamp<float>(frac, 0,1);

	//float frac = 0;
	//if (pos.z<wh) {
	//	frac = 1;
	//}
	
	return frac * szx * szy * szz * WATER_DENSITY * 9.8;
}


NxVec3 FEM_HydroDynamicForce( NxVec3 velocity, EVec4 pos, float szx, float szy, float szz, float wave_height, NxVec3 normal ) 
{
	if (velocity.magnitude()<0.001) {
		return NxVec3(0,0,0);
	}
	
	float cos_normal_flow = abs(velocity.dot( normal ));
	
	//	add some viscosity
	cos_normal_flow = 0.05 + 0.95*cos_normal_flow;
	

	float wh = wave_height;
	
	float frac = 0;
	if (pos.z<wh) {
		frac = 1;
	}
	
	float S		=	sqrt((szx * szy * szz) * (szx * szy * szz));	//	average square
	float Vm	=	velocity.magnitude();						//	velocity abs value
	if (Vm>7) {
		Vm = 7;
	}
	
	float Fm	=	- SHIP_CX * Vm*Vm * S * WATER_DENSITY / 2 * frac * cos_normal_flow;
	
	NxVec3 nvel = velocity;
	nvel.normalize();
	
	return nvel * Fm;
}


NxVec3 CapsuleNormal( EVec4 pos ) {
	float hszy = (SHIP_WIDTH/2 + SHIP_HEIGHT/2) / 2;
	float hszx = SHIP_LENGTH / 2 - 2*hszy;
	
	NxVec3 n(0,0,0);
	
	if ( abs(pos.x) < hszx ) {
		n = NxVec3(0, pos.y, pos.z);
	} else
	if ( pos.x > hszx ) {
		n = NxVec3(pos.x - hszx, pos.y, pos.z);
	} else 
	if ( pos.x < -hszx ) {
		n = NxVec3(pos.x + hszx, pos.y, pos.z);
	}

	n.normalize();
	return n;
}

//
//
//
void ESciVis::UpdateBoat( float dtime )
{
	NxVec3 gravity	=	NxVec3(0,0,-9.8f) * SHIP_MASS;
	
	NxQuat	nxq	=	ship_body->getGlobalOrientationQuat();
	NxVec3	nxv	=	ship_body->getGlobalPosition();
	EQuat	q	=	EQuat(nxq.x, nxq.y, nxq.z, nxq.w);
	EVec4	p	=	EVec4(nxv.x, nxv.y, nxv.z, 1);

	
	ship_body->addForceAtLocalPos( gravity, NxVec3(0,0,SHIP_CM_OFFSET));
	
	UpdateBoatHSF( dtime, p, q );
	UpdateBoatHDF( dtime, p, q );
}

//
//	ESciVis::UpdateBoatHSF
//
void ESciVis::UpdateBoatHSF( float dtime, const EVec4 &position, const EQuat &orient )
{
	uint FEM_X	=	20;
	uint FEM_Y	=	4;
	uint FEM_Z	=	4;
	
	float dx = SHIP_LENGTH / FEM_X;
	float dy = SHIP_WIDTH  / FEM_Y;
	float dz = SHIP_HEIGHT / FEM_Z;
	
	for (uint i=0; i<FEM_X; i++) {
		for (uint j=0; j<FEM_Y; j++) {
			for (uint k=0; k<FEM_Z; k++) {
			
				if (j==0 && k==0) continue;
				if (j==0 && k==3) continue;
				if (j==3 && k==3) continue;
				if (j==3 && k==0) continue;
				
				float x = -(0.5 * SHIP_LENGTH) + 0.5*dx + dx * (float)i;
				float y = -(0.5 * SHIP_WIDTH ) + 0.5*dy + dy * (float)j;
				float z = -(0.5 * SHIP_HEIGHT) + 0.5*dz + dz * (float)k;

				EVec4  pos(x,y,z,0);
				NxVec3 normal = CapsuleNormal( pos );
				
				pos =  QuatRotateVector(pos, orient);
				pos += position;			
				
				float wh = wave_pos2D_x(pos.x);		//	wave height
				
				NxVec3	pvel = ship_body->getPointVelocity( NxVec3(pos.x, pos.y, pos.z) ) - ToNxVec3( wave_vel2D_x(pos.x) );

				float	fs = FEM_StaticWaveForce(pos, dx, dy, dz, wh);					//	static force
				NxVec3	fx = NxVec3(0,0,0);
				
				fx = FEM_HydroDynamicForce(pvel, pos, dx, dy, dz, wh, normal);		//	dynamic force
				
				
				ship_body->addForceAtPos( NxVec3(0,0,fs), NxVec3(pos.x, pos.y, pos.z) );
				
				//ship_body->addForceAtPos( fx, NxVec3(pos.x, pos.y, pos.z) );
				
				//DebugLine( EVec3(pos.x, pos.y, pos.z), EVec3(pos.x, pos.y, pos.z + fs/SHIP_MASS*40), EVec4(0,0,1,1));
				//DebugLine( NxVec3(pos.x, pos.y, pos.z), NxVec3(pos.x, pos.y, pos.z) + fx/SHIP_MASS*40, EVec4(1,0,0,1));
				//DebugLine( NxVec3(pos.x, pos.y, pos.z), NxVec3(pos.x, pos.y, pos.z) + pvel, EVec4(0,1,0,1));

				//DebugLine( NxVec3(pos.x, pos.y, pos.z), NxVec3(pos.x, pos.y, pos.z) + normal * 5, EVec4(1,1,1,1));
			}
		}
	}
}


//
//	ESciVis::UpdateBoatHDF
//	- updates hydrodynamic forces 
//
void ESciVis::UpdateBoatHDF( float dtime, const EVec4 &position, const EQuat &orient )
{
	for (uint i=0; i<mesh_flow->GetTriangleNum(); i++) {
		
		EVec3 fe_pos		=	QuatRotateVector( mesh_flow->TriangleCenter(i), orient ) + EVec3(position.Ptr());
		EVec3 fe_normal		=	QuatRotateVector( mesh_flow->TriangleNormal(i), orient );
		float fe_area		=	mesh_flow->TriangleArea(i);
		EVec3 fe_vel_global	=	ToEVec3( ship_body->getPointVelocity( ToNxVec3(fe_pos) ) );
		
		EVec3 flow_vel		=	wave_vel2D_x(fe_pos.x);
		float wave_h		=	wave_pos2D_x(fe_pos.x);
		
		if (wave_h < fe_pos.z) {	
			continue;
		}
		
		//	element velocity relative to flow :
		EVec3 fe_vel		=	fe_vel_global - flow_vel;
		
		//	cosine angle between velocity and normal :
		float cos_vel_norm		=	Vec3Dot( Vec3Normalize( fe_vel ), fe_normal );
		float cos_2_vel_norm	=	2 * (cos_vel_norm * cos_vel_norm) - 1;
		float cos_sign			=	1 ? -1 : cos_vel_norm > 0;

		//	0.1 - stupid viscosity addition		
		//	0.5 - just front or back pressure 
		//  sqrt(0.5) - because lifting force is maximum at PI/4
		const float	Cxx		=	0.2;
		const float Cyy		=	0.0;
		float Cx			=	Cxx * 0.5 * (0.9 * abs(cos_vel_norm) + 0.1);	
		float Cy			=	Cyy * 0.5 * sqrt(0.5) * cos_2_vel_norm;
		
		float hd_force_x	=	Cx * Vec3LengthSqr( fe_vel ) * fe_area * WATER_DENSITY / 2;
		float hd_force_y	=	Cy * Vec3LengthSqr( fe_vel ) * fe_area * WATER_DENSITY / 2;
		
		//	compute vectors of force applying :
		EVec3 force_x_dir	=	Vec3Normalize( fe_vel ) * cos_sign;
		EVec3 force_y_dir	=	- Vec3Normalize( Vec3Cross( fe_vel, Vec3Cross( fe_normal, fe_vel ) ) ) * cos_sign;
		
		EVec3 vhd_force_x	=	force_x_dir * hd_force_x;
		EVec3 vhd_force_y	=	force_y_dir * hd_force_y;
		
		//	add force :
		NxVec3	vhd_force	=	ToNxVec3( vhd_force_x + vhd_force_y );
		ship_body->addForceAtPos( vhd_force, ToNxVec3(fe_pos) );

		//	draw force :
		//DebugLine( ToNxVec3(fe_pos), ToNxVec3(fe_pos) + vhd_force * 0.0001, EVec4(1,0,0,1));
		//DebugLine( ToNxVec3(fe_pos), ToNxVec3(fe_pos + fe_normal * 5),		EVec4(0,1,0,1));
		DebugLine( ToNxVec3(fe_pos), ToNxVec3(fe_pos + fe_vel * 5),		EVec4(0,1,0,1));
	}
}



//
//
//
void ESciVis::Simulate( float dtime )
{
	dT = dtime * 10;
	init_boukh_wave();
}



//
//	ESciVis::RenderView
//
void ESciVis::RenderView( lua_State * L )
{
	if (!shader_fx) {
		return;
	}

	float	dtime			=	0.016f;
	float	yaw				=	0;
	float	roll			=	0;
	float	pitch			=	0;
	float	distance		=	10;
	float	ship_course		=	0;
	LuaGetField( L, 1, "dtime",				dtime			);
	LuaGetField( L, 1, "yaw",				yaw				);
	LuaGetField( L, 1, "roll",				roll			);
	LuaGetField( L, 1, "pitch",				pitch			);
	LuaGetField( L, 1, "distance",			distance		);
	
	LuaGetField( L, 1, "ship_course",		ship_course		);
	Xi	=	ship_course;
	//
	//	simulate :
	//
	UpdateBoat(dtime);
	FramePhysX(dtime);
	
	Simulate(dtime);

	//
	//	setup view :
	//
	D3DXMATRIX	world, ship_yaw, ship_roll, ship_pitch, ship_heaving;
	D3DXMATRIX	view, view_i, rz, ry, rx, z_view_offset;
	D3DXMATRIX	proj;

	//	setup world matrix :
	D3DXMatrixIdentity		( &world );	
	D3DXMatrixRotationZ		( &ship_yaw		,	deg2rad(Xi+180) );
	D3DXMatrixRotationX		( &ship_roll	,	Q			);
	D3DXMatrixRotationY		( &ship_pitch	,	-F			);
	D3DXMatrixTranslation	( &ship_heaving	,	0, 0,	E	); 
	
	world	=	ship_roll * ship_pitch * ship_yaw * ship_heaving;
	
	if (true) {
		NxQuat	nxq	=	ship_body->getGlobalOrientationQuat();
		NxVec3	nxv	=	ship_body->getGlobalPosition();
		EQuat	q	=	EQuat(nxq.x, nxq.y, nxq.z, nxq.w);
		EVec4	p	=	EVec4(nxv.x, nxv.y, nxv.z, 1);
		
		world		=	(QuatToMatrix(q) * Matrix4Translate(p)).Ptr();
	}

	//	setup view matrix :
	D3DXMATRIX	z_up(	 0, 0, 1, 0,
						 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 0, 1	);
	
	D3DXMatrixRotationX		( &rx,	deg2rad(roll) );
	D3DXMatrixRotationY		( &ry,	deg2rad(pitch) );
	D3DXMatrixRotationZ		( &rz,	deg2rad(yaw) );
	D3DXMatrixTranslation	( &z_view_offset,	0,0, -distance );
	view	=	 rz * ry * rx * z_up * z_view_offset;
	
	D3DXMatrixInverse( &view_i, NULL, &view );

	//	setup projection :	
	D3DXMatrixPerspectiveRH( &proj, 0.100f, 0.075f, 0.075f, 1000 );

	//	setup view & light vectors :
	D3DXVECTOR4		light_dir	( 0, 0, 1, 0);
	D3DXVECTOR4		view_dir	( 0, 0,-1, 0);
	D3DXVECTOR4		atom_color	( 1, 1, 1, 1);
	D3DXVECTOR4		view_point	( 0, 0,-distance, 0);
	D3DXVec4Transform( &view_point, &view_point, &view );
	D3DXVec4Transform( &view_dir, &view_dir, &view_i );
	D3DXVec4Transform( &view_point, &view_point, &view_i );

	//
	//	commit shader consts :
	//
	HRCALL( shader_fx->SetVector("light_dir",		&light_dir) );
	HRCALL( shader_fx->SetVector("view_dir",		&view_dir) );
	HRCALL( shader_fx->SetMatrix("matrix_world",	&world) );
	HRCALL( shader_fx->SetMatrix("matrix_view",	&view) );
	HRCALL( shader_fx->SetMatrix("matrix_proj",	&proj) );

	//
	//	draw ship :
	//
	uint n;
	HRCALL( shader_fx->SetTechnique("solid_body") );
	HRCALL( shader_fx->Begin(&n, 0) );
	
	for (uint pass=0; pass<n; pass++) {
	
		HRCALL( shader_fx->BeginPass(pass) );
	
		ship->DrawSubset(0);
		
		HRCALL( shader_fx->EndPass() );
	}

	HRCALL( shader_fx->End() );

	//
	//	draw sea :
	//
	for (uint i=0; i<mesh_sea->GetVertexNum(); i++) {
		EVertex		v = mesh_sea->GetVertex(i);
		
		v.position.z	=	wave_pos2D_x(v.position.x);
		
		mesh_sea->SetVertex(i, v);
	}
	mesh_sea->ComputeNormals();
	
	UpdateMeshVertices( sea, mesh_sea );
	
	D3DXMatrixIdentity		( &world );	
	HRCALL( shader_fx->SetMatrix("matrix_world",	&world) );
	
	HRCALL( shader_fx->SetTechnique("solid_body") );
	HRCALL( shader_fx->Begin(&n, 0) );
	
	for (uint pass=0; pass<n; pass++) {
	
		HRCALL( shader_fx->BeginPass(pass) );
	
		sea->DrawSubset(0);
		
		HRCALL( shader_fx->EndPass() );
	}

	HRCALL( shader_fx->End() );
	
	//DebugPhysX(world, view, proj);
}

