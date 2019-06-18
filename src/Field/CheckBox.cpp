/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


CheckBox::CheckBox()
{
	ASSERTION_COBJECT(this);

	state = UNCHECKED;
	text = "";
	triStateSupported = false;

	Desktop::getInstance()->getSkin()->setupCheckBox(this);
	resizeInternal(Desktop::getInstance()->getSkin()->getCheckBoxSize(this), true);
}

CheckBox::~CheckBox()
{
	ASSERTION_COBJECT(this);
	removeHotKey(ClientArea::findHotKey(text));
}

void CheckBox::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawCheckBox(context, this);
}

CheckBox::State CheckBox::getState()
{
	ASSERTION_COBJECT(this);
	return state;
}

String * CheckBox::getTextAttribute()
{
	ASSERTION_COBJECT(this);
	return &text;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(CheckBox, InputField);
#endif

bool CheckBox::isChecked()
{
	ASSERTION_COBJECT(this);
	return state == CHECKED;
}

bool CheckBox::isTriStateSupported()
{
	ASSERTION_COBJECT(this);
	return triStateSupported;
}

bool CheckBox::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = InputField::onCharacter(character, option1, option2);

	if (character == ' ' && isActiveIncludingAncestors() && isEditable())
	{
		toggle();
		return true;
	}

	return consumed;
}

void CheckBox::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (isActiveIncludingAncestors() && isEditable())
		toggle();
}

void CheckBox::onMatchedHotKey(bool direct)
{
	ASSERTION_COBJECT(this);

	if (isActiveIncludingAncestors() && isEditable())
		toggle();
}

bool CheckBox::setChecked(bool checked)
{
	ASSERTION_COBJECT(this);
	return setState(checked ? CHECKED : UNCHECKED);
}

bool CheckBox::setState(State state)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	int i;
	for (i=0; i<listenersCount; i++)
	{
		CheckBox::Listener * checkBoxListener
			= dynamic_cast<CheckBox::Listener *>(listeners.Get(i));
		if (checkBoxListener != NULL)
			if (checkBoxListener->onStateChanging(this, state) == VETO)
				return false;
	}

	this->state = state;
	invalidate();

	for (i=0; i<listenersCount; i++)
	{
		CheckBox::Listener * checkBoxListener
			= dynamic_cast<CheckBox::Listener *>(listeners.Get(i));
		if (checkBoxListener != NULL)
			checkBoxListener->onStateChanged(this);
	}

	return true;
}

void CheckBox::setText(const String & text)
{
	ASSERTION_COBJECT(this);

	if (this->text == text)
		return;

	Field::setText(text);

	resizeInternal(Desktop::getInstance()->getSkin()->getCheckBoxSize(this), true);
}

void CheckBox::setFont(Font * font)
{
	ASSERTION_COBJECT(this);

	if (getFont() == font)
		return;

	Field::setFont(font);
	resizeInternal(Desktop::getInstance()->getSkin()->getCheckBoxSize(this), true);
}

void CheckBox::setTriStateSupported(bool triStateSupported)
{
	ASSERTION_COBJECT(this);
	this->triStateSupported = triStateSupported;
}

void CheckBox::toggle()
{
	ASSERTION_COBJECT(this);

	if (triStateSupported)
		if (state == CHECKED)
			setState(TRI_STATE);
		else if (state == UNCHECKED)
			setState(CHECKED);
		else
			setState(UNCHECKED);
	else
		setState(state == CHECKED ? UNCHECKED : CHECKED);
}

String CheckBox::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("CheckBox(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}


CheckBox::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void CheckBox::Listener::onStateChanged(CheckBox * checkBox)
{
	ASSERTION_COBJECT(this);
}

VetoMode CheckBox::Listener::onStateChanging(CheckBox * checkBox, State newState)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}
