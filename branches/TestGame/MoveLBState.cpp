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
	//���� �Ʒ��� �����δ�.
	m_parentObj->MoveTo( -MOVE_SPEED, MOVE_SPEED, 0 );
}
