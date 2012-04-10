#include "StdAfx.h"
#include "MoveRState.h"
#include "Character.h"

CMoveRState::CMoveRState(CCharacter* parent) : CCharacterState(parent)
{
}

CMoveRState::~CMoveRState(void)
{
}

void CMoveRState::Process()
{
	//오른쪽으로 움직인다.
	m_parentObj->MoveTo( MOVE_SPEED, 0 );
}
