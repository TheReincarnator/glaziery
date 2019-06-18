/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int DirectXMouseHandler::BUFFER_SIZE = 768;

DirectXMouseHandler::DirectXMouseHandler()
{
	ASSERTION_COBJECT(this);

	mouseAcquired = false;
	mouseDevice = NULL;
	wheelDeltaFractions = 0;
}

Vector DirectXMouseHandler::getPointerPosition()
{
	ASSERTION_COBJECT(this);
	return pointerPosition;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(DirectXMouseHandler, MouseHandler);
#endif

void DirectXMouseHandler::initialize()
{
	ASSERTION_COBJECT(this);

	MouseHandler::initialize();

	Desktop * desktop = Desktop::getInstance();
	DirectXPlatformAdapter * adapter = (DirectXPlatformAdapter *) desktop->getPlatformAdapter();

	pointerPosition = desktop->getSize() / 2;

	HRESULT result;
	if (FAILED(result = adapter->getDirectInput()->CreateDevice(GUID_SysMouse,
		&mouseDevice, NULL)))
	{
		mouseDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot create mouse device", result);
	}

	if (FAILED(result = mouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		mouseDevice->Release();
		mouseDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot set mouse data format", result);
	}

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;
	if (FAILED(result = mouseDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph)))
	{
		mouseDevice->Release();
		mouseDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot set mouse axis to be relative", result);
	}

	DIPROPDWORD propertyValue = {{sizeof(DIPROPDWORD), sizeof(DIPROPHEADER), 0, DIPH_DEVICE},
		BUFFER_SIZE};
	if (FAILED(result = mouseDevice->SetProperty(DIPROP_BUFFERSIZE, &propertyValue.diph)))
	{
		mouseDevice->Release();
		mouseDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot set mouse's buffer size", result);
	}

	if (FAILED(result = mouseDevice->SetCooperativeLevel(adapter->getWindowHandle(),
		DISCL_FOREGROUND | (desktop->isExclusive() ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE))))
	{
		mouseDevice->Release();
		mouseDevice = NULL;
		throw EDEVICEERROR_RESULT("Cannot set mouse's cooperative level", result);
	}

	::SetForegroundWindow(adapter->getWindowHandle());
	::SetFocus(adapter->getWindowHandle());
	adapter->processMessages();

	if (FAILED(result = mouseDevice->Acquire()))
	{
		LOGPRINTF1(LOG_WARN, "Cannot acquire mouse device (error code %d)", result);
		mouseAcquired = false;
	}
	else
		mouseAcquired = true;

	LOGPRINTF0(LOG_INFO, "Initialized mouse input handler");
}

void DirectXMouseHandler::setPointerPosition(Vector position)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	DirectXKeyboardHandler * keyboardHandler
		= (DirectXKeyboardHandler *) desktop->getPlatformAdapter()->getKeyboardHandler();

	position.constrain(Vector(), desktop->getSize() - Vector(1, 1));
	if (pointerPosition == position)
		return;

	Vector oldPosition = pointerPosition;
	pointerPosition = position;
	::SetCursorPos(position.x, position.y);

	bool shift = keyboardHandler->isShiftPressed();
	bool control = keyboardHandler->isControlPressed();

	InputEvent * event;
	if ((event = new PointerMoveEvent(shift, control, oldPosition, pointerPosition)) == NULL)
		throw EOUTOFMEMORY;
	desktop->addEvent(event);
}

void DirectXMouseHandler::shutDown()
{
	ASSERTION_COBJECT(this);

	LOGPRINTF0(LOG_INFO, "Shutting down mouse input handler");

	if (mouseDevice != NULL)
	{
		// Unacquire is ok if device is not acquired.
		mouseDevice->Unacquire();
		mouseDevice->Release();
		mouseDevice = NULL;
	}

	LOGPRINTF0(LOG_INFO, "Mouse input handler shut down");
}

