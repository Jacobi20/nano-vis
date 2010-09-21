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
	Waving :
-----------------------------------------------------------------------------*/
const uint	NN			=	50;

class EWavingBoukh : public IWaving {
	public:
							EWavingBoukh		( lua_State *L, int idx );
							~EWavingBoukh		( void );
											
		virtual void		Update				( float dtime );
		virtual	EVec4		GetVelocity			( const EVec4 &init_pos ) const;
		virtual	EVec4		GetPosition			( const EVec4 &init_pos ) const;
		virtual float		GetPressure			( const EVec4 &init_pos ) const;
		virtual float		GetWaveSlopeX		( const EVec4 &init_pos ) const;
		
	protected:
		IPxFREntity		r_ent;
		IPxTriMesh		sea_mesh;
	
		float	time;
		EVec4				GetPositionAtTime	( const EVec4 &init_pos, float time ) const;
	
		void				InitGenerator	( void );
		void				GetWave			( float lambda, float x, float zeta, float t, float &offset, float &dpressure, float &angle ) const;
		void				GetWaveC		( EVec4 pos, float t, float &offset, float &dpressure, float &angle ) const;
	
		float	h3		;	// --- высота волный 3%-ой обеспеченности, м --- 	 			
		float	Wmax	;	/*2.0f*/ // == Максимальная частота ==
		float	BSp		;	/*1.3f*/  // == Параметр формы спектра ==
		float	ASp		;	/*10*/ // == Параметр масштаба спектра ==

		float	Wmin	;	/*0.2f*/	//	Минимальная частота
		float	W0		;	/*1.2f*/	//	Средняя частота волны, рад/c
		float	A_2D	[NN];
		float	Fi_2D	[NN];
		float	W		[NN];
	
	};

IWaving	*create_boukh_waving(lua_State *L, int idx) { return new EWavingBoukh(L, idx); }

/*-----------------------------------------------------------------------------
	Implementation :
-----------------------------------------------------------------------------*/

//
//
//
EWavingBoukh::EWavingBoukh( lua_State *L, int idx )
{
	time	=	0;

	Wmax	=	 5.1f	;
	W0		=	 3.51f	;
	Wmin	=	 0.5f	;
	BSp		=	 0.030f;
	ASp		=	 0.233f	;

	h3		=	5		;
	
	//	extreme resonance :
	//Wmax  = 0.5*2.0f;	// == Максимальная частота ==
	//W0	  = 0.5*1.2f ;	//	Средняя частота волны, рад/c
	//BSp   = 0.5*1.3f ;	// == Параметр формы спектра ==
	//ASp   = 10;		// == Параметр масштаба спектра ==
	//Wmin  = 0.5*0.2f ;	//	Минимальная частота
	
	InitGenerator();
	
	sea_mesh	=	ge()->LoadMeshFromFile("sea.esx|sea");
	
	r_ent		=	sci_vis->GetFRScene()->AddEntity();
	r_ent->SetMesh( sea_mesh );
}


//
//
//
void EWavingBoukh::InitGenerator( void )
{
	float dW = (Wmax-Wmin)/(float)NN;
	
	for (int i=0; i<NN; i++) { 
		W[i]=Wmin+i*dW;
	}

	for(int i=0; i<NN; i++) {
		float w	 = W[i];
		A_2D[i]	 = (float)sqrt(2*ASp*(1.0f/(w*w*w*w*w))*exp(-BSp/(w*w*w*w))*dW);
		Fi_2D[i] = (float)FRand()*2.0f*PI ;//rand() - 0..1
	}
}


//
//
//
EWavingBoukh::~EWavingBoukh( void )
{
	sci_vis->GetFRScene()->RemoveEntity( r_ent );
}


//
//
//
void EWavingBoukh::Update( float dtime )
{
	time	+=	dtime;
	
	IPxTriMesh	mesh	=	sea_mesh->Clone();
	
	uint n = mesh->GetVertexNum();
	
	for (uint i=0; i<n; i++) {
		EVertex	v;
		
		v	=	mesh->GetVertex( i );
		v.position.z	=	GetPosition( Vec3ToVec4(v.position) ).z;
		
		mesh->SetVertex( i, v );
	}
	
	r_ent->SetMesh( mesh );
}


