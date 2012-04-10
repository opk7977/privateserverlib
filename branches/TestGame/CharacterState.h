#pragma once

#include "SServerStd.h"

#define MOVE_SPEED	5

class CCharacter;

//상태 class의 부모
class CCharacterState
{
protected:
	CCharacter*		m_parentObj;

public:
	CCharacterState( CCharacter* parent );
	virtual ~CCharacterState(void);

	int GetDir();

	virtual void Process() = 0;
};
