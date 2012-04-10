#include "FSMSession.h"
#include "CharacterMgr.h"
#include "FSMProtocol.h"
#include "Character.h"
#include "SLogger.h"

SIMPLEMENT_DYNAMIC(FSMSession)
SIMPLEMENT_DYNCREATE(FSMSession)

FSMSession::FSMSession(void)
{
}

FSMSession::~FSMSession(void)
{
}

void FSMSession::OnCreate()
{
	SSession::OnCreate();

	SendAllNpcInfo();
}


//////////////////////////////////////////////////////////////////////////

void FSMSession::SendAllNpcInfo()
{
	SPacket	sendPacket;
	int num = GetCharMgr.GetCharCount();

	//======================================
	// 패킷에 data 입력
	//======================================

	//우선 패킷의 id설정
	sendPacket.SetID( SERVER_CONNECT_OK );
	//인원을 넣어 주고
	sendPacket << (int)num;

	//임시로 캐릭터를 담고 있기 위에 변수를 하나 만든다.
	Character* tmpChar;
	//각 캐릭터의 정보를 순서대로 담는다.
	for( int i=0; i<num; ++i )
	{
		tmpChar = GetCharMgr.FindChar( i );
		if( tmpChar == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[FSMSession] %d번째 character는 존재하지 않습니다\n"), i );
			return;
		}

		//받아온 캐릭터의 정보를 넣는다.
		sendPacket << tmpChar->GetId();
		sendPacket << (int)(tmpChar->GetState());
		sendPacket << tmpChar->GetSpeed();
		sendPacket << tmpChar->GetPosX();
		sendPacket << tmpChar->GetPosY();
		sendPacket << tmpChar->GetGoalX();
		sendPacket << tmpChar->GetGoalY();

		//======================================
		// Log Code
		//======================================
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[FSMSession] Packet send==========================================\n")
														_T("\tPacketID	: SERVER_CONNECT_OK\n")
														_T("\tcharID	: %d\n")
														_T("\tcharState	: %s\n")
														_T("\tcharSpeed	: %d\n")
														_T("\tcharX, Y	: %d, %d\n")
														_T("\tgoalX, Y	: %d, %d\n")
														_T("[FSMSession] ==========================================Packet send\n")
														, tmpChar->GetId()
														, (tmpChar->GetState() == 0 ) ? _T("stand") : _T("walk")
														, tmpChar->GetSpeed()
														, tmpChar->GetPosX(), tmpChar->GetPosY()
														, tmpChar->GetGoalX(), tmpChar->GetPosY() );
		//////////////////////////////////////////////////////////////////////////
	}

	SendPacket( sendPacket );
}



