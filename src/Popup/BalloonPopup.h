/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_BALLOONPOPUP_H
#define __GLAZIERY_BALLOONPOPUP_H


namespace glaziery
{
	/**
	 * A balloon popup is a popup that displays some information
	 * for a target point, rectangle, or component on the desktop or within a window.
	 * It is used to instantly inform the user about some circumstance,
	 * e.g. that he has to fill out a given input field.
	 * @see Field::showBalloonPopup()
	 */
	class BalloonPopup : public Popup
	{
		public:

			/**
			 * The position of the arrow pointing to the balloon target.
			 */
			enum ArrowPosition
			{
				TOP, BOTTOM
			};

		private:

			/**
			 * The position of the arrow pointing to the balloon target.
			 */
			ArrowPosition arrowPosition;

			/**
			 * Whether to use all-capital characters when drawing the message text.
			 * Does not change the text itself. Defaults to false.
			 */
			bool caps;

			/**
			 * The font to be used for the message text.
			 */
			Font * font;

			/**
			 * The balloon message to show.
			 */
			String message;

			/**
			 * The x position of the target this balloon popup points to.
			 */
			int targetX;

		protected:

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

			/**
			 * Updates the balloon's size according to the message and border sizes.
			 */
			void updateSize();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(BalloonPopup);
			#endif

		public:

			/**
			 * Creates a new balloon popup.
			 */
			BalloonPopup();

			/**
			 * Draws the content of the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The implementation must clear the context before rendering,
			 * if it does not render to the entire context.
			 * @note This method is called when the cache is invalid only.
			 * See invalidate() and the CachableComponent's class comment for details.
			 * If the cache never gets valid (because the display depends on some event
			 * that is not pushed into the application or game), call invalidate() from
			 * within the implementation of drawContent().
			 * @see Desktop::getSkin()
			 * @see CachableComponent::invalidate()
			 * @see CachableComponent
			 */
			virtual void drawContent(DrawingContext & context);

			/**
			 * Returns the position of the arrow pointing to the balloon target.
			 * @return The position of the arrow pointing to the balloon target.
			 */
			ArrowPosition getArrowPosition();

			/**
			 * Returns the font to be used for the message text.
			 * @return The font to be used for the message text.
			 */
			Font * getFont();

			/**
			 * Returns the balloon message to show.
			 * @return The balloon message to show.
			 */
			const String & getMessage();

			/**
			 * Returns the x position of the target this balloon popup points to.
			 * @return The x position of the target this balloon popup points to.
			 */
			int getTargetX();

			/**
			 * Returns whether to use all-capital characters when drawing the message text.
			 * Does not change the text itself. Defaults to false.
			 * @return Whether to use all-capital characters.
			 */
			bool isCaps();

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
			 * Moves this balloon popup to the best position for a given target point.
			 * The balloon arrow will point to that target.
			 * @param target The target position to point to.
			 * @return Whether the balloon popup was actually moved.
			 */
			bool pointTo(Vector target);

			/**
			 * Moves this balloon popup to the best position for a given target rectangle.
			 * The balloon arrow will point to that target.
			 * @param targetFrom The top-left corner of the rectangle.
			 * @param targetTo The bottom-right corner of the rectangle.
			 * @return Whether the balloon popup was actually moved.
			 */
			bool pointTo(Vector targetFrom, Vector targetTo);

			/**
			 * Moves this balloon popup to the best position for a given target component.
			 * The balloon arrow will point to that component.
			 * @param component The component to point to.
			 * @return Whether the balloon popup was actually moved.
			 */
			bool pointTo(Component * component);

			/**
			 * Sets whether to use all-capital characters when drawing the message text.
			 * Does not change the text itself. Defaults to false.
			 * @param caps Whether to use all-capital characters.
			 */
			void setCaps(bool caps);

			/**
			 * Sets the font to be used for the message text.
			 * @param font The font to be used for the message text.
			 */
			void setFont(Font * font);

			/**
			 * Sets the balloon message to show.
			 * @param message The balloon message to show.
			 */
			void setMessage(const String & message);
	};
}


#endif
