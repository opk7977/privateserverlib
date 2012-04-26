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
	//item ����
	int			m_itemType;

	//�ڽ��� ���� index
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
	//item����
	std::vector<ItemObj*>		m_vecItem;
	//���������� index
	SIndexQueue					m_IndexQ;

private:
	ItemMgr(void);
	~ItemMgr(void);

public:
	//������ ����
	void Init();
	void Release();

	//item���� �Ҵ�
	ItemObj* GetItemSpace();
	//item���� �ݳ�
	void ReturnSpace( ItemObj* item );
	//�ش� index�� ��Ī�� item�� return
	ItemObj* FindItem( int index );
};

#define GetItemMgr ItemMgr::GetInstance()

