#include "NL_FPSCamera.h"
#include "NL_Time.h"
#include "NL_Input.h"
#include "NL_DXInput.h"

NFPSCamera::NFPSCamera()
{
	m_fovYLimitTop =	90.0f;
	m_fovYLimitBottom = 90.0f;
}

void NFPSCamera::Init()
{
	m_pDXInput = I_NDXInput;
	NCamera::Init();
}

void NFPSCamera::FrameEx()
{
	if(m_bLock)
		return;

	m_bMove		= false;
	m_bChange	= false;
	static float delta;
	static float invRoot2 = 1.0f / 1.414f;
	static int flag;

	flag = 0;
	if		( m_pInput->IsKeydown('W') )	flag |= 0x4;
	else if	( m_pInput->IsKeydown('S') )	flag |= 0x2;
	if		( m_pInput->IsKeydown('D') )	flag |= 0x1;
	else if	( m_pInput->IsKeydown('A') )	flag |= 0x8;

	if(flag)
	{
		delta = m_moveSpeed * ( m_pTime->GetElapsedTime() + 0.1f );
		if( m_pInput->IsKeydown(VK_SHIFT) )
		{
			delta *= 2.0f;
		}

		switch( flag )
		{
		case 0x4:	// »ó
			MoveForword2D( delta );
			break;

		case 0x2:	// ÇÏ
			MoveForword2D( -delta );
			break;

		case 0x1:	// ¿ì
			MoveSide2D( delta );
			break;

		case 0x8:	// ÁÂ
			MoveSide2D( -delta );
			break;

		case 0x5:	// ¿ì»ó
			delta *= invRoot2;
			MoveForword2D( delta );
			MoveSide2D( delta );
			break;

		case 0xc:	// ÁÂ»ó
			delta *= invRoot2;
			MoveForword2D( delta );
			MoveSide2D( -delta );
			break;

		case 0x3:	// ¿ìÇÏ
			delta *= invRoot2;
			MoveForword2D( -delta );
			MoveSide2D( delta );
			break;

		case 0xa:	// ÁÂÇÏ
			delta *= invRoot2;
			MoveForword2D( -delta );
			MoveSide2D( -delta );
			break;
		}
		m_bChange = true;
		m_bMove = true;
	}	
	
	m_pDXInput->Frame();
	if( m_pDXInput->IsMoved() )
	{
		delta = m_rotateSpeed * ( m_pTime->GetElapsedTime() + 0.1f );
		RotateXY(	m_pDXInput->GetCursorMoveX() * delta,
					m_pDXInput->GetCursorMoveY() * delta );
	}
}

void NFPSCamera::UpdateView()
{
	NCamera::Frame();
}

void NFPSCamera::RotateXY(float xDelta, float yDelta)
{
	static NQUATERNION qR;
	static NMATRIX tmpMat;
	static NVECTOR3 tmpPos;

	m_yAngle += xDelta;
	m_xAngle += yDelta;

	if( m_xAngle < -m_fovYLimitBottom )
		m_xAngle = -m_fovYLimitBottom;
	else if( m_xAngle > m_fovYLimitTop )
		m_xAngle = m_fovYLimitTop;

	tmpPos = m_matCameraWorld.GetRow3(3);
	D3DXQuaternionRotationYawPitchRoll( qR, 
		DEG_TO_RAD(m_yAngle), 
		DEG_TO_RAD(m_xAngle), 
		0.0f );
	D3DXMatrixAffineTransformation( m_matCameraWorld, 1.0f, NULL, qR, tmpPos );

	m_bChange = true;
}

void NFPSCamera::MoveForword2D(float delta)
{
	m_matCameraWorld._41 += m_matCameraWorld._31 * delta;
	m_matCameraWorld._43 += m_matCameraWorld._33 * delta;
}

void NFPSCamera::MoveSide2D(float delta)
{
	m_matCameraWorld._41 += m_matCameraWorld._11 * delta;
	m_matCameraWorld._43 += m_matCameraWorld._13 * delta;
}