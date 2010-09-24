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

const uint EQ_BAND_NUM			=	7;
const uint EQ_SPECTRUM_SIZE		=	1 << (EQ_BAND_NUM-1);


class EWavingBoukh : public IWaving {
	public:
							EWavingBoukh		( lua_State *L, int idx );
							~EWavingBoukh		( void );
											
		virtual void		Update				( float dtime );
		virtual	EVec4		GetVelocity			( const EVec4 &init_pos ) const;
		virtual	EVec4		GetPosition			( const EVec4 &init_pos ) const;
		virtual float		GetPressure			( const EVec4 &init_pos ) const;
		virtual float		GetWaveSlopeX		( const EVec4 &init_pos ) const;
		virtual void		SetupWaving			( float base, uint num_bands, float *bands );
		
	protected:
		IPxFREntity		r_ent;
		IPxTriMesh		sea_mesh;

		struct {		
			float		base_freq;
			float		bands			[EQ_BAND_NUM];
			float		frequency		[EQ_SPECTRUM_SIZE];
			float		spectrum		[EQ_SPECTRUM_SIZE];
			float		phases			[EQ_SPECTRUM_SIZE];
			float		wave_number		[EQ_SPECTRUM_SIZE];
		} wave;

		float			GetAmplitudeByHarmonic	( uint n );		
	
		float	time;
		EVec4				GetPositionAtTime	( const EVec4 &init_pos, float time ) const;
	
		void				InitGenerator	( void );
		void				GetWave			( float lambda, float x, float zeta, float t, float &offset, float &dpressure, float &angle ) const;
		void				GetWaveC		( EVec4 pos, float t, float &offset, float &dpressure, float &angle ) const;
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

	sea_mesh	=	ge()->LoadMeshFromFile("sea.esx|sea");
	
	r_ent		=	sci_vis->GetFRScene()->AddEntity();
	r_ent->SetMesh( sea_mesh );
}


//
//	EWavingBoukh::~EWavingBoukh
//
EWavingBoukh::~EWavingBoukh( void )
{
	sci_vis->GetFRScene()->RemoveEntity( r_ent );
}


/*-----------------------------------------------------------------------------
	Spectral stuff :
-----------------------------------------------------------------------------*/

//
//	EWavingBoukh::GetAmplitudeByHarmonic
//
float EWavingBoukh::GetAmplitudeByHarmonic( uint hrm )
{
	if (hrm==0) {
		return 0;
	}

	for (uint i=0; i<EQ_BAND_NUM; i++) {
		uint bandhrm0	=	1 << (i);
		uint bandhrm1	=	1 << (i+1);
		
		if ( bandhrm0 <= hrm && hrm < bandhrm1 ) {
			
			float	x	=	hrm;
			float	a0	=	wave.bands[i];
			float	a1	=	wave.bands[i+1];
			float	f0	=	(float)bandhrm0;
			float	f1	=	(float)bandhrm1;
			float	k	=	(a1 - a0) / (f1 - f0);
			
			float	y	=	k * (x - f0) + a0;
			
			return	y;
		}
	}
	
	SIGNAL("hrm > 2^EQ_BAND_NUM-1");
	return 0;
}

//
//	EWavingBoukh::SetupWaving
//
void EWavingBoukh::SetupWaving( float base, uint num_bands, float *bands )
{
	wave.base_freq		=	base;
	
	//	read equalizer bands :
	for (uint i=0; i<EQ_BAND_NUM; i++) {
		if (i<num_bands) {
			wave.bands[i] = bands[i];
		} else {
			wave.bands[i] = 0;
		}
	}
	
	//	setup wave stuff :
	for (uint i=0; i<EQ_SPECTRUM_SIZE; i++) {
		wave.frequency[i]	=	wave.base_freq * i;
		wave.spectrum[i]	=	GetAmplitudeByHarmonic( i );
		wave.phases[i]		=	FRand(0, 2*PI);
		wave.wave_number[i]	=	wave.frequency[i] * wave.frequency[i] / GRAVITY;

		LOGF("%d : %5.2f Hz, %5.2f m, %5.2f Pi", i, wave.frequency[i], wave.spectrum[i], wave.phases[i]/PI);
			
	}
	
}


/*-----------------------------------------------------------------------------
	Spectral runtime stuff :
-----------------------------------------------------------------------------*/

//
//	EWavingBoukh::Update
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
	if (lambda<0.001f) {
		offset		=	0;
		dpressure	=	0;
		angle		=	0;
	}
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
	
	float	gamma	=	GRAVITY * WATER_DENSITY;
	float	x		=	pos.x;
	float	z		=	-pos.z;

	for (uint i=0; i<EQ_SPECTRUM_SIZE; i++) {
	
		float	r	=	wave.spectrum[i];
		float	f	=	wave.frequency[i];
		float	k	=	wave.wave_number[i];
		float	ph	=	wave.phases[i];
	
		float	o	=	- r * exp(-k*z) * cos(ph + k*x - f*t);
		float	p	=	- r * exp(-k*z) * cos(ph + k*x - f*t) * gamma;
		float	a	=	0;
		
		offset		+=	o;
		dpressure	+=	p;
	}
	
	
/*	for (uint i=0; i<3; i++) {
		GetWave( 20 / (float)(1+i), x, -pos.z, time, o, p, a );
		
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
	}	*/
}

//
//	EWavingBoukh::GetPressure
//
float EWavingBoukh::GetPressure( const EVec4 &pos ) const
{
	float offset, dpressure, angle, surf_offs, dummy;

	GetWaveC( EVec4(pos.x, pos.y, 0, 1), time, surf_offs, dummy, dummy );

	if (pos.z>surf_offs) {	
		return 0;
	}

	GetWaveC( pos, time, offset, dpressure, angle );
	
	//float p0	=	GRAVITY * WATER_DENSITY * abs(surf_offs - pos.z);
	float p0	=	GRAVITY * WATER_DENSITY * abs(-pos.z) + dpressure;
	
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
	return EVec4(0,0,0,0);
}

