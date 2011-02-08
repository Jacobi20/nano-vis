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

local ANGULAR_VELOCITY	=	90;
local LINEAR_VELOCITY	=	20;
local SENSITIVITY		=	0.07;
	
function update(dtime)

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
		dx	=	LINEAR_VELOCITY * math.cos( math.rad( yaw ) );
		dy	=	LINEAR_VELOCITY * math.sin( math.rad( yaw ) );
	end
	if state.bw then
		dx	=	- LINEAR_VELOCITY * math.cos( math.rad( yaw ) );
		dy	=	- LINEAR_VELOCITY * math.sin( math.rad( yaw ) );
	end

	if state.sl then
		dx	=	LINEAR_VELOCITY * math.cos( math.rad( yaw + 90 ) );
		dy	=	LINEAR_VELOCITY * math.sin( math.rad( yaw + 90 ) );
	end
	if state.sr then
		dx	=	- LINEAR_VELOCITY * math.cos( math.rad( yaw + 90 ) );
		dy	=	- LINEAR_VELOCITY * math.sin( math.rad( yaw + 90 ) );
	end
	
	if state.dn then 
		dz	=	- 0.5 * LINEAR_VELOCITY;
	end
	if state.up then 
		dz	=	0.5 * LINEAR_VELOCITY;
	end

	posx = posx + dx * dtime;
	posy = posy + dy * dtime;
	posz = posz + dz * dtime;

	game.setView(posx, posy, posz, yaw, pitch, roll);
	game.setProj(120, 1.1, 500, 1);
end

























