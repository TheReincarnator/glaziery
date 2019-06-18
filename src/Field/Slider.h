/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SLIDER_H
#define __GLAZIERY_SLIDER_H


#define GLAZIERY_SLIDER(variable, area, groupField, \
	start, end, current) \
	Slider * variable; \
	if ((variable = new Slider) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setStart(start); \
	variable->setEnd(end); \
	variable->setCurrent(current);

#define GLAZIERY_SLIDER_P(variable, area, groupField, px, py, \
	start, end, current) \
	GLAZIERY_SLIDER(variable, area, groupField, start, end, current) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_SLIDER_S(variable, area, groupField, width, height, \
	start, end, current) \
	GLAZIERY_SLIDER(variable, area, groupField, start, end, current) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SLIDER_PS(variable, area, groupField, px, py, width, height, \
	start, end, current) \
	GLAZIERY_SLIDER(variable, area, groupField, start, end, current) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SLIDER_M(variable, area, groupField, minimumWidth, minimumHeight, \
	start, end, current) \
	GLAZIERY_SLIDER(variable, area, groupField, start, end, current) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_SLIDER_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	start, end, current) \
	GLAZIERY_SLIDER(variable, area, groupField, start, end, current) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_SLIDER_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	start, end, current) \
	GLAZIERY_SLIDER(variable, area, groupField, start, end, current) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SLIDER_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	start, end, current) \
	GLAZIERY_SLIDER(variable, area, groupField, start, end, current) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SLIDER_SL_CREATE(variable) \
	ArrayList<String> * variable; \
	if ((variable = new ArrayList<String>) == NULL) \
		throw EOUTOFMEMORY;

#define GLAZIERY_SLIDER_SL_ITEM(variable, item) \
	{ \
		String * clone; \
		if ((clone = new String(item)) == NULL) \
			throw EOUTOFMEMORY; \
		variable->Append(clone); \
	}

#define GLAZIERY_SLIDER_SL_ATTACH(slider, stringList, target) \
	slider->attachStringList(stringList, target);


namespace glaziery
{
	/**
	 * The slider is an input field where the user can choose
	 * a value from a given range by moving a respective slider.
	 */
	class Slider : public InputField
	{
		public:

			/**
			 * The listener interface for slider events.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * component anymore. So you should not hold any references to them.
			 */
			class Listener : public InputField::Listener
			{
				protected:

					/**
					 * Destroys the listener.
					 * Since listeners are managed automatically,
					 * you do not need to destroy them by yourself.
					 */
					virtual ~Listener();

				public:

					virtual void onCurrentChanged(Slider * field, long oldCurrent);
					virtual VetoMode onCurrentChanging(Slider * field, long newCurrent);
			};

		private:

			/**
			 * The currently chosen value. The user can choose between start and end.
			 * @see start
			 * @see end
			 */
			long current;

			/**
			 * The maximum value for the slider.
			 */
			long end;

			/**
			 * The units to increase or decrease when using the cursor keys with major steps
			 * (holding the range selection key, e.g. SHIFT on Windows systems).
			 * The default is 10.
			 */
			long majorStepUnits;

			/**
			 * The units to increase or decrease when using the cursor keys with minor steps
			 * (not holding the range selection key, e.g. SHIFT on Windows systems).
			 * The default is 1.
			 */
			long minorStepUnits;

			/**
			 * The minimum value for the slider.
			 */
			long start;

			/**
			 * The string list to be set into the stringListAttachmentTarget,
			 * according to the current value, when the current value changes.
			 * NULL, if no attachment has been set.
			 * @see attachStringList()
			 * @see detachStringList()
			 */
			ArrayList<String> * stringListAttachment;

			/**
			 * The field to be updated from the string value of stringListAttachment,
			 * according to the current value, when the current value changes.
			 * NULL, if no attachment has been set.
			 * @see attachStringList()
			 * @see detachStringList()
			 */
			Field * stringListAttachmentTarget;

		protected:

			/**
			 * Destroys the slider.
			 */
			virtual ~Slider();

			/**
			 * Updates the text of the string list attachment target, if any.
			 * @see attachStringList()
			 */
			void updateAttachmentTarget();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Slider);
			#endif

		public:

