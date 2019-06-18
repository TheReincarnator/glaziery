/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int InputManager::ATTACHMENT_AXISMOVEMENT = 7;

const int InputManager::ATTACHMENT_AXISPOSITION = 6;

const int InputManager::ATTACHMENT_KEYHOLD = 1;

const int InputManager::ATTACHMENT_KEYPRESS = 2;

const int InputManager::ATTACHMENT_KEYPRESSRELEASE = 5;

const int InputManager::ATTACHMENT_KEYPRESSREPEAT = 3;

const int InputManager::ATTACHMENT_KEYRELEASE = 4;

const int InputManager::ATTACHMENT_NONE = 0;

const int InputManager::KEY_JOYSTICK_BUTTON1 = 0x101;

const int InputManager::KEY_JOYSTICK_BUTTON2 = 0x102;

const int InputManager::KEY_JOYSTICK_BUTTON3 = 0x103;

const int InputManager::KEY_JOYSTICK_BUTTON4 = 0x104;

const int InputManager::KEY_MOUSE_LEFT = 0x105;

const int InputManager::KEY_MOUSE_MIDDLE = 0x107;

const int InputManager::KEY_MOUSE_RIGHT = 0x106;

const int InputManager::KEY_NONE = 0x00;

// Configurable properties
CF_INT_IMPL(InputManager, WheelLines)

InputManager::InputManager()
{
	ASSERTION_COBJECT(this);

	eventCount = 0;
	eventQuantities = NULL;
	ensureEventCount(16);
}

InputManager::~InputManager()
{
	ASSERTION_COBJECT(this);

	if (eventQuantities) {delete eventQuantities; eventQuantities = NULL;}
}

void InputManager::attachJoystickXMoveGlobal(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	globalAttachments.attachJoystickXMove(eventNumber, factor);
}

void InputManager::attachJoystickXPositionGlobal(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	globalAttachments.attachJoystickXPosition(eventNumber, factor);
}

void InputManager::attachJoystickYMoveGlobal(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	globalAttachments.attachJoystickYMove(eventNumber, factor);
}

void InputManager::attachJoystickYPositionGlobal(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	globalAttachments.attachJoystickYPosition(eventNumber, factor);
}

void InputManager::attachKeyOrButtonGlobal(int eventNumber, int keyCode, int type, long factor)
{
	ASSERTION_COBJECT(this);
	globalAttachments.attachKey(eventNumber, keyCode, type, factor);
}

void InputManager::attachMouseXMoveGlobal(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	globalAttachments.attachMouseXMove(eventNumber, factor);
}

void InputManager::attachMouseYMoveGlobal(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	globalAttachments.attachMouseYMove(eventNumber, factor);
}

void InputManager::attachMouseWheelGlobal(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);
	globalAttachments.attachMouseWheel(eventNumber, factor);
}

void InputManager::Configure(ConfigSection * section, bool write)
{
	ASSERTION_COBJECT(this);
	section->Configure("WheelLines", WheelLines, write, 3);
}

void InputManager::detachAllGlobal()
{
	ASSERTION_COBJECT(this);
	globalAttachments.detachAll();
}

void InputManager::detachGlobal(int eventNumber)
{
	ASSERTION_COBJECT(this);
	globalAttachments.detach(eventNumber);
}

void InputManager::detachJoystickXAxisGlobal()
{
	ASSERTION_COBJECT(this);
	globalAttachments.detachJoystickXAxis();
}

void InputManager::detachJoystickYAxisGlobal()
{
	ASSERTION_COBJECT(this);
	globalAttachments.detachJoystickYAxis();
}

void InputManager::detachKeyOrButtonGlobal(int keyCode)
{
	ASSERTION_COBJECT(this);
	globalAttachments.detachKey(keyCode);
}

void InputManager::detachMouseXMoveGlobal()
{
	ASSERTION_COBJECT(this);
	globalAttachments.detachMouseXMove();
}

void InputManager::detachMouseYMoveGlobal()
{
	ASSERTION_COBJECT(this);
	globalAttachments.detachMouseYMove();
}

void InputManager::detachMouseWheelGlobal()
{
	ASSERTION_COBJECT(this);
	globalAttachments.detachMouseWheel();
}

