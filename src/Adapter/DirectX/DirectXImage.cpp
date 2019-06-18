/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


DirectXImage::DirectXImage(const String & path, Type imageType)
	: Image(path, imageType)
{
	ASSERTION_COBJECT(this);

	fatalErrors = false;
	surface = NULL;
	surfaceRenderer = NULL;
	texture = NULL;
}

DirectXImage::~DirectXImage()
{
	ASSERTION_COBJECT(this);
	destroyTexture();
}

void DirectXImage::destroyTexture()
{
	ASSERTION_COBJECT(this);

	DirectXPlatformAdapter * adapter
		= (DirectXPlatformAdapter *) Desktop::getInstance()->getPlatformAdapter();
	if (surface != NULL && adapter->getCurrentSurface() == surface)
		adapter->releaseScene();

	if (surfaceRenderer != NULL) {surfaceRenderer->Release(); surfaceRenderer = NULL;}
	if (surface != NULL) {surface->Release(); surface = NULL;}

	if (texture != NULL)
	{
		if (adapter->currentTexture == texture)
			adapter->setTexture(NULL);

		texture->Release();
		texture = NULL;
	}

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Image::Listener * imageListener
			= dynamic_cast<Image::Listener *>(listeners.Get(i));
		if (imageListener != NULL)
			imageListener->onRenderTargetInvalidated(this);
	}
}

void DirectXImage::ensureLoaded()
{
	ASSERTION_COBJECT(this);

	if (getPath().IsEmpty())
		return;

	ensureTexture();
}

bool DirectXImage::ensureSurfaceAndHelpers()
{
	ASSERTION_COBJECT(this);

	if (texture == NULL)
		return false;

	if (surface != NULL)
		return true;

	DirectXPlatformAdapter * adapter
		= (DirectXPlatformAdapter *) Desktop::getInstance()->getPlatformAdapter();
	IDirect3DDevice9 * direct3DDevice = adapter->getDirect3DDevice();

	if (direct3DDevice == NULL)
	{
		destroyTexture();
		return false;
	}

	HRESULT result;
	if (FAILED(result = texture->GetSurfaceLevel(0, &surface)))
	{
		destroyTexture();

		if (result == D3DERR_DEVICELOST || result == D3DERR_INVALIDCALL)
			LOGPRINTF0(LOG_WARN, "Direct 3D device has been lost");
		else
		{
			size = Vector();
			fatalErrors = true;
			LOGPRINTF0_RESULT(LOG_ERROR, "Cannot create top surface from texture", result);
		}

		return false;
	}

	if (FAILED(result = D3DXCreateRenderToSurface(direct3DDevice, textureSize.x, textureSize.y,
		getFormat(), false, D3DFMT_UNKNOWN, &surfaceRenderer)))
	{
		surface->Release(); surface = NULL;
		destroyTexture();

		if (result == D3DERR_DEVICELOST || result == D3DERR_INVALIDCALL)
			LOGPRINTF0(LOG_WARN, "Direct 3D device has been lost");
		else
		{
			size = Vector();
			fatalErrors = true;
			LOGPRINTF0_RESULT(LOG_ERROR, "Cannot create image surface renderer", result);
		}

		return false;
	}

	return true;
}

bool DirectXImage::ensureTexture()
{
	ASSERTION_COBJECT(this);

	if (texture != NULL)
		return true;

	if (fatalErrors)
		return false;

	DirectXPlatformAdapter * adapter
		= (DirectXPlatformAdapter *) Desktop::getInstance()->getPlatformAdapter();
	IDirect3DDevice9 * direct3DDevice = adapter->getDirect3DDevice();

	// Delete old texture and helpers
	destroyTexture();

	char * pathString = NULL;
	if (getPath().IsEmpty())
	{
		// Create empty texture
		resize(size);
	}
	else
	{
		// Determine size of file image
		Path * path;
		Path imagePath(getPath());
		if (imagePath.IsAbsolute())
			path = imagePath.HeapClone();
		else
		{
			if ((path = Path::CreateApplicationFolderPath()) == NULL)
				throw EOUTOFMEMORY;
			path->AppendPath(&imagePath);
		}
		pathString = path->ToString(false);
		delete path;

		LOGPRINTF1(LOG_DEBUG, "Loading image file '%s'", pathString);

		D3DXIMAGE_INFO imageInfo;
		::ZeroMemory(&imageInfo, sizeof(imageInfo));
		HRESULT result;
		if (FAILED(result = D3DXGetImageInfoFromFile(pathString, &imageInfo)))
		{
			String message("Cannot get image file info for '");
			message += pathString;
			message += '\'';

			#ifdef _DEBUG
				delete pathString;
				throw EDEVICEERROR_RESULT((const char *) message, result);
			#else
				LOGPRINTF2(LOG_ERROR, "%s: error code %ld", (const char *) message, result);
				imageInfo.Width = 0;
				imageInfo.Height = 0;
				fatalErrors = true;
			#endif
		}

		size = Vector(imageInfo.Width, imageInfo.Height);

		// Constrain to maximum size
		if (maximumSize.x > 0 && size.x > maximumSize.x)
			size.x = maximumSize.x;
		if (maximumSize.y > 0 && size.y > maximumSize.y)
			size.y = maximumSize.y;

		textureSize = adapter->alignSize(size);

		// Create a texture for the image's size
		LOGIFLEVEL_START(LOG_DEBUG)
			LOGPRINTF3(LOG_DEBUG, "Creating a managed pool texture with %dx%d pixels for image ID '%s'",
				textureSize.x, textureSize.y, (const char *) getPath());
		LOGIFLEVEL_END
		if (FAILED(result = direct3DDevice->CreateTexture(textureSize.x, textureSize.y, 1, 0,
			getFormat(), D3DPOOL_MANAGED, &texture, NULL)))
		{
			LOGPRINTF1_RESULT(LOG_ERROR, "Cannot create image file texture for '%s'", result, (const char *) pathString);
			destroyTexture();
			delete pathString;

			size = Vector();
			fatalErrors = true;
			return false;
		}

		if (!ensureSurfaceAndHelpers())
		{
			destroyTexture();
			delete pathString;
			return false;
		}

		// Load the file image

		if (!size.isZero())
		{
			RECT surfaceRect;
			::ZeroMemory(&surfaceRect, sizeof(RECT));
			surfaceRect.right = size.x;
			surfaceRect.bottom = size.y;

			if (FAILED(result = D3DXLoadSurfaceFromFile(surface, NULL, &surfaceRect, pathString, NULL,
				D3DX_FILTER_TRIANGLE, 0, &imageInfo)))
			{
				LOGPRINTF1_RESULT(LOG_ERROR, "Cannot load image file for '%s'", result, (const char *) pathString);
				destroyTexture();
				delete pathString;

				size = Vector();
				fatalErrors = true;
				return false;
			}
		}

		delete pathString;
	}

	return true;
}

