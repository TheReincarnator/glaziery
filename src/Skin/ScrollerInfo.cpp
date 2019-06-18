/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ScrollerInfo::ScrollerInfo()
{
	ASSERTION_COBJECT(this);

	horizontalEndX = 0;
	horizontalEndY = 0;
	horizontalHandleEndX = 0;
	horizontalHandleStartX = 0;
	horizontalScrollAreaEndX = 0;
	horizontalScrollAreaStartX = 0;
	horizontalStartX = 0;
	horizontalStartY = 0;
	verticalEndX = 0;
	verticalEndY = 0;
	verticalHandleEndY = 0;
	verticalHandleStartY = 0;
	verticalScrollAreaEndY = 0;
	verticalScrollAreaStartY = 0;
	verticalStartX = 0;
	verticalStartY = 0;
}

ScrollerInfo::ScrollerInfo(const ScrollerInfo & info)
{
	ASSERTION_COBJECT(this);

	horizontalEndX = info.horizontalEndX;
	horizontalEndY = info.horizontalEndY;
	horizontalHandleEndX = info.horizontalHandleEndX;
	horizontalHandleStartX = info.horizontalHandleStartX;
	horizontalScrollAreaEndX = info.horizontalScrollAreaEndX;
	horizontalScrollAreaStartX = info.horizontalScrollAreaStartX;
	horizontalStartX = info.horizontalStartX;
	horizontalStartY = info.horizontalStartY;
	verticalEndX = info.verticalEndX;
	verticalEndY = info.verticalEndY;
	verticalHandleEndY = info.verticalHandleEndY;
	verticalHandleStartY = info.verticalHandleStartY;
	verticalScrollAreaEndY = info.verticalScrollAreaEndY;
	verticalScrollAreaStartY = info.verticalScrollAreaStartY;
	verticalStartX = info.verticalStartX;
	verticalStartY = info.verticalStartY;
}

void ScrollerInfo::fillInfo(ScrollingArea * area)
{
	ASSERTION_COBJECT(this);
	fillInfo(area, Vector(), Vector(), Vector(), 0);
}

void ScrollerInfo::fillInfo(ScrollingField * field)
{
	ASSERTION_COBJECT(this);
	fillInfo(field, Vector(), Vector(), Vector(), 0);
}

void ScrollerInfo::fillInfo(ScrollingArea * area,
	Vector minimumHandleSize)
{
	ASSERTION_COBJECT(this);
	fillInfo(area, minimumHandleSize, Vector(), Vector(), 0);
}

void ScrollerInfo::fillInfo(ScrollingField * field,
	Vector minimumHandleSize)
{
	ASSERTION_COBJECT(this);
	fillInfo(field, minimumHandleSize, Vector(), Vector(), 0);
}

void ScrollerInfo::fillInfo(ScrollingArea * area,
	Vector minimumHandleSize, Vector horizontalWidgetSize,
	Vector verticalWidgetSize)
{
	ASSERTION_COBJECT(this);
	fillInfo(area, minimumHandleSize, horizontalWidgetSize, verticalWidgetSize, 0);
}

void ScrollerInfo::fillInfo(ScrollingField * field,
	Vector minimumHandleSize, Vector horizontalWidgetSize,
	Vector verticalWidgetSize)
{
	ASSERTION_COBJECT(this);
	fillInfo(field, minimumHandleSize, horizontalWidgetSize, verticalWidgetSize, 0);
}

void ScrollerInfo::fillInfo(ScrollingArea * area,
	Vector minimumHandleSize, Vector horizontalWidgetSize,
	Vector verticalWidgetSize, int widgetSpacing)
{
	ASSERTION_COBJECT(this);

	int scrollerBreadth = area->getScrollerBreadth();
	Vector size = area->getSize();
	bool horizontallyScrollable = area->isHorizontallyScrollable();
	bool verticallyScrollable = area->isVerticallyScrollable();
	Vector borderSize = area->getBorderSize();
	Vector contentSize = area->getContentSize();
	Vector contentViewSize = area->getContentViewSize();
	Vector scrollPosition = area->getScrollPositionNow();

	fillInfo(scrollerBreadth, size, horizontallyScrollable, verticallyScrollable,
		borderSize, contentSize, contentViewSize, scrollPosition,
		minimumHandleSize, horizontalWidgetSize, verticalWidgetSize, widgetSpacing);
}

void ScrollerInfo::fillInfo(ScrollingField * field,
	Vector minimumHandleSize, Vector horizontalWidgetSize,
	Vector verticalWidgetSize, int widgetSpacing)
{
	ASSERTION_COBJECT(this);

	int scrollerBreadth = field->getScrollerBreadth();
	Vector size = field->getSize();
	bool horizontallyScrollable = field->isHorizontallyScrollable();
	bool verticallyScrollable = field->isVerticallyScrollable();
	Vector borderSize = field->getBorderSize();
	Vector contentSize = field->getContentSize();
	Vector contentViewSize = field->getContentViewSize();
	Vector scrollPosition = field->getScrollPositionNow();

	fillInfo(scrollerBreadth, size, horizontallyScrollable, verticallyScrollable,
		borderSize, contentSize, contentViewSize, scrollPosition,
		minimumHandleSize, horizontalWidgetSize, verticalWidgetSize, widgetSpacing);
}

