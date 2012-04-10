#pragma once

#include "Characterstate.h"

class CStandState : public CCharacterState
{
public:
	CStandState( CCharacter* parent );
	~CStandState(void);

	virtual void Process();
};
