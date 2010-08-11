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
#include "../../engine/core/luna.h"

/*-----------------------------------------------------------------------------
	Ship Lua API :
-----------------------------------------------------------------------------*/

class ELuaShip : public Luna<ELuaShip> {
	public:
					ELuaShip	( lua_State *L );
					~ELuaShip	( void );
					
		LUNA_DECLARE_CLASS(ELuaShip);

		int set_vis_mesh	( lua_State *L );		//	sets visible mesh
		int	set_hsf_mesh	( lua_State *L );		//	sets mesh for HSF applying
		int	set_hdf_mesh	( lua_State *L );		//	sets mesh for HDF applying
		
		int	set_resistance	( lua_State *L );		//	sets water resistance
		
		int make_rigidbody	( lua_State *L );		//	makes RB form list of shapes
		int build_voxels	( lua_State *L );		//	makes RB form list of shapes

		int	add_force		( lua_State *L );		//	adds force
		int add_momentum	( lua_State *L );		//	adds momentum
			
		int	get_angles		( lua_State *L );		//	gets yaw, pitch, roll
		int get_position	( lua_State *L );		//	gets x,y,z

		int	set_angles		( lua_State *L );		//	sets yaw, pitch, roll
		int set_position	( lua_State *L );		//	sets x,y,z
		
		int	get_hsf_momentum( lua_State *L );		//	
		int	get_hsf_force	( lua_State *L );		//
		int get_right_arm	( lua_State *L );		//
		
		int	get_center_mass	( lua_State *L );		//
		
	protected:
		IPxShip	ship;
	};