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





class	IGeometryEngine;
class	IPolygon;
class	IPolyMesh;

class	ITriMesh;
class	ISkinCluster;

class	IScene;
class	ISceneNode;
class	ISceneIt;

class	IAnimCurveFloat;
class	IAnimCurveVec4;
class	IAnimCurveQuat;

typedef hard_ref<IGeometryEngine	>	IPxGeometryEngine;
typedef hard_ref<IPolygon			>	IPxPolygon;
typedef hard_ref<IPolyMesh			>	IPxPolyMesh;
typedef hard_ref<ITriMesh			>	IPxTriMesh;
typedef hard_ref<IScene				>	IPxScene;
typedef hard_ref<ISceneNode			>	IPxSceneNode;
typedef hard_ref<ISceneIt			>	IPxSceneIt;

typedef hard_ref<IAnimCurveFloat	>	IPxAnimCurveFloat	;
typedef hard_ref<IAnimCurveVec4		>	IPxAnimCurveVec4	;
typedef hard_ref<IAnimCurveQuat		>	IPxAnimCurveQuat	;

/*-----------------------------------------------------------------------------
	Geometry engine :
-----------------------------------------------------------------------------*/

class	IGeometryEngine : public IDisposable {
	public:
		virtual IPolygon		*CreatePolygon			( void ) = 0;
		virtual IPolyMesh		*CreatePolyMesh			( void ) = 0;
		virtual ITriMesh		*CreateTriMesh			( uint format ) = 0;
		virtual IScene			*CreateScene			( void ) = 0;
		virtual ISceneNode		*CreateSceneNode		( void ) = 0;
		
		virtual	IAnimCurveFloat	*CreateAnimCurveFloat	( void ) = 0;
		virtual IAnimCurveVec4	*CreateAnimCurveVec4	( void ) = 0;
		virtual IAnimCurveQuat	*CreateAnimCurveQuat	( void ) = 0;
	};
	


/*-----------------------------------------------------------------------------
	Animation Curve :
-----------------------------------------------------------------------------*/

class IAnimCurveFloat : public IDisposable {
	public:
		virtual uint				GetKeyNum		( void ) const = 0;
		virtual void				GetKey			( uint index, float &time, float &value ) const = 0;
		virtual void				RemoveKey		( uint index ) = 0;
		virtual	void				InsertKey		( float time, float value ) = 0;
		virtual float				GetMinTime		( void ) const = 0;
		virtual float				GetMaxTime		( void ) const = 0;
		
		virtual	float				Evaluate		( float time, float def_value ) const = 0;
		virtual IPxAnimCurveFloat	Clone			( void ) const = 0;
		
		virtual void				WriteXML		( rapidxml::xml_node<> *xnode ) const = 0;
		virtual void				ReadXML			( rapidxml::xml_node<> *xnode ) = 0;
	};

class IAnimCurveVec4 : public IDisposable {
	public:
		virtual uint				GetKeyNum		( void ) const = 0;
		virtual void				GetKey			( uint index, float &time, EVec4 &value ) const = 0;
		virtual void				RemoveKey		( uint index ) = 0;
		virtual	void				InsertKey		( float time, const EVec4 &value ) = 0;
		virtual float				GetMinTime		( void ) const = 0;
		virtual float				GetMaxTime		( void ) const = 0;
		
		virtual	EVec4				Evaluate		( float time, const EVec4 &def_value ) const = 0;
		virtual IPxAnimCurveVec4	Clone			( void ) const = 0;
		
		virtual void				WriteXML		( rapidxml::xml_node<> *xnode ) const = 0;
		virtual void				ReadXML			( rapidxml::xml_node<> *xnode ) = 0;
	};

class IAnimCurveQuat : public IDisposable {
	public:
		virtual uint				GetKeyNum		( void ) const = 0;
		virtual void				GetKey			( uint index, float &time, EQuat &value ) const = 0;
		virtual void				RemoveKey		( uint index ) = 0;
		virtual	void				InsertKey		( float time, const EQuat &value ) = 0;
		virtual float				GetMinTime		( void ) const = 0;
		virtual float				GetMaxTime		( void ) const = 0;
		
