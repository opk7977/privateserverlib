#pragma once

#include "SSessionObj.h"

class DemoLoginSession : public SSessionObj
{
public:
	DemoLoginSession(void);
	~DemoLoginSession(void);

public:
	virtual SRTClass* GetRTClass() const { return &classDemoLoginSession; }
	SDECLARE_DYNAMIC(DemoLoginSession)			//static Runtime 구조체
	SDECLARE_DYNCREATE(DemoLoginSession)		//static CreateObject함수

	virtual void OnCreate();

	void PacketParsing( SPacket& packet );

	//======================================
	// 각 패킷에 따라 알아서 처리
	//======================================
	// 로그인 요청
	void RecvLogin( SPacket* packet );


	//======================================
	// 보내는 함수들
	//======================================
	// 로그인 결과를 보내는 함수
	void SendLoginNonId();
	void SendLoginNotPw();
	void SendLogin( int loginResult );
};
