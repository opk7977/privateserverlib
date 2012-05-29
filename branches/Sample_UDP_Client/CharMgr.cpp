#include "PacketSender.h"
#include "CharMgr.h"

#include "NL_Time.h"
#include "NL_Input.h"


CharMgr::CharMgr(void)
: m_heroObj(NULL)
{
	m_pInput	= I_NInput;
	m_pTime		= I_NTime;
	m_sender	= &GetSender;
}

CharMgr::~CharMgr(void)
{
}

void CharMgr::Init()
{
}

void CharMgr::Release()
{
	//m_heroObj.Release();
	if( m_heroObj != NULL )
	{
		m_heroObj->Release();
		m_heroObj = NULL;
	}

	if( m_player.IsEmpty() )
		return;

	POSITION pos = m_player.GetStartPosition();
	NObject* tmp;
	while( pos )
	{
		tmp = m_player.GetNextValue( pos );
		tmp->Release();
		tmp = 0;
	}
	m_player.RemoveAll();
}

void CharMgr::Frame()
{
	if( m_heroObj == NULL )
		return;

	static float moveDelta = 1.0f;

	if( m_pInput->IsKeydown( VK_UP ) )
	{
		m_heroObj->MoveForward( -moveDelta * (m_pTime->GetElapsedTime() + 0.05f) );
	}
	else if( m_pInput->IsKeydown( VK_DOWN ) )
	{
		m_heroObj->MoveForward( moveDelta * (m_pTime->GetElapsedTime() + 0.05f) );
	}
	if( m_pInput->IsKeydown( VK_RIGHT ) )
	{
		m_heroObj->RotateY( moveDelta * (m_pTime->GetElapsedTime()) );
	}
	else if( m_pInput->IsKeydown( VK_LEFT ) )
	{
		m_heroObj->RotateY( -moveDelta * (m_pTime->GetElapsedTime()) );
	}

	m_heroObj->BuildOBB();

	static float sendTime = 0.0f;
	sendTime += m_pTime->GetElapsedTime();
	if( sendTime >= 0.1f )
	{
		NVECTOR3 pos, dir;
		pos = m_heroObj->GetPosition();
		dir = m_heroObj->GetDirection();
		m_sender->SendCHARInfo( m_heroObj->GetId(), pos.x, pos.y, pos.z, dir.x, dir.y, dir.z );
	}

}

void CharMgr::Render()
{
	if( m_heroObj == NULL )
		return;

	//나를 출력
	m_heroObj->Render();
	m_heroObj->RenderObjectOBB();

	//남들을 출력
	POSITION pos = m_player.GetStartPosition();
	NObject* tmp;
	while( pos )
	{
		tmp = m_player.GetNextValue( pos );
		tmp->Render();
	}
}

void CharMgr::AddMe( int _id )
{
	m_heroObj = new GameChar;
	m_heroObj->Init();
	m_heroObj->Load(_T("../data/3in.NOE"));
	m_heroObj->SetId( _id );
	m_heroObj->BuildOBB();
}

void CharMgr::AddChar( int _id )
{
	NObject* tmpObj = new NObject;
	tmpObj->Init();
	tmpObj->Load(_T("../data/3in.NOE"));
	m_player[_id] = tmpObj;
}

void CharMgr::DelChar( int _id )
{
	if( m_player.IsEmpty() )
		return;

	m_player.RemoveKey( _id );
}

NObject* CharMgr::FindChar( int _id )
{
	if( m_player.IsEmpty() )
		return NULL;

	NObject* tmpObj;
	if( m_player.Lookup( _id, tmpObj ) == false )
		return NULL;

	return tmpObj;
}
