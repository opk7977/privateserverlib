#pragma once

#include "NL_Base.h"
#include "NL_Font.h"

//=========================================================================
//	Name   : NText
//	Content: 문자열 클래스
//=========================================================================
class NTextAbs
{
protected:
	CString m_name;

public:
	NTextAbs(){}
	virtual ~NTextAbs(){}

	virtual void Render(int x, int y, D3DCOLOR color, NFont* pFont) = 0;
	const TCHAR* GetName() const {	return m_name.GetString();	}
};

template <typename _Type>
class NText : public NTextAbs
{	
	//==================================
	//	Private Data
	//==================================
	_Type	m_data;

public:
	//==================================
	//	Ctor & Dtor
	//==================================
	explicit NText(const TCHAR* name, _Type data)
	{
		m_name = name;
		m_data = data;
	}
	~NText(){}

	//==================================
	//	External Function
	//==================================
	void Render(int x, int y, D3DCOLOR color, NFont* pFont)
	{
		assert( !_T("NText: This Format can't Render") );
	}
};

// 부울
template <>
void NText<bool>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %s"), m_name.GetString(), m_data == true ? _T("True") : _T("False") );
}

// 문자열
template <>
void NText<CString>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %s"), m_name.GetString(), m_data.GetString() );
}

// 정수
template <>
void NText<int>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %d"), m_name.GetString(), m_data );
}

// 실수
template <>
void NText<float>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %f"), m_name.GetString(), m_data );
}


// 벡터2
template <>
void NText<NVECTOR2>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f"), m_name.GetString(), m_data.x, m_data.y );
}

// 벡터3
template <>
void NText<NVECTOR3>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f %10f"), m_name.GetString(), m_data.x, m_data.y, m_data.z );
}

// 벡터4
template <>
void NText<NVECTOR4>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f %10f %10"), m_name.GetString(), m_data.x, m_data.y, m_data.z, m_data.w );
}

// 문자열 포인터
template <>
void NText<const TCHAR*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %s"), m_name.GetString(), m_data );
}

// 정수 포인터
template <>
void NText<int*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %d"), m_name.GetString(), *m_data );
}
template <>
void NText<const int*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %d"), m_name.GetString(), *m_data );
}

// 실수 포인터
template <>
void NText<float*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %f"), m_name.GetString(), *m_data );
}
template <>
void NText<const float*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %f"), m_name.GetString(), *m_data );
}

// 벡터2 포인터
template <>
void NText<NVECTOR2*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f"), m_name.GetString(), m_data->x, m_data->y );
}
void NText<const NVECTOR2*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f"), m_name.GetString(), m_data->x, m_data->y );
}

// 벡터3 포인터
template <>
void NText<NVECTOR3*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f %10f"), m_name.GetString(), m_data->x, m_data->y, m_data->z );
}
template <>
void NText<const NVECTOR3*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f %10f"), m_name.GetString(), m_data->x, m_data->y, m_data->z );
}

// 벡터4 포인터
template <>
void NText<NVECTOR4*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f %10f %10f"), m_name.GetString(), m_data->x, m_data->y, m_data->z, m_data->w );
}
template <>
void NText<const NVECTOR4*>::Render(int x, int y, D3DCOLOR color, NFont* pFont)
{
	pFont->Draw( x, y, color, _T("%s : %10f %10f %10f %10f"), m_name.GetString(), m_data->x, m_data->y, m_data->z, m_data->w );
}