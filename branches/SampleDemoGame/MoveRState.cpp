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
	//���������� �����δ�.
	m_parentObj->MoveTo( MOVE_SPEED, 0 );
}
