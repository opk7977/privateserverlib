#include "NMeshRenderer.h"
#include "NL_Shader.h"
#include "NL_Camera.h"
#include "NL_Texture.h"
#include "NL_Shader.h"
#include "NL_Device.h"
#include "NAnimateMesh.h"
#include "NSkinMesh.h"
#include "NObject.h"

void NMeshRenderer::Init()
{
	m_pDevPool = I_NDevice;
	m_pDevice = I_ND3d;
	m_pCamera = I_NCamera;
	m_pMtlVec = 0;

	ZeroMemory( m_pDecl, sizeof(LPDIRECT3DVERTEXDECLARATION9) * RN_MAX );

#ifdef _DEBUG
	m_debugDecl[0] = NULL;
	m_debugDecl[1] = NULL;
	m_line = 1.0f;
#endif

	m_pfnSetBuffer[RN_P3NCT] = &NMeshRenderer::SetBufferP3NCT;
	m_pfnSetBuffer[RN_P3NCTT] = &NMeshRenderer::SetBufferP3NCTT;
	m_pfnSetBuffer[RN_P3NCTW4] = &NMeshRenderer::SetBufferP3NCTIW;
	m_pfnSetBuffer[RN_P3NCTW4T] = &NMeshRenderer::SetBufferP3NCTIWT;
	m_pfnSetBuffer[RN_P3NCTW8] = &NMeshRenderer::SetBufferP3NCTIW8;
	m_pfnSetBuffer[RN_P3NCTW8T] = &NMeshRenderer::SetBufferP3NCTIW8T;
	
}

bool NMeshRenderer::Load(const TCHAR* fileName)
{
	m_pShader = Mgr_NShader->GetByName(fileName);
	if(!m_pShader)
		return false;

	SplitPath(fileName);

	Remove();
	Reset();
	return true;
}

