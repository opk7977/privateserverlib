#include "CharacterMgr.h"
#include "time.h"

CharacterMgr::CharacterMgr(void)
{
}

CharacterMgr::~CharacterMgr(void)
{
	//전부 해제해주고 지워준다
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

	//캐릭터에 fsm을 포함시켜서 생성한다.
	for( int i=0; i<m_charCount; ++i )
	{
		fsm = new FiniteStateMachine();

		//상태와 이벤트에 맞는 상태를 연결해서 저장해 준다.
		fsm->AddStateTransition( Character::STATE_STAND, Character::EVENT_STARTWALK, Character::STATE_MOVE );
		fsm->AddStateTransition( Character::STATE_MOVE, Character::EVENT_STOPWALK, Character::STATE_STAND );

		//현재의 상태(기본)을 설정
		fsm->SetCurrentState( Character::STATE_STAND );

		//캐릭터 설정/////////////////////////////////////////////////////////////
		Character* tmpCharacter = new Character( fsm, rand()%3+2 );

		tmpCharacter->SetPosition( rand()%800, rand()%600 );
		tmpCharacter->SetGoal( rand()%800, rand()%600 );
		tmpCharacter->SetId(i);
		//////////////////////////////////////////////////////////////////////////

		//map에 character등록
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
	//우선 찾아 봅시다
	std::map< DWORD, Character* >::iterator iter;
	Character* tmp;

	iter = m_mapCharacter.find( _index );
	if( iter == m_mapCharacter.end() )
		return 0;

	tmp = m_mapCharacter[_index];

	return tmp;
}
