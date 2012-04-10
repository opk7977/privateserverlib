#pragma once

#include "SSessionObj.h"

class DemoLoginSession : public SSessionObj
{
public:
	DemoLoginSession(void);
	~DemoLoginSession(void);

public:
	virtual SRTClass* GetRTClass() const { return &classDemoLoginSession; }
	SDECLARE_DYNAMIC(DemoLoginSession)			//static Runtime ����ü
	SDECLARE_DYNCREATE(DemoLoginSession)		//static CreateObject�Լ�

	virtual void OnCreate();

	void PacketParsing( SPacket& packet );

	//======================================
	// �� ��Ŷ�� ���� �˾Ƽ� ó��
	//======================================
	// �α��� ��û
	void RecvLogin( SPacket* packet );


	//======================================
	// ������ �Լ���
	//======================================
	// �α��� ����� ������ �Լ�
	void SendLoginNonId();
	void SendLoginNotPw();
	void SendLogin( int loginResult );
};
