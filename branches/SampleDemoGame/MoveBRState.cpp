#include "StdAfx.h"
#include "MoveBRState.h"
#include "Character.h"

CMoveBRState::CMoveBRState( CCharacter* parent ) : CCharacterState(parent)
{
}

CMoveBRState::~CMoveBRState(void)
{
}

void CMoveBRState::Process()
{
	//������ �Ʒ��� �����δ�.
	m_parentObj->MoveTo( MOVE_SPEED, MOVE_SPEED );
}
