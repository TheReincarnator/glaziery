/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int DirectXFont::FONT_VERSION = 1;

DirectXFont::DirectXFont(const String & path)
	: Font(path)
{
	ASSERTION_COBJECT(this);

	DirectXPlatformAdapter * adapter = (DirectXPlatformAdapter *) Desktop::getInstance()->getPlatformAdapter();
	image = adapter->getImage(path, true);

	IDirect3DSurface9 * surface = ((DirectXImage *) image)->getSurface();
	if (surface == NULL)
		throw EILLEGALSTATE("Cannot load font: The font surface is missing");

	// Read Glaziery Font identifier (GLF), version, and properties

	unsigned long idPixel = getPixel(surface, 0, 0);
	if (GL_ALPHA_OF(idPixel) != 'G' || GL_RED_OF(idPixel) != 'L' || GL_GREEN_OF(idPixel) != 'F')
		throw EILLEGALARGUMENT("Glaziery Font identifier not found:"
			" Image seems not to be a Glaziery font");
	if (GL_BLUE_OF(idPixel) > FONT_VERSION)
		throw EILLEGALARGUMENT("Glaziery Font version too high, please upgrade Glaziery");

	unsigned long heightsPixel = getPixel(surface, 0, 1);
	height = GL_ALPHA_OF(heightsPixel) * 256 + GL_RED_OF(heightsPixel);
	ascent = GL_GREEN_OF(heightsPixel) * 256 + GL_BLUE_OF(heightsPixel);

	// Initialize positions and sizes

	int charNo;
	for (charNo=0; charNo<256; charNo++)
	{
		positions[charNo] = Vector(2, 1);
		widths[charNo] = 1;
	}

	// Now read the glyph sizes

	unsigned int x = 2;
	unsigned int xStart = x;
	unsigned int y = 0;
	charNo = 32;
	while (charNo < 256 && y + height + 1 <= (unsigned int) image->getSize().y)
	{
		unsigned long pixel = getPixel(surface, x, y);
		if (GL_COLOR_OF(pixel) != 0)
		{
			positions[charNo] = Vector(xStart, y + 1);
			widths[charNo] = x - xStart + 1;
			xStart = x + 1;

			charNo++;
			if (charNo == 128)
				charNo = 160;
		}

		x++;
		if (x >= (unsigned int) image->getSize().x)
		{
			x = 0;
			xStart = 0;
			y += height + 1;
		}
	}

	ellipseWidth = Font::getWidth(String(ELLIPSE));
}

DirectXFont::~DirectXFont()
{
	ASSERTION_COBJECT(this);
}

unsigned long DirectXFont::getPixel(IDirect3DSurface9 * surface, int x, int y)
{
	ASSERTION_COBJECT(this);

	D3DLOCKED_RECT lockedRect;
	RECT rect;

	rect.left = x; rect.top = y; rect.right = x + 1; rect.bottom = y + 1;
	HRESULT result;
	if (FAILED(result = surface->LockRect(&lockedRect, &rect, D3DLOCK_READONLY)))
	{
		LOGPRINTF0_RESULT(LOG_ERROR, "Cannot get pixel from Direct 3D surface:"
			" Cannot lock", result);
		return 0;
	}

	unsigned long pixel = ((long *) lockedRect.pBits)[0];

	if (FAILED(result = surface->UnlockRect()))
	{
		LOGPRINTF0_RESULT(LOG_ERROR, "Cannot get pixel from Direct 3D surface:"
			" Cannot unlock", result);
		return 0;
	}

	return pixel;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(DirectXFont, CObject);
#endif
