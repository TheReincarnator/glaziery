/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int SimpleSkin::BLUE = GL_RGBA(40, 115, 155, 255);

const int SimpleSkin::DARK_BLUE = GL_RGBA(20, 57, 116, 255);

const int SimpleSkin::DEEP_DARK_BLUE = GL_RGBA(0, 0, 77, 255);

const char SimpleSkin::MASK_CHARACTER = '*';

const int SimpleSkin::LIGHT_BLUE = GL_RGBA(226, 226, 249, 255);

SimpleSkin::SimpleSkin()
{
	ASSERTION_COBJECT(this);

	cdromImage = NULL;
	checkBoxCheckedImage = NULL;
	checkBoxTriStateImage = NULL;
	checkBoxUncheckedImage = NULL;
	closeWidgetImage = NULL;
	downWidgetImage = NULL;
	draggingPointerType = NULL;
	errorIconImage = NULL;
	expandWidgetImage = NULL;
	fieldFont = NULL;
	fileImage = NULL;
	floppyDiskImage = NULL;
	focusableFactor = 250;
	folderImage = NULL;
	hardDiskImage = NULL;
	hourGlassPointerType = NULL;
	impossiblePointerType = NULL;
	informationIconImage = NULL;
	leftWidgetImage = NULL;
	maximizeWidgetImage = NULL;
	memoryStickImage = NULL;
	menuCheckImage = NULL;
	menuRadioImage = NULL;
	collapseWidgetImage = NULL;
	networkComputerImage = NULL;
	networkDriveImage = NULL;
	questionIconImage = NULL;
	radioBoxCheckedImage = NULL;
	radioBoxUncheckedImage = NULL;
	ramDiskImage = NULL;
	resize0PointerType = NULL;
	resize135PointerType = NULL;
	resize45PointerType = NULL;
	resize90PointerType = NULL;
	rightWidgetImage = NULL;
	rootImage = NULL;
	selectPointerType = NULL;
	unknownImage = NULL;
	upWidgetImage = NULL;
	virtualImage = NULL;
	warningIconImage = NULL;
	windowTitleFont = NULL;
}

SimpleSkin::~SimpleSkin()
{
	ASSERTION_COBJECT(this);

	if (draggingPointerType != NULL)
		{delete draggingPointerType; draggingPointerType = NULL;}
	if (hourGlassPointerType != NULL)
		{delete hourGlassPointerType; hourGlassPointerType = NULL;}
	if (impossiblePointerType != NULL)
		{delete impossiblePointerType; impossiblePointerType = NULL;}
	if (resize0PointerType != NULL)
		{delete resize0PointerType; resize0PointerType = NULL;}
	if (resize45PointerType != NULL)
		{delete resize45PointerType; resize45PointerType = NULL;}
	if (resize90PointerType != NULL)
		{delete resize90PointerType; resize90PointerType = NULL;}
	if (resize135PointerType != NULL)
		{delete resize135PointerType; resize135PointerType = NULL;}
	if (selectPointerType != NULL)
		{delete selectPointerType; selectPointerType = NULL;}
}

void SimpleSkin::addPopupCreateEffect(Popup * popup)
{
	ASSERTION_COBJECT(this);

	CachableComponentEffect * effect;
	if ((effect = new CachableComponentEffect(150, popup)) == NULL)
		throw EOUTOFMEMORY;
	effect->setFocusAlphaStart(0);
	effect->setNonFocusAlphaStart(0);

	Desktop::getInstance()->addEffect(effect);
}

void SimpleSkin::addWindowCreateEffect(Window * window)
{
	ASSERTION_COBJECT(this);

	CachableComponentEffect * effect;
	if ((effect = new CachableComponentEffect(250, window)) == NULL)
		throw EOUTOFMEMORY;
	effect->setFocusAlphaStart(0);
	effect->setNonFocusAlphaStart(0);

	Desktop::getInstance()->addEffect(effect);
}

void SimpleSkin::addPopupDestroyEffect(Popup * popup)
{
	ASSERTION_COBJECT(this);

	CachableComponentEffect * effect;
	if ((effect = new CachableComponentEffect(150, popup)) == NULL)
		throw EOUTOFMEMORY;
	effect->setFocusAlphaEnd(0);
	effect->setNonFocusAlphaEnd(0);
	effect->setDestroying(true);

	Desktop::getInstance()->addEffect(effect);
}

void SimpleSkin::addWindowDestroyEffect(Window * window)
{
	ASSERTION_COBJECT(this);

	window->unfocus();

	CachableComponentEffect * effect;
	if ((effect = new CachableComponentEffect(250, window)) == NULL)
		throw EOUTOFMEMORY;
	effect->setFocusAlphaEnd(0);
	effect->setNonFocusAlphaEnd(0);
	effect->setDestroying(true);

	Desktop::getInstance()->addEffect(effect);
}

void SimpleSkin::drawBalloonPopup(DrawingContext & context, BalloonPopup * popup)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	unsigned long color = GL_COLOR_A(DARK_BLUE, 255);
	adapter->drawRect(context, Vector(), popup->getSize() - Vector(1, 1), color);

	adapter->drawText(context, popup->getFont(), Vector(8, 8), popup->getMessage(),
		getAlpha(popup), popup->isCaps());
}

void SimpleSkin::drawButton(DrawingContext & context, Button * button)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	adapter->draw3DRect(context, Vector(), button->getSize() - Vector(1, 1),
		GL_COLOR_A(BLUE, getAlpha(button)),
		GL_COLOR_A(LIGHT_BLUE, getAlpha(button)),
		GL_COLOR_A(DARK_BLUE, getAlpha(button)));

	Image * image = button->getImage();
	Vector textSize = button->getFont()->getSize(button->getText(), button->isCaps(), true);

	int xPosition;
	if (image != NULL)
		xPosition = (button->getSize().x - image->getSize().x - textSize.x) / 2;
	else
		xPosition = (button->getSize().x - textSize.x) / 2;

	if (image != NULL)
	{
		int yPosition = (button->getSize().y - image->getSize().y) / 2;
		adapter->drawImage(context, image, Vector(xPosition, yPosition),
			GL_WHITE_A(getAlpha(button)));
		xPosition += image->getSize().x;
	}

	int yPosition = (button->getSize().y - button->getFont()->getHeight()) / 2;
	adapter->drawText(context, button->getFont(), Vector(xPosition, yPosition), button->getText(),
		getAlpha(button), button->isCaps(), true);
}

void SimpleSkin::drawCheckBox(DrawingContext & context, CheckBox * checkBox)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	Image * checkBoxImage;
	if (checkBox->getState() == CheckBox::CHECKED)
		checkBoxImage = checkBoxCheckedImage;
	else if (checkBox->getState() == CheckBox::TRI_STATE)
		checkBoxImage = checkBoxTriStateImage;
	else
		checkBoxImage = checkBoxUncheckedImage;
	adapter->drawImage(context, checkBoxImage, Vector(0, 1), GL_WHITE_A(getAlpha(checkBox)));
	adapter->drawText(context, checkBox->getFont(), Vector(20, 0), checkBox->getText(),
		getAlpha(checkBox), checkBox->isCaps(), true);
}

void SimpleSkin::drawDesktop(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
}

void SimpleSkin::drawFieldsArea(DrawingContext & context, FieldsArea * area)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	unsigned long color = GL_COLOR_A(area->getBackgroundColor(), 255);
	adapter->drawRect(context, Vector(), area->getSize() - Vector(1, 1), color);
}

void SimpleSkin::drawGroupField(DrawingContext & context, GroupField * groupField)
{
	ASSERTION_COBJECT(this);

	if (!groupField->isBordered())
		return;

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	Font * font = groupField->getFont();

	int borderTop = groupField->getText().IsEmpty() ? 0 : font->getHeight() / 2;
	adapter->drawRect(context, Vector(1, borderTop + 1),
		groupField->getSize() - Vector(1, 1),
		GL_TRANSPARENCY, GL_COLOR_A(DARK_BLUE, getAlpha(groupField)));
	adapter->drawRect(context, Vector(0, borderTop),
		groupField->getSize() - Vector(2, 2),
		GL_TRANSPARENCY, GL_COLOR_A(BLUE, getAlpha(groupField)));

	adapter->drawText(context, font, Vector(8, 0), groupField->getText(),
		getAlpha(groupField), groupField->isCaps(), true);
}

void SimpleSkin::drawImageElement(DrawingContext & context, ImageElement * element)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	adapter->drawImage(context, element->getImage(), Vector(1, 1), GL_WHITE_A(getAlpha(element->getField())));
}

void SimpleSkin::drawImageField(DrawingContext & context, ImageField * field)
{
	ASSERTION_COBJECT(this);

	Image * image = field->getImage();
	if (image != NULL)
	{
		PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
		adapter->drawImage(context, image, Vector(), GL_WHITE_A(getAlpha(field)));
	}
}

void SimpleSkin::drawKeyStrokeField(DrawingContext & context, KeyStrokeField * field)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	adapter->drawRect(context, Vector(), field->getSize() - Vector(1, 1),
		GL_COLOR_A(BLUE, getAlpha(field)),
		GL_COLOR_A(LIGHT_BLUE, getAlpha(field)));

	int keyCode = field->getKeyCode();
	if (keyCode >= 0)
	{
		DrawingContext textContext(context, Vector(3, 3),
			context.getClippingSize() - Vector(6, 5));
		adapter->drawText(textContext, field->getFont(), Vector(),
			adapter->getKeyName(keyCode), getAlpha(field), field->isCaps());
	}
}

void SimpleSkin::drawLabel(DrawingContext & context, Label * label)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	adapter->drawText(context, label->getFont(), Vector(), label->getText(),
		getAlpha(label), label->isCaps(), true);
}

void SimpleSkin::drawListField(DrawingContext & context, ListField * field)
{
	ASSERTION_COBJECT(this);

	if (!field->isHeadlined())
		return;

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	int headingHeight = getListFieldHeadingHeight(field);
	int columnBorderWidth = getListFieldColumnBorderWidth(field);
	const ArrayList<ListField::Column> & columns = field->getColumns();
	ListField::RowsField * rowsField = field->getRowsField();

	Vector position(rowsField->getBorderSize().x + rowsField->getOuterPaddingSize().x, 0);
	for (int i=0; i<columns.GetCount(); i++)
	{
		ListField::Column * column = columns.Get(i);
		if (!column->isVisible())
			continue;

		int columnWidth = column->getWidth();
		adapter->draw3DRect(context, position,
			position + Vector(columnWidth, headingHeight) - Vector(1, 1),
			GL_COLOR_A(BLUE, getAlpha(field)), GL_COLOR_A(LIGHT_BLUE, getAlpha(field)),
			GL_COLOR_A(DARK_BLUE, getAlpha(field)));

		adapter->drawText(context, field->getFont(), position + Vector(3, 3),
			column->getHeading(), getAlpha(field), field->isCaps(), true);

		position.x += columnWidth + columnBorderWidth;
	}

	if (position.x < context.getClippingSize().x)
		adapter->draw3DRect(context, position,
			Vector(context.getClippingSize().x, position.y + headingHeight - 1),
			GL_COLOR_A(BLUE, getAlpha(field)), GL_COLOR_A(LIGHT_BLUE, getAlpha(field)),
			GL_COLOR_A(DARK_BLUE, getAlpha(field)));
}

