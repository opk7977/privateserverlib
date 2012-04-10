#pragma once

#include "SSocket.h"
#include "SPacket.h"


class CNetwork : public SSingleton <CNetwork>
{
private:
	friend class SSingleton<CNetwork>;

private:
	SSocket		m_srvSock;
	HANDLE		m_recvThread;
	HANDLE		m_startWork;

	SPacket		m_recvPack;


private:
	CNetwork(void);
	~CNetwork(void);

public:
	//소켓 초기화
	BOOL Init( BOOL isNon = FALSE );

	//네트워크 연결
	BOOL ConnectNet( char* ipAddr, int port );

	//다시 연결
	BOOL ReConnectNet( char* ipAddr, int port );

	//네트워크 연결 종료
	void DisConnect();

	void Release();

	//쓰레드에서 돌아갈 Recv함수
	unsigned int RecvEvent();
	//받은 패킷 id를 분석하는 함수
	void PacketParsing( SPacket* packet );

	//Send함수
	int SendPacket( SPacket* packet );

	//======================================
	// Send 관련 사용자 함수들
	//======================================
	// id와 pw를 연결된 서버에 넘긴다
	BOOL SendLogin( TCHAR* id, TCHAR* pw );

	// 연결은 되었으니 세션 ID를 보내서 캐릭터를 할당(?)받는다
	BOOL SendSessionId( int sessionID, TCHAR* charID );

	// 자신의 움직임에 관해 변화된 내용을 넘긴다.
	BOOL SendChangeState( int indexId, int state, int x, int y );

	//======================================
	// Recv 관련 사용자 함수들
	//======================================
	// 로그인 서버 연결 완료 패킷을 받으면 동작하는 함수
	void RecvConnectComplete();
	
	// 로그인의 결과를 받아 동작하는 함수
	void RecvResultNonId();
	void RecvResultNotPw();
	void RecvResultLogin( SPacket* packet );

	//////////////////////////////////////////////////////////////////////////
	// 게임서버에 연결이 정상적으로 됨
	void RecvConnectCompleteToG();

	// 게임서버에 연결이 정상적이지 않아 연결을 종료 해야 함
	void RecvConnectFailed();
	
	// 캐릭터들을 생성하라는 명령을 받아 동작하는 함수
	void RecvCreateCharacter( SPacket* packet );

	// 움직임이 있는 캐릭터에 대한 패킷을 받아 동작하는 함수
	void RecvMoveCharacter( SPacket* packet );

	//다른 캐릭터가 접속을 종료 했다
	void RecvDeleteCharacter( SPacket* packet );
};

#define GetNetwork CNetwork::GetInstance()


