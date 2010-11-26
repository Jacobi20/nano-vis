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

function postfix(path, postfix)
	local ext	=	string.sub(path, -3, -1);
	local name	=	string.sub(path,  0, -5);
	return name .. postfix .. "." .. ext;
end


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
		injection		=	injection;
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
		injection		=	injection;
		is_solid		=	true;
	}
end


function define_water_shader ( path )
	local injection = [[
	
		float3	water_color	=	0.25*float3(73.0f/256.0f, 65.0f/256.0f, 78.0f/256.0f);

		float2 	ofs			=	float2(time, 0);
		float3	normal		=	0;
		float	scale		=	0.5;
				normal		+=	8*sample_normal ( sampler1, scale*input.uv0/4 +  0.012*ofs ).rgb;
				normal		+=	4*sample_normal ( sampler1, scale*input.uv0/8  + 0.014*ofs ).rgb;
				normal		+=	2*sample_normal ( sampler1, scale*input.uv0/16 + 0.018*ofs ).rgb;
				normal		+=	1*sample_normal ( sampler1, scale*input.uv0/32 + 0.026*ofs ).rgb;
				
				normal		=	getWorldNormal(normal);
				
				normal		=	normalize( normal );
				
		float3	ray			=	normalize( view_position.xyz - input.wpos.xyz );
		float2	refl_uv		=	normalize(reflect( ray, normal )).xy;
				refl_uv.y	=	-refl_uv.y;
				refl_uv		*=	0.98;
		float3	refl		=	sample_color( sampler3, 0.5*(refl_uv+1) ).rgb;
		
		float	ff1			=	pow(saturate(input.color0.g - 3), 2);
		float	ff2			=	pow(saturate(input.color0.r), 32);
		
		float	foam_factor	=	ff1 * ff2 ;

		float	fresnel		=	1 - abs(dot( ray, normal ));
				fresnel		=	0.05 + 0.95*pow( saturate(fresnel), 10 );
		
		
		float3	foam		=	0;
				foam		+=	sample_color ( sampler2, input.uv0/1  + 0.012*ofs ).rgb;
				foam		+=	sample_color ( sampler2, input.uv0/2  + 0.015*ofs ).rgb;
				foam		+=	sample_color ( sampler2, input.uv0/3  + 0.022*ofs ).rgb;
				foam		+=	sample_color ( sampler2, input.uv0/4  + 0.052*ofs ).rgb;
				foam		*=	foam_factor;
				foam		/=	4;
				
		surface.diffuse 	= 	0;
		surface.emission	=	refl * fresnel + foam + water_color * (1-fresnel);
		surface.alpha		=	sample_color ( sampler1, input.uv0 ).a;
	]];

	fr.define_shader {
		name			=	path;
		texture_path0	=	"textures/wave_grid.tga";
		texture_path1	=	"textures/ocean_local.tga";
		texture_path2	=	"textures/foam.tga";
		texture_path3	=	"textures/ocean_reflection.tga";
		injection		=	injection;
		is_solid		=	true;
		is_emissive		=	true;
		is_translucent	=	false;
	}
end
