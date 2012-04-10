#include "CharacterMgr.h"
#include "time.h"

CharacterMgr::CharacterMgr(void)
{
}

CharacterMgr::~CharacterMgr(void)
{
	//���� �������ְ� �����ش�
	std::map< DWORD, Character* >::iterator iter, iterPrev;

	iter = m_mapCharacter.begin();
	while( iter != m_mapCharacter.end() )
	{
		iterPrev = iter++;
		delete iterPrev->second;
		m_mapCharacter.erase( iterPrev );
	}
	m_mapCharacter.clear();
}

void CharacterMgr::CreateCharacter( int count )
{
	FiniteStateMachine* fsm;

	m_charCount = count;

	srand( (unsigned int)time(NULL) );

	//ĳ���Ϳ� fsm�� ���Խ��Ѽ� �����Ѵ�.
	for( int i=0; i<m_charCount; ++i )
	{
		fsm = new FiniteStateMachine();

		//���¿� �̺�Ʈ�� �´� ���¸� �����ؼ� ������ �ش�.
		fsm->AddStateTransition( Character::STATE_STAND, Character::EVENT_STARTWALK, Character::STATE_MOVE );
		fsm->AddStateTransition( Character::STATE_MOVE, Character::EVENT_STOPWALK, Character::STATE_STAND );

		//������ ����(�⺻)�� ����
		fsm->SetCurrentState( Character::STATE_STAND );

		//ĳ���� ����/////////////////////////////////////////////////////////////
		Character* tmpCharacter = new Character( fsm, rand()%3+2 );

		tmpCharacter->SetPosition( rand()%800, rand()%600 );
		tmpCharacter->SetGoal( rand()%800, rand()%600 );
		tmpCharacter->SetId(i);
		//////////////////////////////////////////////////////////////////////////

		//map�� character���
		m_mapCharacter[i] = tmpCharacter;
	}
}

void CharacterMgr::MoveCharacter()
{
	std::map< DWORD, Character* >::iterator iter;
	
	iter = m_mapCharacter.begin();
	for( ; iter != m_mapCharacter.end(); iter++ )
	{
		(iter->second)->Process();
	}
}

void CharacterMgr::RenderAll( HDC dc )
{
	std::map< DWORD, Character* >::iterator iter;

	iter = m_mapCharacter.begin();
	for( ; iter != m_mapCharacter.end(); iter++ )
	{
		(iter->second)->Render( dc );
	}
}

Character* CharacterMgr::FindChar( int _index )
{
	//�켱 ã�� ���ô�
	std::map< DWORD, Character* >::iterator iter;
	Character* tmp;

	iter = m_mapCharacter.find( _index );
	if( iter == m_mapCharacter.end() )
		return 0;

	tmp = m_mapCharacter[_index];

	return tmp;
}
