/*-----------------------------------------------------------------------------
	ATOM.FX
-----------------------------------------------------------------------------*/

#pragma pack_matrix(row_major)

float4x4	matrix_world 	;
float4x4	matrix_view 	;
float4x4	matrix_proj 	;


struct VS_INPUT {
	float3 	pos 	: POSITION0;
	float4	color	: COLOR0;
};

struct VS_OUTPUT {
	float4 pos 		: POSITION0;
	float4 color	: COLOR0;
};


/*-----------------------------------------------------------------------------
	VSMain
-----------------------------------------------------------------------------*/

VS_OUTPUT VSMain( VS_INPUT input )
{
	VS_OUTPUT	output;
	
	float4	pos		=	float4(input.pos.xyz, 1);
	pos				=	mul(pos, matrix_world);
	pos				=	mul(pos, matrix_view);
	pos				=	mul(pos, matrix_proj);
	
	output.pos		=	pos;
	output.color	=	input.color;
	
	return output;
}


/*-----------------------------------------------------------------------------
	PSMain
-----------------------------------------------------------------------------*/

float4 PSMain(VS_OUTPUT input) : COLOR0
{
	return input.color;
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

