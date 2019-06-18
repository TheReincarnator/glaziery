/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_FONT_H
#define __GLAZIERY_FONT_H


namespace glaziery
{
	/**
	 * A font is a resource managed by the platform adapter
	 * that provides rendering of texts by drawing multi-color bitmap characters
	 * read from a file resource.
	 * Font bitmaps contain an alpha channel, which enables anti-aliasing.
	 * The text may be rendered as multi-colored using the bitmap information,
	 * or using a provided color. In the latter case, only the alpha channel is used.<br>
	 *
	 * The font is typically read from a targa bitmap file (TGA), because it supports
	 * an alpha channel. The bitmap contains all characters of the ISO-8859-1 charset,
	 * starting from character 32 (space), all characters next to each other, from left
	 * to right, except for some pixel rows and column indicating characters widths and so on.
	 *
	 * <ul>
	 * <li> The left-most column marks the base line of the font, the lowest pixel row
	 * of the ascent. All pixels except of one must be transparent black.</li>
	 * <li> The top-most row marks the character start columns, starting with the space
	 * character.</li>
	 * <li> The area between the left-most column and the first character start mark
	 * is used for underlining text.</li>
	 * </ul>
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class Font : public CObject
	#else
		class Font
	#endif
	{
		friend ArrayList<Font>;
		friend HashMap<Font>;
		friend PlatformAdapter;

		private:

			/**
			 * The path of the font file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 */
			String path;

		protected:

			/**
			 * The number of pixel rows above and including the base line.
			 */
			int ascent;

			/**
			 * The width of the ellipse.
			 */
			int ellipseWidth;

			/**
			 * The number of pixel rows from the bottom to the top of the font.
			 */
			int height;

			/**
			 * The image that contains the font glyphs.
			 */
			Image * image;

			/**
			 * The position of the upper-left corner of the character glyphs within the image.
			 * @note The index corresponds to the ISO-8859-1 encoding of the respective character.
			 */
			Vector positions[256];

			/**
			 * The widths of the character glyphs in pixels.
			 * @note The index corresponds to the ISO-8859-1 encoding of the respective character.
			 */
			int widths[256];

			/**
			 * Destroys the font.
			 */
			virtual ~Font();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Font);
			#endif

		public:

			/**
			 * The string to be used as ellipse.
			 */
			static const char * ELLIPSE;

			/**
			 * Creates a new font.
			 * @param path The path of the font file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 */
			Font(const String & path);

			/**
			 * Returns the number of pixel rows above and including the base line.
			 * @return The font's ascent.
			 */
			int getAscent();

			/**
			 * Returns the number of pixel rows below the base line.
			 * @return The font's descent.
			 */
			int getDescent();

			/**
			 * Returns the number of pixel rows from the bottom to the top of the font.
			 * @return The font's height.
			 */
			int getHeight();

			/**
			 * Returns the image that contains the font glyphs.
			 * @return The image that contains the font glyphs.
			 */
			Image * getImage();

			/**
			 * Returns the path of the font file, may be relative or absolute.
			 * Relative paths are relative to the application's executable location.
			 * @return The path of the font file.
			 */
			const String & getPath();

			/**
			 * Returns the position of a single character within the font images.
			 * @param character The character to return the position of.
			 * @return The position.
			 */
			Vector getPosition(char character);

			/**
			 * Returns the size of a given text if rendered using this font.
			 * This includes character spacing pixels.
			 * @param text The text. Line feeds ('\n') are accounted for.
			 * @param caps Whether to use all-capital characters. Defaults to false.
			 * @param withHotKeys Whether ampersand characters ('&') are considered
			 * as hot key defining prefixes. Double ampersands result in a single ampersand
			 * to be rendered. Defaults to false.
			 * @return The size of the text in pixels.
			 */
			Vector getSize(const String & text, bool caps = false, bool withHotKeys = false);

			/**
			 * Returns the width of a given text if rendered using this font.
			 * This includes character spacing pixels.
			 * @param text The text. Line feeds ('\n') are accounted for.
			 * @param caps Whether to use all-capital characters. Defaults to false.
			 * @param withHotKeys Whether ampersand characters ('&') are considered
			 * as hot key defining prefixes. Double ampersands result in a single ampersand
			 * to be rendered. Defaults to false.
			 * @return The width of the text in pixels.
			 */
			int getWidth(const String & text, bool caps = false, bool withHotKeys = false);

			/**
			 * Returns the width of a single character, without character spacing.
			 * @param character The character to return the width of.
			 * @return The width in pixels.
			 */
			int getWidth(const char character);

			/**
			 * Returns a text that is shortened if its width is larged than a given width.
			 * This is done by leaving out trailing characters, adding an ellipse (three dots, '...').
			 * If the width is even smaller than the ellipse itself, only the ellipse is returned.
			 * @param text The original text.
			 * @param maximumWidth The maximum width to be reached.
			 * @param caps Whether to use all-capital characters. Defaults to false.
			 * @return The shortened and ellipsed text.
			 */
			String shortenTextForEllipse(const String & text, int maximumWidth, bool caps = false);
	};
}


#endif
