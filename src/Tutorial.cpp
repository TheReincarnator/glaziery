/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Tutorial::Tutorial()
{
	ASSERTION_COBJECT(this);
	canceled = false;
}

Tutorial::~Tutorial()
{
	ASSERTION_COBJECT(this);
}

void Tutorial::cancel()
{
	ASSERTION_COBJECT(this);
	canceled = true;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Tutorial, GlazieryObject);
#endif

bool Tutorial::isCanceled()
{
	ASSERTION_COBJECT(this);
	return canceled;
}

void Tutorial::tutorialBackSpace(bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_BACKSPACE);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(false, false);
	focusTarget->onBackSpace();
}

void Tutorial::tutorialBackTab(bool secondary, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_BACKTAB);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(true, secondary);
	focusTarget->onBackTab(secondary);
}

void Tutorial::tutorialCancel(bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_CANCEL);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(false, false);
	focusTarget->onCancel();
}

void Tutorial::tutorialCharacter(char character, bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(String(character));

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onCharacter(character, option1, option2);
}

void Tutorial::tutorialDelete(bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_DELETE);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(false, false);
	focusTarget->onDelete();
}

void Tutorial::tutorialEdit(bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_EDIT);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(false, false);
	focusTarget->onEdit();
}

void Tutorial::tutorialEnter(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_ENTER);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onEnter(option1, option2);
}

void Tutorial::tutorialHotKey(char character, bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();

	if (visualize)
	{
		String keyName("Alt-");
		keyName += character;
		desktop->getSkin()->addKeyVisualizationEffect(keyName);
	}

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onHotKey(character, option1, option2);
}

void Tutorial::tutorialKeyStroke(int keyCode, bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
	{
		const char * keyName = desktop->getPlatformAdapter()->getKeyName(keyCode);
		desktop->getSkin()->addKeyVisualizationEffect(String(keyName));
	}

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onKeyStroke(keyCode, option1, option2);
}

void Tutorial::tutorialMoveDown(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_MOVEDOWN);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onMoveDown(option1, option2);
}

void Tutorial::tutorialMoveLeft(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_MOVELEFT);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onMoveLeft(option1, option2);
}

void Tutorial::tutorialMoveRight(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_MOVERIGHT);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onMoveRight(option1, option2);
}

void Tutorial::tutorialMoveToEnd(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_MOVETOEND);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onMoveToEnd(option1, option2);
}

void Tutorial::tutorialMoveToStart(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_MOVETOSTART);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onMoveToStart(option1, option2);
}

void Tutorial::tutorialMoveUp(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_MOVEUP);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onMoveUp(option1, option2);
}

void Tutorial::tutorialPageDown(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_PAGEDOWN);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onPageDown(option1, option2);
}

void Tutorial::tutorialPageUp(bool option1, bool option2, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_PAGEUP);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(option1, option2);
	focusTarget->onPageUp(option1, option2);
}

void Tutorial::tutorialSelectAll(bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_SELECTALL);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(false, false);
	focusTarget->onSelectAll();
}

void Tutorial::tutorialTab(bool secondary, bool visualize)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (visualize)
		desktop->getSkin()->addKeyVisualizationEffect(STRING_GLAZIERY_KEYNAME_TAB);

	Component * focusTarget = desktop->getFocusWindowOrPopup();
	if (focusTarget == NULL)
		return;

	focusTarget->onAnyKey(false, secondary);
	focusTarget->onTab(secondary);
}

void Tutorial::tutorialWait(long time)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();
	unsigned long endTime = adapter->getTime() + time;
	while (adapter->getTime() < endTime)
		desktop->runOnce();
}

void Tutorial::tutorialWaitForStep()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	desktop->waitingForTutorialStep = true;
	while (desktop->waitingForTutorialStep)
		desktop->runOnce();
}
