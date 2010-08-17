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


-------------------------------------------------------------------------------
--	shader library
-------------------------------------------------------------------------------

function define_dsn_shader ( path )
	local injection = [[
		surface.diffuse 	= 	sample_color ( sampler0, input.uv0 ).rgb;
		surface.specular	=	sample_color ( sampler1, input.uv0 ).r;
		surface.roughness	=	sample_color ( sampler1, input.uv0 ).g;
		surface.metalness	=	sample_color ( sampler1, input.uv0 ).b;
		surface.normal		=	sample_normal( sampler2, input.uv0 ).xyz;
	]];

	fr.define_shader {
		name			=	path;
		texture_path0	=	path;
		texture_path1	=	postfix(path, "_spec");
		texture_path2	=	postfix(path, "_local");
		effect_path		=	inject_surface_fx( injection );
		is_solid		=	true;
	}
end


function define_diffuse_shader ( path )
	local injection = [[
		surface.diffuse 	= 	sample_color ( sampler0, input.uv0 );
	]];

	fr.define_shader {
		name			=	path;
		texture_path0	=	path;
		effect_path		=	inject_surface_fx( injection );
		is_solid		=	true;
	}
end


function define_water_shader ( path )
	local injection = [[
		float2 offset		=	0.01*float2(time*2, time);
		float2	uv0			=	sample_normal ( sampler1, input.uv0*7+offset*2 ).xy * 0.1;
				uv0			+=	sample_normal ( sampler1, -input.uv0*7+offset*3 ).xy * 0.07;
				uv0			+=	sample_normal ( sampler1, input.uv0*7+offset*5 ).xy * 0.2;
		surface.diffuse 	= 	sample_color ( sampler0, input.uv0 + 0.05*normalize(uv0)).rgb;
		surface.alpha		=	sample_color ( sampler0, input.uv0 + 0.05*normalize(uv0)).a+0.3;

		surface.diffuse 	= 	float3(0.5, 0.5, 1.0);
		surface.alpha		=	0.5;
	]];

	fr.define_shader {
		name			=	path;
		texture_path0	=	"textures/waves_color.tga";
		texture_path1	=	"textures/waves.tga";
		effect_path		=	inject_surface_fx( injection );
		is_solid		=	false;
		is_translucent	=	true;
	}
end
