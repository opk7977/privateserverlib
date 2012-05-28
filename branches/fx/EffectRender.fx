////////////////////////////////////
//
//	Global Variable
//
////////////////////////////////////
float4x4 matWVP;

texture diffuseTex;
sampler DiffuseSamp = sampler_state
{
	Texture = <diffuseTex>;
	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

////////////////////////////////////
//
//	Vertex Shader
//
////////////////////////////////////
void VS(float3 pos : POSITION,
		float2 tex : TEXCOORD0,
		out float4 outpos : POSITION,
		out float2 outtex : TEXCOORD0)
{
	outpos = mul( float4( pos, 1.0f ), matWVP );
	outtex = tex;
}

// PixelShader
float4 PS(float2 tex: TEXCOORD0) : COLOR
{
	return tex2D( DiffuseSamp, tex );
}

// Technique
technique SpriteRender
{
	pass P0
	{
		AlphaBlendEnable = True;
		SrcBlend = One;
		DestBlend = One;
		
		//sc * 1 + dc * 1;

		VertexShader = compile vs_1_0 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}