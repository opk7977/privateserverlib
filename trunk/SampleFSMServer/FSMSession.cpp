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
	// ��Ŷ�� data �Է�
	//======================================

	//�켱 ��Ŷ�� id����
	sendPacket.SetID( SERVER_CONNECT_OK );
	//�ο��� �־� �ְ�
	sendPacket << (int)num;

	//�ӽ÷� ĳ���͸� ��� �ֱ� ���� ������ �ϳ� �����.
	Character* tmpChar;
	//�� ĳ������ ������ ������� ��´�.
	for( int i=0; i<num; ++i )
	{
		tmpChar = GetCharMgr.FindChar( i );
		if( tmpChar == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[FSMSession] %d��° character�� �������� �ʽ��ϴ�\n"), i );
			return;
		}

		//�޾ƿ� ĳ������ ������ �ִ´�.
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



