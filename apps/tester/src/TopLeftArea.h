/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __TOPLEFTAREA_H
#define __TOPLEFTAREA_H


class TopLeftArea : public FieldsArea
{
	private:

		class ButtonImageListener : public Button::Listener
		{
			friend MainWindow;

			private:

				Image * danielImage;
				Image * waterImage;

			public:

				ButtonImageListener();
				virtual void onPressed(Button * button);
		};

		class ButtonNewWindowListener : public Button::Listener
		{
			public:

				virtual void onPressed(Button * button);
		};

	private:

		Button * button1;
		Button * button4;
		Button * button5;
		Button * button6;
		Button * button7;
		Button * button8;
		CheckBox * checkBox1;

	public:

		Button * getButton1();
		Button * getButton4();
		Button * getButton5();
		Button * getButton6();
		Button * getButton7();
		Button * getButton8();
		CheckBox * getCheckBox1();
		void initialize();
};


#endif
