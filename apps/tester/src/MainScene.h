/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __MAINSCENE_H
#define __MAINSCENE_H


class MainScene : public Scene
{
	private:

		static const int NEW_EVENT;
		static const int QUIT_EVENT;

		Image * danielImage;
		Image * testImage;
		Font * font;

	public:

		static const char * ID;

		MainScene();
		virtual ~MainScene();
		void createNewWindow();
		virtual const char * getId();
		virtual void onInitialize(bool firstRun);
		virtual void onShutDown();
		virtual void renderBackground(DrawingContext & context);
		virtual void update();
};


#endif
