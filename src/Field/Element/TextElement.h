/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_TEXTELEMENT_H
#define __GLAZIERY_TEXTELEMENT_H


namespace glaziery
{
	/**
	 * Text elements are field elements that display a text.
	 */
	class TextElement : public FieldElement
	{
		public:

			/**
			 * The popup for editing the text element.
			 */
			class EditPopup : public SingleFieldPopup
			{
				friend TextElement;

				protected:

					class ScrollingFieldListener : public ScrollingField::Listener
					{
						private:

							/**
							* The edit popup currently open.
							*/
							EditPopup * popup;

						public:

							/**
							* Creates a new listener for the scrolling field, if the popup was created for one.
							* @param popup The edit popup currently open.
							*/
							ScrollingFieldListener(EditPopup * popup);

							/**
							* Called when the scroll position now is set.
							* @param field The field that received the event.
							* @param previousPosition The previous scrolling position now.
							*/
							virtual void onScrollPositionNowSet(ScrollingField * field, Vector previousPosition);
					};

				protected:

					/**
					 * The text element this edit popup belongs to.
					 */
					TextElement * element;

					/**
					 * The scrolling field the popup is created for, or NULL,
					 * if the popup is not created for any scrolling field.
					 * Setting a scrolling field enables the popup to move if the scrolling field is scrolled.
					 */
					ScrollingField * scrollingField;

					/**
					 * The scrolling field listener, if any.
					 */
					ScrollingFieldListener * scrollingFieldListener;

					/**
					 * Creates a new edit popup.
					 * @param element The text element this edit popup belongs to.
					 * @param scrollingField The scrolling field the popup is created for,
					 * or NULL, if the popup is not created for any scrolling field.
					 * Setting a scrolling field enables the popup to move if the scrolling field is scrolled.
					 */
					EditPopup(TextElement * element, ScrollingField * scrollingField);

					/**
					 * Applies the popup edit field content to the text element this popup has been opened for.
					 */
					void apply();

				public:

					/**
					 * Cancels the popup and destroys it.
					 */
					virtual void cancel();

					/**
					 * Closes this popup and opens another one for the text element one down.
					 */
					void moveDown();

					/**
					 * Closes this popup and opens another one for the text element to the left.
					 */
					void moveLeft();

					/**
					 * Closes this popup and opens another one for the text element to the right.
					 */
					void moveRight();

					/**
					 * Closes this popup and opens another one for the text element one up.
					 */
					void moveUp();

					/**
					 * Handles the cancel event on the event target.
					 * On Windows systems, this is the ESC key.
					 * @return Whether the event target has consumed the event.
					 * If false, the event is first passed to the desktop
					 * and then to the input manager to increase event quantities.
					 */
					virtual bool onCancel();

					/**
					 * Notifies all component listeners about the destruction
					 * of the component, but does not delete it nor adds it to
					 * the list of disposable components.
					 * @note Override this method if you require pre-deletion operations.
					 * The default implementation notifies listeners only.
					 */
					virtual void onDestroying();

					/**
					 * Handles the event that the ENTER key is pressed on the event target.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @return Whether the event target has consumed the event.
					 * If false, the event is first passed to the desktop
					 * and then to the input manager to increase event quantities.
					 */
					virtual bool onEnter(bool option1, bool option2);
			};

			/**
			 * The listener interface for text element events.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * component anymore. So you should not hold any references to them.
			 */
			class Listener : public FieldElement::Listener
			{
				friend TextElement;

				protected:

					/**
					 * Destroys the listener.
					 * Since listeners are managed automatically,
					 * you do not need to destroy them by yourself.
					 */
					virtual ~Listener();

				public:

					/**
					 * Called when the text of a text element has been changed.
					 * @param element The element that received the event.
					 * @param oldText The old text of the text element before the change.
					 * @see onTextChanging()
					 */
					virtual void onTextChanged(TextElement * element, const String & oldText);

					/**
					 * Called when the text of a text element is about to be changed.
					 * The listener may cancel this event by vetoing it.
					 * @param element The element that received the event.
					 * @param newText The new text for the text element.
					 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
					 * @see VetoMode
					 * @see onTextChanged()
					 */
					virtual VetoMode onTextChanging(TextElement * element, const String & newText);
			};

			/**
			 * The listener for the scrolling field, if the popup was created for one.
			 */
			/**
			 * The listener for popup text field navigation key strokes.
			 */
			class TextFieldListener : public TextField::Listener
			{
				private:

					/**
					 * The edit popup currently open.
					 */
					EditPopup * popup;

				public:

					/**
					 * Creates a new listener for popup text field navigation key strokes.
					 * @param popup The edit popup currently open.
					 */
					TextFieldListener(EditPopup * popup);

					/**
					 * Called when the back TAB key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * On Windows systems, the back TAB key is SHIFT+TAB.
					 * Back tabs step the focus from field to field in backward order.
					 * @param target The target that received the event.
					 * @param secondary Whether the secondary tabbing is used.
					 * E.g., on Windows systems, this is done by holding the control key.
					 */
					virtual void onBackTab(EventTarget * target, bool secondary);

					/**
					 * Called when the DOWN key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onMoveDown(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the UP key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * @param target The target that received the event.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onMoveUp(EventTarget * target, bool option1, bool option2);

					/**
					 * Called when the TAB key is pressed on the event target,
					 * the target has the inner-most focus,
					 * and the event not handled otherwise by the target.
					 * Tabs step the focus from field to field.
					 * @param target The target that received the event.
					 * @param secondary Whether the secondary tabbing is used.
					 * E.g., on Windows systems, this is done by holding the control key.
					 */
					virtual void onTab(EventTarget * target, bool secondary);
			};

		friend EditPopup;

		private:

			/**
			 * If the text element is currently edited by the user,
			 * this is the popup window for the editing. Otherwise, it is NULL.
			 */
			EditPopup * editPopup;

			/**
			 * The text of the element.
			 */
			String text;

		protected:

			/**
			 * Destroys the text element.
			 */
			virtual ~TextElement();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(TextElement);
			#endif

		public:

			/**
			 * Creates a new text element.
			 */
			TextElement();

			/**
			 * Draws the content of the field element to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation. This context is already translated
			 * for the current field element position and available size
			 * (e.g. list field column).
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Opens a popup to edit the field element.
			 * This method is typically called by the field (e.g. list or tree field)
			 * when receiving the onEdit() event.
			 * @param position The absolute position of the top-left corner
			 * of the inner edit field of the popup.
			 * @param size The size of the inner edit field of the popup.
			 * @param scrollingField The scrolling field the popup is created for,
			 * or NULL, if the popup is not created for any scrolling field.
			 * Setting a scrolling field enables the popup to move if the scrolling field is scrolled.
			 * Defaults to NULL.
			 */
			virtual void edit(Vector position, Vector size, ScrollingField * scrollingField = NULL);

			/**
			 * If the text element is currently edited by the user,
			 * returns the popup window for the editing. Otherwise, returns NULL.
			 * @return The current edit popup, or NULL.
			 */
			EditPopup * getEditPopup();

			/**
			 * Returns the size of the field element.
			 * @return The size of the field element.
			 */
			virtual Vector getSize();

			/**
			 * Returns the text of the element.
			 * @return The text of the element.
			 */
			const String & getText();

			/**
			 * Sets the text of the element.
			 * @param text The text to be set.
			 * @return Whether the method succeeded. If a listener vetoed, this method returns false.
			 */
			bool setText(const String & text);
	};
}


#endif
