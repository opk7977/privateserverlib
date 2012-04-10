#ifndef __SAMPLE_FSM_SESSION__
#define __SAMPLE_FSM_SESSION__

#include "SSessionObj.h"

class FSMSession : public SSessionObj
{
public:
	FSMSession(void);
	virtual ~FSMSession(void);

public:
	virtual SRTClass* GetRTClass() const { return &classFSMSession; }
	SDECLARE_DYNAMIC(FSMSession)		//static Runtime 구조체
	SDECLARE_DYNCREATE(FSMSession)		//static CreateObject함수

	void OnCreate();

	//////////////////////////////////////////////////////////////////////////
	//--------------------------------------
	// 캐릭터(npc)의 모든 정보를 담아 날린다
	//--------------------------------------
	void SendAllNpcInfo();

};

#endif
