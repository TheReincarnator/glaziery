/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>

const int DirectXPlatformAdapter::CIRCLE_VERTEX_COUNT = 128;
const int DirectXPlatformAdapter::VERTEX_BUFFER_SIZE = 1024;

DirectXPlatformAdapter::DirectXPlatformAdapter()
{
	ASSERTION_COBJECT(this);

	alphaFullySupported = false;
	applicationIcon = 0;
	memset(&backBufferFormat, 0, sizeof(backBufferFormat));
	backBufferSurface = NULL;
	cosineTable = NULL;
	currentBlendType = BLENDTYPE_NORMAL;
	currentRenderer = NULL;
	currentTexture = NULL;
	currentVertices = NULL;

	char formatted[64];
	strcpy(formatted, "0.0");
	GetNumberFormat(LOCALE_USER_DEFAULT, 0, "0.0", NULL, formatted, 64);
	decimalPoint = strlen(formatted) == 3 ? formatted[1] : '.';

	direct3D = NULL;
	direct3DDevice = NULL;
	direct3DProblems = false;
	directInput = NULL;
	endpointVolume = NULL;
	exclusive = false;
	fullScreen = false;
	inScene = false;
	pow2Required = false;
	memset(&presentParameters, 0, sizeof(presentParameters));
	mixer = NULL;

	mixerLine;
	::ZeroMemory(&mixerLine, sizeof(mixerLine));

	LARGE_INTEGER performanceTimerFrequency;
	if (::QueryPerformanceFrequency(&performanceTimerFrequency))
	{
		this->performanceTimerFactor = 1000.0 / performanceTimerFrequency.QuadPart;

		// It is important to set the performance timer to an initial value,
		// because otherwise it is not possible to convert the time difference to a double
		LARGE_INTEGER performanceTimerCounter;
		if (::QueryPerformanceCounter(&performanceTimerCounter))
		{
			__int64 performanceTimerCounter64 = performanceTimerCounter.QuadPart;
			if (performanceTimerCounter64 > 0)
				performanceTimerStart = performanceTimerCounter64;
		}
	}
	else
	{
		this->performanceTimerFactor = 0.0;
		performanceTimerStart = 0;
	}

	sineTable = NULL;
	squareRequired = false;
	vertexNo = 0;
	vertexBuffer = NULL;
	windowClassNameAtom = 0;
	windowHandle = NULL;

	if ((cosineTable = new float[CIRCLE_VERTEX_COUNT]) == NULL)
		throw EOUTOFMEMORY;
	if ((sineTable = new float[CIRCLE_VERTEX_COUNT]) == NULL)
		throw EOUTOFMEMORY;

	const double pi = 3.1415926535897932;
	for (int i=0; i<CIRCLE_VERTEX_COUNT; i++)
	{
		sineTable[i] = (float) sin(2 * pi * i / CIRCLE_VERTEX_COUNT);
		cosineTable[i] = (float) cos(2 * pi * i / CIRCLE_VERTEX_COUNT);
	}
}

DirectXPlatformAdapter::~DirectXPlatformAdapter()
{
	ASSERTION_COBJECT(this);

	if (cosineTable != NULL) {delete cosineTable; cosineTable = NULL;}
	if (sineTable != NULL) {delete sineTable; sineTable = NULL;}
}

Vector DirectXPlatformAdapter::alignSize(Vector size)
{
	ASSERTION_COBJECT(this);

	if (size.x < 1) size.x = 1;
	if (size.y < 1) size.y = 1;

	if (pow2Required)
	{
		Vector alignedSize = Vector(1, 1);
		while (alignedSize.x < size.x)
			alignedSize.x *= 2;
		while (alignedSize.y < size.y)
			alignedSize.y *= 2;

		if (squareRequired)
			if (alignedSize.x < alignedSize.y)
				alignedSize.x = alignedSize.y;
			else
				alignedSize.y = alignedSize.x;

		return alignedSize;
	}
	else
	{
		if (squareRequired)
			if (size.x < size.y)
				size.x = size.y;
			else
				size.y = size.x;

		return size;
	}
}

bool DirectXPlatformAdapter::checkAndChooseMode(D3DFORMAT d3dFormat,
	unsigned int minimumWidth, unsigned int minimumHeight,
	unsigned int maximumWidth, unsigned int maximumHeight,
	unsigned int desiredWidth, unsigned int desiredHeight,
	unsigned int desiredRefreshRate)
{
	ASSERTION_COBJECT(this);

	bool matchingModeFound = false;

	// Iterate over available modes for the format
	int modeCount = direct3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dFormat);
	LOGPRINTF2(LOG_DEBUG, "Checking surface format %d, found %d modes", d3dFormat, modeCount);

	for (int modeNo=0; modeNo < modeCount; modeNo++)
	{
		D3DDISPLAYMODE mode;
		HRESULT result;
		if (FAILED(result = direct3D->EnumAdapterModes(D3DADAPTER_DEFAULT,
			d3dFormat, modeNo, &mode)))
		{
			LOGPRINTF1(LOG_WARN, "Cannot enumerate adapter mode (error code %ld)",
				result);
			continue;
		}

		LOGPRINTF4(LOG_DEBUG, "Checking mode %d x %d (format %d, refresh rate %d)",
			mode.Width, mode.Height, mode.Format, mode.RefreshRate);

		if (mode.Width >= minimumWidth && mode.Width <= maximumWidth
			&& mode.Height >= minimumHeight && mode.Height <= maximumHeight)
		{
			// The mode can be accepted, now check whether it is better
			// than the current one
			bool takeMode = false;
			if (matchingModeFound)
			{
				// We already have chosen a mode, now compare both

				int thisResolutionDistance
					= abs((int) desiredWidth - (int) mode.Width)
					+ abs((int) desiredHeight - (int) mode.Height);
				int chosenResolutionDistance
					= abs((int) desiredWidth - (int) presentParameters.BackBufferWidth)
					+ abs((int) desiredHeight - (int) presentParameters.BackBufferHeight);

				int thisRefreshRateDistance
					= abs((int) desiredRefreshRate - (int) mode.RefreshRate);
				int chosenRefreshRateDistance
					= abs((int) desiredRefreshRate - (int) presentParameters.FullScreen_RefreshRateInHz);

				if (thisResolutionDistance < chosenResolutionDistance)
				{
					// This mode has a more matching resolution, take it
					takeMode = true;
				}
				else if (thisResolutionDistance == chosenResolutionDistance
					&& thisRefreshRateDistance < chosenRefreshRateDistance)
				{
					// This mode has an equally matching resolution,
					// but the refresh rate matches better, so take it
					takeMode = true;
				}
			}
			else
			{
				// We haven't chosen any mode yet, so take this one
				takeMode = true;
			}

			if (takeMode)
			{
				LOGPRINTF0(LOG_DEBUG, "Mode is acceptable");

				presentParameters.BackBufferWidth = mode.Width;
				presentParameters.BackBufferHeight = mode.Height;
				presentParameters.BackBufferFormat = mode.Format;
				presentParameters.FullScreen_RefreshRateInHz = mode.RefreshRate;
				matchingModeFound = true;
			}
		}
	}

	return matchingModeFound;
}

void DirectXPlatformAdapter::clear(DrawingContext & context, unsigned long fillColor)
{
	ASSERTION_COBJECT(this);

	if (!ensureScene(context))
		return;

	if (!setTexture(NULL))
		return;

	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE),
		"Cannot set source color blending"))
		return;

	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO),
		"Cannot set destination color blending"))
		return;

	if (failedObserving(direct3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, fillColor, 1.0, 0),
		"Cannot clear surface"))
		return;

	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA),
		"Cannot set source color blending"))
		return;

	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA),
		"Cannot set destination color blending"))
		return;
}

void DirectXPlatformAdapter::clearClipboard()
{
	ASSERTION_COBJECT(this);

	if (::OpenClipboard(windowHandle))
	{
		::EmptyClipboard();
		::CloseClipboard();
	}
}

Font * DirectXPlatformAdapter::createFont(const String & path)
{
	ASSERTION_COBJECT(this);

	DirectXFont * font;
	if ((font = new DirectXFont(path)) == NULL)
		throw EOUTOFMEMORY;

	return font;
}

Image * DirectXPlatformAdapter::createImage(const String & path, Image::Type imageType)
{
	ASSERTION_COBJECT(this);

	DirectXImage * image;
	if ((image = new DirectXImage(path, imageType)) == NULL)
		throw EOUTOFMEMORY;

	return image;
}

Mutex * DirectXPlatformAdapter::createMutex()
{
	ASSERTION_COBJECT(this);

	Mutex * mutex;
	if ((mutex = new Win32Mutex) == NULL)
		throw EOUTOFMEMORY;
	return mutex;
}

Thread * DirectXPlatformAdapter::createThread(int (*function)(Thread & thread, void * parameter),
	void * parameter)
{
	ASSERTION_COBJECT(this);

	Thread * thread;
	if ((thread = new Win32Thread(function, parameter)) == NULL)
		throw EOUTOFMEMORY;
	return thread;
}

void DirectXPlatformAdapter::drawEllipse(DrawingContext & context, Vector center,
	Vector radius, unsigned long fillColor, unsigned long borderColor)
{
	ASSERTION_COBJECT(this);

	center += context.getDrawingOffset();
	if (!context.isInClippingArea(center - radius, center + radius))
		return;

	if (!ensureScene(context))
		return;

	if (!setTexture(NULL))
		return;

	Point * points;
	if (failedObserving(vertexBuffer->Lock(0, (CIRCLE_VERTEX_COUNT + 2) * sizeof(Point),
		(void **) &points, D3DLOCK_DISCARD),
		"Cannot draw ellipse: Cannot lock vertex buffer"))
		return;

	::ZeroMemory(points, (CIRCLE_VERTEX_COUNT + 2) * sizeof(Point));

	float radiusX = radius.x + 0.5f;
	float radiusY = radius.y + 0.5f;

	points[0].x = (float) center.x;
	points[0].y = (float) center.y;
	points[0].z = 1.0f;
	points[0].rhw = 1.0f;
	points[0].diffuse = fillColor;
	int i;
	for (i=0; i<CIRCLE_VERTEX_COUNT; i++)
	{
		points[i+1].x = center.x + radiusX * sineTable[i];
		points[i+1].y = center.y + radiusY * cosineTable[i];
		points[i+1].z = 1.0f;
		points[i+1].rhw = 1.0f;
		points[i+1].diffuse = fillColor;
	}
	points[CIRCLE_VERTEX_COUNT + 1].x = center.x + radiusX * sineTable[0];
	points[CIRCLE_VERTEX_COUNT + 1].y = center.y + radiusY * cosineTable[0];
	points[CIRCLE_VERTEX_COUNT + 1].z = 1.0f;
	points[CIRCLE_VERTEX_COUNT + 1].rhw = 1.0f;
	points[CIRCLE_VERTEX_COUNT + 1].diffuse = fillColor;

	if (failedObserving(vertexBuffer->Unlock(), "Cannot draw ellipse: Cannot unlock vertex buffer"))
		return;

	if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,
		0, CIRCLE_VERTEX_COUNT), "Cannot draw ellipse: Cannot draw triangle fan"))
		return;

	if (GL_ALPHA_OF(borderColor) == 0)
		return;

	if (failedObserving(vertexBuffer->Lock(1, CIRCLE_VERTEX_COUNT * sizeof(Point),
		(void **) &points, D3DLOCK_DISCARD),
		"Cannot draw ellipse: Cannot lock vertex buffer"))
		return;

	for (i=0; i<CIRCLE_VERTEX_COUNT; i++)
	{
		points[i].diffuse = borderColor;
		points[i].z = 0.5f;
		points[i].rhw = 1.0f;
	}

	if (failedObserving(vertexBuffer->Unlock(), "Cannot draw ellipse: Cannot unlock vertex buffer"))
		return;

	if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_LINESTRIP,
		1, CIRCLE_VERTEX_COUNT), "Cannot draw ellipse: Cannot draw line strip"))
		return;
}

