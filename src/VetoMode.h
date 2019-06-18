/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_VETOMODE_H
#define __GLAZIERY_VETOMODE_H


namespace glaziery
{
	/**
	 * The veto mode, indicating whether the implementation of a listener
	 * or similar issues to cancel the current operation.
	 */
	enum VetoMode
	{
		/**
		 * Vetoable event methods return this value to indicate
		 * that they do not want to cancel the event.
		 */
		NO_VETO,

		/**
		 * Vetoable event methods return this value to indicate
		 * that they want to cancel the event.
		 */
		VETO
	};
}


#endif
