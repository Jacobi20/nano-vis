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
#include "ship.h"
 
/*-----------------------------------------------------------------------------
	Naive ship simulator :
-----------------------------------------------------------------------------*/

EVec3 GetCenterOfBuyoancy( const IPxTriMesh mesh ) 
{
	EVec3	center			=	EVec3(0,0,0);
	float	total_weight	=	0;
	
	for (uint i=0; i<mesh->GetTriangleNum(); i++) {
		uint i0, i1, i2;
		mesh->GetTriangle( i, i0, i1, i2 );
		
		//	vertices :		
		EVec3	v0	=	mesh->GetVertex( i0 ).position;
		EVec3	v1	=	mesh->GetVertex( i1 ).position;
		EVec3	v2	=	mesh->GetVertex( i2 ).position;

		//	area unit weights :
		float	w0	=	GRAVITY * WATER_DENSITY * (-v0.z);
		float	w1	=	GRAVITY * WATER_DENSITY * (-v1.z);
		float	w2	=	GRAVITY * WATER_DENSITY * (-v2.z);
		float	w	=	(w0 + w1 + w2) / 3.0f;
		
		float	cu	=	(w0 + 2*w1 + w2) / 4 / (w0 + w1 + w2);
		float	cv	=	(w0 + w1 + 2*w2) / 4 / (w0 + w1 + w2);
		EVec3	cm	=	(v1 - v0) * cu + (v2 - v0) * cv + v0;
				cm.z/=	2;

		//	section area :
		float	s		=	mesh->TriangleArea( i );
		EVec3	n		=	mesh->TriangleNormal( i );
		float	sa		=	mesh->TriangleArea( i ) * abs(n.z);
		float	weight	=	w * sa;
		
		if (n.z>0) {
			weight *= (-1);
		}
		
		center			+=	(cm * weight);
		total_weight	+=	weight;
	}	
	
	center /= total_weight;

	return center;	
}


IPxTriMesh EShip::GetSubmergedMesh( const EMatrix4 &world, const EPlane &plane )
{
	IPxTriMesh bottom, top, full;
	bottom	= Geometry()->CreateTriMesh(GE_MESH_POSITION);
	top		= Geometry()->CreateTriMesh(GE_MESH_POSITION);
	
	full	= this->mesh_hsf->Clone();
	
	full->Transform( world );
	
	full->Split( plane, bottom, top );
	
	return bottom;
}

