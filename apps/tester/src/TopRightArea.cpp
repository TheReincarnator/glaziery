/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


void TopRightArea::initialize()
{
	ASSERTION_COBJECT(this);

	TextField * textPane3Field;
	if ((textPane3Field = new TextField) == NULL)
		throw EOUTOFMEMORY;
	setContentField(textPane3Field);

	textPane3Field->setMultiLine(true);
}
