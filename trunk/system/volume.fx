/*-----------------------------------------------------------------------------
	VOLUME.FX
-----------------------------------------------------------------------------*/

#pragma pack_matrix(row_major)

float4x4	matrix_world 	;
float4x4	matrix_view 	;
float4x4	matrix_proj 	;
float4x4	matrix_box		;
float4		view_point		;
float		slice_num		;
float		vol_scale		;

// float    time                 ;
// float4x4 matrix_proj          ;
// float4x4 matrix_wv            ;
// float4x4 matrix_wv_i          ;
// float    v_distance           ;
// float4   rcp_screen_size      ;

texture	volume_data_tex;
texture	palette_tex;

sampler3D volume_data_sm = sampler_state
{
	Texture = (volume_data_tex);
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
};

sampler2D palette_sm = sampler_state
{
	Texture 	= (palette_tex);
	AddressU	= CLAMP;
	AddressV	= CLAMP;
    MIPFILTER 	= LINEAR;
    MAGFILTER 	= LINEAR;
    MINFILTER 	= LINEAR;
};




struct VS_INPUT {
	float3 	pos 	: POSITION0;
	float4	color	: COLOR0;
	float2	uv0		: TEXCOORD0;
	float2	uv1		: TEXCOORD1;
	float3 	normal	: NORMAL;
};

struct VS_OUTPUT {
	float4 	pos 	: POSITION0;
	float4	color	: COLOR0;
	float2 	uv0		: TEXCOORD0;
	float2 	uv1		: TEXCOORD1;
	float3 	pos3d	: TEXCOORD2;
	float3 	normal	: TEXCOORD3;
};


/*-----------------------------------------------------------------------------
	VSMain
-----------------------------------------------------------------------------*/

VS_OUTPUT VSMain( VS_INPUT input )
{
	VS_OUTPUT	output;

	float4	pos		=	float4(input.pos.xyz, 1);
	float4	norm	=	float4(input.normal.xyz, 0);


	pos				=	mul(pos, matrix_box);
	pos				=	mul(pos, matrix_world);
	output.pos3d	=	pos.xyz / pos.w;
	pos				=	mul(pos, matrix_view);
	pos				=	mul(pos, matrix_proj);
	output.pos		=	pos;
	
	output.normal	=	normalize(norm);
	
	
	output.uv0		=	input.uv0;
	output.uv1		=	input.uv1;
	output.color	=	input.color;
	
	
	return output;
}


/*-----------------------------------------------------------------------------
	PSMain / Trace
-----------------------------------------------------------------------------*/

float4	PSMainTrace( in VS_OUTPUT input, float2 vpos : VPOS ) : COLOR0
{
	float4 sample 	= 0;
	float3 uvw    	= float3(input.uv0.xy, input.uv1.x);
	float4 s  		= tex3D(volume_data_sm, uvw);
	float4 rgba		= tex2D(palette_sm, float2(s.r, 0.5));
	
	float3 v = normalize(input.pos3d - view_point.xyz);
	float3 n = input.normal;
	float scale = abs(dot(v, n));
	
	rgba.a *= scale;
	rgba.a /= slice_num;
	rgba.a *= vol_scale;

	return rgba;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/

technique volume
{
	pass Pass_0 {
		DestBlend 			= INVSRCALPHA;
		SrcBlend 			= SRCALPHA;
		ZEnable 			= TRUE;
		ZWriteEnable 		= FALSE;
		CullMode 			= NONE;
		AlphaBlendEnable 	= TRUE;
		VertexShader 		= compile vs_2_0 VSMain();
		PixelShader 		= compile ps_2_0 PSMainTrace();
	}
}

