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
	// ���� ������ �ε�
	//======================================
	if( !m_document->SettingData() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("main\n������ �ε� ����!\n\n") );
		return 0;
	}

	//======================================
	// ���� �ʱ�ȭ/ ����
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
	// ���� ����
	//======================================
	//m_network->Release();

	//======================================
	// �Ҵ� ����
	//======================================
	//�ʵ� ���� ���� ����

	//��Ʈ ������ ���� ����

	//..?
}
