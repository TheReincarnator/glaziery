/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_IMAGE_H
#define __GLAZIERY_IMAGE_H


namespace glaziery
{
	/**
	 * A image is a resource managed by the platform adapter
	 * that provides rendering of a bitmap read from a file resource.
	 * Image bitmaps contain an alpha channel, which enables blending.
	 */
	class Image : public Listenable
	{
		friend ArrayList<Image>;
		friend HashMap<Image>;
		friend PlatformAdapter;

		public:

			/**
			 * A listener for image events, such as render target destruction.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * image anymore. So you should not hold any references to them.
			 */
			class Listener : public Listenable::Listener
			{
				public:

					/**
					 * Handles the event that a render target image has been invalidated,
					 * e.g. the physical device has been lost.
					 * @param renderTarget The invalidated render target.
					 */
					virtual void onRenderTargetInvalidated(Image * renderTarget);
			};

		public:

			/**
			 * The image type when creating an image.
			 * @see PlatformAdapter::createImage()
			 */
			enum Type
			{
				/**
				 * The image contains color (RGB) information, but no alpha channel.
				 * When rendering to this image, alpha information in the source
				 * is used to blend the source with the existing image content.
				 * When rendering the image to another image or to the desktop,
				 * the content will be considered as opaque (only the alpha value
				 * supplied to the respective operation is used).
				 * @see PlatformAdapter::createImage()
				 */
				TYPE_RGB,

				/**
				 * The image contains color (RGB) information and an alpha channel.
				 * When rendering to this image, alpha information in the source
				 * is set into this target image. Blending is not performed in this
				 * case, i.e. the color and alpha information is rendered opaquely.
				 * When rendering the image to another image or to the desktop,
				 * the alpha channel will be used for blending.
				 */
				TYPE_RGB_ALPHA,

				/**
				 * The image contains an alpha channel only.
				 * When rendering to this image, alpha information in the source
				 * is set into this target image. Blending is not performed in this,
				 * case i.e. the alpha information is rendered opaquely.
				 */
				TYPE_ALPHA
			};

		protected:

			/**
			 * The type of the image (whether the image contains RGB and/or alpha information).
			 */
			Type imageType;

			/**
			 * The maximum size of this image. Resize and image load operations are constrained to this size.
			 * Defaults to (0,0), which does not constrain.
			 */
			Vector maximumSize;

			/**
			 * The path of the image file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 * An empty string indicates a render target that isn't loaded from a file.
			 */
			String path;

			/**
			 * The current size of this image.
			 */
			Vector size;

			/**
			 * Destroys the image.
			 */
			virtual ~Image();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Image);
			#endif

		public:

			/**
			 * Creates a new image.
			 * @param path The path of the image file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 * An empty string indicates a render target that isn't loaded from a file.
			 * @param imageType The type of the image (whether the image contains RGB
			 * and/or alpha information).
			 */
			Image(const String & path, Type imageType);

			/**
			 * Ensures that the image is loaded, if it is file-based.
			 * Does nothing if it is a render target.
			 * @see PlatformAdapter::getImage()
			 */
			virtual void ensureLoaded() = NULL;

			/**
			 * Returns the type of the image (whether the image contains RGB
			 * and/or alpha information).
			 * @return The type of the image.
			 */
			Type getImageType();

			/**
			 * Returns the maximum size of this image. Resize and image load operations are constrained to this size.
			 * Defaults to (0,0), which does not constrain.
			 * @return The maximum size of this image.
			 */
			Vector getMaximumSize();

			/**
			 * Returns the path of the image file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 * An empty string indicates a render target that isn't loaded from a file.
			 * @return The path of the image file or an empty string.
			 */
			const String & getPath();

			/**
			 * Returns the current size of the image.
			 * @return The current size of the image.
			 */
			virtual Vector getSize();

			/**
			 * Returns whether the image has an alpha channel.
			 * @return Whether the image has an alpha channel.
			 */
			bool hasAlpha();

			/**
			 * Check that the given image's size is at least as large as a given size.
			 * You can reserve some additional space to reduce reallocation,
			 * and you can reduce the space if the new size is smaller.
			 *
			 * @param size The required size.
			 */
			virtual void resize(Vector size) = NULL;

			/**
			 * Sets the maximum size of this image. Resize and image load operations are constrained to this size.
			 * Defaults to (0,0), which does not constrain.
			 * @param maximumSize The maximum size of this image.
			 */
			void setMaximumSize(Vector maximumSize);

			/**
			 * Ensures that the image is not loaded, if it is file-based, releasing system resources.
			 * Does nothing if it is a render target.
			 * @see PlatformAdapter::getImage()
			 */
			virtual void unload() = NULL;
	};
}


#endif
