/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE previousInstanceHandle,
	LPSTR commandLine, int show)
{
	bool abnormal = false;
	Exception caughtException;
	bool caughtExceptionFlag = false;
	try
	{
		PlatformAdapter * adapter = PlatformAdapter::createInstance();
		#ifdef D3D_OVERLOADS
			((DirectXPlatformAdapter *) adapter)->setApplicationIcon(IDR_MAINFRAME);
		#endif

		if (adapter == NULL)
			throw EOUTOFMEMORY;

		Skin * skin;
		if ((skin = new SimpleSkin) == NULL)
			throw EOUTOFMEMORY;

		Desktop * desktop = Desktop::createInstance(adapter, skin, "Partus Parentalis", "Glaziery Tester");
		desktop->setTitle("Glaziery Tester");
		desktop->setMinimumFrameDuration(10);
		desktop->setExtraFrameDuration(5);

		Scene * scene;
		if ((scene = new MainScene) == NULL)
			throw EOUTOFMEMORY;
		desktop->addScene(scene);

		desktop->setActiveScene(MainScene::ID);
		desktop->run();

		Desktop::destroyInstance();
	}
	catch (EOutOfMemory & exception)
	{
		abnormal = true;
		caughtException = exception;
		caughtExceptionFlag = true;
	}
	#ifndef _DEBUG
		catch (Exception & exception)
		{
			abnormal = true;
			caughtException = exception;
			caughtExceptionFlag = true;
		}
		catch (...)
		{
			abnormal = true;
		}
	#endif

	if (abnormal)
	{
		// Try shutting down application and desktop

		try
		{
			Desktop::destroyInstance();
		}
		catch (...)
		{
		}

		String message("The Glaziery tester encountered an error while running and must quit.\n"
			"Sorry for the inconvenience while enjoying the Glaziery tester.\n\n");

		if (caughtExceptionFlag)
		{
			message += "Please report the message below to the developers,\n"
				"so that they can analyze and fix the problem.\n\n";
			message += caughtException.GetClassName();
			message += ": ";
			message += caughtException.GetMessage();
			message += "\nSource: ";
			message += caughtException.GetSource();
			message += ":";
			message += caughtException.GetLineNumber();
			message += "\n";
		}

		::MessageBox(NULL, message, "Glaziery tester - Abnormal program termination",
			MB_OK | MB_ICONERROR);
	}

	::PostQuitMessage(0);
	return 0;
}
