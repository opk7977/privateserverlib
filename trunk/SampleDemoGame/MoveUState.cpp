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
	//���θ� �����δ�.
	m_parentObj->MoveTo( 0, -MOVE_SPEED );
}
