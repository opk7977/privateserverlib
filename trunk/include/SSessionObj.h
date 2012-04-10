#ifndef __SESSION_OBJECT__
#define __SESSION_OBJECT__

#include "SSession.h"

class SSessionObj : public SSession
{
public:
	SSessionObj(void);
	virtual ~SSessionObj(void);

	//각 서버의 session class가 상속받아 사용할 class
	//추상클래스 이므로 반드시 상속받아 사용해야 한다
	virtual void OnCreate() = 0;

};


#endif

