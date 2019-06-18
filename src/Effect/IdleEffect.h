/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_IDLEEFFECT_H
#define __GLAZIERY_IDLEEFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class IdleEffect : public TimedEffect
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(IdleEffect);
			#endif

		public:

			/**
			 * Creates a new idle effect. After creation, add the effect to the Desktop.
			 * @param time The total time of the effect in milli seconds.
			 * If the effect is repetitive, this is the time of one loop.
			 * @see Desktop::addEffect()
			 */
			IdleEffect(long time = 0);
	};
}


#endif
