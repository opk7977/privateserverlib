#include "SIndexList.h"

SIndexList::SIndexList(void)
{
}

SIndexList::~SIndexList(void)
{
	Clear();
}

void SIndexList::Clear()
{
	m_List.clear();
}

SIndexList::Position SIndexList::HeadPosiont()
{
	return m_List.begin();
}

BOOL SIndexList::IsEnd( Position pos )
{
	BOOL tmp = (pos == m_List.end());

	return tmp;
}

int SIndexList::Size()
{
	return m_List.size();
}

void SIndexList::PushTail( int n )
{
	m_List.push_back(n);
}

void SIndexList::RemoveItem( int n )
{
	m_List.remove(n);
}
