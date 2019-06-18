/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __EXAMPLESCENE_H
#define __EXAMPLESCENE_H


class ExampleScene : public Scene
{
	private:

		// TODO: Define events for this scene.
		static const int QUIT_EVENT;

	public:

		static const char * ID;

		ExampleScene();
		virtual ~ExampleScene();
		virtual const char * getId();
		virtual void onInitialize(bool firstRun);
		virtual void onShutDown();
		virtual void renderBackground(DrawingContext & context);
		virtual void update();
};


#endif
