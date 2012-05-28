//=========================================================================
//	Global Variable
//=========================================================================
float4x4	matWVP;
texture		envMap;


//=========================================================================
//	Sampler
//=========================================================================
sampler EnvMapSample = sampler_state
{
	Texture = < envMap >;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU  = Mirror;
	AddressV  = Mirror;
};

//=========================================================================
//	Vertex Shader
//=========================================================================
void SkyVS( in  float3 InPos		: POSITION0,
		out float4 OutPos	: POSITION0,
		out float3 OutTexCoord	: TEXCOORD0 )
{
	// Set z = 1 (instead 'Device Clear(ZBuffer=1.0)')
	OutPos = mul(float4(InPos, 1.0f), matWVP).xyww;

	// Use sky mesh vertex postion in local space, as texcoord into cubemap.
	OutTexCoord = InPos;
}

//=========================================================================
//	Pixel Shader
//=========================================================================
float4 SkyPS( in float3 envTex : TEXCOORD0 ) : COLOR
{
	return texCUBE(EnvMapSample, envTex);
}

//=========================================================================
//	Technique
//=========================================================================
technique SkyTech
{
	pass P0
	{
		CullMode	 = None;
		ZFunc		 = Always;

		VertexShader = compile vs_2_0 SkyVS();
		PixelShader  = compile ps_2_0 SkyPS();
	}
}