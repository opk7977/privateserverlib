#include "WorldMain.h"
#include "SLogger.h"
#include "Document.h"
#include "FieldSrvMgr.h"

WorldMain::WorldMain(void)
{
	m_document		= &GetDocument;
	m_logger		= &GetLogger;
}

WorldMain::~WorldMain(void)
{
}

BOOL WorldMain::Init()
{
	//======================================
	// 서버 데이터 로드
	//======================================
	if( !m_document->SettingData() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("main\n데이터 로드 실패!\n\n") );
		return 0;
	}

	//======================================
	// 서버 초기화/ 셋팅
	//======================================
// 	if( !m_network->Init( m_document->SessionCount ) )
// 		return FALSE;
// 
// 	if( !m_network->SrvSetting( m_document->GameSrvPortNum ) )
// 		return FALSE;

	return TRUE;
}

void WorldMain::Release()
{
	//======================================
	// 서버 종료
	//======================================
	//m_network->Release();

	//======================================
	// 할당 해제
	//======================================
	//필드 서버 공간 해제

	//필트 데이터 공간 해제

	//..?
}
