#pragma once

#include "SServerObj.h"

#define FREEKEY	0x10
#define PULLKEY	0x20
#define PUSHKEY	0x40
#define HOLDKEY	0x80

class SInput
{
private:
	// �Է°���	
	BYTE	byKey[256];

public:
	SInput(void);
	~SInput(void);

	// Ű ���� ����.
	// ���ϰ��� true �ش� Ű�� ���ȴ�.
	//			false �ش� Ű�� �ȴ��ȴ�.
	void KeyboardProcess()
	{
		static	BYTE	byOldKey[256] = { 0 };

		if(GetKeyboardState(byKey))	
		{
			// DOWN �� UP���
			for(int i = 0; i < 256; i++)
			{
				if(byKey[i] & HOLDKEY)
				{
					if(!byOldKey[i])
					{
						byOldKey[i] = 1;
						byKey[i] |= PUSHKEY;
					}
				}
				else
				{
					if(byOldKey[i])
					{
						byOldKey[i] = 0;
						byKey[i] = PULLKEY;
					}
				}
			}
		}
	}

	inline bool GetKeyDown( int KeyCode )
	{
		//return (byKey[KeyCode] & PUSHKEY);
		if( (byKey[KeyCode] & PUSHKEY) )
			return true;

		return false;
	}

	inline bool GetKey( int KeyCode ) const
	{
		//return (byKey[KeyCode] & HOLDKEY);
		if( (byKey[KeyCode] & HOLDKEY) )
			return true;

		return false;
	}

	inline bool GetKeyUp( int KeyCode ) const
	{
		//return (byKey[KeyCode] & PULLKEY);
		if( (byKey[KeyCode] & PULLKEY) )
			return true;

		return false;
	}
};