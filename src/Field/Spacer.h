/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SPACER_H
#define __GLAZIERY_SPACER_H


#define GLAZIERY_SPACER(variable, area, groupField) \
	Spacer * variable; \
	if ((variable = new Spacer) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable);

#define GLAZIERY_SPACER_P(variable, area, groupField, px, py, \
	bordered, text) \
	GLAZIERY_SPACER(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_SPACER_S(variable, area, groupField, width, height) \
	GLAZIERY_SPACER(variable, area, groupField) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SPACER_PS(variable, area, groupField, px, py, width, height) \
	GLAZIERY_SPACER(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_SPACER_M(variable, area, groupField, minimumWidth, minimumHeight) \
	GLAZIERY_SPACER(variable, area, groupField) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_SPACER_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight) \
	GLAZIERY_SPACER(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_SPACER_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight) \
	GLAZIERY_SPACER(variable, area, groupField) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	/**
	 * A spacer is a field that does not have any output or input,
	 * and can be used in layouters to add a static or dynamic (filling) space.
	 */
	class Spacer : public Field
	{
		friend Field;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Spacer);
			#endif

		public:

			/**
			 * Creates a new spacer.
			 */
			Spacer();

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
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
