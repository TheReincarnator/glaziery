/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>

Component::Component()
{
	ASSERTION_COBJECT(this);

	disposed = false;
	maximumSize = Vector(4096, 4096);
	minimumSize = Vector(0, 0);
	parent = NULL;
	size = Vector(64, 32);
	visible = true;
	visibleDeferred = visible;
}

Component::~Component()
{
	ASSERTION_COBJECT(this);

	while (!effects.IsEmpty())
	{
		ComponentEffect * effect = effects.UnlinkFirst();
		effect->onComponentDestroying();
		effect->release();
	}
}

void Component::addEffect(ComponentEffect * effect)
{
	ASSERTION_COBJECT(this);

	effects.Append(effect);
	effect->addReference();
}

void Component::addContextMenuItems(Menu * menu, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void Component::appendWidget(Widget * widget)
{
	ASSERTION_COBJECT(this);

	if (widget->component != NULL)
		throw EILLEGALSTATE("The widget is already registered to another component");
	widget->component = this;
	widgets.Append(widget);
}

void Component::cancelEffects()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<effects.GetCount(); i++)
		effects.Get(i)->cancel();
}

void Component::center()
{
	ASSERTION_COBJECT(this);

	Vector parentSize;
	if (parent != NULL)
		parentSize = parent->getSize();
	else
	{
		PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
		parentSize = adapter->getScreenSize();
	}

	moveTo((parentSize - size) / 2);
}

void Component::deleteChild(Component * child)
{
	ASSERTION_COBJECT(this);
	ASSERTION_COBJECT(child);

	if (child->getParent() != this)
		throw EILLEGALARGUMENT("The component is not a child of this component");

	delete child;
}

void Component::destroy()
{
	ASSERTION_COBJECT(this);

	if (disposed)
		return;

	// Add the component to the disposables.
	// It will be destroyed next frame.
	disposed = true;
	Desktop * desktop = Desktop::getInstance();
	desktop->addDisposable(this);

	// Then, notify listeners.
	onDestroying();
}

void Component::executeDeferrals()
{
	ASSERTION_COBJECT(this);

	EventTarget::executeDeferrals();

	if (visibleDeferred && !visible)
		show();
	else if (!visibleDeferred && visible)
		hide();
}

Vector Component::getAbsolutePosition()
{
	ASSERTION_COBJECT(this);

	if (parent == NULL)
		return position;
	else
		return position + parent->getChildrenOrigin();
}

Vector Component::getChildrenOrigin()
{
	ASSERTION_COBJECT(this);
	return getAbsolutePosition();
}

const ArrayList<ComponentEffect> & Component::getEffects()
{
	ASSERTION_COBJECT(this);
	return effects;
}

EventTarget * Component::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	int widgetsCount = widgets.GetCount();
	for (int i=0; i<widgetsCount; i++)
	{
		Widget * widget = widgets.Get(i);
		if (widget->isHitAt(position))
			return widget;
	}

	return this;
}

Component * Component::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return NULL;
}

Vector Component::getMaximumSize()
{
	ASSERTION_COBJECT(this);
	return maximumSize;
}

Vector Component::getMinimumSize()
{
	ASSERTION_COBJECT(this);
	return minimumSize;
}

Vector Component::getOrigin()
{
	ASSERTION_COBJECT(this);
	return getAbsolutePosition();
}

Component * Component::getParent()
{
	ASSERTION_COBJECT(this);
	return parent;
}

Vector Component::getPosition()
{
	ASSERTION_COBJECT(this);
	return position;
}

class Vector Component::getSize()
{
	ASSERTION_COBJECT(this);
	return size;
}

const ArrayList<Widget> & Component::getWidgets()
{
	ASSERTION_COBJECT(this);
	return widgets;
}

bool Component::hasFocus()
{
	ASSERTION_COBJECT(this);

	Component * parent = getParent();
	if (parent != NULL)
		return parent->getFocusChild() == this && parent->hasFocus();
	else
		return Desktop::getInstance()->getFocusWindowOrPopup() == this;
}

void Component::hide()
{
	ASSERTION_COBJECT(this);

	if (!visible)
		return;

	// Invalidation must take place before hiding, because the invalidate checks that flag
	invalidate();

	visible = false;
	visibleDeferred = visible;

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
			componentListener->onHidden(this);
	}
}

