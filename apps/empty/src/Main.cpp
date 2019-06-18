// Main header file
#include "Headers.h"


int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE previousInstanceHandle,
	LPSTR commandLine, int show)
{
	ASSERTION_COBJECT(this);

	bool abnormal = false;
	Exception caughtException;
	bool caughtExceptionFlag = false;
	try
	{
		// TODO: Choose and instantiate an appropriate platform adapter.
		// The platform adapter typically results from the platform (PC, Mac, ...)
		// and the rendering technology (DirectX, OpenGL, ...).
		// You can also implement your own one and use macros to choose
		// the appropriate adapter in a multi-platform environment.

		#ifdef D3D_OVERLOADS
			DirectXPlatformAdapter * adapter = new DirectXPlatformAdapter(instanceHandle);
			adapter->setApplicationIcon(IDR_MAINFRAME);
		#elif defined(TODO_MACOS_KEYWORD)
			MacOSPlatformAdapter * adapter = new MacOSPlatformAdapter(instanceHandle);
		#else
			#error Unknown platform
		#endif

		if (adapter == NULL)
			throw EOUTOFMEMORY;

		// TODO: Choose and instantiate an appropriate skin.
		// The skin determines how components (windows, client areas, fields, ...)
		// look like (colors, images, fonts, layout, etc.), but it also defines
		// some UI related behavior, i.e. what happens if the user double-clicks
		// the title bar of a window. Glaziery provides a reference skin, the
		// SimpleSkin, and it encourages you to implement your own one.

		Skin * skin;
		if ((skin = new SimpleSkin) == NULL)
			throw EOUTOFMEMORY;

		Desktop * desktop = Desktop::createInstance(adapter, skin, "Unnamed manufacturer", "Unnamed application");

		// TODO: Set the application's (or game's) name, e.g. displayed in the task bar.
		desktop->setTitle("Unnamed application");

		// TODO: If desired, change the minimum time of one frame.
		desktop->setMinimumFrameTime(10);

		// TODO: Instantiate all scenes of the application (or game)
		// and add them to the Desktop.

		Scene * scene;
		if ((scene = new ExampleScene) == NULL)
			throw EOUTOFMEMORY;
		desktop->addScene(scene);

		// TODO: Choose one of the scenes as initial scene.
		desktop->setActiveScene(ExampleScene::ID);

		Application::createInstance();
		desktop->run();
		Application::destroyInstance();

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
			Application::destroyInstance();
		}
		catch (...)
		{
		}

		try
		{
			Desktop::destroyInstance();
		}
		catch (...)
		{
		}

		String message("EmptyApplication encountered an error while running and must quit.\n"
			"Sorry for the inconvenience while enjoying EmptyApplication.\n\n");

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

		::MessageBox(NULL, message, "EmptyApplication - Abnormal program termination",
			MB_OK | MB_ICONERROR);
	}

	::PostQuitMessage(0);
	return 0;
}
