#pragma once

#include "NL_Base.h"
#include "NL_Text.h"

//=========================================================================
//	Name   : NTextbox
//	Content: 텍스트 상자 클래스
//=========================================================================
class NTextbox
{
	//==================================
	//	Private Data
	//==================================
	typedef std::list< NTextAbs* >	TextList;
	typedef TextList::iterator		TextItor;
	
	int			m_leftX;
	int			m_topY;
	D3DCOLOR	m_color;
	int			m_maxTextCount;
	NFont*		m_pFont;
	TextList	m_textList;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	NTextbox();
	~NTextbox();

	//==================================
	//	External Function
	//==================================
	void Init(int leftX, int topY, D3DCOLOR color, NFont* pFont, int maxTextCount = -1);
	void Render();
	void Release();

	bool AddText(NTextAbs* pText);
	bool RemoveText(const TCHAR* removeText);
};