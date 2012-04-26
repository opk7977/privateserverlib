#pragma once

#include "SIndexQueue.h"
#include "GameObj.h"

enum SRV_ITEM_TYPE
{
	SRV_ITEM_TYPE_BOTTLE,
};

const int Item_Space = 50;


class ItemObj : public GameObj
{
private:
	//item 종류
	int			m_itemType;

	//자신의 백터 index
	int			m_vecIndex;

public:
	ItemObj();
	~ItemObj();

	void Init();
	void Init( int index );

	inline void SetItemType( int type ) { m_itemType = type; }
	inline int GetItemType() { return m_itemType; }

	inline int GetVecIndex() { return m_vecIndex; }

};

class ItemMgr : public SSingleton <ItemMgr>
{
private:
	friend class SSingleton<ItemMgr>;

private:
	//item공간
	std::vector<ItemObj*>		m_vecItem;
	//공간관리용 index
	SIndexQueue					m_IndexQ;

private:
	ItemMgr(void);
	~ItemMgr(void);

public:
	//생성과 해제
	void Init();
	void Release();

	//item공간 할당
	ItemObj* GetItemSpace();
	//item공간 반납
	void ReturnSpace( ItemObj* item );
	//해당 index에 매칭된 item을 return
	ItemObj* FindItem( int index );
};

#define GetItemMgr ItemMgr::GetInstance()

