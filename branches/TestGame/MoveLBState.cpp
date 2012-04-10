#include "MoveLBState.h"
#include "Character.h"

CMoveLBState::CMoveLBState( CCharacter* parent ) : CCharacterState(parent)
{
}

CMoveLBState::~CMoveLBState(void)
{
}

void CMoveLBState::Process()
{
	//왼쪽 아래로 움직인다.
	m_parentObj->MoveTo( -MOVE_SPEED, MOVE_SPEED, 0 );
}
