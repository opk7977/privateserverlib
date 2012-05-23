#pragma once

//��ť���� ����///
#include "SServerObj.h"

//typename�� �ٿ��� �Ѵ�.
//�����Ϸ��� std::list<T>::iterator�� �ڷ����̶�� ���� ��
//�����Ǵ� Ŭ������ ���������� �޸� �Ҵ� ���� �۾��� �Ͼ�� �ʴ´�.
//������ �ı��� �ʿ��� ��쿡�� �ܺο��� �۾��� ��� �Ѵ�
#define SListIter	typename std::list<T>::iterator

template <typename T>
class SList : public SServerObj
{
private:
	int							m_itemCount;

/*protected:*/
	std::list<T>				m_list;

public:
	SList();
	virtual ~SList();

	void		Clear();

	void		AddItem( T item );
	T			DelItem( T item );
	BOOL		IsEmpty();
	int			GetItemCount();

	SListIter	GetHeader();
	BOOL		IsEnd( SListIter iter );
};

template <typename T>
SList<T>::SList()
{
}

template <typename T>
SList<T>::~SList()
{
}

template <typename T>
void SList<T>::Clear()
{
	m_list.clear();
	m_itemCount = 0;
}

template <typename T>
void SList<T>::AddItem( T item )
{
	m_list.push_back( item );
	++m_itemCount;
}

template <typename T>
T SList<T>::DelItem( T item )
{
	std::list<T>::iterator iter = m_list.begin();
	for( ; iter != m_list.end(); ++iter )
	{
		if( *iter == item )
		{
			T tmp = *iter;
			m_list.erase( iter );
			--m_itemCount;
			return tmp;
		}
	}
	return 0;
}

template <typename T>
BOOL SList<T>::IsEmpty()
{
	if( m_list.empty() )
		return TRUE;

	return FALSE;
}

template <typename T>
int SList<T>::GetItemCount()
{
	return m_itemCount;
}

template <typename T>
SListIter SList<T>::GetHeader()
{
	return m_list.begin();
}

template <typename T>
BOOL SList<T>::IsEnd( SListIter iter )
{
	return (m_list.end() == iter);
}