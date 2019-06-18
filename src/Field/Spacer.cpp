/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Spacer::Spacer()
{
	ASSERTION_COBJECT(this);

	setMinimumSize(Vector());
	resize(Vector());
}

void Spacer::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	// By definition, spacers do not draw.
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Spacer, Field);
#endif

String Spacer::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Spacer(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}
