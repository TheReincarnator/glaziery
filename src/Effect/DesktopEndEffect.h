/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DESKTOPENDEFFECT_H
#define __GLAZIERY_DESKTOPENDEFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class DesktopEndEffect : public Effect
	{
		protected:

			/**
			 * Performs operations for one frame, advancing the effect by the frame time.
			 * Afterwards, the effect issues whether it is complete now.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param time This frame's time in milli seconds. You should advance the
			 * effect by this amount.
			 * @return Whether to continue the effect. false will destroy it.
			 * @note This method is called by the Desktop. Do not call it directly.
			 */
			virtual bool execute(DrawingContext & context, long time);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DesktopEndEffect);
			#endif
	};
}


#endif
