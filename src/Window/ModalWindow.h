/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_MODALWINDOW_H
#define __GLAZIERY_MODALWINDOW_H


namespace glaziery
{
	/**
	 * A modal window is a window that support a special mode where the user
	 * is constraint to react to this window before being able to operate with
	 * other parts of the Desktop. Programmatically, the run stays in a special
	 * method of the modal window, runModally(), and the Glaziery main loop is
	 * called recursively. Thus, the application (or game) may use modal windows
	 * to display dialogs and continue working with the data entered by the user
	 * just after having called runModally().
	 * @see runModally()
	 */
	class ModalWindow : public Window
	{
		protected:

			/**
			 * Whether to automatically show the (maybe initially hidden) modal window
			 * when running into the modal loop (runModally, e.g. Dialog::showDialog).
			 * Defaults to true.
			 * Use this property to delay showing the window until some event.
			 */
			bool autoShowing;

			/**
			 * Whether the current modal loop run (runModally()) is about to end.
			 * @see runModally()
			 */
			bool endingModalRun;

			/**
			 * Destroys the modal window.
			 */
			virtual ~ModalWindow();

			/**
			 * Performs operations when a modal run ends.
			 * @note Implement this method in derivatives, if necessary.
			 */
			virtual void onModalEnd();

			/**
			 * Performs operations when a modal run starts.
			 * @note Implement this method in derivatives, if necessary.
			 */
			virtual void onModalStart();

			/**
			 * Performs operations for each frame while the window is running modally.
			 * @note Implement this method in derivatives, if necessary.
			 */
			virtual void onModalUpdate();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ModalWindow);
			#endif

		public:

			/**
			 * Creates a new modal window.
			 */
			ModalWindow();

			/**
			 * Gracefully tries to close the window, removing it from the Glaziery system.
			 * This method should be used for regular window closes with user interaction.
			 * First, all window listeners are notified using onClosing().
			 * They may veto the window close. If noone does, they are again notified
			 * about the close having performed, using onClosed(). The window is then
			 * destroyed by adding it to the list of disposable components, which finally
			 * destroys it on next call of Desktop::draw().
			 * @note The component is maybe added to the list of disposable components.
			 * Nevertheless, the memory is still available until the next call to
			 * Desktop::draw() (the next frame).
			 * @note Use Desktop::removeWindow() to remove the window ungracefully,
			 * without user interaction and close notification.
			 * @return Whether the close was performed. Any veto results in false.
			 * @see Listener::onClosing()
			 * @see Listener::onClosed()
			 */
			bool close();

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
			 * Issues to end the current modal loop run (runModally()).
			 * @see isEndingModalRun()
			 * @see runModally()
			 */
			void endModalRun();

			/**
			 * Returns whether to automatically show the (maybe initially hidden) modal window
			 * when running into the modal loop (runModally, e.g. Dialog::showDialog).
			 * Defaults to true.
			 * @return Whether to automatically show the window.
			 */
			bool isAutoShowing();

			/**
			 * Returns whether the current modal loop run (runModally()) is about to end.
			 * @return Whether the current modal run is about to end.
			 */
			bool isEndingModalRun();

			/**
			 * Runs Glaziery with the window in modal mode.
			 * It behaves similarly to the desktop's run() method, but events
			 * are only passed to this window, which is brought to front first.
			 * The method does not return before a modal button has been pressed.
			 * @warning This method causes a recursion in the event handling loop.
			 * Your effected classes should be able to handle respective reentrance
			 * and you should not cause an endless recursion.
			 * @see Desktop::run()
			 * @see Button
			 * @see Button::isModal()
			 */
			void runModally();

			/**
			 * Sets whether to automatically show the (maybe initially hidden) modal window
			 * when running into the modal loop (runModally, e.g. Dialog::showDialog).
			 * Defaults to true.
			 * @param autoShowing Whether to automatically show the window.
			 */
			void setAutoShowing(bool autoShowing);
	};
}


#endif
