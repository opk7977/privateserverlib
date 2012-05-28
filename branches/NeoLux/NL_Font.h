#pragma once

#include "NL_Base.h"
#include "NL_D3d.h"

//===============================================================
//  NAME	  : NFont
//  COMMENTS  : 폰트 클래스 선언
//===============================================================
class NFont
{
	//============================
	//	NFont Data
	//============================
	CString		m_name;
	int			m_height;
	int			m_weight;
	LPD3DXFONT	m_pFont;
	ND3d*		m_pDevice;
	
public:
	//============================
	//	Constructor & Destructor
	//============================
	NFont(void);
	~NFont(void);

	//============================
	//	Function
	//============================
	bool		Init(	ND3d*	pDevice,
						INT		Height			= 18,
						UINT	Weight			= FW_NORMAL, 
						LPCTSTR FontName		= _T("Arial"), 
						UINT	MipsLevel		= 1,
						BOOL	Italic			= FALSE,
						DWORD	CharSet			= DEFAULT_CHARSET,
						DWORD	OutputPrecs		= OUT_DEFAULT_PRECIS, 
						DWORD	Quality			= DEFAULT_QUALITY, 
						DWORD	PitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE	);
	bool		 Release( void );

	void	 	 Draw( int x, int y, const TCHAR* text, D3DCOLOR color = 0xff888888 ) const;
	void		 Draw( int x, int y, D3DCOLOR color, const TCHAR* text, ... ) const;
	LPD3DXFONT	 GetFont( void )		const	{	return m_pFont;				}
	int			 GetHeight( void )	const	{	return m_height;			}
	int			 GetWeight( void )	const	{	return m_weight;			}
	const TCHAR* GetName( void )		const	{	return m_name.GetString();	}

	void		 Reset();
};

enum
{
	NL_BLACK		= 0xff000000,
	NL_GRAY			= 0xff888888,
	NL_LIGHTGRAY	= 0xffbbbbbb,
	NL_WHITE		= 0xffffffff,

	NL_RED			= 0xffff0000,
	NL_GREEN		= 0xff00ff00,
	NL_BLUE			= 0xff0000ff,
	NL_YELLOW		= 0xffffff00,
	NL_PURPLE		= 0xffff00ff,
	NL_CYAN			= 0xff00ffff,
	NL_ORANGE		= 0xffff8800,

	NL_LIGHTRED		= 0xffff8888,
	NL_LIGHTGREEN	= 0xff88ff88,
	NL_LIGHTBLUE	= 0xff8888ff,
	NL_LIGHTYELLOW	= 0xffffff88,
	NL_LIGHTPURPLE	= 0xffff88ff,
	NL_LIGHTCYAN	= 0xff88ffff,
	NL_LIGHTORANGE	= 0xffffbb00,
};