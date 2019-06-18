/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __TOPMIDDLEAREA_H
#define __TOPMIDDLEAREA_H


class TopMiddleArea : public FieldsArea
{
	private:

		CheckBox * checkBox1;
		Slider * slider1;
		Slider * slider2;
		Slider * slider3;

		static void onDirectoryPressed(Button * button);
		static void onLoadFilePressed(Button * button);
		static void onSaveFilePressed(Button * button);

	public:

		CheckBox * getCheckBox1();
		Slider * getSlider1();
		Slider * getSlider2();
		void initialize(Window * window);
};


#endif
