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
	
	--game.playCameraAnim('scenes/uboat_xxi.eax', 30);

	ship	=	ships.createCutter(0, 0, 0, 90,0,0);

	--	ship2	=	ships.createUBoat(-50,  0, 0, 45,0,0);
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


local	ship_data = {};
local	record_counter = 0;
local	degree	= 0;

input.bind ( "F7",	 	"cgame.do_rolling()", "" );

-------------------------------------------------------------------------------
-- rolling stuff :
-------------------------------------------------------------------------------
function do_rolling()	

	print("Virtual rolling started");

	local roll_log 		= io.open("roll.log", "w");
	local x,y,z, a,b,c	= shipmodel.getPose(ship);
	
	for roll=0, 180, 5 do 
		shipmodel.setPose(ship, x,y,z, 0,0,roll);
		
		print("...roll = "..roll);

		local tx = 0;
		local ty = 0;
		local tz = 0;
		
		for n=1, 20 do
			shipmodel.updateForces(ship);
			local sd 	= shipmodel.getDynamics(ship);
			tx = tx + sd.torque_x / 10;
			ty = ty + sd.torque_y / 10;
			tz = tz + sd.torque_z / 10;
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

--
--	frame()
--
function frame( dtime )

	core.debugString("FPS  : "..1/dtime);
	
	control.update( dtime, ship );

	driveShip( dtime, ship );
	
	--
	--	diagram :
	--
	-- if degree <= 120 then
		-- degree = degree + 1;
		
		-- local x,y,z,a,b,c = shipmodel.getPose(ship);
		-- shipmodel.setPose(ship, x,y,z,a,b,degree);
		-- local sd = shipmodel.getDynamics(ship);
		
		
	-- end
	
	-- local x,y,z,a,b,c = shipmodel.getPose(ship);
	-- shipmodel.setPose(ship, 0,0,z,90,b,c);

	
	--
	--	drawing :
	--
	dv.setColor(1,1,1,1);
	
	local sd = shipmodel.getDynamics(ship);
	
	dv.drawPoint( sd.position_x, sd.position_y, sd.position_z, 1 );
	
	if record_counter==0 then
		table.insert( ship_data, sd );
	end
	record_counter = record_counter + 1;
	if record_counter>10 then
		record_counter = 0;
	end
	
	if #ship_data > 100 then table.remove(ship_data, 1); end

	for i=2, #ship_data, 1 do
		dv.setColor(1,1,0,0.5);
		local x0	=	ship_data[i-1].position_x;
		local y0	=	ship_data[i-1].position_y;
		local z0	=	ship_data[i-1].position_z;
		local x1	=	ship_data[i].position_x;
		local y1	=	ship_data[i].position_y;
		local z1	=	ship_data[i].position_z;
		dv.drawLine( x0,y0,z0, x1,y1,z1 );
		
		--plotter.makePlot2D( ship_data,  1,1, "time", "position_z", 10,10,10 );
		--plotter.makePlot2D( ship_data,  1,1, "time", "wave_height", 2,0,0 );
		local offset = ship_data[1].time;
		
		dv.setColor(1,1,0,0.5);
		dv.drawLine(	0, ship_data[i-1].time 	- offset,	0.1 * ship_data[i-1].roll,	
						0, ship_data[i].time	- offset, 	0.1 * ship_data[i].roll 	);
						
		dv.setColor(1,0.5,0.5,0.5);
		dv.drawLine(	0, ship_data[i-1].time 	- offset,	0.1 * ship_data[i-1].pitch,	
						0, ship_data[i].time	- offset, 	0.1 * ship_data[i].pitch 	);
						
		dv.setColor(0,0.5,1,0.5);
		dv.drawLine(	0, ship_data[i-1].time 	- offset, 	1 * ship_data[i-1].wave_height,	
						0, ship_data[i].time 	- offset, 	1 * ship_data[i].wave_height 	);
						
	end
	
	dv.setColor(1,1,0,0.9);
	for x=0, ship_data[#ship_data].time, 1 do
		dv.drawPoint(0,x,0, 0.2);
	end
	for x=0, ship_data[#ship_data].time, 10 do
		dv.drawPoint(0,x,0, 1.0);
	end

	dv.drawLine(0,0,0, 0, ship_data[#ship_data].time+1, 0,0);
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






