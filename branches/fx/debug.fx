// Global Variable
float4x4  matW;
float4x4 matVP;

// Vertex Shader
void VS(float3 pos : POSITION,
		float4 col : COLOR,
		out float4 outpos : POSITION,
		out float4 outcol : COLOR)
{
	outpos = mul( float4( pos, 1.0f ), matVP );
	outcol = col;
}

// Vertex Shader
void VS_UseWorld(	float3 pos : POSITION,
					float4 col : COLOR,
					out float4 outpos : POSITION,
					out float4 outcol : COLOR )
{
	outpos = mul( float4( pos, 1.0f ), matW );
	outpos = mul( outpos, matVP );
	outcol = col;
}

// PixelShader
float4 PS(float4 col : COLOR) : COLOR
{
	return col;
}

// Technique
technique DebugRender
{
	pass P0
	{
		VertexShader = compile vs_1_0 VS();
		PixelShader  = compile ps_2_0 PS();
	}
	
	pass P1
	{
		VertexShader = compile vs_2_0 VS_UseWorld();
		PixelShader  = compile ps_2_0 PS();
	}
}