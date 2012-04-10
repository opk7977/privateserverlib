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
	SDECLARE_DYNAMIC(FSMSession)		//static Runtime ����ü
	SDECLARE_DYNCREATE(FSMSession)		//static CreateObject�Լ�

	void OnCreate();

	//////////////////////////////////////////////////////////////////////////
	//--------------------------------------
	// ĳ����(npc)�� ��� ������ ��� ������
	//--------------------------------------
	void SendAllNpcInfo();

};

#endif
