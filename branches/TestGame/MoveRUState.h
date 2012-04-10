#pragma once

#include "Characterstate.h"

class CMoveRUState : public CCharacterState
{
public:
	CMoveRUState(CCharacter* parent);
	~CMoveRUState(void);

	virtual void Process();
};
