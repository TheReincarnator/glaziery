/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


SharedObject::SharedObject()
{
	ASSERTION_COBJECT(this);
	references = 0;
}

SharedObject::~SharedObject()
{
	ASSERTION_COBJECT(this);
}

void SharedObject::addReference()
{
	ASSERTION_COBJECT(this);
	references++;
}

int SharedObject::getReferences()
{
	ASSERTION_COBJECT(this);
	return references;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SharedObject, GlazieryObject);
#endif

void SharedObject::release()
{
	ASSERTION_COBJECT(this);

	if (--references <= 0)
		delete this;
}
