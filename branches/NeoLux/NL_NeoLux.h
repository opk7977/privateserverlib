#pragma once

#include "NL_Base.h"
#include "NL_Window.h"
#include "NL_Time.h"
#include "NL_Input.h"
#include "NL_Device.h"
#include "NL_FontPool.h"
#include "NL_Camera.h"
#include "NL_TemplatePool.h"
#include "NL_Shader.h"
#include "NL_Renderer.h"
#include "NL_Texture.h"
#include "NL_Debug.h"
#include "NL_Sky.h"
#include "NL_DXInput.h"
#include "NL_Grid.h"

static const D3DCOLOR NEOLUX_BG_COLOR = 0xff4444ff;

//===============================================================
//  NAME	  : NeoLux
//  COMMENTS  : 엔진 클래스 선언
//===============================================================
class NeoLux : public NWindow
{
protected:
	//============================
	//	Engine Data
	//============================
	NDevice*					m_pDevice;
	ND3d*						m_pD3d;
	NInput*						m_pInput;
	NTime*						m_pTime;
	NFont*						m_pFont;
	NCameraPool*				m_pCameraPool;
	NFontPool*					m_pFontPool;
	NTemplatePool<NTexture>*	m_pTexturePool;
	NTemplatePool<NShader>*		m_pShaderPool;
	NTemplatePool<NRenderer>*	m_pRendererPool;
	NDebug*						m_pDebug;
	NSky*						m_pSky;
	NGrid*						m_pGrid;

	D3DCOLOR		m_bgColor;
	bool			m_bUseSky;
		
public:
	//============================
	//	Constructor & Destructor
	//============================
	NeoLux(void);
	virtual ~NeoLux(void);

	//============================
	//	Engine Function
	//============================
	bool			EngineInit(void);
	bool			EngineFrame(void);
	bool			EngineRender(void);
	bool			EngineRelease(void);

	virtual bool	PreEngineInit(void);
	virtual bool	Init(void) = 0;
	virtual bool	PostEngineInit(void);

	virtual bool	PreEngineFrame(void);
	virtual bool	Frame(void)	= 0;
	virtual bool	PostEngineFrame(void);
	
	virtual bool	PreEngineRender(void);
	virtual bool	Render(void) = 0;
	virtual bool	DebugRender(void);
	virtual bool	PostEngineRender(void);

	virtual bool	Release(void) = 0;

	bool			EngineRun(void);
	int				Run(void);
	
	virtual LRESULT EngineMsgProc(HWND	hWnd,	UINT	msg, 
								WPARAM	wParam,	LPARAM	lParam);

	//=============================
	//	Function
	//=============================
	void SetBgColor(D3DCOLOR bgColor)	{	m_bgColor = bgColor;	}
	void SetUseSky(bool bUse)			{	m_bUseSky = bUse;		}
	bool GetUseSky()			const	{	return m_bUseSky;		}
};