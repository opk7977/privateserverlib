#include "MoveBState.h"
#include "Character.h"

CMoveBState::CMoveBState( CCharacter* parent ) : CCharacterState(parent)
{
}

CMoveBState::~CMoveBState(void)
{
}

void CMoveBState::Process()
{
	//�Ʒ��� �����δ�.
	m_parentObj->MoveTo( 0, MOVE_SPEED, 0 );
}
