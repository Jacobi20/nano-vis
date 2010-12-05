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
const uint	COS_TABLE_SIZE			=	1024;
const uint	WAVE_BAND_NUM			=	256;
const uint	WAVE_GRID_SIZE			=	400;
const float WAVE_GRID_OFFSET_X		=	-200.0f;
const float WAVE_GRID_OFFSET_Y		=	-200.0f;
const uint	WAVE_DEPTH_OF_SILENCE	=	50;			// there are no waves

struct point_wave_s {
		float	offset;
		float	pressure;
	};
	
struct wave_s {
		float	amplitude;
		float	frequency;
		float	phase;
		float	wave_num;
	};
	
struct wave_sample_s {
		bool	dirty;							//	is sample dirty
		float	zoffset;						//	vertical offset
	};	


class EWaving : public IWaving {
	public:
							EWaving		( lua_State *L, int idx );
							~EWaving		( void );
											
		virtual void		Update				( float dtime, const EVec4 &view_pos, const EQuat &orient );
		virtual	EVec4		GetVelocity			( const EVec4 &init_pos ) const;
		virtual	EVec4		GetPosition			( const EVec4 &init_pos ) const;
		virtual float		GetPressure			( const EVec4 &init_pos ) const;
		virtual float		GetWaveSlopeX		( const EVec4 &init_pos ) const;
		virtual void		MakeDirty			( void );
		virtual void		SetWindSpeed		( float u );
		virtual void		SetSinWave			( float w, float a );
		
	protected:
		bool			still;
		
		float			u_wind;

		bool			sin_wave;
		float			sin_wave_w;
		float			sin_wave_a;		
		float			time;

		IPxFREntity		r_ent;
		IPxTriMesh		sea_mesh;
		
		IPxFREntity		r_sky;
		IPxTriMesh		sky_mesh;

		struct {		
			float	max_freq;					//	angular, 'omega'
			wave_s	waves[WAVE_BAND_NUM];
			float	amplitudes[WAVE_BAND_NUM];	//	amplitudes
			float	frequency[WAVE_BAND_NUM];	//	i * max_freq / BAND_NUM
			float	phases[WAVE_BAND_NUM];		//	rand(2*Pi)
			float	wave_num[WAVE_BAND_NUM];	//	k
		} wave;
		
		float	cos_table[COS_TABLE_SIZE];
		
		inline	float	FastCos			( float x ) const;
		void			InitFastCos		( void );
		
		mutable wave_sample_s	wave_grid[WAVE_GRID_SIZE][WAVE_GRID_SIZE];
		
		virtual void	InitWaving			( bool new_phases );
		point_wave_s	GetWave				( float x, float y, float depth, float time ) const;
		
		float			SpectrumPM			( float w );
		
		float			GetWaveFast			( float x, float y, float time ) const;

		EVec4			GetPositionAtTime	( const EVec4 &init_pos, float time ) const;
	};

IWaving	*create_waving(lua_State *L, int idx) { return new EWaving(L, idx); }

/*-----------------------------------------------------------------------------
	Implementation :
-----------------------------------------------------------------------------*/

#define USE_SS_GRID

//
//
//
EWaving::EWaving( lua_State *L, int idx )
{
	InitFastCos();

	u_wind	=	0;
	time	=	0;

	#ifdef USE_SS_GRID
		//sea_mesh	=	ge()->LoadMeshFromFile("sea.esx|sea_ss");
		sea_mesh	=	ge()->CreatePlane( 2, 2, 40, 80 );
		EShadingGroup	sg;
		sg.start	= 0;
		sg.num		= sea_mesh->GetTriangleNum();
		sg.shader	= "textures/water.tga";
		sea_mesh->ComputeTangents();
		sea_mesh->SetSGNum(1);
		sea_mesh->SetSG(0, sg);
		
	#else
		sea_mesh	=	ge()->LoadMeshFromFile("sea.esx|sea");
	#endif
	sky_mesh	=	ge()->LoadMeshFromFile("sky.esx|sky");
	
	r_ent		=	sci_vis->GetFRScene()->AddEntity();
	r_ent->SetMesh( sea_mesh );
	
	r_sky		=	sci_vis->GetFRScene()->AddEntity();
	r_sky->SetMesh( sky_mesh );
	//r_sky->SetFlag( RSE_HIDDEN );
	
	InitWaving(true);
}


//
//	EWaving::~EWaving
//
EWaving::~EWaving( void )
{
	sci_vis->GetFRScene()->RemoveEntity( r_ent );
}