void Component::hideDeferred()
{
	ASSERTION_COBJECT(this);
	setVisibleDeferred(false);
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Component, EventTarget);
#endif

void Component::invalidate()
{
	ASSERTION_COBJECT(this);
	invalidateArea(Vector(), size);
}

void Component::invalidateArea(Vector position, Vector size)
{
	ASSERTION_COBJECT(this);

	Component * parent = getParent();
	if (parent != NULL && parent->isChildVisible(this))
		parent->invalidateArea(getPosition() + position, size);
}

bool Component::isChildVisible(Component * child)
{
	ASSERTION_COBJECT(this);
	return child->isVisible();
}

bool Component::isDisposed()
{
	ASSERTION_COBJECT(this);
	return disposed;
}

bool Component::isVisible()
{
	ASSERTION_COBJECT(this);
	return visible;
}

bool Component::isVisibleIncludingAncestors()
{
	ASSERTION_COBJECT(this);

	Component * ancestor = this;
	while (ancestor != NULL)
	{
		if (!ancestor->isVisible())
			return false;

		ancestor = ancestor->getParent();
	}

	return true;
}

void Component::moveComponent(Component * relatedComponent, Vector position)
{
	ASSERTION_COBJECT(this);
	relatedComponent->moveInternal(position, false);
}

bool Component::moveInternal(Vector position, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	if (this->position == position)
		return false;

	Vector oldPosition = this->position;
	this->position = position;
	if (parent != NULL && notifyParent)
		parent->onChildMoved(this, oldPosition);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
			componentListener->onMoved(this, oldPosition);
	}

	invalidate();
	return true;
}

bool Component::moveRelative(Vector delta)
{
	ASSERTION_COBJECT(this);
	return moveTo(position + delta);
}

bool Component::moveTo(Vector position)
{
	ASSERTION_COBJECT(this);
	return moveInternal(position, true);
}

bool Component::onAnyKey(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onAnyKey(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onAnyKey(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onBackSpace()
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onBackSpace();

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onBackSpace())
			consumed = true;

	return consumed;
}

bool Component::onBackTab(bool secondary)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onBackTab(secondary);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onBackTab(secondary))
			consumed = true;

	return consumed;
}

bool Component::onCancel()
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onCancel();

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onCancel())
			consumed = true;

	return consumed;
}

bool Component::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onCharacter(character, option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onCharacter(character, option1, option2))
			consumed = true;

	return consumed;
}

void Component::onChildMaximumSizeChanged(Component * child, Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);
}

void Component::onChildMinimumSizeChanged(Component * child, Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);
}

void Component::onChildMoved(Component * child, Vector oldPosition)
{
	ASSERTION_COBJECT(this);
}

void Component::onChildResized(Component * child, Vector oldSize)
{
	ASSERTION_COBJECT(this);
}

void Component::onContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	EventTarget::onContextClick(position, option1, option2);

	Menu * menu;
	if ((menu = new Menu(this)) == NULL)
		throw EOUTOFMEMORY;

	addContextMenuItems(menu, position, option1, option2);
	if (menu->getItems().IsEmpty())
	{
		delete menu;
		return;
	}

	MenuPopup * popup = new MenuPopup(menu, true);
	Desktop::getInstance()->addPopup(popup);

	Vector popupPosition = getAbsolutePosition() + position;
	if (popupPosition.x + popup->getSize().x > Desktop::getInstance()->getSize().x)
		popupPosition.x -= popup->getSize().x;
	if (popupPosition.y + popup->getSize().y > Desktop::getInstance()->getSize().y)
		popupPosition.y -= popup->getSize().y;

	popup->moveTo(popupPosition);
}

bool Component::onCopy()
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onCopy();

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onCopy())
			consumed = true;

	return consumed;
}

bool Component::onCut()
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onCut();

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onCut())
			consumed = true;

	return consumed;
}

bool Component::onDelete()
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onDelete();

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onDelete())
			consumed = true;

	return consumed;
}

void Component::onDestroying()
{
	ASSERTION_COBJECT(this);

	ArrayList<Component::Listener> listenersToNotify;

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
		{
			listenersToNotify.Append(componentListener);
			componentListener->addReference();
		}
	}

	listenersCount = listenersToNotify.GetCount();
	while (!listenersToNotify.IsEmpty())
	{
		Component::Listener * listener = listenersToNotify.UnlinkFirst();
		listener->onDestroying(this);
		listener->release();
	}
}

