#pragma once

#include "SIndexQueue.h"

class MineItem;

class SLogger;
class DataLeader;

enum SRV_ITEM_TYPE
{
	SRV_ITEM_TYPE_MINE,
	SRV_ITEM_TYPE_BOTTLE,
};

class ItemMgr : public SSingleton <ItemMgr>, public SServerObj
{
private:
	friend class SSingleton<ItemMgr>;

private:
	//======================================
	// singleTon객체들
	//======================================
	SLogger*					m_logger;
	DataLeader*					m_document;

private:
	//======================================
	// MinItem
	//--------------------------------------
	//Item공간
	std::vector<MineItem*>		m_vecMine;
	//공간관리용 index
	SIndexQueue					m_IQMine;
	//======================================

private:
	ItemMgr(void);
	~ItemMgr(void);

public:
	//생성과 해제
	void Init();
	void Release();

	//======================================
	// MinItem
	//--------------------------------------
	//item공간 할당
	MineItem* GetMineItemSpace();
	//item공간 반납
	void ReturnMineSpace( MineItem* item );
	//해당 index에 매칭된 item을 return
	MineItem* FindMineItem( int index );
	//======================================
};

#define GetItemMgr ItemMgr::GetInstance()

