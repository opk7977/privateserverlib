//===============================
//	Global Variables
//===============================
float4x4 matWVP;
float4x4 matWIT;
float4x4 matSolidW;
bool	 bMultiply;
float4	 matRow0[80];
float4	 matRow1[80];
float4	 matRow2[80];
float3   vecEye = { 0.0f, 0.0f, -1.0f };
float	 lineLength = 1.0f;

//==================================
//  Fog
//==================================
bool  bFog = true;
float fogNear = 500.0f;
float fogFar  = 1000.0f;

//==================================
//	Light
//==================================
bool    bLight   = true;
int     lightType = 0;
float   lightRange = 1000.0f;
float3  vecLight = { 0.0f, 1.0f, 0.0f };
float4  l_a		 = { 0.1f, 0.1f, 0.1f, 1.0f  };
float4  l_d		 = { 1.0f, 1.0f, 1.0f, 1.0f  };
float4  l_s		 = { 1.0f, 1.0f, 1.0f, 1.0f  };
int		specPower = 10;

//==================================
//	Sampler
//==================================
#define STM_AMBIENT		0
#define STM_SPECULAR	1
bool bUseTexmap[2];

texture DiffuseTex;
sampler DiffuseSamp = sampler_state
{
	texture		= < DiffuseTex >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
};

texture NormalTex;
sampler NormalSamp = sampler_state
{
	texture		= < NormalTex >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
};

texture SpecularTex;
sampler SpecularSamp = sampler_state
{
	texture		= < SpecularTex >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
};

texture ReflectionTex;
sampler ReflectionSame = sampler_state
{
	texture = < ReflectionTex >;
	MinFilter	= Linear;
	MipFilter	= Linear;
	MagFilter	= Linear;
};

//========================================================================
//	NAnimateMesh
//========================================================================

//===============================
//	Vertex Shader
//===============================
void AniMeshVS(	float3 pos  : POSITION0,
				float3 norm : NORMAL,
				float4 col  : COLOR0,
				out float4 outPos  : POSITION0,
				out float4 outCol  : COLOR0,
				out float3 outNorm : TEXCOORD0,
				out float3 outEye  : TEXCOORD1 )
{
	outPos  = mul( float4(pos, 1.0f), matWVP );
	outCol  = col;
	outNorm = norm;
	outEye  = vecEye - pos;
}

//===============================
//	Pixel Shader
//===============================
void AniMeshPS(	float4 col  : COLOR0,
				float3 norm : TEXCOORD0,
				float3 eye  : TEXCOORD1,
				out float4 outCol : COLOR0 )
{
	float3 eyeN = normalize( eye );
	float3 normN = normalize( norm );
	
	outCol = col * dot( normN, eyeN );
}

//===============================
//	Technique
//===============================
technique NAnimateMeshRender
{
	pass P0
	{
		VertexShader = compile vs_2_0 AniMeshVS();
		PixelShader  = compile ps_2_0 AniMeshPS();
	}
}

//========================================================================
//	NSkinMesh
//========================================================================

//////////////////////////////////////////////////////////////////////////
//
//	STRUCT
//

//===============================
//	P3NCT
//===============================
struct P3NCT
{
	float3 Pos	:	POSITION;
	float3 Norm :	NORMAL;
	float4 Col  :	COLOR;
	float2 Tex  :	TEXCOORD0;
};

//===============================
//	P3NCTT
//===============================
struct P3NCTT
{
	float3 Pos	:	POSITION;
	float3 Norm :	NORMAL;
	float4 Col  :	COLOR;
	float2 Tex  :	TEXCOORD0;
	float3 Tan  :	TANGENT;
};

//===============================
//	P3NCTIW
//===============================
struct P3NCTIW
{
	float3 Pos	  :	POSITION;
	float3 Norm   :	NORMAL;
	float4 Col	  : COLOR;
	float2 Tex    :	TEXCOORD0;
	float4 Index  :	BLENDINDICES;
	float4 Weight :	BLENDWEIGHT;
};