//
//	EWavingBoukh::GetWave
//
void EWavingBoukh::GetWave( float lambda, float x, float zeta, float t, float &offset, float &dpressure, float &angle ) const
{
	float	r0		=	0.5 * 0.5 * 0.17f * pow(lambda, 0.75f);			//	max oscillation radius of water particles
	//float	r0		=	lambda / 20.0f;						//	max oscillation radius of water particles
	float	k		=	2 * PI / lambda;					//	wave number
	float	a0		=	k * r0;								//	maximum wave slope angle
	float	rz		=	r0 * exp(-k * zeta);				//	oscillation radius on depth 'zeta'
	float	az		=	a0 * exp(-k * zeta);				//	slope angle radius on depth 'zeta'
	float	sigma	=	sqrt( GRAVITY * k );				//	sigma
	float	gamma	=	GRAVITY * WATER_DENSITY;			//	water weight density
	
	offset			= - rz * cos(k*x - sigma*t);			//	particle offset
	dpressure		= - gamma * rz * cos(k*x - sigma*t);	//	dynamic pressure
	angle			= - az * sin(k*x - sigma*t);			//	wave slope angle
}


//
//	EWavingBoukh::GetWaveC
//
void EWavingBoukh::GetWaveC( EVec4 pos, float t, float &offset, float &dpressure, float &angle )	const
{
	offset		=	0;
	dpressure	=	0;
	angle		=	0;
	
	//GetWave(20, pos.x, -pos.z, time, offset, dpressure, angle );
	
	for (uint i=0; i<3; i++) {
		float	x	=	pos.x;
		float	o, p, a;
		GetWave( 100 / (float)(1+i), x, -pos.z, time, o, p, a );
		
		offset		+=	o;
		dpressure	+=	p;
		angle		+=	a;
	}	
	
	for (uint i=0; i<3; i++) {
		float	x	=	pos.x - 0.125 * pos.y;
		float	o, p, a;
		GetWave( 50 / (float)(1+i), x, -pos.z, time, o, p, a );
		
		offset		+=	o;
		dpressure	+=	p;
		angle		+=	a;
	}
}

//
//	EWavingBoukh::GetPressure
//
float EWavingBoukh::GetPressure( const EVec4 &pos ) const
{
	float offset, dpressure, angle;
	GetWaveC( pos, time, offset, dpressure, angle );
	
	float p0	=	GRAVITY * WATER_DENSITY * (-pos.z + offset);
	
	
	
	if (pos.z>offset) {	
		return 0;
	}
	return p0;
}


//
//	EWavingBoukh::GetPositionAtTime
//
EVec4 EWavingBoukh::GetPositionAtTime( const EVec4 &init_pos, float time ) const
{
	float offset, pressure, angle;
	GetWaveC( init_pos, time, offset, pressure, angle );
	return EVec4( init_pos.x, init_pos.y, init_pos.z + offset, 1 );
}


//
//	EWavingBoukh::GetWaveSlopeX
//
float EWavingBoukh::GetWaveSlopeX( const EVec4 &init_pos ) const
{
	EVec4	p	=	init_pos;
	float	dx	=	0.1f;
	EVec4	p0	=	GetPosition( EVec4( p.x,		p.y, p.z, 1 ) );
	EVec4	p1	=	GetPosition( EVec4( p.x + dx,	p.y, p.z, 1 ) );
	//if (p1.x <= p0.x) {
	//	LOG_WARNING("wave slope is to large!");
	//}
	return atan2( p1.z - p0.z, dx );
}


//
//	EWavingBoukh::GetPosition
//
EVec4 EWavingBoukh::GetPosition( const EVec4 &init_pos ) const
{
	return GetPositionAtTime( init_pos, time );
}


//
//	EWavingBoukh::GetVelocity
//
EVec4 EWavingBoukh::GetVelocity( const EVec4 &init_pos ) const
{
	float dt = 0.00390625;	//	1 / 256
	EVec4 p1 = GetPositionAtTime(init_pos, time);
	EVec4 p2 = GetPositionAtTime(init_pos, time + dt);
	return (p2 - p1) / dt;
}

