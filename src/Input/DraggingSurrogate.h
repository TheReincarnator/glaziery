/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DRAGGINGSURROGATE_H
#define __GLAZIERY_DRAGGINGSURROGATE_H


namespace glaziery
{
	/**
	 * A dragging surrogate draws a dragged object (event target, list field row, etc.)
	 * next to the pointer, while dragging it. Also defines the pointer targets while
	 * dragging over a possible and an unsupporting event target.
	 * Dragging surrogates are created by the source event target when the dragging starts,
	 * and will be deleted when dragging ends.
	 * @see EventTarget
	 */
	class DraggingSurrogate : public GlazieryObject
	{
		friend Desktop;
		friend PointerButtonEvent;
		friend PointerMoveEvent;

		protected:

			/**
			 * The offset of the surrogate relative to the pointer.
			 * This value is calculated when the dragging starts,
			 * and can be used when drawing the surrogate.
			 */
			Vector offset;

			/**
			 * The pointer type to show when dragging over an event target
			 * that supports dropping of the currently dragged object.
			 * NULL (which is the default) shows the skin's default dragging pointer type.
			 */
			PointerType * supportingPointerType;

			/**
			 * The pointer type to show when dragging over an event target
			 * that does not support dropping of the currently dragged object.
			 * NULL (which is the default) shows the skin's default impossible pointer type.
			 */
			PointerType * unsupportingPointerType;

			/**
			 * The offset of the surrogate relative to the pointer.
			 * This value is calculated when the dragging starts,
			 * and can be used when drawing the surrogate.
			 * @param offset The offset of the surrogate relative to the pointer.
			 */
			void setOffset(Vector offset);

		protected:

			/**
			 * Destroys the dragging surrogate.
			 */
			virtual ~DraggingSurrogate();

			/**
			 * Creates a new dragging surrogate.
			 * @param offset The offset of the surrogate relative to the pointer.
			 * This value is calculated when the dragging starts,
			 * and can be used when drawing the surrogate.
			 * @param supportingPointerType The pointer type to show when dragging over an event target
			 * that supports dropping of the currently dragged object.
			 * NULL (which is the default) shows the skin's default dragging pointer type.
			 * @param unsupportingPointerType The pointer type to show when dragging over an event target
			 * that does not support dropping of the currently dragged object.
			 * NULL (which is the default) shows the skin's default impossible pointer type.
			 */
			DraggingSurrogate(Vector offset, PointerType * supportingPointerType = NULL,
				PointerType * unsupportingPointerType = NULL);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DraggingSurrogate);
			#endif

		public:

			/**
			 * Creates a new dragging surrogate.
			 */
			DraggingSurrogate();

			/**
			 * Helper to easily creates a default dragging surrogate (using the default skin pointer types).
			 */
			static DraggingSurrogate * createDefault();

			/**
			 * Draws the dragging surrogate next to the pointer, using the offset.
			 * By default, draws nothing. This is useful to define the pointer types, only.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Returns the offset of the surrogate relative to the pointer.
			 * This value is calculated when the dragging starts,
			 * and can be used when drawing the surrogate.
			 * @return The surrogate drawing offset.
			 */
			Vector getOffset();

			/**
			 * Returns the pointer type to show when dragging over an event target
			 * that supports dropping of the currently dragged object.
			 * NULL (which is the default) shows the skin's default dragging pointer type.
			 * @note Override this method to provide a dynamic pointer type.
			 * The default implementation returns the value specified in the constructor.
			 * @return The supporting pointer type.
			 */
			virtual PointerType * getSupportingPointerType();

			/**
			 * Returns the pointer type to show when dragging over an event target
			 * that does not support dropping of the currently dragged object.
			 * NULL (which is the default) shows the skin's default impossible pointer type.
			 * @note Override this method to provide a dynamic pointer type.
			 * The default implementation returns the value specified in the constructor.
			 * @return The unsupporting pointer type.
			 */
			virtual PointerType * getUnsupportingPointerType();

			/**
			 * Handles the event that the drag-drop operation is canceled.
			 * This may happen when using the cancel key (ESC on Windows systems)
			 * or when the Desktop's cancelDragging() method is called.
			 * @param draggedObject The event target that has been dragged.
			 * @see Desktop::cancelDragging()
			 * @see SpecialEvent::EVENT_CANCEL
			 */
			virtual void onCanceled(EventTarget * draggedObject);

			/**
			 * Handles the event that the pointer is moved during a dragging operation.
			 * @param draggedObject The event target that has been dragged.
			 * @param startPosition The absolute position where the dragging started.
			 * @param lastPosition The absolute position before the last move step.
			 * @param currentPosition The new absolute position.
			 */
			virtual void onDragMove(EventTarget * draggedObject, const Vector & startPosition,
				const Vector & lastPosition, const Vector & currentPosition);

			/**
			 * Handles the event that a dragging operation successfully ended.
			 * This is when a dragging operation is in progress and the primary pointer
			 * button has been released.
			 * @param draggedObject The event target that has been dragged.
			 * @param dropTarget The event target where the object has been dropped.
			 * This may be, among others, the desktop.
			 * @param relativePosition The relative position within the drop target.
			 * If the dropTarget is the desktop, this is the absolute pointer position.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onDropped(EventTarget * draggedObject, EventTarget * dropTarget,
				Vector relativePosition, bool option1, bool option2);

			/**
			 * Sets the pointer type to show when dragging over an event target
			 * that supports dropping of the currently dragged object.
			 * NULL (which is the default) shows the skin's default dragging pointer type.
			 * @param supportingPointerType The supporting pointer type.
			 */
			void setSupportingPointerType(PointerType * supportingPointerType);

			/**
			 * Sets the pointer type to show when dragging over an event target
			 * that does not support dropping of the currently dragged object.
			 * NULL (which is the default) shows the skin's default impossible pointer type.
			 * @param unsupportingPointerType The supporting pointer type.
			 */
			void setUnsupportingPointerType(PointerType * unsupportingPointerType);

			/**
			 * Returns whether the dragging surrogate supports dropping on a given event target.
			 * By default, dropping is not supported at any position.
			 * @param draggedObject The event target that is dragged.
			 * @param dropTarget The event target where the object may be dropped.
			 * This may be, among others, the desktop.
			 * @param relativePosition The relative position within the drop target.
			 * If the dropTarget is the desktop, this is the absolute pointer position.
			 * @return Whether the event target supports dropping of the dragged object
			 * at the position.
			 * @see getOrigin()
			 */
			virtual bool supportsDropping(EventTarget * draggedObject,
				EventTarget * dropTarget, Vector relativePosition);
	};
}


#endif
