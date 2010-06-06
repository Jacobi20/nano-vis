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

#include <NxPhysics.h>

/*-----------------------------------------------------------------------------
	Nano vis :
-----------------------------------------------------------------------------*/

//
//	ESciVis::InitPhysX
//
void ESciVis::InitPhysX( void )
{
	LOG_INIT("PhysX");
	
	// Create the physics SDK
    nx = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
    if (!nx)  return;

	// Set the physics parameters
	nx->setParameter(NX_SKIN_WIDTH, 0.01f);

	// Set the debug visualization parameters
	nx->setParameter(NX_VISUALIZATION_SCALE, 1);
	nx->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	nx->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

    // Create the scene
    NxSceneDesc sceneDesc;
    sceneDesc.gravity               = NxVec3(0,0,-9.8f);
	sceneDesc.simType				= NX_SIMULATION_SW;
    nx_scene = nx->createScene(sceneDesc);	
	
	if(!nx_scene) { 
		sceneDesc.simType				= NX_SIMULATION_SW; 
		nx_scene = nx->createScene(sceneDesc);  
		if(!nx_scene) return;
	}

	// Create the default material
	NxMaterial* defaultMaterial = nx_scene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);
}


//
//	ESciVis::ShutdownPhysX
//
void ESciVis::ShutdownPhysX( void )
{
	LOG_SHUTDOWN("PhysX");

    if (nx_scene) {
		nx->releaseScene(*nx_scene);
	}
	if (nx)  nx->release();
}


//
//	ESciVis::FramePhysX
//
void ESciVis::FramePhysX( float dtime )
{
    nx_scene->simulate( dtime );
	nx_scene->flushStream();

	bool r = nx_scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
}


//
//
//
NxActor	* ESciVis::CreatePhysBox( float sx, float sy, float sz, const EVec4 &pos )
{
	NxActorDesc actorDesc;
	NxBodyDesc	bodyDesc;

	//
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions	= NxVec3(sx/2, sy/2, sz/2);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.density		= 1.0f;
	actorDesc.body			= &bodyDesc;
	actorDesc.globalPose.t	= NxVec3(pos.x, pos.y, pos.z);

	NxActor *pActor = nx_scene->createActor(actorDesc);
	assert(pActor);

	return pActor;
}



static EVec4	Bytes2Color(uint bytes) 
{
	EVec4 c;
	c.x	=	((bytes >> 24) & 0xFF) / 255.0f;
	c.y	=	((bytes >> 16) & 0xFF) / 255.0f;
	c.z	=	((bytes >>  8) & 0xFF) / 255.0f;
	c.w	=	((bytes >>  0) & 0xFF) / 255.0f;
	return c;
}

//
//	ESciVis::DebugPhysX
//
void ESciVis::DebugPhysX( const D3DXMATRIX &w, const D3DXMATRIX &v, const D3DXMATRIX &p )
{
	IDirect3DVertexDeclaration9 *line_decl	=	NULL;
	HRCALL( d3ddev->CreateVertexDeclaration( VERTEX_DECL_STATIC, &line_decl ) );
	HRCALL( d3ddev->SetVertexDeclaration( line_decl ) );

	const NxDebugRenderable *dbg_rend = nx_scene->getDebugRenderable();

	uint n;
	HRCALL( shader_fx->SetTechnique("solid_body") );
	HRCALL( shader_fx->Begin(&n, 0) );
	
	for (uint pass=0; pass<n; pass++) {
	
		HRCALL( shader_fx->BeginPass(pass) );

		NxU32 num_lines				= dbg_rend->getNbLines();            
		const NxDebugLine* lines	= dbg_rend->getLines();            
		
		while (num_lines--) {                
				
			vertex_s verts[2];
			
			verts[0].normal	=	EVec3(0,0,0);
			verts[0].pos	=	EVec3(lines->p0.x, lines->p0.y, lines->p0.z);
			verts[0].color	=	Bytes2Color(lines->color);
			verts[0].uv		=	EVec2(0,0);
			verts[1].normal	=	EVec3(0,0,0);
			verts[1].pos	=	EVec3(lines->p1.x, lines->p1.y, lines->p1.z);
			verts[1].color	=	Bytes2Color(lines->color);
			verts[1].uv		=	EVec2(0,0);
			
			HRCALL( d3ddev->DrawPrimitiveUP(D3DPT_LINELIST, 1, &verts, sizeof(vertex_s)) );
			
			lines++;            
		}            
		
		HRCALL( shader_fx->EndPass() );
	}

	HRCALL( shader_fx->End() );
	
	SAFE_RELEASE( line_decl );
}