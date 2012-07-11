#pragma once

#include "SSessionObj.h"

//--------------------------------------
// singleTon
//--------------------------------------
class SrvNet;
class SLogger;
class GameMgr;
class CharMgr;
class ItemMgr;

class DBSrvMgr;
//--------------------------------------
class CharObj;
class GameProc;


enum CHATTING_INDEX
{
	CHATTING_ALL = 0,
	CHATTING_TEAM,
};

class GameSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(GameSession)
	SDECLARE_DYNCREATE(GameSession)

private:
	//======================================
	// singleTon��ü��
	//======================================
	static SrvNet*			m_srvNet;
	static SLogger*			m_logger;
	static GameMgr*			m_gameMgr;
	static CharMgr*			m_charMgr;
	static ItemMgr*			m_itemMgr;

	static DBSrvMgr*		m_dbMgr;
	//======================================

	//�� ����
	CharObj*			m_myCharInfo;
	//�� ���� ���μ���(��)
	GameProc*			m_myGameProc;


	//������ �����ǰ�?(�κ�� ������ �ƴϴ�)
	BOOL				isEndGame;

public:
	GameSession(void);
	~GameSession(void);

	void Clear();

	void OnCreate();
	void OnDestroy();

	CharObj*	GetMyInfo() const { return m_myCharInfo; }
	GameProc*	GetMyGame() const { return m_myGameProc; }

	inline void SetGotoGame() { isEndGame = FALSE; }

	//�� ip������ ����
	void PackageMyNetInfo( SPacket& packet );

	//��Ŷ ó��
	void PacketParsing( SPacket& packet );

	//======================================
	// ���� ��Ŷ ó��
	//======================================
	//--------------------------------------
	// DB �������� Ŀ�´����̼�
	//--------------------------------------
	//DB_TO_OTHER_CONNECT_OK
	void RecvDBConnectOK();

	//DB_TO_OTHER_DROP_PLAYER
	void RecvCharDrop( SPacket &packet );

	//--------------------------------------
	// lobby �������� Ŀ�´����̼�
	//--------------------------------------
	//SC_LOBBY_CONNECT_OK
	void RecvLobbyConnectOK();

	//LG_START_GAME
	void RecvLobbyStartGame( SPacket &packet );

	//LG_END_GAME_READY_OK
	void RecvLobbyEndReadyOK( SPacket &packet );

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//CS_GAME_INGAME
	void RecvInGame( SPacket &packet );

	//CS_GAME_GAME_READY_OK
	void RecvGameReadyOK();

	//CS_GAME_CHARACTER_SYNC
	void RecvGameCharacterSync( SPacket &packet );

	//CS_GAME_CHARACTER_JUMP
	void RecvGameCharacterJump();
	
	//CS_GAME_CHARACTER_LAND
	void RecvGameCharacterLand( SPacket &packet );

	//CS_GAME_CHAR_CHANGE_OBJ
	void RecvGameCharChangeObj( SPacket &packet );

	//CS_GAME_ATTACK
	void RecvGameAttack( SPacket &packet );

	//CS_GAME_TRY_ATTACK
	void RecvGameTryAttack( SPacket &packet );

	//CS_GAME_JUST_SHOOT
	void RecvGameJustShoot();

	//CS_GAME_LAY_MINE
	void RecvGameLayMine( SPacket &packet );

	//CS_GAME_WEAPON_CHANGE
	void RecvGameChangeWeapon( SPacket &packet );

	//CS_GAME_VISIBLE_HIDE
	void RecvGameVisibleHide();

	//CS_GAME_INVISIBLE_HIDE
	void RecvGameInvisibleHide();

	//CS_GAME_SKILL_SCAN_ON
	void RecvGameSkillScanOn();

	//CS_GAME_SKILL_SCAN_OFF
	void RecvGameSkillScanOff();

	//CS_GAME_KILL_MYSELF
	void RecvGameKillMyself();

	//CS_GAME_CHANGE_STATE
	void RecvGameChangeState( SPacket &packet );

	//CS_GAME_ASK_REVIVAL
	void RecvGameAskRevival( SPacket &packet );

	//CS_GAME_CHATTING
	void RecvGameChatting( SPacket &packet );

	//CS_GAME_INSTALL_BOOM

	//CS_GAME_UNINSTALL_BOOM

	//CS_GAME_RADIO_PLAY
	void RecvGameRadioPlay( SPacket &packet );

	//CS_GAME_INSTALL_ITEM

