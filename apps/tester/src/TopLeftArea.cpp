/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


Button * TopLeftArea::getButton1()
{
	ASSERTION_COBJECT(this);
	return button1;
}

Button * TopLeftArea::getButton4()
{
	ASSERTION_COBJECT(this);
	return button4;
}

Button * TopLeftArea::getButton5()
{
	ASSERTION_COBJECT(this);
	return button5;
}

Button * TopLeftArea::getButton6()
{
	ASSERTION_COBJECT(this);
	return button6;
}

Button * TopLeftArea::getButton7()
{
	ASSERTION_COBJECT(this);
	return button7;
}

Button * TopLeftArea::getButton8()
{
	ASSERTION_COBJECT(this);
	return button8;
}

CheckBox * TopLeftArea::getCheckBox1()
{
	ASSERTION_COBJECT(this);
	return checkBox1;
}

void TopLeftArea::initialize()
{
	ASSERTION_COBJECT(this);

	GLAZIERY_FIELDSAREALAYOUTDATA(this, 2)

	GLAZIERY_GROUPFIELD(groupField1, this, NULL)
	GLAZIERY_FIELDLAYOUTDATA(groupField1, FILL, FILL, 2)
	GLAZIERY_GROUPFIELDLAYOUTDATA(groupField1, 2)

	GLAZIERY_BUTTON(button1, this, groupField1, STRING_TOPLEFT_BUTTON1, NULL)
	this->button1 = button1;
	GLAZIERY_FIELDLAYOUTDATA(button1, FILL, CENTER, 1)

	GLAZIERY_BUTTON(button2, this, groupField1, STRING_TOPLEFT_BUTTON2, NULL)
	GLAZIERY_FIELDLAYOUTDATA(button2, FILL, CENTER, 1)

	ButtonNewWindowListener * buttonNewWindowListener;
	if ((buttonNewWindowListener = new ButtonNewWindowListener) == NULL)
		throw EOUTOFMEMORY;
	button2->appendListener(buttonNewWindowListener);

	GLAZIERY_BUTTON_I(button3, this, NULL, "", "images\\Daniel.png")

	ButtonImageListener * buttonImageListener;
	if ((buttonImageListener = new ButtonImageListener) == NULL)
		throw EOUTOFMEMORY;
	button3->appendListener(buttonImageListener);

	GLAZIERY_GROUPFIELD(groupField2, this, NULL)
	GLAZIERY_FIELDLAYOUTDATA(groupField2, FILL, FILL, 1)

	GLAZIERY_LABEL(label1, this, groupField2, STRING_TOPLEFT_LABEL1)
	GLAZIERY_FIELDLAYOUTDATA(label1, START, CENTER, 1)
	GLAZIERY_CHECKBOX(checkBox1, this, groupField2, STRING_TOPLEFT_CHECKBOX1, UNCHECKED)
	this->checkBox1 = checkBox1;
	checkBox1->setTriStateSupported(true);
	GLAZIERY_FIELDLAYOUTDATA(checkBox1, START, CENTER, 1)

	GLAZIERY_GROUPFIELD(groupField3, this, groupField2)
	GLAZIERY_FIELDLAYOUTDATA(groupField3, FILL, FILL, 1)
	GLAZIERY_GROUPFIELDLAYOUTDATA(groupField3, 3)

	GLAZIERY_BUTTON(button4, this, groupField3, STRING_TOPLEFT_BUTTON4, NULL)
	this->button4 = button4;
	GLAZIERY_FIELDLAYOUTDATA(button4, FILL, CENTER, 1)

	GLAZIERY_BUTTON(button5, this, groupField3, STRING_TOPLEFT_BUTTON5, NULL)
	this->button5 = button5;
	GLAZIERY_FIELDLAYOUTDATA(button2, FILL, CENTER, 1)

	GLAZIERY_BUTTON(button6, this, groupField3, STRING_TOPLEFT_BUTTON6, NULL)
	this->button6 = button6;
	GLAZIERY_FIELDLAYOUTDATA(button2, FILL, CENTER, 1)

	GLAZIERY_GROUPFIELD(groupField4, this, groupField2)
	GLAZIERY_FIELDLAYOUTDATA(groupField4, FILL, FILL, 1)
	GLAZIERY_GROUPFIELDLAYOUTDATA(groupField4, 2)

	GLAZIERY_BUTTON(button7, this, groupField4, STRING_TOPLEFT_BUTTON7, NULL)
	this->button7 = button7;
	GLAZIERY_FIELDLAYOUTDATA(button7, FILL, CENTER, 1)

	GLAZIERY_BUTTON(button8, this, groupField4, STRING_TOPLEFT_BUTTON8, NULL)
	this->button8 = button8;
	GLAZIERY_FIELDLAYOUTDATA(button8, FILL, CENTER, 1)

	setAutoLayouting(true, false);
}


TopLeftArea::ButtonImageListener::ButtonImageListener()
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	danielImage = adapter->getImage("images\\Daniel.png", false);
	waterImage = adapter->getImage("images\\Water.png", false);
}

void TopLeftArea::ButtonImageListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);

	MainScene * scene = (MainScene *) Desktop::getInstance()->getScene(MainScene::ID);

	if (button->getImage() == danielImage)
		button->setImage(waterImage);
	else
		button->setImage(danielImage);
}


void TopLeftArea::ButtonNewWindowListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);

	MainScene * scene = (MainScene *) Desktop::getInstance()->getScene(MainScene::ID);
	scene->createNewWindow();
}