void SimpleSkin::drawListFieldCell(DrawingContext & context, ListField::Cell * cell,
	Vector cellSize, bool focused, bool textElements)
{
	ASSERTION_COBJECT(this);

	int rowHeight = getListFieldRowHeight(cell->getField());

	if (focused)
	{
		PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
		adapter->drawRect(context, Vector(), cellSize - Vector(1, 1), GL_TRANSPARENCY,
			GL_COLOR_A(LIGHT_BLUE, getAlpha(cell->getRow()->getField())));
	}

	const ArrayList<FieldElement> & elements = cell->getElements();

	int x = 0;
	if (cell->getColumn()->getAlignment() != ListField::ALIGN_LEFT)
	{
		int elementsWidth = 0;
		for (int i=0; i<elements.GetCount(); i++)
		{
			FieldElement * element = elements.Get(i);
			elementsWidth += element->getSize().x;
		}

		if (cell->getColumn()->getAlignment() == ListField::ALIGN_CENTER
			|| cell->getColumn()->getAlignment() == ListField::ALIGN_CENTER_NO_PADDING)
			x = (cell->getColumn()->getWidth() - elementsWidth) / 2;
		else if (cell->getColumn()->getAlignment() == ListField::ALIGN_RIGHT)
			x = cell->getColumn()->getWidth() - elementsWidth;
	}

	FieldElement::drawElements(context, elements, cellSize.y, textElements, !textElements);
}

void SimpleSkin::drawListFieldRow(DrawingContext & context, ListField::Row * row, bool focused)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	ListField * field = row->getField();
	ListField::RowsField * rowsField = field->getRowsField();
	const ArrayList<ListField::Column> & columns = field->getColumns();
	if (columns.IsEmpty())
		return;

	bool selected = row->isSelected();
	const ArrayList<ListField::Cell> & cells = row->getCells();

	if (selected)
	{
		int rowWidth = field->getRowWidth();
		unsigned long color = GL_COLOR_A(BLUE, getAlpha(field));
		adapter->drawRect(context, Vector(), Vector(rowWidth - 1,
			context.getClippingSize().y - 1), color);
	}

	int columnBorderWidth = getListFieldColumnBorderWidth(field);
	int rowHeight = getListFieldRowHeight(field);

	// Draw text elements

	Vector position;
	int i;
	for (i=0; i<columns.GetCount() && i<cells.GetCount(); i++)
	{
		ListField::Column * column = columns.Get(i);
		if (!column->isVisible())
			continue;

		int columnWidth = column->getWidth();
		if (position.x + columnWidth >= rowsField->getScrollPositionNow().x
			&& position.x < rowsField->getScrollPositionNow().x + rowsField->getContentViewSize().x)
		{
			Vector cellSize = Vector(columnWidth, rowHeight);
			drawListFieldCell(DrawingContext(context, position, cellSize),
				cells.Get(i), cellSize, focused && field->getFocusColumnNo() == i, true);
		}

		position.x += columnWidth + columnBorderWidth;
	}

	// Draw image elements

	position = Vector();
	for (i=0; i<columns.GetCount() && i<cells.GetCount(); i++)
	{
		ListField::Column * column = columns.Get(i);
		if (!column->isVisible())
			continue;

		int columnWidth = column->getWidth();
		if (position.x + columnWidth >= rowsField->getScrollPositionNow().x
			&& position.x < rowsField->getScrollPositionNow().x + rowsField->getContentViewSize().x)
		{
			Vector cellSize = Vector(columnWidth, rowHeight);
			drawListFieldCell(DrawingContext(context, position, cellSize),
				cells.Get(i), cellSize, focused && field->getFocusColumnNo() == i, false);
		}

		position.x += columnWidth + columnBorderWidth;
	}
}

void SimpleSkin::drawListFieldRowsField(DrawingContext & context, ListField::RowsField * field)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	ListField * listField = field->getListField();
	const ArrayList<ListField::Row> & rows = listField->getRows();
	const ArrayList<ListField::Column> & columns = listField->getColumns();

	adapter->drawRect(context, Vector(), context.getClippingSize() - Vector(1, 1),
		GL_COLOR_A(DEEP_DARK_BLUE, getAlpha(field)));

	if (!field->isVisible() || rows.IsEmpty() || columns.IsEmpty())
		return;

	int rowHeight = getListFieldRowHeight(listField);

	int firstVisibleRow = field->getScrollPositionNow().y / rowHeight;
	if (firstVisibleRow < 0)
		firstVisibleRow = 0;
	int lastVisibleRow = (field->getScrollPositionNow().y + field->getContentViewSize().y) / rowHeight;
	if (lastVisibleRow > rows.GetCount() - 1)
		lastVisibleRow = rows.GetCount() - 1;

	for (int i=0; i<=lastVisibleRow; i++)
		drawListFieldRow(DrawingContext(context, Vector(0, rowHeight * i),
			Vector(listField->getRowWidth(), rowHeight)), rows.Get(i), i == listField->getFocusRowNo());
}

void SimpleSkin::drawMenuPopup(DrawingContext & context, MenuPopup * menuPopup)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();

	unsigned char alpha = getAlpha(menuPopup);
	adapter->draw3DRect(context, Vector(), menuPopup->getSize() - Vector(1, 1),
		GL_COLOR_A(DARK_BLUE, alpha), GL_COLOR_A(DEEP_DARK_BLUE, alpha),
		GL_COLOR_A(BLUE, alpha));

	Vector topLeftSize;
	Vector bottomRightSize;
	Menu * menu = menuPopup->getMenu();
	getMenuPopupBorderSizes(menuPopup, topLeftSize, bottomRightSize);

	Vector position = topLeftSize;
	const ArrayList<Menu::Item> & items = menu->getItems();
	for (int i=0; i<items.GetCount(); i++)
	{
		Menu::Item * item = items.Get(i);

		if (!item->isSeparator())
			adapter->drawText(context, fieldFont, position, item->getText(),
				item->isActive() ? alpha : alpha / 2, false, true);

		Vector itemSize = getMenuItemSize(item);
		if (menu->isHorizontal())
			position.x += itemSize.x;
		else
			position.y += itemSize.y;
	}
}

void SimpleSkin::drawProgressBar(DrawingContext & context, ProgressBar * progressBar)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();

	adapter->draw3DRect(context, Vector(), progressBar->getSize() - Vector(1, 1),
		GL_COLOR_A(DARK_BLUE, getAlpha(progressBar)),
		GL_COLOR_A(DEEP_DARK_BLUE, getAlpha(progressBar)),
		GL_COLOR_A(BLUE, getAlpha(progressBar)));

	if (progressBar->getEnd() - progressBar->getStart() > 0)
	{
		double percentage = (progressBar->getConstrainedCurrent() * 1.0
			- progressBar->getStart()) / (progressBar->getEnd() - progressBar->getStart());

		int currentX = (int) (percentage * (progressBar->getSize().x - 2));
		unsigned long red = (int) ((1.0 - percentage) * 160.5) + 95;
		unsigned long green = (int) (percentage * 160.5) + 95;
		unsigned long color = GL_RGBA(red, green, 64, getAlpha(progressBar));

		if (currentX > 0)
			adapter->drawRect(context, Vector(1, 1),
				Vector(currentX, progressBar->getSize().y - 2), color);
	}
}

void SimpleSkin::drawRadioBox(DrawingContext & context, RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	Image * radioBoxImage;
	if (radioBox->isChecked())
		radioBoxImage = radioBoxCheckedImage;
	else
		radioBoxImage = radioBoxUncheckedImage;
	adapter->drawImage(context, radioBoxImage, Vector(0, 2), GL_WHITE_A(getAlpha(radioBox)));
	adapter->drawText(context, radioBox->getFont(), Vector(20, 0), radioBox->getText(),
		getAlpha(radioBox), radioBox->isCaps(), true);
}

void SimpleSkin::drawScrollingArea(DrawingContext & context, ScrollingArea * area)
{
	ASSERTION_COBJECT(this);
	drawScrollingComponent(context, area);
}

void SimpleSkin::drawScrollingComponent(DrawingContext & context, Component * component)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	ScrollingField * field = dynamic_cast<ScrollingField *>(component);
	ScrollingArea * area = dynamic_cast<ScrollingArea *>(component);
	unsigned char alpha = getAlpha(component);

	// Calculate scrolling view sizes, scroller positions, and scrollability

	ScrollerInfo info;
	int scrollerBreadth = 16;
	bool horizontallyScrollable = false;
	bool verticallyScrollable = false;
	Skin * skin = Desktop::getInstance()->getSkin();
	if (field != NULL)
	{
		fillScrollerInfo(info, field);
		scrollerBreadth = field->getScrollerBreadth();
		horizontallyScrollable = field->isHorizontallyScrollable();
		verticallyScrollable = field->isVerticallyScrollable();
	}
	else if (area != NULL)
	{
		fillScrollerInfo(info, area);
		scrollerBreadth = area->getScrollerBreadth();
		horizontallyScrollable = area->isHorizontallyScrollable();
		verticallyScrollable = area->isVerticallyScrollable();
	}

	// Draw the background of the component

	bool isInFieldsArea = field != NULL && field->getFieldsArea() != NULL;
	unsigned long color = GL_COLOR_A(isInFieldsArea ? BLUE : DEEP_DARK_BLUE, alpha);
	adapter->drawRect(context, Vector(), component->getSize() - Vector(1, 1), color);

	// Draw the border around of the component, if any

	Vector borderSize;
	if (area != NULL)
		borderSize = area->getBorderSize();
	else if (field != NULL)
		borderSize = field->getBorderSize();

	int biggerBorderValue = borderSize.x > borderSize.y ? borderSize.x : borderSize.y;
	for (int i=0; i<biggerBorderValue; i++)
	{
		Vector borderLineOffset = Vector(i <= borderSize.x ? i : borderSize.x,
			i <= borderSize.y ? i : borderSize.y);
		adapter->draw3DRect(context, borderLineOffset, component->getSize() - Vector(1, 1) - borderLineOffset,
			GL_TRANSPARENCY, GL_COLOR_A(DARK_BLUE, alpha), GL_COLOR_A(LIGHT_BLUE, alpha));
	}

	// Draw scrollers

	if (horizontallyScrollable)
	{
		adapter->draw3DRect(context, Vector(info.horizontalStartX, info.horizontalStartY),
			Vector(info.horizontalStartX + scrollerBreadth - 1, info.horizontalEndY),
			GL_COLOR_A(BLUE, alpha), GL_COLOR_A(LIGHT_BLUE, alpha), GL_COLOR_A(DARK_BLUE, alpha));
		adapter->drawImage(context, leftWidgetImage,
			Vector(info.horizontalStartX + 1, info.horizontalStartY + 1), GL_WHITE_A(alpha));

		if (info.horizontalScrollAreaStartX <= info.horizontalHandleStartX - 1)
			adapter->drawRect(context, Vector(info.horizontalScrollAreaStartX, info.horizontalStartY),
				Vector(info.horizontalHandleStartX - 1, info.horizontalEndY),
				GL_COLOR_A(DARK_BLUE, alpha));
		adapter->draw3DRect(context, Vector(info.horizontalHandleStartX, info.horizontalStartY),
			Vector(info.horizontalHandleEndX, info.horizontalEndY),
			GL_COLOR_A(BLUE, alpha), GL_COLOR_A(LIGHT_BLUE, alpha), GL_COLOR_A(DARK_BLUE, alpha));
		if (info.horizontalHandleEndX + 1 <= info.horizontalScrollAreaEndX)
			adapter->drawRect(context, Vector(info.horizontalHandleEndX + 1, info.horizontalStartY),
				Vector(info.horizontalScrollAreaEndX, info.horizontalEndY),
				GL_COLOR_A(DARK_BLUE, alpha));

		adapter->draw3DRect(context, Vector(info.horizontalEndX - scrollerBreadth + 1, info.horizontalStartY),
			Vector(info.horizontalEndX, info.horizontalEndY),
			GL_COLOR_A(BLUE, alpha), GL_COLOR_A(LIGHT_BLUE, alpha), GL_COLOR_A(DARK_BLUE, alpha));
		adapter->drawImage(context, rightWidgetImage,
			Vector(info.horizontalEndX - scrollerBreadth + 2, info.horizontalStartY + 1),
			GL_WHITE_A(alpha));
	}

	if (verticallyScrollable)
	{
		adapter->draw3DRect(context, Vector(info.verticalStartX, info.verticalStartY),
			Vector(info.verticalEndX, info.verticalStartY + scrollerBreadth - 1),
			GL_COLOR_A(BLUE, alpha), GL_COLOR_A(LIGHT_BLUE, alpha), GL_COLOR_A(DARK_BLUE, alpha));
		adapter->drawImage(context, upWidgetImage,
			Vector(info.verticalStartX + 1, info.verticalStartY + 1), GL_WHITE_A(alpha));

		if (info.verticalScrollAreaStartY <= info.verticalHandleStartY - 1)
			adapter->drawRect(context, Vector(info.verticalStartX, info.verticalScrollAreaStartY),
				Vector(info.verticalEndX, info.verticalHandleStartY - 1),
				GL_COLOR_A(DARK_BLUE, alpha));
		adapter->draw3DRect(context, Vector(info.verticalStartX, info.verticalHandleStartY),
			Vector(info.verticalEndX, info.verticalHandleEndY),
			GL_COLOR_A(BLUE, alpha), GL_COLOR_A(LIGHT_BLUE, alpha), GL_COLOR_A(DARK_BLUE, alpha));
		if (info.verticalHandleEndY + 1 <= info.verticalScrollAreaEndY)
			adapter->drawRect(context, Vector(info.verticalStartX, info.verticalHandleEndY + 1),
				Vector(info.verticalEndX, info.verticalScrollAreaEndY),
				GL_COLOR_A(DARK_BLUE, alpha));

		adapter->draw3DRect(context, Vector(info.verticalStartX, info.verticalEndY - scrollerBreadth + 1),
			Vector(info.verticalEndX, info.verticalEndY),
			GL_COLOR_A(BLUE, alpha), GL_COLOR_A(LIGHT_BLUE, alpha), GL_COLOR_A(DARK_BLUE, alpha));
		adapter->drawImage(context, downWidgetImage,
			Vector(info.verticalStartX + 1, info.verticalEndY - scrollerBreadth + 2),
			GL_WHITE_A(alpha));
	}
}

