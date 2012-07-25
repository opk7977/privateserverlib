#pragma once

#include "SServerStd.h"

class FieldMgr;
class FieldSrvMgr;

class Document : public SSingleton <Document>
{
private:
	friend class SSingleton<Document>;

private:
	//======================================
	// single ton
	//======================================
	FieldMgr*		m_fieldMgr;
	FieldSrvMgr*	m_fieldSrvMgr;

	//======================================
	// Server Data
	//======================================
	char			m_MyIP[16];
	int				m_MyPort;

	//======================================
	// World Data
	//======================================
	int				m_worldTop;
	int				m_worldLeft;
	int				m_worldBottom;
	int				m_worldRight;

private:
	Document(void);
	~Document(void);

public:
	//======================================
	// data �а� �����ϱ�
	//======================================
	BOOL SettingData();
	BOOL LoadFieldData( FILE* fp );

	//======================================
	// IP�� port���
	//======================================
	char*	GetIP()				{ return &m_MyIP[0];	}
	int		GetPort()			{ return m_MyPort;		}

	//======================================
	// World Data���
	//======================================
	int		GetWorldTop	()		{ return m_worldTop;	}
	int		GetWorldLeft()		{ return m_worldLeft;	}
	int		GetWorldBottom()	{ return m_worldBottom;	}
	int		GetWorldRight()		{ return m_worldRight;	}
};

#define GetDocument Document::GetInstance()
