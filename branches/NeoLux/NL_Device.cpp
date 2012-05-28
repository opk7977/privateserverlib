#include "NL_Device.h"
#include "NL_Camera.h"
#include "NL_FontPool.h"
#include "NL_Texture.h"
#include "NL_Shader.h"
#include "NL_Renderer.h"
#include "NL_Debug.h"
#include "NL_Sky.h"
#include "NL_DXInput.h"

bool NDevice::Init()
{
	if( ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL )
		return false;

	return true;
}

int NDevice::AddDevice(HWND hWnd, 
						BOOL bWindowed /*= TRUE*/, 
						UINT presentInterval /*= D3DPRESENT_INTERVAL_IMMEDIATE*/, 
						D3DFORMAT backBufferFormat /*= D3DFMT_UNKNOWN*/, 
						BOOL bAutoDepthStencil /*= TRUE*/, 
						D3DFORMAT depthStencilFormat /*= D3DFMT_D24S8*/, 
						D3DSWAPEFFECT swapEffect /*= D3DSWAPEFFECT_DISCARD */ )
{
	ND3d* pNewDev = new ND3d;
	if( FAILED( pNewDev->CreateDevice(m_pD3D, hWnd, bWindowed, presentInterval,
		backBufferFormat, bAutoDepthStencil, depthStencilFormat, swapEffect) ) )
	{
		delete pNewDev;
		return -1;
	}

	m_pDevMap[m_curIndex] = pNewDev;
	return m_curIndex;
}

void NDevice::ChangeSize( int x, int y )
{
	Reset();
	I_NCamera->GetCurCamera()->SetAspect( float(x) / y );
}

void NDevice::SetDefaultTexture(const TCHAR* fileName)
{
	m_defaultTex = Mgr_NTexture->GetByName(fileName);
}

void NDevice::SetGlobalLight(const NLight& light)
{
	m_globalLight = light;
}

void NDevice::SetGLDirection(const NVECTOR3& dir)
{
	NVECTOR3 normalizedDir = dir;
	normalizedDir.Normalize();
	m_globalLight.vDir.x = normalizedDir.x;
	m_globalLight.vDir.y = normalizedDir.y;
	m_globalLight.vDir.z = normalizedDir.z;
}

void NDevice::SetGLAmbient(const NVECTOR3& value)
{
	m_globalLight.ambient.x = value.x;
	m_globalLight.ambient.y = value.y;
	m_globalLight.ambient.z = value.z;
}

void NDevice::SetGLAmbient(int r, int g, int b)
{
	static float inv255 = 1.0f / 255.0f;
	m_globalLight.ambient.x = r * inv255;
	m_globalLight.ambient.y = g * inv255;
	m_globalLight.ambient.z = b * inv255;
}

void NDevice::SetGLDiffuse(const NVECTOR3& value)
{
	m_globalLight.diffuse.x = value.x;
	m_globalLight.diffuse.y = value.y;
	m_globalLight.diffuse.z = value.z;
}

void NDevice::SetGLDiffuse(int r, int g, int b)
{
	static float inv255 = 1.0f / 255.0f;
	m_globalLight.diffuse.x = r * inv255;
	m_globalLight.diffuse.y = g * inv255;
	m_globalLight.diffuse.z = b * inv255;
}

void NDevice::RemoveResource()
{
	Mgr_NFont->Remove();
// 	Mgr_NTexture->Remove();
// 	Mgr_NShader->Remove();
// 	Mgr_NRenderer->Remove();
	::RemovePoolObject();
	I_NSky->Remove();
	I_NDXInput->Remove();
}

bool NDevice::Reset( int index /*=0*/ )
{
	RemoveResource();
	ND3d* pDev = m_pDevMap[ index ];
	pDev->Reset( m_pD3D );
	ResetResource();

	return true;
}

void NDevice::ResetResource()
{
	Mgr_NFont->Reset();
// 	Mgr_NTexture->Reset();
// 	Mgr_NShader->Reset();
// 	Mgr_NRenderer->Reset();
	::ResetPoolObject();
	I_NSky->Reset();
	I_NDXInput->Reset();
}

void NDevice::Release()
{
	POSITION pos = m_pDevMap.GetStartPosition();
	while(pos)
	{
		m_tmpPair = m_pDevMap.GetNext(pos);
		SAFE_DELETE_AFTER_RELEASE( m_tmpPair->m_value );
	}
	m_pDevMap.RemoveAll();
	SAFE_RELEASE(m_pD3D)
	m_curIndex = 0;
}
