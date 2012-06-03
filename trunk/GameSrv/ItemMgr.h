#pragma once

#include "SIndexQueue.h"

enum SRV_ITEM_TYPE
{
	SRV_ITEM_TYPE_BOTTLE,
};

const int Item_Space = 50;

class ItemObj
{
private:
	int			m_iId;
	POINT3		m_Position;

	//item ����
	int			m_itemType;

	//�ڽ��� ���� index
	int			m_vecIndex;

public:
	ItemObj();
	~ItemObj();

	void Init();
	void Init( int index );

	void SetIndexId( int session ) { m_iId = session; }
	int GetIndexId() { return m_iId; }

	void SetPosition( float x, float y, float z ) { m_Position.SetElement( x, y, z ); }
	void SetPosition( POINT3 pos ) { m_Position = pos; }
	POINT3 GetPosition() const { return m_Position; }
	float GetPosX() { return m_Position.m_X; }
	float GetPosY() { return m_Position.m_Y; }
	float GetPosZ() { return m_Position.m_Z; }

	inline void SetItemType( int type ) { m_itemType = type; }
	inline int GetItemType() { return m_itemType; }

	inline int GetVecIndex() { return m_vecIndex; }

};

//==============================================================

class ItemMgr : public SSingleton <ItemMgr>, public SServerObj
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