//===============================
//	P3NCTIWT
//===============================
struct P3NCTIWT
{
	float3 Pos	  :	POSITION;
	float3 Norm   :	NORMAL;
	float4 Col	  : COLOR;
	float2 Tex    :	TEXCOORD0;
	float4 Index  :	BLENDINDICES;
	float4 Weight :	BLENDWEIGHT;
	float3 Tan	  : TANGENT;
};

//===============================
//	P3NCTIW8
//===============================
struct P3NCTIW8
{
	float3 Pos	   :	POSITION;
	float3 Norm    :	NORMAL;
	float4 Col	   :	COLOR;
	float2 Tex     :	TEXCOORD0;
	float4 Index   :	BLENDINDICES0;
	float4 Index2  :	BLENDINDICES1;
	float4 Weight  :	BLENDWEIGHT0;
	float4 Weight2 :	BLENDWEIGHT1;
};

//===============================
//	P3NCTIW8T
//===============================
struct P3NCTIW8T
{
	float3 Pos	   :	POSITION;
	float3 Norm    :	NORMAL;
	float4 Col	   :	COLOR;
	float2 Tex     :	TEXCOORD0;
	float4 Index   :	BLENDINDICES0;
	float4 Index2  :	BLENDINDICES1;
	float4 Weight  :	BLENDWEIGHT0;
	float4 Weight2 :	BLENDWEIGHT1;
	float3 Tan	   :	TANGENT;
};

//===============================
//	P3CTELF
//===============================
struct P3CTELF
{
	float4 Pos   : POSITION;
	float4 Col	 : COLOR;
	float2 Tex   : TEXCOORD0;
	float3 Eye   : TEXCOORD1;
	float3 Light : TEXCOORD2;
	float  Fog   : TEXCOORD3;
};

//===============================
//	P3CTELNF
//===============================
struct P3CTELNF
{
	float4 Pos  : POSITION;
	float4 Col  : COLOR;
	float2 Tex  : TEXCOORD0;
	float3 Eye  : TEXCOORD1;
	float3 Light: TEXCOORD2;
	float3 Norm : TEXCOORD3;
	float  Fog  : TEXCOORD4;
};

//////////////////////////////////////////////////////////////////////////
//
//	Vertex Shader
//

//===============================
//	VS_P3NCT
//===============================
P3CTELNF VS_P3NCT( P3NCT IN )
{
	P3CTELNF OUT = (P3CTELNF)0;
	
	float4 fixedPos;
	float3 fixedNorm;
	if( bMultiply )
	{
		fixedPos  = mul( float4( IN.Pos, 1.0f ), matSolidW );
		fixedNorm = mul( float4( IN.Norm, 0.0f ), matSolidW ).xyz;
	}
	else
	{
		fixedPos = float4( IN.Pos, 1.0f );
		fixedNorm = IN.Norm;
	}

	OUT.Pos = mul( fixedPos, matWVP );
	OUT.Col = IN.Col;
	OUT.Tex = IN.Tex;
	OUT.Eye = vecEye - fixedPos.xyz;
	OUT.Norm = fixedNorm;
	OUT.Fog  = length( OUT.Eye );

	return OUT;
}

//===============================
//	VS_P3NCTT
//===============================
P3CTELF VS_P3NCTT( P3NCTT IN )
{
	P3CTELF OUT = (P3CTELF)0;
	
	float4 fixedPos;
	float3 fixedNorm;
	float3 fixedTan;
	if( bMultiply )
	{
		fixedPos  = mul( float4( IN.Pos, 1.0f ), matSolidW );
		fixedNorm = mul( float4( IN.Norm, 0.0f ), matSolidW ).xyz;
		fixedTan  = mul( float4( IN.Tan, 0.0f ), matSolidW ).xyz;
	}
	else
	{
		fixedPos  = float4( IN.Pos, 1.0f );
		fixedNorm = IN.Norm;
		fixedTan  = IN.Tan;
	}

	OUT.Pos = mul( fixedPos, matWVP );
	OUT.Col = IN.Col;
	OUT.Tex = IN.Tex;
	
	float3 Binorm = cross( fixedNorm, fixedTan );
	
	float3 eyeDir = vecEye - fixedPos.xyz;
	OUT.Eye.x = dot( eyeDir, fixedTan  );
	OUT.Eye.y = dot( eyeDir, Binorm    );
	OUT.Eye.z = dot( eyeDir, fixedNorm );
	
	OUT.Light.x = dot( vecLight, fixedTan );
	OUT.Light.y = dot( vecLight, Binorm );
	OUT.Light.z = dot( vecLight, fixedNorm );
	OUT.Fog  = length( eyeDir );
	
	return OUT;
}

