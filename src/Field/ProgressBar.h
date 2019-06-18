/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_PROGRESSBAR_H
#define __GLAZIERY_PROGRESSBAR_H


#define GLAZIERY_PROGRESSBAR(variable, area, groupField, \
	start, end, current) \
	ProgressBar * variable; \
	if ((variable = new ProgressBar) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setStart(start); \
	variable->setEnd(end); \
	variable->setCurrent(current);

#define GLAZIERY_PROGRESSBAR_P(variable, area, groupField, px, py, \
	start, end, current) \
	GLAZIERY_PROGRESSBAR(variable, area, groupField, start, end, current) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_PROGRESSBAR_S(variable, area, groupField, width, height, \
	start, end, current) \
	GLAZIERY_PROGRESSBAR(variable, area, groupField, start, end, current) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_PROGRESSBAR_PS(variable, area, groupField, px, py, width, height, \
	start, end, current) \
	GLAZIERY_PROGRESSBAR(variable, area, groupField, start, end, current) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_PROGRESSBAR_M(variable, area, groupField, minimumWidth, minimumHeight, \
	start, end, current) \
	GLAZIERY_PROGRESSBAR(variable, area, groupField, start, end, current) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_PROGRESSBAR_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	start, end, current) \
	GLAZIERY_PROGRESSBAR(variable, area, groupField, start, end, current) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_PROGRESSBAR_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	start, end, current) \
	GLAZIERY_PROGRESSBAR(variable, area, groupField, start, end, current) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_PROGRESSBAR_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	start, end, current) \
	GLAZIERY_PROGRESSBAR(variable, area, groupField, start, end, current) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	/**
	 * The progress bar shows a bar with a percentual indicator,
	 * usually "filling" be space from empty to full in given steps.
	 */
	class ProgressBar : public Field
	{
		private:

			/**
			 * Whether the process bar is in candy mode, i.e. it shows a generic
			 * progress visualization, but no progress percentage.
			 * Use this mode to show that some operation is in progress,
			 * and if you have no hint how long it will take.
			 * Setting the current value automatically stops candy mode.
			 */
			bool candyMode;

			/**
			 * The deferred candy mode.
			 */
			bool candyModeDeferred;

			/**
			 * The currently set value, usually between start and end.
			 * @see start
			 * @see end
			 */
			long current;

			/**
			 * The deferred current.
			 */
			long currentDeferred;

			/**
			 * The maximum value for the progress bar.
			 */
			long end;

			/**
			 * The deferred end.
			 */
			long endDeferred;

			/**
			 * The minimum value for the progress bar.
			 */
			long start;

			/**
			 * The deferred start.
			 */
			long startDeferred;

		protected:

			/**
			 * Destroys the progress bar.
			 */
			virtual ~ProgressBar();

			/**
			 * Executes all deferred operations in this event target.
			 * Override, but super-call if you provide any deferred operations.
			 * @see Desktop::executeDeferrals()
			 */
			virtual void executeDeferrals();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ProgressBar);
			#endif

		public:

			/**
			 * Creates a new progress bar.
			 */
			ProgressBar();

			/**
			 * Draws the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The caller is responsible for not calling this method
			 * if the component is invisible.
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Returns the currently set value, limited to the start and end limits.
			 * @return The constrained set value.
			 * @see getCurrent()
			 * @see getStart()
			 * @see getEnd()
			 */
			long getConstrainedCurrent();

			/**
			 * Returns the currently set value, usually between start and end.
			 * @return The currently set value.
			 * @see getStart()
			 * @see getEnd()
			 */
			long getCurrent();

			/**
			 * Returns the maximum value for the slider.
			 * @return The maximum value for the slider.
			 */
			long getEnd();

			/**
			 * Returns the minimum value for the slider.
			 * @return The minimum value for the slider.
			 */
			long getStart();

			/**
			 * Increases the current value by one or a given amount.
			 * Also unset the candy mode, if set.
			 * @param amount The amount to increase the current value. Defaults to 1.
			 * @see setCandyMode()
			 */
			void increase(long amount = 1);

			/**
			 * Defers the increase operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @param amount The amount to increase the current value. Defaults to 1.
			 * @see Desktop::deferOperation()
			 */
			void increaseDeferred(long amount = 1);

			/**
			 * Returns whether the process bar is in candy mode, i.e. it shows a generic
			 * progress visualization, but no progress percentage.
			 * Use this mode to show that some operation is in progress,
			 * and if you have no hint how long it will take.
			 * Setting the current value automatically stops candy mode.
			 * @return Whether the process bar is in candy mode.
			 */
			bool isCandyMode();

			/**
			 * Sets whether the process bar is in candy mode, i.e. it shows a generic
			 * progress visualization, but no progress percentage.
			 * Use this mode to show that some operation is in progress,
			 * and if you have no hint how long it will take.
			 * Setting the current value automatically stops candy mode.
			 * @param candyMode Whether the process bar is in candy mode. Defaults to true.
			 */
			void setCandyMode(bool candyMode = true);

			/**
			 * Defers the setCandyMode operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @param candyMode Whether the process bar is in candy mode. Defaults to true.
			 * @see Desktop::deferOperation()
			 */
			void setCandyModeDeferred(bool candyMode = true);

			/**
			 * Sets the currently set value, usually between start and end.
			 * Also unset the candy mode, if set.
			 * @param current The new set value.
			 * @see setStart()
			 * @see setEnd()
			 * @see setCandyMode()
			 */
			void setCurrent(long current);

			/**
			 * Defers the setCurrent operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @param current The new set value.
			 * @see Desktop::deferOperation()
			 */
			void setCurrentDeferred(long current);

			/**
			 * Sets the current value to the end value, completing the progress.
			 * @see setCurrent()
			 */
			void setCurrentToEnd();

			/**
			 * Sets the current value to the start value, resetting the progress.
			 * @see setCurrent()
			 */
			void setCurrentToStart();

			/**
			 * Sets the maximum value for the progress bar.
			 * @param end The maximum value for the progress bar.
			 */
			void setEnd(long end);

			/**
			 * Defers the setEnd operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @param end The maximum value for the progress bar.
			 * @see Desktop::deferOperation()
			 */
			void setEndDeferred(long end);

			/**
			 * Sets the minimum value for the progress bar.
			 * @param start The minimum value for the progress bar.
			 */
			void setStart(long start);

			/**
			 * Defers the setStart operation, executing it by the main thread.
			 * @note Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use this method from any concurrent thread.
			 * @param start The minimum value for the progress bar.
			 * @see Desktop::deferOperation()
			 */
			void setStartDeferred(long start);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
