/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_MENUWINDOW_H
#define __GLAZIERY_MENUWINDOW_H


namespace glaziery
{
	class MenuPopup : public Popup
	{
		private:

			bool destroyMenu;

			Menu * menu;

		protected:

			virtual ~MenuPopup();

			virtual void resizeToMenu();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(MenuPopup);
			#endif

		public:

			MenuPopup(Menu * menu, bool destroyMenu);

			/**
			 * Removes the component from the Glaziery system to destroy it.
			 * First performs operations before the component is destroyed,
			 * then adds it to the Desktop's list of disposables.
			 * @note The component is added to the list of disposable components.
			 * Nevertheless, the memory is still available until the next call to
			 * Desktop::draw() (the next frame).
			 * @note This is an ungraceful removal, without user interaction
			 * and close notifications, but with destroy noficiation.
			 * E.g. for windows, use Window::close() to close normally.
			 */
			virtual void destroy();

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

			Menu * getMenu();

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
	};
}


#endif
