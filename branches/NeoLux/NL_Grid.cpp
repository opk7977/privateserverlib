#include "NL_Grid.h"
#include "NL_Debug.h"

NGrid::NGrid(void) :
	m_pLineVert(0)
{
}

NGrid::~NGrid(void)
{
}

void NGrid::Init()
{
	m_pDebug = I_NDebug;
}

void NGrid::Render()
{
	m_pDebug->DrawLineList( m_pLineVert, m_lineCount );
}

void NGrid::SetGrid(int cellCount, DWORD color, float interval)
{
	Release();

	if( cellCount % 2 )
		++cellCount;

	m_interval = interval;
	m_cellCount = cellCount;
	m_color = color;
	m_lineCount = ( m_cellCount + 1 ) * 2;
	SAFE_ARRAY_NEW( m_pLineVert, P3C_VERTEX, m_lineCount * 2 )

	float curPos;
	int curIndex = 0;
	int m_halfCount = m_cellCount / 2;
	float endPos = m_halfCount * interval;
	for(int i = 0; i <= m_cellCount; ++i)
	{
		curPos = (i - m_halfCount) * interval;
	
		// 가로 줄
		m_pLineVert[ curIndex ].vc = m_color;
		m_pLineVert[ curIndex ].vp.y = 0.0f;
		m_pLineVert[ curIndex ].vp.z = curPos;
		m_pLineVert[ curIndex ].vp.x = -endPos;
		++curIndex;

		m_pLineVert[ curIndex ].vc = m_color;
		m_pLineVert[ curIndex ].vp.y = 0.0f;
		m_pLineVert[ curIndex ].vp.z = curPos;
		m_pLineVert[ curIndex ].vp.x = endPos;
		++curIndex;

		// 세로 줄
		m_pLineVert[ curIndex ].vc = m_color;
		m_pLineVert[ curIndex ].vp.y = 0.0f;
		m_pLineVert[ curIndex ].vp.x = curPos;
		m_pLineVert[ curIndex ].vp.z = -endPos;
		++curIndex;

		m_pLineVert[ curIndex ].vc = m_color;
		m_pLineVert[ curIndex ].vp.y = 0.0f;
		m_pLineVert[ curIndex ].vp.x = curPos;
		m_pLineVert[ curIndex ].vp.z = endPos;
		++curIndex;
	}
}

void NGrid::Release()
{
	SAFE_ARRAY_DELETE( m_pLineVert )
}