		virtual	EQuat				Evaluate		( float time, const EQuat &def_value ) const = 0;
		virtual IPxAnimCurveQuat	Clone			( void ) const = 0;
		
		virtual void				WriteXML		( rapidxml::xml_node<> *xnode ) const = 0;
		virtual void				ReadXML			( rapidxml::xml_node<> *xnode ) = 0;
	};



/*-----------------------------------------------------------------------------
	Mesh format
-----------------------------------------------------------------------------*/

const uint GE_MESH_POSITION			=	0x0001;
const uint GE_MESH_NORMAL			=	0x0002;
const uint GE_MESH_COLOR_0			=	0x0004;
const uint GE_MESH_COLOR_1			=	0x0008;
const uint GE_MESH_UV_0				=	0x0010;
const uint GE_MESH_UV_1				=	0x0020;
const uint GE_MESH_TANGENT			=	0x0040;
const uint GE_MESH_BINORMAL			=	0x0080;
const uint GE_MESH_JOINT_WEIGHTS	=	0x0100;
const uint GE_MESH_JOINT_IND		=	0x0200;


/*-----------------------------------------------------------------------------
	Vertex and Shading group :
-----------------------------------------------------------------------------*/

class EVertex {
	public:
		EVertex		( void ) {
			position			=	EVec3(0,0,0);
			normal				=	EVec3(0,0,0);
			color0				=	EVec4(0,0,0,0);
			color1				=	EVec4(0,0,0,0);
			uv0					=	EVec2(0,0);
			uv1		  			=	EVec2(0,0);
			tangent				=	EVec3(0,0,0);
			binormal			=	EVec3(0,0,0);
			joint_inds[0]		=	joint_inds[1]		=	0;
			joint_inds[2]		=	joint_inds[3]		=	0;
			joint_weights[0]	=	joint_weights[1]	=	0;
			joint_weights[2]	=	joint_weights[3]	=	0;
		}
		
		~EVertex	( void ) {
		}
	public:
		EVec3	position;
		EVec3	normal;
		EVec4	color0;
		EVec4	color1;
		EVec2	uv0;
		EVec2	uv1;
		EVec3	tangent;
		EVec3	binormal;
		uint	joint_inds[4];
		float	joint_weights[4];
	};
	
	
class EShadingGroup {
	public:
		EShadingGroup	( void ) {
			start = num = 0;
			shader = "";
		}
		
		~EShadingGroup	( void ) {
		}
	public:
		//	NOTE : 'start' and 'num' are related to primitives 
		//	but not to indices or vertices.
		uint	start;
		uint	num;
		EName	shader;
	};	


class EJoint {
	public:
		EJoint	( void ) {
			name				=	"";
			parent_index		=	0;
			orient_anim_curve	=	NULL;
			pos_anim_curve		=	NULL;
		}
		
		~EJoint	( void ) {
		}
		
		EName	name;			//	joint's name
		EQuat	orient;			//	Maya's joint orient
		EVec4	position;		//	Maya's joint position
		EQuat	def_orient;		//	default rotation of the joint (when curve is NULL)
		EVec4	def_position;	//	default position of the joint (when curve is NULL)
		EQuat	bind_orient;
		EVec4	bind_pos;
		int	    parent_index;	//	index of parent joint

		IPxAnimCurveQuat	orient_anim_curve;
		IPxAnimCurveVec4	pos_anim_curve;
	};	
	

/*------------------------------------------------------------------------------
	Polygon :
------------------------------------------------------------------------------*/

class IPolygon : public IDisposable {
	public:
		virtual void			SetShader		( EName shader_name ) = 0;
		virtual EName			GetShader		( void ) const = 0;

		virtual void			SetVertexNum	( uint num ) = 0;
		virtual uint			GetVertexNum	( void ) const = 0;
		
		virtual uint			AppendVertex	( const EVertex &v ) = 0;
		virtual void			InsertVertex	( uint index, const EVertex &v ) = 0;
		virtual void			RemoveVertex	( uint index ) = 0;
		
		virtual EVertex			GetVertex		( uint index ) const = 0;
		virtual void			SetVertex		( uint index, const EVertex &v ) = 0;
		
