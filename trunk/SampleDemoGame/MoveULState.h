#pragma once

#include "Characterstate.h"

class CMoveULState : public CCharacterState
{
public:
	CMoveULState( CCharacter* parent );
	~CMoveULState(void);

	virtual void Process();
};
