/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_GROUPFIELD_H
#define __GLAZIERY_GROUPFIELD_H


#define GLAZIERY_GROUPFIELD(variable, area, groupField) \
	GroupField * variable; \
	if ((variable = new GroupField) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->getGroupLayoutData()->setTopLeftMargin(Vector()); \
	variable->getGroupLayoutData()->setBottomRightMargin(Vector());

#define GLAZIERY_GROUPFIELD_P(variable, area, groupField, px, py, \
	bordered, text) \
	GLAZIERY_GROUPFIELD(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_GROUPFIELD_S(variable, area, groupField, width, height) \
	GLAZIERY_GROUPFIELD(variable, area, groupField) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_GROUPFIELD_PS(variable, area, groupField, px, py, width, height) \
	GLAZIERY_GROUPFIELD(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_GROUPFIELD_M(variable, area, groupField, minimumWidth, minimumHeight) \
	GLAZIERY_GROUPFIELD(variable, area, groupField) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_GROUPFIELD_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight) \
	GLAZIERY_GROUPFIELD(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_GROUPFIELD_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight) \
	GLAZIERY_GROUPFIELD(variable, area, groupField) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_GROUPFIELD_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight) \
	GLAZIERY_GROUPFIELD(variable, area, groupField) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_GROUPFIELD_B(variable, area, groupField, text) \
	GroupField * variable; \
	if ((variable = new GroupField) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setBordered(true); \
	variable->setText(text);

#define GLAZIERY_GROUPFIELD_BP(variable, area, groupField, px, py, text) \
	GLAZIERY_GROUPFIELD_B(variable, area, groupField, text) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_GROUPFIELD_BS(variable, area, groupField, width, height, text) \
	GLAZIERY_GROUPFIELD_B(variable, area, groupField, text) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_GROUPFIELD_BPS(variable, area, groupField, px, py, width, height, text) \
	GLAZIERY_GROUPFIELD_B(variable, area, groupField, text) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_GROUPFIELD_BM(variable, area, groupField, minimumWidth, minimumHeight, text) \
	GLAZIERY_GROUPFIELD_B(variable, area, groupField, text) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight));

#define GLAZIERY_GROUPFIELD_BPM(variable, area, groupField, px, py, minimumWidth, minimumHeight, text) \
	GLAZIERY_GROUPFIELD_B(variable, area, groupField, text) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight));

