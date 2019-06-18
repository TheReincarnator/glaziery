/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


MessageDialog::MessageDialog(const String & message, Image * iconImage)
{
	ASSERTION_COBJECT(this);

	this->iconImage = iconImage;
	this->message = message;
	rememberCheckbox = NULL;
}

MessageDialog::~MessageDialog()
{
	ASSERTION_COBJECT(this);
}

MessageDialog * MessageDialog::createDialog(const String & title, const String & message,
	Icon icon, Dialog::Buttons buttons)
{
	Image * iconImage = Desktop::getInstance()->getSkin()->getMessageDialogIconImage(icon);
	return createDialog(title, message, iconImage, buttons);
}

MessageDialog * MessageDialog::createDialog(const String & title, const String & message,
	const String & iconImageId, Dialog::Buttons buttons)
{
	Image * iconImage = Desktop::getInstance()->getPlatformAdapter()->getImage(iconImageId);
	return createDialog(title, message, iconImage, buttons);
}

MessageDialog * MessageDialog::createDialog(const String & title, const String & message,
	Image * iconImage, Dialog::Buttons buttons)
{
	MessageDialog * dialog;
	if ((dialog = new MessageDialog(message, iconImage)) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addWindow(dialog);

	dialog->setTitle(title);
	dialog->setButtons(buttons);
	dialog->setMaximizable(false);
	dialog->setResizable(false);

	return dialog;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(MessageDialog, Dialog);
#endif

const String & MessageDialog::getRememberId()
{
	ASSERTION_COBJECT(this);
	return rememberId;
}

void MessageDialog::setRememberId(const String & rememberId)
{
	ASSERTION_COBJECT(this);
	this->rememberId = rememberId;
}

void MessageDialog::setupContent(FieldsArea * area, GroupField * contentGroup)
{
	ASSERTION_COBJECT(this);

	int bottomContentMargin = 8;
	int buttonSpacing = 8;
	int iconContentSpacing = 8;
	Desktop::getInstance()->getSkin()->getDialogMargins(this,
		bottomContentMargin, buttonSpacing, iconContentSpacing);

	contentGroup->getGroupLayoutData()->setColumns(2);

	if (iconImage != NULL)
	{
		GLAZIERY_IMAGEFIELD(iconImageField, area, contentGroup, iconImage)
		GLAZIERY_FIELDLAYOUTDATA_MP(iconImageField, 0, 0, iconContentSpacing, 0, 0, 0, 0, 0, START, START, 1)
	}

	GLAZIERY_GROUPFIELD(messageGroup, area, contentGroup)
	GLAZIERY_FIELDLAYOUTDATA_MP(messageGroup, 0, 0, 0, 0, 0, 0, 0, 0, START, START, 1)

	GLAZIERY_LABEL(messageLabel, area, messageGroup, message);
	GLAZIERY_FIELDLAYOUTDATA_MP(messageLabel, 0, 0, 0, 0, 0, 0, 0, 0, START, START, 1)

	if (!rememberId.IsEmpty())
	{
		GLAZIERY_CHECKBOX(rememberCheckbox, area, messageGroup, STRING_MESSAGEDIALOG_REMEMBER, UNCHECKED);
		this->rememberCheckbox = rememberCheckbox;
		GLAZIERY_FIELDLAYOUTDATA_MP(rememberCheckbox, 0, 8, 0, 0, 0, 0, 0, 0, START, START, 1)
	}
}

Dialog::Buttons MessageDialog::showModally()
{
	ASSERTION_COBJECT(this);

	if (!rememberId.IsEmpty())
	{
		Config & config = Desktop::getInstance()->getConfig();
		ConfigSection * glazierySection = config.GetContent()->GetSection("Glaziery");
		ConfigSection * remembersSection = glazierySection->GetSection("MessageDialogRemembers");
		if (remembersSection->HasParameter(rememberId))
		{
			if (isAutoClosingOnEnd())
			{
				hide();
				close();
			}

			return (Dialog::Buttons) remembersSection->GetIntValue(rememberId, Dialog::BUTTON_CANCEL);
		}
	}

	Dialog::Buttons result = Dialog::showModally();

	if (!rememberId.IsEmpty() && (result == Dialog::BUTTON_YES || result == Dialog::BUTTON_NO)
		&& rememberCheckbox != NULL && rememberCheckbox->isChecked())
	{
		Config & config = Desktop::getInstance()->getConfig();
		ConfigSection * glazierySection = config.GetContent()->GetSection("Glaziery");
		ConfigSection * remembersSection = glazierySection->GetSection("MessageDialogRemembers");
		remembersSection->SetIntValue(rememberId, (int) result);
	}

	return result;
}

Dialog::Buttons MessageDialog::showDialog(const String & title, const String & message,
	Icon icon, Dialog::Buttons buttons, unsigned long autoEndDelay,
	Buttons autoEndButton)
{
	Image * iconImage = Desktop::getInstance()->getSkin()->getMessageDialogIconImage(icon);
	return showDialog(title, message, iconImage, buttons, autoEndDelay, autoEndButton);
}

Dialog::Buttons MessageDialog::showDialog(const String & title, const String & message,
	const String & iconImageId, Dialog::Buttons buttons, unsigned long autoEndDelay,
	Buttons autoEndButton)
{
	Image * iconImage = Desktop::getInstance()->getPlatformAdapter()->getImage(iconImageId);
	return showDialog(title, message, iconImage, buttons, autoEndDelay, autoEndButton);
}

Dialog::Buttons MessageDialog::showDialog(const String & title, const String & message,
	Image * iconImage, Dialog::Buttons buttons, unsigned long autoEndDelay,
	Buttons autoEndButton)
{
	Dialog * dialog = MessageDialog::createDialog(title, message, iconImage, buttons);
	dialog->setAutoEnding(autoEndDelay, autoEndButton);
	return dialog->showModally();
}
