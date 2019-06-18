/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Thread::Thread(int (*function)(Thread & thread, void * parameter), void * parameter)
{
	ASSERTION_COBJECT(this);

	this->function = function;
	this->parameter = parameter;

	canceled = false;
	exitCode = 0;
	progressDialog = NULL;
}

Thread::~Thread()
{
	ASSERTION_COBJECT(this);
}

void Thread::cancel()
{
	ASSERTION_COBJECT(this);
	canceled = true;
}

int Thread::getExitCode()
{
	ASSERTION_COBJECT(this);
	return exitCode;
}

int (*Thread::getFunction())(Thread & thread, void * parameter)
{
	ASSERTION_COBJECT(this);
	return function;
}

void * Thread::getParameter()
{
	ASSERTION_COBJECT(this);
	return parameter;
}

ProgressDialog * Thread::getProgressDialog()
{
	ASSERTION_COBJECT(this);
	return progressDialog;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Thread, CObject);
#endif

bool Thread::isCanceled()
{
	ASSERTION_COBJECT(this);
	return canceled;
}

int Thread::runAndWait(bool deleteThread, bool hourGlass)
{
	ASSERTION_COBJECT(this);

	if (progressDialog != NULL)
		progressDialog->showModally();
	else
	{
		if (hourGlass)
			Desktop::getInstance()->showHourGlass();

		resume();
		while (!isFinished())
		{
			Desktop::getInstance()->runOnce(20);
			Desktop::getInstance()->sleep(2);
		}

		if (hourGlass)
			Desktop::getInstance()->hideHourGlass();
	}

	int exitCode = this->exitCode;

	if (deleteThread)
		delete this;

	return exitCode;
}

void Thread::setExitCode(int exitCode)
{
	ASSERTION_COBJECT(this);
	this->exitCode = exitCode;
}

void Thread::setProgressDialog(ProgressDialog * progressDialog)
{
	ASSERTION_COBJECT(this);
	this->progressDialog = progressDialog;
}
