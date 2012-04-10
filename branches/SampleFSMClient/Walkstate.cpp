#include "Walkstate.h"
#include "Character.h"

Walkstate::Walkstate( Character* parent ) : CharacterState( parent )
{
	//ĳ���Ϳ� �������� �������� ������ �ش�.
}

Walkstate::~Walkstate(void)
{
}

void Walkstate::Process()
{
	//�������� �����ߴٸ� �ٽ� �������� ���ϰ� �׷��� ������ ��� ����
	if( m_parentObj->IsGoal() )
	{
		//�����ߴٸ� �����
		srand((unsigned int)time(NULL));
		//m_parentObj->SetGoal( rand()%800, rand()%600 );
		m_parentObj->SetTransition( Character::EVENT_STOPWALK );

		return;
	}
	else
		m_parentObj->MoveTo();	//�������� ���ߴٸ� ��� ����

	
}
