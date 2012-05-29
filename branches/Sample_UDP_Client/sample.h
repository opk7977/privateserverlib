#pragma once

#include "NL_NeoLux.h"

class CharMgr;
class PacketParser;
class SNetwork;

//===================================================================
//  NAME	  : Sample_UDP_Client
//  COMMENTS  : -
//===================================================================
class Sample_UDP_Client : public NeoLux
{
	//==============================
	//	Sample Data
	//==============================
	CharMgr*			m_charMgr;
	PacketParser*		m_parser;
	SNetwork*			m_net;
	

public:
	//==============================
	//	Constructor & Destructor
	//==============================
	Sample_UDP_Client(void);
	~Sample_UDP_Client(void);

	//==============================
	//	Function
	//==============================
	bool Init(void);
	bool Frame(void);
	bool Render(void);
	bool Release(void);

	//======================================
	// 캐릭터 추가 제거
	//======================================
};