void SimpleSkin::drawScrollingField(DrawingContext & context, ScrollingField * field)
{
	ASSERTION_COBJECT(this);
	drawScrollingComponent(context, field);
}

void SimpleSkin::drawSelector(DrawingContext & context, Selector * selector)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	unsigned char alpha = getAlpha(selector);

	adapter->drawRect(context, Vector(), selector->getSize() - Vector(1, 1),
		GL_COLOR_A(BLUE, alpha),
		GL_COLOR_A(LIGHT_BLUE, alpha));

	int widgetSize = selector->getSize().y - 2;
	adapter->draw3DRect(context, Vector(selector->getSize().x - 1 - widgetSize, 1),
		selector->getSize() - Vector(2, 2), GL_COLOR_A(BLUE, alpha),
		GL_COLOR_A(LIGHT_BLUE, alpha), GL_COLOR_A(DARK_BLUE, alpha));
	adapter->drawImage(context, downWidgetImage, Vector(selector->getSize().x - 1 - widgetSize, 1)
		+ (Vector(widgetSize, widgetSize) - downWidgetImage->getSize()) / 2, GL_WHITE_A(alpha));

	DrawingContext contentContext(context, Vector(2, 1), selector->getSize() - Vector(3 + widgetSize, 1));

	Selector::Option * option = selector->getSelection();
	if (option != NULL)
	{
		const ArrayList<FieldElement> & elements = option->getElements();
		FieldElement::drawElements(contentContext, elements, selector->getSize().y);
	}
}

void SimpleSkin::drawSingleFieldArea(DrawingContext & context, SingleFieldArea * area)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	adapter->clear(context, GL_COLOR_A(area->getBackgroundColor(), 255));
}

void SimpleSkin::drawSingleFieldPopup(DrawingContext & context, SingleFieldPopup * popup)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	adapter->clear(context, GL_TRANSPARENCY);
}

void SimpleSkin::drawSlider(DrawingContext & context, Slider * slider)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();
	int widgetWidth = getSliderWidgetWidth(slider);

	adapter->draw3DRect(context, Vector(widgetWidth / 2 - 1, slider->getSize().y / 2 - 2),
		Vector(slider->getSize().x - widgetWidth / 2, slider->getSize().y / 2 + 2),
		GL_COLOR_A(BLUE, getAlpha(slider)),
		GL_COLOR_A(DARK_BLUE, getAlpha(slider)),
		GL_COLOR_A(LIGHT_BLUE, getAlpha(slider)));

	if (slider->getEnd() - slider->getStart() > 0)
	{
		int currentX = (int) ((slider->getConstrainedCurrent() * 1.0 - slider->getStart())
			/ (slider->getEnd() - slider->getStart()) * (slider->getSize().x - widgetWidth + 1));
		adapter->draw3DRect(context, Vector(currentX, 0),
			Vector(currentX + widgetWidth - 1, slider->getSize().y - 1),
			GL_COLOR_A(BLUE, getAlpha(slider)),
			GL_COLOR_A(LIGHT_BLUE, getAlpha(slider)),
			GL_COLOR_A(DARK_BLUE, getAlpha(slider)));
	}
}

void SimpleSkin::drawSplash(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	unsigned long frameTime = desktop->getFrameTime();
	if (frameTime > 3000 && frameTime < 12000)
	{
		int alpha;
		if (frameTime > 3000 && frameTime < 3500)
			alpha = (int) ((frameTime - 3000) * 255.0 / 500.0);
		else if (frameTime > 10000 && frameTime < 12000)
			alpha = (int) ((12000 - frameTime) * 255.0 / 2000.0);
		else
			alpha = 255;

		String text(STRING_SIMPLESKIN_SPLASH);
		Vector textSize = fieldFont->getSize(text);

		PlatformAdapter * adapter = desktop->getPlatformAdapter();
		adapter->drawRect(context, desktop->getSize() - textSize - Vector(24, 16),
			desktop->getSize() - Vector(8, 8), GL_RGBA(0, 0, 0, alpha / 2), GL_RGBA(0, 0, 0, alpha / 2));
		adapter->drawText(context, fieldFont, desktop->getSize() - textSize - Vector(16, 12),
			text, alpha);
	}
}

void SimpleSkin::drawSplittingArea(DrawingContext & context, SplittingArea * area)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	unsigned long color = GL_COLOR_A(area->getBackgroundColor(), 255);
	adapter->drawRect(context, Vector(), area->getSize() - Vector(1, 1), color);

	const ArrayList<ClientArea> & panes = area->getPanes();
	int panesCount = panes.GetCount();

	if (area->getDirection() == SplittingArea::HORIZONTAL)
	{
		int x = 0;
		for (int i=0; i<panesCount; i++)
		{
			if (i > 0)
			{
				adapter->drawLine(context, Vector(x, 0), Vector(x, area->getSize().y - 1), BLUE);
				x++;
			}
			x += panes.Get(i)->getSize().x;
		}
	}
	else
	{
		int y = 0;
		for (int i=0; i<panesCount; i++)
		{
			if (i > 0)
			{
				adapter->drawLine(context, Vector(0, y), Vector(area->getSize().x - 1, y), BLUE);
				y++;
			}
			y += panes.Get(i)->getSize().y;
		}
	}
}

void SimpleSkin::drawTabbingArea(DrawingContext & context, TabbingArea * area)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	unsigned long color = GL_COLOR_A(area->getBackgroundColor(), 255);
	adapter->drawRect(context, Vector(), area->getSize() - Vector(1, 1), color);

	const ArrayList<ClientArea> & panes = area->getPanes();
	int panesCount = panes.GetCount();
	int x = 0;
	int y = 0;
	for (int i=0; i<panesCount; i++)
	{
		ClientArea * pane = panes.Get(i);
		if (!pane->isVisible())
			continue;

		const String & name = area->getPaneName(i);
		Vector tabTextSize = tabFont->getSize(name, false, true);
		Vector tabTextSizeWithBorder = Vector(8, 2) + tabTextSize + Vector(8, 3);

		if (x > 0 && area->getSize().x - x < tabTextSizeWithBorder.x)
		{
			y += tabTextSizeWithBorder.y;
			x = 0;
		}

		adapter->drawRect(context, Vector(x, y), Vector(x, y) + tabTextSizeWithBorder - Vector(1, 1),
			GL_COLOR_A(BLUE, getAlpha(pane)), GL_COLOR_A(LIGHT_BLUE, getAlpha(pane)));
		adapter->drawText(context, tabFont, Vector(x + 8, y + 2), name, getAlpha(pane),
			false, true);

		x += tabTextSizeWithBorder.x;
	}

	y += 2 + tabFont->getHeight() + 3;

	adapter->drawRect(context, Vector(0, y - 1), area->getSize() - Vector(1, 1),
		GL_TRANSPARENCY, GL_COLOR_A(LIGHT_BLUE, getAlpha(area)));
}

void SimpleSkin::drawTextElement(DrawingContext & context, TextElement * element)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	Field * field = element->getField();
	adapter->drawText(context, field->getFont(), Vector(2, 1), element->getText(),
		getAlpha(field), field->isCaps(), false, true);
}

void SimpleSkin::drawTextField(DrawingContext & context, TextField * field)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();

	Font * font = field->getFont();
	const ArrayList<String> & lines = field->getLines();
	int alpha = getAlpha(field);
	bool masked = field->isMasked();
	int maskWidth = font->getWidth(MASK_CHARACTER) + 1;
	bool caps = field->isCaps();

	bool hasFocus = field->hasFocus();
	int selectionStartRow = field->getSelectionStartRow();
	int selectionStartColumn = field->getSelectionStartColumn();
	int selectionEndRow = field->getCursorRow();
	int selectionEndColumn = field->getCursorColumn();
	if (selectionStartRow > selectionEndRow
		|| selectionStartRow == selectionEndRow && selectionStartColumn > selectionEndColumn)
	{
		selectionStartRow = field->getCursorRow();
		selectionStartColumn = field->getCursorColumn();
		selectionEndRow = field->getSelectionStartRow();
		selectionEndColumn = field->getSelectionStartColumn();
	}

	int fontHeight = font->getHeight();
	int lineHeight = field->getLineSpacingTop() + fontHeight + field->getLineSpacingBottom();
	for (int row = 0; row < lines.GetCount(); row++)
	{
		const String * line = lines.Get(row);
		int y = row * lineHeight + field->getLineSpacingTop();

		// Draw text

		if (masked)
			for (int i=0; i < line->GetLength(); i++)
				adapter->drawText(context, font, Vector(i * maskWidth, y),
					String(MASK_CHARACTER), alpha, caps);
		else
			adapter->drawText(context, font, Vector(0, y), line, alpha, caps);

		if (hasFocus && row >= selectionStartRow && row <= selectionEndRow)
			if (selectionStartRow == selectionEndRow
				&& selectionStartColumn == selectionEndColumn)
			{
				// Draw cursor

				int cursorPositionX;
				if (masked)
					cursorPositionX = selectionStartColumn * maskWidth;
				else
					cursorPositionX = font->getWidth(line->Substring(0, selectionStartColumn),
						field->isCaps());

				unsigned long cursorColor = GL_COLOR_A(LIGHT_BLUE, getAlpha(field));
				adapter->drawRect(context, Vector(cursorPositionX, y),
					Vector(cursorPositionX + getTextFieldCursorWidth(field) - 1,
						y + fontHeight - 1), cursorColor);
			}
			else
			{
				// Draw selection

				int lineSelectionStartColumn = row > selectionStartRow
					? 0 : selectionStartColumn;
				int lineSelectionEndColumn = row < selectionEndRow
					? line->GetLength() : selectionEndColumn;

				int selectionStartX;
				int selectionEndX;
				if (masked)
				{
					selectionStartX = lineSelectionStartColumn * maskWidth;
					selectionEndX = lineSelectionEndColumn * maskWidth;
				}
				else
				{
					selectionStartX = font->getWidth(line->Substring(0, lineSelectionStartColumn),
						field->isCaps());
					selectionEndX = selectionStartX + font->getWidth(
						line->Substring(lineSelectionStartColumn,
						lineSelectionEndColumn - lineSelectionStartColumn));
				}

				unsigned long selectionColor = GL_COLOR_A(LIGHT_BLUE, getAlpha(field) / 2);
				adapter->drawRect(context, Vector(selectionStartX, y),
					Vector(selectionEndX, y + fontHeight - 1), selectionColor);
			}
	}
}

