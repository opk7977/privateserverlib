#pragma once

#include "NL_D3d.h"
#include "NL_Light.h"
#include "NL_Singleton.h"

//=========================================================================
//	Name   : NDevice
//	Content: 장치 관리자
//=========================================================================
class NTexture;
class NDevice : public NSingleton< NDevice >
{
	DECL_FRIEND( NSingleton< NDevice > )

	//==================================
	//	Private Data
	//==================================
	typedef CAtlMap< int, ND3d* >	ND3dMap;
	typedef ND3dMap::CPair			ND3dPair;

	LPDIRECT3D9		m_pD3D;
	ND3dMap			m_pDevMap;
	ND3dPair*		m_tmpPair;
	int				m_curIndex;
	NTexture*		m_defaultTex;
	BOOL			m_bUseGlobalLight;
	NLight			m_globalLight;
	BOOL			m_bUseFog;
	float			m_fogNear;
	float			m_fogFar;

	//==================================
	//	Ctor & Dtor
	//==================================
	NDevice():	m_curIndex(0), 
				m_fogFar(1000.0f), 
				m_fogNear(500.0f), 
				m_bUseFog(TRUE),
				m_bUseGlobalLight(TRUE)
	{

	}
public:
	~NDevice(){	}

	//==================================
	//	External Function
	//==================================
	bool	Init();
	int		AddDevice( HWND				hWnd,
					   BOOL				bWindowed			= TRUE, 
					   UINT				presentInterval		= D3DPRESENT_INTERVAL_IMMEDIATE,
					   D3DFORMAT		backBufferFormat	= D3DFMT_UNKNOWN,
					   BOOL				bAutoDepthStencil	= TRUE,
					   D3DFORMAT		depthStencilFormat	= D3DFMT_D24S8,
					   D3DSWAPEFFECT	swapEffect			= D3DSWAPEFFECT_DISCARD	);
	NL_INLINE
	ND3d*	GetDevice(int index = 0)
	{	
		m_tmpPair = m_pDevMap.Lookup(index);
		if(m_tmpPair)
			return m_tmpPair->m_value;

		return NULL;
	}

	void	ChangeSize( int x, int y );
	void	RemoveResource();
	bool	Reset( int index = 0 );
	void	ResetResource();
	void	Release();

//////////////////////////////////////////////////////////////////////////

	// Default Texture
	void		SetDefaultTexture(const TCHAR* fileName);
	NTexture*	GetDefaultTexture()	{	return m_defaultTex;		}
	
	// Light
	BOOL		GetUseGlobalLight()	{	return m_bUseGlobalLight;	}
	void		EnableGlobalLight(BOOL bUse = TRUE)	{	m_bUseGlobalLight = bUse;	}
	void		SetGlobalLight(const NLight& light);
	void		SetGLDirection(const NVECTOR3& dir);
	void		SetGLAmbient(const NVECTOR3& value);
	void		SetGLAmbient(int r, int g, int b);
	void		SetGLDiffuse(const NVECTOR3& value);
	void		SetGLDiffuse(int r, int g, int b);
	NLight&		GetGlobalLight()	{	return m_globalLight;	}
	
	// Fog
	void		EnableFog(BOOL bUse = TRUE)	{	m_bUseFog = bUse;		}
	void		SetFogNear(float nearDist)	{	m_fogNear = nearDist;	}
	void		SetFogFar(float farDist)	{	m_fogFar  = farDist;	}
	BOOL		GetUseFog()		const		{	return m_bUseFog;		}
	float		GetFogNear()	const		{	return m_fogNear;		}
	float		GetFogFar()		const		{	return m_fogFar;		}
	
private:
	//==================================
	//	Internal Function
	//==================================
	
};

#define I_NDevice		NDevice::GetInstance()
#define I_ND3d			I_NDevice->GetDevice()
#define I_ND3dN(index)	I_NDevice->GetDevice(index)
