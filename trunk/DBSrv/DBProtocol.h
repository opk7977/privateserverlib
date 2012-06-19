#pragma once

//#define CONNECT_LOG_SERVER

enum SERVER_CODE
{
	// ���� ���� �� ���� ��ü���� ������
	// �������� ó���� ���� ���Ѱ��
	// ��������� �� ���� �� �ִ�.
	SERVER_ERROR			= -10,
	// ������ �����
	SUCCESSED_ACK			= 1,

	//--------------------------------------------------------------

	// IDüũ ����� ID�� �ߺ��ɶ�
	OVERLAPPED_ID			= -1,

	// �α��� ����� �ش� ID�� �̹� �α��� �߿� ������
	PRE_LOGIN_ID			= -5,
	// �α��� ����� ID�� ����
	NONEXISTENT_ID			= -1,
	// �α��� ����� PW�� Ʋ��
	WRONG_PW				= 0,

	//--------------------------------------------------------------

	// ���̻� �α��� �� �� �����ϴ�
	CHARAVER_SPACE_IS_FULL	= -20,

	//--------------------------------------------------------------
};

enum LOG_SERVER
{
	//========================================================
	// �α׼��� -> �ٸ�����
	// �α� ������ ���������� ���ӵǾ���
	//--------------------------------------------------------
	// ������ ����
	//========================================================
	LOG_SERVER_CONNECT_OK = 10000,

	//========================================================
	// �ٸ����� -> �α׼���
	// �α� ������ �Ѿ� ���� ��� ��Ŷ�� ������ ID�� ���´�
	//--------------------------------------------------------
	// int			-			ServerID
	// int			-			LogLevel
	// int			-			LogDataLenth
	// TCHAR		-			LogString
	//========================================================
	RECV_SERVER_LOG,

	//========================================================
	// �α׼��� -> �ٸ�����
	// �α� ���� �� ����
	//--------------------------------------------------------
	// ������ ����
	//========================================================
	LOG_SERVER_SHOTDOWN,
};

enum LOGIN_SERVER
{
	// server -> client
	// �α��� ������ ������ ���������� �Ǿ��ٴ� Ȯ�� ��Ŷ
	// ������ ����
	SC_LOGIN_CONNECT_OK = 10,
};

enum LOBBY_SERVER
{
	// server -> client
	// �κ� ������ ������ ���������� �Ǿ��ٴ� Ȯ�� ��Ŷ
	// ������ ����
	SC_LOBBY_CONNECT_OK = 200,
};

enum GAME_SERVER
{
	// server -> client
	// ���Ӽ������� ������ ���������� �Ǿ��ٴ� ��ȣ
	SC_GAME_CONNECT_OK = 300,
};

enum DB_SERVER
{
	// DBSrv -> otherSrv
	// ������ ����� �Ǿ��ٴ� ��ȣ
	// ������ ����
	DB_TO_OTHER_CONNECT_OK = 1000,

	// loginSrv -> DBSrv
	// id�ߺ� üũ
	// int			-		�ӽ� index
	// int			-		id������ ũ��
	// TCHAR		-		id
	LOGIN_TO_DB_CHECK_ID,

	// DBSrv -> loginSrv
	// id�ߺ�üũ ����� ������.
	// int			-		�ӽ� index
	// BOOL			-		result(��밡��:TRUE/���Ұ�:FALSE)
	DB_TO_LOGIN_CHECK_ID_RESULT,

	// loginSrv -> DBSrv
	// ���������� ��û�Ѵ�
	// int			-		�ӽ� index
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		pw�������� ũ��
	// TCHAR		-		pw
	// int			-		email�������� ũ��
	// TCHAR		-		email
	LOGIN_TO_DB_CREATE_ACCOUNT,

	// DBSrv -> loginSrv
	// ���������� ���� ����� ������
	// BOOL			-		result( ����:TRUE/ ����:FALSE )
	DB_TO_LOGIN_CREATE_ACCOUNT_RESULT,

	// loginSrv -> DBSrv
	// �α���
	// int			-		�ӽ� index
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		pw�������� ũ��
	// TCHAR		-		pw
	LOGIN_TO_DB_TRYLOGIN,

	// DBSrv -> lobbySrv
	// �α����� ��ٸ� lobby������ �� user�� data�� ������
	// int			-		�ӽ� index
	// int			-		sessionId
	// int			-		id������ ũ��
	// TCHAR		-		id
	// int			-		Rank_ID
	// int			-		Rank_Point
	// int			-		AccumulatedKillPoint
	// int			-		AccumulatedDeathPoint
	DB_TO_LOBBY_CHARACTER_LOGIN,

	// lobbySrv -> DBSrv
	// �κ� ĳ���� ������ �غ� �Ǿ��ٴ� ��ȣ�� �κ� ��񿡰� ����
	// int			-		�ӽ� index
	// int			-		sessionID
	// int			-		���
	//						( ���������� �غ� �Ϸ� : 1
	//						  ĳ���Ͱ� ���̻� ���� �� �� ���� : -10 )
	LOBBY_TO_DB_CHARINSERT_READY_RESULT,

	// DBSrv -> loginSrv
	// �α��� ����� ������
	// int			-		�ӽ� index
	// int			-		result( �̷̹α�����: PRE_LOGIN_ID(-5)
	//							  / ���� ���̵� : NONEXISTENT_ID(-1)
	//							  / ��� Ʋ��   : WRONG_PW(0)
	//							  / ����        : n(�ڽ���sessionID) )
	DB_TO_LOGIN_LOGINRESULT,

	// DBSrv -> otherSrv
	// �ߺ��� id�� �α��� �˴ϴ�. ������ ���ӵ� user�� ������ �����ϴ�.
	// int			-		sessionID
	DB_TO_OTHER_DROP_PLAYER,

	// gameSrv -> DBSrv
	// ������ ������ ĳ���͵��� ������ �����Ѵ�
	// int			-		game(room)Num
	// int			-		Playercount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_point	// ������ point
	// int			-		kill		// ������ killcount
	// int			-		death		// ������ deathcount
	//--------------------------------------> playerCount��ŭ �ݺ�
	GAME_TO_DB_UPDATE_USERDATA,

	// DBSrv -> GameSrv
	// ĳ���͵��� ������ �����ϰ� ���ŵ� ĳ������ ������ ���� ������ �޴´�
	// int			-		game(room)Num
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_id
	//--------------------------------------
	DB_TO_GAME_LEVEL_UP_PLAYER,

	// DBSrv -> lobbySrv
	// ĳ���� ������ ���Ǿ� ���ŵǸ� �κ� ���ο� �����͸� �˸���
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_id
	// int			-		rank_point	// ������ point
	// int			-		kill		// ������ killcount
	// int			-		death		// ������ deathcount
	//--------------------------------------> playerCount��ŭ �ݺ�
	DB_TO_LOBBY_UPDATE_USERDATA,

	// otherSrv -> DBSrv
	// ĳ���Ͱ� ������ �������ϴ�(������ ����)
	// int			-		sessionId
	OTHER_TO_DB_DISCONNECT_CHARACTER,
};
