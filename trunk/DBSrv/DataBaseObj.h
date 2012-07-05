#pragma once

#include "SServerStd.h"

class PlayerObj;

class DataBaseObj
{
public:
	DataBaseObj(void) {}
	virtual ~DataBaseObj(void) {}

	// DB연결 및 초기화
	//mdb 파일과 연결
	virtual BOOL Init( TCHAR* dbname, TCHAR* _id, TCHAR* _pw ) = 0;
	virtual void Release() = 0;

	//--------------------------------------------------------------
	// DB에서 랭크 정보 받아와서 셋팅해 주기
	//--------------------------------------------------------------
	virtual BOOL RankDataSetting() = 0;
	//--------------------------------------------------------------

	//id체크
	virtual BOOL CheckID( TCHAR* _id ) = 0;
	//계정 생성
	virtual BOOL CreateAccount( TCHAR* _id, TCHAR* _pw, TCHAR* email ) = 0;
	//로그인
	virtual int TryLogin( TCHAR* _id, TCHAR* _pw ) = 0;
	//UserData를 받아 셋팅
	virtual BOOL SettingUserData( int sessionID, PlayerObj* player ) = 0;
	//DB를 업데이트
	virtual BOOL UpdateUserData( int sessionID, int rankId, int rankPoint, int accumulKill, int accumulDeath ) = 0;
};