//===============================
//	VS_P3NCTIW
//===============================
P3CTELNF VS_P3NCTIW(P3NCTIW IN)
{
	P3CTELNF OUT = (P3CTELNF)0;
	
	float4x4 indexMat;
	float4   dummy = { 0.0f, 0.0f, 0.0f, 1.0f };
	float3 newPos = { 0.0f, 0.0f, 0.0f };
	float3 newNorm = { 0.0f, 0.0f, 0.0f };
	
	for( int i = 0; i < IN.Weight.w; ++i )
	{
		indexMat[0] = matRow0[ IN.Index[i] ];
		indexMat[1] = matRow1[ IN.Index[i] ];
		indexMat[2] = matRow2[ IN.Index[i] ];
		indexMat[3] = dummy;

		indexMat = transpose( indexMat );

		if( i == 3 )
		{
			newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * 
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z );
			newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz *
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z );
		}
		else
		{
			newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * IN.Weight[i];
			newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz * IN.Weight[i];
		}
	}

	OUT.Pos   = mul( float4( newPos, 1.0f ), matWVP );
	OUT.Col	  = IN.Col;
	OUT.Tex   = IN.Tex;
	OUT.Eye   = vecEye - newPos;
	OUT.Norm  = newNorm;
	OUT.Fog  = length( OUT.Eye );

	return OUT;
}

//===============================
//	VS_P3NCTIWT
//===============================
P3CTELF VS_P3NCTIWT(P3NCTIWT IN)
{
	P3CTELF OUT = (P3CTELF)0;
	
	float4x4 indexMat;
	float4   dummy = { 0.0f, 0.0f, 0.0f, 1.0f };
	float3 newPos = { 0.0f, 0.0f, 0.0f };
	float3 newNorm = { 0.0f, 0.0f, 0.0f };
	float3 newTan  = { 0.0f, 0.0f, 0.0f };
	
	for( int i = 0; i < IN.Weight.w; ++i )
	{
		indexMat[0] = matRow0[ IN.Index[i] ];
		indexMat[1] = matRow1[ IN.Index[i] ];
		indexMat[2] = matRow2[ IN.Index[i] ];
		indexMat[3] = dummy;

		indexMat = transpose( indexMat );

		if( i == 3 )
		{
			newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * 
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z );
			newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz *
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z );
			newTan  += mul( float4(IN.Tan, 0.0f),  indexMat ).xyz *
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z );
		}
		else
		{
			newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * IN.Weight[i];
			newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz * IN.Weight[i];
			newTan  += mul( float4(IN.Tan, 0.0f),  indexMat ).xyz * IN.Weight[i];
		}
	}

	OUT.Pos = mul( float4( newPos, 1.0f ), matWVP );
	OUT.Col = IN.Col;
	OUT.Tex = IN.Tex;
	
	float3 Binorm = cross( newNorm, newTan );
	
	float3 eyeDir = vecEye - newPos;
	OUT.Eye.x = dot( eyeDir, newTan  );
	OUT.Eye.y = dot( eyeDir, Binorm  );
	OUT.Eye.z = dot( eyeDir, newNorm );
	
	OUT.Light.x = dot( vecLight, newTan );
	OUT.Light.y = dot( vecLight, Binorm );
	OUT.Light.z = dot( vecLight, newNorm );
	
	OUT.Fog  = length( eyeDir );
	
	return OUT;
}

