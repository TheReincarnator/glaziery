// Main header file
#include "Headers.h"


Application * Application::singleton = NULL;

Application::Application()
{
	ASSERTION_COBJECT(this);

	// TODO: Initialize the application.
	// The Desktop, platform adapter, and skin are available at this time.
}

Application::~Application()
{
	ASSERTION_COBJECT(this);

	// TODO: Shut down the application.
	// The Desktop, platform adapter, and skin are available at this time.
}

void Application::createInstance()
{
	ASSERTION_COBJECT(this);

	if (singleton != NULL)
		throw EILLEGALSTATE("The application instance already exists. Use destroyInstance first().");

	if (singleton == NULL)
		if ((singleton = new Application) == NULL)
			throw EOUTOFMEMORY;
}

void Application::destroyInstance()
{
	ASSERTION_COBJECT(this);

	if (singleton != NULL) {delete singleton; singleton = NULL;}
}

Application * Application::getInstance()
{
	ASSERTION_COBJECT(this);
	return singleton;
}
