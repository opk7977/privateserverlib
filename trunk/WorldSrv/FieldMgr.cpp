#include "FieldMgr.h"

#include "SSynchronize.h"
#include "SPacket.h"

#include "Document.h"


//======================================
// FieldObj
//======================================

//--------------------------------------
// singleTon
//--------------------------------------
Document*	FieldObj::m_document	= &GetDocument;

//--------------------------------------
// function
//--------------------------------------
FieldObj::FieldObj( void )
{
	
}

FieldObj::~FieldObj( void )
{
	
}

BOOL FieldObj::Init()
{
	SSynchronize sync( this );

	ZeroMemory( m_fieldName, 16 );

	m_fieldID		= 0;
	m_fieldTop		= 0;
	m_fieldLeft		= 0;
	m_fieldBottom	= 0;
	m_fieldRight	= 0;

	m_fieldExtra	= 0;

	return TRUE;
}

void FieldObj::Release()
{
	
}

void FieldObj::SetFieldData( int _id, TCHAR* _name, int _top, int _left, int _bottom, int _right, int _extra )
{
	SSynchronize sync( this );

	_tcsncpy_s( m_fieldName, _name, _tcslen(_name) );

	m_fieldID		= _id;
	m_fieldTop		= _top;
	m_fieldLeft		= _left;
	m_fieldBottom	= _bottom;
	m_fieldRight	= _right;

	m_fieldExtra	= _extra;
}

void FieldObj::SetID( int _id )
{
	SSynchronize sync( this );

	m_fieldID		= _id;
}

void FieldObj::SetName( TCHAR* _name )
{
	SSynchronize sync( this );

	_tcsncpy_s( m_fieldName, _name, _tcslen(_name) );
}

void FieldObj::SetField( int _top, int _left, int _bottom, int _right, int _extra )
{
	SSynchronize sync( this );

	m_fieldTop		= _top;
	m_fieldLeft		= _left;
	m_fieldBottom	= _bottom;
	m_fieldRight	= _right;

	m_fieldExtra	= _extra;

	//======================================
	// extra를 포함한 가시공간
	//======================================
	//--------------------------------------
	//top
	m_visibleTop = m_fieldTop - m_fieldExtra;
	if( m_visibleTop < m_document->GetWorldTop() )
		m_visibleTop = m_document->GetWorldTop();
	//--------------------------------------
	//left
	m_visibleLeft = m_fieldLeft - m_fieldExtra;
	if( m_visibleLeft < m_document->GetWorldLeft() )
		m_visibleLeft = m_document->GetWorldLeft();
	//--------------------------------------
	//bottom
	m_visibleBottom = m_fieldBottom + m_fieldExtra;
	if( m_visibleBottom > m_document->GetWorldBottom() )
		m_visibleBottom = m_document->GetWorldBottom();
	//--------------------------------------
	//right
	m_visibleRight = m_fieldRight + m_fieldExtra;
	if( m_visibleRight > m_document->GetWorldRight() )
		m_visibleRight = m_document->GetWorldRight();
}

int FieldObj::GetID()
{
	return m_fieldID;
}

TCHAR* FieldObj::GetName()
{
	return m_fieldName;
}

int FieldObj::GetTop()
{
	return m_fieldTop;
}

int FieldObj::GetLeft()
{
	return m_fieldLeft;
}

int FieldObj::GetBottom()
{
	return m_fieldBottom;
}

int FieldObj::GetRight()
{
	return m_fieldRight;
}

int FieldObj::GetExtra()
{
	return m_fieldExtra;
}

void FieldObj::PackageFieldData( SPacket &packet )
{
	SSynchronize sync( this );

	packet << m_fieldID;
	int size = _tcslen( m_fieldName )*sizeof( TCHAR );
	packet << size;
	packet.PutData( m_fieldName, size );
	packet << m_fieldTop;
	packet << m_fieldLeft;
	packet << m_fieldBottom;
	packet << m_fieldRight;
	packet << m_fieldExtra;
}

BOOL FieldObj::Is_InField( int _x, int _y )
{
	if( _y < m_fieldTop )
		return FALSE;
	if( _x < m_fieldLeft )
		return FALSE;
	if( _y > m_fieldBottom )
		return FALSE;
	if( _x > m_fieldRight )
		return FALSE;

	//필드안에 포함되어 있음
	return TRUE;
}

BOOL FieldObj::Is_InExtraField( int _x, int _y )
{
	if( _y < m_visibleTop )
		return FALSE;
	if( _x < m_visibleLeft )
		return FALSE;
	if( _y > m_visibleBottom )
		return FALSE;
	if( _x > m_visibleRight )
		return FALSE;

	//보이는 공간에 있음
	return TRUE;
}

//======================================
// FieldMgr
//======================================
FieldMgr::FieldMgr(void)
{
}

FieldMgr::~FieldMgr(void)
{
}

BOOL FieldMgr::Init( int _count )
{
	m_fieldCount = _count;

	FieldObj* tmpField = 0;
	for( int i=1; i<=m_fieldCount; ++i )
	{
		tmpField = new FieldObj;
		tmpField->Init();

		m_mapField[i] = tmpField;
	}

	return TRUE;
}

void FieldMgr::Release()
{
	for( int i=1; i<=m_fieldCount; ++i )
	{
		delete m_mapField[i];;
		m_mapField[i] = 0;
	}

	m_mapField.RemoveAll();
}

FieldObj* FieldMgr::GetField( int _id )
{
	FieldObj* tmpField = NULL;
	if( !m_mapField.Lookup( _id, tmpField ) )
		//없으면 NULL
		return NULL;

	//찾았으면 정상적으로 반환
	return tmpField;
}

BOOL FieldMgr::SetDataForField( int _id, TCHAR* _name, int _top, int _left, int _bottom, int _right, int _extra )
{
	FieldObj* tmpField = GetField( _id );
	if( tmpField == NULL )
		return FALSE;

	tmpField->SetFieldData( _id, _name, _top, _left, _bottom, _right, _extra );

	return TRUE;
}