void DirectXPlatformAdapter::drawImage(DrawingContext & context, Image * image,
	Vector sourcePosition, Vector sourceSize, Vector targetPosition, Vector targetSize,
	unsigned long modulationColor, BlendType blendType)
{
	ASSERTION_COBJECT(this);

	targetPosition += context.getDrawingOffset();
	if (!context.isInClippingArea(targetPosition, targetPosition + targetSize))
		return;

	if (!ensureScene(context))
		return;

	if (!setTexture(image, blendType))
		return;

	addTextureRectangle(sourcePosition, sourceSize, targetPosition, targetSize, modulationColor);
}

void DirectXPlatformAdapter::drawLine(DrawingContext & context, Vector from, Vector to,
	unsigned long color)
{
	ASSERTION_COBJECT(this);

	if (from == to)
	{
		drawPixel(context, from, color);
		return;
	}

	from += context.getDrawingOffset();
	to += context.getDrawingOffset();
	if (!context.isInClippingArea(from, to))
		return;

	if (!ensureScene(context))
		return;

	if (!setTexture(NULL))
		return;

	Point * points;
	if (failedObserving(vertexBuffer->Lock(0, 2 * sizeof(Point), (void **) &points,
		D3DLOCK_DISCARD), "Cannot draw line: Cannot lock vertex buffer"))
		return;

	::ZeroMemory(points, 2 * sizeof(Point));

	points[0].x = (float) from.x;
	points[0].y = (float) from.y;
	points[0].z = 1.0f;
	points[0].rhw = 1.0f;
	points[0].diffuse = color;

	points[1].x = (float) to.x;
	points[1].y = (float) to.y;
	points[1].z = 1.0f;
	points[1].rhw = 1.0f;
	points[1].diffuse = color;

	if (failedObserving(vertexBuffer->Unlock(), "Cannot draw line: Cannot unlock vertex buffer"))
		return;

	if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1),
		"Cannot draw line: Cannot draw primitive"))
		return;

	// Rendering of last pixel currently does not work. Disabled, and rendering manually now.
	if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_POINTLIST, 1, 1),
		"Cannot draw pixel: Cannot draw primitive"))
		return;
}

void DirectXPlatformAdapter::drawPixel(DrawingContext & context, Vector position,
	unsigned long color)
{
	ASSERTION_COBJECT(this);

	position += context.getDrawingOffset();
	if (!context.isInClippingArea(position))
		return;

	if (!ensureScene(context))
		return;

	if (!setTexture(NULL))
		return;

	Point * points;
	if (failedObserving(vertexBuffer->Lock(0, sizeof(Point), (void **) &points,
		D3DLOCK_DISCARD), "Cannot draw pixel: Cannot lock vertex buffer"))
		return;

	::ZeroMemory(points, sizeof(Point));

	points[0].x = (float) position.x;
	points[0].y = (float) position.y;
	points[0].z = 1.0f;
	points[0].rhw = 1.0f;
	points[0].diffuse = color;

	if (failedObserving(vertexBuffer->Unlock(), "Cannot draw pixel: Cannot unlock vertex buffer"))
		return;

	if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, 1),
		"Cannot draw pixel: Cannot draw primitive"))
		return;
}

void DirectXPlatformAdapter::drawRect(DrawingContext & context, Vector from, Vector to,
	unsigned long fillColor, unsigned long borderColor)
{
	ASSERTION_COBJECT(this);

	from += context.getDrawingOffset();
	to += context.getDrawingOffset();
	if (!context.isInClippingArea(from, to))
		return;

	if (!ensureScene(context))
		return;

	if (!setTexture(NULL))
		return;

	if (to.x < from.x) {int x = to.x; to.x = from.x; from.x = x;}
	if (to.y < from.y) {int y = to.y; to.y = from.y; from.y = y;}

	if (borderColor == fillColor)
	{
		Point * points;
		if (failedObserving(vertexBuffer->Lock(0, 4 * sizeof(Point), (void **) &points,
			D3DLOCK_DISCARD), "Cannot draw rect: Cannot lock vertex buffer"))
			return;

		::ZeroMemory(points, 4 * sizeof(Point));

		points[0].x = (float) from.x - 0.5f;
		points[0].y = (float) from.y - 0.5f;
		points[0].z = 1.0f;
		points[0].rhw = 1.0f;
		points[0].diffuse = fillColor;

		points[1].x = (float) to.x + 0.5f;
		points[1].y = points[0].y;
		points[1].z = 1.0f;
		points[1].rhw = 1.0f;
		points[1].diffuse = fillColor;

		points[2].x = points[1].x;
		points[2].y = (float) to.y + 0.5f;
		points[2].z = 1.0f;
		points[2].rhw = 1.0f;
		points[2].diffuse = fillColor;

		points[3].x = points[0].x;
		points[3].y = points[2].y;
		points[3].z = 1.0f;
		points[3].rhw = 1.0f;
		points[3].diffuse = fillColor;

		if (failedObserving(vertexBuffer->Unlock(), "Cannot draw rect: Cannot unlock vertex buffer"))
			return;

		if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2),
			"Cannot draw rect: Cannot draw primitive"))
			return;
	}
	else
	{
		Point * points;
		if (failedObserving(vertexBuffer->Lock(0, 5 * sizeof(Point), (void **) &points,
			D3DLOCK_DISCARD), "Cannot draw rect: Cannot lock vertex buffer"))
			return;

		::ZeroMemory(points, 5 * sizeof(Point));

		points[0].x = (float) (from.x + 1) - 0.5f;
		points[0].y = (float) (from.y + 1) - 0.5f;
		points[0].z = 1.0f;
		points[0].rhw = 1.0f;
		points[0].diffuse = fillColor;

		points[1].x = (float) (to.x - 1) + 0.5f;
		points[1].y = points[0].y;
		points[1].z = 1.0f;
		points[1].rhw = 1.0f;
		points[1].diffuse = fillColor;

		points[2].x = points[1].x;
		points[2].y = (float) (to.y - 1) + 0.5f;
		points[2].z = 1.0f;
		points[2].rhw = 1.0f;
		points[2].diffuse = fillColor;

		points[3].x = points[0].x;
		points[3].y = points[2].y;
		points[3].z = 1.0f;
		points[3].rhw = 1.0f;
		points[3].diffuse = fillColor;

		if (failedObserving(vertexBuffer->Unlock(), "Cannot draw rect: Cannot unlock vertex buffer"))
			return;

		if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2),
			"Cannot draw rect: Cannot draw primitive"))
			return;

		if (GL_ALPHA_OF(borderColor) == 0)
			return;

		if (failedObserving(vertexBuffer->Lock(0, 5 * sizeof(Point), (void **) &points,
			D3DLOCK_DISCARD), "Cannot draw rect: Cannot lock vertex buffer"))
			return;

		points[0].x = (float) from.x;
		points[0].y = (float) from.y;
		points[0].z = 1.0f;
		points[0].rhw = 1.0f;
		points[0].diffuse = borderColor;

		points[1].x = (float) to.x;
		points[1].y = points[0].y;
		points[1].z = 1.0f;
		points[1].rhw = 1.0f;
		points[1].diffuse = borderColor;

		points[2].x = points[1].x;
		points[2].y = (float) to.y;
		points[2].z = 1.0f;
		points[2].rhw = 1.0f;
		points[2].diffuse = borderColor;

		points[3].x = points[0].x;
		points[3].y = points[2].y;
		points[3].z = 1.0f;
		points[3].rhw = 1.0f;
		points[3].diffuse = borderColor;

		points[4].x = points[0].x;
		points[4].y = points[0].y;
		points[4].z = 1.0f;
		points[4].rhw = 1.0f;
		points[4].diffuse = borderColor;

		if (failedObserving(vertexBuffer->Unlock(), "Cannot draw rect: Cannot unlock vertex buffer"))
			return;

		if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 4),
			"Cannot draw rect: Cannot draw primitive"))
			return;
	}
}

void DirectXPlatformAdapter::drawRotatedImage(DrawingContext & context, Image * image,
	Vector sourcePosition, Vector sourceSize, Vector targetPosition, Vector targetSize,
	Vector rotationAnchor, double rotationAngle,
	unsigned long modulationColor, BlendType blendType)
{
	ASSERTION_COBJECT(this);

	if (!ensureScene(context))
		return;

	if (!setTexture(image, blendType))
		return;

	targetPosition += currentContext.getDrawingOffset();
	float xs[4];
	float ys[4];
	xs[0] = (float) targetPosition.x;
	ys[0] = (float) targetPosition.y;
	xs[1] = (float) (targetPosition.x + targetSize.x);
	ys[1] = ys[0];
	xs[2] = xs[0];
	ys[2] = (float) (targetPosition.y + targetSize.y);
	xs[3] = xs[1];
	ys[3] = ys[2];

	float floatAngle = (float) rotationAngle;
	float sinAngle = sinf(floatAngle);
	float cosAngle = cosf(floatAngle);
	float anchorX = (float) rotationAnchor.x * targetSize.x / sourceSize.x
		- sourcePosition.x + targetPosition.x;
	float anchorY = (float) rotationAnchor.y * targetSize.y / sourceSize.y
		- sourcePosition.y + targetPosition.y;
	for (int i=0; i<4; i++)
	{
		float pointX = xs[i] - anchorX;
		float pointY = ys[i] - anchorY;
		float rotatedPointX = pointX * cosAngle - pointY * sinAngle;
		float rotatedPointY = pointX * sinAngle + pointY * cosAngle;
		xs[i] = rotatedPointX + anchorX;
		ys[i] = rotatedPointY + anchorY;
	}

	addTextureRectangle(sourcePosition.x, sourcePosition.y, sourceSize.x, sourceSize.y,
		xs[0], ys[0], xs[1], ys[1], xs[2], ys[2], xs[3], ys[3], modulationColor);
}

void DirectXPlatformAdapter::drawTextInternal(DrawingContext & context, Font * font, Vector position,
	const String & text, unsigned long modulationColor, BlendType blendType, bool caps,
	bool withHotKeys, bool ellipse)
{
	ASSERTION_COBJECT(this);

	String correctedText;
	if (ellipse)
	{
		int maximumWidth = context.getClippingOffset().x
			+ context.getClippingSize().x - position.x - context.getDrawingOffset().x;
		correctedText = font->shortenTextForEllipse(text, maximumWidth);
	}
	else
		correctedText = text;

	Image * image = font->getImage();
	int originalX = position.x;
	int height = font->getHeight();
	for (int charNo = 0; correctedText[charNo] != 0; charNo++)
	{
		unsigned char character = correctedText[charNo];
		if (character == '\n')
			position = Vector(originalX, position.y + height);
		else
		{
			bool hotKey;
			if (withHotKeys && character == '&' && correctedText[charNo+1] != 0)
			{
				character = correctedText[++charNo];
				hotKey = character != '&';
			}
			else
				hotKey = false;

			if (caps)
				character = String::ToUpperCase(character);

			int width = font->getWidth(character);
			if (width > 0)
			{
				Vector glyphSize(width, height);
				drawImage(context, image, font->getPosition(character), glyphSize,
					position, glyphSize, modulationColor, blendType);

				if (hotKey)
					for (int i=0; i<width; i++)
					{
						Vector underlineSize(1, height - 1);
						drawImage(context, image, Vector(1, 1), underlineSize,
							position + Vector(i, 0), underlineSize, modulationColor, blendType);
					}
			}

			position.x += width + 1;
		}
	}
}

