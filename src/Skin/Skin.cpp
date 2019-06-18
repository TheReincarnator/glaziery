/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Skin::~Skin()
{
	ASSERTION_COBJECT(this);
}

void Skin::addClickVisualizationEffect()
{
	ASSERTION_COBJECT(this);
}

void Skin::addKeyVisualizationEffect(const String & keyName, bool atFocus)
{
	ASSERTION_COBJECT(this);
}

void Skin::cancelScrollingAreaScrollEffects(ScrollingArea * area)
{
	ASSERTION_COBJECT(this);
}

void Skin::cancelScrollingFieldScrollEffects(ScrollingField * field)
{
	ASSERTION_COBJECT(this);
}

void Skin::Configure(ConfigSection * Section, bool write)
{
	ASSERTION_COBJECT(this);
}

void Skin::drawClientAreaPopup(DrawingContext & context, ClientAreaPopup * popup)
{
	ASSERTION_COBJECT(this);
}

void Skin::fillScrollerInfo(ScrollerInfo & info, ScrollingArea * area)
{
	ASSERTION_COBJECT(this);
	info.fillInfo(area);
}

void Skin::fillScrollerInfo(ScrollerInfo & info, ScrollingField * field)
{
	ASSERTION_COBJECT(this);
	info.fillInfo(field);
}

Vector Skin::getScrollingAreaScrollPosition(ScrollingArea * area)
{
	ASSERTION_COBJECT(this);
	return area->getScrollPositionNow();
}

Vector Skin::getScrollingFieldScrollPosition(ScrollingField * field)
{
	ASSERTION_COBJECT(this);
	return field->getScrollPositionNow();
}

unsigned long Skin::getWindowModulationColor(Window * window)
{
	ASSERTION_COBJECT(this);
	return GL_WHITE_A(window->getAlpha());
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Skin, CObject);
#endif

void Skin::initialize()
{
	ASSERTION_COBJECT(this);
}

void Skin::setScrollingAreaScrollPosition(ScrollingArea * area, Vector scrollPosition)
{
	ASSERTION_COBJECT(this);
	area->scrollToNow(scrollPosition);
}

void Skin::setScrollingFieldScrollPosition(ScrollingField * field, Vector scrollPosition)
{
	ASSERTION_COBJECT(this);
	field->scrollToNow(scrollPosition);
}

void Skin::setupBalloonPopup(BalloonPopup * popup)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupButton(Button * button)
{
	ASSERTION_COBJECT(this);
	button->resize(getButtonSize(button));
}

void Skin::setupCheckBox(CheckBox * checkBox)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupClientArea(ClientArea * area)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupClientAreaPopup(ClientAreaPopup * popup)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupDesktop()
{
	ASSERTION_COBJECT(this);
}

void Skin::setupDialog(Dialog * dialog)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupField(Field * field)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupFieldsArea(FieldsArea * area)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupGroupField(GroupField * groupField)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupImageField(ImageField * field)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupKeyStrokeField(KeyStrokeField * field)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupLabel(Label * label)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupListField(ListField * field)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupMenuPopup(MenuPopup * menuPopup)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupModalWindow(ModalWindow * window)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupProgressBar(ProgressBar * progressBar)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupRadioBox(RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupScrollingArea(ScrollingArea * area)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupScrollingField(ScrollingField * field)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupSelector(Selector * selector)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupSelectorPopup(Selector * selector, SingleFieldPopup * popup)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupSingleFieldArea(SingleFieldArea * area)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupSingleFieldPopup(SingleFieldPopup * popup)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupSlider(Slider * slider)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupSplittingArea(SplittingArea * area)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupTabbingArea(TabbingArea * area)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupTextElementEditPopup(Popup * popup, TextField * textField,
	TextElement * element, Vector elementPosition, Vector elementSize)
{
	ASSERTION_COBJECT(this);

	popup->moveTo(elementPosition);
	textField->resize(elementSize);
}

void Skin::setupTextField(TextField * field)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupTreeField(TreeField * field)
{
	ASSERTION_COBJECT(this);
}

void Skin::setupWindow(Window * window)
{
	ASSERTION_COBJECT(this);
}

void Skin::shutDown()
{
	ASSERTION_COBJECT(this);
}

void Skin::update()
{
	ASSERTION_COBJECT(this);
}
