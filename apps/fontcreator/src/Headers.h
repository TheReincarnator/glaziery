#ifndef __HEADERS_H
#define __HEADERS_H


// Standard headers
#include <math.h>

// MFC headers
#include <afx.h>
#include <afxcmn.h>
#include <afxext.h>
#include <afxpriv.h>
#include <afxsock.h>
#include <afxwin.h>

// GDI headers
#include <gdiplus.h>
using namespace Gdiplus;


// ToolBox
#include <ToolBox/Headers.h>

// MFCTriExt
#include <MFCTriExt/Headers.h>

#ifdef D3D_OVERLOADS
	// Choosing DirectX versions
	#define DIRECTINPUT_VERSION 0x0800

	// DirectX headers, level 0
	#include <basetsd.h>

	// DirectX headers, level 1
	#include <d3d9.h>
	#include <d3d9types.h>
	#include <d3dx9core.h>
	#include <d3dx9math.h>
	#include <dinput.h>
#endif

// Class declarations
class MainDialog;
class FontCreator;


#include "../resources/Resources.h"

#include "MainDialog.h"
#include "FontCreator.h"


#endif
