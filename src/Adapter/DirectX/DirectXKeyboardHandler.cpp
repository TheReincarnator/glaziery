/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const char DirectXKeyboardHandler::REPEATING_KEY_CODES[257] =
	/* 0x00 */ "R.RRRRRR"
	/* 0x08 */ "RRRRRRRR"
	/* 0x10 */ "RRRRRRRR"
	/* 0x18 */ "RRRRR.RR"
	/* 0x20 */ "RRRRRRRR"
	/* 0x28 */ "RR.RRRRR"
	/* 0x30 */ "RRRRRR.R"
	/* 0x38 */ ".R.RRRRR"
	/* 0x40 */ "RRRRRR.R"
	/* 0x48 */ "RRRRRRRR"
	/* 0x50 */ "RRRRRRRR"
	/* 0x58 */ "RRRRRRRR"
	/* 0x60 */ "RRRRRRRR"
	/* 0x68 */ "RRRRRRRR"
	/* 0x70 */ "RRRRRRRR"
	/* 0x78 */ "RRRRRRRR"
	/* 0x80 */ "RRRRRRRR"
	/* 0x88 */ "RRRRRRRR"
	/* 0x90 */ ".RRRRRRR"
	/* 0x98 */ "R.RRR.RR"
	/* 0xA0 */ ".R.R.RRR"
	/* 0xA8 */ "RRRRRRRR"
	/* 0xB0 */ "RR.RRRR."
	/* 0xB8 */ ".RRRRRRR"
	/* 0xC0 */ "RRRRRRRR"
	/* 0xC8 */ "RRRRRRRR"
	/* 0xD0 */ "RRRRRRRR"
	/* 0xD8 */ "RRR....."
	/* 0xE0 */ "RRR.R..."
	/* 0xE8 */ "......RR"
	/* 0xF0 */ "RRRRRRRR"
	/* 0xF8 */ "RRRRRRRR";

DirectXKeyboardHandler::DirectXKeyboardHandler()
{
	ASSERTION_COBJECT(this);

	keyboardAcquired = false;
	keyboardDevice = NULL;
	::ZeroMemory(keyStates, sizeof(keyStates));
	::ZeroMemory(pressedCharacters1, sizeof(pressedCharacters1));
	::ZeroMemory(pressedCharacters2, sizeof(pressedCharacters2));
	previousDeadKey = 0;
}

