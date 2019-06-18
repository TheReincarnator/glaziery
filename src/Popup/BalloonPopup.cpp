/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


BalloonPopup::BalloonPopup()
{
	ASSERTION_COBJECT(this);

	arrowPosition = TOP;
	font = Desktop::getInstance()->getSkin()->getDefaultFieldFont();
	targetX = 0;

	Desktop::getInstance()->getSkin()->setupBalloonPopup(this);
}

void BalloonPopup::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawBalloonPopup(context, this);
}

BalloonPopup::ArrowPosition BalloonPopup::getArrowPosition()
{
	ASSERTION_COBJECT(this);
	return arrowPosition;
}

Font * BalloonPopup::getFont()
{
	ASSERTION_COBJECT(this);
	return font;
}

int BalloonPopup::getTargetX()
{
	ASSERTION_COBJECT(this);
	return targetX;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(BalloonPopup, Popup);
#endif

bool BalloonPopup::isCaps()
{
	ASSERTION_COBJECT(this);
	return caps;
}

bool BalloonPopup::onEnter(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Popup::onEnter(option1, option2);

	cancel();
	return true;
}

const String & BalloonPopup::getMessage()
{
	ASSERTION_COBJECT(this);
	return message;
}

void BalloonPopup::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Popup::onClick(position, option1, option2);

	cancel();
}

bool BalloonPopup::pointTo(Vector target)
{
	ASSERTION_COBJECT(this);
	return pointTo(target, target);
}

bool BalloonPopup::pointTo(Vector targetFrom, Vector targetTo)
{
	ASSERTION_COBJECT(this);

	// Set the arrow position and target X

	Vector targetCenter = (targetFrom + targetTo) / 2;
	Vector desktopSize = Desktop::getInstance()->getSize();
	Vector desktopCenter = desktopSize / 2;
	Vector popupSize = getSize();
	bool left = targetCenter.x <= desktopCenter.x;
	bool top = targetCenter.y <= desktopCenter.y;

	if (top)
		arrowPosition = TOP;
	else
		arrowPosition = BOTTOM;

	targetX = targetCenter.x;

	// Relocate the popup

	int balloonArrowOffset = Desktop::getInstance()->getSkin()->getBalloonArrowOffset(this);

	Vector position;
	if (popupSize.x <= balloonArrowOffset * 2)
		position.x = targetX - popupSize.x / 2;
	else if (left)
		position.x = targetCenter.x - balloonArrowOffset;
	else
		position.x = targetCenter.x - popupSize.x + balloonArrowOffset;

	if (position.x > desktopSize.x - popupSize.x)
		position.x = desktopSize.x - popupSize.x;
	if (position.x < 0)
		position.x = 0;

	if (top)
		position.y = targetTo.y + 1;
	else
		position.y = targetFrom.y - popupSize.y;

	if (position.y > desktopSize.y - popupSize.y)
		position.y = desktopSize.y - popupSize.y;
	if (position.y < 0)
		position.y = 0;

	return moveTo(position);
}

bool BalloonPopup::pointTo(Component * component)
{
	ASSERTION_COBJECT(this);

	Vector absolutePosition = component->getAbsolutePosition();
	return pointTo(absolutePosition, absolutePosition + component->getSize() - Vector(1, 1));
}

void BalloonPopup::setCaps(bool caps)
{
	ASSERTION_COBJECT(this);

	this->caps = caps;

	updateSize();
	invalidate();
}

void BalloonPopup::setFont(Font * font)
{
	ASSERTION_COBJECT(this);

	this->font = font;

	updateSize();
	invalidate();
}

void BalloonPopup::setMessage(const String & message)
{
	ASSERTION_COBJECT(this);

	this->message = message;

	updateSize();
	invalidate();
}

void BalloonPopup::updateSize()
{
	ASSERTION_COBJECT(this);

	Vector borderSize = Desktop::getInstance()->getSkin()->getBalloonBorderSize(this);
	Vector messageSize = font->getSize(message, caps);
	resize(messageSize + borderSize);
}
