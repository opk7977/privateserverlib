#include "CharMgr.h"

#include "NL_Time.h"
#include "NL_Input.h"


CharMgr::CharMgr(void)
{
	m_pInput	= I_NInput;
	m_pTime		= I_NTime;
}

CharMgr::~CharMgr(void)
{
}

void CharMgr::Init()
{
	m_heroObj.Init();
	m_heroObj.Load(_T("../data/3in.NOE"));
	m_heroObj.BuildOBB();
}

void CharMgr::Release()
{
	m_heroObj.Release();
}

void CharMgr::Frame()
{
	static float moveDelta = 1.0f;

	if( m_pInput->IsKeydown( VK_UP ) )
	{
		m_heroObj.MoveForward( -moveDelta * (m_pTime->GetElapsedTime() + 0.05f) );
	}
	else if( m_pInput->IsKeydown( VK_DOWN ) )
	{
		m_heroObj.MoveForward( moveDelta * (m_pTime->GetElapsedTime() + 0.05f) );
	}
	if( m_pInput->IsKeydown( VK_RIGHT ) )
	{
		m_heroObj.RotateY( moveDelta * (m_pTime->GetElapsedTime()) );
	}
	else if( m_pInput->IsKeydown( VK_LEFT ) )
	{
		m_heroObj.RotateY( -moveDelta * (m_pTime->GetElapsedTime()) );
	}

	m_heroObj.BuildOBB();
}

void CharMgr::Render()
{
	m_heroObj.Render();
	m_heroObj.RenderObjectOBB();
}
