EMatrix4 GetRangeMatrix( const EMatrix4 &iVP, const EMatrix4 &newVP, const EPlane &water, float wave_h )
{
	//	make bound planes :
	EPlane upper = water.normalize();
	EPlane lower = water.normalize();

	upper.d -= wave_h;
	lower.d += wave_h;
	
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

		//rs()->GetDVScene()->DrawPoint( buffer[i], 0.1, EVec4(0,0,1,1.0) );
		
		EVec4 tp = Matrix4Transform( buffer[i], newVP );
		tp /= tp.w;
		bbox.Expand( tp );
	}
	
	float xmin = bbox.Min().x;
	float xmax = bbox.Max().x;
	float ymin = bbox.Min().y;
	float ymax = bbox.Max().y;
	
	//LOGF("X:[%f %f] Y:[%f %f]", xmin, xmax, ymin, ymax);
	
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

	//r_sky->SetFlag( RSE_HIDDEN );

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
		
		//rs()->GetDVScene()->DrawPoint( p, 2.0f, EVec4(1,0,0,1.0));
		
		//	elevate camera :
		float c_p_dist	=	Vec3Length( EVec3(center.x, center.y, 0) - EVec3(p.x, p.y, 0) );
		float elev		=	h * c_p_dist / 2 / zf;
		
		elev			=	clamp<float>( elev, cam_h, 100000 );
		
		newV			=	Matrix4LookAtRH( EVec3(center.x, center.y, elev), EVec3(p.x, p.y, p.z), EVec3(0,0,1) );
		newP			=	P;
		newVP			=	newV * newP;
		newiVP			=	Matrix4Inverse( newV * newP );
		
	} while (0);
	
	EMatrix4	range	=	GetRangeMatrix( iVP, newVP, water, 10 );

	//
	//	Make grid :
	//
	IPxTriMesh	mesh	=	sea_mesh->Clone();
	uint n = mesh->GetVertexNum();
	
	for (uint i=0; i<n; i++) {
		EVertex	v;
		
		v			=	mesh->GetVertex( i );
		float x		=	v.position.x*0.5;
		float y		=	v.position.y*0.5;
		
		EVec4 r0	=	Matrix4Transform( EVec4(x,y,-1,1), range * newiVP );
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
		
		
		//
		//	write vertex :
		//		
		v.position.x	=	p.x;
		v.position.y	=	p.y;
		v.position.z	=	GetPosition( Vec3ToVec4(v.position) ).z;
		
		float	dist	=	Vec3Length( EVec3(p.x, p.y, 0) - Vec4ToVec3( view_pos ) );
				dist	=	clamp<float>( dist, 0, VIEW_FAR);
		
		float	wave_factor	=	pow(1 - abs(dist/VIEW_FAR), 0.25f);

		v.position.z *=	wave_factor;
		
		mesh->SetVertex( i, v );
	}
	
	
	
	//
	//	post process mesh :
	//
	mesh->ComputeNormals();

	for (uint i=0; i<n; i++) {
		EVertex	v	=	mesh->GetVertex( i );
		v.uv0.x		=	v.position.x/4;
		v.uv0.y		=	v.position.y/4;
		
		float	r	=	v.normal.z;
		float	g	=	v.position.z;
		v.color0	=	EVec4(r,g,0,1);
		
		mesh->SetVertex( i, v );
	}
	
	r_ent->SetMesh( mesh );
	//r_ent->SetFlag( RSE_HIDDEN );  
}
