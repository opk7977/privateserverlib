#ifndef __SESSION_MANAGER__
#define __SESSION_MANAGER__

//////////////////////////////////////////////////////////////////////////
#include "SIndexQueue.h"
#include "SMap.h"
#include "SList.h"
//////////////////////////////////////////////////////////////////////////

#include "SSession.h"

//수용 player 인원
#define DEFUALT_INDEX_COUNT		100


class SSessionMgr : public SSingleton <SSessionMgr>, public SServerObj
{
private:
	friend class SSingleton<SSessionMgr>;

private:
	//모든 player 정보를 담을 공간
	SMap					m_sessionMap;
	//비어있는 공간의 인덱스를 관리
	SIndexQueue				m_indexQueue;
	//접속한 player들을 저장한 공간
	SList<int>				m_listPlayer;


	//제한 인원이 모두 차있어서 수용을 못하는 경우
	//접속불가를 전송하기 위한 임시 Session공간
	SSession				m_tmpSession;

private:
	SSessionMgr(void);
	~SSessionMgr(void);

public:
	//넘겨 받은 class( 세션class )를 count만큼 생성한다
	void Create( SRTClass* pRTC = NULL, int _size = DEFUALT_INDEX_COUNT );
	//player가 들어오면 공간에 Session을 채워 주는것
	//새로 동적할당을 받는것은 아님!
	void CreateNewSession( SOCKET socket, SOCKADDR addr );
	//player가 나가면 공간에 Session을 지워 주는것
	//새로 동적할당을 해제하는 것이 아님!
	void RemoveSession( int index );

	//해당 index에 맞는 Session을 return해 준다.
	//사용자가 사용할 함수
	SServerObj*	GetSession( int index );


	//모든 세션에게 동일한 패킷 전송
// 	void SendAllSession( SPacket &packet );

	//세션 ID로 세션을 찾는다
	SServerObj* FindSession( int SessionId );

	//모든 동적할당 해제
	void Release();
};

#define GetSessionMgr SSessionMgr::GetInstance()

#endif