//===============================
//	VS_P3NCTIW8
//===============================
P3CTELNF VS_P3NCTIW8(P3NCTIW8 IN)
{
	P3CTELNF OUT = (P3CTELNF)0;
	
	float4x4 indexMat;
	float4   dummy = { 0.0f, 0.0f, 0.0f, 1.0f };
	float3 newPos = { 0.0f, 0.0f, 0.0f };
	float3 newNorm = { 0.0f, 0.0f, 0.0f };
	
	int rI;
	int cnt = (int)IN.Weight2.w;
	for( int i = 0; i < cnt; ++i )
	{
		indexMat[3] = dummy;
		if( i < 4 )
		{
			rI = i;
			indexMat[0] = matRow0[ IN.Index[rI] ];
			indexMat[1] = matRow1[ IN.Index[rI] ];
			indexMat[2] = matRow2[ IN.Index[rI] ];
			indexMat = transpose( indexMat );
			
			newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * IN.Weight[i];
			newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz * IN.Weight[i];
		}
		else
		{
			rI = i-4;
			indexMat[0] = matRow0[ IN.Index2[rI] ];
			indexMat[1] = matRow1[ IN.Index2[rI] ];
			indexMat[2] = matRow2[ IN.Index2[rI] ];
			indexMat = transpose( indexMat );
			
			newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * IN.Weight2[rI];
			newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz * IN.Weight2[rI];
		}
	}
	
	if( cnt == 8 )
	{
		indexMat[0] = matRow0[ IN.Index2[3] ];
		indexMat[1] = matRow1[ IN.Index2[3] ];
		indexMat[2] = matRow2[ IN.Index2[3] ];
		indexMat[3] = dummy;
		indexMat = transpose( indexMat );
		
		newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * 
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z - IN.Weight.w
					   - IN.Weight2.x - IN.Weight2.y - IN.Weight2.z );
		newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz *
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z - IN.Weight.w
					   - IN.Weight2.x - IN.Weight2.y - IN.Weight2.z );
	}

	OUT.Pos   = mul( float4( newPos, 1.0f ), matWVP );
	OUT.Col	  = IN.Col;
	OUT.Tex   = IN.Tex;
	OUT.Eye   = vecEye - newPos;
	OUT.Norm  = newNorm;
	OUT.Fog   = length( OUT.Eye );

	return OUT;
}

//===============================
//	VS_P3NCTIW8T
//===============================
P3CTELF VS_P3NCTIW8T(P3NCTIW8T IN)
{
	P3CTELF OUT = (P3CTELF)0;
	
	float4x4 indexMat;
	float4   dummy = { 0.0f, 0.0f, 0.0f, 1.0f };
	float3 newPos = { 0.0f, 0.0f, 0.0f };
	float3 newNorm = { 0.0f, 0.0f, 0.0f };
	float3 newTan  = { 0.0f, 0.0f, 0.0f };
	
	int rI;
	int cnt = (int)IN.Weight2.w;
	for( int i = 0; i < cnt; ++i )
	{
		indexMat[3] = dummy;
		if( i < 4 )
		{
			rI = i;
			indexMat[0] = matRow0[ IN.Index[rI] ];
			indexMat[1] = matRow1[ IN.Index[rI] ];
			indexMat[2] = matRow2[ IN.Index[rI] ];
			indexMat = transpose( indexMat );
			
			newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * IN.Weight[i];
			newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz * IN.Weight[i];
			newTan  += mul( float4(IN.Tan, 0.0f),  indexMat ).xyz * IN.Weight[i];
		}
		else
		{
			rI = i-4;
			indexMat[0] = matRow0[ IN.Index2[rI] ];
			indexMat[1] = matRow1[ IN.Index2[rI] ];
			indexMat[2] = matRow2[ IN.Index2[rI] ];
			indexMat = transpose( indexMat );
			
			newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * IN.Weight2[rI];
			newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz * IN.Weight2[rI];
			newTan  += mul( float4(IN.Tan, 0.0f),  indexMat ).xyz * IN.Weight2[rI];
		}
	}
	
	if( cnt == 8 )
	{
		indexMat[0] = matRow0[ IN.Index2[3] ];
		indexMat[1] = matRow1[ IN.Index2[3] ];
		indexMat[2] = matRow2[ IN.Index2[3] ];
		indexMat[3] = dummy;
		indexMat = transpose( indexMat );
		
		newPos  += mul( float4(IN.Pos, 1.0f),  indexMat ).xyz * 
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z - IN.Weight.w
					   - IN.Weight2.x - IN.Weight2.y - IN.Weight2.z );
		newNorm += mul( float4(IN.Norm, 0.0f), indexMat ).xyz *
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z - IN.Weight.w
					   - IN.Weight2.x - IN.Weight2.y - IN.Weight2.z );
		newTan  += mul( float4(IN.Tan, 0.0f), indexMat ).xyz *
				( 1.0f - IN.Weight.x - IN.Weight.y - IN.Weight.z - IN.Weight.w
					   - IN.Weight2.x - IN.Weight2.y - IN.Weight2.z );
	}

	OUT.Pos = mul( float4( newPos, 1.0f ), matWVP );
	OUT.Col = IN.Col;
	OUT.Tex = IN.Tex;
	
	float3 Binorm = cross( newNorm, newTan );
	float3 eyeDir = vecEye - newPos;
	OUT.Eye.x = dot( eyeDir, newTan  );
	OUT.Eye.y = dot( eyeDir, Binorm  );
	OUT.Eye.z = dot( eyeDir, newNorm );
	
	OUT.Light.x = dot( vecLight, newTan );
	OUT.Light.y = dot( vecLight, Binorm );
	OUT.Light.z = dot( vecLight, newNorm );
	
	OUT.Fog  = length( eyeDir );
	
	return OUT;
}


