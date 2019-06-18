/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_CACHABLECOMPONENTEFFECT_H
#define __GLAZIERY_CACHABLECOMPONENTEFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class CachableComponentEffect : public ComponentEffect
	{
		private:

			int focusAlphaEnd;

			bool focusAlphaEndSet;

			int focusAlphaStart;

			bool focusAlphaStartSet;

			int nonFocusAlphaEnd;

			bool nonFocusAlphaEndSet;

			int nonFocusAlphaStart;

			bool nonFocusAlphaStartSet;

		protected:

			/**
			 * Performs operations for one frame, advancing the effect by the frame time.
			 * If the remaining time of the effect's total time is less than the
			 * frame time, only the remaining time is passed to this method, and
			 * the end() method is called. If both methods return true, the execute()
			 * method is called again with the remainder of the frame time.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param time This frame's time in milli seconds. You should advance the
			 * effect by this amount.
			 * @return Whether to continue the effect. false will destroy it.
			 * @note This method is called by the Desktop. Do not call it directly.
			 */
			virtual bool executeTimed(DrawingContext & context, long time);

			/**
			 * Performs operations when the total time of the effect has elapsed.
			 * The method can decide whether to restart the effect (loop).
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @return Whether to restart the effect. In this case, the onStart()
			 * and execute() methods are called again, and the time of the execute()
			 * method is the remainder of the time (i.e. the frame time minus the
			 * time passed to the execute() method of the previous loop.
			 * @note This method is called by the Desktop. Do not call it directly.
			 * To cancel the effect, use cancel().
			 */
			virtual bool onEnd(DrawingContext & context);

			/**
			 * Performs operations when the effect starts or restarts.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @return Whether to continue the effect. false will destroy it.
			 * @note This method is called by the Desktop. Do not call it directly.
			 */
			virtual bool onStart(DrawingContext & context);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(CachableComponentEffect);
			#endif

		public:

			/**
			 * Creates a new cachable component effect. After creation, add the effect to the Desktop.
			 * @param time The total time of the effect in milli seconds.
			 * If the effect is repetitive, this is the time of one loop.
			 * @param cachableComponent The cachable component the effect is defined for.
			 * @see Desktop::addEffect()
			 */
			CachableComponentEffect(long time, CachableComponent * cachableComponent);

			int getFocusAlphaDelta();

			int getNonFocusAlphaDelta();

			CachableComponent * getCachableComponent();

			void setFocusAlphaEnd(unsigned char focusAlphaEnd);

			void setFocusAlphaStart(unsigned char focusAlphaStart);

			void setNonFocusAlphaEnd(unsigned char nonFocusAlphaEnd);

			void setNonFocusAlphaStart(unsigned char nonFocusAlphaStart);
	};
}


#endif
