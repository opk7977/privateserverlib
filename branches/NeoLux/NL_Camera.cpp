#include "NL_Camera.h"
#include "NL_Device.h"
#include "NL_Input.h"
#include "NL_Time.h"
#include "NL_Debug.h"

NCamera::NCamera():
	m_bUseFrustum(true)
{
	m_bLock = false;
}

NCamera::~NCamera()
{

}

void NCamera::Init()
{
	Release();

	m_pDevice	= I_ND3d;
	m_pInput	= I_NInput;
	m_pTime		= I_NTime;

	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	
	Reset();
}

void NCamera::Frame()
{
	if( m_bChange )
	{
		ResetView();
		if( m_bUseFrustum )
		{
  			m_frustum.Build(
  				m_matCameraWorld.GetRow3(3),
  				(m_matCameraView * m_matProjection).Inverse44());
		}
	}
}

void NCamera::Render()
{

}

void NCamera::RenderFrustum()
{
	if( m_bUseFrustum )
	{
		NDebug* debug = I_NDebug;
		DWORD col = 0xffffff00;
		debug->DrawLine( m_frustum.vertice[0], m_frustum.vertice[1], col );
		debug->DrawLine( m_frustum.vertice[1], m_frustum.vertice[2], col );
		debug->DrawLine( m_frustum.vertice[2], m_frustum.vertice[3], col );
		debug->DrawLine( m_frustum.vertice[3], m_frustum.vertice[0], col );
		
		debug->DrawLine( m_frustum.vertice[0], m_frustum.vertice[4], col );
		debug->DrawLine( m_frustum.vertice[1], m_frustum.vertice[5], col );
		debug->DrawLine( m_frustum.vertice[2], m_frustum.vertice[6], col );
		debug->DrawLine( m_frustum.vertice[3], m_frustum.vertice[7], col );

		debug->DrawLine( m_frustum.vertice[4], m_frustum.vertice[5], col );
		debug->DrawLine( m_frustum.vertice[5], m_frustum.vertice[6], col );
		debug->DrawLine( m_frustum.vertice[6], m_frustum.vertice[7], col );
		debug->DrawLine( m_frustum.vertice[7], m_frustum.vertice[4], col );
	}
}

void NCamera::Reset()
{
	m_moveSpeed = 3.0f;
	m_rotateSpeed = 3.0f;
	m_moveSpeedDelta = 1.0f;
	m_rotateSpeedDelta = 1.0f;
	m_bChange = false;
}

void NCamera::Release()
{
	m_matCameraWorld.Identity();
	m_matCameraView.Identity();

	m_nearDist = 0.0f;
	m_farDist = 0.0f;
	m_aspect = 0.0f;
	m_fov = 0.0f;
	m_matProjection.Identity();

	m_pDevice = 0;
	m_moveSpeed = 0.0f;
	m_rotateSpeed = 0.0f;
}

void NCamera::SetWorld( const NMATRIX& world )
{
	m_matCameraWorld = world;
	NVECTOR3 eulerDegree = m_matCameraWorld.GetEulerDegree();
	this->m_xAngle = eulerDegree.x;
	this->m_yAngle = eulerDegree.y;

	ResetView();
}

void NCamera::SetView( const NVECTOR3& position, const NVECTOR3& target, const NVECTOR3& up /*= NVECTOR3(0.0f, 1.0f, 0.0f) */ )
{
	m_matCameraWorld.ObjectLookAtLH( position, target, up );
	NVECTOR3 eulerDegree = m_matCameraWorld.GetEulerDegree();
	this->m_xAngle = eulerDegree.x;
	this->m_yAngle = eulerDegree.y;
	
	ResetView();
}

void NCamera::SetFrontDir( const NVECTOR3& dir )
{
	const NVECTOR3 up = m_matCameraWorld.GetRow3(1);
	const NVECTOR3 pos = m_matCameraWorld.GetRow3(3);

	m_matCameraWorld.ObjectLookAtLHByDirN(pos, dir, up);
	NVECTOR3 eulerDegree = m_matCameraWorld.GetEulerDegree();
	this->m_xAngle = eulerDegree.x;
	this->m_yAngle = eulerDegree.y;

	ResetView();
}

void NCamera::SetPosition( const NVECTOR3& position )
{
	m_matCameraWorld.GetRow3(3) = position;

	ResetView();
}

void NCamera::SetTarget( const NVECTOR3& target )
{
	const NVECTOR3 up = m_matCameraWorld.GetRow3(1);
	const NVECTOR3 pos = m_matCameraWorld.GetRow3(3);
	
	SetView(pos, target, up);
}

void NCamera::SetProjMatrix(const NMATRIX& projMat)
{
	m_matProjection = projMat;
	m_matVP = m_matCameraView * m_matProjection;
}

