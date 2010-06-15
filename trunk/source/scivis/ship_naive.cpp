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
	
	Ship metrics :	
	http://en.wikipedia.org/wiki/Hull_%28watercraft%29#cite_note-2
-----------------------------------------------------------------------------*/

const float	SHIP_LENGTH			=	105;		//	m	- true: overall length
const float	SHIP_WIDTH			=	6;			//	m	- true: beam
const float	SHIP_HEIGHT			=	7;			//	m	- true: depth (D)
const float	SHIP_MASS			=	1653750;	//	kg
const float SHIP_INERTIA_X		=	0.5 * SHIP_MASS * (0.5*SHIP_WIDTH * 0.5*SHIP_WIDTH);
const float	SHIP_DRAFT			=	SHIP_HEIGHT	/ 2;
const float SHIP_COEFF_BLOCK	=	0.75;		//	not true Block coefficient, just L*W*H
const float SHIP_CM_OFFSET		=  -0.7f;

const float WATER_DENSITY		=	1000;		//	kg/m^3

const float SHIP_CX				=	0.2f;

const float	INIT_ZETA			=	0;			//	Z - position
const float	INIT_YAW			=	90;
const float	INIT_PITCH			=	0;
const float	INIT_ROLL			=	10;



class EShipNaive : public IShip {
	public:
							EShipNaive		( lua_State *L, int idx );
							~EShipNaive		( void );
					
		virtual void		Simulate		( float dtime, IPxWaving waving );
		virtual void		ReloadShader	( void );
		virtual void		GetPose			( EVec4 &position, EQuat &orient );
		virtual void		Render			( ERendEnv_s *rend_env );
		
	protected:
		void				UpdateForces	( float dtime, IPxWaving waving );
		void				UpdateHSF		( float dtime, IPxWaving waving );
		void				UpdateHDF		( float dtime, IPxWaving waving );

		//	stuff for numerical rolling simulation :
		void				UpdateNumeric	( float dtime, IPxWaving waving );
		void				EulerStep		( float dt, IPxWaving waving );
		float	num_roll;
		float	num_roll_d;
		float	num_zeta;
		float	num_zeta_d;
	
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
	

	num_roll		=	INIT_ROLL;
	num_roll_d		=	0;
	num_zeta		=	INIT_ZETA;
	num_zeta_d		=	0;


	EQuat		q	=	QuatRotationAxis( deg2rad( INIT_YAW   ),	EVec3(0,0,1))
					*	QuatRotationAxis( deg2rad( INIT_PITCH ),	EVec3(0,1,0))
					*	QuatRotationAxis( deg2rad( INIT_ROLL  ),	EVec3(1,0,0));

	EVec4		p	=	EVec4(0,0, INIT_ZETA, 1);
	ship_body		=	sci_vis->CreatePhysBox( SHIP_LENGTH, SHIP_WIDTH, SHIP_HEIGHT, p, q, SHIP_MASS);
	
	ReloadShader();
}


EShipNaive::~EShipNaive( void )
{
	sci_vis->GetNxScene()->releaseActor( *ship_body );
	SAFE_RELEASE( d3d_mesh_vis );
	SAFE_RELEASE( shader_fx );
}


void EShipNaive::ReloadShader( void )
{
	shader_fx		=	sci_vis->CompileEffect( "../scidata/shader.fx" );
}


void EShipNaive::Simulate( float dtime, IPxWaving waving )
{
	UpdateNumeric( dtime, waving );
	//	UpdateForces( dtime, waving );
}


void EShipNaive::GetPose( EVec4 &position, EQuat &orient )
{
	NxQuat	nxq	=	ship_body->getGlobalOrientationQuat();
	NxVec3	nxv	=	ship_body->getGlobalPosition();
	EQuat	q	=	EQuat(nxq.x, nxq.y, nxq.z, nxq.w);
	EVec4	p	=	EVec4(nxv.x, nxv.y, nxv.z, 1);
	
	position	=	p;
	orient		=	q;
	
	//	numerical results :
	position	=	EVec4(0,0, num_zeta, 1);
	orient		=	QuatRotationAxis( deg2rad( INIT_YAW   ),	EVec3(0,0,1)) 
				*	QuatRotationAxis( deg2rad( INIT_PITCH ),	EVec3(0,1,0))
				*	QuatRotationAxis( deg2rad( num_roll   ),	EVec3(1,0,0));
	
	
}


