/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_POINTERTYPE_H
#define __GLAZIERY_POINTERTYPE_H


namespace glaziery
{
	/**
	 * A pointer type defines the image and the hotspot of a pointer.
	 * Examples for pointer types are the select pointer and the horizontal
	 * resize pointer. Pointer types are defined by the skin.
	 * @see Skin
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class PointerType : public CObject
	#else
		class PointerType
	#endif
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(PointerType);
			#endif

		public:

			/**
			 * Destroys the pointer type.
			 */
			virtual ~PointerType();

			/**
			 * Draws the pointer at a given position.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param position The position to draw the pointer at.
			 */
			virtual void draw(DrawingContext & context, Vector position) = NULL;

			/**
			 * Handles the event that the pointer has moved, e.g. by creating an effect.
			 * The default implementation does nothing.
			 * @param newPosition The new position of the pointer.
			 * @param oldPosition The old position of the pointer.
			 */
			virtual void onPointerMoved(Vector newPosition, Vector oldPosition);
	};
}


#endif
