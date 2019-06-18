/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int Scene::FORMAT_24BIT = 1;

const int Scene::FORMAT_24BIT_WITH_ALPHA = 2;

const int Scene::FORMAT_16BIT = 3;

const int Scene::FORMAT_15BIT = 4;

const int Scene::FORMAT_15BIT_WITH_ALPHA = 5;

const int Scene::FORMAT_12BIT = 6;

const int Scene::FORMAT_12BIT_WITH_ALPHA = 7;

const int Scene::FORMAT_8BIT = 8;

const int Scene::FORMAT_8BIT_WITH_ALPHA = 9;

const int Scene::KEEP_ADAPTER = 0;

const int Scene::RESTART_ADAPTER = 1;

Scene::Scene()
{
	ASSERTION_COBJECT(this);

	Int * format;

	if ((format = new Int(FORMAT_24BIT)) == NULL)
		throw EOUTOFMEMORY;
	acceptableFormats.Append(format);

	if ((format = new Int(FORMAT_16BIT)) == NULL)
		throw EOUTOFMEMORY;
	acceptableFormats.Append(format);

	if ((format = new Int(FORMAT_15BIT)) == NULL)
		throw EOUTOFMEMORY;
	acceptableFormats.Append(format);

	if ((format = new Int(FORMAT_8BIT)) == NULL)
		throw EOUTOFMEMORY;
	acceptableFormats.Append(format);

	configured = false;
	desiredWidth = 0;
	desiredHeight = 0;
	desiredRefreshRate = 0;

	#ifdef _DEBUG
		fullScreen = false;
		exclusive = false;
	#else
		fullScreen = true;
		exclusive = true;
	#endif

	hardwareTnLDesirable = true;
	minimumWidth = 600;
	minimumHeight = 480;
	maximumWidth = 1280;
	maximumHeight = 1024;
}

Scene::~Scene()
{
	ASSERTION_COBJECT(this);
}

void Scene::attachJoystickXMove(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.attachJoystickXMove(eventNumber, factor);
}

void Scene::attachJoystickXPosition(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.attachJoystickXPosition(eventNumber, factor);
}

void Scene::attachJoystickYMove(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.attachJoystickYMove(eventNumber, factor);
}

void Scene::attachJoystickYPosition(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.attachJoystickYPosition(eventNumber, factor);
}

void Scene::attachKeyOrButton(int eventNumber, int keyCode, int type, long factor)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.attachKey(eventNumber, keyCode, type, factor);
}

void Scene::attachMouseXMove(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.attachMouseXMove(eventNumber, factor);
}

void Scene::attachMouseYMove(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.attachMouseYMove(eventNumber, factor);
}

void Scene::attachMouseWheel(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.attachMouseWheel(eventNumber, factor);
}

void Scene::Configure(ConfigSection * section, bool write)
{
	ASSERTION_COBJECT(this);
	configured = true;
}

void Scene::detachAll()
{
	ASSERTION_COBJECT(this);
	sceneAttachments.detachAll();
}

void Scene::detach(int eventNumber)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.detach(eventNumber);
}

void Scene::detachJoystickXAxis()
{
	ASSERTION_COBJECT(this);
	sceneAttachments.detachJoystickXAxis();
}

void Scene::detachJoystickYAxis()
{
	ASSERTION_COBJECT(this);
	sceneAttachments.detachJoystickYAxis();
}

void Scene::detachKeyOrButton(int keyCode)
{
	ASSERTION_COBJECT(this);
	sceneAttachments.detachKey(keyCode);
}

void Scene::detachMouseXMove()
{
	ASSERTION_COBJECT(this);
	sceneAttachments.detachMouseXMove();
}

void Scene::detachMouseYMove()
{
	ASSERTION_COBJECT(this);
	sceneAttachments.detachMouseYMove();
}

void Scene::detachMouseWheel()
{
	ASSERTION_COBJECT(this);
	sceneAttachments.detachMouseWheel();
}

const ArrayList<Int> & Scene::getAcceptableFormats()
{
	ASSERTION_COBJECT(this);
	return acceptableFormats;
}

int Scene::getAttachmentKeyOrButton(int eventNumber, bool positive, bool negative)
{
	ASSERTION_COBJECT(this);
	return sceneAttachments.getAttachmentKey(eventNumber, positive, negative);
}

unsigned int Scene::getDesiredHeight()
{
	ASSERTION_COBJECT(this);
	return desiredHeight;
}

unsigned int Scene::getDesiredRefreshRate()
{
	ASSERTION_COBJECT(this);
	return desiredRefreshRate;
}

unsigned int Scene::getDesiredWidth()
{
	ASSERTION_COBJECT(this);
	return desiredWidth;
}

InputAttachment & Scene::getJoystickXAxisAttachment()
{
	ASSERTION_COBJECT(this);
	return sceneAttachments.joystickXAxisAttachment;
}

InputAttachment & Scene::getJoystickYAxisAttachment()
{
	ASSERTION_COBJECT(this);
	return sceneAttachments.joystickYAxisAttachment;
}