void DirectXPlatformAdapter::drawTriangle(DrawingContext & context, Vector * vertices,
	unsigned long fillColor, unsigned long borderColor)
{
	ASSERTION_COBJECT(this);

	if (!ensureScene(context))
		return;

	if (!setTexture(NULL))
		return;

	Point * points;
	if (failedObserving(vertexBuffer->Lock(0, 3 * sizeof(Point), (void **) &points,
		D3DLOCK_DISCARD), "Cannot draw triangle: Cannot lock vertex buffer"))
		return;

	::ZeroMemory(points, 3 * sizeof(Point));

	int i; // loop variable

	for (i=0; i<3; i++)
	{
		points[i].x = (float) (vertices[i].x + currentContext.getDrawingOffset().x);
		points[i].y = (float) (vertices[i].y + currentContext.getDrawingOffset().y);
		points[i].z = 1.0f;
		points[i].rhw = 1.0f;
		points[i].diffuse = fillColor;
	}

	if (failedObserving(vertexBuffer->Unlock(), "Cannot draw triangle: Cannot unlock vertex buffer"))
		return;

	if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1),
		"Cannot draw triangle: Cannot draw triangle primitive"))
		return;

	if (GL_ALPHA_OF(borderColor) == 0)
		return;

	if (failedObserving(vertexBuffer->Lock(0, 4 * sizeof(Point), (void **) &points,
		D3DLOCK_DISCARD), "Cannot draw triangle: Cannot lock vertex buffer"))
		return;

	::ZeroMemory(points, 4 * sizeof(Point));

	for (i=0; i<3; i++)
		points[i].diffuse = borderColor;
	points[3].x = (float) (vertices[0].x + currentContext.getDrawingOffset().x);
	points[3].y = (float) (vertices[0].y + currentContext.getDrawingOffset().y);
	points[3].z = 1.0f;
	points[3].rhw = 1.0f;
	points[3].diffuse = borderColor;

	if (failedObserving(vertexBuffer->Unlock(), "Cannot draw triangle: Cannot unlock vertex buffer"))
		return;

	if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 3),
		"Cannot draw triangle: Cannot draw line strip"))
		return;
}

bool DirectXPlatformAdapter::ensureScene(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	if (direct3DProblems)
		return false;

	if (inScene && currentContext.isCompatibleTo(context))
	{
		if (currentContext != context)
			currentContext = context;

		return true;
	}

	DirectXImage * target = (DirectXImage *) context.getTarget();
	IDirect3DSurface9 * surface = target != NULL ? target->getSurface() : backBufferSurface;

	// Release previous scene

	releaseScene();

	// Prepare viewport

	D3DVIEWPORT9 viewport;
	if (!prepareViewport(context, viewport))
		return false;

	// Set alpha blending, if supported

	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, alphaFullySupported),
		"Cannot enable separate alpha blending"))
		return false;

	// Normal color blending is Sc * Sa + Dc * (1-Sa).
	// This ensures that texture are transparent or opaque according to their alpha channel.
	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA),
		"Cannot set source color blending"))
		return false;
	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA),
		"Cannot set destination color blending"))
		return false;

	if (alphaFullySupported)
	{
		// Normal alpha blending is Sa * 1 + Da * (1-Sa) = Sa + Da - Sa * Da.
		// This is smooth-adding of alpha values to give the resulting texture a natural transparency,
		// if it is used for drawing on other surfaces again. E.g., 0.5 and 0.5 result in 1.
		if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE),
			"Cannot set source alpha blending"))
			return false;
		if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA),
			"Cannot set destination alpha blending"))
			return false;
	}

	// Begin new scene

	if (target != NULL)
	{
		currentRenderer = target->getSurfaceRenderer();
		if (failedObserving(currentRenderer->BeginScene(surface, &viewport),
			"Cannot begin scene on image surface"))
			return false;
	}
	else
	{
		if (failedObserving(direct3DDevice->BeginScene(), "Cannot begin scene on DirectX device"))
			return false;

		if (failedObserving(direct3DDevice->SetViewport(&viewport), "Cannot set viewport on DirectX device"))
			return false;
	}

	// Set vertex buffer for lines, rectangles etc.

	if (failedObserving(direct3DDevice->SetStreamSource(0,
		vertexBuffer, 0, sizeof(Point)), "Cannot set stream source"))
		return false;

	// Set vertex format for lines, rectangles etc.

	if (failedObserving(direct3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1),
		"Cannot set vertex format"))
		return false;

	// Disable culling (since we use 2D models)

	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE),
		"Cannot set culling mode"))
		return false;

	// Enable drawing the last pixel of a line

	// Rendering of last pixel currently does not work. Disabling, and rendering manually now.
	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_LASTPIXEL, false),
		"Cannot set draw last pixel mode"))
		return false;

	// Enable alpha blending

	if (failedObserving(direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true),
		"Cannot disable alpha blending"))
		return false;

	// Use bilinear filtering

	if (failedObserving(direct3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR),
		"Cannot set bilinear filtering for minification"))
		return false;
	if (failedObserving(direct3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR),
		"Cannot set bilinear filtering for magnification"))
		return false;

	// Set texture stage states

	if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE),
		"Cannot set texture stage color operation"))
		return false;
	if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE),
		"Cannot set texture stage color argument 1"))
		return false;
	if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE),
		"Cannot set texture stage color argument 2"))
		return false;

	if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE),
		"Cannot set texture stage alpha operator"))
		return false;
	if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE),
		"Cannot set texture stage alpha argument 1"))
		return false;
	if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE),
		"Cannot set texture stage alpha argument 2"))
		return false;

	// Memorize successful scene preparation

	currentContext = context;
	inScene = true;

	return true;
}

unsigned long DirectXPlatformAdapter::getAbsoluteTime()
{
	ASSERTION_COBJECT(this);
	return (unsigned long) ::time(NULL);
}

int DirectXPlatformAdapter::getApplicationIcon()
{
	ASSERTION_COBJECT(this);
	return applicationIcon;
}

unsigned long DirectXPlatformAdapter::getAvailableVideoMemory()
{
	ASSERTION_COBJECT(this);
	return ((unsigned long) direct3DDevice->GetAvailableTextureMem()) / 1024UL;
}

D3DFORMAT DirectXPlatformAdapter::getBackBufferFormat()
{
	ASSERTION_COBJECT(this);
	return backBufferFormat;
}

String DirectXPlatformAdapter::getClipboardText()
{
	ASSERTION_COBJECT(this);

	String text;
	if (::OpenClipboard(windowHandle))
	{
		HGLOBAL memoryHandle;
		if ((memoryHandle = ::GetClipboardData(CF_TEXT)) != NULL)
		{
			char * memory;
			if ((memory = (char *) ::GlobalLock(memoryHandle)) != NULL)
			{
				text = memory;
				::GlobalUnlock(memoryHandle);
			}
		}
		::CloseClipboard();
	}

	return text;
}

String DirectXPlatformAdapter::getConfigFolder(const String & manufacturer, const String & applicationName)
{
	ASSERTION_COBJECT(this);

	class Path * Path;

	char basePath[MAX_PATH];
	if (::SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA,
		NULL, 0, basePath) == E_INVALIDARG)
		Path = Path::CreateApplicationFolderPath();
	else
	{
		if ((Path = new class Path(basePath)) == NULL)
			throw EOUTOFMEMORY;

		Path->AppendElement(manufacturer);
		Path->AppendElement(applicationName);
		if (!Path->Exists())
			if (!Path->MakeDirectories())
			{
				delete Path;
				Path = Path::CreateApplicationFolderPath();
			}
	}

	char * path = Path->ToString(false);
	delete Path;
	String pathString = String(path);
	delete path;

	return pathString;
}

IDirect3DSurface9 * DirectXPlatformAdapter::getCurrentSurface()
{
	ASSERTION_COBJECT(this);

	if (!inScene)
		return NULL;

	DirectXImage * target = (DirectXImage *) currentContext.getTarget();
	return target != NULL ? target->getSurface() : backBufferSurface;
}

D3DFORMAT DirectXPlatformAdapter::getD3DFormat(int format)
{
	ASSERTION_COBJECT(this);

	if (format == Scene::FORMAT_24BIT)
		return D3DFMT_X8R8G8B8;
	else if (format == Scene::FORMAT_24BIT_WITH_ALPHA)
		return D3DFMT_A8R8G8B8;
	else if (format == Scene::FORMAT_16BIT)
		return D3DFMT_R5G6B5;
	else if (format == Scene::FORMAT_15BIT)
		return D3DFMT_X1R5G5B5;
	else if (format == Scene::FORMAT_15BIT_WITH_ALPHA)
		return D3DFMT_A1R5G5B5;
	else if (format == Scene::FORMAT_12BIT)
		return D3DFMT_X4R4G4B4;
	else if (format == Scene::FORMAT_12BIT_WITH_ALPHA)
		return D3DFMT_A4R4G4B4;
	else if (format == Scene::FORMAT_8BIT)
		return D3DFMT_R3G3B2;
	else if (format == Scene::FORMAT_8BIT_WITH_ALPHA)
		return D3DFMT_A8R3G3B2;
	else
		return D3DFMT_X8R8G8B8;
}

char DirectXPlatformAdapter::getDecimalPoint()
{
	ASSERTION_COBJECT(this);
	return decimalPoint;
}

IDirect3DDevice9 * DirectXPlatformAdapter::getDirect3DDevice()
{
	ASSERTION_COBJECT(this);
	return direct3DDevice;
}

IDirectInput8 * DirectXPlatformAdapter::getDirectInput()
{
	ASSERTION_COBJECT(this);
	return directInput;
}

bool DirectXPlatformAdapter::getDirectXVersion(int * major, int * minor, char * letter)
{
	*major = 0;
	*minor = 0;
	*letter = 0;

	IDxDiagProvider * dxDiagProvider;
	if (FAILED(::CoCreateInstance(CLSID_DxDiagProvider, NULL, CLSCTX_INPROC_SERVER,
		IID_IDxDiagProvider, (void **) &dxDiagProvider)))
		return false;

	DXDIAG_INIT_PARAMS dxDiagInitParams;
	::ZeroMemory(&dxDiagInitParams, sizeof(dxDiagInitParams));
	dxDiagInitParams.dwSize = sizeof(dxDiagInitParams);
	dxDiagInitParams.dwDxDiagHeaderVersion = DXDIAG_DX9_SDK_VERSION;
	dxDiagInitParams.bAllowWHQLChecks = false;
	dxDiagInitParams.pReserved = NULL;

	if (FAILED(dxDiagProvider->Initialize(&dxDiagInitParams)))
	{
		dxDiagProvider->Release();
		return false;
	}

	IDxDiagContainer * dxDiagRoot = NULL;
	IDxDiagContainer * dxDiagSystemInfo = NULL;

	if (FAILED(dxDiagProvider->GetRootContainer(&dxDiagRoot)))
	{
		dxDiagProvider->Release();
		return false;
	}

	if (FAILED(dxDiagRoot->GetChildContainer(L"DxDiag_SystemInfo", &dxDiagSystemInfo)))
	{
		dxDiagRoot->Release();
		dxDiagProvider->Release();
		return false;
	}

	VARIANT variant;
	::VariantInit(&variant);

	if (SUCCEEDED(dxDiagSystemInfo->GetProp(L"dwDirectXVersionMajor", &variant)) && variant.vt == VT_UI4)
		*major = variant.ulVal;
	::VariantClear(&variant);

	if (SUCCEEDED(dxDiagSystemInfo->GetProp(L"dwDirectXVersionMinor", &variant)) && variant.vt == VT_UI4)
		*minor = variant.ulVal;
	::VariantClear(&variant);

	if (SUCCEEDED(dxDiagSystemInfo->GetProp(L"szDirectXVersionLetter", &variant))
		&& variant.vt == VT_BSTR && SysStringLen(variant.bstrVal) != 0)
	{
		char letterString[10];
		::WideCharToMultiByte(CP_ACP, 0, variant.bstrVal, -1, letterString, 10 * sizeof(CHAR), NULL, NULL);
		*letter = letterString[0];
	}
	::VariantClear(&variant);

	dxDiagSystemInfo->Release();
	dxDiagRoot->Release();
	dxDiagProvider->Release();

	return major > 0;
}

JoystickHandler * DirectXPlatformAdapter::getJoystickHandler()
{
	ASSERTION_COBJECT(this);
	return &joystickHandler;
}

KeyboardHandler * DirectXPlatformAdapter::getKeyboardHandler()
{
	ASSERTION_COBJECT(this);
	return &keyboardHandler;
}

