#pragma once

#include "Characterstate.h"

class CMoveBState : public CCharacterState
{
public:
	CMoveBState( CCharacter* parent );
	~CMoveBState(void);

	virtual void Process();
};
