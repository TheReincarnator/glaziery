/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Dialog::Dialog()
{
	ASSERTION_COBJECT(this);

	autoClosingOnEnd = true;
	autoEndButton = NO_BUTTON;
	autoEndDelay = -1;
	autoEndTime = 0;
	buttonDelayButtons = NO_BUTTON;
	buttonDelayTime = 0;
	buttons = BUTTON_OK;
	buttonsGroup = NULL;
	cancelButton = NULL;
	closeButton = NULL;
	ignoreButton = NULL;
	initialized = false;
	noButton = NULL;
	okButton = NULL;
	pressedButton = NULL;
	retryButton = NULL;
	started = false;
	yesButton = NULL;

	Desktop::getInstance()->getSkin()->setupDialog(this);
}

Dialog::~Dialog()
{
	ASSERTION_COBJECT(this);
}

void Dialog::cancelDialog()
{
	ASSERTION_COBJECT(this);

	if (cancelButton != NULL)
		cancelButton->press();
	else if (closeButton != NULL)
		closeButton->press();
	else if (noButton != NULL)
		noButton->press();
	else if (okButton != NULL)
		okButton->press();
	else
		endModalRun();
}

void Dialog::endDialog(Button * pressedButton)
{
	ASSERTION_COBJECT(this);

	this->pressedButton = pressedButton;
	endModalRun();
}

void Dialog::endDialog(Buttons buttonId)
{
	ASSERTION_COBJECT(this);
	endDialog(getButton(buttonId));
}

void Dialog::endDialogByDefault()
{
	ASSERTION_COBJECT(this);

	if (okButton != NULL)
		okButton->press();
	else if (closeButton != NULL)
		closeButton->press();
	else if (yesButton != NULL)
		yesButton->press();
	else if (cancelButton != NULL)
		cancelButton->press();
	else
		endModalRun();
}

Dialog::Buttons Dialog::getAutoEndButton()
{
	ASSERTION_COBJECT(this);
	return autoEndButton;
}

unsigned long Dialog::getAutoEndDelay()
{
	ASSERTION_COBJECT(this);
	return autoEndDelay;
}

unsigned long Dialog::getAutoEndTime()
{
	ASSERTION_COBJECT(this);
	return autoEndTime;
}

Button * Dialog::getButton(Buttons buttonId)
{
	ASSERTION_COBJECT(this);

	setup();

	switch (buttonId)
	{
		case BUTTON_OK: return okButton;
		case BUTTON_YES: return yesButton;
		case BUTTON_NO: return noButton;
		case BUTTON_CANCEL: return cancelButton;
		case BUTTON_CLOSE: return closeButton;
		case BUTTON_RETRY: return retryButton;
		case BUTTON_IGNORE: return ignoreButton;
	}

	return NULL;
}

Dialog::Buttons Dialog::getButtons()
{
	ASSERTION_COBJECT(this);
	return buttons;
}

FieldsArea * Dialog::getFieldsArea()
{
	ASSERTION_COBJECT(this);
	return (FieldsArea *) getClientArea();
}

Button * Dialog::getPressedButton()
{
	ASSERTION_COBJECT(this);
	return pressedButton;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Dialog, ModalWindow);
#endif

bool Dialog::isAutoClosingOnEnd()
{
	ASSERTION_COBJECT(this);
	return autoClosingOnEnd;
}

bool Dialog::isInitialized()
{
	ASSERTION_COBJECT(this);
	return initialized;
}

bool Dialog::onAnyKey(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	resetAutoEndTime();
	return ModalWindow::onAnyKey(option1, option2);
}

bool Dialog::onCancel()
{
	ASSERTION_COBJECT(this);

	cancelDialog();
	return true;
}

void Dialog::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	resetAutoEndTime();
	ModalWindow::onClick(position, option1, option2);
}

void Dialog::onCloseWidgetClicked()
{
	ASSERTION_COBJECT(this);
	onCancel();
}

void Dialog::onContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	resetAutoEndTime();
	ModalWindow::onContextClick(position, option1, option2);
}

VetoMode Dialog::onDialogEnd(Buttons button)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}

