#pragma once

#include "NL_Base.h"
#include "NL_Singleton.h"

class ND3d;
class NCameraPool;
class NShader;

//=========================================================================
//	Name   : NSky
//	Content: 하늘 경계구 클래스
//=========================================================================
class NSky : public NSingleton< NSky >
{
	DECL_FRIEND( NSingleton< NSky > )

	//==================================
	//	Private Data
	//==================================
	ND3d*					m_pDevice;
	NCameraPool*			m_pCamera;
	NShader*				m_pShader;
	LPD3DXMESH				m_pSphere;
	float					m_skyRadius;
	CString					m_envMapName;
	LPDIRECT3DCUBETEXTURE9	m_envMap;
	LPD3DXEFFECT			m_pSkyFx;

	D3DXHANDLE				m_hTech;
	D3DXHANDLE				m_hWVP;
	D3DXHANDLE				m_hEnvMap;
	
	//==================================
	//	Ctor & Dtor
	//==================================
	NSky();
public:
	~NSky();

	//==================================
	//	External Function
	//==================================
	bool Init( float skyRadius = 10000.0f );
	void Frame();
	void Render();
	void Remove();
	bool Reset();
	void Release();
	bool SetCubeTexture(const TCHAR* name);
	bool IsInit()	const	{	return m_envMap != NULL;	}
	const TCHAR* GetSkyboxFileName()	const {	  return m_envMapName.GetString();		}
	int			 GetSkyboxFileNameLength() const {	return m_envMapName.GetLength();	}

private:
	//==================================
	//	Internal Function
	//==================================
};

#define I_NSky	NSky::GetInstance()