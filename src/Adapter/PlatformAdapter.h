/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_PLATFORMADAPTER_H
#define __GLAZIERY_PLATFORMADAPTER_H


namespace glaziery
{
	/**
	 * The platform adapter is the adapter to the underlying system libraries, such as
	 * DirectX or OpenGL. The adapter is used by the skin, but it may also be used by the
	 * application (or game), if an abstraction from the system libraries is required.
	 * The platform adapter provides primitive rendering operations such as lines,
	 * rectangles, images, font, and so on. It is set when initializing the desktop.<br>
	 *
	 * The choice of the right platform adapter typically results from the platform
	 * (PC, Mac, Linux, ...) and the rendering technology (DirectX, OpenGL, ...).
	 * You can choose one of the provided implementations,
	 * but you can also implement your own one.
	 *
	 * @see Desktop
	 * @see Desktop::createInstance()
	 * @see DirectXPlatformAdapter
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class PlatformAdapter : public CObject, public Configurable
	#else
		class PlatformAdapter : public Configurable
	#endif
	{
		public:

			enum BlendType
			{
				/**
				 * Renders the image, text, and/or modulation color as normal.
				 * The color of the source pixels is blended with the target pixels
				 * using the source alpha channel as blend factor (0 is transparent, 1 is opaque).
				 * The alpha of the source pixels is smooth-added to the target alpha,
				 * i.e. the new alpha as source + target - source*target.
				 * This gives a natural look when using the target as another draw's source.
				 */
				BLENDTYPE_NORMAL,

				/**
				 * Renders the image, text, and/or modulation color without blending.
				 * Both the color and the alpha channel of the source pixels are rendered
				 * on the target, overwriting the previous content.
				 * When using the target as another draw's source, if alpha is not fully supported,
				 * opaque is a good fallback.
				 */
				BLENDTYPE_OPAQUE,

				/**
				 * Renders only a silhouette of the image or text, using the modulation color as color.
				 * Color and alpha blending with the target is done exactly the same way as BLENDTYPE_NORMAL.
				 */
				BLENDTYPE_SILHOUETTE,

