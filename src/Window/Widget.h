/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_WIDGET_H
#define __GLAZIERY_WIDGET_H


namespace glaziery
{
	/**
	 * A widget (window gadget) is a small part of a component, an interactive
	 * graphical object, with skin-defined pointer behavior.
	 * Since widgets are event targets, their general pointer handling applies.
	 * Examples for widgets are the window title bar, the window close widget,
	 * and the selector pull-down widget. Skins create widgets in their setup
	 * methods and add them to the components.
	 * @see Skin
	 * @see Component
	 */
	class Widget : public EventTarget
	{
		friend ArrayList<Widget>;
		friend Component;

		private:

			/**
			 * The component this widget belongs to.
			 */
			Component * component;

		protected:

			/**
			 * Destroys the widget.
			 */
			virtual ~Widget();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Widget);
			#endif

		public:

			/**
			 * Creates a new widget.
			 */
			Widget();

			/**
			 * Returns the origin for relative position values.
			 * E.g. for components, this is their absolute position,
			 * and for widgets, it is the absolute position of their component.
			 */
			virtual Vector getOrigin();

			/**
			 * Returns the parent component, usually the container
			 * (e.g. the window for the client area).
			 * @return The parent component.
			 */
			virtual Component * getParent();

			/**
			 * Returns the pointer type to be displayed when the pointer is moved over the
			 * widget (where isHitAt() applies), and when dragging a dragging-enabled widget.
			 * The default returns NULL, which means to display the default pointer type
			 * (which usually is the select pointer).
			 * @param position The current position of the pointer, relative to the
			 * position of the widget's component.
			 * @return The pointer type to be displayed or NULL, indicating to display the
			 * default pointer type.
			 * @see isHitAt()
			 */
			virtual PointerType * getPointerType(Vector position);

			/**
			 * Returns whether the widgets area contains a given point.
			 * @param point The point to be tested, relative to the
			 * position of the widget's component.
			 * @return Whether the point lies within the widget.
			 */
			virtual bool isHitAt(Vector point) = NULL;

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
