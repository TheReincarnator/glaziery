/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_IMAGEFIELD_H
#define __GLAZIERY_IMAGEFIELD_H


#define GLAZIERY_IMAGEFIELD(variable, area, groupField, \
	image) \
	ImageField * variable; \
	if ((variable = new ImageField) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	if (image != NULL) \
		variable->setImage(image);

#define GLAZIERY_IMAGEFIELD_P(variable, area, groupField, px, py, \
	image) \
	GLAZIERY_IMAGEFIELD(variable, area, groupField, image) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_IMAGEFIELD_S(variable, area, groupField, width, height, \
	image) \
	GLAZIERY_IMAGEFIELD(variable, area, groupField, image) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_IMAGEFIELD_PS(variable, area, groupField, px, py, width, height, \
	image) \
	GLAZIERY_IMAGEFIELD(variable, area, groupField, image) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_IMAGEFIELD_M(variable, area, groupField, minimumWidth, minimumHeight, \
	image) \
	GLAZIERY_IMAGEFIELD(variable, area, groupField, image) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_IMAGEFIELD_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	image) \
	GLAZIERY_IMAGEFIELD(variable, area, groupField, image) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_IMAGEFIELD_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	image) \
	GLAZIERY_IMAGEFIELD(variable, area, groupField, image) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_IMAGEFIELD_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	image) \
	GLAZIERY_IMAGEFIELD(variable, area, groupField, image) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_IMAGEFIELD_I(variable, area, groupField, \
	imageId) \
	ImageField * variable; \
	if ((variable = new ImageField) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setImageId(imageId);

#define GLAZIERY_IMAGEFIELD_IP(variable, area, groupField, px, py, \
	imageId) \
	GLAZIERY_IMAGEFIELD_I(variable, area, groupField, imageId) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_IMAGEFIELD_IS(variable, area, groupField, width, height, \
	imageId) \
	GLAZIERY_IMAGEFIELD_I(variable, area, groupField, imageId) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_IMAGEFIELD_IPS(variable, area, groupField, px, py, width, height, \
	imageId) \
	GLAZIERY_IMAGEFIELD_I(variable, area, groupField, imageId) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_IMAGEFIELD_IM(variable, area, groupField, minimumWidth, minimumHeight, \
	imageId) \
	GLAZIERY_IMAGEFIELD_I(variable, area, groupField, imageId) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight));

#define GLAZIERY_IMAGEFIELD_IPM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	imageId) \
	GLAZIERY_IMAGEFIELD_I(variable, area, groupField, imageId) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight));

#define GLAZIERY_IMAGEFIELD_ISM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	imageId) \
	GLAZIERY_IMAGEFIELD_I(variable, area, groupField, imageId) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_IMAGEFIELD_IPSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	imageId) \
	GLAZIERY_IMAGEFIELD_I(variable, area, groupField, imageId) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));


namespace glaziery
{
	class ImageField : public Field
	{
		private:

			/**
			 * The image to be displayed. May be NULL, displaying no image.
			 */
			Image * image;

		protected:

			/**
			 * Destroys the image field.
			 */
			virtual ~ImageField();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ImageField);
			#endif

		public:

			/**
			 * Creates a new image field.
			 */
			ImageField();

			/**
			 * Draws the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The caller is responsible for not calling this method
			 * if the component is invisible.
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Returns the image to be displayed. May be NULL, displaying no image.
			 */
			Image * getImage();

			/**
			 * Sets the image to be displayed. May be NULL, displaying no image.
			 * @param image The image to be set or NULL.
			 */
			void setImage(Image * image);

			/**
			 * Sets the image to be displayed by its ID.
			 * @param imageId The ID of the image to be set.
			 */
			void setImageId(const String & imageId);

			/**
			 * Sets the image to be displayed by its ID.
			 * @param imageId The ID of the image to be set.
			 */
			void setImageId(const char * imageId);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
