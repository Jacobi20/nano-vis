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

#if 0
#include <NxPhysics.h>
#include <NxCooking.h>

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
    sceneDesc.gravity               = NxVec3(0,0,0);
	sceneDesc.simType				= NX_SIMULATION_SW;
    nx_scene = nx->createScene(sceneDesc);	
	
	if(!nx_scene) { 
		sceneDesc.simType				= NX_SIMULATION_SW; 
		nx_scene = nx->createScene(sceneDesc);  
		if(!nx_scene) return;
	}
	
	//	coocking lib :
	nx_cook	=	NxGetCookingLib(NX_PHYSICS_SDK_VERSION);
	if (!nx_cook) {
		RUNTIME_ERROR("NxGetCookingLib() failed");
	}
	nx_cook->NxInitCooking();
	

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

	if (nx_cook) {
		nx_cook->NxCloseCooking();
		nx_cook = NULL;
	}

    if (nx_scene) {
		nx->releaseScene(*nx_scene);
		nx_scene = NULL;
	}
	
	if (nx)  {
		nx->release();
		nx = NULL;
	}
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
//	ESciVis::CreatePhysBox
//
NxActor	* ESciVis::CreatePhysBox( float sx, float sy, float sz, const EVec4 &pos, const EQuat &orient, float mass )
{
	NxActorDesc actorDesc;
	NxBodyDesc	bodyDesc;
	
	NxQuat	q;
	q.x	= orient.x;
	q.y	= orient.y;
	q.z	= orient.z;
	q.w	= orient.w;

	//
	NxBoxShapeDesc boxDesc;
	boxDesc.dimensions	= NxVec3(sx/2, sy/2, sz/2);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body			= &bodyDesc;
	actorDesc.globalPose.t	= NxVec3(pos.x, pos.y, pos.z);
	actorDesc.globalPose.M	= q;
	
	bodyDesc.mass			= mass;

	NxActor *pActor = nx_scene->createActor(actorDesc);
	ASSERT(pActor);

	return pActor;
}


//
//	ESciVis::CreatePhysMesh
//
NxActor *ESciVis::CreatePhysMesh( IPxTriMesh mesh, const EVec4 &pos, const EQuat &orient, float mass )
{
	NxVec3	p	=	ToNxVec3( EVec3(pos.x, pos.y, pos.z) );
	NxQuat	q	=	ToNxQuat( orient );
	
	//	actor descriptor
	NxActorDesc actor_desc;
	NxBodyDesc	body_desc;

	NxActor *actor = NULL;	

	//	setup actor shape :
	NxConvexShapeDesc shape_desc;
	shape_desc.meshData		=	BuildConvexMesh( mesh );
	shape_desc.localPose.t	=	NxVec3(0, 0, 0);
	shape_desc.group		=	0;
	actor_desc.shapes.pushBack(&shape_desc);

	ASSERT(shape_desc.isValid());

	//	setup actor :
	actor_desc.body			= &body_desc;
	actor_desc.density		= 0;
	actor_desc.globalPose.t	= p;	
	actor_desc.globalPose.M	= NxMat33(q);
	actor_desc.group		= 0;
	
	//	setup body :
	body_desc.mass			= mass;
	
	ASSERT(body_desc.isValid());
	ASSERT(actor_desc.isValid());

	//	create actor :
	actor = nx_scene->createActor(actor_desc);
	ASSERT(actor);
	
	return actor;	
}	
	
	
//
//	ESciVis::CreatePhysMesh
//
NxActor	* ESciVis::CreatePhysMesh( vector<IPxTriMesh> meshes, vector<float> masses )
{
	return NULL;
}	

//
//	EPhysXPhysEngine::BuildConvexMesh
//
NxConvexMesh *ESciVis::BuildConvexMesh( const IPxTriMesh input_mesh )
{
	//IPxTriMesh	mesh	=	input_mesh->Clone();
	//mesh->SetFormat( GE_MESH_POSITION );
	//mesh->MergeVertices();				//This command causes convex cook crash

	NxConvexMeshDesc	convex_mesh_desc;

	NxArray<NxVec3>	verts;
	
	for (uint i=0; i<input_mesh->GetVertexNum(); i++) {
		EVertex v;
		v = input_mesh->GetVertex(i);
		verts.push_back( NxVec3(v.position.x, v.position.y, v.position.z) );
	}

    convex_mesh_desc.numVertices		= input_mesh->GetVertexNum();
    convex_mesh_desc.pointStrideBytes	= sizeof(NxVec3);
    convex_mesh_desc.points				= &verts[0];
	convex_mesh_desc.flags				= NX_CF_COMPUTE_CONVEX;
	
	ASSERT( convex_mesh_desc.isValid() );

	MemoryWriteBuffer	buf;	
	bool r = nx_cook->NxCookConvexMesh(convex_mesh_desc, buf);
	
	if (!r) {
		RUNTIME_ERROR("mesh contains to many vertices");
	}

	return nx->createConvexMesh(MemoryReadBuffer(buf.data));
}


//
//	Bytes2Color
//
static EVec4 Bytes2Color(uint bytes) 
{
	EVec4 c;
	c.x	=	((bytes >> 24) & 0xFF) / 255.0f;
	c.y	=	((bytes >> 16) & 0xFF) / 255.0f;
	c.z	=	((bytes >>  8) & 0xFF) / 255.0f;
	c.w	=	((bytes >>  0) & 0xFF) / 255.0f;
	return c;
}

/*-----------------------------------------------------------------------------
	Vector conversion :
-----------------------------------------------------------------------------*/

NxVec3 ToNxVec3( const EVec3 &v )
{
	return NxVec3( v.x, v.y, v.z );
}


NxQuat ToNxQuat( const EQuat &q )
{
	NxQuat nxq;
	nxq.x	=	q.x;
	nxq.y	=	q.y;
	nxq.z	=	q.z;
	nxq.w	=	q.w;
	return nxq;
}


EVec3 ToEVec3( const NxVec3 &v )
{
	return EVec3( v.x, v.y, v.z );
}


EQuat ToEQuat( const NxQuat &q )
{
	return EQuat( q.x, q.y, q.z, q.w );
}

#endif
