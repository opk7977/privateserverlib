#ifndef __CHARACTER_STATE__
#define __CHARACTER_STATE__

//���� ����!
class Character;

//srand�� ���� �׳� ������ ����..
#include "SServerStd.h"
#include <time.h>

//�� ���°� �⺻������ ��� ���� Ŭ����
//�߻� Ŭ����
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

