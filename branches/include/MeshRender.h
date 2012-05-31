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

//==================================
//	Light
//==================================
float3  vecLight = { 0.0f, 10.0f, 0.0f };
float4  l_a		 = { 0.1f, 0.1f, 0.1f, 1.0f  };
float4  l_d		 = { 1.0f, 1.0f, 1.0f, 1.0f  };
float4  l_s		 = { 1.0f, 1.0f, 1.0f, 1.0f  };
int		specPower = 60;

//==================================
//	Sampler
//==================================
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
		Lighting = False;
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
//	P3NT
//===============================
struct P3NT
{
	float3 Pos	:	POSITION;
	float3 Norm :	NORMAL;
	float2 Tex  :	TEXCOORD0;
};

//===============================
//	P3NTT
//===============================
struct P3NTT
{
	float3 Pos	:	POSITION;
	float3 Norm :	NORMAL;
	float2 Tex  :	TEXCOORD0;
	float3 Tan	:   TANGENT;
};

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
//	P3TEL
//===============================
struct P3TEL
{
	float4 Pos   : POSITION;
	float2 Tex   : TEXCOORD0;
	float3 Eye   : TEXCOORD1;
	float3 Light : TEXCOORD2;
};

//===============================
//	P3CTEL
//===============================
struct P3CTEL
{
	float4 Pos   : POSITION;
	float4 Col	 : COLOR;
	float2 Tex   : TEXCOORD0;
	float3 Eye   : TEXCOORD1;
	float3 Light : TEXCOORD2;
};

//===============================
//	P3TELN
//===============================
struct P3TELN
{
	float4 Pos  : POSITION;
	float2 Tex  : TEXCOORD0;
	float3 Eye  : TEXCOORD1;
	float3 Light: TEXCOORD2;
	float3 Norm : TEXCOORD3;
};

