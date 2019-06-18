/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Widget::Widget()
{
	ASSERTION_COBJECT(this);
	component = NULL;
}

Widget::~Widget()
{
	ASSERTION_COBJECT(this);
}

Vector Widget::getOrigin()
{
	ASSERTION_COBJECT(this);
	return component->getAbsolutePosition();
}

Component * Widget::getParent()
{
	ASSERTION_COBJECT(this);
	return component;
}

PointerType * Widget::getPointerType(Vector position)
{
	ASSERTION_COBJECT(this);
	return NULL;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Widget, EventTarget);
#endif

String Widget::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Widget@%lX", this);
	return string;
}
