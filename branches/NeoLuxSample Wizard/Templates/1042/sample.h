#pragma once

#include "NL_[!output SUPER_CLASS_ID].h"

//===================================================================
//  NAME	  : [!output PROJECT_NAME]
//  COMMENTS  : -
//===================================================================
class [!output PROJECT_NAME] : public [!output SUPER_CLASS_ID]
{
	//==============================
	//	Sample Data
	//==============================

public:
	//==============================
	//	Constructor & Destructor
	//==============================
	[!output PROJECT_NAME](void);
	~[!output PROJECT_NAME](void);

	//==============================
	//	Function
	//==============================
	bool Init(void);
	bool Frame(void);
	bool Render(void);
	bool Release(void);
};
