#include "MyData.h"

MyData::MyData(void)
: m_isStartPoint(FALSE)
, m_isGolePoint(FALSE)
, m_isSetLoad(FALSE)
{
	m_start.Clear();
	m_gole.Clear();
}

MyData::~MyData(void)
{
}

void MyData::MapInit()
{
	for( int i=0; i<MAP_ROW; ++i )
	{
		for( int j=0; j<MAP_COL; ++j )
		{
			m_mapIndex[i][j] = TRUE;
		}
	}
}

void MyData::AddWell( int row, int col )
{
	if( m_isSetLoad )
		return;

	m_mapIndex[row][col] = m_mapIndex[row][col] ? FALSE : TRUE;
}

void MyData::SetPoint( int row, int col )
{
	if( !m_isStartPoint )
	{
		m_start.SetElement( row, col );
		m_isStartPoint = TRUE;
		return;
	}
	if( !m_isGolePoint )
	{
		m_gole.SetElement( row, col );
		m_isGolePoint = TRUE;
		return;
	}
}

void MyData::SetLoad()
{
	if( !m_isStartPoint || !m_isGolePoint )
		return;

	POINT2 tmpPoint = m_start;

	while(1)
	{
		tmpPoint
	}

	m_isSetLoad = TRUE;
}

void MyData::PrintMap( HDC &hDc )
{
	HPEN oldPen, bPen, rPen, gPen, cPen;
	bPen = CreatePen(PS_SOLID, 1, RGB(0,0,255));	//파랑
	rPen = CreatePen(PS_SOLID, 2, RGB(255,0,0));	//빨강
	gPen = CreatePen(PS_SOLID, 2, RGB(0,255,0));	//초록
	cPen = CreatePen(PS_SOLID, 2, RGB(0,255,255));	//청록

	oldPen = (HPEN)SelectObject( hDc, bPen );

	//기본 맵
	for( int i=0; i<MAP_ROW; ++i )
	{
		for( int j=0; j<MAP_COL; ++j )
		{
			if( m_mapIndex[i][j] )
				//갈 수 있는곳은 파랑색
				SelectObject( hDc, bPen );
			else
				//갈 수 없는곳은 빨간색
				SelectObject( hDc, rPen );

			Rectangle( hDc, i*m_width, j*m_height, (i+1)*m_width, (j+1)*m_height );
		}
	}

	//시작 위치
	if( m_isStartPoint )
	{
		SelectObject( hDc, gPen );

		Rectangle( hDc, m_start.row*m_width, m_start.col*m_height, (m_start.row+1)*m_width, (m_start.col+1)*m_height );
	}

	//골 위치
	if( m_isGolePoint )
	{
		SelectObject( hDc, cPen );

		Rectangle( hDc, m_gole.row*m_width, m_gole.col*m_height, (m_gole.row+1)*m_width, (m_gole.col+1)*m_height );
	}
	SelectObject( hDc, oldPen );
	DeleteObject(bPen);
	DeleteObject(rPen);
	DeleteObject(gPen);
	DeleteObject(cPen);

	//길 출력 검사
	if( !m_isSetLoad )
		return;

	//출력
}
