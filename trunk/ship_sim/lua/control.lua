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
	
module	("control");

-------------------------------------------------------------------------------
--	CONTROL
-------------------------------------------------------------------------------

local yaw 	= 0;
local pitch = 0;
local roll 	= 0;
local posx 	= 0;
local posy 	= 0;
local posz 	= 10;

local	l_camera_posx  = 2;
local	l_camera_posy  = 0;
local	l_camera_posz  = 12;
local	l_camera_yaw   = -90;
local	l_camera_pitch = 0;
local	l_camera_roll  = 90;

local	target_interp  = 0.0;
local	current_interp = 0.0;

state = {
		fw	=	false;	--	forward
		bw	=	false;	--	backward
		sl	=	false;	--	step left
		sr	=	false;	--	step right
		tl	=	false;	--	turn left
		tr	=	false;	--	turn right
		tu	=	false;	--	turn up
		td	=	false;	--	turn down
		up	=	false;	--	up
		dn	=	false;	--	down
		iu	=	false;	--	interp on
		id	=	false;	--	interp off
	};

input.bind ( "S",	 	"control.state.fw = true", "control.state.fw = false" );
input.bind ( "Z", 		"control.state.bw = true", "control.state.bw = false" );
input.bind ( "A", 		"control.state.sl = true", "control.state.sl = false" );
input.bind ( "X", 		"control.state.sr = true", "control.state.sr = false" );
input.bind ( "LEFT", 	"control.state.tl = true", "control.state.tl = false" );
input.bind ( "RIGHT", 	"control.state.tr = true", "control.state.tr = false" );
input.bind ( "UP", 		"control.state.tu = true", "control.state.tu = false" );
input.bind ( "DOWN",	"control.state.td = true", "control.state.td = false" );
input.bind ( "SPACE", 	"control.state.up = true", "control.state.up = false" );
input.bind ( "C", 		"control.state.dn = true", "control.state.dn = false" );
input.bind ( "P",		"control.state.iu = true", "control.state.iu = false" );
input.bind ( "O",		"control.state.id = true", "control.state.id = false" );

local ANGULAR_VELOCITY	=	90;
local LINEAR_VELOCITY	=	20;
local SENSITIVITY		=	0.07;
	
function update(dtime, ship)

	local dx, dy, dz = 0,0,0;
	
	if state.tl then yaw 	= yaw	+ dtime * ANGULAR_VELOCITY;			end
	if state.tr then yaw 	= yaw	- dtime * ANGULAR_VELOCITY;			end
	if state.tu then pitch 	= pitch	+ dtime * ANGULAR_VELOCITY * 0.5;	end
	if state.td then pitch	= pitch	- dtime * ANGULAR_VELOCITY * 0.5;	end
	
	yaw 	=	yaw 	-	input.mouse.dx * SENSITIVITY;
	pitch 	=	pitch 	-	input.mouse.dy * SENSITIVITY;
	if pitch >  85 then pitch =  85; end
	if pitch < -85 then pitch = -85; end
	
	if state.fw then
		dx	=	dx + LINEAR_VELOCITY * math.cos( math.rad( yaw ) );
		dy	=	dy + LINEAR_VELOCITY * math.sin( math.rad( yaw ) );
	end
	if state.bw then
		dx	=	dx - LINEAR_VELOCITY * math.cos( math.rad( yaw ) );
		dy	=	dy - LINEAR_VELOCITY * math.sin( math.rad( yaw ) );
	end

	if state.sl then
		dx	=	dx + LINEAR_VELOCITY * math.cos( math.rad( yaw + 90 ) );
		dy	=	dy + LINEAR_VELOCITY * math.sin( math.rad( yaw + 90 ) );
	end
	if state.sr then
		dx	=	dx - LINEAR_VELOCITY * math.cos( math.rad( yaw + 90 ) );
		dy	=	dy - LINEAR_VELOCITY * math.sin( math.rad( yaw + 90 ) );
	end
	
	if state.dn then 
		dz	=	- 0.5 * LINEAR_VELOCITY;
	end
	if state.up then 
		dz	=	0.5 * LINEAR_VELOCITY;
	end
	
	if state.iu then
		target_interp = 1;
	end
	if state.id then
		target_interp = 0;
	end
	
	current_interp = 0.9*current_interp + 0.1*target_interp;
	if current_interp > 1.0 then 
		current_interp = 1.0; 
	end

	local result_view_x, result_view_y, result_view_z, result_yaw, result_pitch, result_roll;

	local l_x, l_y, l_z, l_yaw, l_pitch, l_roll = game.getLocalCamera(ship, l_camera_posx, l_camera_posy, l_camera_posz, yaw, pitch, roll);
	
	if target_interp == 0 then
		posx = posx + dx * dtime;
		posy = posy + dy * dtime;
		posz = posz + dz * dtime;
	end
	
	result_view_x = posx + (l_x - posx)*current_interp;
	result_view_y = posy + (l_y - posy)*current_interp;
	result_view_z = posz + (l_z - posz)*current_interp;
	
	if target_interp == 1 then
		result_yaw		=	l_yaw;
		result_pitch	=	l_pitch;
		result_roll		=	l_roll;
	else
		result_yaw		=	yaw;
		result_pitch	=	pitch;
		result_roll		=	roll;
	end
	
	game.setView(result_view_x, result_view_y, result_view_z, result_yaw, result_pitch, result_roll);

	game.setProj(120, 1.1, 4000, 1);
end

