const char * DirectXPlatformAdapter::getKeyName(int keyCode)
{
	ASSERTION_COBJECT(this);

	switch (keyCode)
	{
		case DIK_ESCAPE: return STRING_DIRECTX_KEYNAME_ESCAPE;
		case DIK_1: return STRING_DIRECTX_KEYNAME_1;
		case DIK_2: return STRING_DIRECTX_KEYNAME_2;
		case DIK_3: return STRING_DIRECTX_KEYNAME_3;
		case DIK_4: return STRING_DIRECTX_KEYNAME_4;
		case DIK_5: return STRING_DIRECTX_KEYNAME_5;
		case DIK_6: return STRING_DIRECTX_KEYNAME_6;
		case DIK_7: return STRING_DIRECTX_KEYNAME_7;
		case DIK_8: return STRING_DIRECTX_KEYNAME_8;
		case DIK_9: return STRING_DIRECTX_KEYNAME_9;
		case DIK_0: return STRING_DIRECTX_KEYNAME_0;
		case DIK_MINUS: return STRING_DIRECTX_KEYNAME_MINUS;
		case DIK_EQUALS: return STRING_DIRECTX_KEYNAME_EQUALS;
		case DIK_BACK: return STRING_DIRECTX_KEYNAME_BACK;
		case DIK_TAB: return STRING_DIRECTX_KEYNAME_TAB;
		case DIK_Q: return STRING_DIRECTX_KEYNAME_Q;
		case DIK_W: return STRING_DIRECTX_KEYNAME_W;
		case DIK_E: return STRING_DIRECTX_KEYNAME_E;
		case DIK_R: return STRING_DIRECTX_KEYNAME_R;
		case DIK_T: return STRING_DIRECTX_KEYNAME_T;
		case DIK_Y: return STRING_DIRECTX_KEYNAME_Y;
		case DIK_U: return STRING_DIRECTX_KEYNAME_U;
		case DIK_I: return STRING_DIRECTX_KEYNAME_I;
		case DIK_O: return STRING_DIRECTX_KEYNAME_O;
		case DIK_P: return STRING_DIRECTX_KEYNAME_P;
		case DIK_LBRACKET: return STRING_DIRECTX_KEYNAME_LBRACKET;
		case DIK_RBRACKET: return STRING_DIRECTX_KEYNAME_RBRACKET;
		case DIK_RETURN: return STRING_DIRECTX_KEYNAME_RETURN;
		case DIK_LCONTROL: return STRING_DIRECTX_KEYNAME_LCONTROL;
		case DIK_A: return STRING_DIRECTX_KEYNAME_A;
		case DIK_S: return STRING_DIRECTX_KEYNAME_S;
		case DIK_D: return STRING_DIRECTX_KEYNAME_D;
		case DIK_F: return STRING_DIRECTX_KEYNAME_F;
		case DIK_G: return STRING_DIRECTX_KEYNAME_G;
		case DIK_H: return STRING_DIRECTX_KEYNAME_H;
		case DIK_J: return STRING_DIRECTX_KEYNAME_J;
		case DIK_K: return STRING_DIRECTX_KEYNAME_K;
		case DIK_L: return STRING_DIRECTX_KEYNAME_L;
		case DIK_SEMICOLON: return STRING_DIRECTX_KEYNAME_SEMICOLON;
		case DIK_APOSTROPHE: return STRING_DIRECTX_KEYNAME_APOSTROPHE;
		case DIK_GRAVE: return STRING_DIRECTX_KEYNAME_GRAVE;
		case DIK_LSHIFT: return STRING_DIRECTX_KEYNAME_LSHIFT;
		case DIK_BACKSLASH: return STRING_DIRECTX_KEYNAME_BACKSLASH;
		case DIK_Z: return STRING_DIRECTX_KEYNAME_Z;
		case DIK_X: return STRING_DIRECTX_KEYNAME_X;
		case DIK_C: return STRING_DIRECTX_KEYNAME_C;
		case DIK_V: return STRING_DIRECTX_KEYNAME_V;
		case DIK_B: return STRING_DIRECTX_KEYNAME_B;
		case DIK_N: return STRING_DIRECTX_KEYNAME_N;
		case DIK_M: return STRING_DIRECTX_KEYNAME_M;
		case DIK_COMMA: return STRING_DIRECTX_KEYNAME_COMMA;
		case DIK_PERIOD: return STRING_DIRECTX_KEYNAME_PERIOD;
		case DIK_SLASH: return STRING_DIRECTX_KEYNAME_SLASH;
		case DIK_RSHIFT: return STRING_DIRECTX_KEYNAME_RSHIFT;
		case DIK_MULTIPLY: return STRING_DIRECTX_KEYNAME_MULTIPLY;
		case DIK_LMENU: return STRING_DIRECTX_KEYNAME_LMENU;
		case DIK_SPACE: return STRING_DIRECTX_KEYNAME_SPACE;
		case DIK_CAPITAL: return STRING_DIRECTX_KEYNAME_CAPITAL;
		case DIK_F1: return STRING_DIRECTX_KEYNAME_F1;
		case DIK_F2: return STRING_DIRECTX_KEYNAME_F2;
		case DIK_F3: return STRING_DIRECTX_KEYNAME_F3;
		case DIK_F4: return STRING_DIRECTX_KEYNAME_F4;
		case DIK_F5: return STRING_DIRECTX_KEYNAME_F5;
		case DIK_F6: return STRING_DIRECTX_KEYNAME_F6;
		case DIK_F7: return STRING_DIRECTX_KEYNAME_F7;
		case DIK_F8: return STRING_DIRECTX_KEYNAME_F8;
		case DIK_F9: return STRING_DIRECTX_KEYNAME_F9;
		case DIK_F10: return STRING_DIRECTX_KEYNAME_F10;
		case DIK_NUMLOCK: return STRING_DIRECTX_KEYNAME_NUMLOCK;
		case DIK_SCROLL: return STRING_DIRECTX_KEYNAME_SCROLL;
		case DIK_NUMPAD7: return STRING_DIRECTX_KEYNAME_NUMPAD7;
		case DIK_NUMPAD8: return STRING_DIRECTX_KEYNAME_NUMPAD8;
		case DIK_NUMPAD9: return STRING_DIRECTX_KEYNAME_NUMPAD9;
		case DIK_SUBTRACT: return STRING_DIRECTX_KEYNAME_SUBTRACT;
		case DIK_NUMPAD4: return STRING_DIRECTX_KEYNAME_NUMPAD4;
		case DIK_NUMPAD5: return STRING_DIRECTX_KEYNAME_NUMPAD5;
		case DIK_NUMPAD6: return STRING_DIRECTX_KEYNAME_NUMPAD6;
		case DIK_ADD: return STRING_DIRECTX_KEYNAME_ADD;
		case DIK_NUMPAD1: return STRING_DIRECTX_KEYNAME_NUMPAD1;
		case DIK_NUMPAD2: return STRING_DIRECTX_KEYNAME_NUMPAD2;
		case DIK_NUMPAD3: return STRING_DIRECTX_KEYNAME_NUMPAD3;
		case DIK_NUMPAD0: return STRING_DIRECTX_KEYNAME_NUMPAD0;
		case DIK_DECIMAL: return STRING_DIRECTX_KEYNAME_DECIMAL;
		case DIK_OEM_102: return STRING_DIRECTX_KEYNAME_OEM_102;
		case DIK_F11: return STRING_DIRECTX_KEYNAME_F11;
		case DIK_F12: return STRING_DIRECTX_KEYNAME_F12;
		case DIK_F13: return STRING_DIRECTX_KEYNAME_F13;
		case DIK_F14: return STRING_DIRECTX_KEYNAME_F14;
		case DIK_F15: return STRING_DIRECTX_KEYNAME_F15;
		case DIK_KANA: return STRING_DIRECTX_KEYNAME_KANA;
		case DIK_ABNT_C1: return STRING_DIRECTX_KEYNAME_ABNT_C1;
		case DIK_CONVERT: return STRING_DIRECTX_KEYNAME_CONVERT;
		case DIK_NOCONVERT: return STRING_DIRECTX_KEYNAME_NOCONVERT;
		case DIK_YEN: return STRING_DIRECTX_KEYNAME_YEN;
		case DIK_ABNT_C2: return STRING_DIRECTX_KEYNAME_ABNT_C2;
		case DIK_NUMPADEQUALS: return STRING_DIRECTX_KEYNAME_NUMPADEQUALS;
		case DIK_PREVTRACK: return STRING_DIRECTX_KEYNAME_PREVTRACK;
		case DIK_AT: return STRING_DIRECTX_KEYNAME_AT;
		case DIK_COLON: return STRING_DIRECTX_KEYNAME_COLON;
		case DIK_UNDERLINE: return STRING_DIRECTX_KEYNAME_UNDERLINE;
		case DIK_KANJI: return STRING_DIRECTX_KEYNAME_KANJI;
		case DIK_STOP: return STRING_DIRECTX_KEYNAME_STOP;
		case DIK_AX: return STRING_DIRECTX_KEYNAME_AX;
		case DIK_UNLABELED: return STRING_DIRECTX_KEYNAME_UNLABELED;
		case DIK_NEXTTRACK: return STRING_DIRECTX_KEYNAME_NEXTTRACK;
		case DIK_NUMPADENTER: return STRING_DIRECTX_KEYNAME_NUMPADENTER;
		case DIK_RCONTROL: return STRING_DIRECTX_KEYNAME_RCONTROL;
		case DIK_MUTE: return STRING_DIRECTX_KEYNAME_MUTE;
		case DIK_CALCULATOR: return STRING_DIRECTX_KEYNAME_CALCULATOR;
		case DIK_PLAYPAUSE: return STRING_DIRECTX_KEYNAME_PLAYPAUSE;
		case DIK_MEDIASTOP: return STRING_DIRECTX_KEYNAME_MEDIASTOP;
		case DIK_VOLUMEDOWN: return STRING_DIRECTX_KEYNAME_VOLUMEDOWN;
		case DIK_VOLUMEUP: return STRING_DIRECTX_KEYNAME_VOLUMEUP;
		case DIK_WEBHOME: return STRING_DIRECTX_KEYNAME_WEBHOME;
		case DIK_NUMPADCOMMA: return STRING_DIRECTX_KEYNAME_NUMPADCOMMA;
		case DIK_DIVIDE: return STRING_DIRECTX_KEYNAME_DIVIDE;
		case DIK_SYSRQ: return STRING_DIRECTX_KEYNAME_SYSRQ;
		case DIK_RMENU: return STRING_DIRECTX_KEYNAME_RMENU;
		case DIK_PAUSE: return STRING_DIRECTX_KEYNAME_PAUSE;
		case DIK_HOME: return STRING_DIRECTX_KEYNAME_HOME;
		case DIK_UP: return STRING_DIRECTX_KEYNAME_UP;
		case DIK_PRIOR: return STRING_DIRECTX_KEYNAME_PRIOR;
		case DIK_LEFT: return STRING_DIRECTX_KEYNAME_LEFT;
		case DIK_RIGHT: return STRING_DIRECTX_KEYNAME_RIGHT;
		case DIK_END: return STRING_DIRECTX_KEYNAME_END;
		case DIK_DOWN: return STRING_DIRECTX_KEYNAME_DOWN;
		case DIK_NEXT: return STRING_DIRECTX_KEYNAME_NEXT;
		case DIK_INSERT: return STRING_DIRECTX_KEYNAME_INSERT;
		case DIK_DELETE: return STRING_DIRECTX_KEYNAME_DELETE;
		case DIK_LWIN: return STRING_DIRECTX_KEYNAME_LWIN;
		case DIK_RWIN: return STRING_DIRECTX_KEYNAME_RWIN;
		case DIK_APPS: return STRING_DIRECTX_KEYNAME_APPS;
		case DIK_POWER: return STRING_DIRECTX_KEYNAME_POWER;
		case DIK_SLEEP: return STRING_DIRECTX_KEYNAME_SLEEP;
		case DIK_WAKE: return STRING_DIRECTX_KEYNAME_WAKE;
		case DIK_WEBSEARCH: return STRING_DIRECTX_KEYNAME_WEBSEARCH;
		case DIK_WEBFAVORITES: return STRING_DIRECTX_KEYNAME_WEBFAVORITES;
		case DIK_WEBREFRESH: return STRING_DIRECTX_KEYNAME_WEBREFRESH;
		case DIK_WEBSTOP: return STRING_DIRECTX_KEYNAME_WEBSTOP;
		case DIK_WEBFORWARD: return STRING_DIRECTX_KEYNAME_WEBFORWARD;
		case DIK_WEBBACK: return STRING_DIRECTX_KEYNAME_WEBBACK;
		case DIK_MYCOMPUTER: return STRING_DIRECTX_KEYNAME_MYCOMPUTER;
		case DIK_MAIL: return STRING_DIRECTX_KEYNAME_MAIL;
		case DIK_MEDIASELECT: return STRING_DIRECTX_KEYNAME_MEDIASELECT;

		default: return STRING_DIRECTX_KEYNAME_UNKNOWN;
	}
}

