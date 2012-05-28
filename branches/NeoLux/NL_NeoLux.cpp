#include "NL_NeoLux.h"

//===============================================================
//  NAME	  : NeoLux
//  COMMENTS  : 엔진 클래스 구현
//===============================================================
//============================
//	Constructor & Destructor
//============================
NeoLux::NeoLux(void) : 
	m_pD3d(0),
	m_pInput(0),
	m_pTime(0),
	m_bgColor(NEOLUX_BG_COLOR),
	m_bUseSky(false)
{

}

NeoLux::~NeoLux(void)
{

}

//============================
//	Function
//============================
//////////////////////////////////////////////////////////////////////////////
//
//	Init
//
bool NeoLux::EngineInit(void)
{
	PreEngineInit();
	Init();
	PostEngineInit();

	return true;
}

bool NeoLux::PreEngineInit(void)
{
	m_pDevice = I_NDevice;
	m_pDevice->Init();
	m_pDevice->AddDevice( GetHwnd() );
	m_pD3d   = I_ND3d;

	m_pInput = I_NInput;
	m_pInput->Init( GetHwnd() );

	m_pTime  = I_NTime;
	m_pTime->Init();

	m_pFontPool = Mgr_NFont;
	m_pFontPool->Init();
	m_pFont = m_pFontPool->GetFont( _T("Comic Sans MS"), 24, true );

	m_pCameraPool = I_NCamera;
	m_pCameraPool->Init();

	m_pShaderPool = Mgr_NShader;
	m_pShaderPool->SetDir(_T("../fx/"));

	m_pRendererPool = Mgr_NRenderer;
	m_pShaderPool->SetDir(_T("../fx/"));
	
	m_pTexturePool = Mgr_NTexture;
	m_pTexturePool->SetDir(_T("../img/"));
	m_pDevice->SetDefaultTexture(_T("defaultTex.png"));

	m_pDebug = I_NDebug;
	m_pDebug->Init(m_pFont);

	m_pSky = I_NSky;
	m_pSky->Init();

	m_pGrid = I_NGrid;
	m_pGrid->Init();
	m_pGrid->SetGrid();
	
	return true;
}

bool NeoLux::Init(void)
{
	return true;
}

bool NeoLux::PostEngineInit(void)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//	Frame
//
bool NeoLux::EngineFrame(void)
{
	PreEngineFrame();
	Frame();
	PostEngineFrame();

	return true;
}

bool NeoLux::PreEngineFrame(void)
{
	m_pInput->Frame();
	m_pTime->Frame();
	m_pCameraPool->GetCurCamera()->Frame();
	m_pDebug->Frame();

	return true;
}

bool NeoLux::Frame(void)
{
	return true;
}

bool NeoLux::PostEngineFrame(void)
{
	m_pInput->ResetMouseWheel();

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//	Render
//
bool NeoLux::EngineRender(void)
{
	PreEngineRender();
	Render();
	PostEngineRender();

	return true;
}

bool NeoLux::PreEngineRender(void)
{
	if( !m_pDebug->IsFillModeSolid() || !m_bUseSky || !m_pSky->IsInit() )
	{
		m_pD3d->Clear( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_bgColor );
		m_pD3d->BeginScene();
	}
	else
	{
		m_pD3d->BeginScene();
		m_pSky->Render();
	}	

	return true;
}

bool NeoLux::Render(void)
{
	return true;
}

bool NeoLux::DebugRender(void)
{
	m_pDebug->Render();
	return true;
}

bool NeoLux::PostEngineRender(void)
{
	DebugRender();
	
	m_pD3d->EndScene();
	m_pD3d->Present();

	return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//	Release
//
bool NeoLux::EngineRelease(void)
{
	Release();

	::ReleasePoolObject();
	::ReleaseSingletonObject();

// 	NTime::ReleaseInstance();
// 	NInput::ReleaseInstance();
// 	NCameraPool::ReleaseInstance();
// 	NFontPool::ReleaseInstance();
// 	NTemplatePool<NTexture>::ReleaseInstance();
// 	NTemplatePool<NShader>::ReleaseInstance();
// 	NTemplatePool<NRenderer>::ReleaseInstance();
// 	NDebug::ReleaseInstance();
// 	NDevice::ReleaseInstance();
// 	NSky::ReleaseInstance();
// 	NDXInput::ReleaseInstance();
		
	return true;
}

bool NeoLux::Release(void)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//	Run
//
bool NeoLux::EngineRun(void)
{
	EngineFrame();
	EngineRender();

	return true;
}

int NeoLux::Run(void)
{
	MSG		msg = { 0 };
	
	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			EngineRun();
		}
	}

	EngineRelease();
	return (int)msg.wParam;
}

LRESULT NeoLux::EngineMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_MOUSEWHEEL:
		if(m_pInput)
			m_pInput->SetMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		return 0;

	case WM_SIZE:
		if(m_pD3d)
			m_pDevice->ChangeSize(LOWORD(lParam), HIWORD(lParam));
		break;
	}

	return -1;
}