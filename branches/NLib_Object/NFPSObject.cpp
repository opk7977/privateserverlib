#include "NFPSObject.h"
#include "NFPSCamera.h"
#include "NL_Time.h"
#include "NL_Input.h"


NFPSObject::FRAME_FUNC NFPSObject::m_curFrameFunc[FA_MAX] =
{
	&NFPSObject::FrameIdle,
	&NFPSObject::FrameWalk,
	&NFPSObject::FrameRun,
	&NFPSObject::FrameFire,
	&NFPSObject::FrameReload
};

NFPSObject::FRAME_FUNC NFPSObject::m_curInputFunc[FA_MAX] =
{
	&NFPSObject::InputOnIdle,
	&NFPSObject::InputOnWalk,
	&NFPSObject::InputOnRun,
	&NFPSObject::InputOnFire,
	&NFPSObject::InputOnReload
};

//////////////////////////////////////////////////////////////////////////

NFPSObject::NFPSObject(void)
{

}

NFPSObject::~NFPSObject(void)
{

}

void NFPSObject::Init()
{
	NObject::Init();
	m_pInput = I_NInput;
}

void NFPSObject::Frame()
{
	ProcessInput();
	(this->*m_curFrameFunc[ m_curAction ])();
	FrameMove();
	Update();
}

bool NFPSObject::LoadSet(const TCHAR* charSet, NAniMeshObj** pOutAniObj /*=NULL*/)
{
	bool ret = NObject::LoadSet(charSet, pOutAniObj);
	if( ret )
	{
		m_prevAction = FA_IDLE;
		m_curAction = FA_WALK;
		m_direction = -1;
		m_delta = 1.0f;

		this->m_actionFrame[FA_IDLE		].loop = TRUE;
		this->m_actionFrame[FA_WALK		].loop = TRUE;
		this->m_actionFrame[FA_RUN		].loop = TRUE;
		this->m_actionFrame[FA_FIRE		].loop = FALSE;
		this->m_actionFrame[FA_RELOAD	].loop = FALSE;

		this->SetActionAll( m_curAction );
	}

	return ret;
}

void NFPSObject::FrameMove()
{
	if(m_direction >= 0)
		m_pCamera->MoveFrame(m_direction, m_delta * (m_pTime->GetElapsedTime() + 0.1f));
}

void NFPSObject::FrameIdle()
{
	
}

void NFPSObject::FrameWalk()
{
	
}

void NFPSObject::FrameRun()
{
	
}

void NFPSObject::FrameFire()
{
	NFrameInfo& curFrm = this->m_skinObjVec[0].m_frame;
	if( curFrm.curFrameI >= curFrm.lastFrame )
	{
		ChangeState( FA_IDLE );
		return;
	}
}

void NFPSObject::FrameReload()
{
	NFrameInfo& curFrm = this->m_skinObjVec[0].m_frame;
	if( curFrm.curFrameI >= curFrm.lastFrame )
	{
		ChangeState( FA_IDLE );
		return;
	}
}

void NFPSObject::Update()
{
	if( m_pCamera->IsChanged() )
	{
		m_pCamera->UpdateView();

		const NMATRIX& curMat = m_pCamera->GetWorld();
		m_controlWorld.GetRow3(0) = -curMat.GetRow3(0);
		m_controlWorld.GetRow3(1) =  curMat.GetRow3(1);
		m_controlWorld.GetRow3(2) = -curMat.GetRow3(2);
		m_controlWorld.GetRow3(3) =  curMat.GetRow3(3);

		m_controlWorld.GetRow3(3) -= curMat.GetRow3(0) * m_pivot.x;
		m_controlWorld.GetRow3(3) -= curMat.GetRow3(1) * m_pivot.y;
		m_controlWorld.GetRow3(3) -= curMat.GetRow3(2) * m_pivot.z;
	}
}

//////////////////////////////////////////////////////////////////////////

