#include "MoveUState.h"
#include "Character.h"

CMoveUState::CMoveUState( CCharacter* parent ) : CCharacterState(parent)
{
}

CMoveUState::~CMoveUState(void)
{
}

void CMoveUState::Process()
{
	//위로만 움직인다.
	m_parentObj->MoveTo( 0, -MOVE_SPEED );
}
