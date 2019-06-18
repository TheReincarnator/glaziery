/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_LABEL_H
#define __GLAZIERY_LABEL_H


#define GLAZIERY_LABEL(variable, area, groupField, text) \
	Label * variable; \
	if ((variable = new Label) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setText(text);

#define GLAZIERY_LABEL_P(variable, area, groupField, px, py, text) \
	GLAZIERY_LABEL(variable, area, groupField, text) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_LABEL_S(variable, area, groupField, width, height, text) \
	GLAZIERY_LABEL(variable, area, groupField, text) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_LABEL_PS(variable, area, groupField, px, py, width, height, text) \
	GLAZIERY_LABEL(variable, area, groupField, text) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_LABEL_M(variable, area, groupField, minimumWidth, minimumHeight, text) \
	GLAZIERY_LABEL(variable, area, groupField, text) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_LABEL_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, text) \
	GLAZIERY_LABEL(variable, area, groupField, text) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_LABEL_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, text) \
	GLAZIERY_LABEL(variable, area, groupField, text) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_LABEL_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, text) \
	GLAZIERY_LABEL(variable, area, groupField, text) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	/**
	 * A label is a text displayed in a field area to name other fields
	 * or display a simple text. Labels have no function, except that the
	 * hot key indicator (underlined character) activates the respective hot key.
	 */
	class Label : public Field
	{
		public:

			/**
			 * A position of a URL within the text. Used to draw and click URLs.
			 */
			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				class UrlPosition : public CObject
			#else
				class UrlPosition
			#endif
			{
				private:

					/**
					 * The position relative to the label.
					 */
					Vector position;

					/**
					 * The pixel size of the URL.
					 */
					Vector size;

					/**
					 * The URL found.
					 */
					String url;

				public:

					/**
					 * Creates a new UrlPosition object.
					 * @param position The position relative to the label.
					 * @param size The pixel size of the URL.
					 * @param url The URL found.
					 */
					UrlPosition(Vector position, Vector size, const String & url);

					/**
					 * Returns the position relative to the label.
					 * @return The position relative to the label.
					 */
					Vector getPosition();

					/**
					 * Returns the pixel size of the URL.
					 * @return The pixel size of the URL.
					 */
					Vector getSize();

					/**
					 * Returns the URL found.
					 * @return The URL found.
					 */
					const String & getUrl();

					/**
					 * Returns whether the URL position is hit at a given pixel position.
					 * @param position The pixel position.
					 * @return Whether the URL position is hit.
					 */
					bool isHitAt(Vector position);
			};

		private:

			/**
			 * Whether the entire label is clickable.
			 * Defaults to false.
			 */
			bool allClickable;

			/**
			 * Whether this label is auto-resizing according to the size of the content. Defaults to true.
			 * @see autoResize()
			 */
			bool autoResizing;

			/**
			 * The label's text (displayed as caption).
			 * Use an ampersand (&) before a character to indicate it has hot key.
			 * The hot key character is underlined automatically.
			 */
			String text;

			/**
			 * Whether URLs in labels (http://, https://, ftp://, mailto:) are clickable.
			 * Defaults to true.
			 */
			bool urlsClickable;

		protected:

			/**
			 * The positions of URLs in the current text.
			 */
			ArrayList<UrlPosition> urlPositions;

			/**
			 * Destroys the label.
			 */
			virtual ~Label();

			/**
			 * Updates the cache of positions of URLs in the current text.
			 * The default implementation searches for http://, https://, ftp://, and mailto:.
			 * Override to change this behavior.
			 * @note The list of URL locations must be in ascending order.
			 */
			virtual void updateUrlPositions();

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
				DECLARE_DYNAMIC(Label);
			#endif

		public:

			/**
			 * Creates a new label.
			 */
			Label();

			/**
			 * Resizes the label once according to the size of the content.
			 * Useful if auto resizing is disabled.
			 * @see isAutoResizing()
			 * @see setAutoResizing()
			 */
			void autoResize();

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
			 * Returns a URL position at a given pointer position (relative to the label).
			 * @param position The pointer position (relative to the label).
			 * @return The URL position or NULL, if there is no URL or URL clickable is disabled.
			 */
			UrlPosition * getUrlPositionAt(Vector position);

			/**
			 * Returns the positions of URLs in the current text.
			 * @return The positions of URLs in the current text.
			 */
			const ArrayList<UrlPosition> & getUrlPositions();

			/**
			 * Returns whether the entire label is clickable.
			 * Defaults to false.
			 * @return Whether the entire label is clickable.
			 */
			bool isAllClickable();

			/**
			 * Returns whether this label is auto-resizing according to the size of the content. Defaults to true.
			 * @return Whether this label is auto-resizing according to the size of the content.
			 * @see autoResize()
			 */
			bool isAutoResizing();

			/**
			 * Returns whether click operations on this field behave like clicking the title bar,
			 * if supported by the skin.
			 * Defaults to false for input fields and some special fields, true for regular fields.
			 * @param position The position within the field to return the information for.
			 * @return Whether this field is like the title bar.
			 */
			virtual bool isLikeTitle(Vector position);

			/**
			 * Returns whether URLs in labels (http://, https://, ftp://, mailto:) are clickable.
			 * Defaults to true.
			 * @param urlsClickable Whether URLs are clickable.
			 */
			bool isUrlsClickable();

			/**
			 * Handles the event that the event target is clicked
			 * (usually using the left mouse button).
			 * A click is raised after the mouse has been pressed and
			 * released, and no dragging operation has started.
			 * @param position The position where the click occurred
			 * relative to the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onPress()
			 * @see onRelease()
			 * @see onDoubleClick()
			 */
			virtual void onClick(Vector position, bool option1, bool option2);

			/**
			 * Sets whether the entire label is clickable.
			 * Defaults to false. When setting to true, automatically unsets urlsClickable.
			 * @param allClickable Whether the entire label is clickable.
			 */
			void setAllClickable(bool allClickable);

			/**
			 * Sets whether this label is auto-resizing according to the size of the content. Defaults to true.
			 * @param autoResizing Whether this label is auto-resizing according to the size of the content.
			 * @see autoResize()
			 */
			void setAutoResizing(bool autoResizing);

			/**
			 * Sets whether to use all-capital characters when drawing the field's text.
			 * Does not change the text itself. Defaults to false.
			 * @param caps Whether to use all-capital characters when drawing the field's text.
			 */
			virtual void setCaps(bool caps);

			/**
			 * Sets the font to be used for texts included in the field.
			 * @param font The font to be used.
			 */
			virtual void setFont(Font * font);

			/**
			 * Sets the text (the caption) of this field, if the field supports it.
			 * For multi line text fields, the string is split at new line characters ('\n').
			 * If supported by the field implementation, the text may contain a hot key character
			 * (preceded by an ampersand character). Pressing the hot key will set the focus to the field.
			 * An ampersand character indicates a hot key, a double ampersand print the
			 * ampersand character itself.
			 * @param text The text (the caption) of this field.
			 * @note This method uses the attribute returned by the getTextAttribute() method.
			 * If getTextAttribute() returns NULL, the field does not support text editing.
			 * setText does nothing, getText returns an empty string in this case.
			 * @see getTextAttribute()
			 */
			virtual void setText(const String & text);

			/**
			 * Sets whether URLs in labels (http://, https://, ftp://, mailto:) are clickable.
			 * Defaults to true. When setting to true, automatically unsets allClickable.
			 * @return Whether URLs are clickable.
			 */
			void setUrlsClickable(bool urlsClickable);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
