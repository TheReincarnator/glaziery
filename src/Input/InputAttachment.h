/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_INPUTATTACHMENT_H
#define __GLAZIERY_INPUTATTACHMENT_H


namespace glaziery
{
	/**
	 * The properties of an input attachment. An attachment defines the event
	 * that is risen if the object the attached to is triggered.
	 */
	class InputAttachment : public Configurable
	{
		public:

			/**
			 * The number of the attached event.
			 */
			int eventNumber;

			/**
			 * The factor to multiply an event occurrence.
			 */
			int factor;

			/**
			 * The type of the attachment, see the
			 * types constants for details.
			 */
			int type;

			/**
			 * Creates a new attachment.
			 */
			InputAttachment();

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * section, bool write);

			/**
			 * Configures the attachment in a subsection of a given ConfigSection,
			 * maybe deleting the subsection.
			 * @param section The parent ConfigSection.
			 * @param sectionName The subsection name.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			void configureIn(ConfigSection * section, const char * sectionName, bool write);
	};
}


#endif
