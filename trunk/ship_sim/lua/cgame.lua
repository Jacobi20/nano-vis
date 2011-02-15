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
	
module	("cgame");

-------------------------------------------------------------------------------
--	CGAME
-------------------------------------------------------------------------------

local	ship		=	nil;

--
--	init()
--
function init()
	game.setGravity(9.8);

	game.setWaving( true );
	game.setAmbient( 0.7, 0.7, 0.7 );

	ship	=	ships.createSSN668(10,0,10, 0,0,0);
end


--
--	frame()
--
function frame( dtime )
	core.debugString("FPS : "..1/dtime);

	control.update( dtime );
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






