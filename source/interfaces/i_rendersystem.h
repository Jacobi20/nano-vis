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

#pragma once


class	IRenderSystem;
class	IDraw2D;

class	IRScene;
class	IRSLight;
class	IRSEntity;
class	IRSShader;


typedef hard_ref<IRenderSystem			>	IPxRenderSystem			;
typedef hard_ref<IDraw2D				>	IPxDraw2D				;

typedef hard_ref<IRScene				>	IPxRScene			;
typedef hard_ref<IRSLight				>	IPxRSLight			;
typedef hard_ref<IRSEntity				>	IPxRSEntity			;
typedef hard_ref<IRSShader				>	IPxRSShader			;


/*-----------------------------------------------------------------------------
	Enumerations :
-----------------------------------------------------------------------------*/

enum ERSLightShape_t {
		RS_LIGHT_SHAPE_SPOT=0, 
		RS_LIGHT_SHAPE_OMNI,
		RS_LIGHT_SHAPE_DIRECT,
	};


/*-----------------------------------------------------------------------------
	Structures :
-----------------------------------------------------------------------------*/

struct ERSQuad_s {
		EVec4	color;
		EVec2	p0,  p1;
		EVec2	uv0, uv1;
	};

struct ERSLine_s {
		EVec3	v0; 
		EVec4	c0;
		EVec2	uv0;
		
		EVec3	v1;
		EVec4	c1;
		EVec2	uv1;
	};
	
struct ERSVert2D_s {
		EVec4	color;
		EVec2	p;
		EVec2	uv;
	};
	

/*-----------------------------------------------------------------------------
	Drawing stuff :
-----------------------------------------------------------------------------*/

class IDraw2D : public IDisposable {
	public:
		virtual void	Set2DViewMode		( float offset_x=0, float offset_y=0 ) = 0;
		virtual void	SetColorMultiplier	( const EVec4 &color ) = 0;
		virtual	void	DrawQuads			( uint num_quads, const ERSQuad_s *quads, const EName texture ) = 0;
		
		virtual void	DrawLine			( EVec3 &v0, EVec3 &v1, EVec4 &color ) = 0;
		virtual void	DrawGrid			( void ) = 0;

		virtual void	Begin2D				( void ) = 0;
		virtual void	End2D				( void ) = 0;
		
		virtual void	SetTransform		( EMatrix4 &m ) { ASSERT(0); }
		virtual void	SetProjection		( EMatrix4 &m ) { ASSERT(0); }
	};

/*-----------------------------------------------------------------------------
	IRenderSystem
-----------------------------------------------------------------------------*/

struct ERendEntityDesc_s {
		EVec4		position;
		EQuat		orient;
		IPxTriMesh	mesh;
	};
	

struct	ERendLightDesc_s {
		EVec4	position;		//	position of the light
		EQuat	orient;			//	orientation of the light, by default: front is X, up us Z
		EVec4	color;			//	color of the light
		float	radius;			//	falloff_radius / radius / range of the light
		float	spot;			//	radius of the light spot at maximum distance
		bool	shadow;			//	shadows?
		ERSLightShape_t	shape;	//	light shape: Omni, Spot, Directional
		EName	mask;			//	path to mesking texture
	};

class	IRSLight : public ILocatable {
	public:
		virtual void	UpdateLight		( const ERendLightDesc_s *light_desc ) = 0;
	};

	
class	IRSEntity : public ILocatable {
	public:
		virtual void	UpdateEntity	( const ERendEntityDesc_s *entity_desc ) = 0;
		virtual void	Animate			( float time ) = 0;
	};	
	
	
class	IRSShader : public IDisposable {
	public:
		virtual bool	Setup			( void ) = 0;
		virtual EName	Name			( void ) const = 0;
	};	
	

	
class	IRScene : public IDisposable {
	public:
		virtual void			RenderScene		( void ) = 0;

		virtual IPxRSEntity	AddEntity		( IPxTriMesh mesh, const EVec4 &pos, const EQuat &orient ) = 0;
	
		virtual IPxRSLight	AddLight		( const ERendLightDesc_s *light_desc ) = 0;
		virtual void			RemoveLight		( IPxRSLight light ) = 0;
		virtual void			RemoveEntity	( IPxRSEntity entity ) = 0;
		
		virtual void			AddStaticMesh	( const IPxTriMesh mesh, const EVec4 &pos, const EQuat &orient ) = 0;
		virtual void			ProcessStaticGeometry ( void ) = 0;

		virtual void			SetView			( const EVec4 &position, const EQuat &orient ) = 0;
		virtual void			SetProjection	( float znear, float zfar, float width, float height ) = 0;
		virtual void			SetAmbientLevel	( const EVec4 &color ) = 0;
		
		virtual void			DebugLine		( const EVec3 &v0, const EVec3 &v1, const EVec4 &color  ) = 0;
	};
	

/*-----------------------------------------------------------------------------
	IRenderSystem
-----------------------------------------------------------------------------*/

struct  EVidModeDesc_s {
		uint	width;
		uint	height;
		bool	is_supported;
	};



class	IRenderSystem : public IDisposable {
	public:
		virtual				~IRenderSystem		( void ) {};
		
		virtual	void		BeginFrame			( void ) = 0;
		virtual void		RenderFrame			( IPxRScene scene ) = 0;
		virtual void		EndFrame			( void ) = 0;
		
		virtual bool		CheckFallback		( uint fallback ) = 0;

		//
		//	rendering subsystem :
		//
		virtual	IPxDraw2D				GetDraw2D			( void ) = 0;
		
		//
		//	scene stuff :
		//
		virtual IPxRScene				CreateScene			( void ) = 0;

		//
		//	stuff :
		//	
		virtual void		GetScreenSize		( uint &w, uint &h ) = 0;
		virtual void *		GetWndDescriptor	( void ) = 0;
		virtual uint		GetCurrentMode		( void ) = 0;
		virtual uint		GetModeNum			( void ) = 0;
		virtual void		GetModeDesc			( uint nmode, EVidModeDesc_s *mode_desc ) = 0;
		
		virtual void		GetTextureSize		( const EName tex_name, uint &w, uint &h ) = 0;
	};

