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
local	ui			=	require("ui");
local 	control		=	require("control");
local 	ships		=	require("ships");
local	shipmodel	=	require("shipmodel");
	
module	("cgame");

-------------------------------------------------------------------------------
--	CGAME
-------------------------------------------------------------------------------

local	ship		=	nil;
local	ship2		=	nil;
local	ship3		=	nil;
local	ship4		=	nil;

--
--	init()
--
function init()
	game.setGravity(9.8);

	game.setWaving( true );
	game.setAmbient( 0.7, 0.7, 0.7 );

	ship	=	ships.createUBoat(10,  0, 20, 0,0,0);
	-- ship2	=	ships.createUBoat(10, 40,-20, 0,0,0);
	-- ship3	=	ships.createUBoat(10,-40,-30, 0,0,0);
	-- ship4	=	ships.createUBoat(10, 80,-40, 0,0,0);
end


--
--	driveShip
--
local function driveShip( dtime, ship )
	local	tx	=	input.s3dm.tx;
	local	ty	=	input.s3dm.ty;
	local	tz	=	input.s3dm.tz;
	local	rx	=	input.s3dm.rx;
	local	ry	=	input.s3dm.ry;
	local	rz	=	input.s3dm.rz;
	
	local	x,y,z,yaw, pitch, roll = entity.getPose(ship);
	
	core.debugString( yaw );
	
	--	TZ :
	if tz<0 then tz=0; end;
	shipmodel.addForce( ship, 0,0, -tz * 100000, 0,0,0 );
	
	
	--	TY :
	local fmag = 100000;
	local fx = -math.cos( math.rad( yaw + rz/18 ) ) * math.cos( math.rad( pitch ) ) * ty * fmag;
	local fy = -math.sin( math.rad( yaw + rz/18 ) ) * math.cos( math.rad( pitch ) ) * ty * fmag;
	local fz = -math.sin( math.rad( pitch ) ) * ty * fmag;
	shipmodel.addForce( ship, fx, fy, 0, -50,0,-1 );
	
	--	RX :
	shipmodel.addForce( ship, 0,0,  rx*3000,  30,0,0 );
	shipmodel.addForce( ship, 0,0, -rx*3000, -30,0,0 );
	
	--	RY :
	shipmodel.addForce( ship, 0,0, -ry*1000,  0,  10, 0 );
	shipmodel.addForce( ship, 0,0,  ry*1000,  0, -10, 0 );
	
end


--
--	frame()
--
function frame( dtime )
	core.debugString("FPS  : "..1/dtime);
	
	core.debugString("S3DM [T]:", input.s3dm.tx, input.s3dm.ty, input.s3dm.tz );
	core.debugString("S3DM [R]:", input.s3dm.rx, input.s3dm.ry, input.s3dm.rz );

	control.update( dtime );

	driveShip( dtime, ship );
	
end


--
--	entityFrame
--
function entityFrame(id, dtime)	
end


--
--	shutdown()
--
function shutdown()
end


--
--	drawUI()
--
function drawUI()
end


--
--	levelLoader()
--
function levelLoader ( node )
end






