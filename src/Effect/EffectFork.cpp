/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


EffectFork::EffectFork()
{
	ASSERTION_COBJECT(this);
}

EffectFork::~EffectFork()
{
	ASSERTION_COBJECT(this);

	while (!effects.IsEmpty())
		Desktop::getInstance()->addDisposable(effects.UnlinkFirst());
}

void EffectFork::addEffect(Effect * effect)
{
	ASSERTION_COBJECT(this);

	effects.Append(effect);
	effect->addReference();
}

bool EffectFork::execute(DrawingContext & context, long time)
{
	ASSERTION_COBJECT(this);

	bool anyEffectRunning = false;

	for (int i=0; i<effects.GetCount(); i++)
	{
		Effect * effect = effects.Get(i);

		if (!effect->isEnded() && !effect->isExecuting())
		{
			effect->setExecuting(true);

			if (effect->isCanceled())
			{
				effect->onCancel(context);
				effect->setEnded();
			}
			else if (!effect->execute(context, time))
				effect->setEnded();
			else
				anyEffectRunning = true;

			effect->setExecuting(false);
		}
	}

	return anyEffectRunning;
}

const ArrayList<Effect> & EffectFork::getEffects()
{
	ASSERTION_COBJECT(this);
	return effects;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(EffectFork, Effect);
#endif

void EffectFork::onAdded()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<effects.GetCount(); i++)
		effects.Get(i)->onAdded();
}

void EffectFork::onCancel(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<effects.GetCount(); i++)
		effects.Get(i)->onCancel(context);
}