void SimpleSkin::drawTreeField(DrawingContext & context, TreeField * field)
{
	ASSERTION_COBJECT(this);

	const ArrayList<TreeField::Item> & roots = field->getRoots();
	for (int i=0; i<roots.GetCount(); i++)
		drawTreeFieldItem(context, roots.Get(i), i >= roots.GetCount() - 1);
}

void SimpleSkin::drawTreeFieldItem(DrawingContext & context, TreeField::Item * item, bool last)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	TreeField * field = item->getField();

	DrawingContext itemContext(context, item->getPosition());
	Vector widgetSize = getTreeFieldExpansionWidgetSize(item);
	int elementSpaceWidth = getTreeFieldElementSpaceWidth(item);
	int itemsSpace = getTreeFieldItemSpaceHeight(field);

	// Draw selection background

	if (item->isSelected())
	{
		unsigned long color = GL_COLOR_A(BLUE, getAlpha(field));
		adapter->drawRect(itemContext, Vector(widgetSize.x, 0), item->getSize() - Vector(1, 1), color);
	}

	// Draw elements

	int maximumHeight = widgetSize.y;
	const ArrayList<FieldElement> & elements = item->getElements();
	for (int i=0; i<elements.GetCount(); i++)
	{
		Vector elementSize = elements.Get(i)->getSize();
		if (maximumHeight < elementSize.y)
			maximumHeight = elementSize.y;
	}

	DrawingContext elementsContext(itemContext, Vector(widgetSize.x, 0));
	FieldElement::drawElements(elementsContext, elements, maximumHeight, elementSpaceWidth);

	// Draw focus border

	if (item->hasFocus() && field->hasFocus())
		adapter->drawRect(itemContext, Vector(widgetSize.x, 0), item->getSize() - Vector(1, 1),
			GL_TRANSPARENCY, GL_COLOR_A(LIGHT_BLUE, getAlpha(field)));

	// Recursively draw children

	if (item->isExpanded())
	{
		const ArrayList<TreeField::Item> & children = item->getChildren();
		for (int i=0; i<children.GetCount(); i++)
			drawTreeFieldItem(context, children.Get(i), i >= children.GetCount() - 1);
	}

	// Draw the expansion widget

	int middleY = (maximumHeight + 1) / 2;
	adapter->drawLine(itemContext, Vector(5, middleY), Vector(13, middleY),
		GL_COLOR_A(BLUE, getAlpha(field)));

	if (last)
		adapter->drawLine(itemContext, Vector(5, 0), Vector(5, middleY),
			GL_COLOR_A(BLUE, getAlpha(field)));
	else
		adapter->drawLine(itemContext, Vector(11 / 2, 0), Vector(11 / 2, item->getDeepSize().y - 1),
			GL_COLOR_A(BLUE, getAlpha(field)));

	if (!item->getChildren().IsEmpty())
	{
		Image * expansionImage;
		if (item->isExpanded())
			expansionImage = collapseWidgetImage;
		else
			expansionImage = expandWidgetImage;

		int y = maximumHeight <= 11 ? 0 : (maximumHeight - 11 + 1) / 2;
		adapter->drawImage(itemContext, expansionImage, Vector(0, y), GL_WHITE_A(getAlpha(item->getField())));
	}
}

void SimpleSkin::drawWindowAfterClientArea(DrawingContext & context, Window * window)
{
	ASSERTION_COBJECT(this);
}

void SimpleSkin::drawWindowBeforeClientArea(DrawingContext & context, Window * window)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	if (window->isTitled() || window->isResizable())
		adapter->drawRect(context, Vector(),
			window->getSize() - Vector(1, 1), GL_TRANSPARENCY, BLUE);

	if (window->isTitled())
	{
		adapter->drawRect(context, Vector(1, 1), Vector(window->getSize().x - 2, 18), BLUE);

		if (window->isClosable())
		{
			adapter->drawImage(context, closeWidgetImage, Vector(window->getSize().x - 18, 2));
			if (window->isMaximizable())
				adapter->drawImage(context, maximizeWidgetImage, Vector(window->getSize().x - 37, 2));
		}
		else
			if (window->isMaximizable())
				adapter->drawImage(context, maximizeWidgetImage, Vector(window->getSize().x - 18, 2));

		adapter->drawText(context, windowTitleFont, Vector(8, 1), window->getTitle());
	}

	if (window->isStatused())
	{
		if (window->isTitled() || window->isResizable())
			adapter->drawRect(context, Vector(1, window->getSize().y - 19),
				Vector(window->getSize().x - 2, window->getSize().y - 2), DEEP_DARK_BLUE);
		else
			adapter->drawRect(context, Vector(0, window->getSize().y - 18),
				Vector(window->getSize().x - 1, window->getSize().y - 1), DEEP_DARK_BLUE);

		adapter->drawText(context, windowTitleFont, Vector(4, window->getSize().y - 18), window->getStatus());
	}
}

int SimpleSkin::getAlpha(Component * component)
{
	ASSERTION_COBJECT(this);

	// Windows define the alpha on their own.
	Window * window = dynamic_cast<Window *>(component);
	if (window != NULL)
		return window->getAlpha();

	// Inactive fields are grayed out.
	Field * field = dynamic_cast<Field *>(component);
	if (field != NULL && !field->isActiveIncludingAncestors())
		return 96;

	// Non-input fields are drawn somewhat opaquely
	InputField * inputField = dynamic_cast<InputField *>(component);
	if (field != NULL && inputField == NULL)
		return 192;

	// Fields and client areas without window are expected to reside on a popup,
	// so we simulate window transparency.
	if (field != NULL && field->getWindow() == NULL)
		return 160;
	ClientArea * clientArea = dynamic_cast<ClientArea *>(component);
	if (clientArea != NULL && clientArea->getWindow() == NULL)
		return 160;

	// Focused components are drawn opaquely.
	if (component->hasFocus())
		return 255;

	// Not focused components are drawn somewhat transparently.
	return 160;
}

Vector SimpleSkin::getBalloonBorderSize(BalloonPopup * popup)
{
	ASSERTION_COBJECT(this);
	return Vector(16, 16);
}

int SimpleSkin::getBalloonArrowOffset(BalloonPopup * popup)
{
	ASSERTION_COBJECT(this);
	return 8;
}

Vector SimpleSkin::getButtonSize(Button * button)
{
	ASSERTION_COBJECT(this);

	Image * image = button->getImage();
	Vector textSize = button->getFont()->getSize(button->getText(), button->isCaps(), true);

	Vector contentSize;
	if (image != NULL)
		if (image->getSize().y > textSize.y)
			contentSize = Vector(image->getSize().x + textSize.x, image->getSize().y);
		else
			contentSize = Vector(image->getSize().x + textSize.x, textSize.y);
	else
		contentSize = textSize;

	return Vector(8 + 8, 3 + 2) + contentSize;
}

Vector SimpleSkin::getCheckBoxSize(CheckBox * checkBox)
{
	ASSERTION_COBJECT(this);

	Vector textSize = checkBox->getFont()->getSize(checkBox->getText(), checkBox->isCaps(), true);
	return Vector(20 + textSize.x, max(15, textSize.y));
}

PointerType * SimpleSkin::getDefaultDraggingPointerType()
{
	ASSERTION_COBJECT(this);
	return draggingPointerType;
}

Font * SimpleSkin::getDefaultFieldFont()
{
	ASSERTION_COBJECT(this);
	return fieldFont;
}

PointerType * SimpleSkin::getDefaultImpossiblePointerType()
{
	ASSERTION_COBJECT(this);
	return impossiblePointerType;
}

void SimpleSkin::getDialogMargins(Dialog * dialog,
	int & bottomContentMargin, int & buttonSpacing, int & iconContentSpacing)
{
	ASSERTION_COBJECT(this);

	bottomContentMargin = 16;
	buttonSpacing = 8;
	iconContentSpacing = 8;
}

void SimpleSkin::getGroupFieldMarginSizes(GroupField * field,
	Vector & topLeftSize, Vector & bottomRightSize)
{
	ASSERTION_COBJECT(this);

	if (field->isBordered())
	{
		Font * font = field->getFont();
		int borderTop = field->getText().IsEmpty() ? 0 : font->getHeight();

		topLeftSize.x = 2;
		topLeftSize.y = borderTop + 2;
		bottomRightSize.x = 2;
		bottomRightSize.y = 2;
	}
	else
	{
		topLeftSize.x = 0;
		topLeftSize.y = 0;
		bottomRightSize.x = 0;
		bottomRightSize.y = 0;
	}
}

PointerType * SimpleSkin::getHourGlassPointerType()
{
	ASSERTION_COBJECT(this);
	return hourGlassPointerType;
}

int SimpleSkin::getListFieldColumnBorderWidth(ListField * field)
{
	ASSERTION_COBJECT(this);
	return 1;
}

int SimpleSkin::getListFieldColumnPaddingWidth(ListField * field)
{
	ASSERTION_COBJECT(this);
	return 0;
}

int SimpleSkin::getListFieldHeadingHeight(ListField * field)
{
	ASSERTION_COBJECT(this);
	return 3 + field->getFont()->getHeight() + 2;
}

int SimpleSkin::getListFieldRowHeight(ListField * field)
{
	ASSERTION_COBJECT(this);
	return 2 + field->getFont()->getHeight() + 1;
}

Vector SimpleSkin::getMenuItemSize(Menu::Item * item)
{
	ASSERTION_COBJECT(this);

	if (item->isSeparator())
	{
		if (item->getParent()->isHorizontal())
			return Vector(8, 0);
		else
			return Vector(0, 8);
	}
	else
	{
		Vector checkSize;
		if (item->getCheckState() == Menu::CHECK)
			checkSize = menuCheckImage->getSize();
		else if (item->getCheckState() == Menu::RADIO)
			checkSize = menuRadioImage->getSize();

		Vector textSize = fieldFont->getSize(item->getText()) + Vector(16, 0);
		return Vector(checkSize.x + textSize.x, checkSize.y > textSize.y ? checkSize.y : textSize.y);
	}
}

void SimpleSkin::getMenuPopupBorderSizes(MenuPopup * menuPopup,
	Vector & topLeftSize, Vector & bottomRightSize)
{
	ASSERTION_COBJECT(this);

	topLeftSize.x = 8;
	topLeftSize.y = 8;
	bottomRightSize.x = 8;
	bottomRightSize.y = 8;
}

