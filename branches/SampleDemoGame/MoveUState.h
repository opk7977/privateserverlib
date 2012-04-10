#pragma once

#include "Characterstate.h"

class CMoveUState : public CCharacterState
{
public:
	CMoveUState( CCharacter* parent );
	~CMoveUState(void);

	virtual void Process();
};
