/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


InputDialog::InputDialog(const String & message, String & input)
	: input(input)
{
	ASSERTION_COBJECT(this);

	this->message = message;
	inputField = NULL;
}

InputDialog::~InputDialog()
{
	ASSERTION_COBJECT(this);
}

InputDialog * InputDialog::createDialog(const String & title, const String & message,
	String & input, Dialog::Buttons buttons)
{
	InputDialog * dialog;
	if ((dialog = new InputDialog(message, input)) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addWindow(dialog);

	dialog->setTitle(title);
	dialog->setButtons(buttons);

	return dialog;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(InputDialog, Dialog);
#endif

VetoMode InputDialog::onDialogEnd(Buttons button)
{
	ASSERTION_COBJECT(this);

	input = inputField->getText();
	return Dialog::onDialogEnd(button);
}

void InputDialog::setupContent(FieldsArea * area, GroupField * contentGroup)
{
	ASSERTION_COBJECT(this);

	GLAZIERY_GROUPFIELDLAYOUTDATA(contentGroup, 1)

	GLAZIERY_LABEL(messageLabel, area, contentGroup, message);

	GLAZIERY_TEXTFIELD(inputField, area, contentGroup, false)
	GLAZIERY_FIELDLAYOUTDATA(inputField, FILL, START, 1)
	this->inputField = inputField;
}

Dialog::Buttons InputDialog::showDialog(const String & title, const String & message,
	String & input, Dialog::Buttons buttons)
{
	Dialog * dialog = InputDialog::createDialog(title, message, input, buttons);
	return dialog->showModally();
}
