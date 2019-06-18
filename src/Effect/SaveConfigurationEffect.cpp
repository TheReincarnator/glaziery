/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SaveConfigurationEffect, CObject);
#endif

bool SaveConfigurationEffect::execute(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->saveConfig();
	return false;
}
