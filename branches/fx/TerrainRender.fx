//===============================
//	Global Variables
//===============================
float4x4 matVP;
float3   vecEye;
bool	 showLayer[5];

//==================================
//	Fog
//==================================
bool	bFog = true;
float	fogNear = 500.0f;
float	fogFar  = 1000.0f;

//==================================
//	Light
//==================================
bool	 bLight = true;
int		 lightType = 0;
float	 lightRange = 1000.0f;
float3   vecLight = { 0.0f, 1.0f, 0.0f };
float4   l_a      = { 0.1f, 0.1f, 0.1f, 1.0f };
float4	 l_d	  = { 1.0f, 1.0f, 1.0f, 1.0f };

//==================================
//	Sampler
//==================================
texture baseDiffuseTex;
sampler baseDiffuseSamp = sampler_state
{
	texture		= < baseDiffuseTex >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
	
	AddressU    = Wrap;
	AddressV    = Wrap;
};

texture splattingDiffuseTex1;
sampler splattingDiffuseSamp1 = sampler_state
{
	texture		= < splattingDiffuseTex1 >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
};

texture splattingDiffuseTex2;
sampler splattingDiffuseSamp2 = sampler_state
{
	texture		= < splattingDiffuseTex2 >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
};

texture splattingDiffuseTex3;
sampler splattingDiffuseSamp3 = sampler_state
{
	texture		= < splattingDiffuseTex3 >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
};

texture splattingDiffuseTex4;
sampler splattingDiffuseSamp4 = sampler_state
{
	texture		= < splattingDiffuseTex4 >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
};

texture alphaTex;
sampler alphaSamp = sampler_state
{
	texture		= < alphaTex >;
	MinFilter	= None;
	MipFilter	= None;
	MagFilter	= None;
};

//========================================================================
//	PatchRender
//========================================================================

//===============================
//	Vertex Shader
//===============================
void PatchVS(	float3 pos  : POSITION0,
				float3 norm : NORMAL,
				float4 col  : COLOR0,
				float2 tex	: TEXCOORD0,
				out float4 outPos  : POSITION0,
				out float4 outCol  : COLOR0,
				out float2 outTex  : TEXCOORD0,
				out float3 outNorm : TEXCOORD1,
				out float  outFog  : TEXCOORD2 )
{
	outPos  = mul( float4(pos, 1.0f), matVP );
	outCol  = col;
	outTex  = tex;
	outNorm = norm;
	outFog	= length( pos - vecEye );
}

//===============================
//	Pixel Shader
//===============================
void PatchPS(	float4 col  : COLOR0,
				float2 tex : TEXCOORD0,
				float3 norm  : TEXCOORD1,
				float  fog   : TEXCOORD2,
				out float4 outCol : COLOR0 )
{
	float3 normN = normalize( norm );
	float4 texCol = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 alphaCol = tex2D( alphaSamp, tex );
	
	if( showLayer[4] )
		texCol = tex2D( baseDiffuseSamp, tex );
	
	if( showLayer[0] )
		texCol = texCol * (1.0f - alphaCol.a) + tex2D( splattingDiffuseSamp1, tex ) * alphaCol.a;
		
	if( showLayer[1] )
		texCol = texCol * (1.0f - alphaCol.r) + tex2D( splattingDiffuseSamp2, tex ) * alphaCol.r;
		
	if( showLayer[2] )
		texCol = texCol * (1.0f - alphaCol.g) + tex2D( splattingDiffuseSamp3, tex ) * alphaCol.g;
	
	if( showLayer[3] )
		texCol = texCol * (1.0f - alphaCol.b) + tex2D( splattingDiffuseSamp4, tex ) * alphaCol.b;
	
	float diffuseFactor = 1.0f;
	float fogFactor = 0.0f;
	
	if( bLight )
		diffuseFactor = dot( normN, vecLight );
	
	if( bFog )
		fogFactor = fogNear > fog ? 0 : (fog - fogNear) / (fogFar - fogNear);
	
	outCol = l_a * texCol + max(0.0f, ( diffuseFactor * (texCol * col * l_d) - fogFactor ));
}

//===============================
//	Technique
//===============================
technique PatchRender
{
	pass P0
	{
		VertexShader = compile vs_2_0 PatchVS();
		PixelShader  = compile ps_2_0 PatchPS();
	}
}