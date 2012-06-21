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
	// singleTon��ü��
	//======================================
	SLogger*					m_logger;
	DataLeader*					m_document;

private:
	//======================================
	// MinItem
	//--------------------------------------
	//Item����
	std::vector<MineItem*>		m_vecMine;
	//���������� index
	SIndexQueue					m_IQMine;
	//======================================

private:
	ItemMgr(void);
	~ItemMgr(void);

public:
	//������ ����
	void Init();
	void Release();

	//======================================
	// MinItem
	//--------------------------------------
	//item���� �Ҵ�
	MineItem* GetMineItemSpace();
	//item���� �ݳ�
	void ReturnMineSpace( MineItem* item );
	//�ش� index�� ��Ī�� item�� return
	MineItem* FindMineItem( int index );
	//======================================
};

#define GetItemMgr ItemMgr::GetInstance()