void DirectXKeyboardHandler::emitEvent(int scanCode, bool held, bool pressed, bool released, int repeats)
{
	ASSERTION_COBJECT(this);

	int i; // loop variable

	bool shift = isShiftPressed();
	bool control = isControlPressed();
	bool alt = isAltPressed();

	Desktop * desktop = Desktop::getInstance();
	bool keyStrokeMode = desktop->isKeyStrokeMode();
	if (desktop->isTutorialMode())
	{
		if (pressed && (scanCode == DIK_SPACE || scanCode == DIK_RETURN || scanCode == DIK_ESCAPE))
		{
			if (desktop->isModal())
			{
				InputEvent * event;
				if ((event = new SpecialEvent(false, false, scanCode == DIK_ESCAPE ? DIK_ESCAPE : DIK_RETURN,
					false, true, false, 0, scanCode == DIK_ESCAPE ? SpecialEvent::EVENT_CANCEL
					: SpecialEvent::EVENT_ENTER)) == NULL)
					throw EOUTOFMEMORY;
				desktop->addEvent(event);
			}

			desktop->onTutorialStep(scanCode == DIK_ESCAPE);
		}

		pressedCharacters1[scanCode] = 0;
		pressedCharacters2[scanCode] = 0;
		return;
	}

	// In the key stroke mode, ignore tabs (used to change focus) and numlock (problematic with some devices)
	if (keyStrokeMode && scanCode != DIK_TAB && scanCode != DIK_NUMLOCK)
	{
		// We are in the key stroke mode, so emit a key stroke event

		if (pressed)
		{
			InputEvent * event;
			if ((event = new KeyStrokeEvent(shift, control, scanCode, false, true, false, 0)) == NULL)
				throw EOUTOFMEMORY;

			desktop->addEvent(event);
		}

		pressedCharacters1[scanCode] = 0;
		pressedCharacters2[scanCode] = 0;
		return;
	}

	// We are not in the key stroke mode, find a special event

	SpecialEvent::Type eventType = SpecialEvent::EVENT_NONE;
	if (scanCode == DIK_A)
	{
		if (!shift && control)
			eventType = SpecialEvent::EVENT_SELECT_ALL;
	}
	else if (scanCode == DIK_BACKSPACE)
		eventType = SpecialEvent::EVENT_BACK_SPACE;
	else if (scanCode == DIK_C)
	{
		if (!shift && control)
			eventType = SpecialEvent::EVENT_COPY;
	}
	else if (scanCode == DIK_DELETE)
	{
		if (!shift && !control)
			eventType = SpecialEvent::EVENT_DELETE;
		else if (shift && !control)
			eventType = SpecialEvent::EVENT_CUT;
	}
	else if (scanCode == DIK_DOWN)
		eventType = SpecialEvent::EVENT_MOVE_DOWN;
	else if (scanCode == DIK_END)
		eventType = SpecialEvent::EVENT_MOVE_TO_END;
	else if (scanCode == DIK_ESCAPE)
		eventType = SpecialEvent::EVENT_CANCEL;
	else if (scanCode == DIK_F2)
	{
		if (!shift && !control)
			eventType = SpecialEvent::EVENT_EDIT;
	}
	else if (scanCode == DIK_INSERT)
	{
		if (shift && !control)
			eventType = SpecialEvent::EVENT_PASTE;
		else if (!shift && control)
			eventType = SpecialEvent::EVENT_COPY;
	}
	else if (scanCode == DIK_HOME)
		eventType = SpecialEvent::EVENT_MOVE_TO_START;
	else if (scanCode == DIK_LEFT)
		eventType = SpecialEvent::EVENT_MOVE_LEFT;
	else if (scanCode == DIK_NUMPADENTER)
		eventType = SpecialEvent::EVENT_ENTER;
	else if (scanCode == DIK_PGUP)
		eventType = SpecialEvent::EVENT_PAGE_UP;
	else if (scanCode == DIK_PGDN)
		eventType = SpecialEvent::EVENT_PAGE_DOWN;
	else if (scanCode == DIK_RETURN)
		eventType = SpecialEvent::EVENT_ENTER;
	else if (scanCode == DIK_RIGHT)
		eventType = SpecialEvent::EVENT_MOVE_RIGHT;
	else if (scanCode == DIK_TAB)
	{
		if (alt)
		{
			pressedCharacters1[scanCode] = 0;
			pressedCharacters2[scanCode] = 0;
			return;
		}

		if (!shift && !control)
			eventType = SpecialEvent::EVENT_TAB;
		else if (shift && !control)
			eventType = SpecialEvent::EVENT_BACK_TAB;
		else if (!shift && control)
			eventType = SpecialEvent::EVENT_SECONDARY_TAB;
		else if (shift && control)
			eventType = SpecialEvent::EVENT_SECONDARY_BACK_TAB;
	}
	else if (scanCode == DIK_V)
	{
		if (!shift && control)
			eventType = SpecialEvent::EVENT_PASTE;
	}
	else if (scanCode == DIK_UP)
		eventType = SpecialEvent::EVENT_MOVE_UP;
	else if (scanCode == DIK_X)
	{
		if (!shift && control)
			eventType = SpecialEvent::EVENT_CUT;
	}

	if (eventType != SpecialEvent::EVENT_NONE)
	{
		// We have a special event

		InputEvent * event;
		if ((event = new SpecialEvent(shift, control, scanCode, held, pressed, released,
			repeats, eventType)) == NULL)
			throw EOUTOFMEMORY;
		desktop->addEvent(event);

		pressedCharacters1[scanCode] = 0;
		pressedCharacters2[scanCode] = 0;
		return;
	}

	// We have no special event, find the virtual key and real characters

	char characters[16];
	::ZeroMemory(characters, sizeof(characters));
	if (pressed)
	{
		// Note: RALT is equivalent to LALT and LCONTROL
		unsigned char leftControl = keyStates[DIK_LCONTROL] | keyStates[DIK_RALT];
		unsigned char rightControl = keyStates[DIK_RCONTROL];
		unsigned char leftAlt = keyStates[DIK_LALT] | keyStates[DIK_RALT];
		unsigned char rightAlt = keyStates[DIK_RALT];

		unsigned char states[256];
		::ZeroMemory(states, sizeof(states));
		states[VK_SHIFT] = keyStates[DIK_LSHIFT] | keyStates[DIK_RSHIFT];
		states[VK_LSHIFT] = keyStates[DIK_LSHIFT];
		states[VK_RSHIFT] = keyStates[DIK_RSHIFT];
		states[VK_CONTROL] = leftControl | rightControl;
		states[VK_LCONTROL] = leftControl;
		states[VK_RCONTROL] = rightControl;
		states[VK_MENU] = leftAlt | rightAlt;
		states[VK_LMENU] = leftAlt;
		states[VK_RMENU] = rightAlt;
		states[VK_LWIN] = keyStates[DIK_LWIN];
		states[VK_RWIN] = keyStates[DIK_RWIN];

		char virtualKey = ::MapVirtualKey(scanCode, 1);
		int count = ::ToAscii(virtualKey, scanCode, states, (unsigned short int *) characters, 0);
		if (count < 0 || count > 2)
			count = 0;
		for (i=count; i<2; i++)
			characters[i] = 0;

		// Someone steals dead keys from the keyboard layout (and it's not us!),
		// so we have to reimplement dead key logic.

		if (previousDeadKey && count > 0)
		{
			characters[1] = 0;
			if (characters[0] == ' ')
				characters[0] = previousDeadKey;
			else if (previousDeadKey == (char) '´')
			{
				if (characters[0] == 'a') characters[0] = 'á';
				else if (characters[0] == 'e') characters[0] = 'é';
				else if (characters[0] == 'i') characters[0] = 'í';
				else if (characters[0] == 'o') characters[0] = 'ó';
				else if (characters[0] == 'u') characters[0] = 'ú';
				else if (characters[0] == 'y') characters[0] = 'ý';
				else if (characters[0] == 'A') characters[0] = 'Á';
				else if (characters[0] == 'E') characters[0] = 'É';
				else if (characters[0] == 'I') characters[0] = 'Í';
				else if (characters[0] == 'O') characters[0] = 'Ó';
				else if (characters[0] == 'U') characters[0] = 'Ú';
				else if (characters[0] == 'Y') characters[0] = 'Ý';
				else
				{
					characters[1] = characters[0];
					characters[0] = previousDeadKey;
				}
			}
			else if (previousDeadKey == (char) '`')
			{
				if (characters[0] == 'a') characters[0] = 'à';
				else if (characters[0] == 'e') characters[0] = 'è';
				else if (characters[0] == 'i') characters[0] = 'ì';
				else if (characters[0] == 'o') characters[0] = 'ò';
				else if (characters[0] == 'u') characters[0] = 'ù';
				else if (characters[0] == 'A') characters[0] = 'À';
				else if (characters[0] == 'E') characters[0] = 'È';
				else if (characters[0] == 'I') characters[0] = 'Ì';
				else if (characters[0] == 'O') characters[0] = 'Ò';
				else if (characters[0] == 'U') characters[0] = 'Ù';
				else
				{
					characters[1] = characters[0];
					characters[0] = previousDeadKey;
				}
			}
			else if (previousDeadKey == (char) '^')
			{
				if (characters[0] == 'a') characters[0] = 'â';
				else if (characters[0] == 'e') characters[0] = 'ê';
				else if (characters[0] == 'i') characters[0] = 'î';
				else if (characters[0] == 'o') characters[0] = 'ô';
				else if (characters[0] == 'u') characters[0] = 'û';
				else if (characters[0] == 'A') characters[0] = 'Â';
				else if (characters[0] == 'E') characters[0] = 'Ê';
				else if (characters[0] == 'I') characters[0] = 'Î';
				else if (characters[0] == 'O') characters[0] = 'Ô';
				else if (characters[0] == 'U') characters[0] = 'Û';
				else
				{
					characters[1] = characters[0];
					characters[0] = previousDeadKey;
				}
			}

			previousDeadKey = 0;
		}
		else if (characters[0] == (char) '´' || characters[0] == (char) '`' || characters[0] == (char) '^')
		{
			previousDeadKey = characters[0];
			characters[0] = 0;
			characters[1] = 0;
		}

		// Some cleanup

		for (i=0; i<2; i++)
			if (!String::IsValid((unsigned char) characters[i]))
				characters[i] = 0;

		if (characters[1] && !characters[0])
		{
			characters[0] = characters[1];
			characters[1] = 0;
		}

		pressedCharacters1[scanCode] = characters[0];
		pressedCharacters2[scanCode] = characters[1];
	}
	else
	{
		characters[0] = pressedCharacters1[scanCode];
		characters[1] = pressedCharacters2[scanCode];
	}

	// Now determine whether the event is a hot key

	if (alt && !control && !shift)
	{
		// Emit a hot key event

		InputEvent * event;
		if ((event = new HotKeyEvent(shift, control, scanCode, held, pressed, released, repeats,
			(unsigned char) characters[0], (unsigned char) characters[1])) == NULL)
			throw EOUTOFMEMORY;
		desktop->addEvent(event);
	}
	else
	{
		// Emit a character event

		InputEvent * event;
		if ((event = new CharacterEvent(shift, control, scanCode, held, pressed, released, repeats,
			(unsigned char) characters[0], (unsigned char) characters[1])) == NULL)
			throw EOUTOFMEMORY;
		desktop->addEvent(event);
	}

	LOGIFLEVEL_START(LOG_DEBUG)
		String message;
		if (pressed)
			message += message.IsEmpty() ? "Emitting pressed" : " & pressed";
		if (held)
			message += message.IsEmpty() ? "Emitting held" : " & held";
		if (released)
			message += message.IsEmpty() ? "Emitting released" : " & released";
		message += message.IsEmpty() ? "Emitting noop key event" : " event";

		String part;
		part.Format(" (%d repeats): ", repeats);
		message += part;
		
		if (characters[0])
		{
			part.Format("%c (%d)", (unsigned char) characters[0], (unsigned char) characters[0]);
			message += part;
			if (characters[1])
			{
				part.Format(", %c (%d)", (unsigned char) characters[1], (unsigned char) characters[1]);
				message += part;
			}
		}
		else
			message += "No characters";

		part.Format(", scan code %d", scanCode);
		message += part;

		part = "";
		if (shift)
			part += part.IsEmpty() ? ", with shift" : " & shift";
		if (control)
			part += part.IsEmpty() ? ", with control" : " & control";
		if (alt)
			part += part.IsEmpty() ? ", with alt" : " & alt";
		message += part;

		LOGPRINTF1(LOG_DEBUG, "%s", (const char *) message);
	LOGIFLEVEL_END
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(DirectXKeyboardHandler, KeyboardHandler);
#endif

void DirectXKeyboardHandler::initialize()
{
	ASSERTION_COBJECT(this);

	KeyboardHandler::initialize();

	// Initializing DirectX

	Desktop * desktop = Desktop::getInstance();
	DirectXPlatformAdapter * adapter = (DirectXPlatformAdapter *) desktop->getPlatformAdapter();

	HRESULT result;
	if (FAILED(result = adapter->getDirectInput()->CreateDevice(GUID_SysKeyboard,
		&keyboardDevice, NULL)))
	{
		keyboardDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot create keyboard device", result);
	}

	if (FAILED(result = keyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		keyboardDevice->Release();
		keyboardDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot set keyboard data format", result);
	}

	DIPROPDWORD dipdw = {{sizeof(DIPROPDWORD), sizeof(DIPROPHEADER), 0, DIPH_DEVICE},
		BUFFER_SIZE};
	if (FAILED(result = keyboardDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
	{
		keyboardDevice->Release();
		keyboardDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot set keyboard's buffer size", result);
	}

	if (FAILED(result = keyboardDevice->SetCooperativeLevel(adapter->getWindowHandle(),
		DISCL_FOREGROUND | (desktop->isExclusive() ? DISCL_NOWINKEY | DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE))))
	{
		keyboardDevice->Release();
		keyboardDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot set keyboard's cooperative level", result);
	}

	::SetForegroundWindow(adapter->getWindowHandle());
	::SetFocus(adapter->getWindowHandle());
	adapter->processMessages();

	if (FAILED(result = keyboardDevice->Acquire()))
	{
		LOGPRINTF1(LOG_WARN, "Cannot acquire keyboard device (error code %d)", result);
		keyboardAcquired = false;
	}
	else
		keyboardAcquired = true;

	LOGPRINTF0(LOG_INFO, "Initialized keyboard input handler");
}

bool DirectXKeyboardHandler::isAltPressed()
{
	ASSERTION_COBJECT(this);
	return (keyStates[DIK_LMENU] & 0x80) != 0;
}

bool DirectXKeyboardHandler::isControlPressed()
{
	ASSERTION_COBJECT(this);
	return (keyStates[DIK_LCONTROL] & 0x80) != 0 || (keyStates[DIK_RCONTROL] & 0x80) != 0;
}

bool DirectXKeyboardHandler::isShiftPressed()
{
	ASSERTION_COBJECT(this);
	return (keyStates[DIK_LSHIFT] & 0x80) != 0 || (keyStates[DIK_RSHIFT] & 0x80) != 0;
}

void DirectXKeyboardHandler::shutDown()
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_INFO, "Shutting down keyboard input handler");

	if (keyboardDevice != NULL)
	{
		// Unacquire is not problematic if device is not acquired.
		keyboardDevice->Unacquire();
		keyboardDevice->Release();
		keyboardDevice = NULL;
	}

	LOGPRINTF0(LOG_INFO, "Keyboard input handler shut down");
}

void DirectXKeyboardHandler::update()
{
	ASSERTION_COBJECT(this);

	HRESULT result = keyboardDevice->Poll();
	if (FAILED(result) && result != DI_NOEFFECT)
	{
		if (keyboardAcquired)
		{
			LOGPRINTF1(LOG_WARN, "Lost keyboard device (error code %d)", result);
		}

		if (FAILED(result = keyboardDevice->Acquire()))
			keyboardAcquired = false;
		else
			keyboardAcquired = true;
		return;
	}

	result = keyboardDevice->GetDeviceState(sizeof(keyStates), (void *) &keyStates);
	if (result != DI_OK && result != DI_NOEFFECT)
	{
		if (keyboardAcquired)
		{
			LOGPRINTF1(LOG_WARN, "Lost keyboard device (error code %d)", result);
		}

		if (FAILED(result = keyboardDevice->Acquire()))
			keyboardAcquired = false;
		else
			keyboardAcquired = true;
		return;
	}

	Desktop * desktop = Desktop::getInstance();
	InputManager & manager = desktop->getInputManager();
	bool hasFocusWindowOrPopup = desktop->hasFocusWindowOrPopup();
	bool keyStrokeMode = desktop->isKeyStrokeMode();

	bool shift = isShiftPressed();
	bool control = isControlPressed();
	bool alt = isAltPressed();

	unsigned long lastTime = desktop->getLastTime();
	unsigned long frameTime = desktop->getFrameTime();
	unsigned long timeStep = desktop->getTimeStep();

	if (!enabled)
	{
		for (int scanCode = 0; scanCode < 256; scanCode++)
		{
			bool held = (keyStates[scanCode] & 0x80) != 0;
			bool wasHeld = lastFrameKeyHelds[scanCode] != 0;
			lastFrameKeyHelds[scanCode] = held;

			if (held && !wasHeld)
				keyPressTimes[scanCode] = frameTime - timeStep / 2;
		}

		return;
	}

	for (int scanCode = 0; scanCode < 256; scanCode++)
	{
		bool held = (keyStates[scanCode] & 0x80) != 0;
		bool wasHeld = lastFrameKeyHelds[scanCode] != 0;
		lastFrameKeyHelds[scanCode] = held;

		if (!held && !wasHeld)
			continue;

		bool pressed = false;
		bool released = false;
		int repeats = 0;
		if (held && !wasHeld)
		{
			// The key was pressed down
			pressed = true;

			// Guess the time the key was pressed down
			keyPressTimes[scanCode] = frameTime - timeStep / 2;
		}

		if (held && wasHeld && REPEATING_KEY_CODES[scanCode] == 'R')
		{
			// The key was probably held down a while
			int lastRepeatTime = lastTime - keyPressTimes[scanCode] - GetKeyboardRepeatDelay();
			int lastRepeats = lastRepeatTime >= 0 ? lastRepeatTime / GetKeyboardRepeatRate() + 1 : 0;
			int currentRepeatTime = frameTime - keyPressTimes[scanCode] - GetKeyboardRepeatDelay();
			int currentRepeats = currentRepeatTime >= 0 ? currentRepeatTime / GetKeyboardRepeatRate() + 1 : 0;
			repeats = currentRepeats - lastRepeats;
		}

		if (!held && wasHeld)
		{
			// The key has been released
			released = true;
		}

		if (held || released)
			emitEvent(scanCode, held, pressed, released, repeats);
	}
}

void DirectXKeyboardHandler::updateExclusive()
{
	ASSERTION_COBJECT(this);

	shutDown();
	initialize();
}
