/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Button::Button()
{
	ASSERTION_COBJECT(this);

	autoResizing = true;
	dialogButton = false;
	image = NULL;
	text = "";

	Desktop::getInstance()->getSkin()->setupButton(this);
	resizeInternal(Desktop::getInstance()->getSkin()->getButtonSize(this), true);
}

Button::~Button()
{
	ASSERTION_COBJECT(this);

	FieldsArea * fieldsArea = getFieldsArea();
	if (fieldsArea != NULL && fieldsArea->getDefaultButton() == this)
		fieldsArea->setDefaultButton(NULL);

	removeHotKey(ClientArea::findHotKey(text));
}

Button::PressedFunctionListener * Button::addPressedFunction(void (*function)(Button * button))
{
	ASSERTION_COBJECT(this);

	PressedFunctionListener * listener;
	if ((listener = new PressedFunctionListener(function)) == NULL)
		throw EOUTOFMEMORY;

	appendListener(listener);
	return listener;
}

void Button::autoResize()
{
	ASSERTION_COBJECT(this);
	resizeInternal(Desktop::getInstance()->getSkin()->getButtonSize(this), true);
}

bool Button::consumingEnter()
{
	ASSERTION_COBJECT(this);
	return true;
}

void Button::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawButton(context, this);
}

Image * Button::getImage()
{
	ASSERTION_COBJECT(this);
	return image;
}

String * Button::getTextAttribute()
{
	ASSERTION_COBJECT(this);
	return &text;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Button, InputField);
#endif

bool Button::isAutoResizing()
{
	ASSERTION_COBJECT(this);
	return autoResizing;
}

bool Button::isDialogButton()
{
	ASSERTION_COBJECT(this);
	return dialogButton;
}

bool Button::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = InputField::onCharacter(character, option1, option2);

	if (character == ' ')
	{
		press();
		return true;
	}

	return consumed;
}

void Button::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (isActiveIncludingAncestors())
		press();
}

bool Button::onEnter(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onEnter(option1, option2);

	press();
	return true;
}

void Button::onMatchedHotKey(bool direct)
{
	ASSERTION_COBJECT(this);

	if (direct)
		press();
}

void Button::press()
{
	ASSERTION_COBJECT(this);

	if (isActiveIncludingAncestors())
	{
		if (dialogButton)
		{
			Dialog * dialog = dynamic_cast<Dialog *>(getWindow());
			if (dialog != NULL)
				dialog->endDialog(this);
		}

		int listenersCount = listeners.GetCount();
		for (int i=0; i<listenersCount; i++)
		{
			Button::Listener * buttonListener
				= dynamic_cast<Button::Listener *>(listeners.Get(i));
			if (buttonListener != NULL)
				buttonListener->onPressed(this);
		}
	}
}

void Button::setAutoResizing(bool autoResizing)
{
	ASSERTION_COBJECT(this);

	this->autoResizing = autoResizing;
	if (autoResizing)
		autoResize();
}

void Button::setCaps(bool caps)
{
	ASSERTION_COBJECT(this);

	if (isCaps() == caps)
		return;

	Field::setCaps(caps);
	if (autoResizing)
		autoResize();
}

void Button::setDialogButton(bool dialogButton)
{
	ASSERTION_COBJECT(this);
	this->dialogButton = dialogButton;
}

void Button::setFont(Font * font)
{
	ASSERTION_COBJECT(this);

	if (getFont() == font)
		return;

	Field::setFont(font);
	if (autoResizing)
		autoResize();
}

void Button::setImage(Image * image)
{
	ASSERTION_COBJECT(this);

	if (this->image == image)
		return;

	this->image = image;
	if (autoResizing)
		autoResize();

	invalidate();
}

void Button::setImageId(const String & imageId)
{
	ASSERTION_COBJECT(this);

	if (imageId.IsEmpty())
		setImage(NULL);
	else
		setImage(Desktop::getInstance()->getPlatformAdapter()->getImage(imageId, true));
}

void Button::setImageId(const char * imageId)
{
	ASSERTION_COBJECT(this);

	if (imageId == NULL || imageId[0] == 0)
		setImage(NULL);
	else
		setImage(Desktop::getInstance()->getPlatformAdapter()->getImage(String(imageId), true));
}

void Button::setText(const String & text)
{
	ASSERTION_COBJECT(this);

	if (this->text == text)
		return;

	Field::setText(text);

	if (autoResizing)
		autoResize();
}

String Button::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Button(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}


Button::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void Button::Listener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);
}


Button::PressedFunctionListener::PressedFunctionListener(void (*function)(Button * button))
{
	ASSERTION_COBJECT(this);
	this->function = function;
}

void Button::PressedFunctionListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);
	(*function)(button);
}