#define GLAZIERY_GROUPFIELD_BSM(variable, area, groupField, width, height, minimumWidth, minimumHeight, text) \
	GLAZIERY_GROUPFIELD_B(variable, area, groupField, text) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_GROUPFIELD_BPSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, text) \
	GLAZIERY_GROUPFIELD_B(variable, area, groupField, text) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	/**
	 * The group field displays a named border around some other fields.
	 * This group can be used for two purposes:
	 *
	 * <ul>
	 * <li>Drawing a labeled border around other fields to help the user
	 * to understand which fields belong together,</li>
	 * <li>grouping fields for layouting purposes, as group fields
	 * declare a bounding box for the layout process.</li>
	 * </ul>
	 *
	 * If the hot key is activated, the next input field in the field area's
	 * list of fields is selected, regardless of the group field hierarchy.
	 * So it is advisable to add the containing fields to the fields area
	 * directly after having added the group field itself.
	 */
	class GroupField : public Field
	{
		friend Field;

		private:

			/**
			 * Whether the group field draws a border around it, labeled with the field's text.
			 * Defaults to false. Group fields without border are used for layouting purposes,
			 * as they declare a bounding box for the layout process.
			 */
			bool bordered;

			/**
			 * The fields contained in this group field.
			 */
			ArrayList<Field> fields;

			/**
			 * The layout data for this group field. Defaults to a ContainerLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @see ContainerLayoutData
			 */
			ContainerLayoutData * groupLayoutData;

			/**
			 * The group field title (displayed as caption).
			 * Use an ampersand (&) before a character to indicate it has hot key.
			 * The hot key character is underlined automatically.<br>
			 *
			 * If the hot key is activated, the next input field in the field area's
			 * list of fields is selected, regardless of the group field hierarchy.
			 * So it is advisable to add the containing fields to the fields area
			 * directly after having added the group field itself.
			 *
			 * @note This text is displayed only if bordered is true.
			 * @see isBordered()
			 */
			String text;

		protected:

			/**
			 * Destroys the group field.
			 */
			virtual ~GroupField();

			/**
			 * Returns the text string attribute, if the field supports text editing, otherwise NULL.
			 * Defaults to NULL, override in derivatives that support text editing.
			 * @note getTextAttribute() returns NULL, the field does not support text editing.
			 * setText does nothing, getText returns an empty string in this case.
			 * @return The text string attribute or NULL.
			 * @see getText()
			 * @see setText()
			 */
			virtual String * getTextAttribute();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(GroupField);
			#endif

		public:

			/**
			 * Creates a new group field.
			 */
			GroupField();

			/**
			 * Appends a field at the end of the fields list of this group field.
			 * @param field The field to be added.
			 * @note To add a field to a group field, you must add it to the respective fields area first.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning it off while adding fields (resp. not turning it on before having finished).
			 */
			void appendField(Field * field);

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
			 * Returns the event target at a given position that is setup to
			 * receive pointer events. For popup components, this is always true,
			 * for windows, this is true if and only if the pointer event mode is
			 * active or inactive, but bypassing windows are treated as not
			 * existing (transparent for events), and for widgets, this depends on
			 * the widget's hit test (isHitAt()).<br>
			 *
			 * This is a deep search: The desktop delegates to the found
			 * window or popup component, which delegates to the client area
			 * and so on.
			 * @param position The position relative to the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @return The event receiving event target at the position or
			 * <code>NULL</code>, if there is no target.
			 */
			virtual EventTarget * getEventTargetAt(Vector position);

			/**
			 * Returns the fields contained in this group field.
			 * @return The fields contained in this group field.
			 */
			const ArrayList<Field> & getFields();

			/**
			 * Returns the layout data for this group field. Defaults to a ContainerLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @return The layout data for this group field.
			 * @see ContainerLayoutData
			 */
			ContainerLayoutData * getGroupLayoutData();

			/**
			 * Inserts a field at a given position of the fields list of this group field.
			 * @param position The position to insert the field at.
			 * @param field The field to be inserted.
			 * @note To add a field to a group field, you must add it to the respective fields area first.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning it off while adding fields (resp. not turning it on before having finished).
			 */
			void insertField(int position, Field * field);

			/**
			 * Returns whether the field accepts onPress events to focus it. Defaults to true.
			 * @return Whether the field accepts onPress events to focus it.
			 */
			virtual bool isAcceptingPressesToFocus();

			/**
			 * Returns whether the group field draws a border around it, labeled with the field's text.
			 * Defaults to false. Group fields without border are used for layouting purposes,
			 * as they declare a bounding box for the layout process.
			 * @return Whether the group field draws a border around it.
			 */
			bool isBordered();

			/**
			 * Prepends a field at the beginning of the fields list of this group field.
			 * @param field The field to be added.
			 * @note To add a field to a group field, you must add it to the respective fields area first.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning it off while adding fields (resp. not turning it on before having finished).
			 */
			void prependField(Field * field);

			/**
			 * Sets whether the group field draws a border around it, labeled with the field's text.
			 * Defaults to false. Group fields without border are used for layouting purposes,
			 * as they declare a bounding box for the layout process.
			 * @param bordered whether the group field draws a border around it.
			 */
			void setBordered(bool bordered);

			/**
			 * Sets the layout data for this group field. Defaults to a ContainerLayoutData instance.
			 * You may subclass and replace this object to provide more information to the layouter.
			 * @param groupLayoutData The layout data for this group field.
			 * @note Calling this method does not relayouts the fields area, if automatic layouting
			 * is enabled, so you have to call the layout method manually, in this case.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 * @see ContainerLayoutData
			 */
			void setGroupLayoutData(ContainerLayoutData * groupLayoutData);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
