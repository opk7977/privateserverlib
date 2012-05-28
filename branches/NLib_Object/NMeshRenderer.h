#pragma once

#include "NL_Base.h"
#include "NL_Math.h"
#include "NL_Renderer.h"
#include "NMaterial.h"

//==================================
//	Render Type
//==================================
enum MESH_RENDER_SOURCE
{
	RS_P3		= 0x01,
	RS_NORM		= 0x02,
	RS_COL		= 0x04,
	RS_TEX		= 0x08,
	RS_WEIGHT4	= 0x10,
	RS_WEIGHT8	= 0x20,
	RS_TAN		= 0x40,
	RS_BASE		= RS_P3 | RS_NORM | RS_COL | RS_TEX
};

enum MESH_RENDER_TYPE
{
	RT_P3NCT	= RS_BASE,					// POSITION, NORMAL, COLOR, TEXCOORD
	RT_P3NCTT	= RS_BASE  | RS_TAN,		// POSITION, NORMAL, COLOR, TEXCOORD, TANGENT
	RT_P3NCTW4	= RS_BASE  | RS_WEIGHT4,	// POSITION, NORMAL, COLOR, TEXCOORD, INDEX4, WEIGHT4
	RT_P3NCTW4T = RT_P3NCTW4 | RS_TAN,		// POSITION, NORMAL, COLOR, TEXCOORD, INDEX4, WEIGHT4, TANGENT
	RT_P3NCTW8	= RS_BASE  | RS_WEIGHT8,	// POSITION, NORMAL, COLOR, TEXCOORD, INDEX8, WEIGHT8
	RT_P3NCTW8T	= RT_P3NCTW8 | RS_TAN,		// POSITION, NORMAL, COLOR, TEXCOORD, INDEX8, WEIGHT8, TANGENT

	RT_MAX
};

enum MESH_RENDER_NUM
{
	RN_P3NCT,		
	RN_P3NCTT,		
	RN_P3NCTW4,		
	RN_P3NCTW4T,
	RN_P3NCTW8,
	RN_P3NCTW8T,

	RN_MAX
};

#ifdef _DEBUG
enum MESH_NORM_TAN_LINE
{
	RN_NORM_TAN_WEIGHT4 = RN_MAX,
	RN_NORM_TAN_WEIGHT8,
	RN_NORM_TAN_NOWEIGHT
};
#endif

//=========================================================================
//	Name   : NMeshRenderer
//	Content: 메쉬 렌더러 클래스
//=========================================================================
class NShader;
class ND3d;
class NDevice;
class NCameraPool;
struct NAnimateMesh;
struct NSkinMesh;
class NObject;
class NMeshRenderer : public NRenderer
{
	//==================================
	//	Private Data
	//==================================
	NDevice*			m_pDevPool;
	ND3d*				m_pDevice;
	NCameraPool*		m_pCamera;
	NMtlVec*			m_pMtlVec;
	BOOL				m_texmapUse[STM_MAX];
	LPD3DXEFFECT		m_pEff;
	D3DXHANDLE			m_aniTech;
	D3DXHANDLE			m_skinTech;
	D3DXHANDLE			m_matWVP;
	D3DXHANDLE			m_matWIT;
	D3DXHANDLE			m_matSolidW;
	D3DXHANDLE			m_bMultiply;
	D3DXHANDLE			m_matWorld;
	D3DXHANDLE			m_vecEye;
	D3DXHANDLE			m_vecLight;
	D3DXHANDLE			m_matArr;
	D3DXHANDLE			m_matRow0;
	D3DXHANDLE			m_matRow1;
	D3DXHANDLE			m_matRow2;
	D3DXHANDLE			m_specPower;
	D3DXHANDLE			m_bUseTexmap;
	D3DXHANDLE			m_texmap[TM_MAX];
	D3DXHANDLE			m_bUseFog;
	D3DXHANDLE			m_fogNear;
	D3DXHANDLE			m_fogFar;
	D3DXHANDLE			m_bUseLight;
	D3DXHANDLE			m_lightType;
	D3DXHANDLE			m_lightRange;
	D3DXHANDLE			m_ambient;
	D3DXHANDLE			m_diffuse;

	typedef void (NMeshRenderer::*PFN_SETBUFFER)(NSkinMesh&);
	PFN_SETBUFFER					m_pfnSetBuffer[ RN_MAX ];
	LPDIRECT3DVERTEXDECLARATION9	m_pDecl[ RN_MAX ];

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NMeshRenderer():m_pEff(0)
#ifdef _DEBUG
		,m_bRenderNorm(false)
		,m_bRenderTan(false)
		,m_line(1.0f)
#endif
	{
#ifdef _DEBUG
		m_debugDecl[0] = NULL;
		m_debugDecl[1] = NULL;
#endif
	}
	~NMeshRenderer(){}

	//==================================
	//	External Function
	//==================================
	void Init();
	bool Load(const TCHAR* fileName);
	void RenderAniMesh( NAnimateMesh& mesh );
	void RenderSkinMesh( NSkinMesh& mesh );
	void SetWorld(const NMATRIX& world);
	void PreRender(NObject* obj);
	void SetSolidWorld(const NMATRIX& solidWorld);
	void SetMultiply(BOOL bMultiply);
	void SetMaterial(NMtlVec* pMtlVec);
	void Remove();
	bool Reset();
	void Release();

#ifdef _DEBUG
	LPDIRECT3DVERTEXDECLARATION9	m_debugDecl[2];
	bool							m_bRenderNorm;
	bool							m_bRenderTan;
	D3DXHANDLE						m_lineLength;
	float							m_line;
	void SetDebugLineLength(float len);
	void DeltaDebugLineLength(float len);
#endif

private:
	void SetBufferP3NCT( NSkinMesh& mesh );
	void SetBufferP3NCTT( NSkinMesh& mesh );
	void SetBufferP3NCTIW( NSkinMesh& mesh );
	void SetBufferP3NCTIWT( NSkinMesh& mesh );
	void SetBufferP3NCTIW8( NSkinMesh& mesh );
	void SetBufferP3NCTIW8T( NSkinMesh& mesh );
	void SetMatrixArr( NSkinMesh& mesh );
};