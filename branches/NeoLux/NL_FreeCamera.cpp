#include "NL_FreeCamera.h"
#include "NL_Time.h"
#include "NL_Input.h"

NFreeCamera::NFreeCamera()
{

}
NFreeCamera::~NFreeCamera()
{

}

void NFreeCamera::Init()
{
	NCamera::Init();
}

void NFreeCamera::Frame()
{
	static float moveDelta;
	static short wheelData;

	m_bChange = false;

	wheelData = m_pInput->GetWheel();
	if(wheelData != 0)
	{
		if( wheelData > 0 )
			this->IncreaseMoveSpeed();
		else
			this->DecreaseMoveSpeed();
	}

	if( m_pInput->IsKeydown( VK_SHIFT ) )
	{
		moveDelta = 10.0f * m_moveSpeed * m_pTime->GetElapsedTime();
	}
	else
	{
		moveDelta = m_moveSpeed * m_pTime->GetElapsedTime();
	}

	if( m_pInput->IsKeydown( m_cameraKey[ FCK_RIGHT ] ) )
	{
		MoveSide( moveDelta );
	}
	else if( m_pInput->IsKeydown( m_cameraKey[ FCK_LEFT ] ) )
	{
		MoveSide( -moveDelta );
	}

	if( m_pInput->IsKeydown( m_cameraKey[ FCK_UP ] ) )
	{
		MoveUp( moveDelta );
	}
	else if( m_pInput->IsKeydown( m_cameraKey[ FCK_DOWN ] ) )
	{
		MoveUp( -moveDelta );
	}

	if( m_pInput->IsKeydown( m_cameraKey[ FCK_FORWARD ] ) )
	{
		MoveForward( moveDelta );
	}
	else if( m_pInput->IsKeydown( m_cameraKey[ FCK_BACKWARD ] ) )
	{
		MoveForward( -moveDelta );
	}

	if( m_pInput->IsKeydown(VK_RBUTTON) )
	{
		static POINT pt;
		pt = m_pInput->GetCursorMove();
		if( pt.x != 0 || pt.y != 0 )
		{
			static float deltaElapse;
			static float Xdelta, Ydelta;
			deltaElapse = m_rotateSpeed * ( m_pTime->GetElapsedTime() + 0.1f );
			Xdelta = (float)pt.y * deltaElapse;
			Ydelta = (float)pt.x * deltaElapse;
			RotateXY( Xdelta, Ydelta );
		}
	}

	NCamera::Frame();
}

void NFreeCamera::Render()
{

}

void NFreeCamera::Reset()
{
	m_cameraKey[ FCK_RIGHT    ] = 'D';
	m_cameraKey[ FCK_LEFT     ] = 'A';
	m_cameraKey[ FCK_UP       ] = 'E';
	m_cameraKey[ FCK_DOWN     ] = 'Q';
	m_cameraKey[ FCK_FORWARD  ] = 'W';
	m_cameraKey[ FCK_BACKWARD ] = 'S';

	NCamera::Reset();
}

void NFreeCamera::Release()
{
	ZeroMemory( m_cameraKey, sizeof( ULONG ) * FCK_MAX );

	NCamera::Release();
}