#pragma once

#include "NL_Base.h"
#include "NL_Resource.h"
#include "NL_TemplatePool.h"
#include "NL_Shader.h"

//=========================================================================
//	Name   : NRenderer
//	Content: 렌더러 인터페이스
//=========================================================================
class NRenderer : public NResource
{
protected:
	//==================================
	//	Protected Data
	//==================================
	NShader* m_pShader;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NRenderer(): m_pShader(0){}
	virtual ~NRenderer(){}

	virtual void Init()							= 0;
	virtual bool Load(const TCHAR* fileName)	= 0;
	virtual void Remove()						= 0;
	virtual bool Reset()						= 0;
	virtual void Release()						= 0;
	bool IsLoaded() const {	 return m_pShader != NULL;	}
};

#define	 Mgr_NRenderer	NTemplatePool<NRenderer>::GetInstance()