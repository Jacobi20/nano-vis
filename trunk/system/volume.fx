/*-----------------------------------------------------------------------------
	VOLUME.FX
-----------------------------------------------------------------------------*/

#pragma pack_matrix(row_major)

float4x4	matrix_world 	;
float4x4	matrix_view 	;
float4x4	matrix_proj 	;

// float    time                 ;
// float4x4 matrix_proj          ;
// float4x4 matrix_wv            ;
// float4x4 matrix_wv_i          ;
// float    v_distance           ;
// float4   rcp_screen_size      ;

texture	volume_data_tex;

sampler3D volume_data_sm = sampler_state
{
	Texture = (volume_data_tex);
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
};




struct VS_INPUT {
	float3 	pos 	: POSITION0;
	float4	color	: COLOR0;
	float2	uv0		: TEXCOORD0;
	float2	uv1		: TEXCOORD1;
};

struct VS_OUTPUT {
	float4 	pos 	: POSITION0;
	float4	color	: COLOR0;
	float2 	uv0		: TEXCOORD0;
	float2 	uv1		: TEXCOORD1;
	float4 	pos3d	: TEXCOORD2;
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
	output.uv0		=	input.uv0;
	output.uv1		=	input.uv1;
	output.color	=	input.color;
	output.pos3d	=	float4(input.pos.xyz, 1);
	
	return output;
}


/*-----------------------------------------------------------------------------
	PSMain 
-----------------------------------------------------------------------------*/

float4	PSMain( in VS_OUTPUT input, float2 vpos : VPOS ) : COLOR0
{
	float4 sample 	= 0;
	float3 uvw    	= float3(input.uv0.xy, input.uv1.x);
	
	float4 s  		= tex3D(volume_data_sm, uvw);
	
	return 0.01*s.r;
}

/*-----------------------------------------------------------------------------
	PSMain / Trace
-----------------------------------------------------------------------------*/

float4	PSMainTrace( in VS_OUTPUT input, float2 vpos : VPOS ) : COLOR0
{
	float4 sample 	= 0;
	float3 uvw    	= float3(input.uv0.xy, input.uv1.x);
	
	float4 s  		= tex3D(volume_data_sm, uvw);
	
	return 0.1*s.r;
}

/*-----------------------------------------------------------------------------
	PSMain / Isolines
-----------------------------------------------------------------------------*/

float Isoline(float s0, float s1, float s2, float s3, float h)
{
	float s = 0;

	if ((s0<h && s1>h) ||
		(s1<h && s0>h) ||
		(s2<h && s3>h) ||
		(s3<h && s2>h)) {
		s = 1;
	} else {
		s = 0;
	}

	return s;
}


float4	PSMainIso( in VS_OUTPUT input, float2 vpos : VPOS ) : COLOR0
{
	float4 sample = 0;
	
	float2 tex0 = input.uv0.xy;

	//	изменение uv-координат при перемещении на 1 пиксель вдоль оси x и y:
	float udx = 0.5 * ddx(input.uv0.x);
	float vdx = 0.5 * ddx(input.uv0.y);
	float udy = 0.5 * ddy(input.uv0.x);
	float vdy = 0.5 * ddy(input.uv0.y);

	
	// //	слой 3D-текстуры, откуда будет браться данные для отображения :
	float depth = input.uv1.x;

	//	чтение значений из текстуры:
	//	s - центральное значение
	//	s0, s1, s2, s3 - значения прочитанные со сдвигом
	float4 s  = tex3D(volume_data_sm, float3(tex0, depth));
	float4 s0 = tex3D(volume_data_sm, float3(tex0 + float2(-udx, -vdx), depth));
	float4 s1 = tex3D(volume_data_sm, float3(tex0 + float2(+udx, +vdx), depth));
	float4 s2 = tex3D(volume_data_sm, float3(tex0 + float2(-udy, -vdy), depth));
	float4 s3 = tex3D(volume_data_sm, float3(tex0 + float2(+udy, +vdy), depth));

	sample = s.x;
	
	
	//	построение изолиний :
#if 1
	sample = 0;
	float dh = 0.05;
	for (float h=0.05; h<=1; h+=dh) {
		float isoline = ( Isoline(s0.x, s1.x, s2.x, s3.x, h) );
		sample += (0.6 * isoline);
	}
#endif	
	
	//	получение значений из палитры :
	float4 colored = 0.2 * sample;//tex2D(tex1, float2(sample, 0.5));
		
	return colored;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/

technique volume
{
	pass Pass_0 {
		DestBlend 			= ONE;
		SrcBlend 			= ONE;
		ZEnable 			= TRUE;
		ZWriteEnable 		= FALSE;
		CullMode 			= NONE;
		AlphaBlendEnable 	= TRUE;
		VertexShader 		= compile vs_3_0 VSMain();
		PixelShader 		= compile ps_3_0 PSMainTrace();
	}
}