			/**
			 * Creates a new slider.
			 */
			Slider();

			/**
			 * Attaches a given ArrayList of Strings to the slider, defining possible values
			 * to choose from when sliding the position. The current string value is put into
			 * another Label or TextField.
			 * @param strings The string list to be attached.
			 * @param target The field whose text is updated from the string list.
			 * The field must support setting a text, otherwise nothing happens.
			 * @note Calling this method adjusts the start to 0, the end to the count of the list minus one,
			 * and the minor step units to 1.
			 * @warn The slider owns the ArrayList after the call and will delete it when the slider is deleted.
			 * Do not delete the ArrayList on your own.
			 * @note Any previously attached ArrayList will be deleted.
			 * @see detachStringList()
			 */
			void attachStringList(ArrayList<String> * stringList, Field * target);

			/**
			 * Detaches the previously attached ArrayList of Strings from the slider, deleting it.
			 * @see attachStringList()
			 */
			void detachStringList();

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
			 * Returns the currently chosen value, limited to the start and end limits.
			 * @return The constrained chosen value.
			 * @see getCurrent()
			 * @see getStart()
			 * @see getEnd()
			 */
			long getConstrainedCurrent();

			/**
			 * Returns the currently chosen value. The user can choose between start and end.
			 * @return The currently chosen value.
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
			 * Returns the units to increase or decrease when using the cursor keys with major steps
			 * (holding the range selection key, e.g. SHIFT on Windows systems).
			 * The default is 10.
			 * @return The major step units.
			 */
			long getMajorStepUnits();

			/**
			 * Returns the units to increase or decrease when using the cursor keys with minor steps
			 * (not holding the range selection key, e.g. SHIFT on Windows systems).
			 * The default is 1.
			 * @return The minor step units.
			 */
			long getMinorStepUnits();

			/**
			 * Returns the minimum value for the slider.
			 * @return The minimum value for the slider.
			 */
			long getStart();

			/**
			 * Returns the string list to be set into the stringListAttachmentTarget,
			 * according to the current value, when the current value changes.
			 * @return The string list or NULL, if no attachment has been set.
			 * @note If you modify the length of this list, you will also have to
			 * set the end property to the count of the list minus one.
			 * @see attachStringList()
			 * @see detachStringList()
			 */
			ArrayList<String> * getStringListAttachment();

			/**
			 * Returns the field to be updated from the string value of stringListAttachment,
			 * according to the current value, when the current value changes.
			 * @return The field to be updated or NULL, if no attachment has been set.
			 * @see attachStringList()
			 * @see detachStringList()
			 */
			Field * getStringListAttachmentTarget();

			/**
			 * Handles the event that the LEFT key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveLeft(bool option1, bool option2);

			/**
			 * Handles the event that the RIGHT key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveRight(bool option1, bool option2);

			/**
			 * Handles the event that the END key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveToEnd(bool option1, bool option2);

			/**
			 * Handles the event that the START (HOME, POS1, ...) key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveToStart(bool option1, bool option2);

			/**
			 * Handles the event that the PAGE DOWN key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onPageDown(bool option1, bool option2);

			/**
			 * Handles the event that the PAGE UP key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onPageUp(bool option1, bool option2);

			/**
			 * Sets the currently chosen value. The user can choose between start and end.
			 * @param current The new chosen value.
			 * @see setStart()
			 * @see setEnd()
			 */
			void setCurrent(long current);

			/**
			 * Sets the maximum value for the slider.
			 * @param end The maximum value for the slider.
			 */
			void setEnd(long end);

			/**
			 * Sets the units to increase or decrease when using the cursor keys with major steps
			 * (holding the range selection key, e.g. SHIFT on Windows systems).
			 * The default is 10.
			 * @param majorStepUnits The major step units.
			 */
			void setMajorStepUnits(long majorStepUnits);

			/**
			 * Sets the units to increase or decrease when using the cursor keys with minor steps
			 * (not holding the range selection key, e.g. SHIFT on Windows systems).
			 * The default is 1.
			 * @param minorStepUnits The minor step units.
			 */
			void setMinorStepUnits(long minorStepUnits);

			/**
			 * Sets the minimum value for the slider.
			 * @param start The minimum value for the slider.
			 */
			void setStart(long start);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
