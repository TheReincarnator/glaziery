/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __BOTTOMRIGHTCUSTOMAREA_H
#define __BOTTOMRIGHTCUSTOMAREA_H


class BottomRightCustomArea : public ClientArea
{
	public:

		class BallWidgetSurrogate : public DraggingSurrogate
		{
			public:

				virtual void onDropped(EventTarget * draggedObject, EventTarget * dropTarget,
					Vector relativePosition, bool option1, bool option2);
				virtual bool supportsDropping(EventTarget * draggedObject,
					EventTarget * dropTarget, Vector relativePosition);
		};

		class BallWidget : public Widget
		{
			friend BallWidgetSurrogate;
			friend BottomRightCustomArea;

			private:

				Vector direction;
				Vector position;
				bool pressed;
				bool show;

			protected:

				virtual DraggingSurrogate * onDragStart(const Vector & startPosition);
				virtual void onPress(Vector position, bool option1, bool option2);
				virtual void onRelease(bool option1, bool option2);

			public:

				BallWidget();
				void draw(DrawingContext & context);
				virtual bool isHitAt(Vector point);
		};

	protected:

		virtual void drawContent(DrawingContext & context);

	public:

		virtual void onDoubleClick(Vector position, bool option1, bool option2);
};


#endif
