/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SIMPLESKIN_H
#define __GLAZIERY_SIMPLESKIN_H


namespace glaziery
{
	/**
	 * A simple skin implementation for learning and basic rendering purposes.
	 * See the base class for more details.
	 * @see Skin
	 */
	class SimpleSkin : public Skin
	{
		public:

			class ListFieldColumnSizeWidget : public Widget
			{
				protected:

					int lastHitColumnNo;
					bool resize;
					int startWidth;

				public:

					ListFieldColumnSizeWidget();
					virtual PointerType * getPointerType(Vector position);
					virtual bool isHitAt(Vector point);
					virtual void onDragMove(const Vector & startPosition,
						const Vector & lastPosition, const Vector & currentPosition);
					virtual DraggingSurrogate * onDragStart(const Vector & startPosition);
			};

			class ScrollingComponentArrowWidget : public Widget
			{
				public:

					enum Arrow
					{
						NONE, LEFT_ARROW, LEFT_PAGE, RIGHT_ARROW, RIGHT_PAGE,
							UP_ARROW, UP_PAGE, DOWN_ARROW, DOWN_PAGE
					};

				protected:

					Arrow arrow;
					int stepsToSkip;

					void executeArrow(int steps);
					Arrow getHitArrow(Vector position);
					virtual void onDoubleClick(Vector position, bool option1, bool option2);
					virtual long onHold(Vector position, long holdTime);
					virtual void onPress(Vector position, bool option1, bool option2);

				public:

					ScrollingComponentArrowWidget();
					virtual bool isHitAt(Vector point);
			};

			class ScrollingComponentHandleWidget : public Widget
			{
				public:

					enum Direction
					{
						NONE, HORIZONTALLY, VERTICALLY
					};

				protected:

					Direction direction;
					Vector scrollerHandleStartPosition;

					Direction getHitDirection(Vector position);
					virtual void onDragMove(const Vector & startPosition,
						const Vector & lastPosition, const Vector & currentPosition);
					virtual DraggingSurrogate * onDragStart(const Vector & startPosition);

				public:

					ScrollingComponentHandleWidget();
					virtual bool isHitAt(Vector point);
			};

			class SliderWidget : public Widget
			{
				protected:

					virtual void onDragMove(const Vector & startPosition,
						const Vector & lastPosition, const Vector & currentPosition);
					virtual DraggingSurrogate * onDragStart(const Vector & startPosition);

				public:

					virtual bool isHitAt(Vector point);
			};

			class SplitterWidget : public Widget
			{
				protected:

					int splitter;

					virtual void onDragMove(const Vector & startPosition,
						const Vector & lastPosition, const Vector & currentPosition);
					virtual DraggingSurrogate * onDragStart(const Vector & startPosition);

				public:

					SplitterWidget();
					virtual PointerType * getPointerType(Vector position);
					virtual bool isHitAt(Vector point);
			};

			class TabWidget : public Widget
			{
				protected:

					int TabWidget::getTabAt(Vector position);
					virtual void onClick(Vector position, bool option1, bool option2);

				public:

					virtual bool isHitAt(Vector point);
			};

			class WindowCloseWidget : public Widget
			{
				protected:

					virtual void onClick(Vector position, bool option1, bool option2);

				public:

					virtual bool isHitAt(Vector point);
			};

			class WindowBorderWidget : public Widget
			{
				protected:

					bool leftBorderHit;
					bool rightBorderHit;
					bool topBorderHit;
					bool bottomBorderHit;

					virtual void onDragMove(const Vector & startPosition,
						const Vector & lastPosition, const Vector & currentPosition);
					virtual DraggingSurrogate * onDragStart(const Vector & startPosition);

				public:

					WindowBorderWidget();
					virtual PointerType * getPointerType(Vector position);
					virtual bool isHitAt(Vector point);
			};

			class WindowMaximizeWidget : public Widget
			{
				protected:

					virtual void onClick(Vector position, bool option1, bool option2);

				public:

					virtual bool isHitAt(Vector point);
			};

			class WindowTitleWidget : public Widget
			{
				protected:

					bool checkingHit;

					virtual void onDoubleClick(Vector position, bool option1, bool option2);
					virtual void onDragMove(const Vector & startPosition,
						const Vector & lastPosition, const Vector & currentPosition);
					virtual DraggingSurrogate * onDragStart(const Vector & startPosition);

				public:

					WindowTitleWidget();
					virtual bool isHitAt(Vector point);
			};

