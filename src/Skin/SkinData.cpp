/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


SkinData::SkinData()
{
	ASSERTION_COBJECT(this);
	component = NULL;
}

SkinData::~SkinData()
{
	ASSERTION_COBJECT(this);
}

Component * SkinData::getComponent()
{
	ASSERTION_COBJECT(this);
	return component;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SkinData, CObject);
#endif
