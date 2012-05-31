#pragma once

#include "NL_D3d.h"
#include "NL_Singleton.h"

//=========================================================================
//	Name   : ND3dPool
//	Content: 장치 관리자
//=========================================================================
class ND3dPool : public NSingleton< ND3dPool >
{
	DECL_FRIEND( NSingleton< ND3dPool > )

	//==================================
	//	Private Data
	//==================================
	typedef CAtlMap< int, ND3d* >	ND3dMap;
	typedef ND3dMap::CPair			ND3dPair;

	LPDIRECT3D9		m_pD3D;
	ND3dMap			m_pDevMap;
	ND3dPair*		m_tmpPair;
	int				m_curIndex;

	//==================================
	//	Ctor & Dtor
	//==================================
	ND3dPool(): m_curIndex(0){}
public:
	~ND3dPool(){	Release();	}

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
	void	ResetResource();
	bool	RecreateDevice(	int				index,
							HWND			hWnd,
							BOOL			bWindowed			= TRUE, 
							UINT			presentInterval		= D3DPRESENT_INTERVAL_IMMEDIATE,
							D3DFORMAT		backBufferFormat	= D3DFMT_UNKNOWN,
							BOOL			bAutoDepthStencil	= TRUE,
							D3DFORMAT		depthStencilFormat	= D3DFMT_D24S8,
							D3DSWAPEFFECT	swapEffect			= D3DSWAPEFFECT_DISCARD);
	void	ChangeSize( HWND hWnd, int x, int y );
	void	Release();
	
private:
	//==================================
	//	Internal Function
	//==================================
	
};

#define Mgr_ND3d			ND3dPool::GetInstance()
#define Mgr_NDev			Mgr_ND3d->GetDevice()
#define Mgr_NDevN(index)	Mgr_ND3d->GetDevice(index)
