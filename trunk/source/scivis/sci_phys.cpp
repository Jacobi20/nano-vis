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
    sceneDesc.gravity               = NxVec3(0,0,-9.8);
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