#include "ItemMgr.h"
#include "SSynchronize.h"

#include "SLogger.h"


ItemObj::ItemObj()
{
	Init();
}

ItemObj::~ItemObj()
{

}

void ItemObj::Init( int index )
{
	m_vecIndex = index;

	Init();
}

void ItemObj::Init()
{
	m_iId = 0;
	m_Position.Clear();

	m_itemType = -1;
}

//==============================================================

ItemMgr::ItemMgr(void)
{
}

ItemMgr::~ItemMgr(void)
{
	Release();
}

void ItemMgr::Init()
{
	m_IndexQ.Create( Item_Space, 0 );

	m_vecItem.reserve( Item_Space );

	for( int i=0; i<Item_Space; ++i )
	{
		ItemObj* tmpItem = new ItemObj;
		tmpItem->Init(i);

		m_vecItem.push_back(tmpItem);
	}
}

void ItemMgr::Release()
{
	SSynchronize sync( this );

	if( m_vecItem.empty() )
		return;

	for( int i=0; i<Item_Space; ++i )
	{
		delete m_vecItem[i];
	}
	m_vecItem.clear();
}

ItemObj* ItemMgr::GetItemSpace()
{
	SSynchronize sync( this );

	int index = m_IndexQ.GetIndex();
	if( index < 0 )
		return NULL;

	return m_vecItem[index];
}

void ItemMgr::ReturnSpace( ItemObj* item )
{
	SSynchronize sync( this );

	int index = item->GetVecIndex();
	item->Init();

	m_IndexQ.PutIndex( index );
}

ItemObj* ItemMgr::FindItem( int index )
{
	SSynchronize sync( this );

	//0���� �۰� �ʴ밪���� ũ�ų� ������ index����
	if( index < 0 || index >= Item_Space )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("ItemMgr::FindItem()\nIndex�� �̻��մϴ�.\n\n") );
		return NULL;
	}

	return m_vecItem[index];
}
