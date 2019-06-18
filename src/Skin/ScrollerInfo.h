/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SCROLLINGFIELDSCROLLERINFO_H
#define __GLAZIERY_SCROLLINGFIELDSCROLLERINFO_H


namespace glaziery
{
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class ScrollerInfo : public CObject
	#else
		class ScrollerInfo
	#endif
	{
		protected:

			void fillInfo(int scrollerBreadth, Vector size, bool horizontallyScrollable, bool verticallyScrollable,
				Vector borderSize, Vector contentSize, Vector contentViewSize, Vector scrollPosition,
				Vector minimumHandleSize, Vector horizontalWidgetSize,
				Vector verticalWidgetSize, int widgetSpacing);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ScrollerInfo);
			#endif

		public:

			int horizontalEndX;

			int horizontalEndY;

			int horizontalHandleEndX;

			int horizontalHandleStartX;

			int horizontalScrollAreaEndX;

			int horizontalScrollAreaStartX;

			int horizontalStartX;

			int horizontalStartY;

			int verticalEndX;

			int verticalEndY;

			int verticalHandleEndY;

			int verticalHandleStartY;

			int verticalScrollAreaEndY;

			int verticalScrollAreaStartY;

			int verticalStartX;

			int verticalStartY;

			/**
			 * Creates an empty scroller info object.
			 */
			ScrollerInfo();

			/**
			 * Creates a scroller info object from another one.
			 * @param info The other scroller info object.
			 */
			ScrollerInfo(const ScrollerInfo & info);

			/**
			 * Fills the scroller info object that it contains all positions of the widgets,
			 * the scroller area, and the handle of both the horizontal and vertical scrollers, if present.
			 * If both scrollers are present, the bottom right corner is used for the horizontal scroller.
			 * @param area The scrolling area to fill the info for.
			 */
			void fillInfo(ScrollingArea * area);

			/**
			 * Fills the scroller info object that it contains all positions of the widgets,
			 * the scroller area, and the handle of both the horizontal and vertical scrollers, if present.
			 * If both scrollers are present, the bottom right corner is used for the horizontal scroller.
			 * @param area The scrolling area to fill the info for.
			 * @param minimumHandleSize The minimum width of a horizontal scrolling handle
			 * resp. the minimum height of a vertical scrolling handle. If 0 is specified for one
			 * of the breadth, which is the default, then half the widget's respective breadth is used.
			 */
			void fillInfo(ScrollingArea * area, Vector minimumHandleSize);

			/**
			 * Fills the scroller info object that it contains all positions of the widgets,
			 * the scroller area, and the handle of both the horizontal and vertical scrollers, if present.
			 * If both scrollers are present, the bottom right corner is used for the horizontal scroller.
			 * @param area The scrolling area to fill the info for.
			 * @param minimumHandleSize The minimum width of a horizontal scrolling handle
			 * resp. the minimum height of a vertical scrolling handle. If 0 is specified for one
			 * of the breadth, which is the default, then half the widget's respective breadth is used.
			 * @param horizontalWidgetSize The width and height of the horizontal widgets.
			 * If (0, 0) is specified, which is the default, then the area's scroller breadth is used.
			 * @param verticalWidgetSize The width and height of the vertical widgets.
			 * If (0, 0) is specified, which is the default, then the area's scroller breadth is used.
			 */
			void fillInfo(ScrollingArea * area, Vector minimumHandleSize,
				Vector horizontalWidgetSize, Vector verticalWidgetSize);

			/**
			 * Fills the scroller info object that it contains all positions of the widgets,
			 * the scroller area, and the handle of both the horizontal and vertical scrollers, if present.
			 * If both scrollers are present, the bottom right corner is used for the horizontal scroller.
			 * @param area The scrolling area to fill the info for.
			 * @param minimumHandleSize The minimum width of a horizontal scrolling handle
			 * resp. the minimum height of a vertical scrolling handle. If 0 is specified for one
			 * of the breadth, which is the default, then half the widget's respective breadth is used.
			 * @param horizontalWidgetSize The width and height of the horizontal widgets.
			 * If (0, 0) is specified, which is the default, then the area's scroller breadth is used.
			 * @param verticalWidgetSize The width and height of the vertical widgets.
			 * If (0, 0) is specified, which is the default, then the area's scroller breadth is used.
			 * @param widgetSpacing The space in pixels between the widgets and the scroller areas.
			 * Defaults to 0.
			 */
			void fillInfo(ScrollingArea * area, Vector minimumHandleSize,
				Vector horizontalWidgetSize, Vector verticalWidgetSize, int widgetSpacing);

			/**
			 * Fills the scroller info object that it contains all positions of the widgets,
			 * the scroller area, and the handle of both the horizontal and vertical scrollers, if present.
			 * If both scrollers are present, the bottom right corner is used for the horizontal scroller.
			 * @param field The scrolling field to fill the info for.
			 */
			void fillInfo(ScrollingField * field);

			/**
			 * Fills the scroller info object that it contains all positions of the widgets,
			 * the scroller area, and the handle of both the horizontal and vertical scrollers, if present.
			 * If both scrollers are present, the bottom right corner is used for the horizontal scroller.
			 * @param field The scrolling field to fill the info for.
			 * @param minimumHandleSize The minimum width of a horizontal scrolling handle
			 * resp. the minimum height of a vertical scrolling handle. If 0 is specified for one
			 * of the breadth, which is the default, then half the widget's respective breadth is used.
			 */
			void fillInfo(ScrollingField * field, Vector minimumHandleSize);

			/**
			 * Fills the scroller info object that it contains all positions of the widgets,
			 * the scroller area, and the handle of both the horizontal and vertical scrollers, if present.
			 * If both scrollers are present, the bottom right corner is used for the horizontal scroller.
			 * @param field The scrolling field to fill the info for.
			 * @param minimumHandleSize The minimum width of a horizontal scrolling handle
			 * resp. the minimum height of a vertical scrolling handle. If 0 is specified for one
			 * of the breadth, which is the default, then half the widget's respective breadth is used.
			 * @param horizontalWidgetSize The width and height of the horizontal widgets.
			 * If (0, 0) is specified, which is the default, then the field's scroller breadth is used.
			 * @param verticalWidgetSize The width and height of the vertical widgets.
			 * If (0, 0) is specified, which is the default, then the field's scroller breadth is used.
			 */
			void fillInfo(ScrollingField * field, Vector minimumHandleSize,
				Vector horizontalWidgetSize, Vector verticalWidgetSize);

			/**
			 * Fills the scroller info object that it contains all positions of the widgets,
			 * the scroller area, and the handle of both the horizontal and vertical scrollers, if present.
			 * If both scrollers are present, the bottom right corner is used for the horizontal scroller.
			 * @param field The scrolling field to fill the info for.
			 * @param minimumHandleSize The minimum width of a horizontal scrolling handle
			 * resp. the minimum height of a vertical scrolling handle. If 0 is specified for one
			 * of the breadth, which is the default, then half the widget's respective breadth is used.
			 * @param horizontalWidgetSize The width and height of the horizontal widgets.
			 * If (0, 0) is specified, which is the default, then the field's scroller breadth is used.
			 * @param verticalWidgetSize The width and height of the vertical widgets.
			 * If (0, 0) is specified, which is the default, then the field's scroller breadth is used.
			 * @param widgetSpacing The space in pixels between the widgets and the scroller areas.
			 * Defaults to 0.
			 */
			void fillInfo(ScrollingField * field, Vector minimumHandleSize,
				Vector horizontalWidgetSize, Vector verticalWidgetSize, int widgetSpacing);
	};
}


#endif
