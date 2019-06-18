/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_COMPONENTEFFECT_H
#define __GLAZIERY_COMPONENTEFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class ComponentEffect : public TimedEffect
	{
		friend Component;
		friend ArrayList<ComponentEffect>;

		private:

			Component * component;

			bool destroying;

			bool hiding;

			Vector positionEnd;

			bool positionEndRelative;

			bool positionEndSet;

			Vector positionStart;

			bool positionStartRelative;

			bool positionStartSet;

			bool showing;

			Vector sizeEnd;

			bool sizeEndRelative;

			bool sizeEndSet;

			Vector sizeStart;

			bool sizeStartRelative;

			bool sizeStartSet;

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

			void onComponentDestroying();

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
			 * Performs operations when the effect starts or restarts, during the next frame's
			 * run cycle (particularly it is called after onAdded)
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @return Whether to continue the effect. false will destroy it.
			 * @note This method is called by the Desktop. Do not call it directly.
			 */
			virtual bool onStart(DrawingContext & context);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ComponentEffect);
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
			ComponentEffect(long time, Component * component);

			Component * getComponent();

			bool isDestroying();

			bool isHiding();

			bool isShowing();

			void setDestroying(bool destroying);

			void setHiding(bool hiding);

			void setPositionEnd(Vector positionEnd, bool relative = false);

			void setPositionStart(Vector positionStart, bool relative = false);

			void setShowing(bool showing);

			void setSizeEnd(Vector sizeEnd, bool relative = false);

			void setSizeStart(Vector sizeStart, bool relative = false);
	};
}


#endif