Image * SimpleSkin::getMessageDialogIconImage(MessageDialog::Icon icon)
{
	ASSERTION_COBJECT(this);

	switch (icon)
	{
		case MessageDialog::ICON_INFORMATION: return informationIconImage; break;
		case MessageDialog::ICON_QUESTION: return questionIconImage; break;
		case MessageDialog::ICON_WARNING: return warningIconImage; break;
		case MessageDialog::ICON_ERROR: return errorIconImage; break;

		default: return NULL;
	}
}

Image * SimpleSkin::getPathTypeImage(Path::Type type)
{
	ASSERTION_COBJECT(this);

	switch (type)
	{
		case Path::TYPE_ROOT: return rootImage; break;
		case Path::TYPE_FILE: return fileImage; break;
		case Path::TYPE_FOLDER: return folderImage; break;
		case Path::TYPE_HARDDISK: return hardDiskImage; break;
		case Path::TYPE_FLOPPYDISK: return floppyDiskImage; break;
		case Path::TYPE_MEMORYSTICK: return memoryStickImage; break;
		case Path::TYPE_CDROM: return cdromImage; break;
		case Path::TYPE_NETWORKCOMPUTER: return networkComputerImage; break;
		case Path::TYPE_NETWORKDRIVE: return networkDriveImage; break;
		case Path::TYPE_VIRTUAL: return virtualImage; break;
		case Path::TYPE_RAMDISK: return ramDiskImage; break;

		default: return unknownImage;
	}
}

Vector SimpleSkin::getRadioBoxSize(RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);

	Vector textSize = radioBox->getFont()->getSize(radioBox->getText(), radioBox->isCaps(), true);
	return Vector(20 + textSize.x, max(15, textSize.y));
}

int SimpleSkin::getSelectorHeight(Selector * selector)
{
	ASSERTION_COBJECT(this);
	return selector->getFont()->getHeight() + 5;
}

PointerType * SimpleSkin::getSelectPointerType()
{
	ASSERTION_COBJECT(this);
	return selectPointerType;
}

void SimpleSkin::getSingleFieldPopupBorderSizes(SingleFieldPopup * popup,
	Vector & topLeftSize, Vector & bottomRightSize)
{
	ASSERTION_COBJECT(this);

	topLeftSize.x = 0;
	topLeftSize.y = 0;
	bottomRightSize.x = 0;
	bottomRightSize.y = 0;
}

int SimpleSkin::getSliderWidgetWidth(Slider * slider)
{
	ASSERTION_COBJECT(this);
	return 8;
}

int SimpleSkin::getSplitterSize(SplittingArea * area)
{
	ASSERTION_COBJECT(this);
	return 1;
}

void SimpleSkin::getTabbingAreaFrameSizes(TabbingArea * area,
	Vector & topLeftSize, Vector & bottomRightSize)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	const ArrayList<ClientArea> & panes = area->getPanes();
	int panesCount = panes.GetCount();
	int x = 0;
	int y = 0;
	for (int i=0; i<panesCount; i++)
	{
		ClientArea * pane = panes.Get(i);
		if (!pane->isVisible())
			continue;

		const String & name = area->getPaneName(i);
		Vector tabTextSize = tabFont->getSize(name, false, true);
		Vector tabTextSizeWithBorder = Vector(8, 2) + tabTextSize + Vector(8, 3);

		if (x > 0 && area->getSize().x - x < tabTextSizeWithBorder.x)
		{
			y += tabTextSizeWithBorder.y;
			x = 0;
		}

		x += tabTextSizeWithBorder.x;
	}

	y += 2 + tabFont->getHeight() + 3;

	topLeftSize = Vector(1, y);
	bottomRightSize = Vector(1, 1);
}

Vector SimpleSkin::getTextFieldContentSize(TextField * field)
{
	ASSERTION_COBJECT(this);

	Font * font = field->getFont();
	const ArrayList<String> & lines = field->getLines();
	bool masked = field->isMasked();
	int maskWidth = font->getWidth(MASK_CHARACTER) + 1;

	int fontHeight = font->getHeight();
	Vector contentSize = Vector();
	for (int row = 0; row < lines.GetCount(); row++)
	{
		const String * line = lines.Get(row);

		Vector lineSize;
		if (masked)
			lineSize = Vector(maskWidth * line->GetLength(), fontHeight);
		else
			lineSize = font->getSize(line, field->isCaps());

		if (!field->hasSelection() && field->getCursorRow() == row
			&& field->getCursorColumn() == line->GetLength())
			lineSize.x += getTextFieldCursorWidth(field);

		contentSize.y += lineSize.y;
		if (lineSize.x > contentSize.x)
			contentSize.x = lineSize.x;
	}

	contentSize += Vector(0, (field->getLineSpacingTop() + field->getLineSpacingBottom()) * lines.GetCount());
	return contentSize;
}

int SimpleSkin::getTextFieldCursorWidth(TextField * field)
{
	ASSERTION_COBJECT(this);
	return 2;
}

void SimpleSkin::getTextFieldRowAndColumnAtPosition(TextField * field,
	int & row, int & column, Vector position)
{
	ASSERTION_COBJECT(this);

	Font * font = field->getFont();
	int fontHeight = font->getHeight();
	int lineHeight = field->getLineSpacingTop() + fontHeight + field->getLineSpacingBottom();

	if (position.y < 0)
		row = 0;
	else
		row = position.y / lineHeight;

	const ArrayList<String> & lines = field->getLines();
	if (row >= lines.GetCount())
		row = lines.GetCount() - 1;

	String * line = field->getLines().Get(row);
	int lineLength = line->GetLength();
	bool caps = field->isCaps();
	int x = 0;
	for (column = 0; column < lineLength; column++)
	{
		char character = (*line)[column];
		if (caps)
			character = String::ToUpperCase(character);

		int characterWidth = font->getWidth(character);
		if (x + characterWidth * 2 / 3 >= position.x)
			return;
		x += characterWidth + 1;
	}
}

int SimpleSkin::getTreeFieldElementSpaceWidth(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);
	return 2;
}

Vector SimpleSkin::getTreeFieldExpansionWidgetSize(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);
	return Vector(16, 11);
}

int SimpleSkin::getTreeFieldItemSpaceHeight(TreeField * field)
{
	ASSERTION_COBJECT(this);
	return 1;
}

void SimpleSkin::getWindowFrameSizes(Window * window,
	Vector & topLeftSize, Vector & bottomRightSize)
{
	ASSERTION_COBJECT(this);

	topLeftSize.x = 0; topLeftSize.y = 0;
	bottomRightSize.x = 0; bottomRightSize.y = 0;

	// Add a border for titled and/or resizable windows
	if (window->isTitled() || window->isResizable())
	{
		topLeftSize.x += 1; topLeftSize.y += 1;
		bottomRightSize.x += 1; bottomRightSize.y += 1;
	}

	// Add a title bar for titled windows
	if (window->isTitled())
	{
		topLeftSize.y += 18;
	}

	// Add a status bar for statused windows
	if (window->isStatused())
	{
		bottomRightSize.y += 18;
	}
}

unsigned long SimpleSkin::getWindowModulationColor(Window * window)
{
	ASSERTION_COBJECT(this);

	int alpha = window->getAlpha();
	if (Desktop::getInstance()->getWindows().GetLast() == window)
		return GL_WHITE_A(alpha);
	else
	{
		unsigned char darkening = (unsigned char) ((int) 255 * (focusableFactor + 250) / 500);
		return GL_RGBA(darkening, darkening, darkening, alpha);
	}
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SimpleSkin, Skin);
#endif

void SimpleSkin::initialize()
{
	ASSERTION_COBJECT(this);

	Skin::initialize();

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	initializeFonts();
	initializeRegularImages();
	initializePathTypeImages();
	initializePointerTypes();
}

void SimpleSkin::initializeFonts()
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	fieldFont = adapter->getFont("fonts\\Verdana-10-bold-lightblue-brown.png");
	tabFont = adapter->getFont("fonts\\Verdana-10-bold-lightblue-brown.png");
	windowTitleFont = adapter->getFont("fonts\\Verdana-10-bold-lightblue-brown.png");
}

void SimpleSkin::initializePathTypeImages()
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	cdromImage = adapter->getImage("images\\PathTypeCdrom.png", true);
	fileImage = adapter->getImage("images\\PathTypeFile.png", true);
	floppyDiskImage = adapter->getImage("images\\PathTypeFloppyDisk.png", true);
	folderImage = adapter->getImage("images\\PathTypeFolder.png", true);
	hardDiskImage = adapter->getImage("images\\PathTypeHardDisk.png", true);
	memoryStickImage = adapter->getImage("images\\PathTypeMemoryStick.png", true);
	networkComputerImage = adapter->getImage("images\\PathTypeNetworkComputer.png", true);
	networkDriveImage = adapter->getImage("images\\PathTypeNetworkDrive.png", true);
	ramDiskImage = adapter->getImage("images\\PathTypeRamDisk.png", true);
	rootImage = adapter->getImage("images\\PathTypeRoot.png", true);
	virtualImage = adapter->getImage("images\\PathTypeVirtual.png", true);
	unknownImage = adapter->getImage("images\\PathTypeUnknown.png", true);
}

void SimpleSkin::initializeRegularImages()
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	checkBoxCheckedImage = adapter->getImage("images\\CheckBoxChecked.png", true);
	checkBoxUncheckedImage = adapter->getImage("images\\CheckBoxUnchecked.png", true);
	checkBoxTriStateImage = adapter->getImage("images\\CheckBoxTriState.png", true);
	closeWidgetImage = adapter->getImage("images\\CloseWidget.png", true);
	collapseWidgetImage = adapter->getImage("images\\TreeWidgetCollapse.png", true);
	downWidgetImage = adapter->getImage("images\\DownWidget.png", true);
	errorIconImage = adapter->getImage("images\\ErrorIcon.png", true);
	expandWidgetImage = adapter->getImage("images\\TreeWidgetExpand.png", true);
	informationIconImage = adapter->getImage("images\\InformationIcon.png", true);
	leftWidgetImage = adapter->getImage("images\\LeftWidget.png", true);
	maximizeWidgetImage = adapter->getImage("images\\MaximizeWidget.png", true);
	menuCheckImage = adapter->getImage("images\\MenuCheckImage.png", true);
	menuRadioImage = adapter->getImage("images\\MenuRadioImage.png", true);
	questionIconImage = adapter->getImage("images\\QuestionIcon.png", true);
	radioBoxCheckedImage = adapter->getImage("images\\RadioBoxChecked.png", true);
	radioBoxUncheckedImage = adapter->getImage("images\\RadioBoxUnchecked.png", true);
	rightWidgetImage = adapter->getImage("images\\RightWidget.png", true);
	upWidgetImage = adapter->getImage("images\\UpWidget.png", true);
	warningIconImage = adapter->getImage("images\\WarningIcon.png", true);
}

