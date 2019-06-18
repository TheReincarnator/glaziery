/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


PlatformAdapter::~PlatformAdapter()
{
	ASSERTION_COBJECT(this);
}

void PlatformAdapter::Configure(ConfigSection * section, bool write)
{
	ASSERTION_COBJECT(this);

	section->Configure("JoystickHandler", getJoystickHandler(), write);
	section->Configure("KeyboardHandler", getKeyboardHandler(), write);
	section->Configure("MouseHandler", getMouseHandler(), write);
}

PlatformAdapter * PlatformAdapter::createInstance()
{
	PlatformAdapter * adapter = NULL;
	#ifdef D3D_OVERLOADS
		adapter = new DirectXPlatformAdapter;
	#elif defined(TODO_MACOS_KEYWORD)
		adapter = new MacOSPlatformAdapter;
	#else
		#error Unknown platform
	#endif

	if (adapter == NULL)
		throw EOUTOFMEMORY;

	return adapter;
}

Image * PlatformAdapter::createRenderTarget(Vector size, Image::Type imageType)
{
	ASSERTION_COBJECT(this);

	Image * image = createImage("", imageType);
	renderTargetImages.Append(image);

	image->resize(size);

	return image;
}

void PlatformAdapter::destroyRenderTarget(Image * image)
{
	ASSERTION_COBJECT(this);
	renderTargetImages.Delete(image);
}

void PlatformAdapter::draw3DRect(DrawingContext & context, Vector from, Vector to,
	unsigned long fillColor, unsigned long topLeftBorderColor,
	unsigned long bottomRightBorderColor)
{
	ASSERTION_COBJECT(this);

	drawRect(context, from + Vector(1, 1), to - Vector(1, 1), fillColor);
	drawLine(context, from, Vector(to.x - 1, from.y), topLeftBorderColor);
	drawLine(context, Vector(from.x, from.y + 1), Vector(from.x, to.y - 1), topLeftBorderColor);
	drawLine(context, to, Vector(from.x + 1, to.y), bottomRightBorderColor);
	drawLine(context, Vector(to.x, to.y - 1), Vector(to.x, from.y + 1), bottomRightBorderColor);

	unsigned long cornerColor = GL_RGBA((GL_RED_OF(topLeftBorderColor) + GL_RED_OF(bottomRightBorderColor)) / 2,
		(GL_GREEN_OF(topLeftBorderColor) + GL_GREEN_OF(bottomRightBorderColor)) / 2,
		(GL_BLUE_OF(topLeftBorderColor) + GL_BLUE_OF(bottomRightBorderColor)) / 2,
		(GL_ALPHA_OF(topLeftBorderColor) + GL_ALPHA_OF(bottomRightBorderColor)) / 2);
	drawPixel(context, Vector(to.x, from.y), cornerColor);
	drawPixel(context, Vector(from.x, to.y), cornerColor);
}

void PlatformAdapter::drawMonoText(DrawingContext & context, Font * font,
	Vector position, const String & text, unsigned long color, bool caps,
	bool withHotKeys, bool ellipse)
{
	ASSERTION_COBJECT(this);
	drawTextInternal(context, font, position, text, color,
		BLENDTYPE_SILHOUETTE, caps, withHotKeys, ellipse);
}

void PlatformAdapter::drawText(DrawingContext & context, Font * font, Vector position,
	const String & text, unsigned char alpha, bool caps,
	bool withHotKeys, bool ellipse)
{
	ASSERTION_COBJECT(this);
	drawTextInternal(context, font, position, text, GL_RGBA(255, 255, 255, alpha),
		BLENDTYPE_NORMAL, caps, withHotKeys, ellipse);
}

Font * PlatformAdapter::getFont(const String & path)
{
	ASSERTION_COBJECT(this);

	Font * font = fonts.Get(path);
	if (font == NULL)
	{
		font = createFont(path);
		fonts.Set(path, font);
	}

	return font;
}

Image * PlatformAdapter::getImage(const String & path, bool load)
{
	ASSERTION_COBJECT(this);

	Image * image = fileBasedImages.Get(path);
	if (image == NULL)
	{
		image = createImage(path, Image::TYPE_RGB_ALPHA);
		fileBasedImages.Set(path, image);
	}

	if (load)
		image->ensureLoaded();

	return image;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(PlatformAdapter, CObject);
#endif

void PlatformAdapter::onDesktopTitleChanged()
{
	ASSERTION_COBJECT(this);
}

void PlatformAdapter::sleepWithCheatProtection(unsigned long time)
{
	ASSERTION_COBJECT(this);

	while (time > 100)
	{
		sleep(100);
		time -= 100;
	}

	sleep(time);
}

void PlatformAdapter::updateExclusive()
{
	ASSERTION_COBJECT(this);

	getKeyboardHandler()->updateExclusive();
	getMouseHandler()->updateExclusive();
	getJoystickHandler()->updateExclusive();
}
