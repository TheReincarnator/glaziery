/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


void InputAttachmentSet::attachJoystickXMove(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	joystickXAxisAttachment.type = InputManager::ATTACHMENT_AXISMOVEMENT;
	joystickXAxisAttachment.eventNumber = eventNumber;
	joystickXAxisAttachment.factor = factor;
}

void InputAttachmentSet::attachJoystickXPosition(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	joystickXAxisAttachment.type = InputManager::ATTACHMENT_AXISPOSITION;
	joystickXAxisAttachment.eventNumber = eventNumber;
	joystickXAxisAttachment.factor = factor;
}

void InputAttachmentSet::attachJoystickYMove(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	joystickYAxisAttachment.type = InputManager::ATTACHMENT_AXISMOVEMENT;
	joystickYAxisAttachment.eventNumber = eventNumber;
	joystickYAxisAttachment.factor = factor;
}

void InputAttachmentSet::attachJoystickYPosition(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	joystickYAxisAttachment.type = InputManager::ATTACHMENT_AXISPOSITION;
	joystickYAxisAttachment.eventNumber = eventNumber;
	joystickYAxisAttachment.factor = factor;
}

void InputAttachmentSet::attachKey(int eventNumber, int keyCode, int type, long factor)
{
	ASSERTION_COBJECT(this);

	if (keyCode < 0 || keyCode >= GL_KEYCODECOUNT)
		throw EILLEGALARGUMENT("Invalid key code");

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	keyAttachments[keyCode].type = type;
	keyAttachments[keyCode].eventNumber = eventNumber;
	keyAttachments[keyCode].factor = factor;
}

void InputAttachmentSet::attachMouseXMove(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	mouseXAxisAttachment.type = InputManager::ATTACHMENT_AXISMOVEMENT;
	mouseXAxisAttachment.eventNumber = eventNumber;
	mouseXAxisAttachment.factor = factor;
}

void InputAttachmentSet::attachMouseYMove(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	mouseYAxisAttachment.type = InputManager::ATTACHMENT_AXISMOVEMENT;
	mouseYAxisAttachment.eventNumber = eventNumber;
	mouseYAxisAttachment.factor = factor;
}

void InputAttachmentSet::attachMouseWheel(int eventNumber, long factor)
{
	ASSERTION_COBJECT(this);

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	mouseWheelAttachment.type = InputManager::ATTACHMENT_AXISMOVEMENT;
	mouseWheelAttachment.eventNumber = eventNumber;
	mouseWheelAttachment.factor = factor;
}

void InputAttachmentSet::detach(int eventNumber)
{
	ASSERTION_COBJECT(this);

	InputManager & inputManager = Desktop::getInstance()->getInputManager();
	inputManager.ensureEventCount(eventNumber);

	for (int i = 0; i < GL_KEYCODECOUNT; i++)
		if (keyAttachments[i].eventNumber == eventNumber)
			detachKey(i);

	if (joystickXAxisAttachment.eventNumber == eventNumber)
		detachJoystickXAxis();

	if (joystickYAxisAttachment.eventNumber == eventNumber)
		detachJoystickYAxis();

	if (mouseXAxisAttachment.eventNumber == eventNumber)
		detachMouseXMove();

	if (mouseYAxisAttachment.eventNumber == eventNumber)
		detachMouseYMove();

	if (mouseWheelAttachment.eventNumber == eventNumber)
		detachMouseWheel();
}

void InputAttachmentSet::detachAll()
{
	ASSERTION_COBJECT(this);

	for (int i = 0; i < GL_KEYCODECOUNT; i++)
		detachKey(i);

	detachJoystickXAxis();
	detachJoystickYAxis();

	detachMouseXMove();
	detachMouseYMove();
	detachMouseWheel();
}

void InputAttachmentSet::detachJoystickXAxis()
{
	ASSERTION_COBJECT(this);
	joystickXAxisAttachment.type = InputManager::ATTACHMENT_NONE;
}

void InputAttachmentSet::detachJoystickYAxis()
{
	ASSERTION_COBJECT(this);
	joystickYAxisAttachment.type = InputManager::ATTACHMENT_NONE;
}

void InputAttachmentSet::detachKey(int keyCode)
{
	ASSERTION_COBJECT(this);

	if (keyCode < 0 || keyCode >= GL_KEYCODECOUNT)
		throw EILLEGALARGUMENT("Invalid key code");

	keyAttachments[keyCode].type = InputManager::ATTACHMENT_NONE;
}

void InputAttachmentSet::detachMouseWheel()
{
	ASSERTION_COBJECT(this);
	mouseWheelAttachment.type = InputManager::ATTACHMENT_NONE;
}

void InputAttachmentSet::detachMouseXMove()
{
	ASSERTION_COBJECT(this);
	mouseXAxisAttachment.type = InputManager::ATTACHMENT_NONE;
}

void InputAttachmentSet::detachMouseYMove()
{
	ASSERTION_COBJECT(this);
	mouseYAxisAttachment.type = InputManager::ATTACHMENT_NONE;
}

int InputAttachmentSet::getAttachmentKey(int eventNumber, bool positive, bool negative)
{
	ASSERTION_COBJECT(this);

	int result = -1;
	for (int keyCode = 0; keyCode < GL_KEYCODECOUNT; keyCode++)
	{
		if (keyAttachments[keyCode].eventNumber != eventNumber)
			continue;

		int type = keyAttachments[keyCode].type;
		if (type != InputManager::ATTACHMENT_KEYPRESS
			&& type != InputManager::ATTACHMENT_KEYRELEASE
			&& type != InputManager::ATTACHMENT_KEYPRESSRELEASE
			&& type != InputManager::ATTACHMENT_KEYPRESSREPEAT
			&& type != InputManager::ATTACHMENT_KEYHOLD)
			continue;

		int factor = keyAttachments[keyCode].factor;
		if (factor < 0 && negative || factor > 0 && positive)
		{
			if (result >= 0)
				return NULL;
			else
				result = keyCode;
		}
	}

	return result;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(InputAttachmentSet, CObject);
#endif
