/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_RADIOGROUP_H
#define __GLAZIERY_RADIOGROUP_H


#define GLAZIERY_RADIOGROUP(variable, area) \
	RadioGroup * variable; \
	if ((variable = area->createRadioGroup()) == NULL) \
		throw EOUTOFMEMORY;


namespace glaziery
{
	class RadioGroup : public GlazieryObject
	{
		friend ArrayList<RadioGroup>;
		friend FieldsArea;

		private:

			ArrayList<RadioBox> radioBoxes;

			RadioGroup();

			~RadioGroup();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(RadioGroup);
			#endif

		public:

			void addRadioBox(RadioBox * radioBox);

			RadioBox * getChecked();

			const ArrayList<RadioBox> & getRadioBoxes();

			void removeRadioBox(RadioBox * radioBox);

			/**
			 * Validates whether any radio box of this radio group has been checked.
			 * If not, shows a balloon popup with an error message.
			 * @return Whether the field is valid.
			 */
			bool validateRequired();
	};
}


#endif
