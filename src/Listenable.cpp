/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Listenable::Listenable()
{
	ASSERTION_COBJECT(this);
}

Listenable::~Listenable()
{
	ASSERTION_COBJECT(this);

	while (!listeners.IsEmpty())
		listeners.UnlinkFirst()->release();
}

void Listenable::appendListener(Listenable::Listener * listener)
{
	ASSERTION_COBJECT(this);

	listeners.Append(listener);
	listener->addReference();
}

const ArrayList<Listenable::Listener> & Listenable::getListeners()
{
	ASSERTION_COBJECT(this);
	return listeners;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Listenable, GlazieryObject);
#endif

void Listenable::insertListener(int position, Listenable::Listener * listener)
{
	ASSERTION_COBJECT(this);

	listeners.Insert(listener, position);
	listener->addReference();
}

void Listenable::prependListener(Listenable::Listener * listener)
{
	ASSERTION_COBJECT(this);

	listeners.Prepend(listener);
	listener->addReference();
}

void Listenable::removeListener(Listenable::Listener * listener)
{
	ASSERTION_COBJECT(this);

	if (listeners.Unlink(listener) != NULL)
		listener->release();
}


Listenable::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}
