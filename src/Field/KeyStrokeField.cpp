/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


KeyStrokeField::KeyStrokeField()
{
	ASSERTION_COBJECT(this);

	deleteClearing = true;
	keyCode = -1;

	setMinimumSize(Vector(64, getMinimumSize().y));

	Desktop::getInstance()->getSkin()->setupKeyStrokeField(this);
}

void KeyStrokeField::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawKeyStrokeField(context, this);
}

int KeyStrokeField::getKeyCode()
{
	ASSERTION_COBJECT(this);
	return keyCode;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(KeyStrokeField, InputField);
#endif

bool KeyStrokeField::isDeleteClearing()
{
	ASSERTION_COBJECT(this);
	return deleteClearing;
}

bool KeyStrokeField::onKeyStroke(int keyCode, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onKeyStroke(keyCode, option1, option2);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	if (deleteClearing && adapter->isDeleteKey(keyCode))
		unsetKeyCode();
	else
		setKeyCode(keyCode);

	return true;
}

void KeyStrokeField::setDeleteClearing(bool deleteClearing)
{
	ASSERTION_COBJECT(this);
	this->deleteClearing = deleteClearing;
}

void KeyStrokeField::setKeyCode(int keyCode)
{
	ASSERTION_COBJECT(this);

	if (this->keyCode == keyCode)
		return;

	this->keyCode = keyCode;

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		KeyStrokeField::Listener * keyStrokeFieldListener
			= dynamic_cast<KeyStrokeField::Listener *>(listeners.Get(i));
		if (keyStrokeFieldListener != NULL)
			keyStrokeFieldListener->onKeyChanged(this);
	}

	invalidate();
}

String KeyStrokeField::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("KeyStrokeField(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}

void KeyStrokeField::unsetKeyCode()
{
	ASSERTION_COBJECT(this);
	setKeyCode(-1);
}

bool KeyStrokeField::validateRequired()
{
	ASSERTION_COBJECT(this);

	if (keyCode < 0)
	{
		showBalloonPopup(STRING_GLAZIERY_VALIDATE_KEYSTROKEFIELD_REQUIRED);
		return false;
	}

	return true;
}


KeyStrokeField::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void KeyStrokeField::Listener::onKeyChanged(KeyStrokeField * field)
{
	ASSERTION_COBJECT(this);
}
