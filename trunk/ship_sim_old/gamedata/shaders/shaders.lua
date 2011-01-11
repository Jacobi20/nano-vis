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
		surface.diffuse = 	float3(0.15, 0.15, 0.15);
	]];
		
}

fr.define_shader {
		name			=	"*color000000FF";
		is_solid		=	true;
	injection		=	[[
		surface.diffuse		=	0;
	]]
	};
	

function ship_shader (path)
fr.define_shader {
	name			=	path;
	is_solid		=	true;
	is_emissive		=	true;
	texture_path0	=	path;
	texture_path1	=	postfix(path, "_spec");
	texture_path2	=	postfix(path, "_local");
	texture_path3	=	"textures/ocean_reflection.tga";
	
	injection		=	[[
		surface.diffuse		=	0.1*sample_color ( sampler0, input.uv0 ) + 0.25*sample_color ( sampler0, 4*input.uv0 );
		surface.normal		=	sample_normal( sampler2, input.uv0 );


		float3	normal		=	getWorldNormal(surface.normal);
				normal		=	normalize( normal );
				
		float3	ray			=	normalize( view_position.xyz - input.wpos.xyz );
		float	fade		=	1 - (normalize(reflect( ray, normal )).z*0.5 + 0.5);
		float2	refl_uv		=	normalize(reflect( ray, normal )).xy;
				refl_uv.y	=	-refl_uv.y;
				refl_uv		*=	0.98;
		float3	refl		=	sample_color( sampler3, 0.5*(refl_uv+1) ).rgb;
		float3	refl2		=	tex2Dlod( sampler3, float4(0.5*(refl_uv+1),0,8) ).rgb;

		float	fresnel		=	1 - abs(dot( ray, normal ));
		fresnel		=	0.05 + 0.95*pow( saturate(fresnel), 10 );

		surface.emission	=	refl * 10*surface.diffuse * fresnel * fade + 
								refl2 * 0.2*surface.diffuse * fade;
	]];
}
end	
	
ship_shader("textures/uboat_side.tga");
ship_shader("textures/uboat_waterline.tga");
	

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