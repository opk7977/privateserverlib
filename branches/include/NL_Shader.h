#pragma once

#include "NL_Base.h"
#include "NL_Resource.h"
#include "NL_TemplatePool.h"

//=========================================================================
//	Name   : NShader
//	Content: 셰이더 클래스
//=========================================================================
class NShader : public NResource
{
	//==================================
	//	Private Data
	//==================================
	LPD3DXEFFECT	m_pEffect;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NShader(): m_pEffect(0){}
	~NShader(){}

	//==================================
	//	External Function
	//==================================
	void Init();
	bool Load(const TCHAR* fileName);
	void Remove();
	bool Reset();
	void Release();

	NL_INLINE
	LPD3DXEFFECT	GetShader()	const	{	return m_pEffect;			}
	NL_INLINE
	bool			IsLoaded()  const	{	return m_pEffect != NULL;	}
};

#define		Mgr_NShader		NTemplatePool<NShader>::GetInstance()