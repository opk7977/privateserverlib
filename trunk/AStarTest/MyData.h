#pragma once

#include "SServerStd.h"
#include "SList.h"

#define MAP_ROW 10
#define MAP_COL 10

const int	m_width		= 60;
const int	m_height	= 60;

struct POINT2
{
	int row, col;

	POINT2() {}
	POINT2( int r, int c ) : row(r), col(c) {}
	~POINT2() {}

	void Clear()
	{
		row = col = 0;
	}

	void SetElement( int r, int c )
	{
		row = r;
		col = c;
	}
};

class MyData : public SSingleton <MyData>
{
private:
	friend class SSingleton<MyData>;

private:
	//¸Ê(?)
	BOOL				m_mapIndex[MAP_ROW][MAP_COL];

	BOOL				m_isStartPoint;
	POINT2				m_start;

	BOOL				m_isGolePoint;
	POINT2				m_gole;

	BOOL				m_isSetLoad;
	SList<int*>			m_loadList;
	
public:
	MyData(void);
	~MyData(void);

	void MapInit();

	void AddWell( int row, int col );

	void SetPoint( int row, int col );

	void SetLoad();

	void PrintMap( HDC &hDc );
	void PrintStart( HDC &hDc );
	void PrintGole( HDC &hDc );
};

#define GetDocument	MyData::GetInstance()