void SimpleSkin::initializePointerTypes()
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();

	if ((draggingPointerType = new SimplePointerType(
		adapter->getImage("images\\DraggingPointer.png", true), Vector())) == NULL)
		throw EOUTOFMEMORY;
	if ((hourGlassPointerType = new SimplePointerType(
		adapter->getImage("images\\HourGlassPointer.png", true), Vector(5, 10))) == NULL)
		throw EOUTOFMEMORY;
	if ((impossiblePointerType = new SimplePointerType(
		adapter->getImage("images\\ImpossiblePointer.png", true), Vector(6, 6))) == NULL)
		throw EOUTOFMEMORY;
	if ((resize0PointerType = new SimplePointerType(
		adapter->getImage("images\\Resize0Pointer.png", true), Vector(8, 4))) == NULL)
		throw EOUTOFMEMORY;
	if ((resize45PointerType = new SimplePointerType(
		adapter->getImage("images\\Resize45Pointer.png", true), Vector(6, 6))) == NULL)
		throw EOUTOFMEMORY;
	if ((resize90PointerType = new SimplePointerType(
		adapter->getImage("images\\Resize90Pointer.png", true), Vector(4, 8))) == NULL)
		throw EOUTOFMEMORY;
	if ((resize135PointerType = new SimplePointerType(
		adapter->getImage("images\\Resize135Pointer.png", true), Vector(6, 6))) == NULL)
		throw EOUTOFMEMORY;
	if ((selectPointerType = new SimplePointerType(
		adapter->getImage("images\\SelectPointer.png", true), Vector())) == NULL)
		throw EOUTOFMEMORY;
}

void SimpleSkin::setupBalloonPopup(BalloonPopup * popup)
{
	ASSERTION_COBJECT(this);
}

void SimpleSkin::setupClientArea(ClientArea * area)
{
	ASSERTION_COBJECT(this);
	area->setBackgroundColor(DEEP_DARK_BLUE);
}

void SimpleSkin::setupField(Field * field)
{
	ASSERTION_COBJECT(this);

	FieldLayoutData * layoutData = field->getFieldLayoutData();
	layoutData->setTopLeftMargin(Vector(4, 4));
	layoutData->setBottomRightMargin(Vector(4, 4));
}

void SimpleSkin::setupKeyStrokeField(KeyStrokeField * field)
{
	ASSERTION_COBJECT(this);

	field->setMinimumSize(Vector(64, 5 + field->getFont()->getHeight()));
	field->resize(Vector(96, field->getMinimumSize().y));
}

void SimpleSkin::setupListField(ListField * field)
{
	ASSERTION_COBJECT(this);

	int rowHeight = getListFieldRowHeight(field);
	field->getRowsField()->setScrollingUnitSize(Vector(rowHeight, rowHeight));

	ListFieldColumnSizeWidget * columnSizeWidget;
	if ((columnSizeWidget = new ListFieldColumnSizeWidget) == NULL)
		throw EOUTOFMEMORY;
	field->appendWidget(columnSizeWidget);
}

void SimpleSkin::setupProgressBar(ProgressBar * progressBar)
{
	ASSERTION_COBJECT(this);

	progressBar->setMinimumSize(Vector(progressBar->getMinimumSize().x, 12));
	progressBar->resize(Vector(progressBar->getSize().x, progressBar->getMinimumSize().y));
}

void SimpleSkin::setupScrollingArea(ScrollingArea * area)
{
	ASSERTION_COBJECT(this);

	area->setScrollerBreadth(16);

	ScrollingComponentArrowWidget * arrowWidget;
	if ((arrowWidget = new ScrollingComponentArrowWidget) == NULL)
		throw EOUTOFMEMORY;
	area->appendWidget(arrowWidget);

	ScrollingComponentHandleWidget * scrollerWidget;
	if ((scrollerWidget = new ScrollingComponentHandleWidget) == NULL)
		throw EOUTOFMEMORY;
	area->appendWidget(scrollerWidget);
}

void SimpleSkin::setupScrollingField(ScrollingField * field)
{
	ASSERTION_COBJECT(this);

	field->setScrollerBreadth(16);

	ScrollingComponentArrowWidget * arrowWidget;
	if ((arrowWidget = new ScrollingComponentArrowWidget) == NULL)
		throw EOUTOFMEMORY;
	field->appendWidget(arrowWidget);

	ScrollingComponentHandleWidget * scrollerWidget;
	if ((scrollerWidget = new ScrollingComponentHandleWidget) == NULL)
		throw EOUTOFMEMORY;
	field->appendWidget(scrollerWidget);
}

void SimpleSkin::setupSelector(Selector * selector)
{
	ASSERTION_COBJECT(this);
	selector->getPopupField()->getRowsField()->setBorderSize(Vector(1, 1));
}

void SimpleSkin::setupSingleFieldArea(SingleFieldArea * area)
{
	ASSERTION_COBJECT(this);
	area->setBackgroundColor(DEEP_DARK_BLUE);
}

void SimpleSkin::setupSlider(Slider * slider)
{
	ASSERTION_COBJECT(this);

	slider->setMinimumSize(Vector(slider->getMinimumSize().x, 19));
	slider->resize(Vector(slider->getSize().x, slider->getMinimumSize().y));

	SliderWidget * sliderWidget;
	if ((sliderWidget = new SliderWidget) == NULL)
		throw EOUTOFMEMORY;
	slider->appendWidget(sliderWidget);
}

void SimpleSkin::setupSplittingArea(SplittingArea * area)
{
	ASSERTION_COBJECT(this);

	SplitterWidget * splitterWidget;
	if ((splitterWidget = new SplitterWidget) == NULL)
		throw EOUTOFMEMORY;
	area->appendWidget(splitterWidget);
}

void SimpleSkin::setupTabbingArea(TabbingArea * area)
{
	ASSERTION_COBJECT(this);

	TabWidget * tabbingWidget;
	if ((tabbingWidget = new TabWidget) == NULL)
		throw EOUTOFMEMORY;
	area->appendWidget(tabbingWidget);
}

void SimpleSkin::setupTextElementEditPopup(Popup * popup, TextField * textField,
	TextElement * element, Vector elementPosition, Vector elementSize)
{
	ASSERTION_COBJECT(this);

	if (elementSize.x < 64)
		elementSize.x = 64;

	popup->moveTo(elementPosition + Vector(2, 1));
	textField->setMinimumSize(elementSize + Vector(2, 3));
	textField->resize(textField->getMinimumSize());
}

void SimpleSkin::setupTextField(TextField * field)
{
	ASSERTION_COBJECT(this);

	field->setMinimumSize(Vector(field->getMinimumSize().x, 5 + field->getFont()->getHeight()));
	field->resize(Vector(64, field->getMinimumSize().y));
}

void SimpleSkin::setupTreeField(TreeField * field)
{
	ASSERTION_COBJECT(this);
	field->setIndentation(19);
}

void SimpleSkin::setupWindow(Window * window)
{
	ASSERTION_COBJECT(this);

	WindowBorderWidget * borderWidget;
	if ((borderWidget = new WindowBorderWidget) == NULL)
		throw EOUTOFMEMORY;
	window->appendWidget(borderWidget);

	WindowCloseWidget * closeWidget;
	if ((closeWidget = new WindowCloseWidget) == NULL)
		throw EOUTOFMEMORY;
	window->appendWidget(closeWidget);

	WindowMaximizeWidget * maximizeWidget;
	if ((maximizeWidget = new WindowMaximizeWidget) == NULL)
		throw EOUTOFMEMORY;
	window->appendWidget(maximizeWidget);

	WindowTitleWidget * titleWidget;
	if ((titleWidget = new WindowTitleWidget) == NULL)
		throw EOUTOFMEMORY;
	window->appendWidget(titleWidget);
}

void SimpleSkin::update()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (desktop->isModal())
	{
		if (focusableFactor > 0)
		{
			int timeStep = desktop->getTimeStep();
			focusableFactor -= timeStep;
			if (focusableFactor < 0)
				focusableFactor = 0;
		}
	}
	else
	{
		if (focusableFactor < 250)
		{
			int timeStep = desktop->getTimeStep();
			focusableFactor += timeStep;
			if (focusableFactor > 250)
				focusableFactor = 250;
		}
	}
}

void SimpleSkin::updateProgressBarCandyMode(ProgressBar * progressBar)
{
	ASSERTION_COBJECT(this);
	// TODO
}


SimpleSkin::ListFieldColumnSizeWidget::ListFieldColumnSizeWidget()
{
	ASSERTION_COBJECT(this);

	lastHitColumnNo = -1;
	resize = false;
	startWidth = 0;
}

PointerType * SimpleSkin::ListFieldColumnSizeWidget::getPointerType(Vector position)
{
	ASSERTION_COBJECT(this);

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();
	return resize ? skin->resize0PointerType : NULL;
}

bool SimpleSkin::ListFieldColumnSizeWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	ListField * field = (ListField *) getParent();
	if ((!field->isColumnsResizable() && !field->isColumnsMovable()) || !field->isHeadlined())
		return false;

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();
	if (point.x < 0 || point.y < 0 || point.y >= skin->getListFieldHeadingHeight(field))
		return false;

	point.x += field->getRowsField()->getScrollPositionNow().x;

	int x = 0;
	int columnPadding = skin->getListFieldColumnPaddingWidth(field);
	const ArrayList<ListField::Column> & columns = field->getColumns();
	int columnNo;
	for (columnNo = 0; columnNo < columns.GetCount(); columnNo++)
	{
		ListField::Column * column = columns.Get(columnNo);
		x += column->getActualWidth();
		if (column->isVisible() && point.x < x + columnPadding)
		{
			bool resize = point.x >= x - columnPadding;
			if (resize)
			{
				if (!field->isColumnsResizable())
					return false;
			}
			else
			{
				if (!field->isColumnsMovable())
					return false;
			}

			if (Desktop::getInstance()->getDraggingSurrogate() == NULL)
			{
				this->resize = resize;
				lastHitColumnNo = columnNo;
				startWidth = column->getWidth();
			}

			return true;
		}
	}

	return false;
}

DraggingSurrogate * SimpleSkin::ListFieldColumnSizeWidget::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);

	DraggingSurrogate * surrogate;
	if ((surrogate = new DraggingSurrogate) == NULL)
		throw EOUTOFMEMORY;

	return surrogate;
}

void SimpleSkin::ListFieldColumnSizeWidget::onDragMove(const Vector & startPosition,
	const Vector & lastPosition, const Vector & currentPosition)
{
	ASSERTION_COBJECT(this);

	ListField * field = (ListField *) getParent();
	const ArrayList<ListField::Column> & columns = field->getColumns();
	if (lastHitColumnNo < 0 || lastHitColumnNo >= columns.GetCount())
		return;

	ListField::Column * column = columns.Get(lastHitColumnNo);
	if (resize)
		column->setWidth(startWidth + currentPosition.x - startPosition.x);

	// TODO: Column move (resize == false)
}


SimpleSkin::ScrollingComponentArrowWidget::ScrollingComponentArrowWidget()
{
	ASSERTION_COBJECT(this);

	arrow = NONE;
	stepsToSkip = 0;
}

void SimpleSkin::ScrollingComponentArrowWidget::executeArrow(int steps)
{
	ASSERTION_COBJECT(this);

	Component * component = getParent();
	ScrollingField * field = dynamic_cast<ScrollingField *>(component);
	ScrollingArea * area = dynamic_cast<ScrollingArea *>(component);
	if (field != NULL && !field->isActiveIncludingAncestors())
		return;

	Vector scrollingUnitSize;
	Vector contentViewSize;
	if (field != NULL)
	{
		scrollingUnitSize = field->getScrollingUnitSize();
		contentViewSize = field->getContentViewSize();
	}
	else if (area != NULL)
	{
		scrollingUnitSize = area->getScrollingUnitSize();
		contentViewSize = area->getContentViewSize();
	}

	Vector scrollDelta;
	switch (arrow)
	{
		case LEFT_ARROW: scrollDelta = Vector(-scrollingUnitSize.x, 0); break;
		case LEFT_PAGE: scrollDelta = Vector(-contentViewSize.x, 0); break;
		case RIGHT_ARROW: scrollDelta = Vector(scrollingUnitSize.x, 0); break;
		case RIGHT_PAGE: scrollDelta = Vector(contentViewSize.x, 0); break;
		case UP_ARROW: scrollDelta = Vector(0, -scrollingUnitSize.y); break;
		case UP_PAGE: scrollDelta = Vector(0, -contentViewSize.y); break;
		case DOWN_ARROW: scrollDelta = Vector(0, scrollingUnitSize.y); break;
		case DOWN_PAGE: scrollDelta = Vector(0, contentViewSize.y); break;
	}

	if (field != NULL)
		field->scroll(scrollDelta * steps);
	else if (area != NULL)
		area->scroll(scrollDelta * steps);
}

