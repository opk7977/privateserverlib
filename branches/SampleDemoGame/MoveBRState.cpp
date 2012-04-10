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
	//오른쪽 아래로 움직인다.
	m_parentObj->MoveTo( MOVE_SPEED, MOVE_SPEED );
}
