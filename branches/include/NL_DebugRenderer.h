#pragma once

#include "NL_Base.h"
#include "NL_Renderer.h"

//=========================================================================
//	Name   : NDebugRenderer
//	Content: 디버그 렌더러 클래스
//=========================================================================
class ND3d;
class NDebug;
class NCameraPool;
struct NBoundingBox;
class NDebugRenderer : public NRenderer
{
	//==================================
	//	Private Data
	//==================================
	ND3d*			m_pDev;
	NCameraPool*	m_pCam;
	LPD3DXEFFECT	m_pEff;
	D3DXHANDLE		m_hTech;
	D3DXHANDLE		m_hW;
	D3DXHANDLE		m_hVP;
	LPD3DXLINE		m_pLine;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NDebugRenderer();
	~NDebugRenderer();

	//==================================
	//	External Function
	//==================================
	void Init();
	bool Load(const TCHAR* fileName);
	void PreRender();
	void RenderLine(NDebug* pDebug);
	void RenderLineMulti(NDebug* pDebug);
	void RenderLineList(NDebug* pDebug);
	void RenderTriangle(NDebug* pDebug);
	void RenderAxis(NDebug* pDebug);
	void RenderBox(NDebug* pDebug);
	void RenderSphere(NDebug* pDebug);
	void RenderBoundingBoxWorld(NDebug* pDebug, NMATRIX& matWorld);
	void RenderBoundingBox(NBoundingBox* pBox);
	void RenderCapsule( NDebug* pDebug );
	void RenderCrossLine(NDebug* pDebug);
	void PostRender();
	void Remove();
	bool Reset();
	void Release();
	
private:
	//==================================
	//	Internal Function
	//==================================
	
};