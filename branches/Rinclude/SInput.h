#pragma once

#include "SServerObj.h"

#define FREEKEY	0x10
#define PULLKEY	0x20
#define PUSHKEY	0x40
#define HOLDKEY	0x80

class SInput
{
private:
	// 입력감지	
	BYTE	byKey[256];

public:
	SInput(void);
	~SInput(void);

	// 키 값을 전달.
	// 리턴값이 true 해당 키가 눌렸다.
	//			false 해당 키가 안눌렸다.
	void KeyboardProcess()
	{
		static	BYTE	byOldKey[256] = { 0 };

		if(GetKeyboardState(byKey))	
		{
			// DOWN 및 UP계산
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