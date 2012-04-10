#ifndef __CHARACTER_STATE__
#define __CHARACTER_STATE__

//전방 선언!
class Character;

//srand를 위해 그냥 선언해 놓자..
#include "SServerStd.h"
#include <time.h>

//각 상태가 기본적으로 상속 받을 클래스
//추상 클래스
class CharacterState
{
protected:
	Character*		m_parentObj;

public:
	CharacterState( Character* parent );
	virtual ~CharacterState(void);

	virtual void Process()	= 0;
};


#endif

