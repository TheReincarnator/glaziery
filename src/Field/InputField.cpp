/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


InputField::InputField()
{
	ASSERTION_COBJECT(this);

	editable = true;

	setLikeTitle(false);
}

InputField::~InputField()
{
	ASSERTION_COBJECT(this);
}

bool InputField::consumingEnter()
{
	ASSERTION_COBJECT(this);
	return false;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(InputField, Field);
#endif

bool InputField::isEditable()
{
	ASSERTION_COBJECT(this);
	return editable;
}

void InputField::onContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (isAcceptingPressesToFocus() && isActiveIncludingAncestors())
	{
		Window * window = getWindow();
		if (window != NULL)
			window->bringToFront();
	
		setFocus();
	}

	Field::onContextClick(position, option1, option2);
}

bool InputField::onEnter(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = Field::onEnter(option1, option2);

	if (!consumingEnter())
	{
		FieldsArea * fieldsArea = getFieldsArea();
		if (fieldsArea != NULL)
		{
			Button * defaultButton = fieldsArea->getDefaultButton();
			if (defaultButton != NULL)
				defaultButton->press();

			consumed = true;
		}
	}

	return consumed;
}

void InputField::onMatchedHotKey(bool direct)
{
	ASSERTION_COBJECT(this);

	if (isActiveIncludingAncestors())
		setFocus();
}

void InputField::setActive(bool active)
{
	ASSERTION_COBJECT(this);

	Field::setActive(active);

	if (!active && hasFocus())
	{
		FieldsArea * fieldsArea = getFieldsArea();
		if (fieldsArea != NULL)
			fieldsArea->setFocusToNextField();
	}
}

void InputField::setEditable(bool editable)
{
	ASSERTION_COBJECT(this);

	this->editable = editable;
	invalidate();
}

void InputField::setFocus()
{
	ASSERTION_COBJECT(this);

	FieldsArea * fieldsArea = getFieldsArea();
	if (fieldsArea != NULL)
		fieldsArea->setFocusField(this);
}
