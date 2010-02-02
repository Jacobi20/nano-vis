/*-----------------------------------------------------------------------------
	ATOM.FX
-----------------------------------------------------------------------------*/

#pragma pack_matrix(row_major)

float4x4	matrix_world 	;
float4x4	matrix_view 	;
float4x4	matrix_proj 	;
float4x4	matrix_w_it		;	
float4x4	matrix_wv_it	;	
float4		light_dir		;
float4		view_dir		;
float4		atom_color		;


struct VS_INPUT {
	float3 	pos 	: POSITION0;
	float4	color	: COLOR0;
	float3	normal	: NORMAL;
};

struct VS_OUTPUT {
	float4 pos 		: POSITION0;
	float4 color	: COLOR0;
	float4 normal	: TEXCOORD2;
};


/*-----------------------------------------------------------------------------
	VSMain
-----------------------------------------------------------------------------*/

VS_OUTPUT VSMain( VS_INPUT input )
{
	VS_OUTPUT	output;
	
	float4	pos		=	float4(input.pos.xyz, 1);
	float4	normal	=	float4(input.normal.xyz, 0);
	
	normal			=	normalize(mul(normal, matrix_w_it));
	
	pos				=	mul(pos, matrix_world);
	pos				=	mul(pos, matrix_view);
	pos				=	mul(pos, matrix_proj);
	
	output.pos		=	pos;
	output.normal	=	normal;
	output.color	=	input.color;
	
	return output;
}


/*-----------------------------------------------------------------------------
	PSMain
-----------------------------------------------------------------------------*/

// float4 PSMainNoLight(VS_OUTPUT input) : COLOR0
// {
	// return input.color;
// }

float4 PSMain(VS_OUTPUT input) : COLOR0
{
	float n_dot_l 	= 1*pow(saturate(0.7+0.3*dot(input.normal, light_dir)), 2);
	float n_dot_v 	= 1*pow(saturate(1 - abs(dot(input.normal, view_dir ))), 2);
	
	float4	h		= normalize(view_dir + light_dir);
	float n_dot_h 	= 0*pow(saturate(dot(h, normalize(input.normal) )), 64);

	return pow(0.5 * (n_dot_l * atom_color + n_dot_v), 0.7) + 0.7*n_dot_h;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/

technique axis
{
	pass Pass_0 {
		VertexShader 		= compile vs_2_0 VSMain();
		PixelShader 		= compile ps_2_0 PSMain();
	}
}

technique solid_body
{
	pass Pass_0
	{
		DestBlend 			= ONE;
		SrcBlend 			= ONE;
		ZEnable 			= TRUE;
		ZWriteEnable		= TRUE;
		CullMode 			= CW;
		AlphaBlendEnable	= FALSE;

		VertexShader 		= compile vs_2_0 VSMain();
		PixelShader 		= compile ps_2_0 PSMain();
	}
}