#pragma once

#include "SServerobj.h"

class STime : public SServerObj
{
private:
	BOOL		m_bUseQPF;
	float		m_fElapsedTime;
	LONGLONG	m_llQPFTicksPerSec;
	LONGLONG	m_llLastElapsedTime;

public:
	STime(void);
	~STime(void);

	inline float GetElapsedTime() const
	{ return m_fElapsedTime; };

	//���� �ʱ�ȭ. ��ü �������� �ѹ��� ȣ��������.
	void Init();

	//�ð� ������ ����Ű�Ƿ�, �������� �ѹ��� ȣ��������.(����ð��� ����ϰ� �ȴ�)
	void ProcessTime();
};


