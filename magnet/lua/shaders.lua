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


local function defineWaterShader()
	render.defineShader {
		name			=	"textures/water.tga";
		texture_path0	=	"textures/wave_grid.tga";
		texture_path1	=	"textures/ocean_local.tga";
		texture_path2	=	"textures/foam.tga";
		texture_path3	=	"textures/ocean_reflection.tga";
		injection		=	injection;
		is_solid		=	true;
		is_emissive		=	true;
		
		injection		=	[[
			Water( surface, input );
		]];
	}
end

local function defineSkyShader()
	render.defineShader {
		name			=	"textures/ocean_sky.tga";
		texture_path0	=	"textures/ocean_sky.tga";
		is_solid		=	true;
		is_emissive		=	true;
		
		injection		=	[[
			surface.diffuse 	= 	float3( 0, 0, 0 );	
			surface.emission	=	sampleColor ( sampler0, input.uv0 );	
		]];
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
	print ("defineShaders() ENTER");
	defineDsnShader("textures/test/floor01.tga");
	defineDsnShader("scenes/submarine_TM.bmp");
	defineWaterShader();
	defineSkyShader();
	print ("defineShaders() LEAVE");
end










