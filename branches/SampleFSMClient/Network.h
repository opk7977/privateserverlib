#ifndef __NETWORK__
#define __NETWORK__


#include "SSocket.h"
#include "SPacket.h"

class Network
{
	SSocket		m_mySock;
	HANDLE		m_recvThread;
	HANDLE		m_startWork;

	SPacket		m_recvPack;

public:
	Network(void);
	virtual ~Network(void);


	//소켓 초기화 및 설정//소켓을 넌블럭으로 설정하려면 TRUE를 넣어 준다
	BOOL Init( BOOL isNon = FALSE );

	//서버로 연결
	BOOL ConnectSrv( char* ipAddr, int port );

	//소켓 해제
	void Release();

	//--------------------------------------
	// Recv하는 쓰레드 함수
	//--------------------------------------
	unsigned int RecvEvent();


	//======================================
	// 패킷을 받아 패킷에 따라 처리하는 함수를 호출
	//======================================
	void PacketParsing( SPacket* packet );
	//////////////////////////////////////////////////////////////////////////
	//--------------------------------------
	// 패킷을 처리하는 함수들
	//--------------------------------------
	//처음 접속하고 캐릭터를 생성하라는 패킷을 받아 처리하는 함수
	void CreateCharacter( SPacket* packet );
	//npc의 상태가 변해서 변화를 받아와 처리하는 함수
	void ChangeCharacter( SPacket* packet );
};



#endif

