/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DRAWINGCONTEXT_H
#define __GLAZIERY_DRAWINGCONTEXT_H


namespace glaziery
{
	/**
	 * The drawing context specifies the target and the clipping rectangle
	 * for drawing operations. The drawing context is created by the desktop
	 * and the components, passed through the skin and finally passed to
	 * the platform adapter.
	 * @see Desktop
	 * @see Component::draw()
	 * @see Skin
	 * @see PlatformAdapter
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class DrawingContext : public CObject
	#else
		class DrawingContext
	#endif
	{
		private:

			/**
			 * The upper-left corner of the clipping rectangle.
			 * All drawing operations left or above this point are clipped.
			 * @note The drawing offset is applied first, before clipping.
			 */
			Vector clippingOffset;

			/**
			 * The clipping rectangle size.
			 * Drawing right or below (clippingOffset + clippingSize, including) is clipped.
			 * The drawing offset is applied first, before clipping.
			 */
			Vector clippingSize;

			/**
			 * The drawing offset, absolute to the target's origin.
			 * All drawing operations on the target are translated by this offset
			 * (i.e. originated there).
			 * This offset is applied to all coordinates before clipping
			 * and drawing is performed.
			 */
			Vector drawingOffset;

			/**
			 * The target to perform drawing operations on or NULL,
			 * if the desktop background area is the target.
			 * This the latter case, the clipping rectangle equals the desktop,
			 * and the drawing offset is (0, 0).
			 */
			Image * target;

