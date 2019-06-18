/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


PointerButtonEvent::PointerButtonEvent(bool option1, bool option2, bool pressed, bool primary)
	: InputEvent(option1, option2)
{
	ASSERTION_COBJECT(this);

	this->pressed = pressed;
	this->primary = primary;
}

void PointerButtonEvent::cancelFocusLossPopups(EventTarget * hitTarget)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	const ArrayList<Popup> & popups = desktop->getPopups();
	for (int i = popups.GetCount() - 1; i >= 0; i--)
	{
		Popup * popup = popups.Get(i);

		EventTarget * hitTargetAncestor = hitTarget;
		while (hitTargetAncestor != NULL)
		{
			if (popup == hitTargetAncestor)
				return;

			hitTargetAncestor = hitTargetAncestor->getParent();
		}

		popup->cancel();
	}
}

void PointerButtonEvent::collectAncestors(ArrayList<EventTarget> & ancestors, EventTarget * eventTarget)
{
	ASSERTION_COBJECT(this);

	while (eventTarget != NULL)
	{
		ancestors.Append(eventTarget);
		eventTarget = eventTarget->getParent();
	}
}

void PointerButtonEvent::execute()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	Vector pointerPosition = desktop->getPointerPosition();

	if (primary)
	{
		if (pressed)
		{
			if (desktop->secondaryPressTarget != NULL)
			{
				// The primary button cancels the secondary one
				desktop->secondaryPressTarget = NULL;
				return;
			}

			desktop->primaryPressTarget = desktop->getEventTargetAt(pointerPosition);
			desktop->draggingTested = false;
			cancelFocusLossPopups(desktop->primaryPressTarget);

			if (desktop->primaryPressTarget == NULL)
			{
				desktop->primaryPressTime = 0;
				desktop->primaryPressLastOnHoldTime = 0;
				return;
			}

			unsigned long frameTime = desktop->getFrameTime();
			ArrayList<EventTarget> targets;
			collectAncestors(targets, desktop->primaryPressTarget);
			while (!targets.IsEmpty())
			{
				EventTarget * target = targets.UnlinkLast();
				target->onPress(pointerPosition - target->getOrigin(), option1, option2);
			}

			if (desktop->primaryPressTime > 0
				&& (int) (frameTime - desktop->primaryPressTime) < desktop->doubleClickDelay
				&& (desktop->primaryPressPosition - pointerPosition).absolute()
				<= Vector(desktop->dragStartDistance, desktop->dragStartDistance))
			{
				ArrayList<EventTarget> targets;
				collectAncestors(targets, desktop->primaryPressTarget);
				while (!targets.IsEmpty())
				{
					EventTarget * target = targets.UnlinkLast();
					target->onDoubleClick(pointerPosition - target->getOrigin(), option1, option2);
				}
			}

			desktop->primaryPressTime = frameTime;
			desktop->primaryPressLastOnHoldTime = frameTime;
			desktop->primaryPressPosition = pointerPosition;
		}
		else
		{
			if (desktop->primaryPressTarget == NULL)
				return;

			EventTarget * primaryPressTarget = desktop->primaryPressTarget;
			desktop->primaryPressTarget = NULL;

			ArrayList<EventTarget> targets;
			collectAncestors(targets, primaryPressTarget);
			while (!targets.IsEmpty())
			{
				EventTarget * target = targets.UnlinkLast();
				target->onRelease(option1, option2);
			}

			EventTarget * releaseTarget = desktop->getEventTargetAt(pointerPosition);
			if (releaseTarget == primaryPressTarget && desktop->primaryPressTime > 0
				&& desktop->draggingSurrogate == NULL)
			{
				ArrayList<EventTarget> targets;
				collectAncestors(targets, releaseTarget);
				while (!targets.IsEmpty())
				{
					EventTarget * target = targets.UnlinkLast();
					target->onClick(pointerPosition - target->getOrigin(), option1, option2);
				}
			}

			if (desktop->draggingSurrogate != NULL)
			{
				if (releaseTarget == NULL)
					releaseTarget = desktop;

				Vector relativePosition = pointerPosition - releaseTarget->getOrigin();
				if (desktop->draggingSurrogate->supportsDropping(primaryPressTarget,
					releaseTarget, relativePosition))
				{
					desktop->draggingSurrogate->onDropped(primaryPressTarget,
						releaseTarget, relativePosition, option1, option2);

					const ArrayList<Listenable::Listener> & listeners = releaseTarget->getListeners();
					int listenersCount = listeners.GetCount();
					for (int i=0; i<listenersCount; i++)
					{
						EventTarget::Listener * eventTargetListener
							= dynamic_cast<EventTarget::Listener *>(listeners.Get(i));
						if (eventTargetListener != NULL)
							eventTargetListener->onDropped(primaryPressTarget,
								releaseTarget, relativePosition, option1, option2);
					}
				}

				delete desktop->draggingSurrogate;
				desktop->draggingSurrogate = NULL;
			}

			desktop->updatePointerType();
		}
	}
	else
	{
		if (pressed)
		{
			if (desktop->primaryPressTarget != NULL)
			{
				EventTarget * primaryPressTarget = desktop->primaryPressTarget;
				desktop->primaryPressTarget = NULL;

				// The secondary button cancels the primary one
				ArrayList<EventTarget> targets;
				collectAncestors(targets, primaryPressTarget);
				while (!targets.IsEmpty())
				{
					EventTarget * target = targets.UnlinkLast();
					target->onRelease(option1, option2);
				}

				if (desktop->draggingSurrogate != NULL)
				{
					delete desktop->draggingSurrogate;
					desktop->draggingSurrogate = NULL;
				}

				return;
			}

			desktop->secondaryPressTarget = desktop->getEventTargetAt(pointerPosition);
			cancelFocusLossPopups(desktop->secondaryPressTarget);
		}
		else
		{
			if (desktop->secondaryPressTarget == NULL)
				return;

			EventTarget * secondaryPressTarget = desktop->secondaryPressTarget;
			desktop->secondaryPressTarget = NULL;

			EventTarget * releaseTarget = desktop->getEventTargetAt(pointerPosition);
			if (releaseTarget != NULL && releaseTarget == secondaryPressTarget)
				releaseTarget->onContextClick(pointerPosition - releaseTarget->getOrigin(), option1, option2);
		}
	}
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(PointerButtonEvent, InputEvent);
#endif
