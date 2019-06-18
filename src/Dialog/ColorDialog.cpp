/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ColorDialog::ColorDialog()
{
	ASSERTION_COBJECT(this);
}

ColorDialog::~ColorDialog()
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ColorDialog, Dialog);
#endif
