#ifndef __WALK_STATE__
#define __WALK_STATE__

#include "Characterstate.h"

class Walkstate : public CharacterState
{
public:
	Walkstate( Character* parent );
	virtual ~Walkstate(void);

	virtual void Process();
};


#endif

