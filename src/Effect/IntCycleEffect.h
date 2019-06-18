/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_INTCYCLEEFFECT_H
#define __GLAZIERY_INTCYCLEEFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class IntCycleEffect : public TimedEffect
	{
		private:

			/**
			 * The component destroy listener removes the effect when its component is deleted.
			 */
			class ComponentDestroyListener : public Component::Listener
			{
				friend IntCycleEffect;

				private:

					/**
					 * The effect this listener observes.
					 */
					IntCycleEffect * effect;

					/**
					 * Creates a new component destroy listener.
					 * @param effect The effect this listener observes.
					 */
					ComponentDestroyListener(Effect * effect);
			virtual ~ComponentDestroyListener();

					/**
					 * Called when the component is about to be destroyed (its memory
					 * is released). This method is called before the component is deleted,
					 * but its parent container references may have been released already.
					 * @param component The component that received the event.
					 */
					virtual void onDestroying(Component * component);
			};

		friend ArrayList<IntCycleEffect>;

		private:

			Component * component;

			ComponentDestroyListener * componentDestroyListener;

			int maximum;

			int * valuePointer;

		protected:

			virtual ~IntCycleEffect();

			/**
			 * Performs operations when the effect is added to the Desktop.
			 * @note This method is called by the Desktop. Do not call it directly.
			 * @see Desktop::addEffect()
			 */
			virtual void onAdded();

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

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(IntCycleEffect);
			#endif

		public:

			/**
			 * Creates a new component effect. After creation, add the effect to the Desktop.
			 * @param time The total time of the effect in milli seconds.
			 * If the effect is repetitive, this is the time of one loop.
			 * @param component The component the effect is defined for.
			 * @see onAdded()
			 * @see Desktop::addEffect()
			 */
			IntCycleEffect(long time, int * valuePointer, int maximum, Component * component);

			Component * getComponent();

			int getMaximum();

			int * getValuePointer();
	};
}


#endif
