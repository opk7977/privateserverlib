#include "StandState.h"
#include "Character.h"

StandState::StandState( Character* parent ) : CharacterState( parent )
{
}

StandState::~StandState(void)
{
}

void StandState::Process()
{
	//5�ۼ�Ʈ(?)�� Ȯ���� �ٽ� �����̱� �����Ѵ�.
// 	if( rand() % 100 < 5 )
// 	{
// 		m_parentObj->SetTransition( Character::EVENT_STARTWALK );
// 	}
}