bool NMeshRenderer::Reset()
{
	m_pEff		= m_pShader->GetShader();
	m_aniTech	= m_pEff->GetTechniqueByName("NAnimateMeshRender");
	m_skinTech	= m_pEff->GetTechniqueByName("NSkinMeshRender");
	m_matWVP	= m_pEff->GetParameterByName(0, "matWVP");
	m_matWIT	= m_pEff->GetParameterByName(0, "matWIT");
	m_matSolidW	= m_pEff->GetParameterByName(0, "matSolidW");
	m_bMultiply	= m_pEff->GetParameterByName(0, "bMultiply");
	m_matRow0	= m_pEff->GetParameterByName(0, "matRow0");
	m_matRow1	= m_pEff->GetParameterByName(0, "matRow1");
	m_matRow2	= m_pEff->GetParameterByName(0, "matRow2");
	m_vecEye	= m_pEff->GetParameterByName(0, "vecEye");
	m_vecLight	= m_pEff->GetParameterByName(0, "vecLight");
	m_specPower = m_pEff->GetParameterByName(0, "specPower");
	m_bUseTexmap = m_pEff->GetParameterByName(0, "bUseTexmap");
	m_texmap[TM_DIFFUSE]	= m_pEff->GetParameterByName(0, "DiffuseTex");
	m_texmap[TM_SPECULAR]	= m_pEff->GetParameterByName(0, "SpecularTex");
	m_texmap[TM_BUMP]		= m_pEff->GetParameterByName(0, "NormalTex");
	m_texmap[TM_REFLECTION]	= m_pEff->GetParameterByName(0, "ReflectionTex");
	m_bUseFog		= m_pEff->GetParameterByName(0, "bFog");
	m_fogNear		= m_pEff->GetParameterByName(0, "fogNear");
	m_fogFar		= m_pEff->GetParameterByName(0, "fogFar");
	m_bUseLight		= m_pEff->GetParameterByName(0, "bLight");;
	m_lightType		= m_pEff->GetParameterByName(0, "lightType");;
	m_lightRange	= m_pEff->GetParameterByName(0, "lightRange");
	m_ambient		= m_pEff->GetParameterByName(0, "l_a");
	m_diffuse		= m_pEff->GetParameterByName(0, "l_d");

	//----------------------------------
	//	Declaration
	//----------------------------------
	//
	
	// P3NCT
	D3DVERTEXELEMENT9 p3nct[] =
	{
		{	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0	},
		{	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0		},
		{	0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0	},
		{	0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0	},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(p3nct, &(m_pDecl[RN_P3NCT]));
	

	// P3NCTT
	D3DVERTEXELEMENT9 p3nctt[] =
	{
		{	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0	},
		{	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0		},
		{	0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0	},
		{	0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0	},
		{	1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0	},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(p3nctt, &(m_pDecl[RN_P3NCTT]));
	

	// P3NCTW4
	D3DVERTEXELEMENT9 p3nctw4[] =
	{
		{	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0		},
		{	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0		},
		{	0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0		},
		{	0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0		},
		{	1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0	},
		{	1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0	},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(p3nctw4, &(m_pDecl[RN_P3NCTW4]));
	

	// P3NCTW4T
	D3DVERTEXELEMENT9 p3nctw4t[] =
	{
		{	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0		},
		{	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0			},
		{	0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0		},
		{	0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0		},
		{	1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0	},
		{	1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0	},
		{	2, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0		},		
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(p3nctw4t, &(m_pDecl[RN_P3NCTW4T]));

	// P3NCTW8
	D3DVERTEXELEMENT9 p3nctw8[] =
	{
		{	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0		},
		{	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0		},
		{	0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0		},
		{	0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0		},
		{	1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0	},
		{	1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 1	},
		{	1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0	},
		{	1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 1	},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(p3nctw8, &(m_pDecl[RN_P3NCTW8]));


	// P3NCTW8T
	D3DVERTEXELEMENT9 p3nctw8t[] =
	{
		{	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0		},
		{	0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0			},
		{	0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0		},
		{	0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0		},
		{	1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0	},
		{	1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 1	},
		{	1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0	},
		{	1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 1	},
		{	2, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0		},		
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(p3nctw8t, &(m_pDecl[RN_P3NCTW8T]));
	

#ifdef _DEBUG
	D3DVERTEXELEMENT9 dDecl[] =
	{
		{	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0		},
		{	0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0		},
		{	0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0		},
		{	0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0	},
		{	0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0	},
		{	0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0		},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(dDecl, &(m_debugDecl[0]));

	D3DVERTEXELEMENT9 dDecl2[] =
	{
		{	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0		},
		{	0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0		},
		{	0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0		},
		{	0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0	},
		{	0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 1	},
		{	0, 60, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0	},
		{	0, 76, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 1	},
		{	0, 92, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0		},
		D3DDECL_END()
	};
	m_pDevice->CreateVertexDeclaration(dDecl2, &(m_debugDecl[1]));
	
	m_lineLength	= m_pEff->GetParameterByName(0, "lineLength");
#endif

	return true;
}


void NMeshRenderer::RenderAniMesh(NAnimateMesh& mesh)
{
	static NVECTOR4 vecEye;
	static NMATRIX matInvWT, matWVP;
	static NCamera* curCamera;
	
	curCamera = m_pCamera->GetCurCamera();
	matWVP =	mesh.m_matCalculate * 
				curCamera->GetViewProj();
	mesh.m_matCalculate.Inverse44( matInvWT );
	matInvWT.MultiplyCoord(vecEye.GetVector(), curCamera->GetPosition());
	
	m_pDevice->FVF( P3NC_VERTEX::FVF );

	m_pEff = m_pShader->GetShader();

	m_pEff->SetMatrix( m_matWVP, matWVP );
	m_pEff->SetVector( m_vecEye, vecEye );

	m_pEff->SetTechnique( m_aniTech );
	m_pEff->Begin( NULL, 0 );
	m_pEff->BeginPass(0);
	
	m_pDevice->DrawUP( (void*)&mesh.m_vertData.at(0),
					   sizeof(P3NC_VERTEX),	mesh.m_faceCount );
	
	m_pEff->EndPass();
	m_pEff->End();
}

void NMeshRenderer::RenderSkinMesh( NSkinMesh& mesh )
{
	m_pDevice->VertexDeclaration( m_pDecl[ mesh.m_type ] );
	m_pDevice->Indices( mesh.m_optIB );

	// 정점 버퍼 설정
	(this->*m_pfnSetBuffer[ mesh.m_type ])(mesh);

	m_pEff->SetTechnique( m_skinTech );
	m_pEff->Begin( NULL, 0 );
	m_pEff->BeginPass(mesh.m_type);

	for(int i = 0; i < mesh.m_faceGroupCount; ++i)
	{
		static NFaceGroup* pGroup;
		pGroup = &mesh.m_faceGroup[i];

		if( pGroup->mtlRef < 0)
		{
			m_texmapUse[STM_AMBIENT] = FALSE;
			m_texmapUse[STM_SPECULAR] = FALSE;
			m_pEff->SetBoolArray( m_bUseTexmap, m_texmapUse, STM_MAX );
			m_pEff->SetInt( m_specPower, 0 );
			m_pEff->SetTexture( m_texmap[TM_DIFFUSE], 
				I_NDevice->GetDefaultTexture()->GetTexture() );
		}
		else
		{
			NMaterial& curMtl = (*m_pMtlVec)[ pGroup->mtlRef ];

			// 반사광 설정
			m_pEff->SetInt( m_specPower, curMtl.specPower );

			// 텍스쳐 설정
			for(int i = 0; i < curMtl.texmapCount; ++i)
			{
				NTexmap& curTexmap = curMtl.texmap[i];
				if( curTexmap.texture )
				{
					// 텍스쳐 사용 설정
					switch(curTexmap.type)
					{
					case TM_AMBIENT:
						m_texmapUse[STM_AMBIENT] = TRUE;
						break;

					case TM_SPECULAR:
						m_texmapUse[STM_SPECULAR] = TRUE;
						break;
					}
					m_pEff->SetTexture( m_texmap[ curTexmap.type ], 
										curTexmap.texture->GetTexture() );	
				}
				else
				{
					// 텍스쳐 사용 설정
					switch(curTexmap.type)
					{
					case TM_AMBIENT:
						m_texmapUse[STM_AMBIENT] = FALSE;
						break;

					case TM_SPECULAR:
						m_texmapUse[STM_SPECULAR] = FALSE;
						break;
					}
					m_pEff->SetTexture( m_texmap[ curTexmap.type ], NULL );			
				}
			}
			m_pEff->SetBoolArray( m_bUseTexmap, m_texmapUse, STM_MAX );
		}
		
		m_pEff->CommitChanges();
		m_pDevice->DrawIndexed(	mesh.m_vertCount, pGroup->faceCount, pGroup->startIndex );
	}
	
	m_pEff->EndPass();
#ifdef _DEBUG
	if( m_bRenderNorm || m_bRenderTan )
	{
		if( mesh.m_skinType < ST_RIGID_OBJECT )
		{
			m_pDevice->VertexDeclaration( m_debugDecl[0] );
			m_pEff->BeginPass( RN_NORM_TAN_NOWEIGHT );
		}
		else if( mesh.m_maxWeightCount > 4 )
		{
			m_pDevice->VertexDeclaration( m_debugDecl[1] );
			m_pEff->BeginPass( RN_NORM_TAN_WEIGHT8 );
		}
		else
		{
			m_pDevice->VertexDeclaration( m_debugDecl[0] );
			m_pEff->BeginPass( RN_NORM_TAN_WEIGHT4 );
		}
		m_pEff->CommitChanges();
		
		if(m_bRenderNorm)
		{
			if( mesh.m_maxWeightCount > 4 )
			{
				m_pDevice->DrawUP(	&mesh.m_normLine2Vec.at(0), sizeof(LINE_RENDER2),
									mesh.m_faceCount * 3, D3DPT_LINELIST );
			}
			else
			{
				m_pDevice->DrawUP(	&mesh.m_normLineVec.at(0), sizeof(LINE_RENDER),
									mesh.m_faceCount * 3, D3DPT_LINELIST );
			}
		}

		if( !mesh.m_tanVec.empty() && m_bRenderTan)
		{
			if( mesh.m_maxWeightCount > 4 )
			{
				m_pDevice->DrawUP(	&mesh.m_tanLine2Vec.at(0), sizeof(LINE_RENDER2),
									mesh.m_faceCount * 3, D3DPT_LINELIST );
			}
			else
			{
				m_pDevice->DrawUP(	&mesh.m_tanLineVec.at(0), sizeof(LINE_RENDER),
									mesh.m_faceCount * 3, D3DPT_LINELIST );
			}
		}
	
		m_pEff->EndPass();
	}
#endif

	m_pEff->End();
}

void NMeshRenderer::SetBufferP3NCT( NSkinMesh& mesh )
{
	// Position Normal Color Texcoord
	m_pDevice->StreamSource(mesh.m_PNCT_VB, sizeof(P3NCT_VERTEX), 0, 0);
}

void NMeshRenderer::SetBufferP3NCTT( NSkinMesh& mesh )
{
	// Position Normal Color Texcoord Tangent
	m_pDevice->StreamSource(mesh.m_PNCT_VB, sizeof(P3NCT_VERTEX), 0, 0);
	m_pDevice->StreamSource(mesh.m_Tan_VB, sizeof(NVECTOR3), 0, 1);
}

void NMeshRenderer::SetBufferP3NCTIW( NSkinMesh& mesh )
{
	// Position Normal Color Texcoord Index Weight
	m_pDevice->StreamSource(mesh.m_PNCT_VB, sizeof(P3NCT_VERTEX), 0, 0);
	m_pDevice->StreamSource(mesh.m_Weight_VB, sizeof(IW4_VERTEX), 0, 1);
	SetMatrixArr(mesh);
}

void NMeshRenderer::SetBufferP3NCTIWT( NSkinMesh& mesh )
{
	// Position Normal Color Texcoord Index Weight Tangent
	m_pDevice->StreamSource(mesh.m_PNCT_VB, sizeof(P3NCT_VERTEX), 0, 0);
	m_pDevice->StreamSource(mesh.m_Weight_VB, sizeof(IW4_VERTEX), 0, 1);
	m_pDevice->StreamSource(mesh.m_Tan_VB, sizeof(NVECTOR3), 0, 2);
	SetMatrixArr(mesh);
}

void NMeshRenderer::SetBufferP3NCTIW8(NSkinMesh& mesh)
{
	// Position Normal Color Texcoord Index8 Weight8
	m_pDevice->StreamSource(mesh.m_PNCT_VB, sizeof(P3NCT_VERTEX), 0, 0);
	m_pDevice->StreamSource(mesh.m_Weight_VB, sizeof(IW8_VERTEX), 0, 1);
	SetMatrixArr(mesh);
}

void NMeshRenderer::SetBufferP3NCTIW8T(NSkinMesh& mesh)
{
	// Position Normal Color Texcoord Index8 Weight8 Tangent
	m_pDevice->StreamSource(mesh.m_PNCT_VB, sizeof(P3NCT_VERTEX), 0, 0);
	m_pDevice->StreamSource(mesh.m_Weight_VB, sizeof(IW8_VERTEX), 0, 1);
	m_pDevice->StreamSource(mesh.m_Tan_VB, sizeof(NVECTOR3), 0, 2);
	SetMatrixArr(mesh);
}

void NMeshRenderer::SetMatrixArr( NSkinMesh& mesh )
{
	// Set Matrix Array
	m_pEff = m_pShader->GetShader();
	m_pEff->SetVectorArray( m_matRow0, *mesh.m_optMatTransRow0, mesh.m_optMatrixCount );
	m_pEff->SetVectorArray( m_matRow1, *mesh.m_optMatTransRow1, mesh.m_optMatrixCount );
	m_pEff->SetVectorArray( m_matRow2, *mesh.m_optMatTransRow2, mesh.m_optMatrixCount );
}

void NMeshRenderer::PreRender(NObject* obj)
{
	static NVECTOR4 localEye, localLight;
	static NMATRIX invWorld;
	static NCamera* curCamera;
	curCamera = m_pCamera->GetCurCamera();

	// 월드 설정
	m_pEff = m_pShader->GetShader();
	m_pEff->SetMatrix( m_matWVP, 
		obj->m_controlWorld * curCamera->GetViewProj() );
	
	// 카메라 위치 설정
	obj->m_controlWorld.Inverse44( invWorld );
	invWorld.MultiplyCoord( localEye.GetVector(), curCamera->GetPosition() );
	m_pEff->SetVector( m_vecEye, localEye );

	// 포그 설정
	BOOL bUseFog = m_pDevPool->GetUseFog();
	m_pEff->SetBool( m_bUseFog, bUseFog );
	if( bUseFog )
	{
		m_pEff->SetFloat( m_fogNear, m_pDevPool->GetFogNear() );
		m_pEff->SetFloat( m_fogFar, m_pDevPool->GetFogFar() );
	}

	// 조명 설정
	NLight& curLight = m_pDevPool->GetGlobalLight();
	BOOL bUseGLight = m_pDevPool->GetUseGlobalLight();
	m_pEff->SetBool( m_bUseLight, bUseGLight );

	if( bUseGLight )
	{
		// 타입
		m_pEff->SetInt( m_lightType, curLight.type );

		// 속성
		switch(curLight.type)
		{
		case LT_DIRECTIONAL:
			invWorld.MultiplyNormal( localLight.GetVector(), curLight.vDir.GetVector() );
			m_pEff->SetVector( m_vecLight, localLight );
			break;

		case LT_POINT:
			invWorld.MultiplyCoord( localLight.GetVector(), curLight.vPos.GetVector() );
			m_pEff->SetVector( m_vecLight, localLight );
			break;
		}
		
		// 조명 색상
		static NVECTOR4 objDiffuse, objAmbient;
		
		objAmbient.x = curLight.ambient.x * obj->m_ambientCol.x;
		objAmbient.y = curLight.ambient.y * obj->m_ambientCol.y;
		objAmbient.z = curLight.ambient.z * obj->m_ambientCol.z;
		objAmbient.w = 1.0f;

		objDiffuse.x = curLight.diffuse.x * obj->m_diffuseCol.x;
		objDiffuse.y = curLight.diffuse.y * obj->m_diffuseCol.y;
		objDiffuse.z = curLight.diffuse.z * obj->m_diffuseCol.z;
		objDiffuse.w = 1.0f;

		m_pEff->SetVector( m_ambient, objAmbient );
		m_pEff->SetVector( m_diffuse, objDiffuse );
	}
	else
	{
		// 조명 색상
		static NVECTOR4 stdAmb(0.1f, 0.1f, 0.1f, 1.0f);
		static NVECTOR4 stdDiff(1.0f, 1.0f, 1.0f, 1.0f);

		m_pEff->SetVector( m_ambient, stdAmb );
		m_pEff->SetVector( m_diffuse, stdDiff );
	}
}

void NMeshRenderer::SetWorld(const NMATRIX& world)
{
	static NVECTOR4 localEye, localLight;
	static NMATRIX invWorld;
	static NCamera* curCamera;
	curCamera = m_pCamera->GetCurCamera();

	// 월드 설정
	m_pEff = m_pShader->GetShader();
	m_pEff->SetMatrix( m_matWVP, 
		world * curCamera->GetViewProj() );
	
	// 카메라 위치 설정
	world.Inverse44( invWorld );
	invWorld.MultiplyCoord( localEye.GetVector(), curCamera->GetPosition() );
	m_pEff->SetVector( m_vecEye, localEye );

	// 포그 설정
	BOOL bUseFog = m_pDevPool->GetUseFog();
	m_pEff->SetBool( m_bUseFog, bUseFog );
	if( bUseFog )
	{
		m_pEff->SetFloat( m_fogNear, m_pDevPool->GetFogNear() );
		m_pEff->SetFloat( m_fogFar, m_pDevPool->GetFogFar() );
	}

	// 조명 설정
	NLight& curLight = m_pDevPool->GetGlobalLight();
	BOOL bUseGLight = m_pDevPool->GetUseGlobalLight();
	m_pEff->SetBool( m_bUseLight, bUseGLight );

	if( bUseGLight )
	{
		// 타입
		m_pEff->SetInt( m_lightType, curLight.type );

		// 속성
		switch(curLight.type)
		{
		case LT_DIRECTIONAL:
			invWorld.MultiplyNormal( localLight.GetVector(), curLight.vDir.GetVector() );
			m_pEff->SetVector( m_vecLight, localLight );
			break;

		case LT_POINT:
			invWorld.MultiplyCoord( localLight.GetVector(), curLight.vPos.GetVector() );
			m_pEff->SetVector( m_vecLight, localLight );
			break;
		}
		
		// 조명 색상
		m_pEff->SetVector( m_ambient, curLight.ambient );
		m_pEff->SetVector( m_diffuse, curLight.diffuse );
	}
	else
	{
		// 조명 색상
		static NVECTOR4 stdAmb(0.1f, 0.1f, 0.1f, 1.0f);
		static NVECTOR4 stdDiff(1.0f, 1.0f, 1.0f, 1.0f);
		m_pEff->SetVector( m_ambient, stdAmb );
		m_pEff->SetVector( m_diffuse, stdDiff );
	}
}

void NMeshRenderer::SetSolidWorld(const NMATRIX& solidWorld)
{
	m_pEff->SetMatrix( m_matSolidW, solidWorld );
}

void NMeshRenderer::SetMultiply(BOOL bMultiply)
{
	m_pEff->SetBool( m_bMultiply, bMultiply );
}

void NMeshRenderer::SetMaterial(NMtlVec* pMtlVec)
{
	m_pMtlVec = pMtlVec;
}

#ifdef _DEBUG
void NMeshRenderer::SetDebugLineLength(float len)
{
	m_line = max(0.1f, len);
	m_pEff->SetFloat( m_lineLength, m_line );
}
void NMeshRenderer::DeltaDebugLineLength(float len)
{
	m_line = max(0.1f, m_line + len);
	m_pEff->SetFloat( m_lineLength, m_line );
}
#endif

void NMeshRenderer::Remove()
{
	for(int i = 0; i < RN_MAX; ++i)
	{
		SAFE_RELEASE( m_pDecl[i] )
	}

#ifdef _DEBUG
	SAFE_RELEASE( m_debugDecl[0] )
	SAFE_RELEASE( m_debugDecl[1] )
#endif
}

void NMeshRenderer::Release()
{
	for(int i = 0; i < RN_MAX; ++i)
	{
		SAFE_RELEASE( m_pDecl[i] )
	}

#ifdef _DEBUG
	SAFE_RELEASE( m_debugDecl[0] )
	SAFE_RELEASE( m_debugDecl[1] )
#endif
}