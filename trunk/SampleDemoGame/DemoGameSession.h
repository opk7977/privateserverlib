#pragma once

#include "SSessionObj.h"

class DemoGameSession : public SSessionObj
{
private:
	int m_indexID;

public:
	DemoGameSession(void);
	~DemoGameSession(void);

	virtual SRTClass* GetRTClass() const { return &classDemoGameSession; }
	SDECLARE_DYNAMIC(DemoGameSession)			//static Runtime 구조체
	SDECLARE_DYNCREATE(DemoGameSession)		//static CreateObject함수

	virtual void OnCreate();
	virtual void OnDestroy();

	void PacketParsing( SPacket& packet );

	//======================================
	// 각 패킷에 따라 알아서 처리
	//======================================
	// 캐릭터 생성 요청
	void RecvCreateChar( SPacket* packet );

	//캐릭터의 변화를 받음
	void RecvChangeChar( SPacket* packet );


	//======================================
	// 보내는 함수들
	//======================================
	//모든 캐릭터에게 지금의 캐릭터의 정보를 보냄
	void SendCreateCharacter( int indexId, int x, int y, TCHAR* id );

	// 캐릭터를 만들라는 명령을 내림
	void SendCreateCharacter( int myindex );

	//모든 캐릭터에게 지금의 캐릭터의 변화 정보를 보냄
	void SendChangeCharacter( int indexId, int state, int x, int y );

	//접속종료 하니까 지우라는 명령
	void SendDeleteCharacter();
};
