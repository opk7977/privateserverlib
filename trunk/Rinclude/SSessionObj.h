#ifndef __SESSION_OBJECT__
#define __SESSION_OBJECT__

#include "SSession.h"

class SSessionObj : public SSession
{
public:
	SSessionObj(void);
	virtual ~SSessionObj(void);

	//�� ������ session class�� ��ӹ޾� ����� class
	//�߻�Ŭ���� �̹Ƿ� �ݵ�� ��ӹ޾� ����ؾ� �Ѵ�
	virtual void OnCreate() = 0;

};


#endif

