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
	//���� ���� �����δ�.
	m_parentObj->MoveTo( -MOVE_SPEED, -MOVE_SPEED );
}