void NCamera::SetProjection( float nearDist, float farDist, float aspect, float fov /*= NL_PI_QUAT */ )
{
	m_nearDist	= nearDist;
	m_farDist	= farDist;
	m_aspect	= aspect;
	m_fov		= fov;
	m_uFactor	= tanf(fov * 0.5f);
	m_rFactor	= tanf(fov * 0.5f * m_aspect);
	ResetProj();
}

void NCamera::SetNearDist( float nearDist )
{
	m_nearDist = nearDist;
	ResetProj();
}

void NCamera::SetFarDist( float farDist )
{
	m_farDist = farDist;
	ResetProj();
}

void NCamera::SetAspect( float aspect )
{
	m_aspect = aspect;
	ResetProj();
}

void NCamera::SetFOV( float fov )
{
	m_fov		= fov;
	m_uFactor	= tanf(fov * 0.5f);
	m_rFactor	= tanf(fov * 0.5f * m_aspect);
	ResetProj();
}

void NCamera::SetMoveSpeed( float speed )
{
	m_moveSpeed = speed;
}

void NCamera::SetRotateSpeed( float speed )
{
	m_rotateSpeed = speed;
}

void NCamera::SetMoveSpeedDelta( float delta )
{
	m_moveSpeedDelta = delta;
}

void NCamera::SetRotateSpeedDelta( float delta )
{
	m_moveSpeedDelta = delta;
}

void NCamera::SetLock(bool bLock)
{
	m_bLock = bLock;
}

void NCamera::IncreaseMoveSpeed()
{
	m_moveSpeed += m_moveSpeedDelta;
}

void NCamera::IncreaseRotateSpeed()
{
	m_rotateSpeed += m_rotateSpeedDelta;
}

void NCamera::DecreaseMoveSpeed()
{
	m_moveSpeed = max( 0.0f, m_moveSpeed - m_moveSpeedDelta );
}

void NCamera::DecreaseRotateSpeed()
{
	m_rotateSpeed = max( 0.0f, m_rotateSpeed - m_rotateSpeedDelta );
}

void NCamera::MoveSide( float delta )
{
	m_matCameraWorld.GetRow3(3) += 
		m_matCameraWorld.GetRow3(0) * delta;
	m_bChange = true;
}

void NCamera::MoveUp( float delta )
{
	m_matCameraWorld.GetRow3(3) += 
		m_matCameraWorld.GetRow3(1) * delta;
	m_bChange = true;
}

void NCamera::MoveForward( float delta )
{
	m_matCameraWorld.GetRow3(3) += 
		m_matCameraWorld.GetRow3(2) * delta;
	m_bChange = true;
}

void NCamera::RotateX( float delta )
{
	static NQUATERNION tmpQ;
	static NMATRIX tmpMat;
	static NVECTOR3 xAxis( 1.0f, 0.0f, 0.0f );
	tmpQ.SetAxisAngle( xAxis, delta );
	tmpQ.GetMatrix( tmpMat );
	m_matCameraWorld.Multiply33WithMe( tmpMat );
	m_bChange = true;
}

void NCamera::RotateY( float delta )
{
	static NQUATERNION tmpQ;
	static NMATRIX tmpMat;
	static NVECTOR3 yAxis( 0.0f, 1.0f, 0.0f );
	tmpQ.SetAxisAngle( yAxis, delta );
	tmpQ.GetMatrix( tmpMat );
	m_matCameraWorld.Multiply33WithMe( tmpMat );
	m_bChange = true;
}

void NCamera::RotateZ( float delta )
{
	static NQUATERNION tmpQ;
	static NMATRIX tmpMat;
	static NVECTOR3 zAxis( 0.0f, 0.0f, 1.0f );
	tmpQ.SetAxisAngle( zAxis, delta );
	tmpQ.GetMatrix( tmpMat );
	m_matCameraWorld.Multiply33WithMe( tmpMat );
	m_bChange = true;
}

void NCamera::RotateXY( float xDelta, float yDelta )
{
	static NQUATERNION qR;
	static NMATRIX tmpMat;
	static NVECTOR3 tmpPos;

	m_xAngle += xDelta;
	m_yAngle += yDelta;

	tmpPos = m_matCameraWorld.GetRow3(3);
	D3DXQuaternionRotationYawPitchRoll( qR, 
		DEG_TO_RAD(m_yAngle), 
		DEG_TO_RAD(m_xAngle), 
		0.0f );
	D3DXMatrixAffineTransformation( m_matCameraWorld, 1.0f, NULL, qR, tmpPos );

	m_bChange = true;
}

void NCamera::ResetView()
{
	m_matCameraWorld.InverseOrtho( m_matCameraView );
	m_matVP = m_matCameraView * m_matProjection;
}

void NCamera::ResetProj()
{
	m_matProjection.SetPerspectiveFovLH( m_fov, m_aspect, m_nearDist, m_farDist );
	m_matVP = m_matCameraView * m_matProjection;
}

