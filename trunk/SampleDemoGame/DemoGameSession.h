#pragma once

#include "SSessionObj.h"

class DemoGameSession : public SSessionObj
{
private:
	int m_indexID;

public:
	DemoGameSession(void);
	~DemoGameSession(void);

	virtual SRTClass* GetRTClass() const { return &classDemoGameSession; }
	SDECLARE_DYNAMIC(DemoGameSession)			//static Runtime ����ü
	SDECLARE_DYNCREATE(DemoGameSession)		//static CreateObject�Լ�

	virtual void OnCreate();
	virtual void OnDestroy();

	void PacketParsing( SPacket& packet );

	//======================================
	// �� ��Ŷ�� ���� �˾Ƽ� ó��
	//======================================
	// ĳ���� ���� ��û
	void RecvCreateChar( SPacket* packet );

	//ĳ������ ��ȭ�� ����
	void RecvChangeChar( SPacket* packet );


	//======================================
	// ������ �Լ���
	//======================================
	//��� ĳ���Ϳ��� ������ ĳ������ ������ ����
	void SendCreateCharacter( int indexId, int x, int y, TCHAR* id );

	// ĳ���͸� ������ ����� ����
	void SendCreateCharacter( int myindex );

	//��� ĳ���Ϳ��� ������ ĳ������ ��ȭ ������ ����
	void SendChangeCharacter( int indexId, int state, int x, int y );

	//�������� �ϴϱ� ������ ���
	void SendDeleteCharacter();
};
