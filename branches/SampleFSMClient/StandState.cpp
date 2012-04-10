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
	//5퍼센트(?)의 확률로 다시 움직이기 시작한다.
// 	if( rand() % 100 < 5 )
// 	{
// 		m_parentObj->SetTransition( Character::EVENT_STARTWALK );
// 	}
}
