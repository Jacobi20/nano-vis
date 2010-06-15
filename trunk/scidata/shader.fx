/*-----------------------------------------------------------------------------
	ATOM.FX
-----------------------------------------------------------------------------*/

#pragma pack_matrix(row_major)

float4x4	matrix_world 	;
float4x4	matrix_view 	;
float4x4	matrix_proj 	;
float4		light_dir		;
float4		view_pos		;
float4		obj_color		;


struct VS_INPUT {
	float3 	pos 	: POSITION0;
	float3	normal	: NORMAL;
	float4	color	: COLOR;
};

struct VS_OUTPUT {
	float4 pos 		: POSITION0;
	float4 color	: COLOR0;
	
	float4 mpos		: TEXCOORD1;	// model space position
	float4 wpos		: TEXCOORD2;	// world space position
	float4 normal	: TEXCOORD3;	// world space normal
};


/*-----------------------------------------------------------------------------
	VSMain
-----------------------------------------------------------------------------*/

VS_OUTPUT VSMain( VS_INPUT input )
{
	VS_OUTPUT	output;
	
	float4	pos		=	float4(input.pos.xyz, 1);
	float4	normal	=	float4(input.normal.xyz, 0);
	
	normal			=	normalize(mul(normal, matrix_world));

	output.mpos		=	pos;
	pos				=	mul(pos, matrix_world);
	output.wpos		=	pos;
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

float4 PSMainXRay(VS_OUTPUT input, uniform float4 color) : COLOR0
{
	//	light  :
	float4 view_dir	= normalize(view_pos - input.wpos);
	float n_dot_l 	= 0*pow(saturate(0.7+0.3*dot(input.normal, light_dir)), 2);
	float n_dot_v 	= 1*pow(saturate(1 - abs(dot(input.normal, view_dir ))), 2);
	
	float4	h		= normalize(view_dir + light_dir);
	float n_dot_h 	= 0*pow(saturate(dot(h, normalize(input.normal) )), 64);
	
	//	grid :
	float grid 	= pow(1-abs(2*frac(0.2*input.mpos)-1), 10).x
				+ pow(1-abs(2*frac(0.2*input.mpos)-1), 10).y
				+ pow(1-abs(2*frac(0.2*input.mpos)-1), 10).z;
	
	//
	float4 result = 0;
	
	result += pow(0.5 * (n_dot_l * input.color + n_dot_v), 0.7) + 0.7*n_dot_h + 0.1*grid;
	
	result.r = pow(result.r, 1.0);
	result.g = pow(result.g, 1.0);
	result.b = pow(result.b, 2.0);
	
	return float4(result.rgb, 1) * color;
}

float4 PSWhite(VS_OUTPUT input) : COLOR0 {
	return 1;
}

float4 PSColor(VS_OUTPUT input, uniform float4 color) : COLOR0 {
	return color;
}

float4 PSMain(VS_OUTPUT input) : COLOR0 {
	return input.color;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/

technique debug {
	pass p0 {
		ZEnable 			= 	TRUE;
		ZWriteEnable		= 	FALSE;
		CullMode 			= 	NONE;
		DestBlend 			= 	INVSRCALPHA;
		SrcBlend 			= 	SRCALPHA;
		AlphaBlendEnable	= 	TRUE;
		ColorWriteEnable	=	RED|GREEN|BLUE;
		VertexShader 		= 	compile vs_2_0 VSMain();
		PixelShader 		= 	compile ps_2_0 PSMain();
	}
}


technique solid_body
{
	pass XRayBack {
		DestBlend 			= 	ONE;
		SrcBlend 			= 	ONE;
		ZEnable 			= 	TRUE;
		ZWriteEnable		= 	FALSE;
		CullMode 			= 	CCW;
		AlphaBlendEnable	= 	TRUE;
		ColorWriteEnable	=	RED|GREEN|BLUE;
		VertexShader 		= 	compile vs_2_0 VSMain();
		PixelShader 		= 	compile ps_2_0 PSMainXRay(1);
	}
	pass Black {
		DestBlend 			= 	SRCCOLOR;
		SrcBlend 			= 	ZERO;
		ZEnable 			= 	TRUE;
		ZWriteEnable		= 	TRUE;
		CullMode 			= 	CW;
		AlphaBlendEnable	= 	TRUE;
		ColorWriteEnable	=	RED|GREEN|BLUE;
		VertexShader 		= 	compile vs_2_0 VSMain();
		PixelShader 		= 	compile ps_2_0 PSColor(0.3);
	}
	pass XRay {
		DestBlend 			= 	ONE;
		SrcBlend 			= 	ONE;
		ZEnable 			= 	TRUE;
		ZWriteEnable		= 	FALSE;
		CullMode 			= 	CW;
		AlphaBlendEnable	= 	TRUE;
		ColorWriteEnable	=	RED|GREEN|BLUE;
		VertexShader 		= 	compile vs_2_0 VSMain();
		PixelShader 		= 	compile ps_2_0 PSMainXRay(float4(1,1,1,1));
	}
}
