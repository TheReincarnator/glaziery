/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SIMPLEPOINTERTYPE_H
#define __GLAZIERY_SIMPLEPOINTERTYPE_H


namespace glaziery
{
	/**
	 * The simple pointer type is a pointer type implementation
	 * that displays one image at the pointer position, without any effects.
	 */
	class SimplePointerType : public PointerType
	{
		protected:

			/**
			 * The hot spot is the position within the pointer
			 * image that corresponds to the pointer position. The pointer
			 * image is translated negatively by the hot spot offset
			 * to draw it at the current pointer position.
			 */
			Vector hotSpot;

			/**
			 * The image for the pointer.
			 */
			Image * image;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(SimplePointerType);
			#endif

		public:

			/**
			 * Creates a new simple pointer type.
			 * @param hotSpot The hot spot is the position within the pointer
			 * image that corresponds to the pointer position. The pointer
			 * image is translated negatively by the hot spot offset
			 * to draw it at the current pointer position.
			 * @param image The image for the pointer.
			 */
			SimplePointerType(Image * image, Vector hotSpot);

			/**
			 * Draws the pointer at a given position.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param position The position to draw the pointer at.
			 */
			virtual void draw(DrawingContext & context, Vector position);

			/**
			 * Returns the hot spot is the position within the pointer
			 * image that corresponds to the pointer position. The pointer
			 * image is translated negatively by the hot spot offset
			 * to draw it at the current pointer position.
			 * @return The hot spot position.
			 */
			Vector getHotSpot();

			/**
			 * Returns the image for the pointer.
			 * @return The image for the pointer.
			 */
			Image * getImage();
	};
}


#endif
