#pragma once

#include "Characterstate.h"

class CMoveLBState : public CCharacterState
{
public:
	CMoveLBState( CCharacter* parent );
	~CMoveLBState(void);

	virtual void Process();
};
