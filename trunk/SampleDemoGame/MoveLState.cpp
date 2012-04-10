#include "MoveLState.h"
#include "Character.h"

CMoveLState::CMoveLState( CCharacter* parent ) : CCharacterState(parent)
{
}

CMoveLState::~CMoveLState(void)
{
}

void CMoveLState::Process()
{
	//왼쪽으로 움직인다.
	m_parentObj->MoveTo( -MOVE_SPEED, 0 );
}
