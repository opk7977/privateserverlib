#pragma once

#include <windows.h>
#include <tchar.h>
#include <cassert>
#include <atlstr.h>
#include <atlcoll.h>

#ifdef _DEBUG

#pragma comment(lib, "d3dx9d.lib")
#define D3D_DEBUG_INFO

#else

#pragma comment(lib, "d3dx9.lib")

#endif // _DEBUG

#pragma comment( lib, "dxguid.lib" )

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>

#include "NL_SafeMethod.inl"

#include "NL_D3DFormat.h"
#include "NL_D3DType.h"
