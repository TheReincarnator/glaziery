/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


CheckBox * TopMiddleArea::getCheckBox1()
{
	ASSERTION_COBJECT(this);
	return checkBox1;
}

Slider * TopMiddleArea::getSlider1()
{
	ASSERTION_COBJECT(this);
	return slider1;
}

Slider * TopMiddleArea::getSlider2()
{
	ASSERTION_COBJECT(this);
	return slider2;
}

void TopMiddleArea::initialize(Window * window)
{
	ASSERTION_COBJECT(this);

	GLAZIERY_FIELDSAREALAYOUTDATA(this, 2)

	GLAZIERY_LABEL(label1, this, NULL, STRING_TOPMIDDLE_LABEL1)
	GLAZIERY_FIELDLAYOUTDATA(label1, START, CENTER, 1)
	GLAZIERY_CHECKBOX(checkBox1, this, NULL, STRING_TOPMIDDLE_CHECKBOX1, UNCHECKED)
	checkBox1->setActive(false);
	this->checkBox1 = checkBox1;
	GLAZIERY_FIELDLAYOUTDATA(checkBox1, START, CENTER, 1)

	GLAZIERY_GROUPFIELD_B(groupField1, this, NULL, STRING_TOPMIDDLE_GROUPFIELD1)
	GLAZIERY_FIELDLAYOUTDATA(groupField1, FILL, FILL, 1)
	GLAZIERY_GROUPFIELDLAYOUTDATA(groupField1, 2)

	GLAZIERY_LABEL(label2, this, groupField1, STRING_TOPMIDDLE_LABEL2)
	GLAZIERY_FIELDLAYOUTDATA(label2, END, CENTER, 2)
	GLAZIERY_SLIDER(slider1, this, groupField1, 0, 255, 224)
	this->slider1 = slider1;
	slider1->setMinorStepUnits(5);
	slider1->setMajorStepUnits(50);
	slider1->setCurrent(window->getFocusAlpha());
	GLAZIERY_FIELDLAYOUTDATA(slider1, FILL, CENTER, 2)

	GLAZIERY_LABEL(label3, this, groupField1, STRING_TOPMIDDLE_LABEL3)
	GLAZIERY_FIELDLAYOUTDATA(label3, END, CENTER, 2)
	GLAZIERY_SLIDER(slider2, this, groupField1, 300, 2000, 300)
	this->slider2 = slider2;
	slider2->setMinorStepUnits(30);
	slider2->setMajorStepUnits(300);
	GLAZIERY_FIELDLAYOUTDATA(slider2, FILL, CENTER, 2)

	GLAZIERY_LABEL(label4, this, groupField1, STRING_TOPMIDDLE_LABEL4)
	GLAZIERY_FIELDLAYOUTDATA(label4, END, CENTER, 2)
	GLAZIERY_SLIDER(slider3, this, groupField1, 0, 0, 0)
	this->slider3 = slider3;
	GLAZIERY_SLIDER_SL_CREATE(slider3StringList)
	GLAZIERY_SLIDER_SL_ITEM(slider3StringList, STRING_TOPMIDDLE_STRINGLISTITEM1)
	GLAZIERY_SLIDER_SL_ITEM(slider3StringList, STRING_TOPMIDDLE_STRINGLISTITEM2)
	GLAZIERY_SLIDER_SL_ITEM(slider3StringList, STRING_TOPMIDDLE_STRINGLISTITEM3)
	GLAZIERY_SLIDER_SL_ITEM(slider3StringList, STRING_TOPMIDDLE_STRINGLISTITEM4)
	GLAZIERY_SLIDER_SL_ITEM(slider3StringList, STRING_TOPMIDDLE_STRINGLISTITEM5)
	GLAZIERY_FIELDLAYOUTDATA(slider3, FILL, CENTER, 2)

	GLAZIERY_GROUPFIELD(groupField2, this, NULL)
	GLAZIERY_FIELDLAYOUTDATA(groupField2, FILL, FILL, 1)
	GLAZIERY_GROUPFIELDLAYOUTDATA(groupField2, 1)

	GLAZIERY_BUTTON(loadFileButton, this, groupField2, STRING_TOPMIDDLE_LOADFILE, NULL)
	loadFileButton->addPressedFunction(onLoadFilePressed);

	GLAZIERY_BUTTON(saveFileButton, this, groupField2, STRING_TOPMIDDLE_SAVEFILE, NULL)
	saveFileButton->addPressedFunction(onSaveFilePressed);

	GLAZIERY_BUTTON(directoryButton, this, groupField2, STRING_TOPMIDDLE_DIRECTORY, NULL)
	directoryButton->addPressedFunction(onDirectoryPressed);

	GLAZIERY_LABEL(slider3Label, this, groupField2, "dummy")
	GLAZIERY_SLIDER_SL_ATTACH(slider3, slider3StringList, slider3Label)

	setAutoLayouting(true, false);
}

void TopMiddleArea::onDirectoryPressed(Button * button)
{
	FileDialog * dialog = FileDialog::createDirectoryDialog();
	dialog->showModally();
}

void TopMiddleArea::onLoadFilePressed(Button * button)
{
	FileDialog * dialog = FileDialog::createLoadFileDialog();
	dialog->showModally();
}

void TopMiddleArea::onSaveFilePressed(Button * button)
{
	FileDialog * dialog = FileDialog::createSaveFileDialog();
	dialog->showModally();
}