void Dialog::onDialogStart()
{
	ASSERTION_COBJECT(this);

	if (yesButton != NULL && yesButton->getText().IsEmpty())
		yesButton->setText(STRING_DIALOG_YES);

	if (noButton != NULL && noButton->getText().IsEmpty())
		noButton->setText(STRING_DIALOG_NO);

	if (retryButton != NULL && retryButton->getText().IsEmpty())
		retryButton->setText(STRING_DIALOG_RETRY);

	if (ignoreButton != NULL && ignoreButton->getText().IsEmpty())
		ignoreButton->setText(STRING_DIALOG_IGNORE);

	if (okButton != NULL && okButton->getText().IsEmpty())
		okButton->setText(STRING_DIALOG_OK);

	if (closeButton != NULL && closeButton->getText().IsEmpty())
		closeButton->setText(STRING_DIALOG_CLOSE);

	if (cancelButton != NULL && cancelButton->getText().IsEmpty())
		cancelButton->setText(STRING_DIALOG_CANCEL);

	buttonDelayTime += Desktop::getInstance()->getFrameTime();

	if (okButton != NULL && (buttonDelayButtons & Dialog::BUTTON_OK) != 0)
	{
		okButton->setActive(false);
		setClosable(false);
	}

	if (yesButton != NULL && (buttonDelayButtons & Dialog::BUTTON_YES) != 0)
		yesButton->setActive(false);

	if (noButton != NULL && (buttonDelayButtons & Dialog::BUTTON_NO) != 0)
		noButton->setActive(false);

	if (cancelButton != NULL && (buttonDelayButtons & Dialog::BUTTON_CANCEL) != 0)
	{
		cancelButton->setActive(false);
		setClosable(false);
	}

	if (closeButton != NULL && (buttonDelayButtons & Dialog::BUTTON_CLOSE) != 0)
	{
		closeButton->setActive(false);
		setClosable(false);
	}

	if (retryButton != NULL && (buttonDelayButtons & Dialog::BUTTON_RETRY) != 0)
		retryButton->setActive(false);

	if (ignoreButton != NULL && (buttonDelayButtons & Dialog::BUTTON_IGNORE) != 0)
		ignoreButton->setActive(false);

	started = true;
}

void Dialog::onModalStart()
{
	ASSERTION_COBJECT(this);

	ModalWindow::onModalStart();
	pressedButton = NULL;
}

void Dialog::onModalUpdate()
{
	ASSERTION_COBJECT(this);

	ModalWindow::onModalUpdate();

	unsigned long frameTime = Desktop::getInstance()->getFrameTime();
	if (buttonDelayButtons != NO_BUTTON && frameTime >= buttonDelayTime)
	{
		if (okButton != NULL && (buttonDelayButtons & Dialog::BUTTON_OK) != 0)
		{
			okButton->setActive(true);
			setClosable(true);
		}

		if (cancelButton != NULL && (buttonDelayButtons & Dialog::BUTTON_CANCEL) != 0)
		{
			cancelButton->setActive(true);
			setClosable(true);
		}

		if (closeButton != NULL && (buttonDelayButtons & Dialog::BUTTON_CLOSE) != 0)
		{
			closeButton->setActive(true);
			setClosable(true);
		}

		buttonDelayButtons = NO_BUTTON;
	}

	if (autoEndTime > 0 && frameTime >= autoEndTime)
	{
		Button * button = getButton(autoEndButton);
		autoEndButton = NO_BUTTON;

		if (button != NULL)
			button->press();
	}
}

void Dialog::resetAutoEndTime()
{
	ASSERTION_COBJECT(this);

	if (autoEndDelay >= 0)
		autoEndTime = Desktop::getInstance()->getFrameTime() + autoEndDelay;
}

void Dialog::setAutoClosingOnEnd(bool autoClosingOnEnd)
{
	ASSERTION_COBJECT(this);
	this->autoClosingOnEnd = autoClosingOnEnd;
}

void Dialog::setAutoEnding(unsigned long autoEndDelay, Buttons autoEndButton)
{
	ASSERTION_COBJECT(this);

	this->autoEndDelay = autoEndDelay;

	if (autoEndDelay >= 0)
	{
		autoEndTime = Desktop::getInstance()->getFrameTime() + autoEndDelay;
		this->autoEndButton = autoEndButton;
	}
	else
	{
		autoEndTime = 0;
		this->autoEndButton = NO_BUTTON;
	}
}

void Dialog::setButtonDelay(Dialog::Buttons buttons, unsigned long delayMilliSeconds)
{
	ASSERTION_COBJECT(this);

	if (started)
		throw EILLEGALSTATE("The button delay may only be set before starting the dialog");

	buttonDelayButtons = buttons;
	buttonDelayTime = delayMilliSeconds;
}

void Dialog::setButtons(Dialog::Buttons buttons)
{
	ASSERTION_COBJECT(this);

	if (initialized)
		throw EILLEGALSTATE("The button delay may only be set before setting up the dialog (which happens automatically when changing a button)");

	this->buttons = buttons;
}