SimpleSkin::ScrollingComponentArrowWidget::Arrow
	SimpleSkin::ScrollingComponentArrowWidget::getHitArrow(Vector position)
{
	ASSERTION_COBJECT(this);

	Component * component = getParent();
	ScrollingField * field = dynamic_cast<ScrollingField *>(component);
	ScrollingArea * area = dynamic_cast<ScrollingArea *>(component);

	ScrollerInfo info;
	bool horizontallyScrollable = false;
	bool verticallyScrollable = false;
	Skin * skin = Desktop::getInstance()->getSkin();
	if (field != NULL)
	{
		skin->fillScrollerInfo(info, field);
		horizontallyScrollable = field->isHorizontallyScrollable();
		verticallyScrollable = field->isVerticallyScrollable();
	}
	else if (area != NULL)
	{
		skin->fillScrollerInfo(info, area);
		horizontallyScrollable = area->isHorizontallyScrollable();
		verticallyScrollable = area->isVerticallyScrollable();
	}

	int x = position.x;
	int y = position.y;

	if (horizontallyScrollable
		&& x >= info.horizontalStartX && x <= info.horizontalEndX
		&& y >= info.horizontalStartY && y <= info.horizontalEndY)
	{
		if (x < info.horizontalStartX + 16)
			return LEFT_ARROW;
		else if (x < info.horizontalHandleStartX)
			return LEFT_PAGE;

		if (x > info.horizontalEndX - 16)
			return RIGHT_ARROW;
		else if (x > info.horizontalHandleEndX)
			return RIGHT_PAGE;
	}

	if (verticallyScrollable
		&& x >= info.verticalStartX && x <= info.verticalEndX
		&& y >= info.verticalStartY && y <= info.verticalEndY)
	{
		if (y < info.verticalStartY + 16)
			return UP_ARROW;
		else if (y < info.verticalHandleStartY)
			return UP_PAGE;

		if (y > info.verticalEndY - 16)
			return DOWN_ARROW;
		else if (y > info.verticalHandleEndY)
			return DOWN_PAGE;
	}

	return NONE;
}

bool SimpleSkin::ScrollingComponentArrowWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	Arrow hitArrow = getHitArrow(point);
	return hitArrow != NONE;
}

void SimpleSkin::ScrollingComponentArrowWidget::onDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Arrow hitArrow = getHitArrow(position);
	if (hitArrow == arrow)
		executeArrow(1);
}

long SimpleSkin::ScrollingComponentArrowWidget::onHold(Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);

	Arrow hitArrow = getHitArrow(position);
	if (hitArrow != arrow)
		return 0;

	int steps = (int) (holdTime / 64);
	holdTime -= steps * 64;

	if (stepsToSkip >= steps)
		stepsToSkip -= steps;
	else
	{
		steps -= stepsToSkip;
		stepsToSkip = 0;
		executeArrow(steps);
	}

	return holdTime;
}

void SimpleSkin::ScrollingComponentArrowWidget::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	arrow = getHitArrow(position);
	stepsToSkip = 4;
	executeArrow(1);
}


SimpleSkin::ScrollingComponentHandleWidget::ScrollingComponentHandleWidget()
{
	ASSERTION_COBJECT(this);
	direction = NONE;
}

SimpleSkin::ScrollingComponentHandleWidget::Direction
	SimpleSkin::ScrollingComponentHandleWidget::getHitDirection(Vector position)
{
	ASSERTION_COBJECT(this);

	Component * component = getParent();
	ScrollingField * field = dynamic_cast<ScrollingField *>(component);
	ScrollingArea * area = dynamic_cast<ScrollingArea *>(component);

	ScrollerInfo info;
	bool horizontallyScrollable = false;
	bool verticallyScrollable = false;
	Skin * skin = Desktop::getInstance()->getSkin();
	if (field != NULL)
	{
		skin->fillScrollerInfo(info, field);
		horizontallyScrollable = field->isHorizontallyScrollable();
		verticallyScrollable = field->isVerticallyScrollable();
	}
	else if (area != NULL)
	{
		skin->fillScrollerInfo(info, area);
		horizontallyScrollable = area->isHorizontallyScrollable();
		verticallyScrollable = area->isVerticallyScrollable();
	}

	int x = position.x;
	int y = position.y;

	if (horizontallyScrollable
		&& x >= info.horizontalHandleStartX && x <= info.horizontalHandleEndX
		&& y >= info.horizontalStartY && y <= info.horizontalEndY)
		return HORIZONTALLY;

	if (verticallyScrollable
		&& x >= info.verticalStartX && x <= info.verticalEndX
		&& y >= info.verticalHandleStartY && y <= info.verticalHandleEndY)
		return VERTICALLY;

	return NONE;
}

bool SimpleSkin::ScrollingComponentHandleWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	Direction hitDirection = getHitDirection(point);
	return hitDirection != NONE;
}

void SimpleSkin::ScrollingComponentHandleWidget::onDragMove(const Vector & startPosition,
	const Vector & lastPosition, const Vector & currentPosition)
{
	ASSERTION_COBJECT(this);

	Component * component = getParent();
	ScrollingField * field = dynamic_cast<ScrollingField *>(component);
	ScrollingArea * area = dynamic_cast<ScrollingArea *>(component);
	if (field != NULL && !field->isActiveIncludingAncestors())
		return;

	ScrollerInfo info;
	bool horizontallyScrollable = false;
	bool verticallyScrollable = false;
	Skin * skin = Desktop::getInstance()->getSkin();
	if (field != NULL)
	{
		skin->fillScrollerInfo(info, field);
		horizontallyScrollable = field->isHorizontallyScrollable();
		verticallyScrollable = field->isVerticallyScrollable();
	}
	else if (area != NULL)
	{
		skin->fillScrollerInfo(info, area);
		horizontallyScrollable = area->isHorizontallyScrollable();
		verticallyScrollable = area->isVerticallyScrollable();
	}

	if (direction == HORIZONTALLY)
	{
		int handlePosition = 0;
		if (currentPosition.y - component->getAbsolutePosition().y >= info.horizontalStartY - 128
			&& currentPosition.y - component->getAbsolutePosition().y <= info.horizontalEndY + 128)
			handlePosition = currentPosition.x - startPosition.x;

		handlePosition += scrollerHandleStartPosition.x - info.horizontalScrollAreaStartX;
		int maximumHandlePosition = info.horizontalScrollAreaEndX - info.horizontalScrollAreaStartX
			- (info.horizontalHandleEndX - info.horizontalHandleStartX);

		if (field != NULL)
		{
			int scrollPosition;
			if (maximumHandlePosition <= 0)
				scrollPosition = 0;
			else
			{
				int maximumScrollPosition = field->getMaximumScrollPosition().x;
				scrollPosition = (int) (1.0 * handlePosition / maximumHandlePosition * maximumScrollPosition + 0.5);
			}

			field->scrollTo(Vector(scrollPosition, field->getScrollPosition().y));
		}
		else if (area != NULL)
		{
			int scrollPosition;
			if (maximumHandlePosition <= 0)
				scrollPosition = 0;
			else
			{
				int maximumScrollPosition = area->getMaximumScrollPosition().x;
				scrollPosition = (int) (1.0 * handlePosition / maximumHandlePosition * maximumScrollPosition + 0.5);
			}

			area->scrollTo(Vector(scrollPosition, area->getScrollPosition().y));
		}
	}
	else if (direction == VERTICALLY)
	{
		int handlePosition = 0;
		if (currentPosition.x - component->getAbsolutePosition().x >= info.verticalStartX - 128
			&& currentPosition.x - component->getAbsolutePosition().x <= info.verticalEndX + 128)
			handlePosition = currentPosition.y - startPosition.y;

		handlePosition += scrollerHandleStartPosition.y - info.verticalScrollAreaStartY;
		int maximumHandlePosition = info.verticalScrollAreaEndY - info.verticalScrollAreaStartY
			- (info.verticalHandleEndY - info.verticalHandleStartY);

		if (field != NULL)
		{
			int scrollPosition;
			if (maximumHandlePosition <= 0)
				scrollPosition = 0;
			else
			{
				int maximumScrollPosition = field->getMaximumScrollPosition().y;
				scrollPosition = (int) (1.0 * handlePosition / maximumHandlePosition * maximumScrollPosition + 0.5);
			}

			field->scrollTo(Vector(field->getScrollPosition().x, scrollPosition));
		}
		else if (area != NULL)
		{
			int scrollPosition;
			if (maximumHandlePosition <= 0)
				scrollPosition = 0;
			else
			{
				int maximumScrollPosition = area->getMaximumScrollPosition().y;
				scrollPosition = (int) (1.0 * handlePosition / maximumHandlePosition * maximumScrollPosition + 0.5);
			}

			area->scrollTo(Vector(area->getScrollPosition().x, scrollPosition));
		}
	}
}

DraggingSurrogate * SimpleSkin::ScrollingComponentHandleWidget::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);

	Component * component = getParent();
	ScrollingField * field = dynamic_cast<ScrollingField *>(component);
	ScrollingArea * area = dynamic_cast<ScrollingArea *>(component);

	direction = getHitDirection(startPosition - getOrigin());

	ScrollerInfo info;
	Skin * skin = Desktop::getInstance()->getSkin();
	if (field != NULL)
		skin->fillScrollerInfo(info, field);
	else if (area != NULL)
		skin->fillScrollerInfo(info, area);

	scrollerHandleStartPosition = Vector(info.horizontalHandleStartX, info.verticalHandleStartY);

	DraggingSurrogate * surrogate;
	if ((surrogate = new DraggingSurrogate) == NULL)
		throw EOUTOFMEMORY;
	surrogate->setUnsupportingPointerType(skin->getSelectPointerType());

	return surrogate;
}


bool SimpleSkin::SliderWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	Slider * slider = (Slider *) getParent();

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();
	int widgetWidth = skin->getSliderWidgetWidth(slider);
	if (slider->getSize().x <= widgetWidth - 1 || slider->getStart() >= slider->getEnd())
		return false;

	if (slider->getEnd() - slider->getStart() > 0)
	{
		int currentX = (int) ((slider->getConstrainedCurrent() * 1.0 - slider->getStart())
			/ (slider->getEnd() - slider->getStart()) * (slider->getSize().x - widgetWidth + 1));
		return point.x >= currentX && point.x <= currentX + widgetWidth - 1;
	}

	return false;
}

void SimpleSkin::SliderWidget::onDragMove(const Vector & startPosition,
	const Vector & lastPosition, const Vector & currentPosition)
{
	ASSERTION_COBJECT(this);

	Slider * slider = (Slider *) getParent();
	if (!slider->isActiveIncludingAncestors())
		return;

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();
	int widgetWidth = skin->getSliderWidgetWidth(slider);
	if (slider->getSize().x <= widgetWidth - 1 || slider->getStart() >= slider->getEnd())
		return;

	int current = slider->getStart()
		+ (int) (((currentPosition.x - slider->getAbsolutePosition().x) * 1.0 - widgetWidth / 2 - 1)
		/ (slider->getSize().x - widgetWidth - 2) * (slider->getEnd() - slider->getStart()) + 0.5);
	if (current < slider->getStart())
		current = slider->getStart();
	if (current > slider->getEnd())
		current = slider->getEnd();

	slider->setCurrent(current);
}

