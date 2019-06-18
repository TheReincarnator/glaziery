/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


// TODO: Define a unique ID for this scene.
const char * ExampleScene::ID = "ExampleScene";

// TODO: Define events for this scene.
const int ExampleScene::QUIT_EVENT = 0;

ExampleScene::ExampleScene()
{
	ASSERTION_COBJECT(this);

	// TODO: Initialize attributes and super class attributes.
	// TODO: Perform operations that require the platform adapter NOT to be initialized here.

	#ifdef _DEBUG
		// Non-fullscreen mode windows do not support too large heights
		setDesiredHeight(700);
	#endif
}

ExampleScene::~ExampleScene()
{
	ASSERTION_COBJECT(this);

	// Usually, you do not perform shut down operations here.
	// At this point, the platform adapter and the skin are already down.
}

const char * ExampleScene::getId()
{
	ASSERTION_COBJECT(this);
	return ID;
}

void ExampleScene::onInitialize(bool firstRun)
{
	ASSERTION_COBJECT(this);

	// TODO: Perform all initialization operations here that require
	// an initialized platform adapter (such as image or font retrieval).

	if (firstRun)
	{
		// TODO: Assign events to keys, mouse, and/or joystick.
		attachKeyOrButton(QUIT_EVENT, DIK_ESCAPE, InputManager::ATTACHMENT_KEYPRESS, 1);
	}
}

void ExampleScene::onShutDown()
{
	ASSERTION_COBJECT(this);

	// TODO: Perform all shut down operations here that require
	// the platform adapter.
}

void ExampleScene::renderBackground(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();

	// TODO: Probably remove clearing the device,
	// if you ensure rendering on the entire device.
	adapter->clear(context);

	// TODO: Now render the desktop background (e.g. the game 3D scene).
	// To do so, use the desktop's platform adapter interface
	// or cast it to the respective more concrete adapter to
	// retrieve native objects, such as the Direct 3D device.
	// On DirectX environments, you can do the following:
	// IDirect3DDevice9 * direct3DDevice = ((DirectXPlatformAdapter *) adapter)->getDirect3DDevice();
}

void ExampleScene::update()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();

	// TODO: Update internal variables and perform application operations,
	// such as game state updates, collision calculations, move world
	// objects according to the elapsed time using the desktop's timing,
	// do network communication, etc.

	// TODO: Determine user wishes with the desktop's input manager's
	// getEventQuantity() method.
	InputManager & inputManager = desktop->getInputManager();
	if (inputManager.getEventQuantity(QUIT_EVENT) > 0)
		desktop->end();
}