int DirectXPlatformAdapter::getMasterVolume()
{
	ASSERTION_COBJECT(this);

	if (endpointVolume != NULL)
	{
		// On Vista and above, the mixer controls the application only, so we use the endpoint volume

		float volume;
		if (endpointVolume->GetMasterVolumeLevelScalar(&volume) == S_OK)
			return (int) (volume * 255.0 + 0.5);
	}

	// On XP, the endpoint volume is not available, and we use the mixer only

	if (mixer == NULL)
		return 0;

	MIXERCONTROL control;
	::ZeroMemory(&control, sizeof(control));

	MIXERLINECONTROLS lineControls;
	::ZeroMemory(&lineControls, sizeof(lineControls));
	lineControls.cbStruct = sizeof(MIXERLINECONTROLS);
	lineControls.dwLineID = mixerLine.dwLineID;
	lineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	lineControls.cControls = 1;
	lineControls.pamxctrl = &control;
	lineControls.cbmxctrl = sizeof(MIXERCONTROL);

	if (::mixerGetLineControls((HMIXEROBJ) mixer, &lineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
	{
		LOGPRINTF0(LOG_WARN, "Cannot get Win32 mixer line controls");
		return 0;
	}

	MIXERCONTROLDETAILS controlDetails;
	::ZeroMemory(&controlDetails, sizeof(controlDetails));

	MIXERCONTROLDETAILS_UNSIGNED controlDetailsValue;
	::ZeroMemory(&controlDetailsValue, sizeof(controlDetailsValue));

	controlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
	controlDetails.hwndOwner = NULL;
	controlDetails.dwControlID = control.dwControlID;
	controlDetails.paDetails = &controlDetailsValue;
	controlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	controlDetails.cChannels = 1;

	if (::mixerGetControlDetails((HMIXEROBJ) mixer, &controlDetails,
		MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
	{
		LOGPRINTF0(LOG_WARN, "Cannot get Win32 mixer volume");
		return 0;
	}

	return controlDetailsValue.dwValue / 256;
}

MouseHandler * DirectXPlatformAdapter::getMouseHandler()
{
	ASSERTION_COBJECT(this);
	return &mouseHandler;
}

unsigned long DirectXPlatformAdapter::getPixel(DrawingContext & context, Vector position)
{
	ASSERTION_COBJECT(this);

	DirectXImage * target = (DirectXImage *) context.getTarget();

	position += context.getDrawingOffset();

	if (!(position >= context.getClippingOffset()
		&& position < context.getClippingOffset() + context.getClippingSize()))
		return GL_TRANSPARENCY;

	IDirect3DSurface9 * surface = target != NULL ? target->getSurface() : backBufferSurface;

	RECT rect;
	rect.left = position.x;
	rect.top = position.y;
	rect.right = position.x + 1;
	rect.bottom = position.y + 1;

	D3DLOCKED_RECT lockedRect;
	if (failedObserving(surface->LockRect(&lockedRect, &rect, D3DLOCK_READONLY),
		"Cannot get pixel color: Cannot lock surface rectangle"))
		return GL_TRANSPARENCY;

	unsigned long pixel = ((long *) lockedRect.pBits)[0];

	surface->UnlockRect();

	return pixel;
}

Vector DirectXPlatformAdapter::getScreenSize()
{
	ASSERTION_COBJECT(this);
	return backBufferSize;
}

String DirectXPlatformAdapter::getSystemInformation()
{
	ASSERTION_COBJECT(this);

	String information;
	String value;

	OSVERSIONINFO osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(osVersionInfo));
	osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);
	if (::GetVersionEx(&osVersionInfo))
	{
		value.Format("Windows: %lu.%lu (build %lu, %s)\n", osVersionInfo.dwMajorVersion, osVersionInfo.dwMinorVersion,
			osVersionInfo.dwBuildNumber, osVersionInfo.szCSDVersion != NULL && osVersionInfo.szCSDVersion[0]
			? osVersionInfo.szCSDVersion : "no Service Pack");
		information += value;
	}
	else
		information += "OS: Unknown\n";

	int directXMajor;
	int directXMinor;
	char directXLetter;
	if (getDirectXVersion(&directXMajor, &directXMinor, &directXLetter))
	{
		value.Format(directXLetter ? "DirectX: %d.%d%c\n" : "DirectX: %d.%d\n",
			directXMajor, directXMinor, directXLetter);
		information += value;
	}
	else
		value.Format("DirectX: Unknown\n");

	MEMORYSTATUSEX memoryStatus;
	::ZeroMemory(&memoryStatus, sizeof(memoryStatus));
	memoryStatus.dwLength = sizeof(memoryStatus);
	if (::GlobalMemoryStatusEx(&memoryStatus))
	{
		value.Format("PhysicalMemory: %lu MB (%lu MB free)\n",
			(unsigned long) (memoryStatus.ullTotalPhys / 1024UL / 1024UL),
			(unsigned long) (memoryStatus.ullAvailPhys / 1024UL / 1024UL));
		information += value;
		value.Format("PageFileMemory: %lu MB (%lu MB free)\n",
			(unsigned long) (memoryStatus.ullTotalPageFile / 1024UL / 1024UL),
			(unsigned long) (memoryStatus.ullAvailPageFile / 1024UL / 1024UL));
		information += value;
	}
	else
	{
		information += "PhysicalMemory: Unknown\n";
		information += "PageFileMemory: Unknown\n";
	}

	bool directXWasInitialized = direct3DDevice != NULL;
	bool directXInitialized = directXWasInitialized;
	if (!directXWasInitialized)
	{
		if ((direct3D = ::Direct3DCreate9(D3D_SDK_VERSION)) != NULL)
		{
			try
			{
				// Use dummy present parameters
				::ZeroMemory(&presentParameters, sizeof(presentParameters));
				presentParameters.BackBufferWidth = 800;
				presentParameters.BackBufferHeight = 600;
				presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
				presentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
				presentParameters.BackBufferCount = 1;
				presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
				presentParameters.SwapEffect = D3DSWAPEFFECT_COPY;
				presentParameters.Windowed = true;
				presentParameters.EnableAutoDepthStencil = false;
				presentParameters.Flags = 0;
				presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

				initializeWindow(NULL, "Glaziery", "Glaziery");
				initializeDirect3DDevice(NULL);

				directXInitialized = true;
			}
			catch (...)
			{
				// Ignored
			}
		}
	}

	if (directXInitialized)
	{
		try
		{
			value.Format("VideoMemory: %lu MB\n", getAvailableVideoMemory()); information += value;
		}
		catch (...)
		{
			value.Format("VideoMemory: Unknown");
		}
	}
	else
		value.Format("VideoMemory: Unknown");

	SYSTEM_INFO systemInfo;
	::ZeroMemory(&systemInfo, sizeof(systemInfo));
	::GetSystemInfo(&systemInfo);
	value.Format("ProcessorCores: %ld\n", systemInfo.dwNumberOfProcessors); information += value;
	value.Format("ProcessorArchitecture: %0X\n", systemInfo.wProcessorArchitecture); information += value;
	value.Format("ProcessorLevel: %0X\n", systemInfo.wProcessorLevel); information += value;

	D3DCAPS9 capabilities;
	HRESULT result;
	if (directXInitialized && SUCCEEDED(result = direct3DDevice->GetDeviceCaps(&capabilities)))
	{
		value.Format("Caps: %l0X\n", capabilities.Caps); information += value;
		value.Format("Caps2: %l0X\n", capabilities.Caps2); information += value;
		value.Format("Caps3: %l0X\n", capabilities.Caps3); information += value;
		value.Format("PresentationIntervals: %l0X\n", capabilities.PresentationIntervals); information += value;
		value.Format("CursorCaps: %l0X\n", capabilities.CursorCaps); information += value;
		value.Format("DevCaps: %l0X\n", capabilities.DevCaps); information += value;
		value.Format("PrimitiveMiscCaps: %l0X\n", capabilities.PrimitiveMiscCaps); information += value;
		value.Format("RasterCaps: %l0X\n", capabilities.RasterCaps); information += value;
		value.Format("ZCmpCaps: %l0X\n", capabilities.ZCmpCaps); information += value;
		value.Format("SrcBlendCaps: %l0X\n", capabilities.SrcBlendCaps); information += value;
		value.Format("DestBlendCaps: %l0X\n", capabilities.DestBlendCaps); information += value;
		value.Format("AlphaCmpCaps: %l0X\n", capabilities.AlphaCmpCaps); information += value;
		value.Format("ShadeCaps: %l0X\n", capabilities.ShadeCaps); information += value;
		value.Format("TextureCaps: %l0X\n", capabilities.TextureCaps); information += value;
		value.Format("TextureFilterCaps: %l0X\n", capabilities.TextureFilterCaps); information += value;
		value.Format("CubeTextureFilterCaps: %l0X\n", capabilities.CubeTextureFilterCaps); information += value;
		value.Format("VolumeTextureFilterCaps: %l0X\n", capabilities.VolumeTextureFilterCaps); information += value;
		value.Format("TextureAddressCaps: %l0X\n", capabilities.TextureAddressCaps); information += value;
		value.Format("VolumeTextureAddressCaps: %l0X\n", capabilities.VolumeTextureAddressCaps);
		information += value;
		value.Format("LineCaps: %l0X\n", capabilities.LineCaps); information += value;
		value.Format("MaxTextureSize: %lux%lu\n", capabilities.MaxTextureWidth, capabilities.MaxTextureHeight);
		information += value;
		value.Format("MaxVolumeExtent: %lu\n", capabilities.MaxVolumeExtent); information += value;
		value.Format("MaxTextureRepeat: %lu\n", capabilities.MaxTextureRepeat); information += value;
		value.Format("MaxTextureAspectRatio: %lu\n", capabilities.MaxTextureAspectRatio); information += value;
		value.Format("MaxAnisotropy: %lu\n", capabilities.MaxAnisotropy); information += value;
		value.Format("MaxVertexW: %f\n", capabilities.MaxVertexW); information += value;
		value.Format("GuardBand: (%f,%f),(%f,%f)\n", capabilities.GuardBandLeft, capabilities.GuardBandTop,
			capabilities.GuardBandRight, capabilities.GuardBandBottom);
		information += value;
		value.Format("ExtentsAdjust: %f\n", capabilities.ExtentsAdjust); information += value;
		value.Format("StencilCaps: %l0X\n", capabilities.StencilCaps); information += value;
		value.Format("FVFCaps: %l0X\n", capabilities.FVFCaps); information += value;
		value.Format("TextureOpCaps: %l0X\n", capabilities.TextureOpCaps); information += value;
		value.Format("MaxTextureBlendStages: %lu\n", capabilities.MaxTextureBlendStages); information += value;
		value.Format("MaxSimultaneousTextures: %lu\n", capabilities.MaxSimultaneousTextures); information += value;
		value.Format("VertexProcessingCaps: %l0X\n", capabilities.VertexProcessingCaps); information += value;
		value.Format("MaxActiveLights: %lu\n", capabilities.MaxActiveLights); information += value;
		value.Format("MaxUserClipPlanes: %lu\n", capabilities.MaxUserClipPlanes); information += value;
		value.Format("MaxVertexBlendMatrices: %lu\n", capabilities.MaxVertexBlendMatrices); information += value;
		value.Format("MaxVertexBlendMatrixIndex: %lu\n", capabilities.MaxVertexBlendMatrixIndex);
		information += value;
		value.Format("MaxPointSize: %f\n", capabilities.MaxPointSize); information += value;
		value.Format("MaxPrimitiveCount: %lu\n", capabilities.MaxPrimitiveCount); information += value;
		value.Format("MaxVertexIndex: %lu\n", capabilities.MaxVertexIndex); information += value;
		value.Format("MaxStreams: %lu\n", capabilities.MaxStreams); information += value;
		value.Format("MaxStreamStride: %lu\n", capabilities.MaxStreamStride); information += value;
		value.Format("VertexShaderVersion: %lu\n", capabilities.VertexShaderVersion); information += value;
		value.Format("MaxVertexShaderConst: %lu\n", capabilities.MaxVertexShaderConst); information += value;
		value.Format("PixelShaderVersion: %lu\n", capabilities.PixelShaderVersion); information += value;
		value.Format("PixelShader1xMaxValue: %f\n", capabilities.PixelShader1xMaxValue); information += value;
		value.Format("DevCaps2: %l0X\n", capabilities.DevCaps2); information += value;
		value.Format("MaxNpatchTessellationLevel: %f\n", capabilities.MaxNpatchTessellationLevel);
		information += value;
		value.Format("NumSimultaneousRTs: %lu\n", capabilities.NumSimultaneousRTs); information += value;
		value.Format("StretchRectFilterCaps: %l0X\n", capabilities.StretchRectFilterCaps); information += value;
		value.Format("VertexTextureFilterCaps: %l0X\n", capabilities.VertexTextureFilterCaps); information += value;
		value.Format("MaxVShaderInstructionsExecuted: %lu\n", capabilities.MaxVShaderInstructionsExecuted);
		information += value;
		value.Format("MaxPShaderInstructionsExecuted: %lu\n", capabilities.MaxPShaderInstructionsExecuted);
		information += value;
		value.Format("MaxVertexShader30InstructionSlots: %lu\n", capabilities.MaxVertexShader30InstructionSlots);
		information += value;
		value.Format("MaxPixelShader30InstructionSlots: %lu\n", capabilities.MaxPixelShader30InstructionSlots);
		information += value;
	}
	else
		information += "No DirectX information available";

	if (!directXWasInitialized)
	{
		try
		{
			releaseDirect3DDevice();
		}
		catch (...)
		{
			// Ignored
		}

		try
		{
			shutDownWindow();
		}
		catch (...)
		{
			// Ignored
		}

		if (direct3D != NULL) {direct3D->Release(); direct3D = NULL;}
	}

	return information;
}

unsigned long DirectXPlatformAdapter::getTime()
{
	ASSERTION_COBJECT(this);

	if (performanceTimerFactor > 0.0)
	{
		LARGE_INTEGER performanceTimerCounter;
		if (::QueryPerformanceCounter(&performanceTimerCounter))
		{
			__int64 performanceTimerCounter64 = performanceTimerCounter.QuadPart;
			if (performanceTimerCounter64 > 0)
				return (unsigned long) ((performanceTimerCounter64 - performanceTimerStart) * performanceTimerFactor);
		}
	}

	return ::GetTickCount();
}

HWND DirectXPlatformAdapter::getWindowHandle()
{
	ASSERTION_COBJECT(this);
	return windowHandle;
}

LRESULT CALLBACK DirectXPlatformAdapter::getWindowProc(HWND windowHandle, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			DirectXPlatformAdapter * adapter
				= (DirectXPlatformAdapter *) Desktop::getInstance()->getPlatformAdapter();
			if (windowHandle == adapter->windowHandle)
			{
				Desktop::getInstance()->onApplicationCloseReceived();
				return 0;
			}
		}
		break;

		case WM_QUIT:
			::PostQuitMessage(0);
		break;
	}

	return ::DefWindowProc(windowHandle, message, wParam, lParam);
}

