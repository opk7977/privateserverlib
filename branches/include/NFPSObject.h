#pragma once

#include "NObject.h"

enum FPS_ACTION
{
	FA_IDLE,
	FA_WALK,
	FA_RUN,
	FA_FIRE,
	FA_RELOAD,

	FA_MAX
};

//=========================================================================
//	Name   : NFPSObject
//	Content: 1인칭 오브젝트
//=========================================================================
class NInput;
class NFPSCamera;
class NFPSObject : public NObject
{
	//==================================
	//	Private Data
	//==================================
	NVECTOR3	m_pivot;
	NFPSCamera*	m_pCamera;
	NInput*		m_pInput;

	int			m_curAction;
	int			m_prevAction;
	int			m_direction;
	float		m_delta;

	typedef void (NFPSObject::*FRAME_FUNC)();
	static FRAME_FUNC	m_curFrameFunc[FA_MAX];
	static FRAME_FUNC	m_curInputFunc[FA_MAX];

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NFPSObject();
	~NFPSObject();

	

	//==================================
	//	External Function
	//==================================
	void Init();
	void Frame();
	void FrameMove();
	bool LoadSet( const TCHAR* charSet, NAniMeshObj** pOutAniObj = NULL );
	void Update();
	void ProcessInput();
	void SetPivot(const NVECTOR3& pivot)	{	m_pivot = pivot;		}
	void SetFPSCamera(NFPSCamera* pCamera)	{	m_pCamera = pCamera;	}
	void SetPosition(const NVECTOR3& pos);
	void SetPosition(float x, float y, float z);
	const NVECTOR3 GetPosition() const;

	float GetPivotX()	const	{	return m_pivot.x;	}
	float GetPivotY()	const	{	return m_pivot.y;	}
	float GetPivotZ()	const	{	return m_pivot.z;	}

	void ChangeState(int actIndex);

private:
	void InputMove();
	void InputOnIdle();
	void InputOnWalk();
	void InputOnRun();
	void InputOnFire();
	void InputOnReload();

	void FrameRotate();
	void FrameIdle();
	void FrameWalk();
	void FrameRun();
	void FrameFire();
	void FrameReload();
};