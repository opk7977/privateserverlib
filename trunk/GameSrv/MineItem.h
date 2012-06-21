#pragma once

#include "SServerObj.h"

#define BOOM_TIME 0.5

//���� �浹 �ݰ�
#define MINE_COLLISION_ROUND	10
//���� ���� �ݰ�
#define MINE_BOOM_COL_ROUNT		50

//������
#define MINE_BOOM_DAMEGE		70

class MineItem : public SServerObj
{
private:
	//�̹� ����Ѿָ� FALSE
	BOOL		m_isUseable;

 	//���� �غ���?
 	BOOL		m_isRun;

	//Ȱ�� ITEM?
/*	BOOL		m_isVisible;*/

	int			m_masterSessionID;
	int			m_masterTeam;
	POINT3		m_pos;

	//����(?)�ǰ� ����������� �ð�, �⺻ 0.5
	float		m_boomTime;

	//���� ���� ������
	int			m_vecIndex;

public:
	MineItem(void);
	~MineItem(void);

	void Init( int index );
	void Init();

	void Reset();

	inline int GetSessionID() { return m_masterSessionID; }

	int GetVecIndex() { return m_vecIndex; }
	void SetMineSpace( int sessionId, int team );
	//���ڸ� ��ġ, ��ġ�� �����ϸ� FALSE�� return
	BOOL SetMine( float posX, float posY, float posZ );
	inline float GetX() { return m_pos.m_X; }
	inline float GetY() { return m_pos.m_Y; }
	inline float GetZ() { return m_pos.m_Z; }

	//���� �غ� ��?
	inline BOOL IsRun() { return m_isRun; }
	inline void SetRun() { m_isRun = TRUE; }

// 	//�̹� �� ������?
// 	inline BOOL IsVisible() { return m_isVisible; }
// 	//��� �Ϸ�
// 	inline void SetUesd() { m_isVisible = FALSE; }
	
	//���� ������ �Ǵ� ��������?
	//������ �Ǹ� TRUE�� return
	BOOL IsBoom();

	//ĳ���Ϳ��� ���� �浹üũ
	BOOL IsCollision( float posX, float posZ );

	//���������� �ִ� ���ڵ��� �ð��� ����
	void DownTimer( float ElapsedTime );
	//������ ����
	void Boom();

	//ĳ���Ϳ� ���߹����� �浹üũ
	//return �Ǵ� ���� damege )
	int IsBoomCollision( float posX, float posZ );
	int GetDamege( float posX, float posZ );
};