void DirectXPlatformAdapter::handleFailure(HRESULT result, const char * message)
{
	ASSERTION_COBJECT(this);

	if (direct3DProblems)
		return;

	direct3DProblems = true;

	if (FAILED(direct3DDevice->TestCooperativeLevel()))
		LOGPRINTF1(LOG_WARN, "Direct3D device lost: %s", message);
	else
		LOGPRINTF1(LOG_ERROR, "%s", message);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(DirectXPlatformAdapter, PlatformAdapter);
#endif

void DirectXPlatformAdapter::initialize()
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_INFO, "Initializing DirectX platform adapter");

	// Create Direct 3D
	if ((direct3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		throw EDEVICEERROR_RESULT("Direct3D object couldn't be created", GetLastError());

	// Determine resolution and color format
	updatePresentParameters();

	// Create and initialize window class and window
	Desktop * desktop = Desktop::getInstance();
	Scene * scene = desktop != NULL ? desktop->getActiveScene() : NULL;
	initializeWindow(scene, desktop != NULL ? desktop->getApplicationName() : "Glaziery",
		desktop != NULL ? desktop->getTitle() : "Glaziery");

	// Initialize Direct 3D device
	initializeDirect3DDevice(scene);

	// Get device capabilities
	D3DCAPS9 capabilities;
	HRESULT result;
	if (FAILED(result = direct3DDevice->GetDeviceCaps(&capabilities)))
		throw EDEVICEERROR_RESULT("Cannot get device capabilities", result);

	pow2Required = capabilities.TextureCaps & D3DPTEXTURECAPS_POW2 ? true : false;
	squareRequired = capabilities.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY ? true : false;
	alphaFullySupported = capabilities.PrimitiveMiscCaps & D3DPMISCCAPS_SEPARATEALPHABLEND ? true : false;
	LOGPRINTF3(LOG_DEBUG, "Capabilities: POW2 = %s, SQUAREONLY = %s, SEPARATEALPHABLEND = %s",
		pow2Required ? "true" : "false", squareRequired ? "true" : "false",
		alphaFullySupported ? "true" : "false");

	// Initialize buffers
	initializeBuffers();

	// Initialize Direct Input and Direct Input devices
	initializeDirectInput();

	// Initialize mixer and endpoint volume
	initializeAudio();

	LOGPRINTF0(LOG_INFO, "DirectX platform adapter initialized");
}

void DirectXPlatformAdapter::initializeAudio()
{
	ASSERTION_COBJECT(this);

	// Open mixer and mixer line

	if (mixer != NULL) {::mixerClose(mixer); mixer = NULL;}
	::ZeroMemory(&mixerLine, sizeof(mixerLine));

	if (::mixerOpen(&mixer, 0, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
		mixer = NULL;

	if (mixer == NULL)
	{
		LOGPRINTF0(LOG_ERROR, "Cannot open Win32 mixer");
		return;
	}

	mixerLine.cbStruct = sizeof(MIXERLINE);
	mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	if (::mixerGetLineInfo((HMIXEROBJ) mixer, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
		LOGPRINTF0(LOG_ERROR, "Cannot open Win32 mixer line");

	// On Vista and above, the mixer controls the application only, so open the new endpoint volume as well
	// On XP, the endpoint volume is not available, and we use the mixer only

	IMMDeviceEnumerator * deviceEnumerator = NULL;
	IMMDevice * defaultDevice = NULL;
	if (CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID *) &deviceEnumerator) == S_OK)
	{
		if (deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice) == S_OK)
		{
			if (defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER,
				NULL, (LPVOID *) &endpointVolume) != S_OK)
			{
				LOGPRINTF0(LOG_DEBUG, "Initialized endpoint volume (Vista and above)");
			}
			else
			{
				LOGPRINTF0(LOG_DEBUG, "Cannot open endpoint volume, assuming we have a mixer only (XP)");
			}
		}
		else
		{
			LOGPRINTF0(LOG_DEBUG, "Cannot open default audio endpoint, assuming we have a mixer only (XP)");
		}
	}
	else
	{
		LOGPRINTF0(LOG_DEBUG, "Cannot open device enumerator for the endpoint volume, assuming we have a mixer only (XP)");
	}

	if (deviceEnumerator != NULL) {deviceEnumerator->Release(); deviceEnumerator = NULL;}
	if (defaultDevice != NULL) {defaultDevice->Release(); defaultDevice = NULL;}

	// If we have an endpoint volume, we use it as master volume, so the mixer volume is set to maximum

	if (endpointVolume != NULL)
	{
		MIXERCONTROL control;
		::ZeroMemory(&control, sizeof(control));

		MIXERLINECONTROLS lineControls;
		::ZeroMemory(&lineControls, sizeof(lineControls));
		lineControls.cbStruct = sizeof(MIXERLINECONTROLS);
		lineControls.dwLineID = mixerLine.dwLineID;
		lineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		lineControls.cControls = 1;
		lineControls.pamxctrl = &control;
		lineControls.cbmxctrl = sizeof(MIXERCONTROL);

		if (::mixerGetLineControls((HMIXEROBJ) mixer, &lineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE)
			!= MMSYSERR_NOERROR)
		{
			LOGPRINTF0(LOG_WARN, "Cannot get Win32 mixer line controls");
			return;
		}

		MIXERCONTROLDETAILS controlDetails;
		::ZeroMemory(&controlDetails, sizeof(controlDetails));

		MIXERCONTROLDETAILS_UNSIGNED controlDetailsValue;
		::ZeroMemory(&controlDetailsValue, sizeof(controlDetailsValue));
		controlDetailsValue.dwValue = 65535;

		controlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		controlDetails.hwndOwner = NULL;
		controlDetails.dwControlID = control.dwControlID;
		controlDetails.paDetails = &controlDetailsValue;
		controlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
		controlDetails.cChannels = 1;

		if (::mixerSetControlDetails((HMIXEROBJ) mixer, &controlDetails,
			MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		{
			LOGPRINTF0(LOG_WARN, "Cannot set Win32 mixer volume");
		}
	}
}

void DirectXPlatformAdapter::initializeBuffers()
{
	ASSERTION_COBJECT(this);

	// Release any existing buffers

	releaseBuffers();

	// Prepare surface buffers

	HRESULT result;
	if (FAILED(result = direct3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBufferSurface)))
		throw EDEVICEERROR_RESULT("Cannot get device back buffer surface", result);

	D3DSURFACE_DESC description;
	if (FAILED(result = backBufferSurface->GetDesc(&description)))
		throw EDEVICEERROR_RESULT("Cannot get back buffer surface description", result);

	backBufferSize = Vector(description.Width, description.Height);
	backBufferFormat = description.Format;

	// Prepare vertex buffers

	int vertexBufferSize = VERTEX_BUFFER_SIZE;
	if (CIRCLE_VERTEX_COUNT + 2 > vertexBufferSize)
		vertexBufferSize = CIRCLE_VERTEX_COUNT + 2;
	if (FAILED(result = direct3DDevice->CreateVertexBuffer(vertexBufferSize * sizeof(Point),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1,
		D3DPOOL_DEFAULT, &vertexBuffer, NULL)))
		throw EDEVICEERROR_RESULT("Cannot create vertex buffer", result);
}

void DirectXPlatformAdapter::initializeDirect3DDevice(Scene * scene)
{
	ASSERTION_COBJECT(this);

	// Release any existing buffers and device

	releaseBuffers();
	releaseDirect3DDevice();

	// Ensure that full screen applications have focus
	if (windowHandle != NULL && fullScreen && !isApplicationFocused())
		::SetForegroundWindow(windowHandle);

	// Try creating device using hardware rendering and T&L

	HRESULT mainResult = NULL;
	if (scene == NULL || scene->isHardwareTnLDesirable())
	{
		mainResult = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			windowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&presentParameters, &direct3DDevice);

		// Falling back to a fixed refresh rate, if rate is below 60

		if (FAILED(mainResult) && presentParameters.FullScreen_RefreshRateInHz > 0
			&& presentParameters.FullScreen_RefreshRateInHz < 60)
		{
			LOGPRINTF1(LOG_WARN, "Enumeration provided invalid refresh rate %d, using 75",
				presentParameters.FullScreen_RefreshRateInHz);
			presentParameters.FullScreen_RefreshRateInHz = 75;

			mainResult = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				windowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&presentParameters, &direct3DDevice);
		}

		if (FAILED(mainResult))
		{
			LOGPRINTF1(LOG_INFO, "Hardware T&L not supported. Trying mixed T&L (error code %ld).",
				mainResult);
		}
	}

	// Falling back to mixed rendering, but software T&L

	if (direct3DDevice == NULL)
	{
		if (FAILED(mainResult = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			windowHandle, D3DCREATE_MIXED_VERTEXPROCESSING,
			&presentParameters, &direct3DDevice)))
		{
			LOGPRINTF1(LOG_WARN,
				"Hardware and mixed T&L not supported. Falling back to software T&L (error code %ld).",
				mainResult);
		}
	}

	// Falling back to hardware rendering, but software T&L

	if (direct3DDevice == NULL)
	{
		HRESULT result;
		if (FAILED(result = direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			windowHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&presentParameters, &direct3DDevice)))
		{
			LOGPRINTF1(LOG_WARN,
				"Hardware rendering not supported. Falling back to software rendering"
				" (error code %ld).", result);
		}
	}

	// Falling back to software rendering

	if (direct3DDevice == NULL)
	{
		if (FAILED(direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
			windowHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&presentParameters, &direct3DDevice)))
			throw EDEVICEERROR_RESULT("Direct3D device couldn't be created", mainResult);
	}
}

