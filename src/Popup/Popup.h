/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_POPUPWINDOW_H
#define __GLAZIERY_POPUPWINDOW_H


namespace glaziery
{
	/**
	 * A popup window, e.g. used for menu popups or text element editing.
	 * This class is not related to the Window class.
	 * Popup windows automatically close themselves when they do not
	 * have the focus anymore and the focus is not owned by a popup
	 * window lying on top of this popup window. I.e. if no popup
	 * window has focus, all popup windows are destroyed. If a popup
	 * window gains focus, all popup windows lying on top of the popup
	 * window are destroyed, too.
	 */
	class Popup : public CachableComponent
	{
		private:

			/**
			 * Whether this window is already on the way to be destroyed,
			 * i.e. the destroy effect is already issued.
			 */
			bool destroying;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Popup);
			#endif

		public:

			/**
			 * Creates a new popup.
			 */
			Popup();

			/**
			 * Cancels the popup and destroys it.
			 * Call this method if you want to initiate closing the popup manually.
			 * E.g., the method is also called when the user clicks next to the popup.
			 */
			virtual void cancel();

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
			 * Returns whether this window is already on the way to be destroyed,
			 * i.e. the destroy effect is already issued.
			 * @return Whether this window is already on the way to be destroyed.
			 */
			bool isDestroying();

			/**
			 * Handles the cancel event on the event target.
			 * On Windows systems, this is the ESC key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCancel();

			/**
			 * First notifies the listeners of the deepest grand-child of a focus loss,
			 * then its parent, and so on, until this component is reached,
			 * which is then notified (calling the listeners' onLostFocus() method).
			 * @see Listener::onLostFocus()
			 */
			virtual void onLostFocus();

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