DraggingSurrogate * SimpleSkin::SliderWidget::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	DraggingSurrogate * surrogate;
	if ((surrogate = new DraggingSurrogate) == NULL)
		throw EOUTOFMEMORY;
	surrogate->setUnsupportingPointerType(skin->getSelectPointerType());

	return surrogate;
}


SimpleSkin::SplitterWidget::SplitterWidget()
{
	ASSERTION_COBJECT(this);
	splitter = 0;
}

PointerType * SimpleSkin::SplitterWidget::getPointerType(Vector position)
{
	ASSERTION_COBJECT(this);

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();
	SplittingArea * area = (SplittingArea *) getParent();

	if (area->getDirection() == SplittingArea::HORIZONTAL)
		return skin->resize0PointerType;
	else
		return skin->resize90PointerType;
}

bool SimpleSkin::SplitterWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	SplittingArea * area = (SplittingArea *) getParent();
	const ArrayList<ClientArea> & panes = area->getPanes();
	int paneCount = panes.GetCount();

	int iteratePosition = 0;
	if (area->getDirection() == SplittingArea::HORIZONTAL)
		for (int i=0; i<paneCount-1; i++)
		{
			iteratePosition += panes.Get(i)->getSize().x;
			if (point.x >= iteratePosition - 2 && point.x <= iteratePosition + 2)
				return true;
			iteratePosition++;
		}
	else
		for (int i=0; i<paneCount-1; i++)
		{
			iteratePosition += panes.Get(i)->getSize().y;
			if (point.y >= iteratePosition - 2 && point.y <= iteratePosition + 2)
				return true;
			iteratePosition++;
		}

	return false;
}

void SimpleSkin::SplitterWidget::onDragMove(const Vector & startPosition,
	const Vector & lastPosition, const Vector & currentPosition)
{
	ASSERTION_COBJECT(this);

	SplittingArea * area = (SplittingArea *) getParent();
	if (area->getDirection() == SplittingArea::HORIZONTAL)
		area->moveSplitter(splitter, currentPosition.x - lastPosition.x);
	else
		area->moveSplitter(splitter, currentPosition.x - lastPosition.y);
}

DraggingSurrogate * SimpleSkin::SplitterWidget::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);

	SplittingArea * area = (SplittingArea *) getParent();
	const ArrayList<ClientArea> & panes = area->getPanes();
	int paneCount = panes.GetCount();

	Vector relativePosition = startPosition - getOrigin();

	splitter = -1;
	int iteratePosition = 0;
	if (area->getDirection() == SplittingArea::HORIZONTAL)
		for (int i=0; i<paneCount-1; i++)
		{
			iteratePosition += panes.Get(i)->getSize().x;
			if (relativePosition.x >= iteratePosition - 2
				&& relativePosition.x <= iteratePosition + 2)
			{
				splitter = i;
				break;
			}
			iteratePosition++;
		}
	else
		for (int i=0; i<paneCount-1; i++)
		{
			iteratePosition += panes.Get(i)->getSize().y;
			if (relativePosition.y >= iteratePosition - 2
				&& relativePosition.y <= iteratePosition + 2)
			{
				splitter = i;
				break;
			}
			iteratePosition++;
		}

	if (splitter < 0)
		return NULL;

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();

	PointerType * pointerType;
	if (area->getDirection() == SplittingArea::HORIZONTAL)
		pointerType = skin->resize0PointerType;
	else
		pointerType = skin->resize90PointerType;

	DraggingSurrogate * surrogate;
	if ((surrogate = new DraggingSurrogate) == NULL)
		throw EOUTOFMEMORY;
	surrogate->setUnsupportingPointerType(pointerType);

	return surrogate;
}


int SimpleSkin::TabWidget::getTabAt(Vector position)
{
	ASSERTION_COBJECT(this);

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();
	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	TabbingArea * area = (TabbingArea *) getParent();

	const ArrayList<ClientArea> & panes = area->getPanes();
	int panesCount = panes.GetCount();
	int x = 0;
	int y = 0;
	for (int i=0; i<panesCount; i++)
	{
		ClientArea * pane = panes.Get(i);
		if (!pane->isVisible())
			continue;

		const String & name = area->getPaneName(i);
		Vector tabTextSize = skin->tabFont->getSize(name, false, true);
		Vector tabTextSizeWithBorder = Vector(8, 4) + tabTextSize + Vector(8, 4);

		if (x > 0 && area->getSize().x - x < tabTextSizeWithBorder.x)
		{
			y += tabTextSizeWithBorder.y;
			x = 0;
		}

		if (position >= Vector(x, y) && position < Vector(x, y) + tabTextSizeWithBorder)
			return i;

		x += tabTextSizeWithBorder.x;
	}

	return -1;
}

bool SimpleSkin::TabWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);
	return getTabAt(point) >= 0;
}

void SimpleSkin::TabWidget::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	TabbingArea * area = (TabbingArea *) getParent();
	ClientArea * pane = area->getPanes().Get(getTabAt(position));
	if (pane == NULL)
		return;

	area->setFocusPane(pane);
}


SimpleSkin::WindowBorderWidget::WindowBorderWidget()
{
	ASSERTION_COBJECT(this);

	leftBorderHit = false;
	rightBorderHit = false;
	topBorderHit = false;
	bottomBorderHit = false;
}

PointerType * SimpleSkin::WindowBorderWidget::getPointerType(Vector position)
{
	ASSERTION_COBJECT(this);

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();
	Window * window = (Window *) getParent();

	bool leftBorderHit = position.x < 16;
	bool rightBorderHit = position.x >= window->getSize().x - 16;
	bool topBorderHit = position.y < 16;
	bool bottomBorderHit = position.y >= window->getSize().y - 16;

	if (leftBorderHit && topBorderHit || rightBorderHit && bottomBorderHit)
		return skin->resize135PointerType;
	if (rightBorderHit && topBorderHit || leftBorderHit && bottomBorderHit)
		return skin->resize45PointerType;
	if (rightBorderHit || leftBorderHit)
		return skin->resize0PointerType;
	if (topBorderHit || bottomBorderHit)
		return skin->resize90PointerType;

	return NULL;
}

bool SimpleSkin::WindowBorderWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	Window * window = (Window *) getParent();
	return window->isResizable()
		&& (point.x < 4 || point.x >= window->getSize().x - 4
		|| point.y < 4 || point.y >= window->getSize().y - 4);
}

void SimpleSkin::WindowBorderWidget::onDragMove(const Vector & startPosition,
	const Vector & lastPosition, const Vector & currentPosition)
{
	ASSERTION_COBJECT(this);

	Window * window = (Window *) getParent();
	if (!window->isActive())
		return;

	Vector moveDelta = currentPosition - lastPosition;
	Vector positionDelta = Vector();
	Vector sizeDelta = Vector();

	if (leftBorderHit)
	{
		positionDelta.x += moveDelta.x;
		sizeDelta.x -= moveDelta.x;
	}
	else if (rightBorderHit)
		sizeDelta.x += moveDelta.x;

	if (topBorderHit)
	{
		positionDelta.y += moveDelta.y;
		sizeDelta.y -= moveDelta.y;
	}
	else if (bottomBorderHit)
		sizeDelta.y += moveDelta.y;

	window->moveRelative(positionDelta);
	window->resize(window->getSize() + sizeDelta);
}

DraggingSurrogate * SimpleSkin::WindowBorderWidget::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);

	Vector relativePosition = startPosition - getOrigin();

	Window * window = (Window *) getParent();
	leftBorderHit = relativePosition.x < 16;
	rightBorderHit = relativePosition.x >= window->getSize().x - 16;
	topBorderHit = relativePosition.y < 16;
	bottomBorderHit = relativePosition.y >= window->getSize().y - 16;

	SimpleSkin * skin = (SimpleSkin *) Desktop::getInstance()->getSkin();

	PointerType * pointerType;
	if (leftBorderHit && topBorderHit || rightBorderHit && bottomBorderHit)
		pointerType = skin->resize135PointerType;
	else if (rightBorderHit && topBorderHit || leftBorderHit && bottomBorderHit)
		pointerType = skin->resize45PointerType;
	else if (rightBorderHit || leftBorderHit)
		pointerType = skin->resize0PointerType;
	else if (topBorderHit || bottomBorderHit)
		pointerType = skin->resize90PointerType;
	else
		return NULL;

	DraggingSurrogate * surrogate;
	if ((surrogate = new DraggingSurrogate) == NULL)
		throw EOUTOFMEMORY;
	surrogate->setUnsupportingPointerType(pointerType);

	return surrogate;
}


bool SimpleSkin::WindowCloseWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	Window * window = (Window *) getParent();
	return window->isTitled() && window->isClosable()
		&& point >= Vector(window->getSize().x - 18, 2)
		&& point <= Vector(window->getSize().x - 4, 16);
}

void SimpleSkin::WindowCloseWidget::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Window * window = (Window *) getParent();
	if (!window->isActive())
		return;

	window->onCloseWidgetClicked();
}


bool SimpleSkin::WindowMaximizeWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	Window * window = (Window *) getParent();
	if (window->isTitled() && window->isMaximizable())
		if (window->isClosable())
			return point >= Vector(window->getSize().x - 37, 2)
				&& point <= Vector(window->getSize().x - 23, 16);
		else
			return point >= Vector(window->getSize().x - 18, 2)
				&& point <= Vector(window->getSize().x - 4, 16);

	return false;
}

void SimpleSkin::WindowMaximizeWidget::onClick(Vector position,
	bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Window * window = (Window *) getParent();
	if (!window->isActive())
		return;

	if (window->isMaximized())
		window->restore();
	else
		window->maximize();
}


SimpleSkin::WindowTitleWidget::WindowTitleWidget()
{
	ASSERTION_COBJECT(this);
	checkingHit = false;
}

bool SimpleSkin::WindowTitleWidget::isHitAt(Vector point)
{
	ASSERTION_COBJECT(this);

	if (checkingHit)
		return false;

	Window * window = (Window *) getParent();

	if (!window->isTitled() || !window->isMovable())
		return false;

	if (point >= Vector(1, 1) && point <= Vector(window->getSize().x - 2, 18))
		return true;

	checkingHit = true;
	EventTarget * target = window->getEventTargetAt(point);
	checkingHit = false;

	if (dynamic_cast<FieldsArea *>(target) != NULL)
		return true;
	else if (dynamic_cast<Field *>(target) != NULL)
	{
		Field * field = (Field *) target;
		return field->isLikeTitle(point - field->getPositionOnWindow());
	}

	return false;
}

void SimpleSkin::WindowTitleWidget::onDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Window * window = (Window *) getParent();
	if (!window->isActive() || !window->isMaximizable() || !window->isTitled())
		return;

	if (position >= Vector(1, 2) && position <= Vector(window->getSize().x - 2, 16))
		if (window->isMaximized())
			window->restore();
		else
			window->maximize();
}

void SimpleSkin::WindowTitleWidget::onDragMove(const Vector & startPosition,
	const Vector & lastPosition, const Vector & currentPosition)
{
	ASSERTION_COBJECT(this);

	Window * window = (Window *) getParent();
	if (!window->isActive())
		return;

	if (!window->isMaximized())
		window->moveRelative(currentPosition - lastPosition);
}

DraggingSurrogate * SimpleSkin::WindowTitleWidget::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	DraggingSurrogate * surrogate;
	if ((surrogate = new DraggingSurrogate) == NULL)
		throw EOUTOFMEMORY;
	surrogate->setUnsupportingPointerType(skin->getSelectPointerType());

	return surrogate;
}
