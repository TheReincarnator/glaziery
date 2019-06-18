/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


PasswordDialog::PasswordDialog(const String & message)
{
	ASSERTION_COBJECT(this);

	this->message = message;
	password = NULL;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(PasswordDialog, Dialog);
#endif

VetoMode PasswordDialog::onDialogEnd(Buttons button)
{
	ASSERTION_COBJECT(this);

	if (button == BUTTON_OK)
	{
		if (!password->validateRequired()) return VETO;
	}

	return NO_VETO;
}

void PasswordDialog::setupContent(FieldsArea * area, GroupField * contentGroup)
{
	ASSERTION_COBJECT(this);

	contentGroup->getGroupLayoutData()->setColumns(1);

	GLAZIERY_LABEL(messageLabel, area, contentGroup, message)

	GLAZIERY_GROUPFIELD(passwordGroup, area, contentGroup)
	GLAZIERY_GROUPFIELDLAYOUTDATA(passwordGroup, 0)
	GLAZIERY_FIELDLAYOUTDATA_MP(passwordGroup, 8, 8, 8, 8, 32, 0, 0, 0, START, START, 1)
	GLAZIERY_LABEL(label, area, passwordGroup, STRING_PASSWORDDIALOG_PASSWORDLABEL)
	GLAZIERY_TEXTFIELD_S(password, area, passwordGroup, 256, -1, false)
	this->password = password;
	password->setMasked(true);
}

String PasswordDialog::showDialog(const String & title, const String & message)
{
	PasswordDialog * dialog;
	if ((dialog = new PasswordDialog(message)) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addWindow(dialog);

	dialog->setTitle(title);
	dialog->setButtons(Dialog::BUTTONS_OKCANCEL);
	dialog->setMaximizable(false);
	dialog->setResizable(false);

	if (((Dialog *) dialog)->showModally() != Dialog::BUTTON_OK)
		return "";

	return dialog->password->getText();
}
