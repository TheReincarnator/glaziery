/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_CLIENTAREA_H
#define __GLAZIERY_CLIENTAREA_H


namespace glaziery
{
	/**
	 * The client area is the main part of a window, defining the client
	 * content.
	 * @note If you implement a class that derives from ClientArea,
	 * remember to call the window's invalidate() method in every method
	 * that changes something concerning the graphical representation
	 * (in any skin, if applicable).
	 */
	class ClientArea : public CachableComponent
	{
		friend ArrayList<ClientArea>;
		friend Window;

		private:

			/**
			 * The background color of this fields area.
			 */
			unsigned long backgroundColor;

			/**
			 * A lookup mapping hot key characters to corresponding components
			 * (e.g. input fields).
			 */
			IntKeyHashMap<Component> hotKeyComponents;

			/**
			 * Dummy panes array for client areas that do not support child panes.
			 */
			ArrayList<ClientArea> noPanes;

		protected:

			/**
			 * Destroys the client area.
			 */
			virtual ~ClientArea();

			/**
			 * Returns the sub client area (pane) that currently has focus.
			 * The focus pane e.g. receives all key events of the tabbing area.
			 * if no pane has focus or this client area does not support child panes,
			 * the method returns NULL.
			 * @return The current focus pane or NULL.
			 */
			virtual ClientArea * getFocusPane();

			/**
			 * Returns the sub client areas (panes) of this tabbing area
			 * or an empty array, if this client area does not support child panes.
			 * @return The sub client areas (panes) of this tabbing area.
			 */
			virtual const ArrayList<ClientArea> & getPanes();

			/**
			 * Sets the sub client area (pane) that shall get focus.
			 * The focus pane e.g. receives all key events of the splitting area then.
			 * The default implementation does nothing, this is the default for client
			 * areas that do not support child panes.
			 * @param focusPane The new focus pane.
			 */
			virtual void setFocusPane(ClientArea * focusPane);

			/**
			 * Sets the focus pane to the first available and valid pane
			 * and also sets its pane's focus pane to the first one, and so on.
			 */
			void setFocusToFirstPaneDeeply();

			/**
			 * Sets the focus pane to the next available and valid pane.
			 * The method rolls around the end of the panes list.
			 * @return Whether the method rolled around the end of the panes list.
			 * @note The invocation is first propagated to the focused child area.
			 * Only if the focus pane rolls around the end of the panes list,
			 * this area steps to the next pane. If the child area does not support
			 * focusing, this area steps to the next pane, too.
			 */
			bool setFocusToNextPane();

			/**
			 * Sets the focus pane to the last available and valid pane
			 * and also sets its pane's focus pane to the last one, and so on.
			 */
			void setFocusToLastPaneDeeply();

			/**
			 * Sets the focus pane to the previous available and valid pane.
			 * The method rolls around the beginning of the panes list.
			 * @return Whether the method rolled around the beginning of the panes list.
			 * @note The invocation is first propagated to the focused child area.
			 * Only if the focus pane rolls around the beginning of the panes list,
			 * this area steps to the previous pane. If the child area does not support
			 * focusing, this area steps to the previous pane, too.
			 */
			bool setFocusToPreviousPane();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ClientArea);
			#endif

		public:

			/**
			 * Creates a new client area.
			 */
			ClientArea();

			/**
			 * Adds a hot key for a component. Use this method when implementing
			 * a component that has a hot key text, e.g. a sub client area (pane) tab
			 * in a tabbing area, and call it when setting the text. Note that
			 * the Field class provides a method to set a field's text. This method
			 * also calls addHotKey(), so in this case, you don't need to.
			 * @param hotKey The hot key character to be set.
			 * @param component The component that is associated to the hot key.
			 * @see Field::setText()
			 */
			void addHotKey(char hotKey, Component * component);

			/**
			 * Searches a text for hot key indicators (&).
			 * If one is found, the character after the first indicator is returned.
			 * @param text The text to search for hot keys in.
			 * @return The first hot key or 0, if no hot key is found.
			 */
			static char findHotKey(const String & text);

			/**
			 * Returns the background color of this fields area.
			 * @return The background color of this fields area.
			 */
			unsigned long getBackgroundColor();

			/**
			 * Returns the component that is currently assigned to a given hot key.
			 * @param hotKey The hot key character to be found.
			 * It is safe to provide 0: In this case, the method returns NULL.
			 * @return The component that is assigned to the hot key or NULL,
			 * if no component is assigned to the key.
			 */
			Component * getHotKeyComponent(char hotKey);

			/**
			 * Returns the position relative to the absolute window coordinates.
			 * @return The window relative position.
			 */
			Vector getPositionOnWindow();

			/**
			 * Handles the event that the event target is pressed down
			 * (usually using the left mouse button).
			 * @param position The position where the click occurred
			 * relative to the upper-left corner of the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onRelease()
			 */
			virtual void onPress(Vector position, bool option1, bool option2);

			/**
			 * Returns the window this component is directly or indirectly contained in.
			 * @return The window or NULL, if this component is not yet attached.
			 */
			Window * getWindow();

			/**
			 * Removes a hot key from a component. Use this method when implementing
			 * a component that has a hot key text, e.g. a sub client area (pane) tab
			 * in a tabbing area, and call it when setting the text, and when deleting
			 * the object. Note that the Field class provides a method to set a
			 * field's text. This method also calls removeHotKey(), so in this case,
			 * you don't need to.
			 * @param hotKey The hot key character to be removed.
			 * It is safe to provide 0: In this case, the method does nothing.
			 */
			void removeHotKey(char hotKey);

			/**
			 * Sets the background color of this fields area.
			 * @param backgroundColor The background color of this fields area.
			 */
			void setBackgroundColor(unsigned long backgroundColor);

			/**
			 * Shows the component (makes it visible again).
			 */
			virtual void show();

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
