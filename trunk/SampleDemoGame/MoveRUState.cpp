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
	//������ ���� �����δ�.
	m_parentObj->MoveTo( MOVE_SPEED, -MOVE_SPEED );
}
