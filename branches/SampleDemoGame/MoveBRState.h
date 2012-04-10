#pragma once

#include "Characterstate.h"

class CMoveBRState : public CCharacterState
{
public:
	CMoveBRState( CCharacter* parent );
	~CMoveBRState(void);

	virtual void Process();
};
