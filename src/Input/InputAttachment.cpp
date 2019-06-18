/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


InputAttachment::InputAttachment()
{
	eventNumber = 0;
	factor = 0;
	type = InputManager::ATTACHMENT_NONE;
}

void InputAttachment::Configure(ConfigSection * section, bool write)
{
	section->Configure("Type", type, write, InputManager::ATTACHMENT_NONE);
	section->Configure("EventNumber", eventNumber, write, 0);
	section->Configure("Factor", factor, write, 0);
}

void InputAttachment::configureIn(ConfigSection * section, const char * sectionName, bool write)
{
	if (write)
	{
		if (type != InputManager::ATTACHMENT_NONE)
			Configure(section->GetSection(sectionName), true);
		else
			section->DeleteSection(sectionName);
	}
	else
	{
		if (section->HasSection(sectionName))
			Configure(section->GetSection(sectionName), false);
		else
			type = InputManager::ATTACHMENT_NONE;
	}
}
