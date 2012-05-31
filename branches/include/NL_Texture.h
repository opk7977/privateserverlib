#pragma once

#include "NL_Base.h"
#include "NL_Resource.h"
#include "NL_TemplatePool.h"

//=========================================================================
//	Name   : NTexture
//	Content: 텍스쳐 클래스
//=========================================================================
class NTexture : public NResource
{
	//==================================
	//	Private Data
	//==================================
	LPDIRECT3DTEXTURE9		m_texture;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NTexture():m_texture(0){}
	~NTexture(){}

	//==================================
	//	External Function
	//==================================
	void Init();
	bool Load(const TCHAR* filePath);
	void Remove();
	bool Reset();
	void Release();

	NL_INLINE
	LPDIRECT3DTEXTURE9
		GetTexture()	const	{	return m_texture;		}
	NL_INLINE
	bool
		IsLoaded()		const	{	return m_texture != 0;	}

private:
	//==================================
	//	Internal Function
	//==================================
};

#define   Mgr_NTexture  NTemplatePool< NTexture >::GetInstance()