/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_EFFECT_H
#define __GLAZIERY_EFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class Effect : public SharedObject
	{
		friend ArrayList<Effect>;
		friend Desktop;
		friend EffectFork;
		friend EffectSequence;

		private:

			/**
			 * Whether the effect has been canceled.
			 * Canceled effects are not executed anymore
			 * and will be destroyed automatically by the Desktop.
			 */
			bool canceled;

			/**
			 * Whether the effect has ended already.
			 */
			bool ended;

			/**
			 * Whether this effect is currently handled.
			 * Required to detect modally run recursions.
			 */
			bool executing;

			/**
			 * Sets that the effect has ended already.
			 */
			void setEnded();

			/**
			 * Sets whether this effect is currently handled.
			 * Required to detect modally run recursions.
			 * @param executing Whether this effect is currently handled.
			 */
			void setExecuting(bool executing);

		protected:

			/**
			 * Creates a new effect. After creation, add the effect to the Desktop.
			 * @see Desktop::addEffect()
			 */
			Effect();

			/**
			 * Destroys the effect.
			 */
			virtual ~Effect();

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
			virtual bool execute(DrawingContext & context, long time) = NULL;

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
				DECLARE_DYNAMIC(Effect);
			#endif

		public:

			/**
			 * Cancels the effect. Canceled effects are not executed anymore.
			 * The desktop calls cancel(DrawingContext) next time it processes
			 * events and then destroys it automatically.
			 */
			void cancel();

			/**
			 * Draws the effect, if this effect causes a visualization.
			 * Usually, effects don't, but change properties of components etc.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note This method is called by the Desktop. Do not call it directly.
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Returns whether the effect has been canceled.
			 * Canceled effects are not executed anymore
			 * and will be destroyed automatically by the Desktop.
			 * @return Whether the effect has been canceled.
			 */
			bool isCanceled();

			/**
			 * Returns whether the effect has ended already.
			 * @return Whether the effect has ended already.
			 */
			bool isEnded();

			/**
			 * Returns whether this effect is currently handled.
			 * Required to detect modally run recursions.
			 * @return Whether this effect is currently handled.
			 */
			bool isExecuting();

			/**
			 * Performs the desktop's run loop until this effect has ended.
			 * Use this method if you want to perform operations that need the completedness of this effect.
			 * @warn This method calls Desktop::runOnce(), so it is possible that your code gets called in turn.
			 * Please be aware of this situation, maybe by added guard flags while calling this method.
			 */
			void waitFor();
	};
}


#endif