		virtual IPxPolygon		Clone			( void ) const = 0;
		virtual void			SortVertsCCW	( const EVec3 &winding_vector ) = 0;
	};
	
	
/*------------------------------------------------------------------------------
	Polygonal mesh :
------------------------------------------------------------------------------*/

class IPolyMesh : public IDisposable {
	public:
		virtual uint				GetPolyNum			( void ) const = 0;
		virtual void				SetPolyNum			( uint num ) = 0;
													
		virtual const IPxPolygon	GetPolygon			( uint index ) const = 0;
		virtual IPxPolygon			GetPolygon			( uint index ) = 0;
		virtual void				SetPolygon			( uint index, const IPxPolygon poly ) = 0;
		virtual void				AddPolygon			( const IPxPolygon poly ) = 0;
		virtual void				RemovePolygon		( uint index ) = 0;
		
		virtual IPxPolyMesh			Clone				( void ) const = 0;
													
		virtual void				MergeMeshes			( IPxPolyMesh src_mesh ) = 0;
		virtual void				BuildConvex			( uint plane_num, const EPlane *planes ) = 0;
		virtual void				SortPolysByShader	( void ) = 0;
		
		virtual IPxTriMesh			BuildTriMesh	( uint mesh_format ) = 0;
	};

/*------------------------------------------------------------------------------
	Polyhedron :
------------------------------------------------------------------------------*/

//class	IPolyhedron : public IDisposable {
//	public:
//		virtual	uint			GetPlaneNum		( void ) const = 0;
//		virtual EPlane			GetPlane		( uint index ) = 0;
//		virtual void			AddPlane		( const EPlane &plane ) = 0;
//
//		
//	};
//	

/*------------------------------------------------------------------------------
	Indexed triangle mesh :
------------------------------------------------------------------------------*/

class	ITriMesh : public IDisposable {			
	public:
		virtual uint			GetFormat		( void ) const = 0;
		virtual void			SetFormat		( uint format ) = 0;
	
		virtual void			SetVertexNum	( uint num ) = 0;
		virtual uint			GetVertexNum	( void ) const = 0;
		virtual uint			AddVertex		( const EVertex &v ) = 0;
		virtual void			SetVertex		( uint index, const EVertex &v ) = 0;
		virtual EVertex			GetVertex		( uint index ) const = 0;
		
		virtual uint			AddUniqueVertex	( const EVertex &v, float tolerance ) = 0;
		
		virtual void			SetTriangleNum	( uint num ) = 0;
		virtual uint			GetTriangleNum	( void ) const = 0;
		virtual uint			AddTriangle		( uint i0, uint i1, uint i2 ) = 0;
		virtual uint			AddTriangle		( const EVertex& v0, const EVertex& v1, const EVertex & v2) = 0;
		virtual void			SetTriangle		( uint index, uint i0,  uint i1,  uint i2 ) = 0;
		virtual void			GetTriangle		( uint index, uint &i0, uint &i1, uint &i2 ) const = 0;

		virtual void			AttachSG		( const EShadingGroup &sg ) = 0;
		virtual void			DetachSG		( uint index ) = 0;
		virtual uint			GetSGNum		( void ) const = 0;
		virtual EShadingGroup	GetSG			( uint index ) const = 0;
		virtual void			SetSG			( uint index, const EShadingGroup &sg ) = 0;
		
		virtual IPxTriMesh		Clone			( void ) const = 0;
		
		virtual void			WriteXML		( rapidxml::xml_node<> *xnode ) const = 0;
		virtual void			ReadXML			( rapidxml::xml_node<> *xnode ) = 0;
		
		//	sophistic stuff :
		virtual void			Transform		( const EMatrix4 &T ) = 0;
		virtual void			GlueSGs			( void ) = 0;
		virtual void			MergeVertices	( void ) = 0;
		virtual void			Split			( const EPlane &plane, IPxTriMesh left, IPxTriMesh right ) = 0;
		virtual void			ComputeTangents	( void ) = 0;
		virtual EBBox			ComputeBBox		( void ) const = 0;

		virtual void			Merge			( IPxTriMesh mesh) = 0;
		virtual bool			IsEmpty			( void ) = 0;
		
