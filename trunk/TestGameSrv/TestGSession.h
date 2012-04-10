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
	int		m_iSession;					//����ID
	int		m_iGameNo;					//����(��)��ȣ
	int		m_iState;					//����
	int		m_iRotate;

	float	m_iDirX;					//����
	float	m_iDirY;
	float	m_iDirZ;

	float	m_iPosX;					//��ġ
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

	//���ӹ�ȣ
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

	//�� ������ �Ѱ� ���� ��Ŷ�� �ִ� �Լ�
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
	// ��Ŷ�� ������ �Լ�
	//======================================
	//SC_TGAME_CREATECHAR
	void SendCreateCharacter();			//������ ��θ� ������ �Ѵ�
	void SendCreateMyCharacter();		//�� ������ ��ο��� ������

	//SC_TGAME_STARTGAME
	void SendStartGame();

	//SC_TGAME_STATEROTATE
	void SendChangeRotate();

	//SC_TGAME_STATECHANGE
	void SendChangeState();

	//SC_TGAME_DELETECHAR
	void SendDeleteCharacter();

	//======================================
	// ��Ŷ�� �޾� ó���ϴ� �Լ�
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
