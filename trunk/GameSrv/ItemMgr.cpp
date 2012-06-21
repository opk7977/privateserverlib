#include "ItemMgr.h"
#include "SSynchronize.h"
#include "SLogger.h"
#include "DataLeader.h"
#include "MineItem.h"


ItemMgr::ItemMgr(void)
{
	m_logger	= &GetLogger;
	m_document	= &GetDocument;
}

ItemMgr::~ItemMgr(void)
{
	Release();
}

void ItemMgr::Init()
{
	//==============================================================
	// MineItem
	//--------------------------------------------------------------
	{
		SSynchronize sync( &m_IQMine );

		m_IQMine.Create( m_document->SessionCount, 0 );
		m_vecMine.reserve( m_document->SessionCount );
		for( int i=0; i<m_document->SessionCount; ++i )
		{
			MineItem* tmpItem = new MineItem;
			tmpItem->Init(i);
			m_vecMine.push_back(tmpItem);
		}
	}
	//==============================================================
}

void ItemMgr::Release()
{
	{
		SSynchronize sync( &m_IQMine );
		if( m_vecMine.empty() )
			return;

		for( int i=0; i<m_document->SessionCount; ++i )
		{
			delete m_vecMine[i];
		}
		m_vecMine.clear();
	}
}

MineItem* ItemMgr::GetMineItemSpace()
{
	SSynchronize sync( &m_IQMine );

	int index = m_IQMine.GetIndex();
	if( index < 0 )
		return NULL;

	return m_vecMine[index];
}

void ItemMgr::ReturnMineSpace( MineItem* item )
{
	SSynchronize sync( &m_IQMine );

	item->Init();
	m_IQMine.PutIndex( item->GetVecIndex() );
}

MineItem* ItemMgr::FindMineItem( int index )
{
	//¿Œµ¶Ω∫ »Æ¿Œ
	if( index < 0 || index > m_document->SessionCount )
		return NULL;

	SSynchronize sync( &m_IQMine );

	return m_vecMine[index];
}