void Dialog::setButtonText(int buttonId, const String & text)
{
	ASSERTION_COBJECT(this);

	setup();

	switch (buttonId)
	{
		case BUTTON_OK: if (okButton != NULL) okButton->setText(text); return;
		case BUTTON_YES: if (yesButton != NULL) yesButton->setText(text); return;
		case BUTTON_NO: if (noButton != NULL) noButton->setText(text); return;
		case BUTTON_CANCEL: if (cancelButton != NULL) cancelButton->setText(text); return;
		case BUTTON_CLOSE: if (closeButton != NULL) closeButton->setText(text); return;
		case BUTTON_RETRY: if (retryButton != NULL) retryButton->setText(text); return;
		case BUTTON_IGNORE: if (ignoreButton != NULL) ignoreButton->setText(text); return;
	}
}

void Dialog::setup()
{
	ASSERTION_COBJECT(this);

	if (initialized)
		return;

	Desktop * desktop = Desktop::getInstance();

	// Setup the dialog window

	hide();
	setMaximizable(false);
	setClosable((buttons & (Dialog::BUTTON_OK | Dialog::BUTTON_CANCEL | Dialog::BUTTON_CLOSE)) != 0);

	// Prepare the fields area

	FieldsArea * area;
	if ((area = new FieldsArea) == NULL)
		throw EOUTOFMEMORY;
	setClientArea(area);

	GLAZIERY_FIELDSAREALAYOUTDATA(area, 1)

	// Create the content group

	int bottomContentMargin = 8;
	int buttonSpacing = 8;
	int iconContentSpacing = 8;
	desktop->getSkin()->getDialogMargins(this,
		bottomContentMargin, buttonSpacing, iconContentSpacing);

	GLAZIERY_GROUPFIELD(contentGroup, area, NULL)
	GLAZIERY_GROUPFIELDLAYOUTDATA(contentGroup, 0)
	GLAZIERY_FIELDLAYOUTDATA_MP(contentGroup, 0, 0, 0, bottomContentMargin, 0, 0, 0, 0, FILL, FILL, 1)

	setupContent(area, contentGroup);

	// Create the buttons group

	GLAZIERY_GROUPFIELD(buttonsGroup, area, NULL)
	this->buttonsGroup = buttonsGroup;
	GLAZIERY_GROUPFIELDLAYOUTDATA(buttonsGroup, 0)
	GLAZIERY_FIELDLAYOUTDATA_MP(buttonsGroup, 0, 0, 0, 0, 0, 0, 0, 0, END, START, 1)

	// Add buttons

	setupButtons();

	// Set default button

	if (buttons & Dialog::BUTTON_OK)
		area->setDefaultButton(this->okButton);
	else if (buttons & Dialog::BUTTON_CLOSE)
		area->setDefaultButton(this->closeButton);
	else if (buttons & Dialog::BUTTON_CANCEL)
		area->setDefaultButton(this->cancelButton);

	// Layout and center

	area->setAutoLayouting(true);

	// Add listeners

	ButtonListener * buttonListener;
	if ((buttonListener = new ButtonListener) == NULL)
		throw EOUTOFMEMORY;

	if (okButton != NULL)
		okButton->appendListener(buttonListener);
	if (yesButton != NULL)
		yesButton->appendListener(buttonListener);
	if (noButton != NULL)
		noButton->appendListener(buttonListener);
	if (cancelButton != NULL)
		cancelButton->appendListener(buttonListener);
	if (closeButton != NULL)
		closeButton->appendListener(buttonListener);
	if (retryButton != NULL)
		retryButton->appendListener(buttonListener);
	if (ignoreButton != NULL)
		ignoreButton->appendListener(buttonListener);

	initialized = true;
}