void EWaving::MakeDirty( void )
{
	for (uint i=0; i<WAVE_GRID_SIZE; i++) {
		for (uint j=0; j<WAVE_GRID_SIZE; j++) {
			wave_grid[i][j].dirty	=	true;
		}
	}
}

/*-----------------------------------------------------------------------------
	Spectral stuff :
-----------------------------------------------------------------------------*/


void EWaving::SetWindSpeed( float u )
{
	time		=	0;
	sin_wave	=	false;
	u_wind		=	u;
	
	InitWaving(false);
}


void EWaving::SetSinWave( float w, float a )
{
	time		=	0;
	sin_wave	=	true;
	sin_wave_w	=	w;
	sin_wave_a	=	a;
	
	still = false;
}


float EWaving::SpectrumPM(float w)
{
	//	Pierson-Moskowitz :
	//float	Asp	=	10.0f;
	//float	Bsp	=	1.30f;
	//return	Asp * expf(-Bsp / (w*w*w*w)) / (w*w*w*w*w);
	
	//	Lopatuhin, (60)
	float	U	=	u_wind;
	float	g	=	GRAVITY;
	return 0.0081 * (g*g) * powf(w, -5) * exp( -0.74 * pow(w*U/g, -4) );
}

//
//	EWaving::SetupWaving
//
void EWaving::InitWaving( bool new_phases )
{
	wave.max_freq	=	 4.0;
	
	float	dw		=	wave.max_freq / (float)WAVE_BAND_NUM;

	wave.frequency[0]	=	0;
	wave.amplitudes[0]	=	0;
	wave.phases[0]		=	0;
	wave.wave_num[0]	=	0;

	wave.waves[0].frequency	=	0;
	wave.waves[0].amplitude	=	0;
	wave.waves[0].phase		=	0;
	wave.waves[0].wave_num	=	0;
	
	for (uint i=1; i<WAVE_BAND_NUM; i++) {
	
		float	w	=	dw * i;

		wave.amplitudes[i]		=	sqrt(2* SpectrumPM(w) * dw);
		wave.waves[i].amplitude	=	sqrt(2 * SpectrumPM(w) * dw);
		
		if (new_phases) {	
			wave.frequency[i]	=	w;
			wave.phases[i]		=	vmath::randf(0, 2 * PI);
			wave.wave_num[i]	=	w * w / GRAVITY;

			wave.waves[i].frequency	=	w;
			wave.waves[i].phase		=	wave.phases[i];
			wave.waves[i].wave_num	=	w * w / GRAVITY;
		}
	}
	
	still = true;
	for (uint i=1; i<WAVE_BAND_NUM; i++) {
		if (abs(wave.waves[i].amplitude) > 0.00001f ) {
			still = false;
		}
	}
}


/*-----------------------------------------------------------------------------
	Spectral runtime stuff :
-----------------------------------------------------------------------------*/

#ifndef USE_SS_GRID
//
//	EWaving::Update
//
void EWaving::Update( float dtime, const EVec4 &_view_pos, const EQuat &orient )
{
	time	+=	dtime;

	r_sky->SetPose( _view_pos, QuatIdentity() );
	r_sky->SetFlag( RSE_HIDDEN );
	
	EFrustum fr	=	ESciVis::self->view.frustum;
	
	EVec4	view_pos	=	_view_pos;

	float	elevation		=	sqrt(1+abs(view_pos.z));
	float	view_area_scale	=	elevation * 100;
	float	step			=	1.0f / 64.0f * view_area_scale;

	view_pos.x	=	floor( view_pos.x / step ) * step;
	view_pos.y	=	floor( view_pos.y / step ) * step;

	//
	//	Make grid :
	//
	IPxTriMesh	mesh	=	sea_mesh->Clone();
	uint n = mesh->GetVertexNum();
	
	for (uint i=0; i<n; i++) {
		EVertex	v;
		
		float	wave_factor		=	1;
		
		v			=	mesh->GetVertex( i );
		float gx	=	v.position.x;
		float gy	=	v.position.y;
		
		float x		=	gx * view_area_scale + view_pos.x;
		float y		=	gy * view_area_scale + view_pos.y;
		
		wave_factor	=	clamp<float>(1 - (gx*gx + gy*gy), 0, 1);
		wave_factor	=	pow( wave_factor, 2 );
		
		//
		//	write vertex :
		//		
		v.position.x	=	x;
		v.position.y	=	y;
		v.position.z	=	0;
		
		float	dist	=	Vec3Length( v.position - Vec4ToVec3(view_pos) );

		v.position.z	=	GetPosition( Vec3ToVec4(v.position) ).z * wave_factor;
		/*if (dist < view_area_scale*0.1 || fr.IsPointInside( EVec4(x,y,0,1), 0.1 * dist )) {
		}  */

		mesh->SetVertex( i, v );
	}
	
	//
	//	post process mesh :
	//
	for (uint i=0; i<n; i++) {
		EVertex	v	=	mesh->GetVertex( i );
		v.uv0.x		=	v.position.x/4;
		v.uv0.y		=	v.position.y/4;
		mesh->SetVertex( i, v );
	}
	
	r_ent->SetMesh( mesh );
	//r_ent->SetFlag( RSE_HIDDEN );  
}
#else
	#include "water_grid_ss.h"
