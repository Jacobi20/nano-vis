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
	Ship rendering stuff :
-----------------------------------------------------------------------------*/

//
//	EShip::ReloadShader
//
void EShip::ReloadShader( void )
{
	shader_fx	=	sci_vis->CompileEffect( "../scidata/shader.fx" );
}


//
//	EShip::Render
//
void EShip::Render( ERendEnv_s *rend_env )
{
	if (!shader_fx) return;
	
	if (!d3d_mesh_vis) {	RAISE_EXCEPTION("view mesh not created")	}

	EQuat	q;
	EVec4	p;
	GetPose(p, q);
	
	EMatrix4 world	=	QuatToMatrix(q) * Matrix4Translate(p);

	HRCALL( shader_fx->SetMatrix("matrix_world",	&D3DXMATRIX( world.Ptr() ) ) );
	HRCALL( shader_fx->SetMatrix("matrix_view",		&D3DXMATRIX( rend_env->matrix_view.Ptr() ) ) );
	HRCALL( shader_fx->SetMatrix("matrix_proj",		&D3DXMATRIX( rend_env->matrix_proj.Ptr() ) ) );
	HRCALL( shader_fx->SetVector("view_pos",		&D3DXVECTOR4( rend_env->view_pos.Ptr() ) ) );


	//
	//	draw ship :
	//
	if (ship_show_hull->Bool()) {	
		uint n;
		HRCALL( shader_fx->SetTechnique("solid_body") );
		HRCALL( shader_fx->Begin(&n, 0) );
		
		for (uint pass=0; pass<n; pass++) {
		
			HRCALL( shader_fx->BeginPass(pass) );
		
			d3d_mesh_vis->DrawSubset(0);
			
			HRCALL( shader_fx->EndPass() );
		}

		HRCALL( shader_fx->End() );
	}
	
	
	if (ship_show_submerge->Bool()) {
	
		HRCALL( shader_fx->SetMatrix("matrix_world",	&D3DXMATRIX( Matrix4Identity().Ptr() ) ) );
		HRCALL( shader_fx->SetMatrix("matrix_view",		&D3DXMATRIX( rend_env->matrix_view.Ptr() ) ) );
		HRCALL( shader_fx->SetMatrix("matrix_proj",		&D3DXMATRIX( rend_env->matrix_proj.Ptr() ) ) );
		HRCALL( shader_fx->SetVector("view_pos",		&D3DXVECTOR4( rend_env->view_pos.Ptr() ) ) );
	
		ID3DXMesh *d3d_ship_bottom = NULL;
		d3d_ship_bottom = sci_vis->CreateMesh( mesh_submerged_hsf );
		
		
		uint n;
		HRCALL( shader_fx->SetTechnique("submerged_body") );
		HRCALL( shader_fx->Begin(&n, 0) );
		
		for (uint pass=0; pass<n; pass++) {
		
			HRCALL( shader_fx->BeginPass(pass) );

			if (d3d_ship_bottom) {		
				d3d_ship_bottom->DrawSubset(0);
			}
			
			HRCALL( shader_fx->EndPass() );
		}

		HRCALL( shader_fx->End() );

		SAFE_RELEASE( d3d_ship_bottom );
	}
	
	
		
	//
	//	draw cubes :
	//
	if (ship_show_voxels->Bool()) {
		
		if (!d3d_cube) {
			LOG_WARNING("voxel grid not created");
		}
	
		uint n;
		HRCALL( shader_fx->SetTechnique("cubes") );
		HRCALL( shader_fx->Begin(&n, 0) );
		
		for (uint pass=0; pass<n; pass++) {
		
			EMatrix4 rot_trans = QuatToMatrix(q) * Matrix4Translate(p);
			EMatrix4 inv_rot = Matrix4Inverse( QuatToMatrix(q) );
		
			HRCALL( shader_fx->BeginPass(pass) );
		
			for (uint i=0; i<voxel_grid->GetVoxelNum(); i++) {
			
				EVoxel vx;
				voxel_grid->GetVoxel(i, vx);
			
				EMatrix4 world	=	inv_rot * Matrix4Translate(vx.center.x, vx.center.y, vx.center.z) * rot_trans;
				HRCALL( shader_fx->SetMatrix("matrix_world",	&D3DXMATRIX( world.Ptr() ) ) );
				
				shader_fx->CommitChanges();

				d3d_cube->DrawSubset(0);
			}
			
			HRCALL( shader_fx->EndPass() );
		}

		HRCALL( shader_fx->End() );
	}
}

