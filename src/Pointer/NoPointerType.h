/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_NOPOINTERTYPE_H
#define __GLAZIERY_NOPOINTERTYPE_H


namespace glaziery
{
	/**
	 * The no pointer type is a pointer type implementation that does not draw any pointer at all.
	 */
	class NoPointerType : public PointerType
	{
		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(NoPointerType);
			#endif

		public:

			/**
			 * Draws the pointer at a given position.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param position The position to draw the pointer at.
			 */
			virtual void draw(DrawingContext & context, Vector position);
	};
}


#endif