#endif



void EWaving::InitFastCos( void )
{
	for (uint i=0; i<COS_TABLE_SIZE; i++) {
		cos_table[i] = cos( 2*PI * ((float)i)/(float)COS_TABLE_SIZE );
	}
}


float EWaving::FastCos( float x )  const
{
	uint b = (uint)(x / 2/PI * (float)COS_TABLE_SIZE);
	b &= (COS_TABLE_SIZE-1);
	return cos_table[b];
}


inline float	EWaving::GetWaveFast( float x, float y, float time ) const
{
	float wave_h = 0;
	
	for (uint i=0; i<WAVE_BAND_NUM; i++) {
	
		float x2 = x;

		float	amp		=	wave.waves[i].amplitude;
		float	freq	=	wave.waves[i].frequency;
		float	phase	=	wave.waves[i].phase;
		float	k		=	wave.waves[i].wave_num;

		wave_h	+=	amp * FastCos(freq * time + k * x2 + phase);
	}
	return wave_h;
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

	if (still) {
		if (depth<0) { 
			pw.pressure = 0;
		}
		return pw;
	}
	
	//	compute vertical offset :	
	if (!sin_wave) {
		for (uint i=0; i<WAVE_BAND_NUM; i++) {
		
			float x2 = x;

			register float	amp		=	wave.waves[i].amplitude;
			register float	freq	=	wave.waves[i].frequency;
			register float	phase	=	wave.waves[i].phase;
			register float	k		=	wave.waves[i].wave_num;

			float	fade	=	(depth<0) ? 1 : exp( - k * depth );
			
			pw.offset	+=	fade * amp * FastCos(freq * time + k * x2 + phase);
		}
	} else {
		float	w	=	sin_wave_w;
		float	k	=	w * w / GRAVITY;
		pw.offset	=	sin_wave_a * sin(w * time + k * x);
	}
	
	
	//pw.offset	*=	pw.offset;
	//pw.offset	/=	4.0f;
	
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
	float gx	=	pos.x - WAVE_GRID_OFFSET_X;
	float gy	=	pos.y - WAVE_GRID_OFFSET_Y;
	int ix		=	(int)floor(gx);	//	integer part
	int iy		=	(int)floor(gy);	//	integer part
	float fx	=	gx - floor(gx);	//	fractional part
	float fy	=	gy - floor(gy);	//	fractional part
	float cx	=	floor(pos.x) + 0.5;
	float cy	=	floor(pos.y) + 0.5;
	
	ix	=	clamp<int>(ix, 0, WAVE_GRID_SIZE-1);	
	iy	=	clamp<int>(iy, 0, WAVE_GRID_SIZE-1);	

	point_wave_s	pw	=	GetWave( pos.x, pos.y, -pos.z, time );
	return pw.pressure;
	
#if 0	
	if (wave_grid[ix][iy].dirty) {
		wave_sample_s	*ws =	&wave_grid[ix][iy];
	
		point_wave_s	pw	=	GetWave( cx, cy, 0, time );
		ws->zoffset			=	pw.offset;
		ws->dirty			=	false;
		
		rs()->GetDVScene()->DrawPoint( EVec4(cx, cy, pw.offset, 1), 0.33f, EVec4(0,0,1,1) );
	}

	float zoffset	= wave_grid[ix][iy].zoffset;
	float depth		= -pos.z;
	
	if (pos.z>zoffset) {
		return 0;
	} else {
		float	ps		=	(depth) * WATER_DENSITY * GRAVITY;
		float	fade	=	(depth<0) ? 1 : exp( - 0.5f * depth );	//	'great' approximation
		float	pd		=	fade * zoffset * WATER_DENSITY * GRAVITY;
		return	ps + pd;
	}
#endif	

	return 0;
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

