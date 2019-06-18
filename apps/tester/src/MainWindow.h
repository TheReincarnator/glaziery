/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H


class MainWindow : public Window
{
	private:

		class AppendButtonListener : public Button::Listener
		{
			private:

				BottomRightListArea * listArea;

			public:

				AppendButtonListener(BottomRightListArea * listArea);
				virtual void onPressed(Button * button);
		};

		class ButtonCheckBoxListener : public Button::Listener
		{
			private:

				CheckBox * checkBox;

			public:

				ButtonCheckBoxListener(CheckBox * checkBox);
				virtual void onPressed(Button * button);
		};

		class CheckBoxListener : public CheckBox::Listener
		{
			private:

				ListField * listField;
				TreeField * treeField;
				Window * window;

			public:

				CheckBoxListener(TreeField * treeField, ListField * listField, Window * window);
				virtual void onStateChanged(CheckBox * checkBox);
		};

		class DeleteSelectedButtonListener : public Button::Listener
		{
			private:

				BottomRightListArea * listArea;

			public:

				DeleteSelectedButtonListener(BottomRightListArea * listArea);
				virtual void onPressed(Button * button);
		};

		class DeleteUnselectedButtonListener : public Button::Listener
		{
			private:

				BottomRightListArea * listArea;

			public:

				DeleteUnselectedButtonListener(BottomRightListArea * listArea);
				virtual void onPressed(Button * button);
		};

		class InsertButtonListener : public Button::Listener
		{
			private:

				BottomRightListArea * listArea;

			public:

				InsertButtonListener(BottomRightListArea * listArea);
				virtual void onPressed(Button * button);
		};

		class PrependButtonListener : public Button::Listener
		{
			private:

				BottomRightListArea * listArea;

			public:

				PrependButtonListener(BottomRightListArea * listArea);
				virtual void onPressed(Button * button);
		};

		class SliderAlphaListener : public Slider::Listener
		{
			private:

				Window * window;

			public:

				SliderAlphaListener(Window * window);
				virtual void onCurrentChanged(Slider * field);
		};

		class SliderProgressListener : public Slider::Listener
		{
			private:

				ProgressBar * progressBar;

			public:

				SliderProgressListener(ProgressBar * progressBar);
				virtual void onCurrentChanged(Slider * field);
		};

		class WindowListener : public Window::Listener
		{
			public:

				virtual VetoMode onClosing(Window * window);
		};

	private:

		BottomRightCustomArea * bottomRightCustomArea;

	public:

		MainWindow();
		void initialize();
};


#endif
