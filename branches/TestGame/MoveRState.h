#pragma once

#include "Characterstate.h"

class CMoveRState : public CCharacterState
{
public:
	CMoveRState(CCharacter* parent);
	~CMoveRState(void);

	virtual void Process();
};
