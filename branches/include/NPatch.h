#pragma once

#include "NL_Base.h"
#include "NL_BoundingBox.h"

enum PATCH_NEIGHBOR_DIR
{
	NB_LEFT,
	NB_RIGHT,
	NB_TOP,
	NB_BOTTOM,

	NB_MAX_DIR
};

enum PATCH_CORNER
{
	PC_LT,
	PC_RT,
	PC_LB,
	PC_RB,

	PC_MAX
};

#define ALPHA_TEX_SIZE 64

//=========================================================================
//	Name   : NPatch
//	Content: 패치 구조체
//=========================================================================
class ND3d;
class NTexture;
class NTerrain;
class NTerrainRenderer;
struct NQuadNode;
struct NPatch
{
	//==================================
	//	Patch Data
	//==================================
	ND3d*			m_pDevice;
	int				m_patchID;
	int				m_cellWidth;
	int				m_cellCount;
	int				m_vertWidth;
	int				m_vertCount;
	int				m_faceCount;
	int				m_centerIndex;
	float			m_camDistance;
	int				m_lodLevel;
	int				m_lodIndex;
	int**			m_lodFaceCount;
	
	NTexture*			m_splattingTex[4];
	BOOL				m_showSplattingTex[5];
	LPDIRECT3DTEXTURE9	m_alphaTex;
	DWORD				m_alphaTexVal[ALPHA_TEX_SIZE * ALPHA_TEX_SIZE];
	NVECTOR3*			m_corner[PC_MAX];
	NQuadNode*			m_pPatchQuad;
	NTerrain*			m_pTerrain;

	//==================================
	//	Neighbor
	//==================================
	NPatch*			m_neighborPatch[NB_MAX_DIR];

	//==================================
	//	Buffer
	//==================================
	P3NCT_VERTEX**			m_vertexArray;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DINDEXBUFFER9**	m_ppLODIB;

	//==================================
	//	Renderer
	//==================================
	NTerrainRenderer*	m_pRenderer;

	//==================================
	//	Ctor & Dtor
	//==================================
	NPatch();
	~NPatch();

	//==================================
	//	External Function
	//==================================
	void Init(NTerrainRenderer* pRenderer);
	void Render();
	void Remove();
	bool Reset();
	void Release();

	void CalcBoundingBox();
	bool SetRenderBuffer();
	void NotifyRender();
};