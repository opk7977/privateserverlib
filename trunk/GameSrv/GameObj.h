#pragma once

#include "SServerObj.h"

struct POINT3
{
	float m_X;
	float m_Y;
	float m_Z;

	POINT3() {}
	POINT3( float x, float y, float z )
	{
		m_X = x;
		m_Y = y;
		m_Z = z;
	}
	~POINT3() {}

	void Clear()
	{
		m_X = m_Y = m_Z = 0.f;
	}

	void SetElement( float x, float y, float z )
	{
		m_X = x;
		m_Y = y;
		m_Z = z;
	}
};

class GameObj : public SServerObj
{
private:
	int			m_iId;
	POINT3		m_Position;


public:
	GameObj(void);
	virtual ~GameObj(void);

	virtual void Init();

	void SetIndexId( int session ) { m_iId = session; }
	int GetIndexId() { return m_iId; }

	void SetPosition( float x, float y, float z ) { m_Position.SetElement( x, y, z ); }
	void SetPosition( POINT3 pos ) { m_Position = pos; }
	POINT3 GetPosition() { return m_Position; }
	float GetPosX() { return m_Position.m_X; }
	float GetPosY() { return m_Position.m_Y; }
	float GetPosZ() { return m_Position.m_Z; }
};
