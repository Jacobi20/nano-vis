-------------------------------------------------------------------------------
-- The MIT License
--
-- Copyright (c) 2010 IFMO/GameDev Studio
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
-- THE SOFTWARE.
-------------------------------------------------------------------------------

local string		=	require("string");
local render		=	require("render");
local print			=	print;

-------------------------------------------------------------------------------
--	Shaders :
-------------------------------------------------------------------------------

module("shaders");

-------------------------------------------------------------------------------
--	shader library
-------------------------------------------------------------------------------

--
--	postfix
--
local function postfix(path, postfix)
	local ext	=	string.sub(path, -3, -1);
	local name	=	string.sub(path,  0, -5);
	return name .. postfix .. "." .. ext;
end


--
--	defineDiffuseShader
--
local function defineDiffuseShader ( path )
	local surface = [[
		surface.diffuse 	= 	sampleColor ( sampler0, input.uv0 );	
	]];

	render.defineShader {
		name			=	path;
		texture_path0	=	path;
		injection		=	surface;
		is_solid		=	true;
	}
end


--
--	defineDsnShader
--
local function defineDsnShader ( path )
	local surface = [[
		surface.diffuse 	= 	sampleColor ( sampler0, input.uv0 );	
		surface.specular	=	sampleColor ( sampler1, input.uv0 ).r;	
		surface.roughness	=	sampleColor ( sampler1, input.uv0 ).g;	
		surface.metalness	=	sampleColor ( sampler1, input.uv0 ).b;
		surface.normal		=	sampleNormal( sampler2, input.uv0 );	
	]];
	
	render.defineShader {
		name			=	path;
		
		texture_path0	=	path;
		texture_path1	=	postfix(path, "_spec");
		texture_path2	=	postfix(path, "_local");
		
		injection		=	surface;
		
		is_solid		=	true;
	}
end



-------------------------------------------------------------------------------
--	shader list itself :
-------------------------------------------------------------------------------

--
--	createShaderFallback
--
function defineShaderFB(path)
	defineDiffuseShader(path);
end


--
--	createShaders
--
function defineShaders()
	defineDsnShader("textures/test/floor01.tga");
end