InputAttachment & Scene::getKeyOrButtonAttachment(int keyCode)
{
	ASSERTION_COBJECT(this);
	return sceneAttachments.keyAttachments[keyCode];
}

unsigned int Scene::getMaximumHeight()
{
	ASSERTION_COBJECT(this);
	return maximumHeight;
}

unsigned int Scene::getMaximumWidth()
{
	ASSERTION_COBJECT(this);
	return maximumWidth;
}

unsigned int Scene::getMinimumHeight()
{
	ASSERTION_COBJECT(this);
	return minimumHeight;
}

unsigned int Scene::getMinimumWidth()
{
	ASSERTION_COBJECT(this);
	return minimumWidth;
}

InputAttachment & Scene::getMouseWheelAttachment()
{
	ASSERTION_COBJECT(this);
	return sceneAttachments.mouseWheelAttachment;
}

InputAttachment & Scene::getMouseXAxisAttachment()
{
	ASSERTION_COBJECT(this);
	return sceneAttachments.mouseXAxisAttachment;
}

InputAttachment & Scene::getMouseYAxisAttachment()
{
	ASSERTION_COBJECT(this);
	return sceneAttachments.mouseYAxisAttachment;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Scene, CObject);
#endif

bool Scene::isConfigured()
{
	ASSERTION_COBJECT(this);
	return configured;
}

int Scene::onEnter()
{
	ASSERTION_COBJECT(this);

	// The default does nothing.
	return KEEP_ADAPTER;
}

void Scene::onExit()
{
	ASSERTION_COBJECT(this);
	// The default does nothing.
}

void Scene::onInitialize(bool firstRun)
{
	ASSERTION_COBJECT(this);
	// The default does nothing.
}

void Scene::onShutDown()
{
	ASSERTION_COBJECT(this);
	// The default does nothing.
}

bool Scene::isExclusive()
{
	ASSERTION_COBJECT(this);
	return exclusive;
}

bool Scene::isFullScreen()
{
	ASSERTION_COBJECT(this);
	return fullScreen;
}

bool Scene::isHardwareTnLDesirable()
{
	ASSERTION_COBJECT(this);
	return hardwareTnLDesirable;
}

void Scene::renderBackground(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	adapter->clear(context);
}

void Scene::renderForeground(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	// The default does nothing.
}

void Scene::setAcceptableFormats(int * acceptableFormats, int length)
{
	ASSERTION_COBJECT(this);

	this->acceptableFormats.DeleteAll();

	for (int i=0; i<length; i++)
	{
		Int * intWrapper;
		if ((intWrapper = new Int(acceptableFormats[i])) == NULL)
			throw EOUTOFMEMORY;
		this->acceptableFormats.Append(intWrapper);
	}
}

void Scene::setDesiredHeight(unsigned int desiredHeight)
{
	ASSERTION_COBJECT(this);
	this->desiredHeight = desiredHeight;
}

void Scene::setDesiredRefreshRate(unsigned int desiredRefreshRate)
{
	ASSERTION_COBJECT(this);
	this->desiredRefreshRate = desiredRefreshRate;
}

void Scene::setDesiredWidth(unsigned int desiredWidth)
{
	ASSERTION_COBJECT(this);
	this->desiredWidth = desiredWidth;
}

void Scene::setExclusive(bool exclusive)
{
	ASSERTION_COBJECT(this);

	if (this->exclusive == exclusive)
		return;

	this->exclusive = exclusive;
	if (exclusive)
		fullScreen = true;

	if (Desktop::getInstance()->getActiveScene() == this)
		Desktop::getInstance()->getPlatformAdapter()->updateExclusive();
}

void Scene::setFullScreen(bool fullScreen)
{
	ASSERTION_COBJECT(this);

	this->fullScreen = fullScreen;
	if (!fullScreen)
	{
		if (exclusive)
		{
			exclusive = false;

			if (Desktop::getInstance()->getActiveScene() == this)
				Desktop::getInstance()->getPlatformAdapter()->updateExclusive();
		}
	}
}

void Scene::setHardwareTnLDesirable(bool hardwareTnLDesirable)
{
	ASSERTION_COBJECT(this);
	this->hardwareTnLDesirable = hardwareTnLDesirable;
}

void Scene::setMinimumHeight(unsigned int minimumHeight)
{
	ASSERTION_COBJECT(this);
	this->minimumHeight = minimumHeight;
}

void Scene::setMinimumWidth(unsigned int minimumWidth)
{
	ASSERTION_COBJECT(this);
	this->minimumWidth = minimumWidth;
}

void Scene::setMaximumHeight(unsigned int maximumHeight)
{
	ASSERTION_COBJECT(this);
	this->maximumHeight = maximumHeight;
}

void Scene::setMaximumWidth(unsigned int maximumWidth)
{
	ASSERTION_COBJECT(this);
	this->maximumWidth = maximumWidth;
}

String Scene::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Scene(%s)", getId());
	return string;
}

void Scene::update()
{
	ASSERTION_COBJECT(this);
}
