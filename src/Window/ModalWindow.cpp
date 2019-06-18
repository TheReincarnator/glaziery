/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ModalWindow::ModalWindow()
{
	ASSERTION_COBJECT(this);

	autoShowing = true;
	endingModalRun = false;
	setEventConsuming(true);
	setStayOnTop(true);

	Desktop::getInstance()->getSkin()->setupModalWindow(this);
}

ModalWindow::~ModalWindow()
{
	ASSERTION_COBJECT(this);
}

bool ModalWindow::close()
{
	if (!Window::close())
		return false;

	Desktop::getInstance()->updateModal();

	return true;
}

void ModalWindow::destroy()
{
	ASSERTION_COBJECT(this);

	Window::destroy();

	// Just in case the window wasn't closed
	Desktop::getInstance()->updateModal();
}

void ModalWindow::endModalRun()
{
	ASSERTION_COBJECT(this);
	endingModalRun = true;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ModalWindow, Window);
#endif

bool ModalWindow::isAutoShowing()
{
	ASSERTION_COBJECT(this);
	return autoShowing;
}

bool ModalWindow::isEndingModalRun()
{
	ASSERTION_COBJECT(this);
	return endingModalRun;
}

void ModalWindow::onModalEnd()
{
	ASSERTION_COBJECT(this);
	// Do nothing as default
}

void ModalWindow::onModalStart()
{
	ASSERTION_COBJECT(this);

	bringToFront();

	if (autoShowing)
		show();
}

void ModalWindow::onModalUpdate()
{
	ASSERTION_COBJECT(this);
	// Do nothing as default
}

void ModalWindow::runModally()
{
	ASSERTION_COBJECT(this);

	endingModalRun = false;

	onModalStart();

	Desktop * desktop = Desktop::getInstance();
	desktop->runOnce();
	while (!endingModalRun && !isDisposed() && !desktop->isEnding())
	{
		onModalUpdate();
		desktop->runOnce(20);
	}

	if (!isDisposed())
		onModalEnd();
}

void ModalWindow::setAutoShowing(bool autoShowing)
{
	ASSERTION_COBJECT(this);
	this->autoShowing = autoShowing;
}
