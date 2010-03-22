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


/*-----------------------------------------------------------------------------
	Physic Engine :
-----------------------------------------------------------------------------*/

enum EPhysObject_t {
		PHYS_RIGID_BODY,
		PHYS_CHARACTER,
		PHYS_TRIGGER,
		PHYS_STATIC_BODY,
	};

class	IPhysObject;
class	IPhysRigidBody;
class	IPhysCharacter;
class	IPhysTrigger;
typedef hard_ref<IPhysRigidBody>	IPxPhysRigidBody;
typedef hard_ref<IPhysTrigger>		IPxPhysTrigger;
typedef hard_ref<IPhysCharacter>	IPxPhysCharacter;


struct EPhysCharDesc_s {
		float	height;
		float	radius;
		float	max_veloctiy;
		float	acceleration;
		float	deceleration;
		float	air_control;
		float	step_offset;
		float	slope_limit;
	};	
	
struct EPhysTriggerDesc_s {
		IPxTriMesh		mesh;
		EVec4			pos;
		EQuat			orient;
	};	

enum EPhysCallback_t {
		PHYS_CB_CONTACT,
		PHYS_CB_TRIGGER,
	};	

/*-----------------------------------------------------------------------------
	Physic entities :
-----------------------------------------------------------------------------*/

class IPhysCallback : public IDisposable {
	public:
		virtual void		Callback		( IPhysObject *other ) = 0;
	};


class IPhysObject : public ILocatable {
	public:
		virtual void			SetGameHandle	( uint h ) { handle = h; }
		virtual uint			GetGameHandle	( void ) const { return handle; }
		virtual EPhysObject_t	GetType			( void ) const = 0;
	private:
		uint handle;
	};


class IPhysRigidBody : public IPhysObject {
	public:
		virtual EPhysObject_t	GetType			( void ) const { return PHYS_RIGID_BODY; }
	};
	
	
class IPhysTrigger : public IPhysObject {
	public:
		virtual EPhysObject_t	GetType			( void ) const { return PHYS_TRIGGER; }
	};
	

class	IPhysCharacter : public IPhysObject {
	public:
		virtual EPhysObject_t	GetType			( void ) const { return PHYS_CHARACTER; }
		virtual void			Update			( float dtime ) = 0;
		virtual void			Move			( const EVec3 &dir ) = 0;
		virtual void			Jump			( float z_vel ) = 0;
		virtual void			Crouch			( void ) = 0;
	};


/*-----------------------------------------------------------------------------
	Phys engine itself :
-----------------------------------------------------------------------------*/

class IPhysEngine : public IDisposable {
	public:
		virtual void				Frame				( uint msec ) = 0;
		virtual void				RenderDebug			( IPxRScene scene ) = 0;
		
		virtual void				CreateStaticActor	( const IPxTriMesh mesh, const EVec4 &pos, const EQuat &orient ) = 0;
		virtual IPxPhysCharacter	CreateCharacter		( const EPhysCharDesc_s *char_desc, const EVec4 &pos ) = 0;
		virtual IPxPhysRigidBody	CreateRigidBody		( const IPxTriMesh mesh, const EVec4 &pos, const EQuat &orient, float density ) = 0;
		virtual IPxPhysTrigger		CreateTrigger		( const EPhysTriggerDesc_s *desc ) = 0;

		virtual void				CleanupStaticScene	( void ) = 0;
	};