void ScrollerInfo::fillInfo(int scrollerBreadth, Vector size, bool horizontallyScrollable, bool verticallyScrollable,
	Vector borderSize, Vector contentSize, Vector contentViewSize, Vector scrollPosition,
	Vector minimumHandleSize, Vector horizontalWidgetSize,
	Vector verticalWidgetSize, int widgetSpacing)
{
	ASSERTION_COBJECT(this);

	if (horizontalWidgetSize.x <= 0) horizontalWidgetSize.x = scrollerBreadth;
	if (horizontalWidgetSize.y <= 0) horizontalWidgetSize.y = scrollerBreadth;
	if (verticalWidgetSize.x <= 0) verticalWidgetSize.x = scrollerBreadth;
	if (verticalWidgetSize.y <= 0) verticalWidgetSize.y = scrollerBreadth;

	if (minimumHandleSize.x <= 0) minimumHandleSize.x = horizontalWidgetSize.x / 2;
	if (minimumHandleSize.y <= 0) minimumHandleSize.y = verticalWidgetSize.y / 2;

	horizontalStartX = 0;
	horizontalEndX = 0;
	horizontalStartY = size.y - scrollerBreadth;
	horizontalEndY = size.y - 1;
	verticalStartX = size.x - scrollerBreadth;
	verticalEndX = size.x - 1;
	verticalStartY = 0;
	verticalEndY = 0;

	if (horizontallyScrollable && verticallyScrollable)
	{
		horizontalStartX = 0; horizontalEndX = size.x - 1 - scrollerBreadth - widgetSpacing;
		verticalStartY = 0; verticalEndY = size.y - 1;
	}
	else if (horizontallyScrollable)
	{
		horizontalStartX = 0; horizontalEndX = size.x - 1;
	}
	else if (verticallyScrollable)
	{
		verticalStartY = 0; verticalEndY = size.y - 1;
	}

	horizontalStartX += borderSize.x;
	horizontalEndX -= borderSize.x;
	horizontalStartY -= borderSize.y;
	horizontalEndY -= borderSize.y;
	verticalStartX -= borderSize.x;
	verticalEndX -= borderSize.x;
	verticalStartY += borderSize.y;
	verticalEndY -= borderSize.y;

	horizontalScrollAreaStartX = horizontalStartX + horizontalWidgetSize.x + widgetSpacing;
	horizontalScrollAreaEndX = horizontalEndX - horizontalWidgetSize.x - widgetSpacing;
	verticalScrollAreaStartY = verticalStartY + verticalWidgetSize.y + widgetSpacing;
	verticalScrollAreaEndY = verticalEndY - verticalWidgetSize.y - widgetSpacing;

	int horizontalScrollAreaSize = horizontalScrollAreaEndX - horizontalScrollAreaStartX + 1;
	horizontalHandleStartX = horizontalScrollAreaStartX;
	horizontalHandleEndX = horizontalScrollAreaEndX;

	if (horizontalScrollAreaSize > 0 && contentViewSize.x > 0
		&& contentSize.x > contentViewSize.x)
	{
		int horizontalHandleWidth = (int) (horizontalScrollAreaSize * 1.0
			* contentViewSize.x / contentSize.x);
		if (horizontalHandleWidth < minimumHandleSize.x)
			horizontalHandleWidth = minimumHandleSize.x;

		double horizontalScrollRatio = scrollPosition.x * 1.0
			/ (contentSize.x - contentViewSize.x + 1);
		if (horizontalScrollRatio < 0.0) horizontalScrollRatio = 0.0;
		if (horizontalScrollRatio > 1.0) horizontalScrollRatio = 1.0;

		horizontalHandleStartX = horizontalScrollAreaStartX
			+ (int) ((horizontalScrollAreaSize - horizontalHandleWidth) * horizontalScrollRatio);
		horizontalHandleEndX = horizontalHandleStartX + horizontalHandleWidth - 1;
	}

	int verticalScrollAreaSize = verticalScrollAreaEndY - verticalScrollAreaStartY + 1;
	verticalHandleStartY = verticalScrollAreaStartY;
	verticalHandleEndY = verticalScrollAreaEndY;

	if (verticalScrollAreaSize > 0 && contentViewSize.y > 0
		&& contentSize.y > contentViewSize.y)
	{
		int verticalHandleHeight = (int) (verticalScrollAreaSize * 1.0
			* contentViewSize.y / contentSize.y);
		if (verticalHandleHeight < minimumHandleSize.y)
			verticalHandleHeight = minimumHandleSize.y;

		double verticalScrollRatio = scrollPosition.y * 1.0
			/ (contentSize.y - contentViewSize.y + 1);
		if (verticalScrollRatio < 0.0) verticalScrollRatio = 0.0;
		if (verticalScrollRatio > 1.0) verticalScrollRatio = 1.0;

		verticalHandleStartY = verticalHandleStartY
			+ (int) ((verticalScrollAreaSize - verticalHandleHeight) * verticalScrollRatio);
		verticalHandleEndY = verticalHandleStartY + verticalHandleHeight - 1;
	}
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ScrollerInfo, CObject);
#endif