		friend SliderWidget;
		friend SplitterWidget;
		friend TabWidget;
		friend WindowBorderWidget;
		friend WindowTitleWidget;

		protected:

			static const int BLUE;
			static const int DARK_BLUE;
			static const int DEEP_DARK_BLUE;
			static const int LIGHT_BLUE;
			static const char MASK_CHARACTER;

			Image * cdromImage;
			Image * checkBoxCheckedImage;
			Image * checkBoxTriStateImage;
			Image * checkBoxUncheckedImage;
			Image * closeWidgetImage;
			Image * collapseWidgetImage;
			PointerType * draggingPointerType;
			Image * downWidgetImage;
			Image * errorIconImage;
			Image * expandWidgetImage;
			Font * fieldFont;
			Image * fileImage;
			Image * floppyDiskImage;
			int focusableFactor;
			Image * folderImage;
			Image * hardDiskImage;
			PointerType * hourGlassPointerType;
			PointerType * impossiblePointerType;
			Image * informationIconImage;
			Image * leftWidgetImage;
			Image * maximizeWidgetImage;
			Image * memoryStickImage;
			Image * menuCheckImage;
			Image * menuRadioImage;
			Image * networkComputerImage;
			Image * networkDriveImage;
			Image * questionIconImage;
			Image * radioBoxCheckedImage;
			Image * radioBoxUncheckedImage;
			Image * ramDiskImage;
			PointerType * resize0PointerType;
			PointerType * resize45PointerType;
			PointerType * resize90PointerType;
			PointerType * resize135PointerType;
			Image * rightWidgetImage;
			Image * rootImage;
			PointerType * selectPointerType;
			Font * tabFont;
			Image * unknownImage;
			Image * upWidgetImage;
			Image * virtualImage;
			Image * warningIconImage;
			Font * windowTitleFont;

			virtual ~SimpleSkin();
			virtual void drawListFieldCell(DrawingContext & context, ListField::Cell * cell,
				Vector cellSize, bool focused, bool textElements);
			virtual void drawListFieldRow(DrawingContext & context, ListField::Row * row, bool focused);
			void drawScrollingComponent(DrawingContext & context, Component * component);
			virtual void drawTreeFieldItem(DrawingContext & context, TreeField::Item * item, bool last);
			virtual int getAlpha(Component * component);
			virtual void initializeFonts();
			virtual void initializePathTypeImages();
			virtual void initializeRegularImages();
			virtual void initializePointerTypes();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(SimpleSkin);
			#endif

		public:

