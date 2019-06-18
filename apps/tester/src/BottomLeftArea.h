/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __BOTTOMLEFTAREA_H
#define __BOTTOMLEFTAREA_H


class BottomLeftArea : public FieldsArea
{
	private:

		class MaskedListener : public RadioBox::Listener
		{
			private:

				TextField * textField;

			public:

				MaskedListener(TextField * textField);
				virtual void onStateChanged(RadioBox * radioBox);
		};

		ProgressBar * progressBar1;

	public:

		ProgressBar * getProgressBar1();
		void initialize();
};


#endif
