#ifndef __INDEX_LIST__
#define __INDEX_LIST__

#include "SServerobj.h"

class SIndexList : public SServerObj
{
private:
	std::list<int>						m_List;

public:
	typedef std::list<int>::iterator	Position;

public:
	SIndexList(void);
	virtual ~SIndexList(void);

	//ÇÔ¼ö
	virtual	void		Clear();
	virtual Position	HeadPosiont();
	virtual BOOL		IsEnd( Position pos );
	virtual int			Size();
	virtual void		PushTail( int n );
	virtual void		RemoveItem( int n );
};


#endif


