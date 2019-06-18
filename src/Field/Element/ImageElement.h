/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_IMAGEELEMENT_H
#define __GLAZIERY_IMAGEELEMENT_H


namespace glaziery
{
	/**
	 * Image elements are field elements that display an image.
	 */
	class ImageElement : public FieldElement
	{
		private:

			/**
			 * The image of the element. May be NULL, displaying no image.
			 */
			Image * image;

		protected:

			/**
			 * Destroys the image element.
			 */
			virtual ~ImageElement();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ImageElement);
			#endif

		public:

			/**
			 * Creates a new image element.
			 */
			ImageElement();

			/**
			 * Draws the content of the field element to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation. This context is already translated
			 * for the current field element position and available size
			 * (e.g. list field column).
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Opens a popup to edit the field element.
			 * This method is typically called by the field (e.g. list or tree field)
			 * when receiving the onEdit() event.
			 * @param position The absolute position of the top-left corner
			 * of the inner edit field of the popup.
			 * @param size The size of the inner edit field of the popup.
			 * @param scrollingField The scrolling field the popup is created for,
			 * or NULL, if the popup is not created for any scrolling field.
			 * Setting a scrolling field enables the popup to move if the scrolling field is scrolled.
			 * Defaults to NULL.
			 */
			virtual void edit(Vector position, Vector size, ScrollingField * scrollingField = NULL);

			/**
			 * Returns the image of the element. May be NULL, displaying no image.
			 * @return The image of the element or NULL.
			 */
			Image * getImage();

			/**
			 * Returns the size of the field element.
			 * @return The size of the field element.
			 */
			virtual Vector getSize();

			/**
			 * Sets the image of the element. May be NULL, displaying no image.
			 * @param image The image to be set or NULL.
			 */
			void setImage(Image * image);

			/**
			 * Sets the image of the element by its ID.
			 * @param imageId The ID of the image to be set.
			 */
			void setImageId(const String & imageId);

			/**
			 * Sets the image of the element by its ID.
			 * @param imageId The ID of the image to be set.
			 */
			void setImageId(const char * imageId);
	};
}


#endif