			SimpleSkin();
			virtual void addPopupCreateEffect(Popup * popup);
			virtual void addWindowCreateEffect(Window * window);
			virtual void addPopupDestroyEffect(Popup * popup);
			virtual void addWindowDestroyEffect(Window * window);
			virtual void drawBalloonPopup(DrawingContext & context, BalloonPopup * popup);
			virtual void drawButton(DrawingContext & context, Button * button);
			virtual void drawCheckBox(DrawingContext & context, CheckBox * checkBox);
			virtual void drawDesktop(DrawingContext & context);
			virtual void drawFieldsArea(DrawingContext & context, FieldsArea * area);
			virtual void drawGroupField(DrawingContext & context, GroupField * groupField);
			virtual void drawImageElement(DrawingContext & context, ImageElement * element);
			virtual void drawImageField(DrawingContext & context, ImageField * field);
			virtual void drawKeyStrokeField(DrawingContext & context, KeyStrokeField * field);
			virtual void drawLabel(DrawingContext & context, Label * label);
			virtual void drawListField(DrawingContext & context, ListField * field);
			virtual void drawListFieldRowsField(DrawingContext & context,
				ListField::RowsField * field);
			virtual void drawMenuPopup(DrawingContext & context, MenuPopup * menuPopup);
			virtual void drawProgressBar(DrawingContext & context,
				ProgressBar * progressBar);
			virtual void drawRadioBox(DrawingContext & context, RadioBox * radioBox);
			virtual void drawScrollingArea(DrawingContext & context, ScrollingArea * area);
			virtual void drawScrollingField(DrawingContext & context, ScrollingField * field);
			virtual void drawSelector(DrawingContext & context, Selector * selector);
			virtual void drawSingleFieldArea(DrawingContext & context, SingleFieldArea * area);
			virtual void drawSingleFieldPopup(DrawingContext & context, SingleFieldPopup * popup);
			virtual void drawSlider(DrawingContext & context, Slider * slider);
			virtual void drawSplash(DrawingContext & context);
			virtual void drawSplittingArea(DrawingContext & context, SplittingArea * area);
			virtual void drawTabbingArea(DrawingContext & context, TabbingArea * area);
			virtual void drawTextElement(DrawingContext & context, TextElement * element);
			virtual void drawTextField(DrawingContext & context, TextField * field);
			virtual void drawTreeField(DrawingContext & context, TreeField * field);
			virtual void drawWindowAfterClientArea(DrawingContext & context, Window * window);
			virtual void drawWindowBeforeClientArea(DrawingContext & context, Window * window);
			virtual Vector getBalloonBorderSize(BalloonPopup * popup);
			virtual int getBalloonArrowOffset(BalloonPopup * popup);
			virtual Vector getButtonSize(Button * button);
			virtual Vector getCheckBoxSize(CheckBox * checkBox);
			virtual PointerType * getDefaultDraggingPointerType();
			virtual Font * getDefaultFieldFont();
			virtual PointerType * getDefaultImpossiblePointerType();
			virtual void getDialogMargins(Dialog * dialog, int & bottomContentMargin,
				int & buttonSpacing, int & iconContentSpacing);
			virtual void getGroupFieldMarginSizes(GroupField * field,
				Vector & topLeftSize, Vector & bottomRightSize);
			virtual PointerType * getHourGlassPointerType();
			virtual int getListFieldColumnBorderWidth(ListField * field);
			virtual int getListFieldColumnPaddingWidth(ListField * field);
			virtual int getListFieldHeadingHeight(ListField * field);
			virtual int getListFieldRowHeight(ListField * field);
			virtual Image * getMessageDialogIconImage(MessageDialog::Icon icon);
			virtual Vector getMenuItemSize(Menu::Item * item);
			virtual void getMenuPopupBorderSizes(MenuPopup * menuPopup,
				Vector & topLeftSize, Vector & bottomRightSize);
			virtual Image * getPathTypeImage(Path::Type type);
			virtual Vector getRadioBoxSize(RadioBox * radioBox);
			virtual int getSelectorHeight(Selector * selector);
			virtual PointerType * getSelectPointerType();
			virtual void getSingleFieldPopupBorderSizes(SingleFieldPopup * popup,
				Vector & topLeftSize, Vector & bottomRightSize);
			virtual int getSliderWidgetWidth(Slider * slider);
			virtual int getSplitterSize(SplittingArea * area);
			virtual void getTabbingAreaFrameSizes(TabbingArea * area,
				Vector & topLeftSize, Vector & bottomRightSize);
			virtual Vector getTextFieldContentSize(TextField * field);
			virtual int getTextFieldCursorWidth(TextField * field);
			virtual void getTextFieldRowAndColumnAtPosition(TextField * field,
				int & row, int & column, Vector position);
			virtual int getTreeFieldElementSpaceWidth(TreeField::Item * item);
			virtual Vector getTreeFieldExpansionWidgetSize(TreeField::Item * item);
			virtual int getTreeFieldItemSpaceHeight(TreeField * field);
			virtual void getWindowFrameSizes(Window * window,
				Vector & topLeftSize, Vector & bottomRightSize);
			virtual unsigned long getWindowModulationColor(Window * window);
			virtual void initialize();
			virtual void setupBalloonPopup(BalloonPopup * popup);
			virtual void setupClientArea(ClientArea * area);
			virtual void setupField(Field * field);
			virtual void setupKeyStrokeField(KeyStrokeField * field);
			virtual void setupListField(ListField * field);
			virtual void setupProgressBar(ProgressBar * progressBar);
			virtual void setupScrollingArea(ScrollingArea * area);
			virtual void setupScrollingField(ScrollingField * field);
			virtual void setupSelector(Selector * selector);
			virtual void setupSingleFieldArea(SingleFieldArea * area);
			virtual void setupSlider(Slider * slider);
			virtual void setupSplittingArea(SplittingArea * area);
			virtual void setupTabbingArea(TabbingArea * area);
			virtual void setupTextElementEditPopup(Popup * popup, TextField * textField,
				TextElement * element, Vector elementPosition, Vector elementSize);
			virtual void setupTextField(TextField * field);
			virtual void setupTreeField(TreeField * field);
			virtual void setupWindow(Window * window);
			virtual void update();
			virtual void updateProgressBarCandyMode(ProgressBar * progressBar);
	};
}


#endif