void DirectXMouseHandler::update()
{
	ASSERTION_COBJECT(this);

	HRESULT result = mouseDevice->Poll();
	if (result != DI_OK && result != DI_NOEFFECT)
	{
		if (mouseAcquired)
		{
			LOGPRINTF1(LOG_WARN, "Lost mouse device (error code %d)", result);
		}

		if (FAILED(result = mouseDevice->Acquire()))
			mouseAcquired = false;
		else
			mouseAcquired = true;
		return;
	}

	Desktop * desktop = Desktop::getInstance();
	Scene * scene = desktop->getActiveScene();
	DirectXPlatformAdapter * adapter = (DirectXPlatformAdapter *) desktop->getPlatformAdapter();
	DirectXKeyboardHandler * keyboardHandler = (DirectXKeyboardHandler *) adapter->getKeyboardHandler();

	// Update pointer position

	POINT point;
	if (::GetCursorPos(&point))
	{
		Vector newPosition(point.x, point.y);
		if (!desktop->isFullScreen())
		{
			WINDOWINFO windowInfo;
			if (::GetWindowInfo(adapter->getWindowHandle(), &windowInfo))
				newPosition -= Vector(windowInfo.rcClient.left, windowInfo.rcClient.top);
		}

		newPosition.constrain(Vector(), desktop->getSize() - Vector(1, 1));
		if (newPosition != pointerPosition)
		{
			bool shift = keyboardHandler->isShiftPressed();
			bool control = keyboardHandler->isControlPressed();
			Vector oldPosition = pointerPosition;
			pointerPosition = newPosition;

			InputEvent * event;
			if ((event = new PointerMoveEvent(shift, control, oldPosition, newPosition)) == NULL)
				throw EOUTOFMEMORY;
			desktop->addEvent(event);
		}
	}

	// Create and emit input handler events

	InputManager & manager = desktop->getInputManager();
	bool pointerCurrentlyShown = desktop->isPointerCurrentlyShown();
	bool enabledAndNoTutorial = enabled && !desktop->isTutorialMode();
	unsigned long timeStep = desktop->getTimeStep();
	while (true)
	{
		DWORD elementCount = 1;
		DIDEVICEOBJECTDATA data;

		result = mouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA),
			&data, &elementCount, 0);
		if (result != DI_OK && result != DI_NOEFFECT)
		{
			if (mouseAcquired)
			{
				LOGPRINTF1(LOG_WARN, "Lost mouse device (error code %d)", result);
			}

			if (FAILED(result = mouseDevice->Acquire()))
				mouseAcquired = false;
			else
				mouseAcquired = true;
			return;
		}

		if (elementCount == 0)
			break;

		int buttonNo;
		switch (data.dwOfs)
		{
			case DIMOFS_BUTTON0: buttonNo = 0; break;
			case DIMOFS_BUTTON1: buttonNo = 1; break;
			case DIMOFS_BUTTON2: buttonNo = 2; break;

			default: buttonNo = -1;
		}

		if (buttonNo >= 0)
		{
			bool isPressed = (data.dwData & 0x80) != 0;
			bool wasPressed = buttonWasPressed[buttonNo];

			if (enabledAndNoTutorial)
			{
				if (buttonNo == 0)
				{
					if (isPressed && !wasPressed)
					{
						bool shift = keyboardHandler->isShiftPressed();
						bool control = keyboardHandler->isControlPressed();

						InputEvent * event;
						if ((event = new PointerButtonEvent(shift, control, true, true)) == NULL)
							throw EOUTOFMEMORY;
						desktop->addEvent(event);
					}
					else if (!isPressed && wasPressed)
					{
						bool shift = keyboardHandler->isShiftPressed();
						bool control = keyboardHandler->isControlPressed();

						InputEvent * event;
						if ((event = new PointerButtonEvent(shift, control, false, true)) == NULL)
							throw EOUTOFMEMORY;
						desktop->addEvent(event);
					}
				}
				else if (buttonNo == 1)
				{
					if (isPressed && !wasPressed)
					{
						bool shift = keyboardHandler->isShiftPressed();
						bool control = keyboardHandler->isControlPressed();

						InputEvent * event;
						if ((event = new PointerButtonEvent(shift, control, true, false)) == NULL)
							throw EOUTOFMEMORY;
						desktop->addEvent(event);
					}
					else if (!isPressed && wasPressed)
					{
						bool shift = keyboardHandler->isShiftPressed();
						bool control = keyboardHandler->isControlPressed();

						InputEvent * event;
						if ((event = new PointerButtonEvent(shift, control, false, false)) == NULL)
							throw EOUTOFMEMORY;
						desktop->addEvent(event);
					}
				}

				if (!pointerCurrentlyShown)
				{
					const InputAttachment & attachment = manager.getEffectiveKeyOrButtonAttachment(
						InputManager::KEY_MOUSE_LEFT + buttonNo);
					if (isPressed && attachment.type == InputManager::ATTACHMENT_KEYHOLD)
					{
						manager.increaseEventQuantity(attachment.eventNumber,
							timeStep * attachment.factor);
					}
					else if (isPressed && !wasPressed && (attachment.type == InputManager::ATTACHMENT_KEYPRESS
						|| attachment.type == InputManager::ATTACHMENT_KEYPRESSRELEASE))
					{
						manager.increaseEventQuantity(attachment.eventNumber,
							attachment.factor);
					}
					else if (!isPressed && wasPressed && (attachment.type == InputManager::ATTACHMENT_KEYRELEASE))
					{
						manager.increaseEventQuantity(attachment.eventNumber,
							attachment.factor);
					}
				}
			}

			buttonWasPressed[buttonNo] = isPressed;
		}

		if (enabledAndNoTutorial)
			switch (data.dwOfs)
			{
				case DIMOFS_X:
					if (!pointerCurrentlyShown)
					{
						const InputAttachment & attachment = manager.getEffectiveMouseXAxisAttachment();
						if (attachment.type == InputManager::ATTACHMENT_AXISMOVEMENT)
							manager.increaseEventQuantity(attachment.eventNumber,
								attachment.factor * data.dwData);
					}
				break;

				case DIMOFS_Y:
					if (!pointerCurrentlyShown)
					{
						const InputAttachment & attachment = manager.getEffectiveMouseYAxisAttachment();
						if (attachment.type == InputManager::ATTACHMENT_AXISMOVEMENT)
							manager.increaseEventQuantity(attachment.eventNumber,
								attachment.factor * data.dwData);
					}
				break;

				case DIMOFS_Z:
					if (pointerCurrentlyShown)
						wheelDeltaFractions += (long) data.dwData;
					else
					{
						const InputAttachment & attachment = manager.getEffectiveMouseWheelAttachment();
						if (attachment.type == InputManager::ATTACHMENT_AXISMOVEMENT)
							manager.increaseEventQuantity(attachment.eventNumber,
								attachment.factor * data.dwData);
					}
				break;
			}
	}

	// Microsoft sends 120 units per scroll unit, future devices may send fractions of it, so accumulate
	int mouseWheelDeltaIntegers = wheelDeltaFractions / 120;
	if (mouseWheelDeltaIntegers != 0)
	{
		bool shift = keyboardHandler->isShiftPressed();
		bool control = keyboardHandler->isControlPressed();
		wheelDeltaFractions -= mouseWheelDeltaIntegers * 120;

		InputEvent * event;
		if ((event = new PointerWheelEvent(shift, control, mouseWheelDeltaIntegers)) == NULL)
			throw EOUTOFMEMORY;
		desktop->addEvent(event);
	}
}

void DirectXMouseHandler::updateExclusive()
{
	ASSERTION_COBJECT(this);

	shutDown();
	initialize();
}