void VS_LINE_P3NCIWU(float3 Pos : POSITION,
					 float3 Norm : NORMAL,
					 float4 Col  : COLOR,
					 float4 Index : BLENDINDICES,
					 float4 Weight : BLENDWEIGHT,
					 float2 UV : TEXCOORD0,
					 out float4 OutPos : POSITION,
					 out float4 OutCol : COLOR)
{
	float4x4 indexMat;
	float4   dummy = { 0.0f, 0.0f, 0.0f, 1.0f };
	float3 newPos = { 0.0f, 0.0f, 0.0f };
	float3 newNorm = { 0.0f, 0.0f, 0.0f };
	float3 newTan  = { 0.0f, 0.0f, 0.0f };
	
	for( int i = 0; i < Weight.w; ++i )
	{
		indexMat[0] = matRow0[ Index[i] ];
		indexMat[1] = matRow1[ Index[i] ];
		indexMat[2] = matRow2[ Index[i] ];
		indexMat[3] = dummy;

		indexMat = transpose( indexMat );

		if( i == 3 )
		{
			newPos  += mul( float4(Pos, 1.0f),  indexMat ).xyz * 
				( 1.0f - Weight.x - Weight.y - Weight.z );
			newNorm += mul( float4(Norm, 0.0f), indexMat ).xyz *
				( 1.0f - Weight.x - Weight.y - Weight.z );
		}
		else
		{
			newPos  += mul( float4(Pos, 1.0f),  indexMat ).xyz * Weight[i];
			newNorm += mul( float4(Norm, 0.0f), indexMat ).xyz * Weight[i];
		}
	}

	if(UV.x == 0.0f)
		newPos += newNorm * lineLength;

	OutPos = mul( float4( newPos, 1.0f ), matWVP );
	OutCol = Col;
}

