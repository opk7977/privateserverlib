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
	//�������� �����δ�.
	m_parentObj->MoveTo( -MOVE_SPEED, 0 );
}
