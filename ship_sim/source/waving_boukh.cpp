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

const uint	WAVE_BAND_NUM			=	10;
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
		
		mutable wave_sample_s	wave_grid[WAVE_GRID_SIZE][WAVE_GRID_SIZE];
		
		virtual void	InitWaving			( bool new_phases );
		point_wave_s	GetWave				( float x, float y, float depth, float time ) const;
		
		float			SpectrumPM			( float w );

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
	u_wind	=	0;
	time	=	0;

	sea_mesh	=	ge()->LoadMeshFromFile("sea.esx|sea");
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

class EGrid {
	public:
				EGrid		( void );
				~EGrid		( void );

		void	ComputeWeights	( const EFrustum &frustum );
		void	RelaxGrid		( void );
		//void	GetMesh
				
		EVec3	grid[64][64];
	};


EGrid::EGrid( void )
{
	for (uint i=0; i<64; i++) {
		for (uint j=0; j<64; j++) {
			grid[i][j].x	=	2.0f * ((float)j / 64.0f) - 1.0f;
			grid[i][j].y	=	2.0f * ((float)i / 64.0f) - 1.0f;
		}
	}
}


EGrid::~EGrid( void )
{
}


void EGrid::ComputeWeights( const EFrustum &frustum )
{
}


EMatrix4 GetRangeMatrix( const EMatrix4 &iVP, const EMatrix4 &newVP, const EPlane &water, float wave_h )
{
	//	make bound planes :
	EPlane upper = water.normalize();
	EPlane lower = water.normalize();

	//upper.d -= wave_h;
	//lower.d += wave_h;
	
	vector<EVec4>	buffer;

	//	transform frustum conrners to world space :
	float	u	=	1;
	EVec4	v0	=	Matrix4Transform( EVec4( u, u, u, 1), iVP );	v0	/=	v0.w;
	EVec4	v1	=	Matrix4Transform( EVec4(-u, u, u, 1), iVP );	v1	/=	v1.w;
	EVec4	v2	=	Matrix4Transform( EVec4(-u,-u, u, 1), iVP );	v2	/=	v2.w;
	EVec4	v3	=	Matrix4Transform( EVec4( u,-u, u, 1), iVP );	v3	/=	v3.w;
	EVec4	v4	=	Matrix4Transform( EVec4( u, u,-u, 1), iVP );	v4	/=	v4.w;
	EVec4	v5	=	Matrix4Transform( EVec4(-u, u,-u, 1), iVP );	v5	/=	v5.w;
	EVec4	v6	=	Matrix4Transform( EVec4(-u,-u,-u, 1), iVP );	v6	/=	v6.w;
	EVec4	v7	=	Matrix4Transform( EVec4( u,-u,-u, 1), iVP );	v7	/=	v7.w;
	
	rs()->GetDVScene()->DrawPoint( v0, 0.1f, EVec4(1,1,0,0.5) );
	rs()->GetDVScene()->DrawPoint( v1, 0.1f, EVec4(1,1,0,0.5) );
	rs()->GetDVScene()->DrawPoint( v2, 0.1f, EVec4(1,1,0,0.5) );
	rs()->GetDVScene()->DrawPoint( v3, 0.1f, EVec4(1,1,0,0.5) );
	rs()->GetDVScene()->DrawPoint( v4, 0.1f, EVec4(1,0,1,1.0) );
	rs()->GetDVScene()->DrawPoint( v5, 0.1f, EVec4(1,0,1,1.0) );
	rs()->GetDVScene()->DrawPoint( v6, 0.1f, EVec4(1,0,1,1.0) );
	rs()->GetDVScene()->DrawPoint( v7, 0.1f, EVec4(1,0,1,1.0) );

	struct segment_s {
		void fromVec4( const EVec4 &a, const EVec4 &b ) {
			this->a	=	Vec4ToVec3( a );
			this->b	=	Vec4ToVec3( b );
		}
		EVec3 a, b;
	};
	
	segment_s s[12];
	
	s[ 0].fromVec4(	v0,	v1 );	s[ 1].fromVec4(	v1,	v2 );
	s[ 2].fromVec4(	v2,	v3 );	s[ 3].fromVec4(	v3,	v0 );
	
	s[ 4].fromVec4(	v4,	v5 );	s[ 5].fromVec4(	v5,	v6 );
	s[ 6].fromVec4(	v6,	v7 );	s[ 7].fromVec4(	v7,	v4 );
	
	s[ 8].fromVec4(	v0,	v4 );	s[ 9].fromVec4(	v1,	v5 );
	s[10].fromVec4(	v2,	v6 );	s[11].fromVec4(	v3,	v7 );
	
	EVec3	corners[8] = { 
		Vec4ToVec3( v0 ), 	Vec4ToVec3( v1 ), 	Vec4ToVec3( v2 ), 
		Vec4ToVec3( v3 ), 	Vec4ToVec3( v4 ), 	Vec4ToVec3( v5 ), 
		Vec4ToVec3( v6 ), 	Vec4ToVec3( v7 )
	};
	
	
	//	add intersections :
	for (uint i=0; i<12; i++) {
		EVec3 a = s[i].a;
		EVec3 b = s[i].b;
		
		float frac;
		
		if ( upper.traceSegment( a, b, frac ) ) {
			EVec3 p	=	a + (b-a) * frac;
			buffer.push_back( Vec3ToPoint4( p ) );
		}
		
		if ( lower.traceSegment( a, b, frac ) ) {
			EVec3 p	=	a + (b-a) * frac;
			buffer.push_back( Vec3ToPoint4( p ) );
		}
	}
	
	//	add corners :
	for (uint i=0; i<8; i++) {
		if ( upper.classifyPoint( corners[i], 0 )<=0 && lower.classifyPoint( corners[i], 0 )>=0 ) {
			buffer.push_back( Vec3ToPoint4( corners[i] ) );
		}
	}
	
	EBBox bbox;
	bbox.MakeSingular();
	
	if (buffer.empty()) {
		return Matrix4Identity();
	}
	
	for (uint i=0; i<buffer.size(); i++) {
		buffer[i].z = 0;
		buffer[i].w = 1;

		rs()->GetDVScene()->DrawPoint( buffer[i], 0.1, EVec4(0,0,1,1.0) );
		
		EVec4 tp = Matrix4Transform( buffer[i], newVP );
		tp /= tp.w;
		bbox.Expand( tp );
	}
	
	float xmin = bbox.Min().x;
	float xmax = bbox.Max().x;
	float ymin = bbox.Min().y;
	float ymax = bbox.Max().y;
	
	LOGF("X:[%f %f] Y:[%f %f]", xmin, xmax, ymin, ymax);
	
	EMatrix4	m2 (
			xmax - xmin,		0,				0,		0,	
				0,			ymax - ymin,		0,		0,
				0,				0,				1,		0,
			(xmin+xmax)/2,	(ymin+ymax)/2,		0,		1	
		);	//*/
		
	return m2;
}


