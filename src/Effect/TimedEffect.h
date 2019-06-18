/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_TIMEDEFFECT_H
#define __GLAZIERY_TIMEDEFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class TimedEffect : public Effect
	{
		private:

			/**
			 * The remaining time in milli seconds for the effect.
			 * If the effect is repetitive, this is the remaining time
			 * for this loop.
			 */
			long remainingTime;

			/**
			 * The time curve to use. Translates the progress ranging from 0.0 to 1.0
			 * to another value starting from 0.0, ending in 1.0, allowing acceleration
			 * and any other type of time curve.
			 * @note May be NULL, indicating to use a linear time curve.
			 */
			TimeCurve * timeCurve;

			/**
			 * The total time of the effect in milli seconds.
			 * If the effect is repetitive, this is the time of one loop.
			 */
			long totalTime;

		protected:

			/**
			 * Creates a new effect. After creation, add the effect to the Desktop.
			 * @param time The total time of the effect in milli seconds.
			 * If the effect is repetitive, this is the time of one loop.
			 * @see Desktop::addEffect()
			 */
			TimedEffect(long time);

			/**
			 * Destroys the effect.
			 */
			virtual ~TimedEffect();

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
			 * The default implementation does not.
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
			 * The method can decide whether to continue the effect, which is the default.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @return Whether to continue the effect. false will destroy it.
			 * @note This method is called by the Desktop. Do not call it directly.
			 */
			virtual bool onStart(DrawingContext & context);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(TimedEffect);
			#endif

		public:

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
			 * Returns the elapsed time in milli seconds for the effect.
			 * If the effect is repetitive, this is the elapsed time
			 * for this loop.
			 * @return The elapsed time in milli seconds for the effect.
			 */
			long getElapsedTime();

			/**
			 * Returns the portion of the elapsed time to the total time,
			 * maybe translated by a time curve. This value ranges from 0.0 to 1.0.
			 * @return The portion, starting at 0.0, ending at 1.0.
			 */
			double getProgress();

			/**
			 * Returns the remaining time in milli seconds for the effect.
			 * If the effect is repetitive, this is the remaining time
			 * for this loop.
			 * @return The remaining time in milli seconds for the effect.
			 */
			long getRemainingTime();

			/**
			 * Returns the time curve in use. Translates the progress ranging from 0.0 to 1.0
			 * to another value starting from 0.0, ending in 1.0, allowing acceleration
			 * and any other type of time curve.
			 * May be NULL, indicating to use a linear time curve.
			 * @return The time curve in use or NULL.
			 */
			TimeCurve * getTimeCurve();

			/**
			 * Returns the total time of the effect in milli seconds.
			 * If the effect is repetitive, this is the time of one loop.
			 * @return The total time of the effect in milli seconds.
			 */
			long getTotalTime();

			/**
			 * Sets the progress of this effect. This value ranges from 0.0 to 1.0.
			 * @note Use this method to adjust the progress of a running effect,
			 * for example if the effect situation changed.
			 * You should not use this method for repetitions or sequences.
			 * @param progress The new value, starting at 0.0, ending at 1.0.
			 * @see onEnd()
			 * @see EffectSequence
			 */
			void setProgress(double getProgress);

			/**
			 * Sets the time curve to use. Translates the progress ranging from 0.0 to 1.0
			 * to another value starting from 0.0, ending in 1.0, allowing acceleration
			 * and any other type of time curve.
			 * @param timeCurve The time curve to be used.
			 * May be NULL, indicating to use a linear time curve.
			 * @warn The method owns the time curve after setting it and will delete its memory
			 * when the effect is destroyed or when another time curve is set.
			 * @see setTimeCurveToAcceleration()
			 */
			void setTimeCurve(TimeCurve * timeCurve);

			/**
			 * Convenience method: Sets a new AccelerationTimeCurve into this effect.
			 * @see setTimeCurve()
			 * @see AccelerationTimeCurve
			 */
			void setTimeCurveToAcceleration();

			/**
			 * If the effect is repetitive, this method sets the total time
			 * of the next loop(s) in milli seconds.
			 * @param totalTime The next loop(s) total time in milli seconds.
			 */
			void setTotalTime(long totalTime);
	};
}


#endif
