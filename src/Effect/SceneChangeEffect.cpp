/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


SceneChangeEffect::SceneChangeEffect(Scene * scene, bool changeOnCancel)
{
	ASSERTION_COBJECT(this);

	sceneId = scene->getId();
	this->changeOnCancel = changeOnCancel;
}

SceneChangeEffect::SceneChangeEffect(const char * sceneId, bool changeOnCancel)
{
	ASSERTION_COBJECT(this);

	this->sceneId = sceneId;
	this->changeOnCancel = changeOnCancel;
}

bool SceneChangeEffect::execute(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->setActiveScene(sceneId);
	return false;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(SceneChangeEffect, Effect);
#endif

void SceneChangeEffect::onCancel(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Effect::onCancel(context);

	if (changeOnCancel)
		Desktop::getInstance()->setActiveScene(sceneId);
}
