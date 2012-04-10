#include "MoveRUState.h"
#include "Character.h"

CMoveRUState::CMoveRUState(CCharacter* parent) : CCharacterState(parent)
{
}

CMoveRUState::~CMoveRUState(void)
{
}

void CMoveRUState::Process()
{
	//오른쪽 위로 움직인다.
	m_parentObj->MoveTo( MOVE_SPEED, -MOVE_SPEED );
}
