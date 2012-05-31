#pragma once

#include "NL_Base.h"
#include "NL_Renderer.h"

//=========================================================================
//	Name   : NEffectRenderer
//	Content: ¿Ã∆Â∆Æ ∑ª¥ı∑Ø ≈¨∑°Ω∫
//=========================================================================
class ND3d;
class NShader;
class NSprite;
class NCameraPool;
class NEffectRenderer : public NRenderer
{
	//==================================
	//	Private Data
	//==================================
	ND3d*			m_pDev;
	NShader*		m_pShader;
	NCameraPool*	m_pCamera;
	LPD3DXEFFECT	m_pEffect;
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;

	D3DXHANDLE		m_tech;
	D3DXHANDLE		m_matWVP;
	D3DXHANDLE		m_diffuseTex;

	typedef void (NEffectRenderer::*PFN_SET_FUNC)(NSprite* pSpr);
	PFN_SET_FUNC	m_setFunc[2];
	
public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NEffectRenderer();
	~NEffectRenderer();

	//==================================
	//	External Function
	//==================================
	void Init();
	bool Load(const TCHAR* fileName);
	void PreRender();
	void SetForSingleSpr(NSprite* pSpr);
	void SetForMultiSpr(NSprite* pSpr);
	void RenderSprite(NSprite* pSpr);
	void EndRender();
	void Remove();
	bool Reset();
	void Release();
	
private:
	//==================================
	//	Internal Function
	//==================================
	
};