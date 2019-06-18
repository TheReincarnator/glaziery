/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Slider::Slider()
{
	ASSERTION_COBJECT(this);

	current = 0;
	end = 100;
	majorStepUnits = 20;
	minorStepUnits = 2;
	start = 0;
	stringListAttachment = NULL;
	stringListAttachmentTarget = NULL;

	setMinimumSize(Vector(64, 16));
	resizeInternal(Vector(128, 16), true);

	Desktop::getInstance()->getSkin()->setupSlider(this);
}

Slider::~Slider()
{
	ASSERTION_COBJECT(this);
	detachStringList();
}

void Slider::attachStringList(ArrayList<String> * stringList, Field * target)
{
	ASSERTION_COBJECT(this);

	detachStringList();

	if (stringList == NULL || target == NULL)
		return;

	stringListAttachment = stringList;
	stringListAttachmentTarget = target;

	start = 0;
	end = stringListAttachment->GetCount() - 1;
	minorStepUnits = 1;
	updateAttachmentTarget();
}

void Slider::detachStringList()
{
	ASSERTION_COBJECT(this);

	if (stringListAttachment != NULL) {delete stringListAttachment; stringListAttachment = NULL;}
	stringListAttachmentTarget = NULL;
}

void Slider::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawSlider(context, this);
}

long Slider::getConstrainedCurrent()
{
	ASSERTION_COBJECT(this);

	if (current < start)
		return start;
	else if (current > end)
		return end;
	else
		return current;
}

long Slider::getCurrent()
{
	ASSERTION_COBJECT(this);
	return current;
}

long Slider::getEnd()
{
	ASSERTION_COBJECT(this);
	return end;
}

long Slider::getMajorStepUnits()
{
	ASSERTION_COBJECT(this);
	return majorStepUnits;
}

long Slider::getMinorStepUnits()
{
	ASSERTION_COBJECT(this);
	return minorStepUnits;
}

long Slider::getStart()
{
	ASSERTION_COBJECT(this);
	return start;
}

ArrayList<String> * Slider::getStringListAttachment()
{
	ASSERTION_COBJECT(this);
	return stringListAttachment;
}

Field * Slider::getStringListAttachmentTarget()
{
	ASSERTION_COBJECT(this);
	return stringListAttachmentTarget;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Slider, InputField);
#endif

bool Slider::onMoveLeft(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveLeft(option1, option2);

	long newCurrent;
	if (option1)
		newCurrent = current - majorStepUnits;
	else
		newCurrent = current - minorStepUnits;

	if (newCurrent < start)
		newCurrent = start;

	setCurrent(newCurrent);

	return true;
}

bool Slider::onMoveRight(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveRight(option1, option2);

	long newCurrent;
	if (option1)
		newCurrent = current + majorStepUnits;
	else
		newCurrent = current + minorStepUnits;

	if (newCurrent > end)
		newCurrent = end;

	setCurrent(newCurrent);

	return true;
}

bool Slider::onMoveToEnd(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveToEnd(option1, option2);

	if (current != end)
		setCurrent(end);

	return true;
}

bool Slider::onMoveToStart(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveToStart(option1, option2);

	setCurrent(start);
	return true;
}

bool Slider::onPageDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onPageDown(option1, option2);

	long newCurrent = current + majorStepUnits;
	if (newCurrent > end)
		newCurrent = end;

	setCurrent(newCurrent);

	return true;
}

bool Slider::onPageUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onPageUp(option1, option2);

	long newCurrent = current - majorStepUnits;
	if (newCurrent < start)
		newCurrent = start;

	setCurrent(newCurrent);

	return true;
}

void Slider::setCurrent(long current)
{
	ASSERTION_COBJECT(this);

	if (this->current == current)
		return;

	long listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Slider::Listener * sliderListener
			= dynamic_cast<Slider::Listener *>(listeners.Get(i));
		if (sliderListener != NULL)
			if (sliderListener->onCurrentChanging(this, current) == VETO)
				return;
	}

	long oldCurrent = this->current;
	this->current = current;

	for (int i=0; i<listenersCount; i++)
	{
		Slider::Listener * sliderListener
			= dynamic_cast<Slider::Listener *>(listeners.Get(i));
		if (sliderListener != NULL)
			sliderListener->onCurrentChanged(this, oldCurrent);
	}

	updateAttachmentTarget();
	invalidate();
}

void Slider::setEnd(long end)
{
	ASSERTION_COBJECT(this);

	if (this->end == end)
		return;

	this->end = end;
	invalidate();
}

void Slider::setMajorStepUnits(long majorStepUnits)
{
	ASSERTION_COBJECT(this);
	this->majorStepUnits = majorStepUnits;
}

void Slider::setMinorStepUnits(long minorStepUnits)
{
	ASSERTION_COBJECT(this);
	this->minorStepUnits = minorStepUnits;
}

void Slider::setStart(long start)
{
	ASSERTION_COBJECT(this);

	if (this->start == start)
		return;

	this->start = start;
	invalidate();
}

String Slider::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Slider(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}

void Slider::updateAttachmentTarget()
{
	ASSERTION_COBJECT(this);

	if (stringListAttachmentTarget == NULL)
		return;

	String * string = stringListAttachment->Get(current);
	if (string != NULL)
		stringListAttachmentTarget->setText(*string);
}


Slider::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void Slider::Listener::onCurrentChanged(Slider * field, long oldCurrent)
{
	ASSERTION_COBJECT(this);
}

VetoMode Slider::Listener::onCurrentChanging(Slider * field, long newCurrent)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}
