#pragma once

#include "NL_Base.h"
#include "NL_Renderer.h"

enum TERRAIN_TEXTURE
{
	TT_BASE,
	TT_SPLATTING1,
	TT_SPLATTING2,
	TT_SPLATTING3,
	TT_SPLATTING4,
	TT_ALPHA,

	TT_MAX
};

//=========================================================================
//	Name   : NTerrainRenderer
//	Content: 지형 렌더러 클래스
//=========================================================================
class NTerrain;
class NShader;
class ND3d;
class NDevice;
class NCameraPool;
struct NPatch;
class NTerrainRenderer : public NRenderer
{
	//==================================
	//	Private Data
	//==================================
	NDevice*		m_pDevPool;
	NCameraPool*	m_pCamera;
	ND3d*			m_pDevice;
	LPD3DXEFFECT	m_pEff;
	D3DXHANDLE		m_tech;
	D3DXHANDLE		m_matVP;
	D3DXHANDLE		m_vecEye;
	D3DXHANDLE		m_lightType;
	D3DXHANDLE		m_lightRange;
	D3DXHANDLE		m_vecLight;
	D3DXHANDLE		m_showLayer;
	D3DXHANDLE		m_texmap[ TT_MAX ];
	D3DXHANDLE		m_fogNear;
	D3DXHANDLE		m_fogFar;
	D3DXHANDLE		m_bFog;
	D3DXHANDLE		m_bLight;
	D3DXHANDLE		m_ambient;
	D3DXHANDLE		m_diffuse;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NTerrainRenderer();
	~NTerrainRenderer();

	//==================================
	//	External Function
	//==================================
	void Init();
	bool Load(const TCHAR* fileName);
	void PreRender(NTerrain* pTerrain);
	void RenderPatch( NPatch& patch );
	void PostRender();
	void Remove();
	bool Reset();
	void Release();
};