		//	animation stuff :
		virtual void			AddJoint		( EJoint joint ) = 0;
		//virtual void			SetJointNum		( uint num ) = 0;
		virtual uint			GetJointNum		( void ) const = 0;
		virtual EJoint			*GetJoint		( uint index ) = 0;
		//virtual const EJoint	*GetJoint		( int index ) const = 0;
		//virtual EJoint			*GetJoint		( const EName name ) = 0;
		//virtual const EJoint	*GetJoint		( const EName name ) const = 0;

		virtual void			AnimateSkeleton	( float time ) = 0;
		//virtual void			AnimateSkeleton	( 
		//								/* in:  list of root nodes + list of times */
		//								/* out:	target mesh */
		//										 ) = 0;
	};

/*-----------------------------------------------------------------------------
	Scene graph :	
-----------------------------------------------------------------------------*/

class IScene : public IDisposable {
	public:
		virtual IPxSceneNode		Root			( void ) = 0;
		virtual const IPxSceneNode	Root			( void ) const = 0;
		virtual void				WriteXML		( rapidxml::xml_node<> *xnode ) const = 0;
		virtual void				ReadXML			( rapidxml::xml_node<> *xnode ) = 0;

		virtual void				Clear			( void ) = 0;
		virtual void				BreakHierarchy	( IPxSceneNode node ) = 0;
		
		virtual IPxSceneIt			GetIterator		( void ) = 0;
		virtual	IPxSceneNode		GetNodeByPath	( const char *path ) = 0;
	};
	

class	ISceneIt : public IDisposable {
	public:
		virtual void			Reset		( void ) = 0;
		virtual bool			Done		( void ) = 0;
		virtual void			Next		( void ) = 0;
		virtual IPxSceneNode	Current		( void ) = 0;
	};	
	
	
class ISceneNode : public IDisposable {
	public:						
		virtual IPxTriMesh			GetMesh			( void ) = 0;
		virtual const IPxTriMesh	GetMesh			( void ) const = 0;
		virtual void				SetMesh			( IPxTriMesh mesh ) = 0;

		virtual EName				GetName			( void ) const = 0;
		virtual void				SetName			( EName name ) = 0;
		virtual EName				GetClass		( void ) const = 0;
		virtual void				SetClass		( EName classname ) = 0;
		virtual const char			*GetAffector	( const char *key, const char *def_value ) const = 0;
		virtual void				SetAffector		( const char *affector_string ) = 0;
		virtual void				AddAffector		( const char *affector_string ) = 0;

		virtual void				GetLocalPose	( EVec4 &pos, EQuat &orient ) const = 0;
		virtual void				SetLocalPose	( const EVec4 &pos, const EQuat &orient ) = 0;
		virtual void				GetGlobalPose	( EVec4 &pos, EQuat &orient ) const = 0;

		virtual void				GetPoseAnim		( IPxAnimCurveVec4 &pos_anim, IPxAnimCurveQuat &orient_anim ) = 0;
		virtual void				SetPoseAnim		( IPxAnimCurveVec4 pos_anim, IPxAnimCurveQuat orient_anim ) = 0;
		
		//	hierarchy stuff :
		virtual void				AttachChild		( IPxSceneNode node ) = 0;
		virtual void				DetachChild		( IPxSceneNode node ) = 0;		
		virtual void				DetachChild		( uint index ) = 0;	
		virtual void				DetachAll		( void ) = 0;
		
		virtual uint				GetChildNum		( void ) const = 0;
		virtual IPxSceneNode		GetChild		( uint index ) = 0;
		virtual const IPxSceneNode	GetChild		( uint index ) const = 0;
		virtual IPxSceneNode		GetChild		( EName name ) = 0;
		virtual const IPxSceneNode	GetChild		( EName name ) const = 0;
		
		virtual void				SetParent		( IPxSceneNode parent ) = 0;
		virtual IPxSceneNode		GetParent		( void ) = 0;
		virtual const IPxSceneNode	GetParent		( void ) const = 0;

		//	xml stuff :
		virtual void			WriteXML		( rapidxml::xml_node<> *xnode ) const = 0;
		virtual void			ReadXML			( rapidxml::xml_node<> *xnode ) = 0;
	};
	
	
	
	
	
	
	
	
	