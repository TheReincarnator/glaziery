/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


TabbingField::TabbingField()
{
	ASSERTION_COBJECT(this);
	setLikeTitle(false);
}

TabbingField::~TabbingField()
{
	ASSERTION_COBJECT(this);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(TabbingField, Field);
#endif

String TabbingField::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("TabbingField(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}
