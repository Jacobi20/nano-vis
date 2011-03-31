-------------------------------------------------------------------------------
-- The MIT License

-- Copyright (c) 2010 IFMO/GameDev Studio

-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:

-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.

-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.
-------------------------------------------------------------------------------

local	math		=	require("math");
local	print		=	_G["print"];
local	core		=	require("core");
local	user		=	user;
local	cfg			=	require("cfg");
local	input		=	require("input");
local	game		=	require("game");
local	entity		=	require("entity");
local	character	=	require("character");
local	shipmodel	=	require("shipmodel");
local	ui			=	require("ui");
	
module	("ships");

-------------------------------------------------------------------------------
--	CONTROL
-------------------------------------------------------------------------------

function createSSN668( x, y, z, yaw, pitch, roll )
	local	id	=	game.spawnEntity();
	
	local desc = {
		hxf_mesh	=	"scenes/ssn668.esx|hydromesh";
		vis_mesh	=	"scenes/ssn668.esx|vismesh";
		phys_mesh	=	"scenes/ssn668.esx|physmesh";
		mass		=	6000000;
		
		integral_density	=	0.1;
		water_resistance	=	1;
	}
	
	entity.setupShip( id, desc );
	entity.setPose( id, x, y, z, yaw, pitch, roll );
	
	return id;
end


function createUBoat( x, y, z, yaw, pitch, roll )
	local	id	=	game.spawnEntity();
	
	local desc = {
		hxf_mesh	=	"scenes/uboat.esx|hydromesh";
		vis_mesh	=	"scenes/uboat.esx|vismesh";
		phys_mesh	=	"scenes/uboat.esx|physmesh";
		engine_snd  =	"sound/submarine.mp3";
		mass		=	2000000;
		cm_offset_x	=	0;
		cm_offset_z	=	0;
		
		integral_density	=	1;
		water_resistance	=	2;
	}
	
	entity.setupShip( id, desc );
	entity.setPose( id, x, y, z, yaw, pitch, roll );
	
	return id;
end



function createCutter( x, y, z, yaw, pitch, roll )
	local	id	=	game.spawnEntity();
	
	local desc = {
		hxf_mesh	=	"scenes/stealth.esx|hydromesh";
		vis_mesh	=	"scenes/stealth.esx|vismesh";
		phys_mesh	=	"scenes/stealth.esx|physmesh";
		engine_snd  =	"sound/submarine.mp3";
		mass		=	1500000;
		cm_offset_x	=	51.5;
		cm_offset_z	=	-2.7;
		
		integral_density	=	0.6;
		water_resistance	=	1.3;
		
		
		-- hxf_mesh	=	"scenes/boat.esx|hydromesh";
		-- vis_mesh	=	"scenes/boat.esx|vismesh";
		-- phys_mesh	=	"scenes/boat.esx|physmesh";
		-- engine_snd  =	"sound/submarine.mp3";
		-- mass		=	500000;
		-- cm_offset_x	=	20.5;
		-- cm_offset_z	=	-2.5;
		
		-- integral_density	=	1;
		-- water_resistance	=	0.2;
	}
	
	entity.setupShip( id, desc );
	entity.setPose( id, x, y, z, yaw, pitch, roll );

	local sd = shipmodel.getDynamics(id);
	
	print("L = "..sd.ship_length);
	print("B = "..sd.ship_width);
	
	return id;
end