void DirectXPlatformAdapter::initializeDirectInput()
{
	ASSERTION_COBJECT(this);

	HRESULT result;
	if (FAILED(result = DirectInput8Create(afxCurrentInstanceHandle, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void **) &directInput, NULL)))
		throw EDEVICEERROR_RESULT("Cannot create DirectInput device", result);

	keyboardHandler.initialize();
	mouseHandler.initialize();
	joystickHandler.initialize();
}

void DirectXPlatformAdapter::initializeWindow(Scene * scene, const String & applicationName,
	const String & windowTitle)
{
	ASSERTION_COBJECT(this);

	// Create a unique window class name

	String windowClassName = String("Afx");
	windowClassName += applicationName;
	for (int i=0; i<windowClassName.GetLength();)
		if (!String::IsLetter(windowClassName[i]) && !String::IsDigit(windowClassName[i]))
			windowClassName.Cut(i, 1);
		else
			i++;

	// Create main window class

	WNDCLASSEX windowClass;
	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = getWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = afxCurrentInstanceHandle;
	windowClass.hCursor = ::LoadCursor(NULL, fullScreen ? NULL : IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = (const char *) windowClassName;

	if (applicationIcon != 0)
	{
		windowClass.hIcon = ::LoadIcon(afxCurrentInstanceHandle, (const char *) applicationIcon);
		windowClass.hIconSm = windowClass.hIcon;
	}

	if ((windowClassNameAtom = ::RegisterClassEx(&windowClass)) == 0)
		throw EDEVICEERROR_RESULT("Cannot register main window class", GetLastError());

	// Get desktop size
	RECT desktopRect;
	::GetWindowRect(::GetDesktopWindow(), &desktopRect);

	// Determine position and size of main window

	DWORD windowStyles = fullScreen ? WS_POPUP : WS_BORDER | WS_CAPTION | WS_OVERLAPPED | WS_SYSMENU;

	Vector topRight = Vector((desktopRect.right - desktopRect.left - presentParameters.BackBufferWidth) / 2,
		(desktopRect.bottom - desktopRect.top - presentParameters.BackBufferHeight) / 2);
	RECT rect = {topRight.x, topRight.y, topRight.x + presentParameters.BackBufferWidth,
		topRight.y + presentParameters.BackBufferHeight};
	if (!(::AdjustWindowRect(&rect, windowStyles, false)))
		throw EDEVICEERROR_RESULT("Cannot determine window size", GetLastError());

	// Create main window instance

	if ((windowHandle = ::CreateWindow((const char *) windowClassNameAtom, windowTitle,
		windowStyles, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, afxCurrentInstanceHandle, NULL)) == NULL)
		throw EDEVICEERROR_RESULT("Cannot open main window", GetLastError());
	presentParameters.hDeviceWindow = windowHandle;

	::ShowWindow(windowHandle, scene != NULL ? SW_SHOW : SW_HIDE);
	::UpdateWindow(windowHandle);
}

bool DirectXPlatformAdapter::isAlphaFullySupported()
{
	ASSERTION_COBJECT(this);
	return alphaFullySupported;
}

bool DirectXPlatformAdapter::isApplicationFocused()
{
	ASSERTION_COBJECT(this);
	return ::GetForegroundWindow() == windowHandle;
}

bool DirectXPlatformAdapter::isDeleteKey(int keyCode)
{
	ASSERTION_COBJECT(this);
	return keyCode == DIK_DELETE;
}

bool DirectXPlatformAdapter::isExclusive()
{
	ASSERTION_COBJECT(this);
	return exclusive;
}

bool DirectXPlatformAdapter::isFullScreen()
{
	ASSERTION_COBJECT(this);
	return fullScreen;
}

bool DirectXPlatformAdapter::isMute()
{
	ASSERTION_COBJECT(this);

	if (endpointVolume != NULL)
	{
		// On Vista and above, the mixer controls the application only, so we use the endpoint volume

		BOOL mute;
		if (endpointVolume->GetMute(&mute) == S_OK)
			return mute != 0;
	}

	// On XP, the endpoint volume is not available, and we use the mixer only

	if (mixer == NULL)
		return false;

	MIXERCONTROL control;
	::ZeroMemory(&control, sizeof(control));

	MIXERLINECONTROLS lineControls;
	::ZeroMemory(&lineControls, sizeof(lineControls));
	lineControls.cbStruct = sizeof(MIXERLINECONTROLS);
	lineControls.dwLineID = mixerLine.dwLineID;
	lineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
	lineControls.cControls = 1;
	lineControls.pamxctrl = &control;
	lineControls.cbmxctrl = sizeof(MIXERCONTROL);

	if (::mixerGetLineControls((HMIXEROBJ) mixer, &lineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
	{
		LOGPRINTF0(LOG_WARN, "Cannot get Win32 mixer line controls");
		return false;
	}

	MIXERCONTROLDETAILS controlDetails;
	::ZeroMemory(&controlDetails, sizeof(controlDetails));

	MIXERCONTROLDETAILS_BOOLEAN controlDetailsValue;
	::ZeroMemory(&controlDetailsValue, sizeof(controlDetailsValue));

	controlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
	controlDetails.hwndOwner = NULL;
	controlDetails.dwControlID = control.dwControlID;
	controlDetails.paDetails = &controlDetailsValue;
	controlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	controlDetails.cChannels = 1;

	if (::mixerGetControlDetails((HMIXEROBJ) mixer, &controlDetails,
		MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		LOGPRINTF0(LOG_WARN, "Cannot get Win32 mixer mute");

	return controlDetailsValue.fValue != 0;
}

bool DirectXPlatformAdapter::isPow2Required()
{
	ASSERTION_COBJECT(this);
	return pow2Required;
}

bool DirectXPlatformAdapter::isSquareRequired()
{
	ASSERTION_COBJECT(this);
	return squareRequired;
}

void DirectXPlatformAdapter::minimizeDesktop()
{
	ASSERTION_COBJECT(this);
	::ShowWindow(windowHandle, SW_MINIMIZE);
}

void DirectXPlatformAdapter::onDesktopTitleChanged()
{
	ASSERTION_COBJECT(this);

	if (windowHandle != NULL)
		::SetWindowText(windowHandle, Desktop::getInstance()->getTitle());
}

void DirectXPlatformAdapter::openUrl(const String & url)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->minimize();
	::ShellExecute(windowHandle, "open", (const char *) url, NULL, "", SW_SHOW);
}

bool DirectXPlatformAdapter::prepareViewport(DrawingContext & context,
	D3DVIEWPORT9 & viewport)
{
	ASSERTION_COBJECT(this);

	Vector clippingSize = context.getClippingSize();
	if (!(clippingSize > Vector(0, 0)))
		return false;

	Vector clippingOffset = context.getClippingOffset();
	memset(&viewport, 0, sizeof(viewport));
	viewport.X = clippingOffset.x;
	viewport.Y = clippingOffset.y;
	viewport.Width = clippingSize.x;
	viewport.Height = clippingSize.y;
	viewport.MinZ = 0.0;
	viewport.MaxZ = 1.0;

	return true;
}

void DirectXPlatformAdapter::processMessages()
{
	ASSERTION_COBJECT(this);

	MSG message;
	while (::PeekMessage(&message, NULL, 0U, 0U, PM_REMOVE) > 0)
	{
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
}

void DirectXPlatformAdapter::recoverFromDirect3DProblems()
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_DEBUG, "Trying to recover from a Direct3D problem");

	HRESULT result = direct3DDevice->TestCooperativeLevel();
	if (SUCCEEDED(result))
	{
		direct3DProblems = false;
		LOGPRINTF0(LOG_DEBUG, "Direct3D Problem disappeared");
	}
	else if (result == D3DERR_DEVICENOTRESET)
	{
		// Release all video memory resources
		for (int i=0; i<renderTargetImages.GetCount(); i++)
		{
			DirectXImage * image = (DirectXImage *) renderTargetImages.Get(i);
			image->destroyTexture();
		}

		// Release buffers
		releaseBuffers();

		// Now reset the Direct3D device
		D3DPRESENT_PARAMETERS presentParametersCopy;
		memmove(&presentParametersCopy, &presentParameters, sizeof(D3DPRESENT_PARAMETERS));
		if (SUCCEEDED(direct3DDevice->Reset(&presentParametersCopy)))
		{
			direct3DProblems = false;
			LOGPRINTF0(LOG_INFO, "Recovered from a Direct3D problem");

			// Initialize buffers again
			initializeBuffers();
		}
		else
		{
			LOGPRINTF0(LOG_DEBUG, "Cannot recover from a Direct3D problem");
		}
	}
}

void DirectXPlatformAdapter::releaseBuffers()
{
	ASSERTION_COBJECT(this);

	if (backBufferSurface != NULL) {backBufferSurface->Release(); backBufferSurface = NULL;}
	if (vertexBuffer != NULL) {vertexBuffer->Release(); vertexBuffer = NULL;}
}

void DirectXPlatformAdapter::releaseDirect3DDevice()
{
	ASSERTION_COBJECT(this);

	releaseBuffers();
	if (direct3DDevice != NULL) {direct3DDevice->Release(); direct3DDevice = NULL;}
}

void DirectXPlatformAdapter::releaseScene()
{
	ASSERTION_COBJECT(this);

	if (!inScene)
		return;

	flushVertexBuffer();

	DirectXImage * target = (DirectXImage *) currentContext.getTarget();
	IDirect3DSurface9 * surface = target != NULL ? target->getSurface() : backBufferSurface;

	// End scene

	if (surface == backBufferSurface)
		failedObserving(direct3DDevice->EndScene(), "Cannot end Direct3D device scene");
	else
		failedObserving(currentRenderer->EndScene(D3DX_FILTER_NONE), "Cannot end surface renderer scene");

	// Release scene objects

	currentRenderer = NULL;
	inScene = false;
}

void DirectXPlatformAdapter::restoreDesktop()
{
	ASSERTION_COBJECT(this);
	::ShowWindow(windowHandle, SW_RESTORE);
}

void DirectXPlatformAdapter::setApplicationIcon(int applicationIcon)
{
	ASSERTION_COBJECT(this);
	this->applicationIcon = applicationIcon;
}

void DirectXPlatformAdapter::setClipboardText(const String & text)
{
	ASSERTION_COBJECT(this);

	if (::OpenClipboard(windowHandle))
	{
		if (::EmptyClipboard())
		{
			int textLengthWithNUL = text.GetLength() + 1;
			HGLOBAL memoryHandle;
			if ((memoryHandle = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, textLengthWithNUL)) != NULL)
			{
				char * memory;
				if ((memory = (char *) ::GlobalLock(memoryHandle)) != NULL)
				{
					::CopyMemory(memory, (const char *) text, textLengthWithNUL);
					::GlobalUnlock(memoryHandle);
				}
				::SetClipboardData(CF_TEXT, (void *) memoryHandle);
			}
		}
		::CloseClipboard();
	}
}

void DirectXPlatformAdapter::setMasterVolume(int masterVolume)
{
	ASSERTION_COBJECT(this);

	if (mixer == NULL)
		return;

	if (endpointVolume != NULL)
	{
		LOGPRINTF0(LOG_DEBUG, "Setting master volume using endpoint volume");
		if (endpointVolume->SetMasterVolumeLevelScalar(masterVolume / 255.0f, NULL) != S_OK)
		{
			LOGPRINTF0(LOG_WARN, "Cannot set master volume using endpoint volume");
		}
	}
	else
	{
		// On XP, the endpoint volume is not available, and we use the mixer only

		LOGPRINTF0(LOG_DEBUG, "Setting master volume using Win32 mixer");
		MIXERCONTROL control;
		::ZeroMemory(&control, sizeof(control));

		MIXERLINECONTROLS lineControls;
		::ZeroMemory(&lineControls, sizeof(lineControls));
		lineControls.cbStruct = sizeof(MIXERLINECONTROLS);
		lineControls.dwLineID = mixerLine.dwLineID;
		lineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
		lineControls.cControls = 1;
		lineControls.pamxctrl = &control;
		lineControls.cbmxctrl = sizeof(MIXERCONTROL);

		if (::mixerGetLineControls((HMIXEROBJ) mixer, &lineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE)
			!= MMSYSERR_NOERROR)
		{
			LOGPRINTF0(LOG_WARN, "Cannot get Win32 mixer line controls");
			return;
		}

		MIXERCONTROLDETAILS controlDetails;
		::ZeroMemory(&controlDetails, sizeof(controlDetails));

		MIXERCONTROLDETAILS_UNSIGNED controlDetailsValue;
		::ZeroMemory(&controlDetailsValue, sizeof(controlDetailsValue));
		controlDetailsValue.dwValue = (unsigned short) (masterVolume / 255.0 * 65535.0);

		controlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		controlDetails.hwndOwner = NULL;
		controlDetails.dwControlID = control.dwControlID;
		controlDetails.paDetails = &controlDetailsValue;
		controlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
		controlDetails.cChannels = 1;

		if (::mixerSetControlDetails((HMIXEROBJ) mixer, &controlDetails,
			MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		{
			LOGPRINTF0(LOG_WARN, "Cannot set Win32 mixer volume");
		}
	}
}

void DirectXPlatformAdapter::setMute(bool mute)
{
	ASSERTION_COBJECT(this);

	if (mixer == NULL)
		return;

	if (endpointVolume != NULL)
	{
		// On Vista and above, the mixer controls the application only, so we use the endpoint volume

		LOGPRINTF0(LOG_DEBUG, "Setting mute using endpoint volume");
		if (endpointVolume->SetMute(mute, NULL) != S_OK)
		{
			LOGPRINTF0(LOG_WARN, "Cannot set mute using endpoint volume");
		}
	}
	else
	{
		// On XP, the endpoint volume is not available, and we use the mixer only

		MIXERCONTROL control;
		::ZeroMemory(&control, sizeof(control));

		MIXERLINECONTROLS lineControls;
		::ZeroMemory(&lineControls, sizeof(lineControls));
		lineControls.cbStruct = sizeof(MIXERLINECONTROLS);
		lineControls.dwLineID = mixerLine.dwLineID;
		lineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
		lineControls.cControls = 1;
		lineControls.pamxctrl = &control;
		lineControls.cbmxctrl = sizeof(MIXERCONTROL);

		if (::mixerGetLineControls((HMIXEROBJ) mixer, &lineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE)
			!= MMSYSERR_NOERROR)
		{
			LOGPRINTF0(LOG_WARN, "Cannot get Win32 mixer line controls");
			return;
		}

		MIXERCONTROLDETAILS controlDetails;
		::ZeroMemory(&controlDetails, sizeof(controlDetails));

		MIXERCONTROLDETAILS_BOOLEAN controlDetailsValue;
		::ZeroMemory(&controlDetailsValue, sizeof(controlDetailsValue));
		controlDetailsValue.fValue = mute;

		controlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
		controlDetails.hwndOwner = NULL;
		controlDetails.dwControlID = control.dwControlID;
		controlDetails.paDetails = &controlDetailsValue;
		controlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		controlDetails.cChannels = 1;

		if (::mixerSetControlDetails((HMIXEROBJ) mixer, &controlDetails,
			MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
			LOGPRINTF0(LOG_WARN, "Cannot set Win32 mixer mute");
	}
}

void DirectXPlatformAdapter::showFrame()
{
	ASSERTION_COBJECT(this);

	// First end current scene
	releaseScene();

	// Now present it, if successful
	failedObserving(direct3DDevice->Present(NULL, NULL, NULL, NULL), "Cannot present scene");

	// Finally, check for any problem situation
	if (direct3DProblems)
		recoverFromDirect3DProblems();
}

void DirectXPlatformAdapter::shutDown()
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_INFO, "Shutting down DirectX platform adapter");

	// Release endpoint volume if opened
	if (endpointVolume != NULL) {endpointVolume->Release(); endpointVolume = NULL;}

	// Release mixer
	if (mixer != NULL) {::mixerClose(mixer); mixer = NULL;}

	// Release Direct Input devices and Direct Input
	shutDownDirectInput();

	// Just to make sure, usually not required
	releaseScene();

	// Release buffers and Direct 3D device
	releaseBuffers();
	releaseDirect3DDevice();

	// Close and destroy window and window class
	shutDownWindow();

	// Shut down Direct 3D
	if (direct3D != NULL) {direct3D->Release(); direct3D = NULL;}

	LOGPRINTF0(LOG_INFO, "DirectX platform adapter shut down");
}

void DirectXPlatformAdapter::shutDownDirectInput()
{
	ASSERTION_COBJECT(this);

	keyboardHandler.shutDown();
	mouseHandler.shutDown();
	joystickHandler.shutDown();

	if (directInput != NULL)
	{
		directInput->Release();
		directInput = NULL;
	}
}

void DirectXPlatformAdapter::shutDownWindow()
{
	ASSERTION_COBJECT(this);

	// Close and destroy window
	if (windowHandle != NULL)
	{
		::CloseWindow(windowHandle);
		::DestroyWindow(windowHandle);
		windowHandle = NULL;
	}

	// Unregister window class
	if (windowClassNameAtom != 0)
	{
		if (!::UnregisterClass((const char *) windowClassNameAtom, afxCurrentInstanceHandle))
			throw EDEVICEERROR_RESULT("Cannot unregister main window class", GetLastError());
		windowClassNameAtom = 0;
	}
}

void DirectXPlatformAdapter::sleep(unsigned long msecs)
{
	ASSERTION_COBJECT(this);

	#ifdef GL_DEBUG_CPUPROFILER
		CPUProfiler & profiler = Desktop::getInstance()->getProfiler();
		CPUProfiler::UsageType usageType = profiler.getUsageType();
		profiler.setUsageType(CPUProfiler::IDLE);
	#endif

	::Sleep(msecs);

	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(usageType);
	#endif
}

void DirectXPlatformAdapter::update()
{
	ASSERTION_COBJECT(this);
	processMessages();
}

void DirectXPlatformAdapter::updatePresentParameters()
{
	ASSERTION_COBJECT(this);

	Scene * scene = Desktop::getInstance()->getActiveScene();

	// Clear the present parameters
	::ZeroMemory(&presentParameters, sizeof(presentParameters));

	unsigned int minimumWidth = scene->getMinimumWidth();
	unsigned int minimumHeight = scene->getMinimumHeight();
	unsigned int maximumWidth = scene->getMaximumWidth();
	unsigned int maximumHeight = scene->getMaximumHeight();

	unsigned int desiredWidth = scene->getDesiredWidth();
	unsigned int desiredHeight = scene->getDesiredHeight();
	unsigned int desiredRefreshRate = scene->getDesiredRefreshRate();

	if (desiredWidth == 0 || desiredHeight == 0 || desiredRefreshRate == 0)
	{
		D3DDISPLAYMODE mode;
		HRESULT result;
		if (FAILED(result = direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
			throw EDEVICEERROR_RESULT("Cannot get adapter display mode", result);

		if (desiredWidth == 0) desiredWidth = mode.Width;
		if (desiredHeight == 0) desiredHeight = mode.Height;
		if (desiredRefreshRate == 0) desiredRefreshRate = mode.RefreshRate;
	}

	const ArrayList<Int> & acceptableFormats = scene->getAcceptableFormats();

	bool matchingModeFound = false;
	exclusive = scene->isExclusive();
	fullScreen = scene->isFullScreen();
	if (fullScreen)
	{
		// Full screen rendering requires to choose the display mode

		// First try acceptable formats
		for (int formatNo=0; formatNo < acceptableFormats.GetCount(); formatNo++)
		{
			int format = acceptableFormats.Get(formatNo)->Get();
			D3DFORMAT d3dFormat = getD3DFormat(format);
			if (checkAndChooseMode(d3dFormat, minimumWidth, minimumHeight, maximumWidth, maximumHeight,
				desiredWidth, desiredHeight, desiredRefreshRate))
			{
				matchingModeFound = true;
				break;
			}
		}

		if (!matchingModeFound)
		{
			LOGPRINTF0(LOG_ERROR, "Did not find any acceptable format/mode combination, trying all available formats now");
			for (D3DFORMAT d3dFormat = (D3DFORMAT) 20; d3dFormat <= (D3DFORMAT) 36;
				d3dFormat = (D3DFORMAT) ((int) d3dFormat + 1))
				if (checkAndChooseMode(d3dFormat, minimumWidth, minimumHeight, maximumWidth, maximumHeight,
					desiredWidth, desiredHeight, desiredRefreshRate))
				{
					matchingModeFound = true;
					break;
				}
		}
	}
	else
	{
		// Windowed rendering can select desired parameters (if the screen is large enough)

		presentParameters.BackBufferWidth = desiredWidth;
		presentParameters.BackBufferHeight = desiredHeight;

		D3DDISPLAYMODE mode;
		HRESULT result;
		if (FAILED(result = direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
			throw EDEVICEERROR_RESULT("Cannot get adapter display mode", result);

		if (presentParameters.BackBufferWidth > mode.Width)
			presentParameters.BackBufferWidth = mode.Width;
		if (presentParameters.BackBufferHeight > mode.Height)
			presentParameters.BackBufferHeight = mode.Height;

		presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
		presentParameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		matchingModeFound = true;
	}

	if (!matchingModeFound)
		throw EDEVICEERROR("System supports no acceptable display mode");

	if (!fullScreen)
		presentParameters.FullScreen_RefreshRateInHz = 0;

	presentParameters.BackBufferCount = 1;
	presentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	presentParameters.SwapEffect = fullScreen ? D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_COPY;
	presentParameters.hDeviceWindow = windowHandle;
	presentParameters.Windowed = !fullScreen;
	presentParameters.EnableAutoDepthStencil = false;
	presentParameters.Flags = 0;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	LOGPRINTF4(LOG_INFO, "Selecting display resolution %d x %d, format %d, refresh rate %d",
		presentParameters.BackBufferWidth, presentParameters.BackBufferHeight,
		presentParameters.BackBufferFormat, presentParameters.FullScreen_RefreshRateInHz);
	LOGPRINTF1(LOG_DEBUG, "Present parameter BackBufferCount = %d", presentParameters.BackBufferCount);
	LOGPRINTF1(LOG_DEBUG, "Present parameter MultiSampleType = %d", presentParameters.MultiSampleType);
	LOGPRINTF1(LOG_DEBUG, "Present parameter MultiSampleQuality = %lu", presentParameters.MultiSampleQuality);
	LOGPRINTF1(LOG_DEBUG, "Present parameter SwapEffect = %d", presentParameters.SwapEffect);
	LOGPRINTF1(LOG_DEBUG, "Present parameter Windowed = %s", presentParameters.Windowed ? "true" : "false");
	LOGPRINTF1(LOG_DEBUG, "Present parameter EnableAutoDepthStencil = %s",
		presentParameters.EnableAutoDepthStencil ? "true" : "false");
	LOGPRINTF1(LOG_DEBUG, "Present parameter AutoDepthStencilFormat = %d", presentParameters.AutoDepthStencilFormat);
	LOGPRINTF1(LOG_DEBUG, "Present parameter Flags = %lu", presentParameters.Flags);
	LOGPRINTF1(LOG_DEBUG, "Present parameter PresentationInterval = %d", presentParameters.PresentationInterval);
}
