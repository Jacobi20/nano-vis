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

const uint WAVE_BAND_NUM		=	20;

struct point_wave_s {
		float	offset;
		float	pressure;
	};

class EWaving : public IWaving {
	public:
							EWaving		( lua_State *L, int idx );
							~EWaving		( void );
											
		virtual void		Update				( float dtime );
		virtual	EVec4		GetVelocity			( const EVec4 &init_pos ) const;
		virtual	EVec4		GetPosition			( const EVec4 &init_pos ) const;
		virtual float		GetPressure			( const EVec4 &init_pos ) const;
		virtual float		GetWaveSlopeX		( const EVec4 &init_pos ) const;
		
	protected:
		float			time;

		IPxFREntity		r_ent;
		IPxTriMesh		sea_mesh;

		struct {		
			float	max_freq;					//	angular, 'omega'
			float	amplitudes[WAVE_BAND_NUM];	//	amplitudes
			float	frequency[WAVE_BAND_NUM];	//	i * max_freq / BAND_NUM
			float	phases[WAVE_BAND_NUM];		//	rand(2*Pi)
			float	wave_num[WAVE_BAND_NUM];	//	k
		} wave;
		
		virtual void	InitWaving			( void );
		point_wave_s	GetWave				( float x, float y, float depth, float time ) const;

		EVec4			GetPositionAtTime	( const EVec4 &init_pos, float time ) const;
	};

IWaving	*create_waving(lua_State *L, int idx) { return new EWaving(L, idx); }

/*-----------------------------------------------------------------------------
	Implementation :
-----------------------------------------------------------------------------*/

//
//
//
EWaving::EWaving( lua_State *L, int idx )
{
	time	=	0;

	sea_mesh	=	ge()->LoadMeshFromFile("sea.esx|sea");
	
	r_ent		=	sci_vis->GetFRScene()->AddEntity();
	r_ent->SetMesh( sea_mesh );
	
	InitWaving();
}


//
//	EWaving::~EWaving
//
EWaving::~EWaving( void )
{
	sci_vis->GetFRScene()->RemoveEntity( r_ent );
}


/*-----------------------------------------------------------------------------
	Spectral stuff :
-----------------------------------------------------------------------------*/

static float SpectrumPM(float w)
{
	//	Pierson-Moskowitz :
	float	Asp	=	20.0f;
	float	Bsp	=	4.0f;
	
	return Asp * expf(-Bsp / (w*w*w*w)) / (w*w*w*w*w);
}

//
//	EWaving::SetupWaving
//
void EWaving::InitWaving( void )
{
	wave.max_freq	=	 6;
	
	float	dw		=	wave.max_freq / (float)WAVE_BAND_NUM;

	wave.frequency[0]	=	0;
	wave.amplitudes[0]	=	0;
	wave.phases[0]		=	0;
	wave.wave_num[0]	=	0;
	
	for (uint i=1; i<WAVE_BAND_NUM; i++) {
	
		float	w	=	dw * i;
	
		wave.frequency[i]	=	w;
		wave.amplitudes[i]	=	sqrt(2* SpectrumPM(w) * dw);
		wave.phases[i]		=	FRand(0, 2 * PI);
		wave.wave_num[i]	=	w * w / GRAVITY;
	}
}


/*-----------------------------------------------------------------------------
	Spectral runtime stuff :
-----------------------------------------------------------------------------*/

//
//	EWaving::Update
//
void EWaving::Update( float dtime )
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
//	EWaving::GetWave
//	returns wave attributes in a point
//
point_wave_s EWaving::GetWave( float x, float y, float depth, float time )  const
{
	point_wave_s	pw;
	pw.offset	=	0;
	pw.pressure	=	depth * WATER_DENSITY * GRAVITY;

	//	compute vertical offset :	
	for (uint i=0; i<WAVE_BAND_NUM; i++) {
		float	amp		=	wave.amplitudes[i];
		float	freq	=	wave.frequency[i];
		float	phase	=	wave.phases[i];
		float	k		=	wave.wave_num[i];
		float	fade	=	(depth<0) ? 1 : exp( - k * depth );
		pw.offset	+=	fade * amp * cos(freq * time + k * x + phase);
	}
	
	//	compute pressure offset :	
	pw.pressure	 +=	pw.offset * GRAVITY * WATER_DENSITY;
	
	if ((-depth) > pw.offset) {
		pw.pressure = 0;
	}
	
	return pw;
}


//
//	EWaving::GetPressure
//
float EWaving::GetPressure( const EVec4 &pos ) const
{
	point_wave_s	pw	=	GetWave(pos.x, pos.y, -pos.z, time);
	
	return pw.pressure;
}


//
//	EWaving::GetPositionAtTime
//
EVec4 EWaving::GetPositionAtTime( const EVec4 &init_pos, float time ) const
{
	point_wave_s	pw	=	GetWave(init_pos.x, init_pos.y, -init_pos.z, time);
	
	return EVec4( init_pos.x, init_pos.y, init_pos.z + pw.offset, 1 );
}


//
//	EWaving::GetWaveSlopeX
//
float EWaving::GetWaveSlopeX( const EVec4 &init_pos ) const
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
//	EWaving::GetPosition
//
EVec4 EWaving::GetPosition( const EVec4 &init_pos ) const
{
	return GetPositionAtTime( init_pos, time );
}


//
//	EWaving::GetVelocity
//
EVec4 EWaving::GetVelocity( const EVec4 &init_pos ) const
{
	return EVec4(0,0,0,0);
}

