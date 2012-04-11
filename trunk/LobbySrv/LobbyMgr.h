#pragma once

#include "SServerStd.h"

class LobbyMgr : public SSingleton <LobbyMgr>
{
private:
	friend class SSingleton<LobbyMgr>;

private:
	//로비에 있는 player list
	std::map<int, int>					m_listPlayerInLobby;

	//로비에 있는 사람 수
	int									m_iPlayerCountInLobby;

private:
	LobbyMgr(void);
	~LobbyMgr(void);

public:
	//로비에 있는 사람들에게만 패킷 보내기

	//사람 추가하기
	//사람 뺴기

	//map에 value값 수정하기

	//방으로 보내기


};

#define GetLobbyMgr LobbyMgr::GetInstance()