void InputManager::ensureEventCount(int requestedNumber)
{
	ASSERTION_COBJECT(this);

	if (requestedNumber < eventCount)
		return;

	long * oldQuantities = eventQuantities;
	int oldEventCount = eventCount;

	eventCount = (requestedNumber + 16) / 16 * 16;

	if ((eventQuantities = new long[eventCount]) == NULL)
		throw EOUTOFMEMORY;
	memset(eventQuantities, 0, sizeof(long) * eventCount);

	if (oldQuantities != NULL)
	{
		memcpy(eventQuantities, oldQuantities, sizeof(long) * oldEventCount);
		delete oldQuantities;
	}
}

int InputManager::getAttachmentKeyOrButton(int eventNumber, bool positive, bool negative)
{
	ASSERTION_COBJECT(this);
	return globalAttachments.getAttachmentKey(eventNumber, positive, negative);
}

InputAttachment & InputManager::getEffectiveJoystickXAxisAttachment()
{
	ASSERTION_COBJECT(this);

	Scene * scene = Desktop::getInstance()->getActiveScene();
	if (scene != NULL)
	{
		InputAttachment & attachment = scene->sceneAttachments.joystickXAxisAttachment;
		if (attachment.type != InputManager::ATTACHMENT_NONE)
			return attachment;
	}

	return globalAttachments.joystickXAxisAttachment;
}

InputAttachment & InputManager::getEffectiveJoystickYAxisAttachment()
{
	ASSERTION_COBJECT(this);

	Scene * scene = Desktop::getInstance()->getActiveScene();
	if (scene != NULL)
	{
		InputAttachment & attachment = scene->sceneAttachments.joystickYAxisAttachment;
		if (attachment.type != InputManager::ATTACHMENT_NONE)
			return attachment;
	}

	return globalAttachments.joystickYAxisAttachment;
}

InputAttachment & InputManager::getEffectiveKeyOrButtonAttachment(int keyCode)
{
	ASSERTION_COBJECT(this);

	Scene * scene = Desktop::getInstance()->getActiveScene();
	if (scene != NULL)
	{
		InputAttachment & attachment = scene->sceneAttachments.keyAttachments[keyCode];
		if (attachment.type != InputManager::ATTACHMENT_NONE)
			return attachment;
	}

	return globalAttachments.keyAttachments[keyCode];
}

InputAttachment & InputManager::getEffectiveMouseWheelAttachment()
{
	ASSERTION_COBJECT(this);

	Scene * scene = Desktop::getInstance()->getActiveScene();
	if (scene != NULL)
	{
		InputAttachment & attachment = scene->sceneAttachments.mouseWheelAttachment;
		if (attachment.type != InputManager::ATTACHMENT_NONE)
			return attachment;
	}

	return globalAttachments.mouseWheelAttachment;
}

InputAttachment & InputManager::getEffectiveMouseXAxisAttachment()
{
	ASSERTION_COBJECT(this);

	Scene * scene = Desktop::getInstance()->getActiveScene();
	if (scene != NULL)
	{
		InputAttachment & attachment = scene->sceneAttachments.mouseXAxisAttachment;
		if (attachment.type != InputManager::ATTACHMENT_NONE)
			return attachment;
	}

	return globalAttachments.mouseXAxisAttachment;
}

InputAttachment & InputManager::getEffectiveMouseYAxisAttachment()
{
	ASSERTION_COBJECT(this);

	Scene * scene = Desktop::getInstance()->getActiveScene();
	if (scene != NULL)
	{
		InputAttachment & attachment = scene->sceneAttachments.mouseYAxisAttachment;
		if (attachment.type != InputManager::ATTACHMENT_NONE)
			return attachment;
	}

	return globalAttachments.mouseYAxisAttachment;
}

int InputManager::getEventCount()
{
	ASSERTION_COBJECT(this);
	return eventCount;
}

long InputManager::getEventQuantity(int eventNumber)
{
	ASSERTION_COBJECT(this);

	ensureEventCount(eventNumber);

	long currentQuantity = eventQuantities[eventNumber];
	eventQuantities[eventNumber] = 0;

	return currentQuantity;
}

InputAttachment & InputManager::getGlobalJoystickXAxisAttachment()
{
	ASSERTION_COBJECT(this);
	return globalAttachments.joystickXAxisAttachment;
}

InputAttachment & InputManager::getGlobalJoystickYAxisAttachment()
{
	ASSERTION_COBJECT(this);
	return globalAttachments.joystickYAxisAttachment;
}

