#pragma once

#include "characterstate.h"

class CMoveLState : public CCharacterState
{
public:
	CMoveLState( CCharacter* parent );
	~CMoveLState(void);

	virtual void Process();
};
