#ifndef __STAND_STATE__
#define __STAND_STATE__

#include "Characterstate.h"

class StandState : public CharacterState
{
public:
	StandState( Character* parent );
	virtual ~StandState(void);

	virtual void Process();
};


#endif

