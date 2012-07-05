#pragma once

#include "SServerStd.h"

class PlayerObj;

class DataBaseObj
{
public:
	DataBaseObj(void) {}
	virtual ~DataBaseObj(void) {}

	// DB���� �� �ʱ�ȭ
	//mdb ���ϰ� ����
	virtual BOOL Init( TCHAR* dbname, TCHAR* _id, TCHAR* _pw ) = 0;
	virtual void Release() = 0;

	//--------------------------------------------------------------
	// DB���� ��ũ ���� �޾ƿͼ� ������ �ֱ�
	//--------------------------------------------------------------
	virtual BOOL RankDataSetting() = 0;
	//--------------------------------------------------------------

	//idüũ
	virtual BOOL CheckID( TCHAR* _id ) = 0;
	//���� ����
	virtual BOOL CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* email ) = 0;
	//�α���
	virtual int TryLogin( TCHAR* _id, TCHAR* _pw ) = 0;
	//UserData�� �޾� ����
	virtual BOOL SettingUserData( int sessionID, PlayerObj* player ) = 0;
	//DB�� ������Ʈ
	virtual BOOL UpdateUserData( int sessionID, int rankId, int rankPoint, int accumulKill, int accumulDeath ) = 0;
};