			/**
			 * Calculates the clipping intersection of the current context
			 * and given offset and size parameters, and updates the
			 * properties of the context.
			 * @note This method is used by the constructors. See there.
			 * @param offset The drawing and clipping offset.
			 * All drawing operations' coordinates is relative to this point.
			 * Drawing left or above this point is clipped.
			 * If the offset lies outside the target's rectangle,
			 * drawing is clipped additionally.
			 * @param size The clipping rectangle size.
			 * Drawing right or below (offset + size) (including) is clipped.
			 * If the clipping rectangle lies outside the target's rectangle,
			 * drawing is clipped additionally.
			 */
			void clipTo(Vector offset, Vector size);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DrawingContext);
			#endif

		public:

			/**
			 * Creates a void drawing context (e.g. to initialize other objects).
			 */
			DrawingContext();

			/**
			 * Creates a drawing context for the desktop background area
			 * drawing on the entire target (no clipping).
			 * @param desktop The desktop.
			 */
			DrawingContext(Desktop * desktop);

			/**
			 * Creates a drawing context for a given target (e.g. window)
			 * drawing on the entire target (no clipping).
			 * @param target The target to perform drawing operations on.
			 */
			DrawingContext(Image * target);

			/**
			 * Creates a drawing context from a given one (copy constructor).
			 * The new context has the same properties as the template.
			 * @param context The context template.
			 */
			DrawingContext(const DrawingContext & context);

			/**
			 * Creates a drawing context as a sub context of a given one
			 * with a given translation offset (e.g. for scrolling components).
			 * Drawing operations are translated (added) by this offset,
			 * so this equals the negative scrolling position.
			 * Clipping is not changed.
			 * @param context The outer context. This must be a target
			 * context (not a desktop drawing context).
			 * The new context's target is taken from this context.
			 * @param offset The drawing offset.
			 * Drawing operations are translated (added) by this offset,
			 * so this equals the negative scrolling position.
			 */
			DrawingContext(const DrawingContext & context, Vector offset);

			/**
			 * Creates a drawing context as a sub context of a given one.
			 * The clipping rectangles are intersected, and the offset is calculated
			 * from the new offset relative to the old one.
			 * Drawing operations are offset relatively to this rectangle.
			 * @param context The outer context. This must be a target
			 * context (not a desktop drawing context).
			 * The new context's target is taken from this context.
			 * @param offset The drawing and clipping offset within the context
			 * template (i.e. relative to the old context's offset). All drawing
			 * operations' coordinates are relative to this new offset.
			 * Drawing left or above this point is clipped.
			 * @param size The clipping rectangle size.
			 * The rectangle is intersected with the old one to meet both restrictions.
			 * Drawing right or below (offset + size) (including) is clipped.
			 * If the clipping rectangle lies outside the target's rectangle,
			 * drawing is clipped additionally.
			 */
			DrawingContext(const DrawingContext & context, Vector offset, Vector size);

			/**
			 * Creates a drawing context as a sub context of a given one
			 * using a component's position and size as clipping offset and size.
			 * The clipping rectangles are intersected, and the offset is calculated
			 * from the new offset relative to the old one.
			 * Drawing operations are offset relatively to this rectangle.
			 * @param context The outer context. This must be a target
			 * context (not a desktop drawing context).
			 * The new context's target is taken from this context.
			 * @param component The component to use the position and size of
			 * as clipping rectangle's offset and size.
			 */
			DrawingContext(const DrawingContext & context, Component * component);

			/**
			 * Sets all the surface, clipping, and drawing properties
			 * of the drawing context to those of another one.
			 * @param drawingContext The other drawing context.
			 */
			void operator =(const DrawingContext & drawingContext);

			/**
			 * Returns whether all the surface, clipping, and drawing properties
			 * of the drawing context equal another one.
			 * @param drawingContext The other drawing context.
			 * @return Whether both contexts are equal.
			 */
			bool operator ==(const DrawingContext & drawingContext) const;

			/**
			 * Returns whether any of the surface, clipping, and drawing properties
			 * of the drawing context do not equal another one.
			 * @param drawingContext The other drawing context.
			 * @return Whether both contexts are not equal.
			 */
			bool operator !=(const DrawingContext & drawingContext) const;

			/**
			 * Returns the upper-left corner of the clipping rectangle.
			 * All drawing operations left or above this point is clipped.
			 * The drawing offset is applied first, before clipping.
			 * @return The clipping offset.
			 */
			inline Vector getClippingOffset();

			/**
			 * Returns the clipping rectangle size.
			 * Drawing right or below (clippingOffset + clippingSize) (including) is clipped.
			 * The drawing offset is applied first, before clipping.
			 * @return The clipping rectangle size.
			 */
			inline Vector getClippingSize();

			/**
			 * Returns the drawing offset. All drawing operations on the target
			 * are translated by this offset (i.e. originated there).
			 * This offset is applied to all coordinates before clipping
			 * and drawing is performed.
			 * @return The drawing offset.
			 */
			inline Vector getDrawingOffset();

			/**
			 * Returns the target to perform drawing operations on or NULL,
			 * if the desktop background area is the target.
			 * This the latter case, the clipping rectangle equals the desktop,
			 * and the drawing offset is (0, 0).
			 * @return The target to perform drawing operations on or NULL
			 * for the desktop background area.
			 */
			inline Image * getTarget();

			/**
			 * Returns whether the surface and clipping properties
			 * of the drawing context equal another one.
			 * In this case, the platform adapter does not need to focus on a different clipping rectangle.
			 * @param drawingContext The other drawing context.
			 * @return Whether both contexts are compatible.
			 */
			bool isCompatibleTo(const DrawingContext & drawingContext) const;

			/**
			 * Returns whether a given vector is within the clipping rectangle of this context.
			 * The given rectangles coordinates must be translated by the drawing offsets before this call.
			 * @param position The position to test.
			 * @return Whether the vector is within the clipping rectangle.
			 */
			bool isInClippingArea(Vector position) const;

			/**
			 * Returns whether a given rectangle intersects with the clipping rectangle of this context.
			 * The given rectangles coordinates must be translated by the drawing offsets before this call.
			 * @param from The from coordinates, may be any of the four corners.
			 * @param to The to coordinates, must be the opposite corner of the from vector.
			 * @return Whether the rectangles intersect.
			 */
			bool isInClippingArea(Vector from, Vector to) const;
	};
}


#endif
