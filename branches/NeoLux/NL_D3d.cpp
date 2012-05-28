#include "NL_D3d.h"
#include "NL_Camera.h"
#include "NL_FontPool.h"
#include "NL_Debug.h"

HRESULT ND3d::Reset(LPDIRECT3D9 pD3d)
{
	MSG msg = {0};
	HRESULT hr = E_FAIL;
	D3DPRESENT_PARAMETERS tmpParam = m_presentParam;
	hr = m_pd3dDevice->Reset( &tmpParam );
	while(hr != S_OK)
	{
		hr = m_pd3dDevice->TestCooperativeLevel();
		if(hr == D3DERR_DEVICENOTRESET)
		{
			D3DPRESENT_PARAMETERS tmpParam = m_presentParam;
			hr = m_pd3dDevice->Reset( &tmpParam );
			if(hr == D3DERR_DEVICELOST)
				continue;
			
			if(hr == D3DERR_DEVICEREMOVED)
			{
				return E_FAIL;
			}

			if(hr == D3DERR_DRIVERINTERNALERROR)
			{
				return E_FAIL;
			}

			if(hr == D3DERR_OUTOFVIDEOMEMORY)
			{
				return E_FAIL;
			}
		}
		else if(hr == D3DERR_DEVICELOST)
		{
			if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			
			continue;
		}
	}
	
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	pBackBuffer->GetDesc( &m_backBuffer );
	pBackBuffer->Release();

	ViewPort(&m_viewPort);

	RenderState( D3DRS_ZENABLE, TRUE );
	RenderState( D3DRS_LIGHTING, (DWORD)FALSE );

	return S_OK;
}

HRESULT ND3d::Release(void)
{
	//SAFE_RELEASE( m_pd3dDevice );
	HRESULT hr = S_FALSE;
	if( m_pd3dDevice )
	{
		hr = m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
	
	return hr;
}