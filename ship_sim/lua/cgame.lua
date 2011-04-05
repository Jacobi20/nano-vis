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
local	table		=	require("table");
local	print		=	_G["print"];
local	string		=	require("string");
local	core		=	require("core");
local	dv			=	require("dv");
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
local	plotter		=	require("plotter");
local	io			=	require("io");
	
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
	game.setAmbient( 0.0, 0.0, 0.0 );
	
	ship	=	ships.createCutter(5, 0, -0.86, 90,0,0);
	-- ship	=	ships.createUBoat(-50,  0, -5, 45,0,0);
	
	setup_waving();
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
	
	-- core.debugString( yaw );
	
	--	TZ :
	if tz<0 then tz=0; end;
	shipmodel.addForce( ship, 0,0, -tz * 100000, 0,0,00 );
	
	
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


local	ship_data 		= 	{};
local	record_counter 	= 	0;
local	degree			=	0;
local 	ship_log 		=	nil;
local 	shipwreck 		=	false;


input.bind ( "F7",	 	"cgame.do_rolling()", "" );
input.bind ( "T",	 	"cgame.touch_ship_heave();	cgame.restart_log()", "" );
input.bind ( "R",	 	"cgame.touch_ship_roll();	cgame.restart_log()", "" );
input.bind ( "Q",	 	"cgame.restart_log()", "" );
input.bind ( "W",	 	"cgame.setup_waving()", "" );

input.bind ( "F2",	 	"cgame.setup_exp03()", "" );

function touch_ship_roll()
	local x,y,z,a,b,c = shipmodel.getPose(ship);
	shipmodel.setPose(ship, x,y,z,a,0,15);
end

function touch_ship_heave()
	local x,y,z,a,b,c = shipmodel.getPose(ship);
	shipmodel.setPose(ship, x,y,z-0.5,a,0,0);
end

function restart_log()
	ship_log = io.open("ship.log", "w");
end

function setup_waving()
	user.fr_waving_wind			=	0;
	user.fr_waving_omega		=	2.3;
	user.fr_waving_narrowness	=	800;
	game.setWaving( false );
	game.setWaving( true );
end

-------------------------------------------------------------------------------
-- rolling stuff :
-------------------------------------------------------------------------------
function do_rolling()	

	print("Virtual rolling started");
	
	user.fr_waving_wind		=	0;
	user.fr_waving_omega	=	0;
	game.setWaving( false );
	game.setWaving( true );
	
	
	local roll_log 		= io.open("roll.log", "w");
	local x,y,z, a,b,c	= shipmodel.getPose(ship);
	
	record_counter = record_counter + 1;
	
	for roll=0, 180, 5 do 
		shipmodel.setPose(ship, x,y,z, 0,0,roll);
		
		print("...roll = "..roll);

		local tx = 0;
		local ty = 0;
		local tz = 0;
		local num = 20
		
		for n=1, num do
			shipmodel.updateForces(ship);
			local sd 	= shipmodel.getDynamics(ship);
			tx = tx + sd.torque_x / num;
			ty = ty + sd.torque_y / num;
			tz = tz + sd.torque_z / num;
		end
		
		local out	= string.format("%f %f %f %f", roll, tx, ty, tz);
		
		roll_log:write(out.."\n");
		roll_log:flush();
	end
	
	roll_log:close();

	shipmodel.setPose(ship, x,y,z, a,b,c);

	print("Done!");
end

-------------------------------------------------------------------------------

local exp03_running	=	nil;
local EXP03_TIME	=	800;

function setup_exp03()

	if not exp03_running then
		exp03_running = 0
	else
		exp03_running = exp03_running + 1;
	end
	
	local a	=	0;
	local b = 	0;
	local n	=	0;
	
	if exp03_running ==  0 then a = 2; b = 4; n = 256; end;
	if exp03_running ==  1 then a = 3; b = 4; n = 256; end;
	if exp03_running ==  2 then a = 4; b = 4; n = 256; end;
	if exp03_running ==  3 then a = 5; b = 4; n = 256; end;
	if exp03_running ==  4 then a = 6; b = 4; n = 256; end;
	
	if exp03_running ==  5 then a = 2; b = 4; n = 64; end;
	if exp03_running ==  6 then a = 3; b = 4; n = 64; end;
	if exp03_running ==  7 then a = 4; b = 4; n = 64; end;
	if exp03_running ==  8 then a = 5; b = 4; n = 64; end;
	if exp03_running ==  9 then a = 6; b = 4; n = 64; end;
	
	if exp03_running == 10 then a = 2; b = 4; n = 16; end;
	if exp03_running == 11 then a = 3; b = 4; n = 16; end;
	if exp03_running == 12 then a = 4; b = 4; n = 16; end;
	if exp03_running == 13 then a = 5; b = 4; n = 16; end;
	if exp03_running == 14 then a = 6; b = 4; n = 16; end;
	
	if exp03_running == 15 then a = 2; b = 4; n = 4; end;
	if exp03_running == 16 then a = 3; b = 4; n = 4; end;
	if exp03_running == 17 then a = 4; b = 4; n = 4; end;
	if exp03_running == 18 then a = 5; b = 4; n = 4; end;
	if exp03_running == 19 then a = 6; b = 4; n = 4; end;
	
	if exp03_running == 20 then
		game.killEntity( ship );
		ship		=	ships.createCutter(5, 0, -0.86, 90,0,0);
		ship_log 	= 	nil;
	end
	
	user.fr_waving_wind			=	0;
	user.fr_waving_omega		=	2.3 * a / b;
	user.fr_waving_narrowness	=	n;
	game.setWaving( false );
	game.setWaving( true );

	game.killEntity( ship );
	ship	=	ships.createCutter(5, 0, -0.86, 90,0,0);
	ship_log = io.open(string.format("ship_pr_%03d_%02d_%02d.log", n, a,b), "w");
end


--
--	frame()
--
function frame( dtime )

	if not ship_log then
		restart_log();
	end

	core.debugString("FPS  : "..1/dtime);
	
	control.update( dtime, ship );

	driveShip( dtime, ship );
	
	local x,y,z,a,b,c = shipmodel.getPose(ship);
	shipmodel.setPose(ship, 50,50,z,90,b,c);
	
	--
	--	drawing :
	--
	dv.setColor(1,1,1,1);
	
	local sd = shipmodel.getDynamics(ship);
	
	
	if sd.roll > 120  then shipwreck = true end
	if sd.roll < -120 then shipwreck = true end

	record_counter = record_counter + 1;
	if record_counter > 5 then
		if not shipwreck and ship_log then
			local out	= string.format("%f %f %f %f %f %f %f %f", sd.time, sd.yaw, sd.pitch, sd.roll, sd.position_x, sd.position_y, sd.position_z, sd.wave_height);
			ship_log:write(out.."\n");
			ship_log:flush();
		end
		record_counter = 0;
	end

	if exp03_running then
		core.debugString("* EXPERIMENT IS RUNNING *");
		if sd.time > EXP03_TIME then
			setup_exp03()
		end
	end
	

	core.debugString("Time ", sd.time);
	
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






