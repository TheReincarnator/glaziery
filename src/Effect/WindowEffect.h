/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_WINDOWEFFECT_H
#define __GLAZIERY_WINDOWEFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class WindowEffect : public CachableComponentEffect
	{
		public:

			enum FocusingType
			{
				FOCUSING_NEVER, FOCUSING_ALWAYS, FOCUSING_IF_NONE
			};

		private:

			bool bringingToFront;

			bool closing;

			FocusingType focusingType;

			bool maximizing;

			bool restoring;

			bool sendingToBack;

		protected:

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
				DECLARE_DYNAMIC(WindowEffect);
			#endif

		public:

			/**
			 * Creates a new window effect. After creation, add the effect to the Desktop.
			 * @param time The total time of the effect in milli seconds.
			 * If the effect is repetitive, this is the time of one loop.
			 * @param window The window the effect is defined for.
			 * @see Desktop::addEffect()
			 */
			WindowEffect(long time, Window * window);

			FocusingType getFocusingType();

			Window * getWindow();

			bool isBringingToFront();

			bool isClosing();

			bool isMaximizing();

			bool isRestoring();

			bool isSendingToBack();

			void setBringingToFront(bool bringingToFront);

			void setClosing(bool closing);

			void setFocusingType(FocusingType focusingType);

			void setMaximizing(bool maximizing);

			void setRestoring(bool restoring);

			void setSendingToBack(bool sendingToBack);
	};
}


#endif
