/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


ProgressBar * BottomLeftArea::getProgressBar1()
{
	ASSERTION_COBJECT(this);
	return progressBar1;
}

void BottomLeftArea::initialize()
{
	ASSERTION_COBJECT(this);

	GLAZIERY_FIELDSAREALAYOUTDATA(this, 2)

	GLAZIERY_LABEL(label1, this, NULL, STRING_BOTTOMLEFT_LABEL1)
	GLAZIERY_FIELDLAYOUTDATA(label1, END, CENTER, 1)

	GLAZIERY_GROUPFIELD(groupField1, this, NULL)
	GLAZIERY_FIELDLAYOUTDATA(groupField1, START, CENTER, 1)
	GLAZIERY_GROUPFIELDLAYOUTDATA(groupField1, 2)

	GLAZIERY_RADIOGROUP(group1, this)
	GLAZIERY_RADIOBOX(radioBox1, this, groupField1, STRING_BOTTOMLEFT_RADIOBOX1, CheckBox::UNCHECKED, group1)
	GLAZIERY_FIELDLAYOUTDATA(radioBox1, START, CENTER, 1)
	GLAZIERY_RADIOBOX(radioBox2, this, groupField1, STRING_BOTTOMLEFT_RADIOBOX2, CheckBox::UNCHECKED, group1)
	GLAZIERY_FIELDLAYOUTDATA(radioBox2, START, CENTER, 1)

	GLAZIERY_LABEL(label2, this, NULL, STRING_BOTTOMLEFT_LABEL2)
	GLAZIERY_FIELDLAYOUTDATA(label2, END, CENTER, 1)
	GLAZIERY_KEYSTROKEFIELD(keyStrokeField1, this, NULL)
	GLAZIERY_FIELDLAYOUTDATA(keyStrokeField1, FILL, CENTER, 1)

	GLAZIERY_TEXTFIELD(textField1, this, NULL, false)
	GLAZIERY_FIELDLAYOUTDATA(textField1, FILL, CENTER, 2)
	GLAZIERY_TEXTFIELD(textField2, this, NULL, true)
	GLAZIERY_FIELDLAYOUTDATA(textField2, FILL, FILL, 2)

	GLAZIERY_LABEL(label3, this, NULL, STRING_BOTTOMLEFT_LABEL3)
	GLAZIERY_FIELDLAYOUTDATA(label3, END, CENTER, 1)
	GLAZIERY_SELECTOR(selector1, this, NULL)
	GLAZIERY_FIELDLAYOUTDATA(selector1, FILL, CENTER, 1)
	GLAZIERY_SELECTOR_IMAGETEXTOPTION_I(maleOption, maleImageElement, maleTextElement, selector1,
		"images\\Male.png", "Daniel", false)
	GLAZIERY_SELECTOR_IMAGETEXTOPTION_I(femaleOption1, femaleImageElement, femaleTextElement, selector1,
		"images\\Female.png", "Anna", true)
	GLAZIERY_SELECTOR_TEXTOPTION(annaOption, annaTextElement, selector1, "Anna", false)
	GLAZIERY_SELECTOR_IMAGEOPTION_I(female2Option, female2ImageElement, selector1, "images\\Female.png", false)

	GLAZIERY_LABEL(label4, this, NULL, STRING_BOTTOMLEFT_LABEL4)
	GLAZIERY_FIELDLAYOUTDATA(label4, END, CENTER, 1)
	GLAZIERY_PROGRESSBAR(progressBar1, this, NULL, 300, 2000, 0)
	GLAZIERY_FIELDLAYOUTDATA(progressBar1, FILL, CENTER, 1)

	this->progressBar1 = progressBar1;

	textField1->setText("A single-line text field");
	textField2->setText("A multi-line\ntext field");

	MaskedListener * maskedListener;
	if ((maskedListener = new MaskedListener(textField1)) == NULL)
		throw EOUTOFMEMORY;
	radioBox2->appendListener(maskedListener);

	setAutoLayouting(true, false);
}


BottomLeftArea::MaskedListener::MaskedListener(TextField * textField)
{
	ASSERTION_COBJECT(this);
	this->textField = textField;
}

void BottomLeftArea::MaskedListener::onStateChanged(RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);
	textField->setMasked(radioBox->isChecked());
}
