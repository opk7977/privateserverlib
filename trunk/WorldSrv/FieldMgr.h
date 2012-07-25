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
	// Visible Field Space(���� ���̴� ����)
	//======================================
	int		m_visibleTop;
	int		m_visibleLeft;
	int		m_visibleBottom;
	int		m_visibleRight;

public:
	FieldObj(void);
	~FieldObj(void);

	//--------------------------------------
	// �ʱ�ȭ & ����
	//--------------------------------------
	BOOL	Init();
	void	Release();

	//--------------------------------------
	// Set�Լ�
	//--------------------------------------
	void	SetFieldData( int _id, TCHAR* _name, int _top, int _left, int _bottom, int _right, int _extra );

	void	SetID( int _id );
	void	SetName( TCHAR* _name );
	void	SetField( int _top, int _left, int _bottom, int _right, int _extra );

	//--------------------------------------
	// Get�Լ���
	//--------------------------------------
	int		GetID();
	TCHAR*	GetName();
	int		GetTop();
	int		GetLeft();
	int		GetBottom();
	int		GetRight();
	int		GetExtra();

	//--------------------------------------
	// �ʵ��� �⺻ �����͸� ����(?)
	//--------------------------------------
	void	PackageFieldData( SPacket &packet );

	//--------------------------------------
	// ĳ���Ͱ� �ش� �ʵ� ������
	// ���� �ϴ����� üũ�ϰ� �����ϸ� TRUE
	//--------------------------------------
	BOOL	Is_InField( int _x, int _y );
	//--------------------------------------
	// ĳ���Ͱ� �ش� �ʵ尡 ��Ÿ���� extra������
	// ���� �ϴ����� üũ�ϰ� �����ϸ� TRUE
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
	// �ʱ�ȭ & ����
	//--------------------------------------
	BOOL		Init( int _count );
	void		Release();

	//--------------------------------------
	// �ش� ��ȣ�� Field�� ã�� �ش�
	//--------------------------------------
	FieldObj*	GetField( int _id );

	//--------------------------------------
	// �ش� ��ȣ�� field�� ������ �־� �ش�
	//--------------------------------------
	BOOL		SetDataForField( int _id, TCHAR* _name, int _top, int _left, int _bottom, int _right, int _extra );

public:

};

#define GetFieldMgr FieldMgr::GetInstance()

