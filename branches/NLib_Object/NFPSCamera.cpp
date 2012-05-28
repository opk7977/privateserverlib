#include "NFPSCamera.h"
#include "NL_Time.h"
#include "NL_Input.h"
#include "NL_DXInput.h"
#include "NTerrain.h"

NFPSCamera::FUNC_MOVE NFPSCamera::m_moveFunc[ MD_MAX ] =
{
	&NFPSCamera::MoveForward2D,
	&NFPSCamera::MoveBackward2D,
	&NFPSCamera::MoveLeft,
	&NFPSCamera::MoveRight,
	&NFPSCamera::MoveForwardLeft2D,
	&NFPSCamera::MoveForwardRight2D,
	&NFPSCamera::MoveBackwardLeft2D,
	&NFPSCamera::MoveBackwardRight2D
};

NFPSCamera::NFPSCamera()
{
	m_fovYLimitTop =	90.0f;
	m_fovYLimitBottom = 90.0f;
}

NFPSCamera::~NFPSCamera()
{
	
}

void NFPSCamera::Init()
{
	m_pDXInput = I_NDXInput;
	NCamera::Init();
}

void NFPSCamera::UpdateView()
{
	m_matCameraWorld._42 = 
		m_pTerrain->GetHeightAt(
		m_matCameraWorld._41, 
		m_matCameraWorld._43) 
		+ m_baseHeight;

	ResetView();
	if( m_bUseFrustum )
	{
		m_frustum.Build(
			m_matCameraWorld.GetRow3(3),
			(m_matCameraView * m_matProjection).Inverse44());
	}
}

void NFPSCamera::RotateXY()
{
	m_pDXInput->Frame();
	if( !m_pDXInput->IsMoved() )
		return;

	static NQUATERNION qR;
	static NMATRIX tmpMat;
	static NVECTOR3 tmpPos;
	static float xDelta, yDelta, delta;

	delta = m_rotateSpeed * (m_pTime->GetElapsedTime() + 0.1f);
	xDelta = (float)m_pDXInput->GetCursorMoveX() * delta;
	yDelta = (float)m_pDXInput->GetCursorMoveY() * delta;

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

void NFPSCamera::MoveFrame(int dir, float delta)
{
	delta *= m_moveSpeed;
	(this->*m_moveFunc[dir])(delta);
}

void NFPSCamera::MoveForward2D(float delta)
{
	m_matCameraWorld._41 += m_matCameraWorld._31 * delta;
	m_matCameraWorld._43 += m_matCameraWorld._33 * delta;

	m_bChange = true;
}

void NFPSCamera::MoveBackward2D(float delta)
{
	m_matCameraWorld._41 += -m_matCameraWorld._31 * delta;
	m_matCameraWorld._43 += -m_matCameraWorld._33 * delta;

	m_bChange = true;
}

void NFPSCamera::MoveLeft(float delta)
{
	m_matCameraWorld._41 += -m_matCameraWorld._11 * delta;
	m_matCameraWorld._43 += -m_matCameraWorld._13 * delta;

	m_bChange = true;
}

void NFPSCamera::MoveRight(float delta)
{
	m_matCameraWorld._41 += m_matCameraWorld._11 * delta;
	m_matCameraWorld._43 += m_matCameraWorld._13 * delta; 

	m_bChange = true;
}

void NFPSCamera::MoveForwardLeft2D(float delta)
{
	m_matCameraWorld._41 += (m_matCameraWorld._31 - m_matCameraWorld._11) * delta;
	m_matCameraWorld._43 += (m_matCameraWorld._33 - m_matCameraWorld._13) * delta;

	m_bChange = true;
}

void NFPSCamera::MoveForwardRight2D(float delta)
{
	m_matCameraWorld._41 += (m_matCameraWorld._31 + m_matCameraWorld._11) * delta;
	m_matCameraWorld._43 += (m_matCameraWorld._33 + m_matCameraWorld._13) * delta;

	m_bChange = true;
}

void NFPSCamera::MoveBackwardLeft2D(float delta)
{
	m_matCameraWorld._41 += -(m_matCameraWorld._31 + m_matCameraWorld._11) * delta;
	m_matCameraWorld._43 += -(m_matCameraWorld._33 + m_matCameraWorld._13) * delta;

	m_bChange = true;
}

void NFPSCamera::MoveBackwardRight2D(float delta)
{
	m_matCameraWorld._41 += (m_matCameraWorld._11 - m_matCameraWorld._31) * delta;
	m_matCameraWorld._43 += (m_matCameraWorld._13 - m_matCameraWorld._33) * delta;

	m_bChange = true;
}

//////////////////////////////////////////////////////////////////////////

void NFPSCamera::SetPosition(const NVECTOR3& pos)
{
	m_matCameraWorld._41 = pos.x;
	m_matCameraWorld._42 = pos.y + m_baseHeight;
	m_matCameraWorld._43 = pos.z;
}

void NFPSCamera::SetPosition(float x, float y, float z)
{
	m_matCameraWorld._41 = x;
	m_matCameraWorld._42 = y + m_baseHeight;
	m_matCameraWorld._43 = z;
}

const NVECTOR3 NFPSCamera::GetPosition() const
{
	static NVECTOR3 tmpVec;
	tmpVec.x = m_matCameraWorld._41;
	tmpVec.y = m_matCameraWorld._42 - m_baseHeight;
	tmpVec.z = m_matCameraWorld._43;

	return tmpVec;
}
