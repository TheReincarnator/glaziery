/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


const char * MainScene::ID = "MainScene";

const int MainScene::NEW_EVENT = 1;

const int MainScene::QUIT_EVENT = 0;

MainScene::MainScene()
{
	ASSERTION_COBJECT(this);

	#ifdef _DEBUG
		// Non-fullscreen mode windows do not support too large heights
		setDesiredHeight(700);
	#endif
}

MainScene::~MainScene()
{
	ASSERTION_COBJECT(this);
}

void MainScene::createNewWindow()
{
	ASSERTION_COBJECT(this);

	int windowCount = Desktop::getInstance()->getWindows().GetCount();
	if (windowCount < 10)
	{
		MainWindow * window;
		if ((window = new MainWindow) == NULL)
			throw EOUTOFMEMORY;
		Desktop::getInstance()->addWindow(window);
		window->initialize();

		String title("Glaziery test window ");
		title += windowCount + 1;
		window->setEventConsuming(true);
		window->setTitle(title);

		window->moveTo(Vector(16 + windowCount * 16, 16 + windowCount * 16));
	}
}

const char * MainScene::getId()
{
	ASSERTION_COBJECT(this);
	return ID;
}

void MainScene::onInitialize(bool firstRun)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	font = adapter->getFont("fonts\\Verdana-10-bold-lightblue-brown.png");
	danielImage = adapter->getImage("images\\Daniel.png", true);
	testImage = adapter->getImage("images\\Test.png", true);

	// Attach keys

	if (firstRun)
	{
		attachKeyOrButton(QUIT_EVENT, DIK_ESCAPE, InputManager::ATTACHMENT_KEYPRESS, 1);
		attachKeyOrButton(NEW_EVENT, InputManager::KEY_MOUSE_MIDDLE, InputManager::ATTACHMENT_KEYPRESS, 1);
		attachKeyOrButton(NEW_EVENT, DIK_N, InputManager::ATTACHMENT_KEYPRESS, 1);
	}
}

void MainScene::onShutDown()
{
	ASSERTION_COBJECT(this);
	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
}

void MainScene::renderBackground(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Scene::renderBackground(context);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();

	if (desktop->getWindows().IsEmpty())
	{
		String text = "Press 'N' to create a new window\nPress 'ESC' to quit";
		adapter->drawText(context, font, (desktop->getSize() - font->getSize(text, false)) / 2, text);
	}
}

void MainScene::update()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	InputManager & inputManager = desktop->getInputManager();

	if (inputManager.getEventQuantity(QUIT_EVENT) > 0)
		desktop->end();

	if (inputManager.getEventQuantity(NEW_EVENT) > 0)
		createNewWindow();
}