void NCamera::BuildFrustum()
{
	// GPG 5
	static NVECTOR3 rightNearVec, rightFarVec;
	static NVECTOR3 upNearVec, upFarVec;
	static NVECTOR3 frontNearVec, frontFarVec;
	
	rightNearVec = m_rFactor * m_nearDist * m_matCameraWorld.GetRow3(0);
	rightFarVec  = m_rFactor * m_farDist  * m_matCameraWorld.GetRow3(0);
	upNearVec	 = m_uFactor * m_nearDist * m_matCameraWorld.GetRow3(1);
	upFarVec	 = m_uFactor * m_farDist  * m_matCameraWorld.GetRow3(1);
	frontNearVec = m_matCameraWorld.GetRow3(3) + m_matCameraWorld.GetRow3(2) * m_nearDist;
	frontFarVec  = m_matCameraWorld.GetRow3(3) + m_matCameraWorld.GetRow3(2) * m_farDist;
	
	// 원 좌 위 
	m_frustum.vertice[0] = frontFarVec - rightFarVec + upFarVec;
	
	// 원 우 위 
	m_frustum.vertice[1] = frontFarVec + rightFarVec + upFarVec;
		
	// 근 우 위 
	m_frustum.vertice[2] = frontNearVec + rightNearVec + upNearVec;
		
	// 근 좌 위 
	m_frustum.vertice[3] = frontNearVec - rightNearVec + upNearVec;
		
	// 원 좌 아래 
	m_frustum.vertice[4] = frontFarVec - rightFarVec - upFarVec;
		
	// 원 우 아래 
	m_frustum.vertice[5] = frontFarVec + rightFarVec - upFarVec;
		
	// 근 우 아래 
	m_frustum.vertice[6] = frontNearVec + rightNearVec - upNearVec;
		
	// 근 좌 아래 
	m_frustum.vertice[7] = frontNearVec - rightNearVec - upNearVec;

	//m_frustum.bBuild = true;
}

bool NCamera::IsInPoint(const NVECTOR3& pt) const
{
	static NVECTOR3 OP;
	static float limit;
	static float frontDot, chk;
	OP = pt - m_matCameraWorld.GetRow3(3);

	// 1. 근단면 사이 판정
	//
	// 정면 위치 = OP 내적 LookVector
	//
	frontDot = OP.Dot( m_matCameraWorld.GetRow3(2) );
	if( frontDot < m_nearDist || frontDot > m_farDist )
		return false;

	// 2. 위아래 사이 판정
	//
	// 높이 위치 = OP 내적 UpVector
	//
	chk = OP.Dot( m_matCameraWorld.GetRow3(1) );
	limit = m_uFactor * frontDot;
	if( chk < -limit || chk > limit )
		return false;

	// 3. 좌우 사이 판정
	//
	// 좌우 위치 = OP 내적 RightVector
	//
	chk = OP.Dot( m_matCameraWorld.GetRow3(0) );
	limit = m_rFactor * frontDot;
	if( chk < -limit || chk > limit )
		return false;

	return true;
}

bool NCamera::IsInSphere(const NSphere& sphere) const
{
	static NVECTOR3 OP;
	static float limit;
	static float frontDot, chk;
	OP = sphere.centerPoint - m_matCameraWorld.GetRow3(3);

	// 1. 근단면 사이 판정
	//
	// 정면 위치 = OP 내적 LookVector
	//
	frontDot = OP.Dot( m_matCameraWorld.GetRow3(2) );
	if( frontDot < m_nearDist - sphere.radius ||
		frontDot > m_farDist + sphere.radius )
		return false;

	// 2. 위아래 사이 판정
	//
	// 높이 위치 = OP 내적 UpVector
	//
	chk = OP.Dot( m_matCameraWorld.GetRow3(1) );
	limit = m_uFactor * frontDot + sphere.radius;
	if( chk < -limit || chk > limit )
		return false;

	// 3. 좌우 사이 판정
	//
	// 좌우 위치 = OP 내적 RightVector
	//
	chk = OP.Dot( m_matCameraWorld.GetRow3(0) );
	limit = m_rFactor * frontDot + sphere.radius;
	if( chk < -limit || chk > limit )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////

#include "NL_FreeCamera.h"

void NCameraPool::Init()
{
	Reset();
}

int NCameraPool::Add( NCamera* pCamera )
{
	m_cameraVec.push_back(pCamera);
	return int(m_cameraVec.size() - 1);
}

void NCameraPool::Reset()
{
	Release();
	NFreeCamera* pDebugCamera = new NFreeCamera;
	pDebugCamera->Init();
	pDebugCamera->SetView( NVECTOR3(100.0f, 100.0f, -100.0f), NVECTOR3(0.0f, 0.0f, 0.0f) );
	pDebugCamera->SetProjection( 1.0f, 1000.0f, 1.333333f );
	SelectCamera( Add(pDebugCamera) );
}

void NCameraPool::Release()
{
	SAFE_STL_VECTOR_DELETE_AFTER_RELEASE(m_cameraVec);
}