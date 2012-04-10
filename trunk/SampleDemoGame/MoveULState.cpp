#include "MoveULState.h"
#include "Character.h"

CMoveULState::CMoveULState( CCharacter* parent ) : CCharacterState(parent)
{
}

CMoveULState::~CMoveULState(void)
{
}

void CMoveULState::Process()
{
	//왼쪽 위로 움직인다.
	m_parentObj->MoveTo( -MOVE_SPEED, -MOVE_SPEED );
}