//	//CS_GAME_GOTO_LOBBY

	//test
	//CS_GAME_END_GUNSELECT
	void RecvGameEndGunSelect();

	//test
	//CS_GAME_START_TIME_COUNTDOWN
	void RecvGameStartCountDown();


	//======================================
	// ���� ��Ŷ ó��
	//======================================
	//--------------------------------------
	// lobby �������� Ŀ�´����̼�
	//--------------------------------------
	//GL_CONNECT_SERVER
	BOOL SendConnectOK();

	//GL_START_FAILD
	BOOL SendStartFaild( int roomNum );

	//GL_START_OK
	BOOL SendStartOK( int roomNum );

	//GL_GAME_END
	BOOL SendGameEnd( int roomNum );

	//GL_PLAYER_DISCONNECT
	BOOL SendToSrvThatPlayerDisconnect();

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//SC_GAME_CONNECT_OK
	//�Լ� ����

	//SC_GAME_CHARINFO_INGAME
	BOOL SendOtherCharInfoToMe();				//���ӳ��� ������ ������� ������ �޴´�
	BOOL SendMyCharInfoToInGamePlayer();		//���ӳ��� ����鿡�� �� ������ ������

	//SC_GAME_START_GAME
	//����Proc�� ����

	//SC_GAME_CHARACTER_JUMP
	BOOL SendGameCharacterJump();

	//SC_GAME_CHARACTER_LAND
	BOOL SendGameCharacterLand( int objIndex, int charState );

	//SC_GAME_CHAR_CHANGE_OBJ
	BOOL SendGameCharChangeObj( int objIndex );

	//SC_GAME_ATTACK
	BOOL SendGameAttack( CharObj* attactedChar, float posX, float posY, float posZ, float normalX, float normalY, float normalZ );

	//SC_GAME_YOU_ATTACKED
	BOOL SendGameYouAttack( CharObj* attactedChar );

	//SC_GAME_TRY_ATTACK
	BOOL SendGameTryAttact( SPacket& packet );

	//SC_GAME_JUST_SHOOT
	//�Լ� ����

	//SC_GAME_CHAR_DIE //SC_GAME_YOU_DIE
	BOOL SendGameDie( BOOL isHead, CharObj* dieChar );
	//ĳ���� �ڻ�
	BOOL SendGameDie();

	//SC_GAME_LAY_MINE
	BOOL SendGameLayMine( float posX, float posY, float posZ, float dirX, float dirY, float dirZ );

	//SC_GAME_LAY_MINE_FAILD
	//�Լ� ����

	//SC_GAME_RUN_MINE
	//����proc�� ����

	//SC_GAME_EXPLOSION_MINE
	//����proc�� ����

	//SC_GAME_CHARACTER_DAMEGED_BY_MINE
	//����proc�� ����

	//SC_GAME_YOU_DAMEGED_BY_MINE
	//����proc�� ����

	//SC_GAME_CHARACTER_DIE_BY_MINE
	//����proc�� ����

	//SC_GAME_YOU_DIE_BY_MINE
	//����proc�� ����

	//SC_GAME_WEAPON_CHANGE
	BOOL SendGameChangeWeapon( int weapon );

	//SC_GAME_CHAR_VISIBLE_HIDE
	BOOL GameCharVisibleHide();

	//SC_GAME_TIMEOUT_HIDE

	//SC_GAME_CHAR_INVISIBLE_HIDE
	BOOL GameCharInvisibleHide();

	//SC_GAME_HIDE_POINT

	//SC_GAME_CHANGE_STATE
	BOOL SendGameChangeState( int state, BOOL isJump, int objIndex );

	//SC_GAME_CHAR_REVIVAL
	BOOL SendGameCharRevival( int pointIndex );

	//SC_GAME_CHATTING
	BOOL SendGameChatting( TCHAR* chatting );
	BOOL SendGameTeamChat( TCHAR* chatting );

	//SC_GAME_INSTALL_BOOM

	//SC_GAME_BOOM_COUNTDOWN

	//SC_GAME_UNINSTALL_BOOM

	//SC_GAME_RADIO_PLAY
	BOOL SendGameRadioPlay( int index );

	//SC_GAME_TIME_COUNTDOWN
	//����Proc�� ����

	//SC_TIME_OUT
	//����proc�� ����

//	//SC_GAME_END

	//SC_GAME_INSTALL_ITEM

	//SC_GAME_RUN_ITEM

	//SC_GAME_RESTART
	//����Proc�� ����

	//SC_GAME_GOTO_LOBBY
	//����Proc�� ����

	//SC_LOBBY_GAME_SELF_DISCONNECT
	BOOL SendGameSelfDisconnect();

//	//SC_GAME_CHAR_GOTO_LOBBY

	//SC_GAME_CHAR_DISCONNECT
	BOOL SendCharDisconnect();

};
