#pragma once

#include "SServerStd.h"

#define MOVE_SPEED	2

class CCharacter;

//���� class�� �θ�
class CCharacterState
{
protected:
	CCharacter*		m_parentObj;

public:
	CCharacterState( CCharacter* parent );
	virtual ~CCharacterState(void);

	virtual void Process() = 0;
};