void VS_LINE_P3NCIW8U(float3 Pos : POSITION,
					  float3 Norm : NORMAL,
					  float4 Col  : COLOR,
					  float4 Index : BLENDINDICES0,
					  float4 Index2 : BLENDINDICES1,
					  float4 Weight : BLENDWEIGHT0,
					  float4 Weight2 : BLENDWEIGHT1,
					  float2 UV : TEXCOORD0,
					  out float4 OutPos : POSITION,
					  out float4 OutCol : COLOR)
{
	float4x4 indexMat;
	float4   dummy = { 0.0f, 0.0f, 0.0f, 1.0f };
	float3 newPos = { 0.0f, 0.0f, 0.0f };
	float3 newNorm = { 0.0f, 0.0f, 0.0f };
	float3 newTan  = { 0.0f, 0.0f, 0.0f };
	
	int rI;
	int cnt = (int)Weight2.w;
	for( int i = 0; i < cnt; ++i )
	{
		indexMat[3] = dummy;
		if( i < 4 )
		{
			indexMat[0] = matRow0[ Index[i] ];
			indexMat[1] = matRow1[ Index[i] ];
			indexMat[2] = matRow2[ Index[i] ];
			indexMat = transpose( indexMat );
			
			newPos  += mul( float4(Pos, 1.0f),  indexMat ).xyz * Weight[i];
			newNorm += mul( float4(Norm, 0.0f), indexMat ).xyz * Weight[i];
		}
		else
		{
			rI = i-4;
			indexMat[0] = matRow0[ Index2[rI] ];
			indexMat[1] = matRow1[ Index2[rI] ];
			indexMat[2] = matRow2[ Index2[rI] ];
			indexMat = transpose( indexMat );
			
			newPos  += mul( float4(Pos, 1.0f),  indexMat ).xyz * Weight2[rI];
			newNorm += mul( float4(Norm, 0.0f), indexMat ).xyz * Weight2[rI];
		}
	}
	
	if( cnt == 8 )
	{
		indexMat[0] = matRow0[ Index2[3] ];
		indexMat[1] = matRow1[ Index2[3] ];
		indexMat[2] = matRow2[ Index2[3] ];
		indexMat[3] = dummy;
		indexMat = transpose( indexMat );
		
		newPos  += mul( float4(Pos, 1.0f),  indexMat ).xyz * 
				( 1.0f - Weight.x - Weight.y - Weight.z - Weight.w
					   - Weight2.x - Weight2.y - Weight2.z );
		newNorm += mul( float4(Norm, 0.0f), indexMat ).xyz *
				( 1.0f - Weight.x - Weight.y - Weight.z - Weight.w
					   - Weight2.x - Weight2.y - Weight2.z );
	}
	
	if(UV.x == 0.0f)
		newPos += newNorm * lineLength;

	OutPos = mul( float4( newPos, 1.0f ), matWVP );
	OutCol = Col;
}

void VS_LINE_P3NCU(	float3 Pos : POSITION,
					float3 Norm : NORMAL,
					float4 Col  : COLOR,
					float2 UV : TEXCOORD0,
					out float4 OutPos : POSITION,
					out float4 OutCol : COLOR)
{
	float4 fixedPos;
	float3 fixedNorm;
	if( bMultiply )
	{
		fixedPos  = mul( float4( Pos, 1.0f ), matSolidW );
		fixedNorm = mul( float4( Norm, 0.0f ), matSolidW ).xyz;
	}
	else
	{
		fixedPos = float4( Pos, 1.0f );
		fixedNorm = Norm;
	}

	if(UV.x == 0.0f)
		fixedPos.xyz += fixedNorm * lineLength;

	OutPos = mul( fixedPos, matWVP );
	OutCol = Col;
}

float4 PS_Line(float4 Col : COLOR) : COLOR
{
	return Col;
}

//////////////////////////////////////////////////////////////////////////
//
//	Pixel Shader
//

//===============================
//	PS_PhongCol
//===============================
float4 PS_PhongCol( P3CTELNF IN ) : COLOR0
{
	float3 eyeN		= normalize( IN.Eye );
	float3 normN	= normalize( IN.Norm );
	float4 texCol	= tex2D( DiffuseSamp, IN.Tex );
	float4 diffCol  = texCol * IN.Col;

	// Ambient
	float4 ambient = diffCol * l_a;

	// Diffuse
	float  diffuseFactor = 1.0f;
	if( bLight )
		diffuseFactor = max( 0.0f, dot( vecLight, normN ) );
		
	float4 diffuse = diffCol * l_d * diffuseFactor;
			
	// Specular
	float specFactor = 0.0f;
	if(specPower > 0)
	{
		float3 reflVec = normalize( reflect( -vecLight, normN ) );
		specFactor = max(0.0f, dot( reflVec, eyeN ));
		specFactor = pow( specFactor, specPower );
			
		if( bUseTexmap[STM_SPECULAR] )
			specFactor = specFactor * tex2D( SpecularSamp, IN.Tex ).x;
	}
	float4 specular = l_s * specFactor;
				
	// Fog
	float fogFactor = 0.0f;
	if( bFog )
		fogFactor = fogNear > IN.Fog ? 0.0f : (IN.Fog - fogNear) / (fogFar - fogNear);
	
	float4 finalCol = saturate( diffuse + specular - fogFactor );
	finalCol.a = 1.0f;
	
	return ambient + finalCol;
}

