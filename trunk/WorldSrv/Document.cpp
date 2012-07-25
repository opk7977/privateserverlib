#include "Document.h"
#include "FieldMgr.h"
#include "FieldSrvMgr.h"

Document::Document(void)
{
	m_fieldMgr		= &GetFieldMgr;
	m_fieldSrvMgr	= &GetFieldSrvMgr;
}

Document::~Document(void)
{
}

BOOL Document::SettingData()
{
	//파일 열고
	FILE*	fp;
	fopen_s( &fp, "../data/WorldData.txt", "r" );

	//======================================
	// 열렸나 확인
	//======================================
	if( fp == NULL )
		return FALSE;

	//======================================
	// 읽을 데이터 변수 공간
	//======================================
	char text[256] = {0,};
	char tmpNum[8] = {0,};

	//======================================
	// 서버데이터를 읽어 저장
	//======================================
	//<<Srv Data>>
	fgets( text, 256, fp );
	//192.168.0.56 8888
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", m_MyIP, 16, tmpNum, 8 );
	m_MyPort = atoi( tmpNum );

	//======================================
	// field전체(world) 데이터를 읽어 저장
	//======================================
	//<<World Data>>
	fgets( text, 256, fp );
	
	//WorldTop: 0
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 8 );
	m_worldTop = atoi( tmpNum );
	
	//WorldLeft: 0
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 8 );
	m_worldLeft = atoi( tmpNum );
	
	//WorldBottom: 1000
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 8 );
	m_worldBottom = atoi( tmpNum );

	//WorldRight: 1000
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 8 );
	m_worldRight = atoi( tmpNum );

	//======================================
	// 필드 개수를 읽고 mgr의 공간을 생성
	//======================================
	// 임시 변수
	int count = 0;
	//======================================
	//<<Field Count>>
	fgets( text, 256, fp );
	//4
	fgets( text, 256, fp );
	sscanf_s( text, "%s", tmpNum, 8 );
	count = atoi( tmpNum );

	//mgr로 필드섭 개수를 넘겨 준다.
	if( !m_fieldMgr->Init( count ) )
		return FALSE;

	//srvMgr로 개수를 넘겨 준다
	if( !m_fieldSrvMgr->Init( count ) )
		return FALSE;

	for( int i=0; i<count; ++i )
	{
		if( !LoadFieldData( fp ) )
			return FALSE;
	}

	//======================================
	// 파일 종료
	//======================================
	fclose( fp );

	return TRUE;
}

BOOL Document::LoadFieldData( FILE* fp )
{
	int		field_id		= 0;
	char	field_name[16]	= {0,};
	TCHAR	field_tName[16]	= {0,};
	int		field_top		= 0;
	int		field_left		= 0;
	int		field_bottom	= 0;
	int		field_right		= 0;
	int		field_extra		= 0;

	char	tmpChar[16]		= {0,};
	char	text[256]		= {0,};
	char	tmpNum[8]		= {0,};


	//<<Field n>>
	fgets( text, 256, fp );

	//Field_ID: n
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", tmpChar, 16, tmpNum, 8 );
	field_id = atoi( tmpNum );

	//Field_Name: text
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", tmpChar, 16, field_name, 16 );
	MultiByteToWideChar( CP_ACP, 0, tmpChar, -1, field_tName, 16 );

	//Field_Top: n
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", tmpChar, 16, tmpNum, 8 );
	field_top = atoi( tmpNum );

	//Field_Left: n
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", tmpChar, 16, tmpNum, 8 );
	field_left = atoi( tmpNum );

	//Field_Bottom: n
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", tmpChar, 16, tmpNum, 8 );
	field_bottom = atoi( tmpNum );

	//Field_Right: n
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", tmpChar, 16, tmpNum, 8 );
	field_right = atoi( tmpNum );

	//Field_Extra: n
	fgets( text, 256, fp );
	sscanf_s( text, "%s %s", tmpChar, 16, tmpNum, 8 );
	field_extra = atoi( tmpNum );

	//mgr로 필드섭 정보를 넘겨 준다.
	return m_sigFieldMgr->SetDataForField( field_id, field_tName, field_top, field_left, field_bottom, field_right, field_extra );
}