//===============================
//	P3CTELN
//===============================
struct P3CTELN
{
	float4 Pos  : POSITION;
	float4 Col  : COLOR;
	float2 Tex  : TEXCOORD0;
	float3 Eye  : TEXCOORD1;
	float3 Light: TEXCOORD2;
	float3 Norm : TEXCOORD3;
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
//	P3NTIW
//===============================
struct P3NTIW
{
	float3 Pos	  :	POSITION;
	float3 Norm   :	NORMAL;
	float2 Tex    :	TEXCOORD0;
	float4 Index  :	BLENDINDICES;
	float4 Weight :	BLENDWEIGHT;
};

//===============================
//	P3NTIWT
//===============================
struct P3NTIWT
{
	float3 Pos	  :	POSITION;
	float3 Norm   :	NORMAL;
	float2 Tex    :	TEXCOORD0;
	float4 Index  :	BLENDINDICES;
	float4 Weight :	BLENDWEIGHT;
	float3 Tan	  : TANGENT;
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

//////////////////////////////////////////////////////////////////////////
//
//	Vertex Shader
//

//===============================
//	VS_P3NT
//===============================
P3TELN VS_P3NT( P3NT IN )
{
	P3TELN OUT = (P3TELN)0;
	
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
	OUT.Tex = IN.Tex;
	OUT.Eye = vecEye - fixedPos;
	OUT.Light = vecLight - fixedPos;
	OUT.Norm = fixedNorm;
	
	return OUT;
}

//===============================
//	VS_P3NCT
//===============================
P3CTELN VS_P3NCT( P3NCT IN )
{
	P3CTELN OUT = (P3CTELN)0;
	
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
	OUT.Eye = vecEye - fixedPos;
	OUT.Light = vecLight - fixedPos;
	OUT.Norm = fixedNorm;

	return OUT;
}

//===============================
//	VS_P3NTT
//===============================
P3TEL VS_P3NTT( P3NTT IN )
{
	P3TEL OUT = (P3TEL)0;
	
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
	OUT.Tex = IN.Tex;
	
	float3 Binorm = cross( fixedNorm, fixedTan );
	
	float3 eyeDir = vecEye - fixedPos;
	OUT.Eye.x = dot( eyeDir, fixedTan   );
	OUT.Eye.y = dot( eyeDir, Binorm    );
	OUT.Eye.z = dot( eyeDir, fixedNorm );
	
	float3 lightDir = vecLight - fixedPos;
	OUT.Light.x = dot( lightDir, fixedTan  );
	OUT.Light.y = dot( lightDir, Binorm    );
	OUT.Light.z = dot( lightDir, fixedNorm );
	
	return OUT;
}

//===============================
//	VS_P3NCTT
//===============================
P3CTEL VS_P3NCTT( P3NCTT IN )
{
	P3CTEL OUT = (P3CTEL)0;
	
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
	
	float3 eyeDir = vecEye - fixedPos;
	OUT.Eye.x = dot( eyeDir, fixedTan   );
	OUT.Eye.y = dot( eyeDir, Binorm    );
	OUT.Eye.z = dot( eyeDir, fixedNorm );
	
	float3 lightDir = vecLight - fixedPos;
	OUT.Light.x = dot( lightDir, fixedTan  );
	OUT.Light.y = dot( lightDir, Binorm    );
	OUT.Light.z = dot( lightDir, fixedNorm );

	return OUT;
}

//===============================
//	VS_P3NTIW
//===============================
P3TELN VS_P3NTIW( P3NTIW IN )
{
	P3TELN OUT = (P3TELN)0;
	
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

	OUT.Pos = mul( float4( newPos, 1.0f ), matWVP );
	OUT.Tex = IN.Tex;
	OUT.Eye = vecEye - newPos;
	OUT.Light = vecLight - newPos;
	OUT.Norm = newNorm;

	return OUT;
}

//===============================
//	VS_P3NTIWT
//===============================
P3TEL VS_P3NTIWT(P3NTIWT IN)
{
	P3TEL OUT = (P3TEL)0;
	
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
	OUT.Tex = IN.Tex;
	
	newNorm = normalize( newNorm );
	newTan = normalize( newTan );
	float3 Binorm = cross( newNorm, newTan );
	
	float3 eyeDir = vecEye - newPos;
	OUT.Eye.x = dot( eyeDir, newTan  );
	OUT.Eye.y = dot( eyeDir, Binorm  );
	OUT.Eye.z = dot( eyeDir, newNorm );
	
	float3 lightDir = vecLight - newPos;
	OUT.Light.x = dot( lightDir, newTan  );
	OUT.Light.y = dot( lightDir, Binorm  );
	OUT.Light.z = dot( lightDir, newNorm );

	return OUT;
}

//===============================
//	VS_P3NCTIW
//===============================
P3CTELN VS_P3NCTIW(P3NCTIW IN)
{
	P3CTELN OUT = (P3CTELN)0;
	
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
	OUT.Light = vecLight - newPos;
	OUT.Norm  = newNorm;

	return OUT;
}

//===============================
//	VS_P3NCTIWT
//===============================
P3CTEL VS_P3NCTIWT(P3NCTIWT IN)
{
	P3CTEL OUT = (P3CTEL)0;
	
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
	
	float3 lightDir = vecLight - newPos;
	OUT.Light.x = dot( lightDir, newTan  );
	OUT.Light.y = dot( lightDir, Binorm  );
	OUT.Light.z = dot( lightDir, newNorm );

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
		newPos += newNorm * 0.1f;

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
		fixedPos.xyz += fixedNorm * 0.1f;

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
//	PS_Phong
//===============================
float4 PS_Phong( P3TELN IN ) : COLOR0
{
	float3 eyeN		= normalize( IN.Eye );
	float3 lightN	= normalize( IN.Light );
	float3 normN	= normalize( IN.Norm );
	float4 texCol	= tex2D( DiffuseSamp, IN.Tex );

	// Ambient + Diffuse
	float  diffuseFactor = max( 0.0f, dot( eyeN, normN ) );
	float4 diffuse = (l_a + diffuseFactor) * texCol * l_d;
		
	// Specular
	float3 vecReflect = 2.0f * dot( lightN, normN ) * normN - lightN;
	float  specFactor = max( 0.0f, dot( vecReflect, eyeN ) );
	if( specFactor > 0.0f )
	{
		specFactor = pow( specFactor, specPower );
	}
	float4 specular = specFactor * l_s;
	specular.a = l_s.a;

	return diffuse + specular;
	//return diffuse;
}

//===============================
//	PS_BumpPhong
//===============================
float4 PS_BumpPhong( P3TEL IN ) : COLOR0
{
	float3 eyeN		= normalize( IN.Eye );
	float3 lightN   = normalize( IN.Light );
	float3 normN	= normalize( 2.0f * tex2D( NormalSamp, IN.Tex ) - 1.0f );
	float4 texCol	= tex2D( DiffuseSamp, IN.Tex );

	//Ambient + Diffuse
	float  diffuseFactor = max( 0.0f, dot( eyeN, normN ) );
	float4 diffuse = (l_a + diffuseFactor) * texCol * l_d;
			
	// Specular
	float3 vecReflect = 2.0f * dot( lightN, normN ) * normN - lightN;
	float  specFactor = max( 0.0f, dot( vecReflect, eyeN ) );
	if( specFactor > 0.0f )
	{
		specFactor = pow( specFactor, specPower );
	}
	float4 specular = specFactor * l_s;
	specular.a = l_s.a;

 	return diffuse + specular;
  	//return diffuseFactor;
  	//return specular;
}

//===============================
//	PS_PhongCol
//===============================
float4 PS_PhongCol( P3CTELN IN ) : COLOR0
{
	float3 eyeN		= normalize( IN.Eye );
	float3 lightN	= normalize( IN.Light );
	float3 normN	= normalize( IN.Norm );
	float4 texCol	= tex2D( DiffuseSamp, IN.Tex );

	// Ambient + Diffuse
	float  diffuseFactor = max( 0.0f, dot( eyeN, normN ) );
	float4 diffuse = (l_a + diffuseFactor) * texCol * IN.Col * l_d;
		
	// Specular
// 	float3 vecReflect = 2.0f * dot( lightN, normN ) * normN - lightN;
// 	float  specFactor = max( 0.0f, dot( vecReflect, eyeN ) );
// 	if( specFactor > 0.0f )
// 	{
// 		specFactor = pow( specFactor, specPower );
// 	}
// 	float4 specular = specFactor * l_s;
// 	specular.a = l_s.a;

	//return diffuse + specular;
	return diffuse;
	//return IN.Col;
}

//===============================
//	PS_BumpPhongCol
//===============================
float4 PS_BumpPhongCol( P3CTEL IN ) : COLOR0
{
	float3 eyeN		= normalize( IN.Eye );
	float3 lightN   = normalize( IN.Light );
	float3 normN	= normalize( 2.0f * tex2D( NormalSamp, IN.Tex ) - 1.0f );
	float4 texCol	= tex2D( DiffuseSamp, IN.Tex );

	// Ambient + Diffuse
	float  diffuseFactor = max( 0.0f, dot( lightN, normN ) );
	float4 diffuse = (l_a + diffuseFactor) * texCol * IN.Col * l_d;
		
	// Specular
	float3 vecReflect = 2.0f * dot( lightN, normN ) * normN - lightN;
	float  specFactor = max( 0.0f, dot( vecReflect, eyeN ) );
	if( specFactor > 0.0f )
	{
		specFactor = pow( specFactor, specPower );
	}
	float4 specular = specFactor * l_s;
	specular.a = l_s.a;

	return diffuse + specular;
}

//===============================
//	Technique
//===============================
technique NSkinMeshRender
{
	pass P3NT
	{
		VertexShader = compile vs_2_0 VS_P3NT();
		PixelShader  = compile ps_2_0 PS_Phong();
	}
	
	pass P3NTT
	{
		VertexShader = compile vs_2_0 VS_P3NTT();
		PixelShader  = compile ps_2_0 PS_BumpPhong();
	}
	
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
	
	pass P3NTIW
	{
		VertexShader = compile vs_2_0 VS_P3NTIW();
		PixelShader  = compile ps_2_0 PS_Phong();
	}
	
	pass P3NTIWT
	{
		VertexShader = compile vs_3_0 VS_P3NTIWT();
		PixelShader  = compile ps_2_0 PS_BumpPhong();
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

	pass LINE_PASS
	{
		VertexShader = compile vs_2_0 VS_LINE_P3NCIWU();
		PixelShader  = compile ps_2_0 PS_Line();
	}

	pass NOWEIGHT
	{
		VertexShader = compile vs_2_0 VS_LINE_P3NCU();
		PixelShader  = compile ps_2_0 PS_Line();
	}
}