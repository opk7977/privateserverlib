#pragma once

#define PRE_UDP_PORTNUM	8000

#include "SSocket.h"
#include "SPacket.h"
#include "SThread.h"

class TmpPacketQueue;
class PreUDPNet : public SThread, public SSingleton <PreUDPNet>
{
private:
	friend class SSingleton<PreUDPNet>;

private:
	//╫л╠шео
	TmpPacketQueue*			m_packetQ;

private:
	SSocket					m_sock;
	SPacket					m_recvPack;

	BOOL					m_startThread;

private:
	PreUDPNet(void);
	~PreUDPNet(void);

public:
	BOOL Init();
	void Release();

	BOOL Run();
	void EndThread();
};

#define GetPreUDP PreUDPNet::GetInstance()