void Dialog::setupButtons()
{
	ASSERTION_COBJECT(this);

	ClientArea * area = getClientArea();
	int bottomContentMargin = 8;
	int buttonSpacing = 8;
	int iconContentSpacing = 8;
	Desktop::getInstance()->getSkin()->getDialogMargins(this,
		bottomContentMargin, buttonSpacing, iconContentSpacing);

	if (buttons & Dialog::BUTTON_YES)
	{
		GLAZIERY_BUTTON(yesButton, area, buttonsGroup, "", NULL)
		this->yesButton = yesButton;
		yesButton->setDialogButton(true);
		GLAZIERY_FIELDLAYOUTDATA_MP(yesButton, buttonSpacing, 0, buttonSpacing, 0, 0, 0, 0, 0, START, START, 1)
	}

	if (buttons & Dialog::BUTTON_NO)
	{
		GLAZIERY_BUTTON(noButton, area, buttonsGroup, "", NULL)
		this->noButton = noButton;
		noButton->setDialogButton(true);
		GLAZIERY_FIELDLAYOUTDATA_MP(noButton, buttonSpacing, 0, buttonSpacing, 0, 0, 0, 0, 0, START, START, 1)
	}

	if (buttons & Dialog::BUTTON_RETRY)
	{
		GLAZIERY_BUTTON(retryButton, area, buttonsGroup, "", NULL)
		this->retryButton = retryButton;
		retryButton->setDialogButton(true);
		GLAZIERY_FIELDLAYOUTDATA_MP(retryButton, buttonSpacing, 0, buttonSpacing, 0, 0, 0, 0, 0, START, START, 1)
	}

	if (buttons & Dialog::BUTTON_IGNORE)
	{
		GLAZIERY_BUTTON(ignoreButton, area, buttonsGroup, "", NULL)
		this->ignoreButton = ignoreButton;
		ignoreButton->setDialogButton(true);
		GLAZIERY_FIELDLAYOUTDATA_MP(ignoreButton, buttonSpacing, 0, buttonSpacing, 0, 0, 0, 0, 0, START, START, 1)
	}

	if (buttons & Dialog::BUTTON_OK)
	{
		GLAZIERY_BUTTON(okButton, area, buttonsGroup, "", NULL)
		this->okButton = okButton;
		okButton->setDialogButton(true);
		GLAZIERY_FIELDLAYOUTDATA_MP(okButton, buttonSpacing, 0, buttonSpacing, 0, 0, 0, 0, 0, START, START, 1)
	}

	if (buttons & Dialog::BUTTON_CLOSE)
	{
		GLAZIERY_BUTTON(closeButton, area, buttonsGroup, "", NULL)
		this->closeButton = closeButton;
		closeButton->setDialogButton(true);
		GLAZIERY_FIELDLAYOUTDATA_MP(closeButton, buttonSpacing, 0, buttonSpacing, 0, 0, 0, 0, 0, START, START, 1)
	}

	if (buttons & Dialog::BUTTON_CANCEL)
	{
		GLAZIERY_BUTTON(cancelButton, area, buttonsGroup, "", NULL)
		this->cancelButton = cancelButton;
		cancelButton->setDialogButton(true);
		GLAZIERY_FIELDLAYOUTDATA_MP(cancelButton, buttonSpacing, 0, buttonSpacing, 0, 0, 0, 0, 0, START, START, 1)
	}
}

Dialog::Buttons Dialog::showModally()
{
	ASSERTION_COBJECT(this);

	// Setup dialog

	setup();

	// Perform final initializations

	onDialogStart();

	// Run the dialog modally and fetch the pressed button

	Desktop * desktop = Desktop::getInstance();
	desktop->cancelAllPopups();

	Dialog::Buttons pressedButtonId = NO_BUTTON;
	do
	{
		runModally();

		Button * pressedButton = getPressedButton();
		if (pressedButton == NULL)
			pressedButtonId = NO_BUTTON;
		else if (pressedButton == okButton)
			pressedButtonId = BUTTON_OK;
		else if (pressedButton == cancelButton)
			pressedButtonId = BUTTON_CANCEL;
		else if (pressedButton == closeButton)
			pressedButtonId = BUTTON_CLOSE;
		else if (pressedButton == retryButton)
			pressedButtonId = BUTTON_RETRY;
		else if (pressedButton == ignoreButton)
			pressedButtonId = BUTTON_IGNORE;
		else if (pressedButton == yesButton)
			pressedButtonId = BUTTON_YES;
		else if (pressedButton == noButton)
			pressedButtonId = BUTTON_NO;
		else
			pressedButtonId = NO_BUTTON;
	}
	while (onDialogEnd(pressedButtonId) == VETO);

	if (autoClosingOnEnd)
		close();

	return pressedButtonId;
}

String Dialog::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Dialog(position:%s,size:%s)",
		(const char *) getPosition().toString(), (const char *) getSize().toString());
	return string;
}


Dialog::ButtonListener::ButtonListener()
{
	ASSERTION_COBJECT(this);
	inResize = false;
}

void Dialog::ButtonListener::onResized(Component * component, Vector oldSize)
{
	ASSERTION_COBJECT(this);

	if (inResize)
		return;

	inResize = true;

	FieldsArea * area = ((Button *) component)->getFieldsArea();
	if (area == NULL)
	{
		inResize = false;
		return;
	}

	if (area->isAutoLayouting())
	{
		inResize = false;
		return;
	}

	Layouter * layouter = area->getLayouter();
	if (layouter == NULL)
	{
		inResize = false;
		return;
	}

	Dialog * dialog = (Dialog *) area->getWindow();
	Vector oldGroupSize = dialog->buttonsGroup->getSize();
	layouter->layout(dialog->buttonsGroup, true);
	dialog->buttonsGroup->moveRelative(oldGroupSize - dialog->buttonsGroup->getSize());

	inResize = false;
}
