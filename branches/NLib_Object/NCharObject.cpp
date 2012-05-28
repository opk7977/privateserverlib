#include "NCharObject.h"
#include "NL_Time.h"

NCharObject::FRAME_FUNC NCharObject::m_curFrameFunc[CA_MAX] =
{
	&NCharObject::FrameIdle,
	&NCharObject::FrameWalk,
	&NCharObject::FrameRun
};

NCharObject::FUNC_MOVE NCharObject::m_moveFunc[ MD_MAX ] =
{
	&NCharObject::MoveForward2D,
	&NCharObject::MoveBackward2D,
	&NCharObject::MoveLeft,
	&NCharObject::MoveRight,
	&NCharObject::MoveForwardLeft2D,
	&NCharObject::MoveForwardRight2D,
	&NCharObject::MoveBackwardLeft2D,
	&NCharObject::MoveBackwardRight2D
};

//////////////////////////////////////////////////////////////////////////

NCharObject::NCharObject(void)
{

}

NCharObject::~NCharObject(void)
{

}

void NCharObject::Init()
{
	NObject::Init();
}

void NCharObject::Frame()
{
	(this->*m_curFrameFunc[ m_curFrameAction ])();
}

bool NCharObject::LoadSet(const TCHAR* charSet, NAniMeshObj** pOutAniObj /*=NULL*/)
{
	bool ret = NObject::LoadSet(charSet, pOutAniObj);
	if( ret )
	{
		m_prevFrameAction = CA_IDLE;
		m_curFrameAction = CA_IDLE;

		this->m_actionFrame[CA_IDLE		].loop = TRUE;
		this->m_actionFrame[CA_WALK		].loop = TRUE;
		this->m_actionFrame[CA_RUN		].loop = TRUE;

		this->SetActionAll( m_curFrameAction );
	}

	return ret;
}

void NCharObject::FrameIdle()
{
	
}

void NCharObject::FrameWalk()
{
	(this->*m_moveFunc[m_direction])(  ( m_pTime->GetElapsedTime() + 0.1f )  );
}

void NCharObject::FrameRun()
{
	(this->*m_moveFunc[m_direction])(  2.0f * ( m_pTime->GetElapsedTime() + 0.1f )  );
}

void NCharObject::Render()
{
	if(m_curFrameAction == CA_IDLE)
		this->RenderStatic();
	else
		this->Render();
}

//////////////////////////////////////////////////////////////////////////

void NCharObject::MoveForward2D(float delta)
{
	m_controlWorld._41 += m_controlWorld._31 * delta;
	m_controlWorld._43 += m_controlWorld._33 * delta;
}

void NCharObject::MoveBackward2D(float delta)
{
	m_controlWorld._41 += m_controlWorld._31 * delta;
	m_controlWorld._43 += m_controlWorld._33 * delta;
}

void NCharObject::MoveLeft(float delta)
{
	m_controlWorld._41 += -m_controlWorld._11 * delta;
	m_controlWorld._43 += -m_controlWorld._13 * delta;
}

void NCharObject::MoveRight(float delta)
{
	m_controlWorld._41 += m_controlWorld._11 * delta;
	m_controlWorld._43 += m_controlWorld._13 * delta; 
}

void NCharObject::MoveForwardLeft2D(float delta)
{
	m_controlWorld._41 += (m_controlWorld._31 - m_controlWorld._11) * delta;
	m_controlWorld._43 += (m_controlWorld._33 - m_controlWorld._13) * delta;
}

void NCharObject::MoveForwardRight2D(float delta)
{
	m_controlWorld._41 += (m_controlWorld._31 + m_controlWorld._11) * delta;
	m_controlWorld._43 += (m_controlWorld._33 + m_controlWorld._13) * delta;
}

void NCharObject::MoveBackwardLeft2D(float delta)
{
	m_controlWorld._41 += -(m_controlWorld._31 + m_controlWorld._11) * delta;
	m_controlWorld._43 += -(m_controlWorld._33 + m_controlWorld._13) * delta;
}

void NCharObject::MoveBackwardRight2D(float delta)
{
	m_controlWorld._41 += (m_controlWorld._11 - m_controlWorld._31) * delta;
	m_controlWorld._43 += (m_controlWorld._13 - m_controlWorld._33) * delta;
}

//////////////////////////////////////////////////////////////////////////

void NCharObject::ChangeState(int actIndex)
{
	m_prevFrameAction = m_curFrameAction;
	m_curFrameAction = actIndex;

	if(actIndex == CA_IDLE)
		this->SetCurFrameAll(0); 
}