				/**
				 * Renders the alpha channel of the image or text directly to the target alpha channel.
				 * The source color is not used, and the target color is not modified.
				 * No blending takes place at all.
				 */
				BLENDTYPE_STENCIL
			};

		friend Desktop;
		friend Scene;

		protected:

			/**
			 * The currently managed file-based images.
			 * @see getImage()
			 */
			StringKeyHashMap<Image> fileBasedImages;

			/**
			 * The currently managed fonts.
			 * @see getFont()
			 */
			StringKeyHashMap<Font> fonts;

			/**
			 * The currently managed render target images.
			 * @see createImage()
			 */
			ArrayList<Image> renderTargetImages;

			/**
			 * Creates a new font object for a given font file path.
			 * @param path The path of the font file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 * @return The new font object.
			 */
			virtual Font * createFont(const String & path) = NULL;

			/**
			 * Creates a new image object from a file, or an empty render target.
			 * @param path The path of the font file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 * @param imageType The type of the image (whether the image contains RGB
			 * and/or alpha information).
			 * @return The new image object.
			 */
			virtual Image * createImage(const String & path, Image::Type imageType) = NULL;

			/**
			 * Draws a multi-colored text at a given position, using a given font
			 * and alpha blending.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param font The font to be used.
			 * @param position The top-left position of the text.
			 * @param text The text to be drawn.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the text opaquely.
			 * Using GL_WHITE_A(alpha), renders the text using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target.
			 * @param caps Whether to use all-capital characters.
			 * @param withHotKeys Whether ampersand characters ('&') are considered
			 * as hot key defining prefixes, resulting in underlined characters.
			 * Double ampersands result in a single ampersand to be rendered.
			 * Defaults to false.
			 * @param ellipse Whether to use an ellipse (three periods, '...'),
			 * if the text does not fit the context's clipping rectangle.
			 * In this case, the text is reduced in a way to fit, leaving out
			 * trailing characters, adding the ellipse.
			 */
			virtual void drawTextInternal(DrawingContext & context, Font * font, Vector position,
				const String & text, unsigned long modulationColor, BlendType blendType, bool caps,
				bool withHotKeys, bool ellipse) = NULL;

			/**
			 * Updates all handlers when the scene's exclusive changes.
			 */
			void updateExclusive();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(PlatformAdapter);
			#endif

		public:

			/**
			 * Destroys the platform adapter. Do not use this destructor if the adapter has been passed to the Desktop.
			 */
			virtual ~PlatformAdapter();

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * section, bool write);

			/**
			 * Clears the entire surface defined by a drawing context.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param fillColor The fill color to be used. The color consists of alpha,
			 * red, green, and blue components. Use the color macros to synthesize this color.
			 * Defaults to black.
			 */
			virtual void clear(DrawingContext & context, unsigned long fillColor = GL_BLACK) = NULL;

			/**
			 * Clears the contents of the system's clipboard or at least the text version of it,
			 * so that getClipboardText() will return an empty string.
			 */
			virtual void clearClipboard() = NULL;

			/**
			 * Creates a new platform adapter, determining the current platform's implementation.
			 * @return The new platform adapter;
			 */
			static PlatformAdapter * createInstance();

			/**
			 * Creates a new mutex object for inter-thread synchronisation.
			 * Only needed when working with multiple thread, using createThread. Use rarely.
			 * @return The new mutex. You must delete this object after use.
			 * @see createThread()
			 */
			virtual Mutex * createMutex() = NULL;

			/**
			 * Creates a new render target image.
			 * @param size The initial size of the image.
			 * @param imageType The type of the image (whether the image contains RGB
			 * and/or alpha information). Defaults to TYPE_RGB_ALPHA.
			 * @return The new image object.
			 * @note The object is managed by the platform adapter,
			 * but it may be destroyed calling destroyRenderTargetImage().
			 * @see destroyRenderTarget()
			 */
			Image * createRenderTarget(Vector size, Image::Type imageType = Image::TYPE_RGB_ALPHA);

			/**
			 * Creates a new thread for a given function or static method.
			 * The thread starts in suspended state, so you have to resume it to start it.
			 * @param function The function or static method to create the thread for.
			 * @param parameter The parameter to be passed to the function. May be NULL.
			 * @return The new thread.
			 * @warn Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use Desktop::deferOperation() from any concurrent thread.
			 * Some classes also provide convenience methods, usually suffixed by 'Deferred'.
			 * Also note that the thread's methods are safe to use from both the main and
			 * the concurrent thread.
			 * @see Thread::start()
			 * @see Desktop::deferOperation()
			 */
			virtual Thread * createThread(int (*function)(Thread & thread, void * parameter),
				void * parameter) = NULL;

			/**
			 * Destroys a render target image.
			 * @param image The image to be destroyed.
			 * @see createRenderTarget()
			 */
			void destroyRenderTarget(Image * image);

			/**
			 * Draws a rectangle (or square) with colored top-left and bottom-right borders,
			 * generating a pseudo-3D effect.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param from The top-left corner position of the rectangle.
			 * @param to The bottom-right corner position of the rectangle.
			 * @param fillColor The fill color to be used. The color consists of alpha,
			 * red, green, and blue components. Use the color macros to synthesize this color.
			 * Use transparent black to omit the filling.
			 * @param topLeftBorderColor The color to be used for the top and left borders.
			 * The color consists of alpha, red, green, and blue components.
			 * Use the color macros to synthesize this color.
			 * @param bottomRightBorderColor The color to be used for the bottom and right
			 * borders. The color consists of alpha, red, green, and blue components.
			 * Use the color macros to synthesize this color.
			 */
			void draw3DRect(DrawingContext & context, Vector from, Vector to,
				unsigned long fillColor, unsigned long topLeftBorderColor,
				unsigned long bottomRightBorderColor);

			/**
			 * Draws an ellipse (or circle) without an outline.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param center The center position of the rectangle.
			 * @param radius The x and y radius of the ellipse. If both coordinates are equal,
			 * the method draws a circle.
			 * @param fillColor The fill color to be used. The color consists of alpha,
			 * red, green, and blue components. Use the color macros to synthesize this color.
			 * Use transparent black to omit the filling.
			 */
			inline void drawEllipse(DrawingContext & context, Vector center, Vector radius,
				unsigned long fillColor);

			/**
			 * Draws an ellipse (or circle) with (or without) an outline.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param center The center position of the rectangle.
			 * @param radius The x and y radius of the ellipse. If both coordinates are equal,
			 * the method draws a circle.
			 * @param fillColor The fill color to be used. The color consists of alpha,
			 * red, green, and blue components. Use the color macros to synthesize this color.
			 * Use transparent black to omit the filling.
			 * @param borderColor The border color to be used.
			 * The color consists of alpha, red, green, and blue components.
			 * Use the color macros to synthesize this color.
			 * If the border color equals the fill color (which is the default),
			 * the ellipse has no outline.
			 */
			virtual void drawEllipse(DrawingContext & context, Vector center, Vector radius,
				unsigned long fillColor, unsigned long borderColor) = NULL;

			/**
			 * Draws all of an image without stretching or shrinking.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param image The image to be drawn.
			 * @param targetPosition The top-left position to draw the image at.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the image opaquely.
			 * Using GL_WHITE_A(alpha), renders the image using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target. Defaults to BLENDTYPE_NORMAL.
			 */
			inline void drawImage(DrawingContext & context, Image * image,
				Vector targetPosition, unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);

			/**
			 * Draws all of an image with stretching or shrinking.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param image The image to be drawn.
			 * @param targetPosition The top-left position to draw the image at.
			 * @param targetSize The target image size.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the image opaquely.
			 * Using GL_WHITE_A(alpha), renders the image using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target. Defaults to BLENDTYPE_NORMAL.
			 */
			inline void drawImage(DrawingContext & context, Image * image,
				Vector targetPosition, Vector targetSize,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);

			/**
			 * Draws a part of an image without stretching or shrinking.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param image The image to be drawn.
			 * @param sourcePosition The top-left position to draw the image at.
			 * @param sourceSize The target image size.
			 * @param targetPosition The top-left position to draw the image at.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the image opaquely.
			 * Using GL_WHITE_A(alpha), renders the image using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target. Defaults to BLENDTYPE_NORMAL.
			 */
			inline void drawImage(DrawingContext & context, Image * image,
				Vector sourcePosition, Vector sourceSize, Vector targetPosition,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);

			/**
			 * Draws a part of an image with stretching or shrinking.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param image The image to be drawn.
			 * @param sourcePosition The top-left position to draw the image at.
			 * @param sourceSize The target image size.
			 * @param targetPosition The top-left position to draw the image at.
			 * @param targetSize The target image size.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the image opaquely.
			 * Using GL_WHITE_A(alpha), renders the image using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target. Defaults to BLENDTYPE_NORMAL.
			 */
			virtual void drawImage(DrawingContext & context, Image * image,
				Vector sourcePosition, Vector sourceSize, Vector targetPosition, Vector targetSize,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL) = NULL;

			/**
			 * Draws a line from a given position to another one.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param from The start position of the line.
			 * @param to The end position of the line.
			 * @param color The color to be used. The color consists of alpha, red,
			 * green, and blue components. Use the color macros to synthesize this color.
			 */
			virtual void drawLine(DrawingContext & context, Vector from, Vector to,
				unsigned long color) = NULL;

			/**
			 * Draws a mono-colored text at a given position, using a given font
			 * and a provided color.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param font The font to be used.
			 * @param position The top-left position of the text.
			 * @param text The text to be drawn.
			 * @param color The color to be used. The color consists of alpha, red,
			 * green, and blue components. Use the color macros to synthesize this color.
			 * @param caps Whether to use all-capital characters. Defaults to false.
			 * @param withHotKeys Whether ampersand characters ('&') are considered
			 * as hot key defining prefixes, resulting in underlined characters.
			 * Double ampersands result in a single ampersand to be rendered.
			 * Defaults to false.
			 * @param ellipse Whether to use an ellipse (three periods, '...'),
			 * if the text does not fit the context's clipping rectangle.
			 * In this case, the text is reduced in a way to fit, leaving out
			 * trailing characters, adding the ellipse.
			 */
			virtual void drawMonoText(DrawingContext & context, Font * font,
				Vector position, const String & text, unsigned long color, bool caps = false,
				bool withHotKeys = false, bool ellipse = true);

			/**
			 * Draws a pixel at a given position, using a provided drawing context.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param position The position of the pixel.
			 * @param color The color to be used. The color consists of alpha, red,
			 * green, and blue components. Use the color macros to synthesize this color.
			 */
			virtual void drawPixel(DrawingContext & context, Vector position,
				unsigned long color) = NULL;

			/**
			 * Draws a rectangle (or square) without an outline.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param from The top-left corner position of the rectangle.
			 * @param to The bottom-right corner position of the rectangle.
			 * @param fillColor The fill color to be used. The color consists of alpha,
			 * red, green, and blue components. Use the color macros to synthesize this color.
			 * Use transparent black to omit the filling.
			 */
			inline void drawRect(DrawingContext & context, Vector from, Vector to,
				unsigned long fillColor);

			/**
			 * Draws a rectangle (or square) with (or without) an outline.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param from The top-left corner position of the rectangle.
			 * @param to The bottom-right corner position of the rectangle.
			 * @param fillColor The fill color to be used. The color consists of alpha,
			 * red, green, and blue components. Use the color macros to synthesize this color.
			 * Use transparent black to omit the filling.
			 * @param borderColor The border color to be used.
			 * The color consists of alpha, red, green, and blue components.
			 * Use the color macros to synthesize this color.
			 * If the border color equals the fill color (which is the default),
			 * the rectangle has no outline.
			 */
			virtual void drawRect(DrawingContext & context, Vector from, Vector to,
				unsigned long fillColor, unsigned long borderColor) = NULL;

			/**
			 * Draws all of an image without stretching or shrinking, and using rotation.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param image The image to be drawn.
			 * @param targetPosition The top-left position to draw the image at.
			 * @param rotationAnchor The position to rotate at. This pixel will stay in position.
			 * @param rotationAngle The counter-clockwise radians angle to rotate the image.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the image opaquely.
			 * Using GL_WHITE_A(alpha), renders the image using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target. Defaults to BLENDTYPE_NORMAL.
			 */
			inline void drawRotatedImage(DrawingContext & context, Image * image,
				Vector targetPosition, Vector rotationAnchor, double rotationAngle,
            unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);

			/**
			 * Draws all of an image with stretching or shrinking, and using rotation.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param image The image to be drawn.
			 * @param targetPosition The top-left position to draw the image at.
			 * @param targetSize The target image size.
			 * @param rotationAnchor The position to rotate at. This pixel will stay in position.
			 * @param rotationAngle The counter-clockwise radians angle to rotate the image.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the image opaquely.
			 * Using GL_WHITE_A(alpha), renders the image using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target. Defaults to BLENDTYPE_NORMAL.
			 */
			inline void drawRotatedImage(DrawingContext & context, Image * image,
				Vector targetPosition, Vector targetSize,
				Vector rotationAnchor, double rotationAngle,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);

			/**
			 * Draws a part of an image without stretching or shrinking, and using rotation.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param image The image to be drawn.
			 * @param sourcePosition The top-left position to draw the image at.
			 * @param sourceSize The target image size.
			 * @param targetPosition The top-left position to draw the image at.
			 * @param rotationAnchor The position to rotate at. This pixel will stay in position.
			 * @param rotationAngle The counter-clockwise radians angle to rotate the image.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the image opaquely.
			 * Using GL_WHITE_A(alpha), renders the image using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target. Defaults to BLENDTYPE_NORMAL.
			 */
			inline void drawRotatedImage(DrawingContext & context, Image * image,
				Vector sourcePosition, Vector sourceSize, Vector targetPosition,
				Vector rotationAnchor, double rotationAngle,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);

			/**
			 * Draws a part of an image with stretching or shrinking, and using rotation.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param image The image to be drawn.
			 * @param sourcePosition The top-left position to draw the image at.
			 * @param sourceSize The target image size.
			 * @param targetPosition The top-left position to draw the image at.
			 * @param targetSize The target image size.
			 * @param rotationAnchor The position to rotate at. This pixel will stay in position.
			 * @param rotationAngle The counter-clockwise radians angle to rotate the image.
			 * @param modulationColor The modulation color to be used.
			 * Using GL_WHITE, which is the default, renders the image opaquely.
			 * Using GL_WHITE_A(alpha), renders the image using a given alpha value,
			 * where 0 is entirely transparent, and 255 is entirely opaque.
			 * @param blendType How to blend the image on the context's target. Defaults to BLENDTYPE_NORMAL.
			 */
			virtual void drawRotatedImage(DrawingContext & context, Image * image,
				Vector sourcePosition, Vector sourceSize, Vector targetPosition, Vector targetSize,
				Vector rotationAnchor, double rotationAngle,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL) = NULL;

			/**
			 * Draws a multi-colored text at a given position, using a given font
			 * and alpha blending.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param font The font to be used.
			 * @param position The top-left position of the text.
			 * @param text The text to be drawn.
			 * @param alpha The alpha blending value to be used (0 is fully transparent,
			 * 255, the default, is fully opaque).
			 * @param caps Whether to use all-capital characters. Defaults to false.
			 * @param withHotKeys Whether ampersand characters ('&') are considered
			 * as hot key defining prefixes, resulting in underlined characters.
			 * Double ampersands result in a single ampersand to be rendered.
			 * Defaults to false.
			 * @param ellipse Whether to use an ellipse (three periods, '...'),
			 * if the text does not fit the context's clipping rectangle.
			 * In this case, the text is reduced in a way to fit, leaving out
			 * trailing characters, adding the ellipse.
			 */
			virtual void drawText(DrawingContext & context, Font * font, Vector position,
				const String & text, unsigned char alpha = 255, bool caps = false,
				bool withHotKeys = false, bool ellipse = false);

			/**
			 * Draws a triangle without an outline.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param vertices A vector array with 3 elements,
			 * defining the corners of the triangle.
			 * @param fillColor The fill color to be used. The color consists of alpha,
			 * red, green, and blue components. Use the color macros to synthesize this color.
			 * Use transparent black to omit the filling.
			 */
			inline void drawTriangle(DrawingContext & context, Vector * vertices,
				unsigned long fillColor);

			/**
			 * Draws a triangle with (or without) an outline.
			 * @param context The drawing context to be used. The drawing context defines
			 * the target surface image and clipping and drawing offsets and sizes.
			 * @param vertices A vector array with 3 elements,
			 * defining the corners of the triangle.
			 * @param fillColor The fill color to be used. The color consists of alpha,
			 * red, green, and blue components. Use the color macros to synthesize this color.
			 * Use transparent black to omit the filling.
			 * @param borderColor The border color to be used.
			 * The color consists of alpha, red, green, and blue components.
			 * Use the color macros to synthesize this color.
			 * If the border color equals the fill color (which is the default),
			 * the triangle has no outline.
			 */
			virtual void drawTriangle(DrawingContext & context, Vector * vertices,
				unsigned long fillColor, unsigned long borderColor) = NULL;

			/**
			 * Returns the current time since the beginning of the ERA
			 * (since 1st of January 1970, 0:00) in seconds.
			 * @return The absolute time in seconds.
			 */
			virtual unsigned long getAbsoluteTime() = NULL;

			/**
			 * Returns the currently available video memory in KB.
			 * @return The currently available video memory in KB.
			 */
			virtual unsigned long getAvailableVideoMemory() = NULL;

			/**
			 * Returns a text version of the current content of the system's clipboard.
			 * @return The clipboard text.
			 */
			virtual String getClipboardText() = NULL;

			/**
			 * Returns the path to the configuration folder for this application on the OS.
			 * This may be the application path itself, or another folder designated to configurations.
			 * @param manufacturer The technical name of the manufacturer (may be used to build folder name).
			 * @param applicationName The technical name of the application (may be used to build folder name).
			 * @return The path to the configuration folder.
			 */
			virtual String getConfigFolder(const String & manufacturer, const String & applicationName) = NULL;

			/**
			 * Returns a font by its font file path.
			 * The font is managed by the adapter, so using the font the first time
			 * loads it into memory. Consequtive uses return the same instance again.
			 * @param path The path of the font file.
			 * @return The font. The memory is managed by the adapter, so do not delete it.
			 * @see Font::ensureLoaded()
			 */
			Font * getFont(const String & path);

			/**
			 * Returns the decimal point used by the system's default locale.
			 * @return The decimal point (e.g. '.' or ',').
			 */
			virtual char getDecimalPoint() = NULL;

			/**
			 * Returns an image by its file path.
			 * The image is managed by the adapter, so using the image the first time
			 * loads it into memory. Consequtive uses return the same instance again.
			 * @param path The path of the image file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 * @param load Whether to ensure that the image has been cached into memory immediately.
			 * Otherwise, the file may be loaded on first use. Defaults to false.
			 * @return The image. The memory is managed by the adapter, so do not delete it.
			 * @see Image::ensureLoaded()
			 */
			Image * getImage(const String & path, bool load = false);

			/**
			 * Returns the joystick input handler of this platform adapter.
			 * @return The joystick input handler.
			 */
			virtual JoystickHandler * getJoystickHandler() = NULL;

			/**
			 * Returns the keyboard input handler of this platform adapter.
			 * @return The keyboard input handler.
			 */
			virtual KeyboardHandler * getKeyboardHandler() = NULL;

			/**
			 * Returns the human friendly name of a key for a given key code.
			 * @param keyCode The key code.
			 * @return The human friendly name.
			 */
			virtual const char * getKeyName(int keyCode) = NULL;

			/**
			 * Returns the current master volume, which ranges from 0 to 255.
			 * @return The current master volume.
			 */
			virtual int getMasterVolume() = NULL;

			/**
			 * Returns the mouse input handler of this platform adapter.
			 * @return The mouse input handler.
			 */
			virtual MouseHandler * getMouseHandler() = NULL;

			/**
			 * Returns the color of an image's pixel at a given position.
			 * @param image The image to read from.
			 * @param position The position to return the pixel color of.
			 * @return The point color. The color consists of alpha, red, green,
			 * and blue components. Use the color macros to analyze this color.
			 */
			inline unsigned long getPixel(Image * image, Vector position);

			/**
			 * Returns the color of a pixel at a given position, using a given drawing context.
			 * @param context The drawing context to be used. The drawing context defines
			 * the source surface image and clipping and drawing offsets and sizes.
			 * @param position The position to return the pixel color of.
			 * @return The point color. The color consists of alpha, red, green,
			 * and blue components. Use the color macros to analyze this color.
			 */
			virtual unsigned long getPixel(DrawingContext & context, Vector position) = NULL;

			/**
			 * Returns the size of the entire screen.
			 * @return The screen size.
			 */
			virtual Vector getScreenSize() = NULL;

			/**
			 * Returns a string that describes the system (hardware, software, metrics, capabilities, etc.).
			 * Useful for debugging on crashes etc.
			 * @return The system information.
			 */
			virtual String getSystemInformation() = NULL;

			/**
			 * Returns the current time in milli seconds since the application's start.
			 * @return The current time in milli seconds
			 */
			virtual unsigned long getTime() = NULL;

			/**
			 * Initializes the platform adapter. This method is called by the desktop
			 * when it is initialized. Do not call this method manually.
			 * @see Desktop::createInstance()
			 */
			virtual void initialize() = NULL;

			/**
			 * Returns whether the alpha operations are fully supported.
			 * If not, some alpha blending may fail and fall back to opaqueness.
			 * @return Whether the alpha operations are fully supported.
			 */
			virtual bool isAlphaFullySupported() = NULL;

			/**
			 * Returns whether the application is currently focused.
			 * @return Whether the application is currently focused.
			 */
			virtual bool isApplicationFocused() = NULL;

			/**
			 * Returns whether a given key is the delete key.
			 * @param keyCode The key code.
			 * @return Whether the key is the delete key.
			 */
			virtual bool isDeleteKey(int keyCode) = NULL;

			/**
			 * Returns whether the platform adapter is initialized as exclusive.
			 * This is controlled by the last scene that issued an adapter restart.
			 * @see Scene::isExlusive()
			 */
			virtual bool isExclusive() = NULL;

			/**
			 * Returns whether the platform adapter is initialized as full screen.
			 * This is controlled by the last scene that issued an adapter restart.
			 * @see Scene::isFullScreen()
			 */
			virtual bool isFullScreen() = NULL;

			/**
			 * Returns whether the speakers are mute (which is independent from the master volume).
			 * @return Whether the speakers are mute.
			 */
			virtual bool isMute() = NULL;

			/**
			 * Returns whether all images widths and height require to be a power of two.
			 * @return Whether all images widths and height require to be a power of two.
			 */
			virtual bool isPow2Required() = NULL;

			/**
			 * Returns whether all images widths and height require to be equal.
			 * @return Whether all images widths and height require to be equal.
			 */
			virtual bool isSquareRequired() = NULL;

			/**
			 * Minimizes the entire application.
			 */
			virtual void minimizeDesktop() = NULL;

			/**
			 * Called when the desktop title changes.
			 * Update the desktop title in any OS window etc.
			 */
			virtual void onDesktopTitleChanged();

			/**
			 * Opens a given URL in a web browser.
			 * @param url The URL to open.
			 */
			virtual void openUrl(const String & url) = NULL;

			/**
			 * Restores the entire application.
			 */
			virtual void restoreDesktop() = NULL;

			/**
			 * Sets a given text into system's clipboard.
			 * @param text The new clipboard text.
			 */
			virtual void setClipboardText(const String & text) = NULL;

			/**
			 * Sets the current master volume, which ranges from 0 to 255.
			 * @param volume The current master volume.
			 */
			virtual void setMasterVolume(int masterVolume) = NULL;

			/**
			 * Sets whether the speakers are mute (which is independent from the master volume).
			 * @param mute Whether the speakers are mute.
			 */
			virtual void setMute(bool mute) = NULL;

			/**
			 * Issues to show the drawing operations since the last call to showFrame().
			 * Call this method at the end of a frame.
			 */
			virtual void showFrame() = NULL;

			/**
			 * Shuts down the platform adapter. This method is called by the desktop
			 * when it is shut down. Do not call this method manually.
			 * @see Desktop::destroyInstance()
			 */
			virtual void shutDown() = NULL;

			/**
			 * Suspends the current thread for a given number of milli seconds.
			 * @param msecs The number of milli seconds to wait.
			 */
			virtual void sleep(unsigned long msecs) = NULL;

			/**
			 * Suspends the current thread for a given number of milli seconds,
			 * splitting the time into slices to prevent cheating by shifting the clock.
			 * On the other hand, this is a little less accurate.
			 * @param time The number of milli seconds to wait.
			 */
			void sleepWithCheatProtection(unsigned long time);

			/**
			 * Called each frame to perform operations depending on the system,
			 * such as dispatching Windows messages.
			 */
			virtual void update() = NULL;
	};
}


#endif
