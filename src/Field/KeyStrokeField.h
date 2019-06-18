/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_KEYSTROKEFIELD_H
#define __GLAZIERY_KEYSTROKEFIELD_H


#define GLAZIERY_KEYSTROKEFIELD(variable, area, groupField) \
	KeyStrokeField * variable; \
	if ((variable = new KeyStrokeField) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable);

#define GLAZIERY_KEYSTROKEFIELD_P(variable, area, groupField, px, py) \
	GLAZIERY_KEYSTROKEFIELD(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_KEYSTROKEFIELD_S(variable, area, groupField, width, height) \
	GLAZIERY_KEYSTROKEFIELD(variable, area, groupField) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_KEYSTROKEFIELD_PS(variable, area, groupField, px, py, width, height) \
	GLAZIERY_KEYSTROKEFIELD(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_KEYSTROKEFIELD_M(variable, area, groupField, minimumWidth, minimumHeight) \
	GLAZIERY_KEYSTROKEFIELD(variable, area, groupField) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_KEYSTROKEFIELD_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight) \
	GLAZIERY_KEYSTROKEFIELD(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_KEYSTROKEFIELD_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight) \
	GLAZIERY_KEYSTROKEFIELD(variable, area, groupField) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_KEYSTROKEFIELD_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight) \
	GLAZIERY_KEYSTROKEFIELD(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	class KeyStrokeField : public InputField
	{
		public:

			/**
			 * The listener interface for key stroke field events.
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

					virtual void onKeyChanged(KeyStrokeField * field);
			};

		private:

			bool deleteClearing;

			int keyCode;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(KeyStrokeField);
			#endif

		public:

			/**
			 * Creates a new key stroke field.
			 */
			KeyStrokeField();

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

			int getKeyCode();

			bool isDeleteClearing();

			/**
			 * Handles the event that a planned key stroke has been pressed
			 * on the event target.
			 * Key strokes are usually received by key stroke fields.
			 * When the desktop is in key stroke mode, the next key pressed
			 * should be sent to this method.
			 * This is useful e.g. to define keyboard layouts for games.
			 * @param keyCode The key stroke key code.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 * @see Desktop::isKeyStrokeMode()
			 */
			virtual bool onKeyStroke(int keyCode, bool option1, bool option2);

			void setDeleteClearing(bool deleteClearing);

			void setKeyCode(int keyCode);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();

			void unsetKeyCode();

			/**
			 * Validates whether this field has been filled out.
			 * If not, shows a balloon popup with an error message.
			 * @return Whether the field is valid.
			 */
			bool validateRequired();
	};
}


#endif
