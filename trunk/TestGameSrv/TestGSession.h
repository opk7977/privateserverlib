#pragma once

#include "SSessionObj.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

class TestGSession : public SSessionObj
{
private:
	static SServerObj m_staticCritical;

private:
	int		m_iSession;					//세션ID
	int		m_iGameNo;					//게임(방)번호
	int		m_iState;					//상태
	int		m_iRotate;

	float	m_iDirX;					//방향
	float	m_iDirY;
	float	m_iDirZ;

	float	m_iPosX;					//위치
	float	m_iPosY;
	float	m_iPosZ;

public:
	TestGSession(void);
	~TestGSession(void);

public:
	virtual SRTClass* GetRTClass() const { return &classTestGSession; }
	SDECLARE_DYNAMIC(TestGSession)
	SDECLARE_DYNCREATE(TestGSession)

	void OnCreate();
	void OnDestroy();

	void Init();

	//id
	//void SetSessionId( int id );
	int GetSessionId();

	//게임번호
	//void SetGameNo( int num );
	int GetGameNo();

	//state
	void SetState( int state );
	int GetState();

	//direction
	void SetDir( float x, float y, float z );
	float GetDirX();
	float GetDirY();
	float GetDirZ();

	//pos
	void SetPos( float x, float y, float z );
	float GetPosX();
	float GetPosY();
	float GetPosZ();

	//내 정보를 넘겨 받은 패킷에 넣는 함수
	void operator()( TestGSession* session, SPacket& packet )
	{
		if( session->GetSessionId() == m_iSession )
			return;

		packet << session->GetSessionId();
		packet << session->GetState();
		packet << session->GetDirX();
		packet << session->GetDirY();
		packet << session->GetDirZ();
		packet << session->GetPosX();
		packet << session->GetPosY();
		packet << session->GetPosZ();

		//--------------------------------------------------------------
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("\t---------------------------------------------\n")
													 _T("\tSessionId	: %d\n")
													 _T("\tState		: %d\n")
													 _T("\tDir(x/y/z)	: %f, %f, %f\n")
													 _T("\tPos(x/y/z)	: %f, %f, %f\n")
													 , session->GetSessionId()
													 , session->GetState()
													 , session->GetDirX(), session->GetDirY(), session->GetDirZ()
													 , session->GetPosX(), session->GetPosY(), session->GetPosZ() );
#endif
		//--------------------------------------------------------------
	}

	//======================================

	void PacketParsing( SPacket& packet );

	//======================================
	// 패킷을 보내는 함수
	//======================================
	//SC_TGAME_CREATECHAR
	void SendCreateCharacter();			//나에게 모두를 보내게 한다
	void SendCreateMyCharacter();		//내 정보를 모두에게 보낸다

	//SC_TGAME_STARTGAME
	void SendStartGame();

	//SC_TGAME_STATEROTATE
	void SendChangeRotate();

	//SC_TGAME_STATECHANGE
	void SendChangeState();

	//SC_TGAME_DELETECHAR
	void SendDeleteCharacter();

	//======================================
	// 패킷을 받아 처리하는 함수
	//======================================
	//CS_TGAME_SENDINFO
	void RecvInfo( SPacket& packet );

	//CS_TGAME_READYOK
	void RecvReadyOK();

	//CS_TGAME_STATEROTATE
	void RecvChangRotate( SPacket& packet );

	//CS_TGAME_STATECHANGE
	void RecvChangeState( SPacket& packet );
};
