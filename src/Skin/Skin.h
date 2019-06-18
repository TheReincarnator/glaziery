/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SKIN_H
#define __GLAZIERY_SKIN_H


namespace glaziery
{
	/**
	 * A skin defines the graphical representation (colors, images, fonts,
	 * layout, etc.) and some of the user input behavior of the whole Glaziery
	 * system and all its components (windows, client areas, fields, ...).<br>
	 *
	 * Skins define the default sizes of components and add listeners
	 * to the components to add behavior. The glaziery components only
	 * contain representation-less states such as the size or whether
	 * the component is visible. The drawing is performed by the skin,
	 * using the platform adapter selected on desktop initialization.
	 * Application developers have to choose a skin before starting to
	 * design forms (field area dialog windows), because the sizes of
	 * components differ from skin to skin.<br>
	 *
	 * Most pointer event handling on components is defined by the skin
	 * using widgets. A widget (window gadget) is a small part of a component,
	 * an interactive graphical object, with skin-defined pointer behavior.
	 * Since widgets are event targets, they general pointer handling applies.
	 * Examples for widgets are the window title bar, the window close widget,
	 * and the selector pull-down widget. Skins create widgets in their setup
	 * methods and add them to the components.<br>
	 *
	 * A skin is selected when initializing the Glaziery system using
	 * Desktop::createInstance(). The skin cannot be changes after that point.
	 * Glaziery provides a reference skin, the SimpleSkin, and it encourages
	 * you to implement your own one.
	 *
	 * @see Desktop
	 * @see Desktop::createInstance()
	 * @see Component
	 * @see Widget
	 * @see SimpleSkin
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class Skin : public CObject, public Configurable
	#else
		class Skin : public Configurable
	#endif
	{
		friend Desktop;

		protected:

			/**
			 * Destroys the skin.
			 */
			virtual ~Skin();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Skin);
			#endif

		public:

			/**
			 * Creates a new effect for visualizating a click in a tutorial.
			 */
			virtual void addClickVisualizationEffect();

			/**
			 * Creates a new effect for visualizating a key press in a tutorial.
			 * @param keyName A string containing the character or key name.
			 * @param atFocus Whether to show the effect at the focus window or popup.
			 * false shows it at the pointer position. Defaults to true.
			 */
			virtual void addKeyVisualizationEffect(const String & keyName, bool atFocus = true);

			/**
			 * Creates a new effect for creating a popup and adds it to the Desktop.
			 * The effect does nothing but visual effects.
			 * If this skin does not emit effects, the method does nothing.
			 * @param popup The popup to be created visually.
			 */
			virtual void addPopupCreateEffect(Popup * popup) = NULL;

			/**
			 * Creates a new effect for destroying a popup and adds it to the Desktop.
			 * The effect must at least destroy the popup.
			 * If this skin does not emit effects, the method must destroy the
			 * popup directly.
			 * @param popup The popup to be destroyed with or without effect.
			 * @see Component::destroy()
			 */
			virtual void addPopupDestroyEffect(Popup * popup) = NULL;

			/**
			 * Creates a new effect for creating a window and adds it to the Desktop.
			 * The effect does nothing but visual effects.
			 * If this skin does not emit effects, the method does nothing.
			 * @param window The window to be created visually.
			 */
			virtual void addWindowCreateEffect(Window * window) = NULL;

			/**
			 * Creates a new effect for destroying a window and adds it to the Desktop.
			 * The effect must at least destroy the window.
			 * If this skin does not emit effects, the method must destroy the
			 * window directly.
			 * @param window The window to be destroyed with or without effect.
			 * @see Component::destroy()
			 */
			virtual void addWindowDestroyEffect(Window * window) = NULL;

			/**
			 * Cancels any pending scrolling area scroll effects.
			 * @param area The scrolling area to cancel the effects of.
			 */
			virtual void cancelScrollingAreaScrollEffects(ScrollingArea * area);

			/**
			 * Cancels any pending scrolling area scroll effects.
			 * @param field The scrolling area to cancel the effects of.
			 */
			virtual void cancelScrollingFieldScrollEffects(ScrollingField * field);

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param Section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * Section, bool write);

			/**
			 * Draws a balloon popup. This method is called from the popup's drawContent() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param popup The popup to be drawn.
			 * @see BalloonPopup
			 */
			virtual void drawBalloonPopup(DrawingContext & context, BalloonPopup * popup) = NULL;

			/**
			 * Draws a button. This method is called from the button's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param button The button to be drawn.
			 * @see Button
			 */
			virtual void drawButton(DrawingContext & context, Button * button) = NULL;

			/**
			 * Draws a check box. This method is called from the check box's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param checkBox The check box to be drawn.
			 * @see CheckBox
			 */
			virtual void drawCheckBox(DrawingContext & context, CheckBox * checkBox) = NULL;

			/**
			 * Draws the border and background of a client area popup.
			 * This method is called from the client area popup's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param popup The client area popup to be drawn.
			 * @see ClientAreaPopup
			 */
			virtual void drawClientAreaPopup(DrawingContext & context, ClientAreaPopup * popup);

			/**
			 * Draws the desktop background area (if applicable to this skin).
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @see Desktop
			 */
			virtual void drawDesktop(DrawingContext & context) = NULL;

			/**
			 * Draws the client area background of a fields area.
			 * This method is called from the fields area's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param area The fields area to be drawn.
			 * @see FieldsArea
			 */
			virtual void drawFieldsArea(DrawingContext & context, FieldsArea * area) = NULL;

			/**
			 * Draws a group field. This method is called from the group's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param groupField The group field to be drawn.
			 * @see ProgressBar
			 */
			virtual void drawGroupField(DrawingContext & context,
				GroupField * groupField) = NULL;

			/**
			 * Draws an image field element. This method is called from the field element's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param element The image element to be drawn.
			 * @see ImageElement
			 */
			virtual void drawImageElement(DrawingContext & context, ImageElement * element) = NULL;

			/**
			 * Draws an image field. This method is called from the field's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param field The image field to be drawn.
			 * @see ImageField
			 */
			virtual void drawImageField(DrawingContext & context, ImageField * field) = NULL;

			/**
			 * Draws a key stroke field. This method is called from the field's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param field The key stroke field to be drawn.
			 * @see KeyStrokeField
			 */
			virtual void drawKeyStrokeField(DrawingContext & context,
				KeyStrokeField * field) = NULL;

			/**
			 * Draws a label. This method is called from the label's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param label The label to be drawn.
			 * @see Label
			 */
			virtual void drawLabel(DrawingContext & context, Label * label) = NULL;

			/**
			 * Draws a list field background, border, and heading.
			 * Does not draw the rows field or rows themselves.
			 * This method is called from the field's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param field The list field to be drawn.
			 * @see ListField
			 */
			virtual void drawListField(DrawingContext & context, ListField * field) = NULL;

			/**
			 * Draws a list field's rows field background.
			 * Does not draw the rows themselves.
			 * This method is called from the field's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param field The list field's rows field to be drawn.
			 * @see ListField
			 */
			virtual void drawListFieldRowsField(DrawingContext & context,
				ListField::RowsField * field) = NULL;

			/**
			 * Draws a menu popup. This method is called from the menu popup's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param menuPopup The menu popup to be drawn.
			 * @see MenuPopup
			 */
			virtual void drawMenuPopup(DrawingContext & context, MenuPopup * menuPopup) = NULL;

			/**
			 * Draws a progress bar. This method is called from the bar's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param progressBar The progress bar to be drawn.
			 * @see ProgressBar
			 */
			virtual void drawProgressBar(DrawingContext & context,
				ProgressBar * progressBar) = NULL;

			/**
			 * Draws a radio box. This method is called from the radio box's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param radioBox The radio box to be drawn.
			 * @see RadioBox
			 */
			virtual void drawRadioBox(DrawingContext & context, RadioBox * radioBox) = NULL;

			/**
			 * Draws the scrolling widgets of a scrolling area.
			 * This method is called from the scrolling area's drawContent() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param area The scrolling area to be drawn.
			 * @see ScrollingArea
			 */
			virtual void drawScrollingArea(DrawingContext & context,
				ScrollingArea * area) = NULL;

			/**
			 * Draws the scrolling widgets of a scrolling field.
			 * This method is called from the scrolling field's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param field The scrolling field to be drawn.
			 * @see ScrollingField
			 */
			virtual void drawScrollingField(DrawingContext & context,
				ScrollingField * field) = NULL;

			/**
			 * Draws a selector. This method is called from the selector's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param selector The selector to be drawn.
			 * @see Selector
			 */
			virtual void drawSelector(DrawingContext & context, Selector * selector) = NULL;

			/**
			 * Draws the client area background of a single field area.
			 * This method is called from the list area's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param area The single field area to be drawn.
			 * @see SingleFieldArea
			 */
			virtual void drawSingleFieldArea(DrawingContext & context, SingleFieldArea * area) = NULL;

			/**
			 * Draws the border and background of a single field popup.
			 * This method is called from the single field popup's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param popup The single field popup to be drawn.
			 * @see SingleFieldPopup
			 */
			virtual void drawSingleFieldPopup(DrawingContext & context, SingleFieldPopup * popup) = NULL;

			/**
			 * Draws a slider field. This method is called from the slider's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param slider The slider field to be drawn.
			 * @see Slider
			 */
			virtual void drawSlider(DrawingContext & context, Slider * slider) = NULL;

			/**
			 * Draws the splash text to show credits about this skin and about Glaziery.
			 * This drawing method is called last, after all the other ones.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 */
			virtual void drawSplash(DrawingContext & context) = NULL;

			/**
			 * Draws the client area background of a splitting area.
			 * This method is called from the splitting area's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param area The splitting area to be drawn.
			 * @see SplittingArea
			 */
			virtual void drawSplittingArea(DrawingContext & context,
				SplittingArea * area) = NULL;

			/**
			 * Draws the client area background and the tabs of a tabbing area.
			 * This method is called from the tabbing area's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param area The tabbing area to be drawn.
			 * @see TabbingArea
			 */
			virtual void drawTabbingArea(DrawingContext & context, TabbingArea * area) = NULL;

			/**
			 * Draws an text field element. This method is called from the field element's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param element The text element to be drawn.
			 * @see TextElement
			 */
			virtual void drawTextElement(DrawingContext & context, TextElement * element) = NULL;

			/**
			 * Draws a text field. This method is called from the field's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param field The text field to be drawn.
			 * @see TextField
			 */
			virtual void drawTextField(DrawingContext & context, TextField * field) = NULL;

			/**
			 * Draws a tree field. This method is called from the field's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param field The tree field to be drawn.
			 * @see TreeField
			 */
			virtual void drawTreeField(DrawingContext & context, TreeField * field) = NULL;

			/**
			 * Draws a window after the client area has drawn, called from the window's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param window The window to be drawn.
			 * @see Window
			 */
			virtual void drawWindowAfterClientArea(DrawingContext & context, Window * window) = NULL;

			/**
			 * Draws a window before the client area has drawn, called from the window's draw() method.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param window The window to be drawn.
			 * @see Window
			 */
			virtual void drawWindowBeforeClientArea(DrawingContext & context, Window * window) = NULL;

			/**
			 * Fills a given scroller info according to the properties of a given scrolling area.
			 * Defaults to the default fill method of the ScrollerInfo.
			 * @param info The scroller info.
			 * @param area The scrolling area.
			 */
			virtual void fillScrollerInfo(ScrollerInfo & info, ScrollingArea * area);

			/**
			 * Fills a given scroller info according to the properties of a given scrolling field.
			 * Defaults to the default fill method of the ScrollerInfo.
			 * @param info The scroller info.
			 * @param field The scrolling field.
			 */
			virtual void fillScrollerInfo(ScrollerInfo & info, ScrollingField * field);

			/**
			 * Returns the size of the border around the message of a balloon popup,
			 * including the pointing arrow.
			 * @param popup The popup to return the border size of.
			 * @return The border size.
			 */
			virtual Vector getBalloonBorderSize(BalloonPopup * popup) = NULL;

			/**
			 * Returns the X offset of a balloon popup's pointing arrow from the nearer side.
			 * @param popup The popup to return the arrow offset.
			 * @return The arrow offset.
			 */
			virtual int getBalloonArrowOffset(BalloonPopup * popup) = NULL;

			/**
			 * Returns the size of a button to readjust it on image and/or text changes.
			 * @param button The button to return the size of.
			 * @return The size of the button.
			 */
			virtual Vector getButtonSize(Button * button) = NULL;

			/**
			 * Returns the size of a check box to readjust it on text changes.
			 * @param checkBox The check box to return the size of.
			 * @return The size of the check box.
			 */
			virtual Vector getCheckBoxSize(CheckBox * checkBox) = NULL;

			/**
			 * Returns the default pointer type to be used when dragging.
			 * The default is used if the dragged component does not define
			 * another pointer type.
			 * @return the default dragging pointer type.
			 */
			virtual PointerType * getDefaultDraggingPointerType() = NULL;

			/**
			 * Returns the default font for fields.
			 * @return the default font for fields.
			 */
			virtual Font * getDefaultFieldFont() = NULL;

			/**
			 * Returns the default pointer type to be used when an operation
			 * is impossible, such as an invalid dropping.
			 * The default is used if the dragged component does not define
			 * another pointer type.
			 * @return the default impossible pointer type.
			 */
			virtual PointerType * getDefaultImpossiblePointerType() = NULL;

			/**
			 * Fills integers with the dialog margins and spacings.
			 * @param dialog The dialog to return the margins and spacings for.
			 * @param bottomContentMargin The bottom margin of the content group field.
			 * @param buttonSpacing The space between adjacent buttons.
			 * @param iconContentSpacing The space between the message dialog icon
			 * and the content group field.
			 * @return The horizontal dialog button margin.
			 */
			virtual void getDialogMargins(Dialog * dialog, int & bottomContentMargin,
				int & buttonSpacing, int & iconContentSpacing) = NULL;

			/**
			 * Fills two vectors with the widths and heights of the top-left
			 * and bottom-right margins of a group field (considering all its properties).
			 * The sizes are consulted to calculate the remaining content size.
			 * @param field The group field to return the border sizes of.
			 * @param topLeftSize The vector to be filled with the top and left sizes.
			 * @param bottomRightSize The vector to be filled with the bottom and right sizes.
			 */
			virtual void getGroupFieldMarginSizes(GroupField * field,
				Vector & topLeftSize, Vector & bottomRightSize) = NULL;

			/**
			 * Returns the pointer type for the hour glass (waiting for an operation).
			 * @return the hour glass pointer type.
			 */
			virtual PointerType * getHourGlassPointerType() = NULL;

			/**
			 * Returns the width of the border between two columns of a list field.
			 * The column width does not include this amount.
			 * @param field The list field to return the border width of.
			 * @return The width of the border.
			 */
			virtual int getListFieldColumnBorderWidth(ListField * field) = NULL;

			/**
			 * Returns the width of the padding between the elements of a list field's cell
			 * and the column border. The column width includes this amount twice.
			 * @param field The list field to return the padding width of.
			 * @return The width of the padding.
			 */
			virtual int getListFieldColumnPaddingWidth(ListField * field) = NULL;

			/**
			 * Returns the height of the heading row of a list field.
			 * @param field The list field to return the heading height of.
			 * @return The height of the heading row.
			 */
			virtual int getListFieldHeadingHeight(ListField * field) = NULL;

			/**
			 * Returns the height of one row of a list field.
			 * @param field The list field to return the row height of.
			 * @return The height of one row.
			 */
			virtual int getListFieldRowHeight(ListField * field) = NULL;

			/**
			 * Returns the size of a menu item.
			 * @param item The item to return the size of.
			 * @return The size.
			 */
			virtual Vector getMenuItemSize(Menu::Item * item) = NULL;

			/**
			 * Fills two vectors with the widths and heights of the top-left
			 * and bottom-right borders of a menu popup.
			 * This size is consulted to calculate the popup size.
			 * @param menuPopup The menu popup to return the border sizes of.
			 * @param topLeftSize The vector to be filled
			 * with the top and left sizes.
			 * @param bottomRightSize The vector to be filled
			 * with the bottom and right sizes.
			 */
			virtual void getMenuPopupBorderSizes(MenuPopup * menuPopup,
				Vector & topLeftSize, Vector & bottomRightSize) = NULL;

			/**
			 * Returns a message dialog icon for a given identifier.
			 * @param icon The icon identifier.
			 * @return The icon image.
			 */
			virtual Image * getMessageDialogIconImage(MessageDialog::Icon icon) = NULL;

			/**
			 * Returns the icon image used for resources of a given type,
			 * such as folders, files, drives, etc.
			 * @param type The type of resource to return the image for.
			 * @return The icon image.
			 */
			virtual Image * getPathTypeImage(Path::Type type) = NULL;

			/**
			 * Returns the size of a radio box to readjust it on text changes.
			 * @param radioBox The radio box to return the size of.
			 * @return The size of the radio box.
			 */
			virtual Vector getRadioBoxSize(RadioBox * radioBox) = NULL;

			/**
			 * Returns the scroll position of a scrolling area, e.g. by getting the
			 * position directly using ScrollingArea::getScrollPositionNow(), or e.g.
			 * by using an effect that sets the scroll position.
			 * @param area The scrolling field to get the scroll position of.
			 * @return The scroll position.
			 */
         virtual Vector getScrollingAreaScrollPosition(ScrollingArea * area);

			/**
			 * Returns the scroll position of a scrolling field, e.g. by getting the
			 * position directly using ScrollingField::getScrollPositionNow(), or e.g.
			 * by using an effect that sets the scroll position.
			 * @param field The scrolling field to get the scroll position of.
			 * @return The scroll position.
			 */
         virtual Vector getScrollingFieldScrollPosition(ScrollingField * field);

			/**
			 * Returns the height of a selector to readjust it on image and/or text changes.
			 * @param selector The selector to return the height of.
			 * @return The height of the selector.
			 */
			virtual int getSelectorHeight(Selector * selector) = NULL;

			/**
			 * Returns the pointer type for the select pointer.
			 * @return the select pointer type.
			 */
			virtual PointerType * getSelectPointerType() = NULL;

			/**
			 * Fills two vectors with the widths and heights of the top-left
			 * and bottom-right borders of a single field popup.
			 * The sizes are consulted to calculate the remaining content size.
			 * @param popup The single field popup to return the border sizes of.
			 * @param topLeftSize The vector to be filled
			 * with the top and left sizes.
			 * @param bottomRightSize The vector to be filled
			 * with the bottom and right sizes.
			 */
			virtual void getSingleFieldPopupBorderSizes(SingleFieldPopup * popup,
				Vector & topLeftSize, Vector & bottomRightSize) = NULL;

			/**
			 * Returns the size (width resp. height) of the splitter of a splitter area.
			 * @param area The splitting area to return the size of.
			 * @return The size of the splitter of a splitter area.
			 */
			virtual int getSplitterSize(SplittingArea * area) = NULL;

			/**
			 * Fills two vectors with the widths and heights of the top-left
			 * and bottom-right borders of a tabbing area's frame.
			 * The frame contains the tabs themselves, and all the additional
			 * space that reduces the size of the sub client areas (panes).
			 * This size is consulted e.g. to calculate the client area size.
			 * @param area The tabbing area to return the frame sizes of.
			 * @param topLeftSize The vector to be filled
			 * with the top and left sizes.
			 * @param bottomRightSize The vector to be filled
			 * with the bottom and right sizes.
			 */
			virtual void getTabbingAreaFrameSizes(TabbingArea * area,
				Vector & topLeftSize, Vector & bottomRightSize) = NULL;

			/**
			 * Returns the size of the scrolling field content
			 * of a text field. This is the space the field requires
			 * if it was not scrolling and displaying its entire content.
			 * @param field The text field to return content size of.
			 * @return The size of the text field content.
			 */
			virtual Vector getTextFieldContentSize(TextField * field) = NULL;

			/**
			 * Returns the cursor width of a text field.
			 * @param field The text field to cursor widht of.
			 * @return The width of the cursor.
			 */
			virtual int getTextFieldCursorWidth(TextField * field) = NULL;

			/**
			 * Sets the row and column of a given pixel position into given variables.
			 * @param row The row variable to be set.
			 * @param column The column variable to be set.
			 * @param position The position to set the row and column of.
			 */
			virtual void getTextFieldRowAndColumnAtPosition(TextField * field,
				int & row, int & column, Vector position) = NULL;

			/**
			 * Returns the width of the space between the elements of a tree field item.
			 * @param item The tree field item.
			 * @return The width of the item elements space.
			 */
			virtual int getTreeFieldElementSpaceWidth(TreeField::Item * item) = NULL;

			/**
			 * Returns the size of the tree field expansion widget of a tree field item.
			 * @param item The tree field item.
			 * @return The size of the tree field expansion widget.
			 */
			virtual Vector getTreeFieldExpansionWidgetSize(TreeField::Item * item) = NULL;

			/**
			 * Returns the height of the space between items of a tree field.
			 * @param field The tree field.
			 * @return The height of the items space.
			 */
			virtual int getTreeFieldItemSpaceHeight(TreeField * field) = NULL;

			/**
			 * Fills two vectors with the widths and heights of the top-left
			 * and bottom-right borders of a window's frame.
			 * This size is consulted to calculate the client area size.
			 * @param window The window to return the frame sizes of.
			 * @param topLeftSize The vector to be filled
			 * with the top and left sizes.
			 * @param bottomRightSize The vector to be filled
			 * with the bottom and right sizes.
			 */
			virtual void getWindowFrameSizes(Window * window,
				Vector & topLeftSize, Vector & bottomRightSize) = NULL;

			/**
			 * Returns the modulation color used when actually rendering a given window.
			 * Defaults to white with the alpha value returned by the window's getAlpha().
			 * @return The modulation color for rendering.
			 */
			virtual unsigned long getWindowModulationColor(Window * window);

			/**
			 * Initializes the skin. This method is called after the skin and graphics
			 * adapter have been set, and after the platform adapter has been initialized.
			 * The method is called by the desktop, so do not call it manually.
			 * @note Perform all initialization operations here that require an initialized
			 * platform adapter (such as image or font retrieval).
			 * @see PlatformAdapter
			 * @see Desktop::createInstance()
			 */
			virtual void initialize();

			/**
			 * Sets the scroll position of a scrolling area, e.g. by setting the
			 * position directly using ScrollingArea::scrollToNow(), or e.g.
			 * by creating an effect that sets the scroll position.
			 * @param area The scrolling area to set the scroll position of.
			 * @param scrollPosition The scroll position.
			 */
         virtual void setScrollingAreaScrollPosition(ScrollingArea * area, Vector scrollPosition);

			/**
			 * Sets the scroll position of a scrolling field, e.g. by setting the
			 * position directly using ScrollingField::scrollToNow(), or e.g.
			 * by creating an effect that sets the scroll position.
			 * @param field The scrolling field to set the scroll position of.
			 * @param scrollPosition The scroll position.
			 */
         virtual void setScrollingFieldScrollPosition(ScrollingField * field, Vector scrollPosition);

			/**
			 * Performs skin specific initialization operations for a balloon popup.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the balloon popup is created.
			 * @param popup The balloon popup to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupBalloonPopup(BalloonPopup * popup);

			/**
			 * Performs skin specific initialization operations for a button.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the button is created.
			 * @param button The button to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupButton(Button * button);

			/**
			 * Performs skin specific initialization operations for a check box.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the check box is created.
			 * @param checkBox The check box to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupCheckBox(CheckBox * checkBox);

			/**
			 * Performs skin specific initialization operations for any client area.
			 * For example, add pointer behavior widgets and/or listeners or set the background color here.
			 * This method is called when any client area is created.
			 * @param area The client area to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupClientArea(ClientArea * area);

			/**
			 * Performs skin specific initialization operations for a client area popup.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the client area popup is created.
			 * @param popup The client area popup to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupClientAreaPopup(ClientAreaPopup * popup);

			/**
			 * Performs skin specific initialization operations for the desktop.
			 */
			virtual void setupDesktop();

			/**
			 * Performs skin specific initialization operations for a dialog.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the dialog is created.
			 * @param field The field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupDialog(Dialog * dialog);

			/**
			 * Performs skin specific initialization operations for a field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the field is created.
			 * @param field The field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupField(Field * field);

			/**
			 * Performs skin specific initialization operations for a fields area.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the fields area is created.
			 * @param area The fields area to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupFieldsArea(FieldsArea * area);

			/**
			 * Performs skin specific initialization operations for a group field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the group is created.
			 * @param groupField The group field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupGroupField(GroupField * groupField);

			/**
			 * Performs skin specific initialization operations for an image field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the image field is created.
			 * @param field The image field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupImageField(ImageField * field);

			/**
			 * Performs skin specific initialization operations for a key stroke field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the key stroke field is created.
			 * @param field The key stroke field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupKeyStrokeField(KeyStrokeField * field);

			/**
			 * Performs skin specific initialization operations for a label.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the label is created.
			 * @param label The label to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupLabel(Label * label);

			/**
			 * Performs skin specific initialization operations for a list field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the list field is created.
			 * @param field The list field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupListField(ListField * field);

			/**
			 * Performs skin specific initialization operations for a menu popup.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the menu popup is created.
			 * @param menuPopup The menu popup to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupMenuPopup(MenuPopup * menuPopup);

			/**
			 * Performs skin specific initialization operations for a modal window.
			 * This method is called when the modal window is created.
			 * @param window The modal window to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupModalWindow(ModalWindow * window);

			/**
			 * Performs skin specific initialization operations for a progress bar.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the bar is created.
			 * @param progressBar The progress bar to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupProgressBar(ProgressBar * progressBar);

			/**
			 * Performs skin specific initialization operations for a radio box.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the radio box is created.
			 * @param radioBox The radio box to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupRadioBox(RadioBox * radioBox);

			/**
			 * Performs skin specific initialization operations for a scrolling area.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the scrolling area is created.
			 * @param area The scrolling area to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupScrollingArea(ScrollingArea * area);

			/**
			 * Performs skin specific initialization operations for a scrolling field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the scrolling field is created.
			 * @param field The scrolling field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupScrollingField(ScrollingField * field);

			/**
			 * Performs skin specific initialization operations for a selector.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the selector is created.
			 * @param selector The selector to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupSelector(Selector * selector);

			/**
			 * Performs skin specific initialization operations for a selector popup.
			 * @param selector The selector to setup the popup of.
			 * @param popup The selector popup to be setup.
			 */
			virtual void setupSelectorPopup(Selector * selector, SingleFieldPopup * popup);

			/**
			 * Performs skin specific initialization operations for a single field area.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the single field area is created.
			 * @param area The single field area to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupSingleFieldArea(SingleFieldArea * area);

			/**
			 * Performs skin specific initialization operations for a single field popup.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the single field popup is created.
			 * @param popup The single field popup to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupSingleFieldPopup(SingleFieldPopup * popup);

			/**
			 * Performs skin specific initialization operations for a slider field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the slider is created.
			 * @param slider The slider field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupSlider(Slider * slider);

			/**
			 * Performs skin specific initialization operations for a splitting area.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the splitting area is created.
			 * @param area The splitting area to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupSplittingArea(SplittingArea * area);

			/**
			 * Performs skin specific initialization operations for a tabbing area.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the tabbing area is created.
			 * @param area The tabbing area to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupTabbingArea(TabbingArea * area);

			/**
			 * Sets up the edit popup and text field for editing a text element.
			 * This includes positioning and sizing.
			 * @param popup The edit popup.
			 * @param textField The edit text field.
			 * @param element The text element being edited.
			 * @param position The absolute position of the text element.
			 * @param size The size of the text element.
			 */
			virtual void setupTextElementEditPopup(Popup * popup, TextField * textField,
				TextElement * element, Vector elementPosition, Vector elementSize);

			/**
			 * Performs skin specific initialization operations for a text field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the text field is created.
			 * @param field The text field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupTextField(TextField * field);

			/**
			 * Performs skin specific initialization operations for a tree field.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the tree field is created.
			 * @param field The tree field to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupTreeField(TreeField * field);

			/**
			 * Performs skin specific initialization operations for a window.
			 * For example, add pointer behavior widgets and/or listeners here.
			 * This method is called when the window is created.
			 * @param window The window to be setup.
			 * @see Widget
			 * @see Component::Listener
			 */
			virtual void setupWindow(Window * window);

			/**
			 * Shuts down the skin. This method is called before the platform adapter
			 * has been shut down, so you can still use its methods.
			 * The method is called by the desktop, so do not call it manually.
			 * @see PlatformAdapter
			 * @see Desktop::destroyInstance()
			 */
			virtual void shutDown();

			/**
			 * Perform regular skin operations.
			 */
			virtual void update();

			/**
			 * Updates a progress bar field (sets and/or cancels effects etc.),
			 * if its candy mode changes.
			 * @param progressBar The progress bar whose candy mode changed.
			 */
			virtual void updateProgressBarCandyMode(ProgressBar * progressBar) = NULL;
	};
}


#endif
