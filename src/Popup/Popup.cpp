/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Popup::Popup()
{
	ASSERTION_COBJECT(this);

	destroying = false;

	setCaching(true);
	setNonFocusAlpha(240);
	setFocusAlpha(240);
}

void Popup::cancel()
{
	ASSERTION_COBJECT(this);

	if (!destroying && !isDisposed())
	{
		destroying = true;
		Desktop::getInstance()->getSkin()->addPopupDestroyEffect(this);
	}
}

void Popup::destroy()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	desktop->popups.Unlink(this);
	desktop->notifyPossibleFocusChange(NULL);

	Component::destroy();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Popup, Component);
#endif

bool Popup::isDestroying()
{
	ASSERTION_COBJECT(this);
	return destroying;
}

bool Popup::onCancel()
{
	ASSERTION_COBJECT(this);

	Component::onCancel();

	cancel();
	return true;
}

void Popup::onLostFocus()
{
	ASSERTION_COBJECT(this);

	Component::onLostFocus();

	// Destroy all popups on top of the newly focused popup,
	// or destroy all popups, if no popup has the focus now.

	Desktop * desktop = Desktop::getInstance();
	Component * focusComponent = desktop->getFocusComponent();
	const ArrayList<Popup> & popups = desktop->getPopups();
	for (int i = popups.GetCount() - 1; i >= 0; i--)
	{
		Popup * popup = popups.Get(i);
		if (popup == focusComponent)
			break;

		popup->cancel();
	}
}

String Popup::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Popup(position:%s,size:%s)",
		(const char *) getPosition().toString(), (const char *) getSize().toString());
	return string;
}
