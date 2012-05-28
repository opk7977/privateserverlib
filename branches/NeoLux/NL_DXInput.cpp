#include "NL_DXInput.h"

//===============================================================
//  NAME	  : NInput
//  COMMENTS  : 입력 클래스 구현
//===============================================================
//============================
//	Constructor & Destructor
//============================
NDXInput::NDXInput( void )
{
	m_hWnd = 0;
	m_pDInput = 0;
	m_pMouse = 0;
}

NDXInput::~NDXInput( void )
{
	
}

void NDXInput::Init(HWND hWnd)
{
	Release();

	m_hWnd = hWnd;

	Reset();
}

void NDXInput::Frame(void)
{
	ZeroMemory(&m_mouseState, sizeof(DIMOUSESTATE));
	if( FAILED( m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE), &m_mouseState ) ) )
		while( m_pMouse->Acquire() == DIERR_INPUTLOST );
}

void NDXInput::Reset(void)
{
	if(!m_hWnd)
		return;

	// Direct Input
	DirectInput8Create( ::GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		 IID_IDirectInput8, ( VOID** )&m_pDInput, NULL );
	 
	 // 마우스
	m_pDInput->CreateDevice( GUID_SysMouse, &m_pMouse, NULL );
	m_pMouse->SetDataFormat( &c_dfDIMouse );
	if( FAILED(m_pMouse->SetCooperativeLevel( m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
	{
		while( m_pMouse->Acquire() == DIERR_INPUTLOST );
		m_pMouse->SetCooperativeLevel( m_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );
	}
	while( m_pMouse->Acquire() == DIERR_INPUTLOST );
}

void NDXInput::Remove(void)
{
	Release();
}

void NDXInput::Release( void )
{
	if( m_pMouse )
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = 0;
	}
	SAFE_RELEASE( m_pDInput )
}