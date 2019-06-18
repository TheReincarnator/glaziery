/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


RadioBox::RadioBox()
{
	ASSERTION_COBJECT(this);

	checked = false;
	radioGroup = NULL;
	text = "";

	Desktop::getInstance()->getSkin()->setupRadioBox(this);
	resizeInternal(Desktop::getInstance()->getSkin()->getRadioBoxSize(this), true);
}

RadioBox::~RadioBox()
{
	ASSERTION_COBJECT(this);
	removeHotKey(ClientArea::findHotKey(text));
}

void RadioBox::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawRadioBox(context, this);
}

int RadioBox::getPositionInGroup()
{
	ASSERTION_COBJECT(this);

	if (radioGroup == NULL)
		return -1;

	const ArrayList<RadioBox> & radioBoxes = radioGroup->getRadioBoxes();
	for (int i=0; i<radioBoxes.GetCount(); i++)
		if (radioBoxes.Get(i) == this)
			return i;

	return -1;
}

RadioGroup * RadioBox::getRadioGroup()
{
	ASSERTION_COBJECT(this);
	return radioGroup;
}

String * RadioBox::getTextAttribute()
{
	ASSERTION_COBJECT(this);
	return &text;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(RadioBox, InputField);
#endif

bool RadioBox::isChecked()
{
	ASSERTION_COBJECT(this);
	return checked;
}

bool RadioBox::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = InputField::onCharacter(character, option1, option2);

	if (character == ' '  && isActiveIncludingAncestors() && isEditable())
	{
		setChecked(true);
		return true;
	}

	return consumed;
}

void RadioBox::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (isActiveIncludingAncestors() && isEditable())
		setChecked(true);
}

void RadioBox::onMatchedHotKey(bool direct)
{
	ASSERTION_COBJECT(this);

	if (isActiveIncludingAncestors() && isEditable())
		setChecked(true);
}

bool RadioBox::onMoveDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveDown(option1, option2);

	selectNextRadioBox();
	return true;
}

bool RadioBox::onMoveLeft(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveLeft(option1, option2);

	selectPreviousRadioBox();
	return true;
}

bool RadioBox::onMoveRight(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveRight(option1, option2);

	selectNextRadioBox();
	return true;
}

bool RadioBox::onMoveUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveUp(option1, option2);

	selectPreviousRadioBox();
	return true;
}

void RadioBox::selectNextRadioBox()
{
	ASSERTION_COBJECT(this);

	int radioBoxNo = getPositionInGroup();
	if (radioBoxNo < 0)
		return;

	const ArrayList<RadioBox> & radioBoxes = radioGroup->getRadioBoxes();
	int groupBoxesCount = radioBoxes.GetCount();
	RadioBox * targetBox = radioBoxes.Get((radioBoxNo + 1) % groupBoxesCount);
	targetBox->setChecked(true);
	targetBox->setFocus();
}

void RadioBox::selectPreviousRadioBox()
{
	ASSERTION_COBJECT(this);

	int radioBoxNo = getPositionInGroup();
	if (radioBoxNo < 0)
		return;

	const ArrayList<RadioBox> & radioBoxes = radioGroup->getRadioBoxes();
	int groupBoxesCount = radioBoxes.GetCount();
	RadioBox * targetBox = radioBoxes.Get((radioBoxNo + groupBoxesCount - 1) % groupBoxesCount);
	targetBox->setChecked(true);
	targetBox->setFocus();
}

void RadioBox::setChecked(bool checked)
{
	ASSERTION_COBJECT(this);

	if (this->checked == checked)
		return;

	this->checked = checked;
	invalidate();

	ArrayList<RadioBox> notifyRadioBoxes;
	notifyRadioBoxes.Append(this);

	if (checked && radioGroup != NULL)
	{
		const ArrayList<RadioBox> & radioBoxes = radioGroup->getRadioBoxes();
		int boxCount = radioBoxes.GetCount();
		for (int i=0; i<boxCount; i++)
		{
			RadioBox * radioBox = radioBoxes.Get(i);
			if (radioBox != this && radioBox->isChecked())
			{
				radioBox->checked = false;
				radioBox->invalidate();
				notifyRadioBoxes.Append(radioBox);
			}
		}
	}

	for (int fieldNo = 0; fieldNo < notifyRadioBoxes.GetCount(); fieldNo++)
	{
		RadioBox * radioBox = notifyRadioBoxes.Get(fieldNo);
		int listenersCount = radioBox->getListeners().GetCount();
		for (int i=0; i<listenersCount; i++)
		{
			RadioBox::Listener * radioBoxListener
				= dynamic_cast<RadioBox::Listener *>(radioBox->getListeners().Get(i));
			if (radioBoxListener != NULL)
				radioBoxListener->onStateChanged(radioBox);
		}
	}

	notifyRadioBoxes.UnlinkAll();
}

void RadioBox::setFont(Font * font)
{
	ASSERTION_COBJECT(this);

	if (getFont() == font)
		return;

	Field::setFont(font);
	resizeInternal(Desktop::getInstance()->getSkin()->getRadioBoxSize(this), true);
}

void RadioBox::setText(const String & text)
{
	ASSERTION_COBJECT(this);

	if (this->text == text)
		return;

	Field::setText(text);

	resizeInternal(Desktop::getInstance()->getSkin()->getRadioBoxSize(this), true);
}

void RadioBox::setRadioGroup(RadioGroup * radioGroup)
{
	ASSERTION_COBJECT(this);
	this->radioGroup = radioGroup;
}

String RadioBox::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("RadioBox(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}


RadioBox::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void RadioBox::Listener::onStateChanged(RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);
}