void NFPSObject::ProcessInput()
{
	m_pCamera->RotateXY();
	InputMove();
	(this->*m_curInputFunc[ m_curAction ])();
}

void NFPSObject::InputMove()
{
	static int flag;

	flag = 0;
	if		( m_pInput->IsKeydown('W') )	flag |= 0x4;
	else if	( m_pInput->IsKeydown('S') )	flag |= 0x2;
	if		( m_pInput->IsKeydown('D') )	flag |= 0x1;
	else if	( m_pInput->IsKeydown('A') )	flag |= 0x8;

	if(flag)
	{
		switch( flag )
		{
		case 0x4:	m_direction = MD_FORWARD;			break;	// »ó
		case 0x2:	m_direction = MD_BACKWARD;			break;	// ÇÏ
		case 0x8:	m_direction = MD_LEFT;				break;	// ÁÂ
		case 0x1:	m_direction = MD_RIGHT;				break;	// ¿ì
		case 0xc:	m_direction = MD_FORWARD_LEFT;		break;	// ÁÂ»ó
		case 0x5:	m_direction = MD_FORWARD_RIGHT;		break;	// ¿ì»ó
		case 0xa:	m_direction = MD_BACKWARD_LEFT;		break;	// ÁÂÇÏ
		case 0x3:	m_direction = MD_BACKWARD_RIGHT;	break;	// ¿ìÇÏ
		}

		if( m_pInput->IsKeydown(VK_SHIFT) )
			m_delta = 2.0f;
		else
			m_delta = 1.0f;

		if( m_curAction == FA_IDLE )
		{
			if( m_pInput->IsKeydown(VK_SHIFT) )
				ChangeState(FA_RUN);
			else
				ChangeState(FA_WALK);
		}
	}
	else
	{
		m_direction = -1;
	}
}

void NFPSObject::InputOnIdle()
{
	if( m_pInput->IsKeydown('R') )
	{
		ChangeState( FA_RELOAD );
		return;
	}

	if( m_pInput->IsKeydownFirst(VK_LBUTTON) )
	{
		ChangeState( FA_FIRE );
		return;
	}
}

void NFPSObject::InputOnWalk()
{
	if( m_pInput->IsKeyup('W') &&
		m_pInput->IsKeyup('S') &&
		m_pInput->IsKeyup('A') &&
		m_pInput->IsKeyup('D') )
	{
		ChangeState( FA_IDLE );
		return;
	}

	if( m_pInput->IsKeydown( VK_SHIFT ) )
	{
		m_delta = 2.0f;
		ChangeState( FA_RUN );
		return;
	}
}

void NFPSObject::InputOnRun()
{
	if( m_pInput->IsKeyup('W') &&
		m_pInput->IsKeyup('S') &&
		m_pInput->IsKeyup('A') &&
		m_pInput->IsKeyup('D') )
	{
		ChangeState( FA_IDLE );
		return;
	}

	if( m_pInput->IsKeyup( VK_SHIFT ) )
	{
		m_delta = 1.0f;
		ChangeState( FA_WALK );
		return;
	}
}

void NFPSObject::InputOnFire()
{
	if( m_pInput->IsKeydownFirst(VK_LBUTTON) )
	{
		ChangeState( FA_FIRE );
		return;
	}

	if( m_pInput->IsKeydown('R') )
	{
		ChangeState( FA_RELOAD );
		return;
	}
}

void NFPSObject::InputOnReload()
{
	
}

//////////////////////////////////////////////////////////////////////////

void NFPSObject::ChangeState(int actIndex)
{
	m_prevAction = m_curAction;
	m_curAction = actIndex;

	this->SetActionAll( m_curAction );
}

void NFPSObject::SetPosition(const NVECTOR3& pos)
{
	m_pCamera->SetPosition( pos );
}

void NFPSObject::SetPosition(float x, float y, float z)
{
	m_pCamera->SetPosition( x, y, z );
}

const NVECTOR3 NFPSObject::GetPosition() const
{
	return m_pCamera->GetPosition();
}
