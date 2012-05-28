#pragma once

#include "NObject.h"
#include "NObjDefine.h"

enum CH_ACTION
{
	CA_IDLE,
	CA_WALK,
	CA_RUN,

	CA_MAX
};


//=========================================================================
//	Name   : NCharObject
//	Content: 캐릭터 오브젝트
//=========================================================================
class NCharObject : public NObject
{
	//==================================
	//	Private Data
	//==================================
	int			m_curFrameAction;
	int			m_prevFrameAction;
	int			m_direction;

	typedef void (NCharObject::*FRAME_FUNC)();
	static FRAME_FUNC	m_curFrameFunc[CA_MAX];

	typedef void (NCharObject::*FUNC_MOVE)(float);
	static FUNC_MOVE	m_moveFunc[ MD_MAX ];

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NCharObject();
	~NCharObject();

	//==================================
	//	External Function
	//==================================
	void Init();
	void Frame();
	void Render();
	bool LoadSet( const TCHAR* charSet, NAniMeshObj** pOutAniObj = NULL );
	void SetMoveDir(int dir)	{	m_direction = dir;			}
	int  GetMoveDir()	const	{	return m_direction;			}
	int  GetState()		const	{	return m_curFrameAction;	}
	void ChangeState(int actIndex);

private:
	void FrameIdle();
	void FrameWalk();
	void FrameRun();

	void MoveFrame(int dir, float delta);
	void MoveForward2D(float delta);
	void MoveBackward2D(float delta);
	void MoveLeft(float delta);
	void MoveRight(float delta);
	void MoveForwardLeft2D(float delta);
	void MoveForwardRight2D(float delta);
	void MoveBackwardLeft2D(float delta);
	void MoveBackwardRight2D(float delta);
};