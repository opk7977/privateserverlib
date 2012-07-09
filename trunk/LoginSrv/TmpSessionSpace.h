#pragma once

//======================================
// DB서버로부터의 답(?)을
// 기다리는 session들을
// 임시 index로 관리하는 class
// 입출력과 검색이 잦다
//======================================

#include "SIndexQueue.h"

class LoginSession;

class TmpSessionSpace : public SSingleton <TmpSessionSpace>, public SServerObj
{
private:
	friend class SSingleton<TmpSessionSpace>;

private:
	//응답을 기다리며 데기중인 Session
	std::vector<LoginSession*>		m_waitSession;

	//index
	SIndexQueue						m_indexQ;

	//공간 크기
	int								m_spaceSize;

public:
	TmpSessionSpace(void);
	~TmpSessionSpace(void);

	void Init( int SessionCount );
	void Release();

	//번호를 하나 받아 간다.
	int GetIndex();

	//대기 공간에 들어간다
	void WaitForDBSignal( int index, LoginSession* session );
	//신호를 받아 처리했음 세션공간을 비워준다/ index를 반환해 준다.
	void GetSignal( int index );

	//index로 해당 session을 찾는다
	LoginSession* FindSessionByIndex( int index );
};

#define GetTmpSpace TmpSessionSpace::GetInstance()