void EShipNaive::Render( ERendEnv_s *rend_env )
{
	if (!shader_fx) return;

	EQuat	q;
	EVec4	p;
	GetPose(p, q);
	
	EMatrix4 world	=	QuatToMatrix(q) * Matrix4Translate(p);

	HRCALL( shader_fx->SetMatrix("matrix_world",	&D3DXMATRIX( world.Ptr() ) ) );
	HRCALL( shader_fx->SetMatrix("matrix_view",		&D3DXMATRIX( rend_env->matrix_view.Ptr() ) ) );
	HRCALL( shader_fx->SetMatrix("matrix_proj",		&D3DXMATRIX( rend_env->matrix_proj.Ptr() ) ) );
	HRCALL( shader_fx->SetVector("view_pos",		&D3DXVECTOR4( rend_env->view_pos.Ptr() ) ) );

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


/*-----------------------------------------------------------------------------
	Forces :
-----------------------------------------------------------------------------*/

void EShipNaive::UpdateForces( float dtime, IPxWaving waving )
{
	NxVec3 gravity	=	NxVec3(0,0,-9.8f) * SHIP_MASS;
	
	EQuat	q;
	EVec4	p;
	GetPose(p, q);
	
	ship_body->addForceAtLocalPos( gravity, NxVec3(0,0,SHIP_CM_OFFSET));
	
	UpdateHSF( dtime, waving );
	UpdateHDF( dtime, waving );
}


static float FEM_StaticWaveForce( EVec4 pos, float szx, float szy, float szz, float wave_height ) 
{
	float wh = wave_height;		//	wave height

	float frac = (wh - (pos.z - 0.5f*szz)) / szz;
	frac = Clamp<float>(frac, 0,1);
	
	return frac * szx * szy * szz * WATER_DENSITY * 9.8;
}



void EShipNaive::UpdateHSF( float dtime, IPxWaving waving )
{
	EVec4	position;
	EQuat	orient;
	GetPose(position, orient);

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
				
				pos =  QuatRotateVector(pos, orient);
				pos += position;			
				
				float	wh = waving->GetPosition(EVec4(pos.x, pos.y, pos.z, 1)).z;

				float	fs = FEM_StaticWaveForce(pos, dx, dy, dz, wh);					//	static force
				
				ship_body->addForceAtPos( NxVec3(0,0,fs), NxVec3(pos.x, pos.y, pos.z) );

				//DebugLine( EVec3(pos.x, pos.y, pos.z), EVec3(pos.x, pos.y, pos.z + fs/SHIP_MASS*40), EVec4(0,0,1,1));
				//DebugLine( NxVec3(pos.x, pos.y, pos.z), NxVec3(pos.x, pos.y, pos.z) + fx/SHIP_MASS*40, EVec4(1,0,0,1));
				//DebugLine( NxVec3(pos.x, pos.y, pos.z), NxVec3(pos.x, pos.y, pos.z) + pvel, EVec4(0,1,0,1));

				//DebugLine( NxVec3(pos.x, pos.y, pos.z), NxVec3(pos.x, pos.y, pos.z) + normal * 5, EVec4(1,1,1,1));
			}
		}
	}
}

void EShipNaive::UpdateHDF( float dtime, IPxWaving waving )
{
	EVec4	position;
	EQuat	orient;
	GetPose(position, orient);

	for (uint i=0; i<mesh_flow->GetTriangleNum(); i++) {
		
		EVec3 fe_pos		=	QuatRotateVector( mesh_flow->TriangleCenter(i), orient ) + EVec3(position.Ptr());
		EVec3 fe_normal		=	QuatRotateVector( mesh_flow->TriangleNormal(i), orient );
		float fe_area		=	mesh_flow->TriangleArea(i);
		EVec3 fe_vel_global	=	ToEVec3( ship_body->getPointVelocity( ToNxVec3(fe_pos) ) );
		
		EVec3 flow_vel		=	Vec4ToVec3( waving->GetVelocity( Vec3ToPoint4(fe_pos) ) );
		float wave_h		=				waving->GetPosition( Vec3ToPoint4(fe_pos) ).z;
		
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
		const float	Cxx		=	2.0;
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
		//DebugLine( ToNxVec3(fe_pos), ToNxVec3(fe_pos + fe_vel * 5),		EVec4(0,1,0,1));
	}
}






//
//	EShipNaive::UpdateNumeric
//
//	Equation :
//	M * zeta''  + N1 * zeta'  + lamda * S * zeta = 0	//	Hanovich	(formula 41)
//	M * zeta_d'	+ N1 * zeta_d + lamba * S * zeta = 0
//
//	zeta_d'		=	- 1 / M  *  ( lamda * S * zeta  +  N1 * zeta_d )
//	zeta'		=	
//

const float N1			=	0.1 * SHIP_LENGTH	* SHIP_WIDTH;	//	Hanovich (formula 56)
const float N2			=	1.0015 * SHIP_LENGTH * (SHIP_WIDTH * SHIP_WIDTH * SHIP_WIDTH * SHIP_WIDTH);	// devided by 'roll_d'
const float M			=	SHIP_MASS;
const float S			=	SHIP_LENGTH			* SHIP_WIDTH;
const float lamda		=	SHIP_COEFF_BLOCK	* WATER_DENSITY;
const float I			=	SHIP_INERTIA_X;
const float MCH			=	1;	//	metacenter height

static float expr_zeta_dd ( float zeta, float zeta_d )	{	return - ( lamda * S * zeta  +  N1 * zeta_d ) / M;	}
static float expr_zeta_d  ( float zeta, float zeta_d )	{	return zeta_d;	}
static float expr_roll_dd ( float roll, float roll_d )	{	return - ( N2 * roll_d * roll_d + M * 9.8 * MCH * roll ) / I; }
static float expr_roll_d  ( float roll, float roll_d )	{	return roll_d;	}

void EShipNaive::EulerStep( float dt, IPxWaving waving )
{
	float	next_zeta_d		=	num_zeta_d  + dt * expr_zeta_dd( num_zeta, num_zeta_d );
	float	next_zeta		=	num_zeta	+ dt * expr_zeta_d ( num_zeta, num_zeta_d );
	float	next_roll_d		=	num_roll_d  + dt * expr_roll_dd( num_roll, num_roll_d );
	float	next_roll		=	num_roll	+ dt * expr_roll_d ( num_roll, num_roll_d );
	
	num_zeta_d	=	next_zeta_d;
	num_zeta	=	next_zeta;
	num_roll_d	=	next_roll_d;
	num_roll	=	next_roll;
}


void EShipNaive::UpdateNumeric( float dtime, IPxWaving waving )
{
	for (uint i=0; i<10; i++) {
		EulerStep( dtime / 10.0f, waving );
	}
}

	