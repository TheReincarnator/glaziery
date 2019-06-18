/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


RadioGroup::RadioGroup()
{
	ASSERTION_COBJECT(this);
}

RadioGroup::~RadioGroup()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<radioBoxes.GetCount(); i++)
		radioBoxes.Get(i)->setRadioGroup(NULL);

	// Do not delete the radio boxes, they belong to the fields area
	radioBoxes.UnlinkAll();
}

void RadioGroup::addRadioBox(RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);

	radioBoxes.Append(radioBox);
	radioBox->setRadioGroup(this);
}

RadioBox * RadioGroup::getChecked()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<radioBoxes.GetCount(); i++)
	{
		RadioBox * radioBox = radioBoxes.Get(i);
		if (radioBoxes.Get(i)->isChecked())
			return radioBox;
	}

	return NULL;
}

const ArrayList<RadioBox> & RadioGroup::getRadioBoxes()
{
	ASSERTION_COBJECT(this);
	return radioBoxes;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(RadioGroup, GlazieryObject);
#endif

void RadioGroup::removeRadioBox(RadioBox * radioBox)
{
	ASSERTION_COBJECT(this);

	radioBox->setRadioGroup(NULL);
	radioBoxes.Unlink(radioBox);
}

bool RadioGroup::validateRequired()
{
	ASSERTION_COBJECT(this);

	if (!radioBoxes.IsEmpty() && getChecked() == NULL)
	{
		radioBoxes.GetFirst()->showBalloonPopup(STRING_GLAZIERY_VALIDATE_RADIOGROUP_REQUIRED);
		return false;
	}

	return true;
}