bool Component::onEdit()
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onEdit();

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onEdit())
			consumed = true;

	return consumed;
}

bool Component::onEnter(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onEnter(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onEnter(option1, option2))
			consumed = true;

	return consumed;
}

void Component::onGotFocus(bool byParent)
{
	ASSERTION_COBJECT(this);

	// First notify listeners of this component about focus gain
	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
			componentListener->onGotFocus(this, byParent);
	}

	// Then notify child
	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		focusChild->onGotFocus(true);

	// Finally, invalidate the component
	invalidate();
}

bool Component::onHotKey(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onHotKey(character, option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onHotKey(character, option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onKeyStroke(int keyCode, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onKeyStroke(keyCode, option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onKeyStroke(keyCode, option1, option2))
			consumed = true;

	return consumed;
}

void Component::onLostFocus()
{
	ASSERTION_COBJECT(this);

	// First notify child about focus loss
	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		focusChild->onLostFocus();

	// Then notify listeners of this component
	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
			componentListener->onLostFocus(this);
	}

	// Finally, invalidate the component
	invalidate();
}

void Component::onMaximumSizeChanged(Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);
}

void Component::onMinimumSizeChanged(Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);
}

bool Component::onMoveDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onMoveDown(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onMoveDown(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onMoveLeft(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onMoveLeft(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onMoveLeft(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onMoveRight(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onMoveRight(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onMoveRight(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onMoveToEnd(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onMoveToEnd(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onMoveToEnd(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onMoveToStart(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onMoveToStart(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onMoveToStart(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onMoveUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onMoveUp(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onMoveUp(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onPageDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onPageDown(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onPageDown(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onPageUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onPageUp(option1, option2);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onPageUp(option1, option2))
			consumed = true;

	return consumed;
}

bool Component::onPaste()
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onPaste();

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onPaste())
			consumed = true;

	return consumed;
}

bool Component::onSelectAll()
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onSelectAll();

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onSelectAll())
			consumed = true;

	return consumed;
}

bool Component::onTab(bool secondary)
{
	ASSERTION_COBJECT(this);

	bool consumed = EventTarget::onTab(secondary);

	Component * focusChild = getFocusChild();
	if (focusChild != NULL)
		if (focusChild->onTab(secondary))
			consumed = true;

	return consumed;
}

void Component::prependWidget(Widget * widget)
{
	ASSERTION_COBJECT(this);

	if (widget->component != NULL)
		throw EILLEGALARGUMENT("The widget is already registered to another component");
	widget->component = this;
	widgets.Prepend(widget);
}

void Component::removeEffect(ComponentEffect * effect)
{
	ASSERTION_COBJECT(this);

	ComponentEffect * unlinkedEffect = effects.Unlink(effect);
	if (unlinkedEffect != NULL)
		unlinkedEffect->release();
}

void Component::removeWidget(Widget * widget)
{
	ASSERTION_COBJECT(this);
	widgets.Delete(widget);
}

bool Component::resize(Vector size)
{
	ASSERTION_COBJECT(this);
	return resizeInternal(size, true);
}

bool Component::resizeToMaximum()
{
	ASSERTION_COBJECT(this);
	return resizeInternal(maximumSize, true);
}

bool Component::resizeToMinimum()
{
	ASSERTION_COBJECT(this);
	return resizeInternal(minimumSize, true);
}

void Component::resizeComponent(Component * relatedComponent, Vector size)
{
	ASSERTION_COBJECT(this);
	relatedComponent->resizeInternal(size, false);
}

bool Component::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	size = Vector(size.x > maximumSize.x ? maximumSize.x : size.x,
		size.y > maximumSize.y ? maximumSize.y : size.y);
	size = Vector(size.x < minimumSize.x ? minimumSize.x : size.x,
		size.y < minimumSize.y ? minimumSize.y : size.y);

	if (this->size == size)
		return false;

	Vector oldSize = this->size;
	this->size = size;
	if (parent != NULL && notifyParent)
		parent->onChildResized(this, oldSize);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
			componentListener->onResized(this, oldSize);
	}

	invalidate();
	return true;
}

void Component::setComponentParent(Component * child)
{
	ASSERTION_COBJECT(this);

	if (child->getParent() != NULL)
		throw EILLEGALARGUMENT("The component already has a parent, it may not be changed");

	child->parent = this;
}

void Component::setComponentMaximumSize(Component * component, Vector maximumSize, bool notifyParent)
{
	ASSERTION_COBJECT(this);
	component->setMaximumSizeInternal(maximumSize, notifyParent);
}

void Component::setComponentMinimumSize(Component * component, Vector minimumSize, bool notifyParent)
{
	ASSERTION_COBJECT(this);
	component->setMinimumSizeInternal(minimumSize, notifyParent);
}

void Component::setDisposed()
{
	ASSERTION_COBJECT(this);
	disposed = true;
}

void Component::setMaximumSize(Vector maximumSize)
{
	ASSERTION_COBJECT(this);
	setMaximumSizeInternal(maximumSize, true);
}

void Component::setMaximumSizeInternal(Vector maximumSize, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	if (maximumSize.x < 0)
		maximumSize.x = 0;
	if (maximumSize.y < 0)
		maximumSize.y = 0;

	if (this->maximumSize == maximumSize)
		return;

	Vector oldMaximumSize = this->maximumSize;
	this->maximumSize = maximumSize;
	onMaximumSizeChanged(oldMaximumSize);
	if (notifyParent && parent != NULL)
		parent->onChildMaximumSizeChanged(this, oldMaximumSize);

	if (!(minimumSize <= maximumSize))
		setMinimumSize(Vector(minimumSize.x > maximumSize.x ? maximumSize.x : minimumSize.x,
			minimumSize.y > maximumSize.y ? maximumSize.y : minimumSize.y));

	if (!(size <= maximumSize))
		resize(Vector(size.x > maximumSize.x ? maximumSize.x : size.x,
			size.y > maximumSize.y ? maximumSize.y : size.y));
}

void Component::setMinimumSize(Vector minimumSize)
{
	ASSERTION_COBJECT(this);
	setMinimumSizeInternal(minimumSize, true);
}

void Component::setMinimumSizeInternal(Vector minimumSize, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	if (minimumSize.x < 0)
		minimumSize.x = 0;
	if (minimumSize.y < 0)
		minimumSize.y = 0;

	if (this->minimumSize == minimumSize)
		return;

	Vector oldMinimumSize = this->minimumSize;
	this->minimumSize = minimumSize;
	onMinimumSizeChanged(oldMinimumSize);
	if (notifyParent && parent != NULL)
		parent->onChildMinimumSizeChanged(this, oldMinimumSize);

	if (!(maximumSize >= minimumSize))
		setMaximumSize(Vector(maximumSize.x < minimumSize.x ? minimumSize.x : maximumSize.x,
			maximumSize.y < minimumSize.y ? minimumSize.y : maximumSize.y));

	if (!(size >= minimumSize))
		resize(Vector(size.x < minimumSize.x ? minimumSize.x : size.x,
			size.y < minimumSize.y ? minimumSize.y : size.y));
}

void Component::setSkinData(SkinData * skinData)
{
	ASSERTION_COBJECT(this);

	GlazieryObject::setSkinData(skinData);
	skinData->component = this;
}

void Component::setVisible(bool visible)
{
	ASSERTION_COBJECT(this);

	if (visible)
		show();
	else
		hide();
}

void Component::setVisibleDeferred(bool visible)
{
	ASSERTION_COBJECT(this);

	if (visible)
		showDeferred();
	else
		hideDeferred();
}

void Component::show()
{
	ASSERTION_COBJECT(this);

	if (visible)
		return;

	visible = true;
	visibleDeferred = visible;

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
			componentListener->onShown(this);
	}

	invalidate();
}

BalloonPopup * Component::showBalloonPopup(const String & text)
{
	ASSERTION_COBJECT(this);

	BalloonPopup * popup;
	if ((popup = new BalloonPopup) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addPopup(popup);

	popup->setMessage(text);
	popup->pointTo(this);

	return popup;
}

void Component::showDeferred()
{
	ASSERTION_COBJECT(this);

	Mutex * mutex = Desktop::getInstance()->getDeferralMutex();
	if (!mutex->lock())
		return;

	visibleDeferred = visible;
	Desktop::getInstance()->deferObject(this);

	mutex->release();
}

String Component::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Component(position:%s,size:%s)",
		(const char *) position.toString(), (const char *) size.toString());
	return string;
}

void Component::tutorialClick(PointerEffect::ButtonEffect buttonEffect, bool option1, bool option2, long time)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (!Desktop::getInstance()->isTutorialMode())
		throw EILLEGALSTATE("Use the tutorial methods in Tutorial::run() implementations only");

	if (time < 0)
		time = 1000;

	if (buttonEffect == PointerEffect::BUTTONEFFECT_DRAGDROP)
		throw EILLEGALARGUMENT("tutorialClick cannot have a drag-drop button effect. Use tutorialDragDropTo instead.");

	Vector positionEnd = getAbsolutePosition() + getSize() / 2;
	if (Desktop::getInstance()->getPointerPosition() == positionEnd)
		time = 0;

	PointerEffect * pointerEffect;
	if ((pointerEffect = new PointerEffect(time)) == NULL) \
		throw EOUTOFMEMORY;
	pointerEffect->setPositionEnd(positionEnd);
	pointerEffect->setTimeCurveToAcceleration();
	pointerEffect->setButtonEffect(buttonEffect);
	pointerEffect->setButtonOption1(option1);
	pointerEffect->setButtonOption2(option2);

	Desktop::getInstance()->addEffect(pointerEffect);
	pointerEffect->waitFor();
}

void Component::tutorialDragDropTo(Vector position, bool option1, bool option2, long time)
{
	ASSERTION_COBJECT(this);

	if (!Desktop::getInstance()->isTutorialMode())
		throw EILLEGALSTATE("Use the tutorial methods in Tutorial::run() implementations only");

	if (time < 0)
		time = 1000;

	EffectSequence * sequence;
	if ((sequence = new EffectSequence) == NULL)
		throw EOUTOFMEMORY;

	Vector dragPosition = getAbsolutePosition() + getSize() / 2;
	if (Desktop::getInstance()->getPointerPosition() == dragPosition)
		time = 0;

	PointerEffect * effect;
	if ((effect = new PointerEffect(time)) == NULL) \
		throw EOUTOFMEMORY;
	effect->setPositionEnd(dragPosition);
	effect->setTimeCurveToAcceleration();
	sequence->appendEffect(effect);

	if ((effect = new PointerEffect(time)) == NULL) \
		throw EOUTOFMEMORY;
	effect->setPositionEnd(position);
	effect->setTimeCurveToAcceleration();
	effect->setButtonEffect(PointerEffect::BUTTONEFFECT_DRAGDROP);
	effect->setButtonOption1(option1);
	effect->setButtonOption2(option2);
	sequence->appendEffect(effect);

	Desktop::getInstance()->addEffect(sequence);
	effect->waitFor();
}

void Component::tutorialMoveTo(long time)
{
	ASSERTION_COBJECT(this);

	if (!Desktop::getInstance()->isTutorialMode())
		throw EILLEGALSTATE("Use the tutorial methods in Tutorial::run() implementations only");

	Vector positionEnd = getAbsolutePosition() + getSize() / 2;
	if (Desktop::getInstance()->getPointerPosition() == positionEnd)
		return;

	if (time < 0)
		time = 1000;

	PointerEffect * effect;
	if ((effect = new PointerEffect(time)) == NULL) \
		throw EOUTOFMEMORY;
	effect->setPositionEnd(positionEnd);
	effect->setTimeCurveToAcceleration();

	Desktop::getInstance()->addEffect(effect);
	effect->waitFor();
}

void Component::unsetComponentParent(Component * child)
{
	ASSERTION_COBJECT(this);
	ASSERTION_COBJECT(child);

	child->parent = NULL;
}


void Component::Listener::onDestroying(Component * component)
{
	ASSERTION_COBJECT(this);
}

void Component::Listener::onGotFocus(Component * component, bool byParent)
{
	ASSERTION_COBJECT(this);
}

void Component::Listener::onHidden(Component * component)
{
	ASSERTION_COBJECT(this);
}

void Component::Listener::onLostFocus(Component * component)
{
	ASSERTION_COBJECT(this);
}

void Component::Listener::onMoved(Component * component, Vector oldPosition)
{
	ASSERTION_COBJECT(this);
}

void Component::Listener::onResized(Component * component, Vector oldSize)
{
	ASSERTION_COBJECT(this);
}

void Component::Listener::onShown(Component * component)
{
	ASSERTION_COBJECT(this);
}
