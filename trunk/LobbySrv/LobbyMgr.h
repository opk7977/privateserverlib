#pragma once

#include "SServerStd.h"

class LobbyMgr : public SSingleton <LobbyMgr>
{
private:
	friend class SSingleton<LobbyMgr>;

private:
	//�κ� �ִ� player list
	std::map<int, int>					m_listPlayerInLobby;

	//�κ� �ִ� ��� ��
	int									m_iPlayerCountInLobby;

private:
	LobbyMgr(void);
	~LobbyMgr(void);

public:
	//�κ� �ִ� ����鿡�Ը� ��Ŷ ������

	//��� �߰��ϱ�
	//��� ����

	//map�� value�� �����ϱ�

	//������ ������


};

#define GetLobbyMgr LobbyMgr::GetInstance()
