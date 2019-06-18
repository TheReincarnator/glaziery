/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


AttachmentCheckBoxListener::AttachmentCheckBoxListener(Field * field, CheckBox * checkBox)
{
	ASSERTION_COBJECT(this);

	this->field = field;
	this->checkBox = checkBox;
}

CheckBox * AttachmentCheckBoxListener::getCheckBox()
{
	ASSERTION_COBJECT(this);
	return checkBox;
}

void AttachmentCheckBoxListener::onDestroying(Component * component)
{
	ASSERTION_COBJECT(this);
	field->detachFrom(checkBox);
}

void AttachmentCheckBoxListener::onStateChanged(CheckBox * checkBox)
{
	ASSERTION_COBJECT(this);
	field->setActiveFromAttachments();
}


AttachmentRadioBoxListener::AttachmentRadioBoxListener(Field * field, RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);

	this->field = field;
	this->radioBox = radioBox;
}

RadioBox * AttachmentRadioBoxListener::getRadioBox()
{
	ASSERTION_COBJECT(this);
	return radioBox;
}

void AttachmentRadioBoxListener::onDestroying(Component * component)
{
	ASSERTION_COBJECT(this);
	field->detachFrom(radioBox);
}

void AttachmentRadioBoxListener::onStateChanged(RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);
	field->setActiveFromAttachments();
}
