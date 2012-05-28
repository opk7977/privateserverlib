#include "NL_Font.h"

//===============================================================
//  NAME	  : NFont
//  COMMENTS  : 폰트 클래스 구현
//===============================================================

//============================
//	Constructor & Destructor
//============================
NFont::NFont(void):
	m_pFont(0)
{}
NFont::~NFont(void)
{}

//============================
//	Function
//============================
bool NFont::Init(	ND3d*	pDevice,
					INT		Height,
					UINT	Weight,
					LPCTSTR FontName,
					UINT	MipsLevel,
					BOOL	Italic,
					DWORD	CharSet,
					DWORD	OutputPrecs,
					DWORD	Quality,
					DWORD	PitchAndFamily	)
{
	Release();

	if( FAILED( D3DXCreateFont( pDevice->GetDevice(),	// D3dDevice
								Height,					// Height
								0,						// Width
								Weight,					// Weight
								MipsLevel,				// Mips Level
								Italic,					// Italic
								CharSet,				// Char Set
								OutputPrecs,			// Output Precsion
								Quality,				// Quality
								PitchAndFamily,			// Pitch and Family
								FontName,				// Face Name
								&m_pFont ) ) )			// Font device
	{
		return false;
	}
	m_pDevice = pDevice;	
	m_name   = FontName;
	m_height = Height;
	m_weight = Weight;

	return true;
}

void NFont::Draw( int x, int y, const TCHAR* text, D3DCOLOR color ) const
{
	static RECT rt = { 0, 0, 0, 0 };

	// 좌표
	rt.left	= x;
	rt.top	= y;

	// 출력
	m_pFont->DrawText( NULL, text, -1, &rt, DT_NOCLIP, color );
}
void NFont::Draw( int x, int y, D3DCOLOR color, const TCHAR* text, ... ) const
{
	static RECT		rt = { 0, 0, 0, 0 };
	static TCHAR	textBuffer[4096] = { 0 };

	// 좌표
	rt.left	= x;
	rt.top	= y;

	// 문자열 구성
	va_list arg;
	va_start( arg, text );
	_vstprintf_s( textBuffer, 4096, text, arg );
	va_end( arg );

	// 출력
	m_pFont->DrawText( NULL, textBuffer, -1, &rt, DT_NOCLIP, color );
}

void NFont::Reset()
{
	CString tmpStr = m_name;
	Init( m_pDevice, m_height, m_weight, tmpStr.GetString()  );
}

bool NFont::Release( void )
{
	SAFE_RELEASE( m_pFont )
	return true;
}