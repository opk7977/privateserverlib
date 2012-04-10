#pragma once

#include "SServerObj.h"

class GameProc;

class GProcMgr : public SSingleton <GProcMgr>
{
private:
	friend class SSingleton<GProcMgr>;

private:
	//���� ���μ��� map
	std::map<int, GameProc*>			m_mapGame;
	//���� ����
	int									m_iGameCount;


	//�Ӱ迵���� ���� obj
	SServerObj							m_critical;

public:
	GProcMgr(void);
	~GProcMgr(void);

	//���� ���μ����� ó���� �����Ѵ�
	void Init( int gameCount );

	//���� ���μ����� ��� ����
	void Release();

	//�ϳ��� ������ �����Ѵ�.
	GameProc* CreateGameProcess( int roomNo, int playerCount );

	//�����Ǿ� �ִ� ���� ���μ����� �޾� �´�
	GameProc* GetGameProcess( int roomNo );
};

#define GetGPMgr	GProcMgr::GetInstance()
