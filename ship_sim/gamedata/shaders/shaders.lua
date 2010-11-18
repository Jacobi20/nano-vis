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

dofile('shaders/shader_lib.lua');

-------------------------------------------------------------------------------
--	Shaders :
-------------------------------------------------------------------------------

--define_dsn_shader("textures/plain/steel.tga");

define_water_shader("textures/water.tga");
--define_diffuse_shader("textures/water.tga");

fr.define_shader {
	name			=	"*color7F7F7FFF";
	is_solid		=	true;
	
	injection		=	[[
		surface.diffuse = 	float3(0.25, 0.25, 0.25);
	]];
		
}

fr.define_shader {
	name			=	"textures/plain/steel.tga";
	is_solid		=	true;
	
	injection		=	[[
		surface.diffuse = 	float3(0.35, 0.35, 0.35);
	]];
		
}


fr.define_shader {
	name			=	"textures/ocean_sky.tga";
	is_solid		=	true;
	is_emissive		=	true;
	texture_path0 	=	"textures/ocean_sky.tga";
	
	injection		=	[[
		surface.diffuse		=	float3(0,0,0);
		surface.emission	=	sample_color( sampler0, input.uv0 );
	]];
}