#pragma once

#include "NL_Base.h"
#include "NL_Math.h"
#include "NL_Singleton.h"

//=========================================================================
//	Name   : NCamera
//	Content: 카메라 클래스
//=========================================================================
class ND3d;
class NTime;
class NInput;
class NCamera
{
protected:
	//==================================
	//	Protected Data
	//==================================

	// View Data
	NMATRIX		m_matCameraWorld;
	NMATRIX		m_matCameraView;

	// Projection data
	float		m_nearDist;
	float		m_farDist;
	float		m_aspect;
	float		m_fov;
	NMATRIX		m_matProjection;

	// Camera Data
	ND3d*		m_pDevice;
	NInput*		m_pInput;
	NTime*		m_pTime;
	float		m_xAngle;
	float		m_yAngle;
	float		m_moveSpeed;
	float		m_rotateSpeed;
	float		m_moveSpeedDelta;
	float		m_rotateSpeedDelta;
	float		m_rFactor;
	float		m_uFactor;
	bool		m_bChange;
	NMATRIX		m_matVP;
	bool		m_bLock;

	// Frustum Data
	bool		m_bUseFrustum;
	NFRUSTUM	m_frustum;
	NSphere		m_frustumSphere;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NCamera();
	virtual ~NCamera();

	//==================================
	//	External Function
	//==================================
	virtual void Init();
	virtual void Frame();
	virtual void Render();
	virtual void Reset();
	virtual void Release();

	//==================================
	//	Setter
	//==================================
	void SetWorld( const NMATRIX& world );
	void SetView(	const NVECTOR3& position,
					const NVECTOR3& target = NVECTOR3(0.0f, 0.0f, 0.0f),
					const NVECTOR3& up = NVECTOR3(0.0f, 1.0f, 0.0f) );
	void SetFrontDir(const NVECTOR3& dir);
	void SetPosition(const NVECTOR3& position);
	void SetTarget(const NVECTOR3& target);
	void SetProjMatrix(const NMATRIX& projMat);
	void SetProjection( float nearDist, 
						float farDist, 
						float aspect, 
						float fov = NL_PI_QUAT );
	void SetNearDist( float nearDist );
	void SetFarDist( float farDist );
	void SetAspect( float aspect);
	void SetFOV( float fov );
	void SetMoveSpeed( float speed );
	void SetRotateSpeed( float speed );
	void SetMoveSpeedDelta( float delta );
	void SetRotateSpeedDelta( float delta );
	void SetLock(bool bLock);

	//==================================
	//	Getter
	//==================================
	const NMATRIX&	GetWorld()		const	{	return m_matCameraWorld;				}
	const NMATRIX&	GetView()		const	{	return m_matCameraView;					}
	const NMATRIX&	GetProj()		const	{	return m_matProjection;					}
	const NMATRIX&	GetViewProj()	const	{	return m_matVP;							}
	const NVECTOR3& GetRightDir()	const	{	return m_matCameraWorld.GetRow3(0);		}
	const NVECTOR3& GetUpDir()		const	{	return m_matCameraWorld.GetRow3(1);		}
	const NVECTOR3& GetFrontDir()	const	{	return m_matCameraWorld.GetRow3(2);		}
	const NVECTOR3& GetPosition()	const	{	return m_matCameraWorld.GetRow3(3);		}

	float GetNearDist()			const	{	return m_nearDist;			}
	float GetFarDist()			const	{	return m_farDist;			}
	float GetAspect()			const	{	return m_aspect;			}
	float GetFOV()				const	{	return m_fov;				}
	float GetMoveSpeed()		const	{	return m_moveSpeed;			}
	float GetRotateSpeed()		const	{	return m_rotateSpeed;		}
	float GetMoveSpeedDelta()	const	{	return m_moveSpeedDelta;	}
	float GetRotateSpeedDelta()	const	{	return m_rotateSpeedDelta;	}
	NFRUSTUM& GetFrustum()				{	return m_frustum;			}
	bool  IsChanged()			const	{	return m_bChange;			}
	bool  IsLocked()			const	{	return m_bLock;				}

	//==================================
	//	Camera Function
	//==================================
	void IncreaseMoveSpeed();
	void IncreaseRotateSpeed();
	void DecreaseMoveSpeed();
	void DecreaseRotateSpeed();

	void MoveSide( float delta );
	void MoveUp( float delta );
	void MoveForward( float delta );

	void RotateX( float delta );
	void RotateY( float delta );
	void RotateZ( float delta );
	void RotateXY( float xDelta, float yDelta );

	void ResetView();
	void ResetProj();

	void BuildFrustum();
	bool IsInPoint(const NVECTOR3& pt) const;
	bool IsInSphere(const NSphere& sphere) const;
	void RenderFrustum();

private:
	//==================================
	//	Internal Function
	//==================================

};
typedef std::vector<NCamera*> NCameraVec;

//=========================================================================
//	Name   : NCameraPool
//	Content: 카메라 매니져 클래스
//=========================================================================
class NCameraPool : public NSingleton< NCameraPool >
{
	DECL_FRIEND( NSingleton< NCameraPool > )
	//==================================
	//	Private Data
	//==================================
	int			m_curCamera;
	NCameraVec	m_cameraVec;
	
	//==================================
	//	Ctor & Dtor
	//==================================
	NCameraPool():m_curCamera(-1){}
public:
	~NCameraPool(){}

	//==================================
	//	External Function
	//==================================
	void		Init();
	int			Add(NCamera* pCamera);
	NL_INLINE
	int			GetCameraCount() const	{	return (int)m_cameraVec.size();		}
	NL_INLINE
	NCamera*	GetCamera(int index)const{	return m_cameraVec[index];			}
	NL_INLINE
	NCamera*	GetCurCamera()			{	return m_cameraVec[m_curCamera];							}
	NL_INLINE
	NCamera*	SelectCamera(int index)	{	m_curCamera = index;	return m_cameraVec[m_curCamera];	}
	void		Reset();
	void		Release();
	
private:
	//==================================
	//	Internal Function
	//==================================
	
};

#define I_NCamera NCameraPool::GetInstance()