InputAttachment & InputManager::getGlobalKeyOrButtonAttachment(int keyCode)
{
	ASSERTION_COBJECT(this);
	return globalAttachments.keyAttachments[keyCode];
}

InputAttachment & InputManager::getGlobalMouseWheelAttachment()
{
	ASSERTION_COBJECT(this);
	return globalAttachments.mouseWheelAttachment;
}

InputAttachment & InputManager::getGlobalMouseXAxisAttachment()
{
	ASSERTION_COBJECT(this);
	return globalAttachments.mouseXAxisAttachment;
}

InputAttachment & InputManager::getGlobalMouseYAxisAttachment()
{
	ASSERTION_COBJECT(this);
	return globalAttachments.mouseYAxisAttachment;
}

JoystickHandler * InputManager::getJoystickHandler()
{
	ASSERTION_COBJECT(this);
	return Desktop::getInstance()->getPlatformAdapter()->getJoystickHandler();
}

KeyboardHandler * InputManager::getKeyboardHandler()
{
	ASSERTION_COBJECT(this);
	return Desktop::getInstance()->getPlatformAdapter()->getKeyboardHandler();
}

int InputManager::getKeyboardRepeatDelay()
{
	ASSERTION_COBJECT(this);
	return getKeyboardHandler()->GetKeyboardRepeatDelay();
}

int InputManager::getKeyboardRepeatRate()
{
	ASSERTION_COBJECT(this);
	return getKeyboardHandler()->GetKeyboardRepeatDelay();
}

MouseHandler * InputManager::getMouseHandler()
{
	ASSERTION_COBJECT(this);
	return Desktop::getInstance()->getPlatformAdapter()->getMouseHandler();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(InputManager, CObject);
#endif

void InputManager::increaseEventQuantity(int eventNumber, long quantity)
{
	ASSERTION_COBJECT(this);

	ensureEventCount(eventNumber);
	eventQuantities[eventNumber] += quantity;
}

bool InputManager::isJoystickEnabled()
{
	ASSERTION_COBJECT(this);
	return getJoystickHandler()->isEnabled();
}

bool InputManager::isKeyboardEnabled()
{
	ASSERTION_COBJECT(this);
	return getKeyboardHandler()->isEnabled();
}

bool InputManager::isMouseEnabled()
{
	ASSERTION_COBJECT(this);
	return getMouseHandler()->isEnabled();
}

long InputManager::peekEventQuantity(int eventNumber)
{
	ASSERTION_COBJECT(this);

	ensureEventCount(eventNumber);
	return eventQuantities[eventNumber];
}

void InputManager::resetEventQuantities()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<eventCount; i++)
		eventQuantities[i] = 0;
}

void InputManager::resetEventQuantity(int eventNumber)
{
	ASSERTION_COBJECT(this);

	ensureEventCount(eventNumber);
	eventQuantities[eventNumber] = 0;
}

void InputManager::resetHelds()
{
	ASSERTION_COBJECT(this);

	getJoystickHandler()->resetHelds();
	getKeyboardHandler()->resetHelds();
	getMouseHandler()->resetHelds();
}

void InputManager::setJoystickEnabled(bool joystickEnabled)
{
	ASSERTION_COBJECT(this);
	getJoystickHandler()->setEnabled(joystickEnabled);
}

void InputManager::setKeyboardEnabled(bool keyboardEnabled)
{
	ASSERTION_COBJECT(this);
	getKeyboardHandler()->setEnabled(keyboardEnabled);
}

void InputManager::setKeyboardRepeatDelay(int keyboardRepeatDelay)
{
	ASSERTION_COBJECT(this);
	getKeyboardHandler()->SetKeyboardRepeatDelay(keyboardRepeatDelay);
}

void InputManager::setKeyboardRepeatRate(int keyboardRepeatRate)
{
	ASSERTION_COBJECT(this);
	getKeyboardHandler()->SetKeyboardRepeatRate(keyboardRepeatRate);
}

void InputManager::setMouseEnabled(bool mouseEnabled)
{
	ASSERTION_COBJECT(this);
	getMouseHandler()->setEnabled(mouseEnabled);
}

void InputManager::update()
{
	ASSERTION_COBJECT(this);

	getJoystickHandler()->update();
	getKeyboardHandler()->update();
	getMouseHandler()->update();
}
