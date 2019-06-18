/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ProgressDialog::ProgressDialog(Thread * thread)
{
	ASSERTION_COBJECT(this);

	autoEndingOnCompletion = true;
	canceled = false;
	finished = false;
	function = NULL;
	functionCalled = false;
	functionParameter = NULL;
	functionResult = 0;
	itemLabel = NULL;
	mayCancel = true;
	progressBar = NULL;
	summaryLabel = NULL;
	this->thread = thread;
}

ProgressDialog::ProgressDialog(int (*function)(ProgressDialog & dialog, void * parameter), void * parameter)
{
	ASSERTION_COBJECT(this);

	autoEndingOnCompletion = true;
	canceled = false;
	finished = false;
	this->function = function;
	functionCalled = false;
	functionParameter = parameter;
	functionResult = 0;
	itemLabel = NULL;
	mayCancel = true;
	progressBar = NULL;
	summaryLabel = NULL;
	thread = NULL;
}

void ProgressDialog::cancel()
{
	ASSERTION_COBJECT(this);

	canceled = true;
	if (thread != NULL)
		thread->cancel();
}

ProgressDialog * ProgressDialog::createDialog(Thread * thread)
{
	ProgressDialog * dialog;
	if ((dialog = new ProgressDialog(thread)) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addWindow(dialog);

	dialog->setTitle(STRING_PROGRESSDIALOG_TITLE);
	dialog->setButtons(Dialog::BUTTON_CLOSE);
	dialog->setMayCancel(dialog->isMayCancel());

	return dialog;
}

ProgressDialog * ProgressDialog::createDialog(int (*function)(ProgressDialog & dialog, void * parameter),
	void * parameter)
{
	ProgressDialog * dialog;
	if ((dialog = new ProgressDialog(function, parameter)) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addWindow(dialog);

	dialog->setTitle(STRING_PROGRESSDIALOG_TITLE);
	dialog->setButtons(Dialog::BUTTON_CLOSE);
	dialog->setMayCancel(dialog->isMayCancel());

	return dialog;
}

Label * ProgressDialog::getItemLabel()
{
	ASSERTION_COBJECT(this);
	return itemLabel;
}

ProgressBar * ProgressDialog::getProgressBar()
{
	ASSERTION_COBJECT(this);
	return progressBar;
}

int ProgressDialog::getResult()
{
	ASSERTION_COBJECT(this);
	return functionResult;
}

Label * ProgressDialog::getSummaryLabel()
{
	ASSERTION_COBJECT(this);
	return summaryLabel;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ProgressDialog, Dialog);
#endif

bool ProgressDialog::isAutoEndingOnCompletion()
{
	ASSERTION_COBJECT(this);
	return autoEndingOnCompletion;
}

bool ProgressDialog::isCanceled()
{
	ASSERTION_COBJECT(this);
	return canceled;
}

bool ProgressDialog::isMayCancel()
{
	ASSERTION_COBJECT(this);
	return mayCancel;
}

VetoMode ProgressDialog::onDialogEnd(Buttons button)
{
	ASSERTION_COBJECT(this);

	if (!finished)
	{
		cancel();

		Button * closeButton = getButton(BUTTON_CLOSE);
		if (closeButton != NULL)
		{
			closeButton->setText(STRING_PROGRESSDIALOG_CANCELLING);
			closeButton->setActive(false);
		}

		return VETO;
	}
	else
		return NO_VETO;
}

void ProgressDialog::onDialogStart()
{
	ASSERTION_COBJECT(this);

	Dialog::onDialogStart();

	progressBar->setCandyMode();

	Button * closeButton = getButton(BUTTON_CLOSE);
	if (closeButton != NULL)
		closeButton->setText(STRING_DIALOG_CANCEL);

	if (thread != NULL)
		thread->resume();
}

void ProgressDialog::onModalUpdate()
{
	ASSERTION_COBJECT(this);

	Dialog::onModalUpdate();

	if (function != NULL && !functionCalled)
	{
		functionCalled = true;
		functionResult = function(*this, functionParameter);
		finished = true;

		if (autoEndingOnCompletion)
			endDialog(BUTTON_CLOSE);
	}

	if (thread != NULL && !finished && thread->isFinished())
	{
		finished = true;
		if (autoEndingOnCompletion)
			endDialog(BUTTON_CLOSE);
		else
		{
			Button * closeButton = getButton(BUTTON_CLOSE);
			if (closeButton != NULL)
			{
				closeButton->setText(STRING_DIALOG_CLOSE);
				closeButton->setActive(true);
			}
		}
	}
}

void ProgressDialog::setAutoEndingOnCompletion(bool autoEndingOnCompletion)
{
	ASSERTION_COBJECT(this);
	this->autoEndingOnCompletion = autoEndingOnCompletion;
}

void ProgressDialog::setMayCancel(bool mayCancel)
{
	ASSERTION_COBJECT(this);

	this->mayCancel = mayCancel;

	setClosable(mayCancel);
	getButton(Dialog::BUTTON_CLOSE)->setVisible(mayCancel);
	getFieldsArea()->setDefaultButton(mayCancel ? getButton(Dialog::BUTTON_CLOSE) : NULL);
}

void ProgressDialog::setupContent(FieldsArea * area, GroupField * contentGroup)
{
	ASSERTION_COBJECT(this);

	contentGroup->getGroupLayoutData()->setColumns(1);

	GLAZIERY_LABEL(itemLabel, area, contentGroup, STRING_PROGRESSDIALOG_ITEMTEXT)
	GLAZIERY_FIELDLAYOUTDATA(itemLabel, START, CENTER, 1)
	this->itemLabel = itemLabel;

	GLAZIERY_PROGRESSBAR_S(progressBar, area, contentGroup, 500, -1, 0, 100, 0)
	GLAZIERY_FIELDLAYOUTDATA_MP(progressBar, 4, 8, 4, 8, 0, 0, 0, 0, FILL, CENTER, 1)
	this->progressBar = progressBar;

	GLAZIERY_LABEL(summaryLabel, area, contentGroup, STRING_PROGRESSDIALOG_SUMMARYTEXT)
	GLAZIERY_FIELDLAYOUTDATA(summaryLabel, START, CENTER, 1)
	this->summaryLabel = summaryLabel;
}
