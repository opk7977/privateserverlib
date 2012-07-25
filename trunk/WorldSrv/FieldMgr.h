#pragma once

#include "SServerObj.h"

class SPacket;
class Document;

//======================================
// FieldObj
//======================================
class FieldObj : public SServerObj
{
private:
	//======================================
	// single ton
	//======================================
	static Document*		m_document;

private:
	//======================================
	// Field ID
	//======================================
	int		m_fieldID;
	TCHAR	m_fieldName[16];

	//======================================
	// Field
	//======================================
	int		m_fieldTop;
	int		m_fieldLeft;
	int		m_fieldBottom;
	int		m_fieldRight;

	int		m_fieldExtra;

	//======================================
	// Visible Field Space(눈에 보이는 공간)
	//======================================
	int		m_visibleTop;
	int		m_visibleLeft;
	int		m_visibleBottom;
	int		m_visibleRight;

public:
	FieldObj(void);
	~FieldObj(void);

	//--------------------------------------
	// 초기화 & 해제
	//--------------------------------------
	BOOL	Init();
	void	Release();

	//--------------------------------------
	// Set함수
	//--------------------------------------
	void	SetFieldData( int _id, TCHAR* _name, int _top, int _left, int _bottom, int _right, int _extra );

	void	SetID( int _id );
	void	SetName( TCHAR* _name );
	void	SetField( int _top, int _left, int _bottom, int _right, int _extra );

	//--------------------------------------
	// Get함수들
	//--------------------------------------
	int		GetID();
	TCHAR*	GetName();
	int		GetTop();
	int		GetLeft();
	int		GetBottom();
	int		GetRight();
	int		GetExtra();

	//--------------------------------------
	// 필드의 기본 대이터를 포장(?)
	//--------------------------------------
	void	PackageFieldData( SPacket &packet );

	//--------------------------------------
	// 캐릭터가 해당 필드 공간에
	// 존재 하는지를 체크하고 존재하면 TRUE
	//--------------------------------------
	BOOL	Is_InField( int _x, int _y );
	//--------------------------------------
	// 캐릭터가 해당 필드가 나타내는 extra공간에
	// 존재 하는지를 체크하고 존재하면 TRUE
	//--------------------------------------
	BOOL	Is_InExtraField( int _x, int _y );
};


//======================================
// FieldMgr
//======================================
class FieldMgr : public SSingleton <FieldMgr>
{
private:
	friend class SSingleton<FieldMgr>;

private:
	int								m_fieldCount;
	ATL::CAtlMap<int, FieldObj*>	m_mapField;

private:
	FieldMgr(void);
	~FieldMgr(void);

public:
	//--------------------------------------
	// 초기화 & 해제
	//--------------------------------------
	BOOL		Init( int _count );
	void		Release();

	//--------------------------------------
	// 해당 번호의 Field를 찾아 준다
	//--------------------------------------
	FieldObj*	GetField( int _id );

	//--------------------------------------
	// 해당 번호의 field에 정보를 넣어 준다
	//--------------------------------------
	BOOL		SetDataForField( int _id, TCHAR* _name, int _top, int _left, int _bottom, int _right, int _extra );

public:

};

#define GetFieldMgr FieldMgr::GetInstance()

