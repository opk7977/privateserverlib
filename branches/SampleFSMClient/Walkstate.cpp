#include "Walkstate.h"
#include "Character.h"

Walkstate::Walkstate( Character* parent ) : CharacterState( parent )
{
	//캐릭터에 목적지를 랜덤으로 설정해 준다.
}

Walkstate::~Walkstate(void)
{
}

void Walkstate::Process()
{
	//목적지에 도착했다면 다시 목적지를 정하고 그렇지 않으면 계속 가자
	if( m_parentObj->IsGoal() )
	{
		//도착했다면 멈춘다
		srand((unsigned int)time(NULL));
		//m_parentObj->SetGoal( rand()%800, rand()%600 );
		m_parentObj->SetTransition( Character::EVENT_STOPWALK );

		return;
	}
	else
		m_parentObj->MoveTo();	//도착하지 못했다면 계속 간다

	
}
