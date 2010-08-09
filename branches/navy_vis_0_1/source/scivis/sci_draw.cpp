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
	texture stuff :
-----------------------------------------------------------------------------*/

uint Vec4ToByte4(const EVec4 &v)
{
	EVec4 cv = v;
	cv.x = Clamp<float>(cv.x, 0, 1);
	cv.y = Clamp<float>(cv.y, 0, 1);
	cv.z = Clamp<float>(cv.z, 0, 1);
	cv.w = Clamp<float>(cv.w, 0, 1);
	uchar r = (uchar)(255 * cv.x);
	uchar g = (uchar)(255 * cv.y);
	uchar b = (uchar)(255 * cv.z);
	uchar a = (uchar)(255 * cv.w);
	
	return D3DCOLOR_ARGB(a,r,g,b);
}

//
//	ED3DDraw2D::DrawQuad
//
void RS_DrawQuad( LPDIRECT3DDEVICE9 d3ddev, float x0, float y0, float x1, float y1, float z, float u0, float v0, float u1, float v1, uint32_t color4b )
{
	struct vertex2d_hw_s {
			float	 x, y, z;
			uint32_t color;
			float	 u, v;
		};

	vertex2d_hw_s quad[4];

	quad[0].x = x0;	quad[0].y = y0;	quad[0].z = z;
	quad[0].u = u0; quad[0].v = v0; quad[0].color = color4b;

	quad[1].x = x1;	quad[1].y = y0;	quad[1].z = z;
	quad[1].u = u1; quad[1].v = v0; quad[1].color = color4b;

	quad[2].x = x1;	quad[2].y = y1;	quad[2].z = z;
	quad[2].u = u1; quad[2].v = v1; quad[2].color = color4b;

	quad[3].x = x0;	quad[3].y = y1;	quad[3].z = z;
	quad[3].u = u0; quad[3].v = v1; quad[3].color = color4b;
	
	HRCALL( d3ddev->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1) );
	
	HRCALL( d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &quad, sizeof(vertex2d_hw_s)) );
}


//
//	ESciVis::RenderQuads
//
void ESciVis::RenderQuads( uint quad_num, const EUIQuad *quads, EString image )
{
	uint w, h;
	GetScreenSize(w, h);

	float fw = (float)w;
	float fh = (float)h;
	float offset = 0.5f;
	float offset_x = 0.0f;
	float offset_y = 0.0f;
	
	//	Set 2D projection matrix :
	D3DXMATRIX	wrld, view,proj;

	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&wrld);
    D3DXMatrixOrthoOffCenterLH(&proj,	0+offset - offset_x, 
										fw+offset - offset_x, 
										fh+offset - offset_y, 
										0+offset - offset_y, 
										-9999, +9999);

	d3ddev->SetTransform(D3DTS_WORLD,		&wrld);
	d3ddev->SetTransform(D3DTS_VIEW,		&view);
	d3ddev->SetTransform(D3DTS_PROJECTION,	&proj);
	
	// PROFILER_PROBE();

	float zero  = 0;
	//	Config modulation params :
	HRCALL( d3ddev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE) );
	HRCALL( d3ddev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE) );

	//	Disable stuff :	
	HRCALL( d3ddev->SetRenderState( D3DRS_LIGHTING,		FALSE) );
	HRCALL( d3ddev->SetRenderState( D3DRS_FILLMODE,		D3DFILL_SOLID) );
	HRCALL( d3ddev->SetRenderState( D3DRS_CULLMODE,		D3DCULL_NONE) );
	
	
	//	Config blending :
	HRCALL( d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,	TRUE) );
	HRCALL( d3ddev->SetRenderState(D3DRS_BLENDOP,			D3DBLENDOP_ADD) );
	
	//	Config Z-buffer :
	HRCALL( d3ddev->SetRenderState( D3DRS_ZWRITEENABLE, D3DZB_TRUE ) );
	HRCALL( d3ddev->SetRenderState( D3DRS_ZENABLE,		D3DZB_TRUE ) );
	HRCALL( d3ddev->SetRenderState( D3DRS_ZFUNC,		D3DCMP_LESSEQUAL ) );
	HRCALL( d3ddev->SetRenderState( D3DRS_DEPTHBIAS,			*(int*)&zero) );
	HRCALL( d3ddev->SetRenderState( D3DRS_SLOPESCALEDEPTHBIAS,	*(int*)&zero) );

	//	Config stencil :
	HRCALL( d3ddev->SetRenderState( D3DRS_STENCILENABLE, FALSE ) );

	uint mask =	 D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED;
	HRCALL( d3ddev->SetRenderState( D3DRS_COLORWRITEENABLE, mask ) );


	uint src = D3DBLEND_SRCALPHA;
	uint dst = D3DBLEND_INVSRCALPHA;
	uint op  = D3DBLENDOP_ADD;
	HRCALL( d3ddev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ) );
	HRCALL( d3ddev->SetRenderState( D3DRS_SRCBLEND,		src ) );
	HRCALL( d3ddev->SetRenderState( D3DRS_DESTBLEND,	dst ) );
	HRCALL( d3ddev->SetRenderState( D3DRS_BLENDOP,		op )  );

	for (uint i=0; i<8; i++) {
		HRCALL( d3ddev->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 16) );
		//HRCALL( d3ddev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC) );
		HRCALL( d3ddev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR) );
		HRCALL( d3ddev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR) );
		HRCALL( d3ddev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR) );
		
		HRCALL( d3ddev->SetSamplerState(i,  D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP) );
		HRCALL( d3ddev->SetSamplerState(i,  D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP) );
		HRCALL( d3ddev->SetSamplerState(i,  D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP) );
	}
	
	SetTexture( image );

	for (uint i=0; i<quad_num; i++) {
		const EUIQuad *q = &quads[i];
		
		EVec4	color = q->color;
		RS_DrawQuad(d3ddev, 
					q->p0.x,	q->p0.y,	q->p1.x, q->p1.y,  0.5f,
					q->uv0.x,	q->uv0.y,   q->uv1.x, q->uv1.y, Vec4ToByte4(color) );
	}
}


//
//	ESciVis::GetImageSize
//
void ESciVis::GetImageSize( EString image, uint &width, uint &height )
{
	GetTextureSize( image, width, height );
}

