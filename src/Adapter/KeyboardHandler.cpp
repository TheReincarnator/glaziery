/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int KeyboardHandler::BUFFER_SIZE = 16;

// Configurable properties
CF_INT_IMPL(KeyboardHandler, KeyboardRepeatDelay)
CF_INT_IMPL(KeyboardHandler, KeyboardRepeatRate)

KeyboardHandler::KeyboardHandler()
{
	ASSERTION_COBJECT(this);

	enabled = true;
	memset(lastFrameKeyHelds, 0, sizeof(lastFrameKeyHelds));
}

KeyboardHandler::~KeyboardHandler()
{
	ASSERTION_COBJECT(this);
}

void KeyboardHandler::Configure(ConfigSection * section, bool write)
{
	ASSERTION_COBJECT(this);

	section->Configure("KeyboardRepeatDelay", KeyboardRepeatDelay, write, 300);
	section->Configure("KeyboardRepeatRate", KeyboardRepeatRate, write, 45);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(KeyboardHandler, CObject);
#endif

bool KeyboardHandler::increaseManagerEventQuantities(int scanCode, bool held,
	bool pressed, bool released, int repeats)
{
	ASSERTION_COBJECT(this);

	bool consumed = false;

	Desktop * desktop = Desktop::getInstance();
	InputManager & manager = desktop->getInputManager();
	const InputAttachment & attachment = manager.getEffectiveKeyOrButtonAttachment(scanCode);

	unsigned long timeStep = desktop->getTimeStep();

	if (held && attachment.type == InputManager::ATTACHMENT_KEYHOLD)
	{
		manager.increaseEventQuantity(attachment.eventNumber, timeStep * attachment.factor);
		consumed = true;
	}

	if (pressed && (attachment.type == InputManager::ATTACHMENT_KEYPRESS
		|| attachment.type == InputManager::ATTACHMENT_KEYPRESSRELEASE
		|| attachment.type == InputManager::ATTACHMENT_KEYPRESSREPEAT))
	{
		manager.increaseEventQuantity(attachment.eventNumber, attachment.factor);
		consumed = true;
	}

	if (released && (attachment.type == InputManager::ATTACHMENT_KEYPRESSRELEASE
		|| attachment.type == InputManager::ATTACHMENT_KEYRELEASE))
	{
		manager.increaseEventQuantity(attachment.eventNumber, attachment.factor);
		consumed = true;
	}

	if (repeats > 0 && attachment.type == InputManager::ATTACHMENT_KEYPRESSREPEAT)
	{
		manager.increaseEventQuantity(attachment.eventNumber, attachment.factor * repeats);
		consumed = true;
	}

	return consumed;
}

void KeyboardHandler::initialize()
{
	ASSERTION_COBJECT(this);
	LOGPRINTF0(LOG_INFO, "Initializing keyboard");
}

bool KeyboardHandler::isEnabled()
{
	ASSERTION_COBJECT(this);
	return enabled;
}

void KeyboardHandler::resetHelds()
{
	ASSERTION_COBJECT(this);
	memset(lastFrameKeyHelds, 0, sizeof(lastFrameKeyHelds));
}

void KeyboardHandler::setEnabled(bool enabled)
{
	ASSERTION_COBJECT(this);
	this->enabled = enabled;
}

void KeyboardHandler::updateExclusive()
{
	ASSERTION_COBJECT(this);
}
