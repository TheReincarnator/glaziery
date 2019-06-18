/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


CachableComponent::CachableComponent()
{
	ASSERTION_COBJECT(this);

	withAlphaChannel = Desktop::getInstance()->getPlatformAdapter()->isAlphaFullySupported();
	caching = false;
	focusAlpha = 255;
	nonFocusAlpha = 255;
	surface = NULL;
	valid = true;
}

CachableComponent::~CachableComponent()
{
	ASSERTION_COBJECT(this);
	releaseCache();
}

void CachableComponent::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	if (!caching)
	{
		drawContent(context);
		return;
	}

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	if (surface == NULL)
	{
		surface = adapter->createRenderTarget(getSize(), withAlphaChannel ? Image::TYPE_RGB_ALPHA : Image::TYPE_RGB);

		SurfaceListener * surfaceListener;
		if ((surfaceListener = new SurfaceListener(this)) == NULL)
			throw EOUTOFMEMORY;
		surface->appendListener(surfaceListener);
	}

	if (!valid)
		surface->resize(getSize());

	if (!valid)
	{
		// Set valid to true before updating the surface,
		// to enable the drawing method to invalidate the surface while drawing
		valid = true;

		LOGPRINTF0(LOG_DEBUG, "Redrawing cachable component's content...");
		drawContent(DrawingContext(DrawingContext(DrawingContext(surface),
			invalidAreaPosition, invalidAreaSize), -invalidAreaPosition));
		LOGPRINTF0(LOG_DEBUG, "Cachable component's redrawn");
	}

	Skin * skin = Desktop::getInstance()->getSkin();
	adapter->drawImage(context, surface, Vector(), getModulationColor());

	#ifdef GL_DEBUG_INVALIDATIONS
		if (!redrawRegions.IsEmpty())
		{
			unsigned long latestTime = Desktop::getInstance()->getFrameTime() - 500;
			int i;
			for (i = redrawRegions.GetCount() - 1; i >= 0; i--)
			{
				RedrawRegion * region = redrawRegions.Get(i);
				if (region->time < latestTime)
					break;

				Desktop * desktop = Desktop::getInstance();
				PlatformAdapter * adapter = desktop->getPlatformAdapter();

				unsigned int alpha = (unsigned int) (region->time - latestTime);
				if (alpha > 255)
					alpha = 255;

				adapter->drawRect(context, region->position, region->position + region->size - Vector(1, 1),
					GL_RGBA(128, 16, 64, alpha / 2), GL_RGBA(255, 32, 128, alpha));
			}

			for (; i >= 0; i--)
				redrawRegions.Delete(i);

			Component * parent = getParent();
			if (parent != NULL)
				parent->invalidate();
		}
	#endif
}

unsigned char CachableComponent::getAlpha()
{
	ASSERTION_COBJECT(this);
	return hasFocus() ? focusAlpha : nonFocusAlpha;
}

unsigned char CachableComponent::getFocusAlpha()
{
	ASSERTION_COBJECT(this);
	return focusAlpha;
}

unsigned long CachableComponent::getModulationColor()
{
	ASSERTION_COBJECT(this);
	return GL_WHITE_A(hasFocus() ? focusAlpha : nonFocusAlpha);
}

unsigned char CachableComponent::getNonFocusAlpha()
{
	ASSERTION_COBJECT(this);
	return nonFocusAlpha;
}