//===============================
//	PS_BumpPhongCol
//===============================
float4 PS_BumpPhongCol( P3CTELF IN ) : COLOR0
{
	float3 lightN	= normalize( IN.Light );
	float3 eyeN		= normalize( IN.Eye );
	float3 normN	= normalize( 2.0f * tex2D( NormalSamp, IN.Tex ) - 1.0f );
	float4 texCol	= tex2D( DiffuseSamp, IN.Tex );
	float4 diffCol  = texCol * IN.Col;
	
	// Ambient
	float4 ambient = diffCol * l_a;

	// Diffuse
	float  diffuseFactor = 1.0f;
	if( bLight )
		diffuseFactor = max( 0.0f, dot( lightN, normN ) );
			
	float4 diffuse = diffCol * l_d * diffuseFactor;
	
	// Specular
	float specFactor = 0.0f;
	if(specPower > 0)
	{
		float3 reflVec = normalize( reflect( -lightN, normN ) );
		specFactor = max(0.0f, dot( reflVec, eyeN ));
		specFactor = pow( specFactor, specPower );
			
		if( bUseTexmap[STM_SPECULAR] )
			specFactor = specFactor * tex2D( SpecularSamp, IN.Tex ).x;
	}
	float4 specular = l_s * specFactor;
	
	// Fog
	float fogFactor = 0.0f;
	if( bFog )
		fogFactor = fogNear > IN.Fog ? 0.0f : (IN.Fog - fogNear) / (fogFar - fogNear);
			
	float4 finalCol = saturate( diffuse + specular - fogFactor );
	finalCol.a = 1.0f;
	
	return ambient + finalCol;
}

//===============================
//	Technique
//===============================
technique NSkinMeshRender
{
	pass P3NCT
	{
		VertexShader = compile vs_2_0 VS_P3NCT();
		PixelShader  = compile ps_2_0 PS_PhongCol();
	}
	
	pass P3NCTT
	{
		VertexShader = compile vs_2_0 VS_P3NCTT();
		PixelShader  = compile ps_2_0 PS_BumpPhongCol();
	}
	
	pass P3NCTIW
	{
		VertexShader = compile vs_2_0 VS_P3NCTIW();
		PixelShader  = compile ps_2_0 PS_PhongCol();
	}
	
	pass P3NCTIWT
	{
		VertexShader = compile vs_2_0 VS_P3NCTIWT();
		PixelShader  = compile ps_2_0 PS_BumpPhongCol();
	}
	
	pass P3NCTIW8
	{
		VertexShader = compile vs_2_0 VS_P3NCTIW8();
		PixelShader  = compile ps_2_0 PS_PhongCol();
	}
	
	pass P3NCTIW8T
	{
		VertexShader = compile vs_2_0 VS_P3NCTIW8T();
		PixelShader  = compile ps_2_0 PS_BumpPhongCol();
	}

	pass LINE_PASS
	{
		VertexShader = compile vs_2_0 VS_LINE_P3NCIWU();
		PixelShader  = compile ps_2_0 PS_Line();
	}
	
	pass LINE_PASS2
	{
		VertexShader = compile vs_2_0 VS_LINE_P3NCIW8U();
		PixelShader  = compile ps_2_0 PS_Line();
	}

	pass NOWEIGHT
	{
		VertexShader = compile vs_2_0 VS_LINE_P3NCU();
		PixelShader  = compile ps_2_0 PS_Line();
	}
}