/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_EFFECTSEQUENCE_H
#define __GLAZIERY_EFFECTSEQUENCE_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class EffectSequence : public Effect
	{
		private:

			ArrayList<Effect> effects;

		protected:

			bool firstEffectStarted;

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

			/**
			 * Performs operations when the effect is added to the Desktop.
			 * @note This method is called by the Desktop. Do not call it directly.
			 * @see Desktop::addEffect()
			 */
			virtual void onAdded();

			/**
			 * Performs operations if the effect now ends because it was canceled.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 */
			virtual void onCancel(DrawingContext & context);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(EffectSequence);
			#endif

		public:

			/**
			 * Creates a new effect sequence. After creation, add the effect to the Desktop.
			 * @see Desktop::addEffect()
			 */
			EffectSequence();

			virtual ~EffectSequence();

			void appendEffect(Effect * effect);

			const ArrayList<Effect> & getEffects();

			void insertEffect(int position, Effect * effect);

			void prependEffect(Effect * effect);
	};
}


#endif