void CachableComponent::hide()
{
	ASSERTION_COBJECT(this);

	Component::hide();
	releaseCache();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(CachableComponent, Component);
#endif

void CachableComponent::invalidateArea(Vector position, Vector size)
{
	ASSERTION_COBJECT(this);

	Component::invalidateArea(position, size);

	Vector componentSize = getSize();
	position.constrain(Vector(0, 0), componentSize - Vector(1, 1));
	size.constrain(Vector(0, 0), componentSize - position);

	if (valid)
	{
		valid = false;

		invalidAreaPosition = position;
		invalidAreaSize = size;
	}
	else
	{
		Vector toPosition = position + size;
		Vector currentToPosition = invalidAreaPosition + invalidAreaSize;

		if (position.x < invalidAreaPosition.x)
		{
			if (position.x < 0)
				invalidAreaPosition.x = 0;
			else
				invalidAreaPosition.x = position.x;
		}

		if (position.y < invalidAreaPosition.y)
		{
			if (position.y < 0)
				invalidAreaPosition.y = 0;
			else
				invalidAreaPosition.y = position.y;
		}

		if (toPosition.x > currentToPosition.x)
		{
			if (toPosition.x > componentSize.x)
				currentToPosition.x = componentSize.x;
			else
				currentToPosition.x = toPosition.x;

		}

		if (toPosition.y > currentToPosition.y)
		{
			if (toPosition.y > componentSize.y)
				currentToPosition.y = componentSize.y;
			else
				currentToPosition.y = toPosition.y;
		}

		invalidAreaSize = currentToPosition - invalidAreaPosition;
	}

	if (invalidAreaSize.y == 1)
		invalidAreaSize.y = 1;

	#ifdef GL_DEBUG_INVALIDATIONS
		LOGPRINTF5(LOG_DEBUG, "Invalidation on %s at (%d,%d)-(%d,%d)", (const char *) toString(),
			position.x, position.y, position.x + size.x - 1, position.y + size.y - 1);

		for (int i=0; i<redrawRegions.GetCount();)
		{
			RedrawRegion * region = redrawRegions.Get(i);
			if (region->position == position && region->size == size)
				redrawRegions.Delete(i);
			else
				i++;
		}

		RedrawRegion * region;
		if ((region = new RedrawRegion(position, size)) == NULL)
			throw EOUTOFMEMORY;
		redrawRegions.Append(region);
	#endif
}

bool CachableComponent::isCaching()
{
	ASSERTION_COBJECT(this);
	return caching;
}

bool CachableComponent::isValid()
{
	ASSERTION_COBJECT(this);
	return valid;
}

bool CachableComponent::isWithAlphaChannel()
{
	ASSERTION_COBJECT(this);
	return withAlphaChannel;
}

void CachableComponent::onRenderTargetInvalidated(Image * renderTarget)
{
	ASSERTION_COBJECT(this);

	if (renderTarget == surface)
		invalidate();
}

void CachableComponent::releaseCache()
{
	ASSERTION_COBJECT(this);

	if (surface != NULL)
	{
		Desktop::getInstance()->getPlatformAdapter()->destroyRenderTarget(surface);
		surface = NULL;
	}
}

void CachableComponent::setCaching(bool caching)
{
	ASSERTION_COBJECT(this);

	if (this->caching != caching)
	{
		this->caching = caching;
		invalidate();
		releaseCache();
	}
}

void CachableComponent::setFocusAlpha(unsigned char focusAlpha)
{
	ASSERTION_COBJECT(this);

	if (this->focusAlpha != focusAlpha)
	{
		this->focusAlpha = focusAlpha;
		invalidate();
	}
}

void CachableComponent::setNonFocusAlpha(unsigned char nonFocusAlpha)
{
	ASSERTION_COBJECT(this);

	if (this->nonFocusAlpha != nonFocusAlpha)
	{
		this->nonFocusAlpha = nonFocusAlpha;
		invalidate();
	}
}

void CachableComponent::setWithAlphaChannel(bool withAlphaChannel)
{
	ASSERTION_COBJECT(this);

	if (this->withAlphaChannel != withAlphaChannel)
	{
		this->withAlphaChannel = withAlphaChannel;
		invalidate();
		releaseCache();
	}
}

String CachableComponent::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("CachableComponent(position:%s,size:%s)",
		(const char *) getPosition().toString(), (const char *) getSize().toString());
	return string;
}


CachableComponent::RedrawRegion::RedrawRegion(const Vector & position, const Vector & size)
{
	ASSERTION_COBJECT(this);

	this->position = position;
	this->size = size;
	time = Desktop::getInstance()->getFrameTime();
}


CachableComponent::SurfaceListener::SurfaceListener(CachableComponent * component)
{
	ASSERTION_COBJECT(this);
	this->component = component;
}

void CachableComponent::SurfaceListener::onRenderTargetInvalidated(Image * renderTarget)
{
	ASSERTION_COBJECT(this);
	component->onRenderTargetInvalidated(renderTarget);
}
