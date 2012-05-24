#pragma once

//용큐오빠 감사///
#include "SServerObj.h"

//typename을 붙여야 한다.
//컴파일러는 std::list<T>::iterator가 자료형이라는 것을 모름
//제공되는 클래스는 내부적으로 메모리 할당 해제 작업이 일어나지 않는다.
//생성과 파괴가 필요한 경우에는 외부에서 작업해 줘야 한다
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