D3DFORMAT DirectXImage::getFormat()
{
	ASSERTION_COBJECT(this);

	if (getImageType() == Image::TYPE_RGB)
		return D3DFMT_X8R8G8B8;
	else if (getImageType() == Image::TYPE_ALPHA)
		return D3DFMT_A8;
	else
		return D3DFMT_A8R8G8B8;
}

Vector DirectXImage::getSize()
{
	ASSERTION_COBJECT(this);

	if (!ensureTexture())
		size = Vector();

	return Image::getSize();
}

IDirect3DSurface9 * DirectXImage::getSurface()
{
	ASSERTION_COBJECT(this);

	if (!ensureTexture())
		return NULL;

	return surface;
}

ID3DXRenderToSurface * DirectXImage::getSurfaceRenderer()
{
	ASSERTION_COBJECT(this);

	if (!ensureTexture())
		return NULL;

	return surfaceRenderer;
}

IDirect3DTexture9 * DirectXImage::getTexture()
{
	ASSERTION_COBJECT(this);

	if (!ensureTexture())
		return NULL;

	return texture;
}

Vector DirectXImage::getTextureSize()
{
	ASSERTION_COBJECT(this);

	if (!ensureTexture())
		return Vector();

	return textureSize;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(DirectXImage, Image);
#endif

void DirectXImage::resize(Vector size)
{
	ASSERTION_COBJECT(this);

	// Ensure minimum image size
	if (!(size >= Vector(1, 1)))
		size = Vector(1, 1);

	// Constrain to maximum size
	if (maximumSize.x > 0 && size.x > maximumSize.x)
		size.x = maximumSize.x;
	if (maximumSize.y > 0 && size.y > maximumSize.y)
		size.y = maximumSize.y;

	// Memorize the image size
	this->size = size;

	// Check whether the texture should be recreated
	if (texture != NULL && size <= this->textureSize && size > this->textureSize / 4)
	{
		// The requested new size is smaller or equal in both x and y direction,
		// but it is not MUCH smaller, to leave it, to reduce number of recreations
		return;
	}

	DirectXPlatformAdapter * adapter
		= (DirectXPlatformAdapter *) Desktop::getInstance()->getPlatformAdapter();

	// Align size
	Vector alignedSize = adapter->alignSize(size);

	// Delete old texture, surface, and helpers
	destroyTexture();

	// Get the Direct3D device
	IDirect3DDevice9 * direct3DDevice = adapter->getDirect3DDevice();

	// Check device presence
	if (direct3DDevice == NULL)
		return;

	// Now create new texture
	LOGIFLEVEL_START(LOG_DEBUG)
		LOGPRINTF2(LOG_DEBUG, "Creating a default pool texture with %dx%d pixels", alignedSize.x, alignedSize.y);
	LOGIFLEVEL_END
	HRESULT result;
	if (FAILED(result = direct3DDevice->CreateTexture(alignedSize.x, alignedSize.y, 1,
		D3DUSAGE_RENDERTARGET, getFormat(), D3DPOOL_DEFAULT, &texture, NULL)))
	{
		if (result == D3DERR_DEVICELOST || result == D3DERR_INVALIDCALL)
			LOGPRINTF0(LOG_WARN, "Direct 3D device has been lost");
		else
		{
			size = Vector();
			fatalErrors = true;
			LOGPRINTF0_RESULT(LOG_ERROR, "Cannot create surface texture", result);
		}

		return;
	}

	// Memorize the texture's size
	this->textureSize = alignedSize;

	// Ensure surface and helpers
	if (!ensureSurfaceAndHelpers())
		return;
}

void DirectXImage::unload()
{
	ASSERTION_COBJECT(this);

	if (getPath().IsEmpty())
		return;

	destroyTexture();
}
