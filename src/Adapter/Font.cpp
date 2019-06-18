/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const char * Font::ELLIPSE = "...";

Font::Font(const String & path)
{
	ASSERTION_COBJECT(this);

	ascent = 0;
	ellipseWidth = 0;
	height = 0;
	image = NULL;
	this->path = path;

	for (int i=0; i<256; i++)
		positions[i] = Vector();

	memset(widths, 0, sizeof(widths));
}

Font::~Font()
{
	ASSERTION_COBJECT(this);
}

int Font::getAscent()
{
	ASSERTION_COBJECT(this);
	return ascent;
}

int Font::getDescent()
{
	ASSERTION_COBJECT(this);
	return height - ascent;
}

int Font::getHeight()
{
	ASSERTION_COBJECT(this);
	return height;
}

Image * Font::getImage()
{
	ASSERTION_COBJECT(this);
	return image;
}

const String & Font::getPath()
{
	ASSERTION_COBJECT(this);
	return path;
}

Vector Font::getPosition(char character)
{
	ASSERTION_COBJECT(this);
	return positions[(unsigned char) character];
}

Vector Font::getSize(const String & text, bool caps, bool withHotKeys)
{
	ASSERTION_COBJECT(this);

	Vector size = Vector();

	int currentLineWidth = 0;
	for (int characterNo=0; text[characterNo] != 0; characterNo++)
	{
		char character = text[characterNo];
		if (character == '\n')
		{
			if (currentLineWidth > size.x)
				size.x = currentLineWidth;
			currentLineWidth = 0;
			size.y += height;
		}
		else
		{
			// Skip hot key indicators
			if (withHotKeys && character == '&' && text[characterNo+1] != 0)
				character = text[++characterNo];
			if (caps)
				character = String::ToUpperCase(character);

			currentLineWidth += getWidth(character) + 1;
		}
	}

	if (currentLineWidth > size.x)
		size.x = currentLineWidth;
	size.y += height;

	return size;
}

int Font::getWidth(const String & text, bool caps, bool withHotKeys)
{
	ASSERTION_COBJECT(this);
	return getSize(text, caps, withHotKeys).x;
}

int Font::getWidth(const char character)
{
	ASSERTION_COBJECT(this);
	return widths[(const unsigned char) character];
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Font, CObject);
#endif

String Font::shortenTextForEllipse(const String & text, int maximumWidth, bool caps)
{
	ASSERTION_COBJECT(this);

	String shortenedText;
	int currentLineStart = 0;
	int currentLineWidth = 0;

	int characterNo;
	for (characterNo=0; text[characterNo] != 0; characterNo++)
	{
		unsigned char character = text[characterNo];
		if (character == '\n')
		{
			shortenedText += text.Substring(currentLineStart, characterNo - currentLineStart);
			shortenedText += '\n';

			currentLineStart = characterNo+1;
			currentLineWidth = 0;
		}
		else
		{
			if (caps)
				character = String::ToUpperCase(character);

			int characterWidth = getWidth(character) + 1;
			if (currentLineWidth + characterWidth + ellipseWidth > maximumWidth)
			{
				int memorizedNo = characterNo;
				while (character != 0 && character != '\n')
				{
					currentLineWidth += characterWidth;

					characterNo++;
					character = text[characterNo];

					if (caps)
						character = String::ToUpperCase(character);

					characterWidth = getWidth(character) + 1;
				}

				if (currentLineWidth > maximumWidth)
				{
					shortenedText += text.Substring(currentLineStart, memorizedNo - currentLineStart);
					shortenedText += ELLIPSE;
				}
				else
					shortenedText += text.Substring(currentLineStart, characterNo - currentLineStart);

				if (character == 0)
					return shortenedText;

				shortenedText += '\n';

				currentLineStart = characterNo+1;
				currentLineWidth = 0;
			}
			else
				currentLineWidth += characterWidth;
		}
	}

	shortenedText += text.Substring(currentLineStart, characterNo - currentLineStart);

	return shortenedText;
}
