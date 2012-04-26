#include "ItemMgr.h"

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
	GameObj::Init();

	m_vecIndex = index;
	m_itemType = -1;
}

void ItemObj::Init()
{
	GameObj::Init();

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
	for( int i=0; i<Item_Space; ++i )
	{
		delete m_vecItem[i];
	}
	m_vecItem.clear();
}

ItemObj* ItemMgr::GetItemSpace()
{
	int index = m_IndexQ.GetIndex();
	if( index < 0 )
		return NULL;

	return m_vecItem[index];
}

void ItemMgr::ReturnSpace( ItemObj* item )
{
	int index = item->GetVecIndex();
	item->Init();

	m_IndexQ.PutIndex( index );
}

ItemObj* ItemMgr::FindItem( int index )
{
	//0보다 작고 초대값보다 크거나 같으면 index에러
	if( index < 0 || index >= Item_Space )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("ItemMgr::FindItem()\nIndex가 이상합니다.\n\n") );
		return NULL;
	}

	return m_vecItem[index];
}