//
//	EWaving::Update
//
void EWaving::Update( float dtime, const EVec4 &view_pos, const EQuat &orient )
{
	time	+=	dtime;

	uint sw, sh;
	rs()->GetScreenSize(sw, sh);
	float aspect = (float)sw / (float)sh;

	r_sky->SetPose( view_pos, QuatIdentity() );

	EFrustum	Fr		=	ESciVis::self->view.frustum;
	float		w		=	Fr.getWidth();
	float		h		=	Fr.getHeight();
	float		zn		=	Fr.getZNear();
	float		zf		=	Fr.getZFar();

	EMatrix4	T		=	Matrix4Translate( -ESciVis::self->view.position);
	EMatrix4	R		=	QuatToMatrix( QuatInverse( ESciVis::self->view.orient ) );
	EMatrix4	V		=	T * R;
	EMatrix4	P		=	Matrix4PerspectiveRH( zn/zf*w, zn/zf*h, zn, zf );
	EPlane		water(0,0,1,0);
	
	EMatrix4	VP		=	V * P;
	EMatrix4	iVP		=	Matrix4Inverse( V * P );
	EMatrix4	newiVP;
	EMatrix4	newVP;
	EMatrix4	newV;
	EMatrix4	newP;
	
	
	//
	//	Make projector matrix :
	//
	do {
	
		EVec4	center	=	ESciVis::self->view.position;
		float	cam_h	=	center.z;
		center.z		=	0;

		//	find view ray-water intersection point :
		EVec4 r0	=	Matrix4Transform( EVec4(0,0, 0,1), iVP );
		EVec4 r1	=	Matrix4Transform( EVec4(0,0, 1,1), iVP );
		r0 /= r0.w;
		r1 /= r1.w;
		EVec4 p;		
		EVec4 o		= r0;
		//EVec4 r	= Vec4Normalize( r1 - r0 );
		EVec4 r		= r1 - r0;
		
		float t		= PlaneTraceRayAgainstPlane(p, o, r, water );
		
		const float RANGE = 0.1f;
		
		if ( t < 0 ) {
			t = RANGE;
		}
		if ( t > RANGE ) {
			t = RANGE;
		}
		
		p = o + r * t;
		p.z = 0;
		
		rs()->GetDVScene()->DrawPoint( p, 2.0f, EVec4(1,0,0,1.0));
		
		//	elevate camera :
		float c_p_dist	=	Vec3Length( EVec3(center.x, center.y, 0) - EVec3(p.x, p.y, 0) );
		float elev		=	h * c_p_dist / 2 / zf;
		
		elev			=	clamp<float>( elev, cam_h, 100000 );
		
		newV			=	Matrix4LookAtRH( EVec3(center.x, center.y, elev), EVec3(p.x, p.y, p.z), EVec3(0,0,1) );
		newP			=	P;
		newVP			=	newV * newP;
		newiVP			=	Matrix4Inverse( newV * newP );
		
	} while (0);
	
	EMatrix4	range	=	GetRangeMatrix( iVP, newVP, water, 0 );

	//
	//	Make grid :
	//
	IPxTriMesh	mesh	=	sea_mesh->Clone();
	uint n = mesh->GetVertexNum();
	
	for (uint i=0; i<n; i++) {
		EVertex	v;
		
		v			=	mesh->GetVertex( i );
		float x		=	v.position.x*1.0;
		float y		=	v.position.y*1.0;
		
		EVec4 r0	=	Matrix4Transform( EVec4(x,y, 0,1), range * newiVP );
		EVec4 r1	=	Matrix4Transform( EVec4(x,y, 1,1), range * newiVP );
		r0 /= r0.w;
		r1 /= r1.w;
		

		EVec4 p;		
		EVec4 o = r0;
		EVec4 r = r1 - r0;
		float t = PlaneTraceRayAgainstPlane(p, o, r, water );
		if (t>1) { t = 1; }
		if (t<0) { t = 1; }
		
		p	=	o + r*t;
		
		//rs()->GetDVScene()->DrawPoint( p, 0.1f, EVec4(0,1,0,0.5));

		//
		//	write vertex :
		//		
		v.position.x	=	p.x;
		v.position.y	=	p.y;
		v.position.z	=	GetPosition( Vec3ToVec4(v.position) ).z;

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
		
			float x2 = x + y * (i%5)/10.0;

			register float	amp		=	wave.waves[i].amplitude;
			register float	freq	=	wave.waves[i].frequency;
			register float	phase	=	wave.waves[i].phase;
			register float	k		=	wave.waves[i].wave_num;

			float	fade	=	(depth<0) ? 1 : exp( - k * depth );
			
			pw.offset	+=	fade * amp * cos(freq * time + k * x2 + phase);
		}
	} else {
		float	w	=	sin_wave_w;
		float	k	=	w * w / GRAVITY;
		pw.offset	=	sin_wave_a * sin(w * time + k * x);
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

