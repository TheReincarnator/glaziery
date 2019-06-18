/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Window::Window()
{
	ASSERTION_COBJECT(this);

	size = Vector(256, 128);

	active = true;
	cancelClosing = true;
	clientArea = NULL;
	closable = true;
	destroying = false;
	eventConsuming = false;
	lastPressFocused = false;
	maximizable = true;
	maximized = false;
	menu = NULL;
	movable = true;
	pointerShown = true;
	regularPosition = position;
	regularSize = size;
	resizable = true;
	status = "";
	statused = false;
	stayOnTop = false;
	title = "";
	titleDeferred = title;
	titled = true;

	setCaching(true);
	setFocusAlpha(210);
	setNonFocusAlpha(157);

	Desktop::getInstance()->getSkin()->setupWindow(this);
}

Window::~Window()
{
	ASSERTION_COBJECT(this);

	if (clientArea != NULL) {delete clientArea; clientArea = NULL;}
	if (menu != NULL) {delete menu; menu = NULL;}
}

void Window::adjustClientArea()
{
	ASSERTION_COBJECT(this);

	if (clientArea == NULL)
		return;

	Skin * skin = Desktop::getInstance()->getSkin();
	Vector topLeftFrameSize;
	Vector bottomRightFrameSize;
	skin->getWindowFrameSizes(this, topLeftFrameSize, bottomRightFrameSize);

	Vector clientPosition = topLeftFrameSize;
	Vector clientSize = size - topLeftFrameSize - bottomRightFrameSize;
	moveComponent(clientArea, clientPosition);
	resizeComponent(clientArea, clientSize);
}

void Window::bringToFront(bool focus)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	Window * previousFocusWindow = dynamic_cast<Window *>(desktop->getFocusWindowOrPopup());

	ArrayList<Window> & windows = desktop->windows;
	windows.Unlink(this);
	int targetPosition = stayOnTop ? windows.GetCount() : desktop->getOnTopWindowsBoundary();
	windows.Insert(this, targetPosition);

	if (focus)
		setFocus();

	desktop->notifyPossibleFocusChange(previousFocusWindow);
}

bool Window::close()
{
	ASSERTION_COBJECT(this);

	if (destroying || isDisposed())
		return true;

	int listenersCount = listeners.GetCount();
	int i;
	for (i=0; i<listenersCount; i++)
	{
		Window::Listener * windowListener
			= dynamic_cast<Window::Listener *>(listeners.Get(i));
		if (windowListener != NULL)
			if (windowListener->onClosing(this) == VETO)
				return false;
	}

	destroying = true;

	for (i=0; i<listenersCount; i++)
	{
		Window::Listener * windowListener
			= dynamic_cast<Window::Listener *>(listeners.Get(i));
		if (windowListener != NULL)
			windowListener->onClosed(this);
	}

	setActive(false);

	Desktop::getInstance()->getSkin()->addWindowDestroyEffect(this);

	return true;
}

Menu * Window::createMenu()
{
	ASSERTION_COBJECT(this);

	if (menu != NULL)
		delete menu;

	if ((menu = new Menu(this)) != NULL)
		throw EOUTOFMEMORY;

	adjustClientArea();
	invalidate();

	return menu;
}

void Window::destroy()
{
	ASSERTION_COBJECT(this);

	unfocus();

	Desktop * desktop = Desktop::getInstance();
	desktop->windows.Unlink(this);

	CachableComponent::destroy();
}

void Window::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();
	Skin * skin = desktop->getSkin();

	adapter->clear(context);
	skin->drawWindowBeforeClientArea(context, this);
	if (clientArea != NULL && clientArea->isVisible())
		clientArea->draw(DrawingContext(context, clientArea));
	skin->drawWindowAfterClientArea(context, this);
}

void Window::executeDeferrals()
{
	ASSERTION_COBJECT(this);

	CachableComponent::executeDeferrals();

	if (titleDeferred != title)
		setTitle(titleDeferred);
}

class ClientArea * Window::getClientArea()
{
	ASSERTION_COBJECT(this);
	return clientArea;
}

EventTarget * Window::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = CachableComponent::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	Skin * skin = Desktop::getInstance()->getSkin();
	Vector topLeftFrameSize;
	Vector bottomRightFrameSize;
	skin->getWindowFrameSizes(this, topLeftFrameSize, bottomRightFrameSize);

	if (clientArea != NULL && clientArea->isVisible()
		&& position >= topLeftFrameSize && position < size - bottomRightFrameSize)
		return clientArea->getEventTargetAt(position - topLeftFrameSize);
	else
		return this;
}

Component * Window::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return clientArea;
}

Menu * Window::getMenu()
{
	ASSERTION_COBJECT(this);
	return menu;
}

unsigned long Window::getModulationColor()
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	return skin->getWindowModulationColor(this);
}

Vector Window::getRegularPosition()
{
	ASSERTION_COBJECT(this);
	return maximized ? regularPosition : position;
}

Vector Window::getRegularSize()
{
	ASSERTION_COBJECT(this);
	return maximized ? regularSize : size;
}

const String & Window::getStatus()
{
	ASSERTION_COBJECT(this);
	return status;
}

const String & Window::getTitle()
{
	ASSERTION_COBJECT(this);
	return title;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Window, CachableComponent);
#endif

bool Window::isActive()
{
	ASSERTION_COBJECT(this);
	return active;
}

bool Window::isCancelClosing()
{
	ASSERTION_COBJECT(this);
	return cancelClosing;
}

bool Window::isClosable()
{
	ASSERTION_COBJECT(this);
	return closable;
}

bool Window::isDestroying()
{
	ASSERTION_COBJECT(this);
	return destroying;
}

bool Window::isEventConsuming()
{
	ASSERTION_COBJECT(this);
	return eventConsuming;
}

bool Window::isLastPressFocused()
{
	ASSERTION_COBJECT(this);
	return lastPressFocused;
}

bool Window::isMaximizable()
{
	ASSERTION_COBJECT(this);
	return maximizable;
}

bool Window::isMaximized()
{
	ASSERTION_COBJECT(this);
	return maximized;
}

bool Window::isMovable()
{
	ASSERTION_COBJECT(this);
	return movable;
}

bool Window::isPointerShown()
{
	ASSERTION_COBJECT(this);
	return pointerShown;
}

bool Window::isResizable()
{
	ASSERTION_COBJECT(this);
	return resizable;
}

bool Window::isStatused()
{
	ASSERTION_COBJECT(this);
	return statused;
}

bool Window::isStayOnTop()
{
	ASSERTION_COBJECT(this);
	return stayOnTop;
}

bool Window::isTitled()
{
	ASSERTION_COBJECT(this);
	return titled;
}

void Window::maximize()
{
	ASSERTION_COBJECT(this);

	if (maximized)
		return;

	regularPosition = position;
	regularSize = size;

	position = Vector();
	size = Desktop::getInstance()->getSize();
	maximized = true;

	adjustClientArea();
	invalidate();

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
		{
			componentListener->onMoved(this, regularPosition);
			componentListener->onResized(this, regularSize);
		}

		Window::Listener * windowListener
			= dynamic_cast<Window::Listener *>(listeners.Get(i));
		if (windowListener != NULL)
			windowListener->onMaximized(this);
	}
}

bool Window::moveTo(Vector position)
{
	ASSERTION_COBJECT(this);

	if (maximized)
	{
		regularPosition = position;
		return false;
	}
	else
		return Component::moveTo(position);
}

bool Window::onCancel()
{
	ASSERTION_COBJECT(this);

	bool consumed = CachableComponent::onCancel();
	if (cancelClosing && closable)
	{
		close();
		consumed = true;
	}

	return consumed;
}

void Window::onChildMoved(Component * child, Vector oldPosition)
{
	ASSERTION_COBJECT(this);

	CachableComponent::onChildMoved(child, oldPosition);
	moveInternal(getPosition() + child->getPosition() - oldPosition, true);
}

void Window::onChildMaximumSizeChanged(Component * child, Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	CachableComponent::onChildMaximumSizeChanged(child, oldMaximumSize);
	setMaximumSize(getMaximumSize() + child->getMaximumSize() - oldMaximumSize);
}

void Window::onChildMinimumSizeChanged(Component * child, Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	CachableComponent::onChildMinimumSizeChanged(child, oldMinimumSize);
	setMinimumSize(getMinimumSize() + child->getMinimumSize() - oldMinimumSize);
}

void Window::onChildResized(Component * child, Vector oldSize)
{
	ASSERTION_COBJECT(this);

	CachableComponent::onChildResized(child, oldSize);
	resizeInternal(getSize() + child->getSize() - oldSize, true);
}

void Window::onCloseWidgetClicked()
{
	ASSERTION_COBJECT(this);
	close();
}

void Window::onDestroying()
{
	ASSERTION_COBJECT(this);

	CachableComponent::onDestroying();

	if (clientArea != NULL)
	{
		clientArea->setDisposed();
		clientArea->onDestroying();
	}

	Desktop::getInstance()->windows.Unlink(this);
}

void Window::onMaximumSizeChanged(Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	CachableComponent::onMaximumSizeChanged(oldMaximumSize);

	if (clientArea != NULL)
	{
		Skin * skin = Desktop::getInstance()->getSkin();
		Vector topLeftFrameSize;
		Vector bottomRightFrameSize;
		skin->getWindowFrameSizes(this, topLeftFrameSize, bottomRightFrameSize);

		clientArea->setMaximumSizeInternal(getMaximumSize() - topLeftFrameSize - bottomRightFrameSize, false);
	}
}

void Window::onMinimumSizeChanged(Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	CachableComponent::onMinimumSizeChanged(oldMinimumSize);

	if (clientArea != NULL)
	{
		Skin * skin = Desktop::getInstance()->getSkin();
		Vector topLeftFrameSize;
		Vector bottomRightFrameSize;
		skin->getWindowFrameSizes(this, topLeftFrameSize, bottomRightFrameSize);

		clientArea->setMinimumSizeInternal(getMinimumSize() - topLeftFrameSize - bottomRightFrameSize, false);
	}
}

void Window::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool hadFocus = hasFocus();
	bringToFront();
	lastPressFocused = !hadFocus && hasFocus();

	CachableComponent::onPress(position, option1, option2);
}

void Window::releaseCache()
{
	ASSERTION_COBJECT(this);

	CachableComponent::releaseCache();

	if (clientArea != NULL)
		clientArea->releaseCache();
}

void Window::removeMenu()
{
	ASSERTION_COBJECT(this);

	if (menu != NULL) {delete menu; menu = NULL;}

	adjustClientArea();
	invalidate();
}

bool Window::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	if (maximized)
	{
		regularSize = size;
		return false;
	}
	else
		if (CachableComponent::resizeInternal(size, notifyParent))
		{
			adjustClientArea();
			return true;
		}
		else
			return false;
}

void Window::restore()
{
	ASSERTION_COBJECT(this);

	if (!maximized)
		return;

	position = regularPosition;
	size = regularSize;
	maximized = false;

	adjustClientArea();
	invalidate();

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Component::Listener * componentListener
			= dynamic_cast<Component::Listener *>(listeners.Get(i));
		if (componentListener != NULL)
		{
			componentListener->onMoved(this, Vector());
			componentListener->onResized(this, Desktop::getInstance()->getSize());
		}

		Window::Listener * windowListener
			= dynamic_cast<Window::Listener *>(listeners.Get(i));
		if (windowListener != NULL)
			windowListener->onRestored(this);
	}
}

void Window::sendToBack()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	Window * previousFocusWindow = dynamic_cast<Window *>(desktop->getFocusWindowOrPopup());

	ArrayList<Window> & windows = desktop->windows;
	windows.Unlink(this);
	int targetPosition = stayOnTop ? desktop->getOnTopWindowsBoundary() : 0;
	windows.Insert(this, targetPosition);

	desktop->notifyPossibleFocusChange(previousFocusWindow);
}

void Window::setActive(bool active)
{
	ASSERTION_COBJECT(this);
	this->active = active;
}

void Window::setAdded()
{
	ASSERTION_COBJECT(this);
	added = true;
}

void Window::setCancelClosing(bool cancelClosing)
{
	ASSERTION_COBJECT(this);
	this->cancelClosing = cancelClosing;
}

void Window::setClientArea(ClientArea * clientArea)
{
	ASSERTION_COBJECT(this);

	if (this->clientArea != NULL)
		this->clientArea->destroy();

	this->clientArea = clientArea;

	if (clientArea != NULL)
	{
		setComponentParent(clientArea);

		Skin * skin = Desktop::getInstance()->getSkin();
		Vector topLeftFrameSize;
		Vector bottomRightFrameSize;
		skin->getWindowFrameSizes(this, topLeftFrameSize, bottomRightFrameSize);
		moveComponent(clientArea, topLeftFrameSize);
		resizeInternal(clientArea->getSize() + topLeftFrameSize + bottomRightFrameSize, true);
	}

	invalidate();
}

void Window::setClosable(bool closable)
{
	ASSERTION_COBJECT(this);

	if (this->closable == closable)
		return;

	this->closable = closable;
	invalidate();
}

void Window::setEventConsuming(bool eventConsuming)
{
	ASSERTION_COBJECT(this);
	this->eventConsuming = eventConsuming;
}

void Window::setFocus()
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->setFocusWindow(this);
}

void Window::setMaximizable(bool maximizable)
{
	ASSERTION_COBJECT(this);

	if (this->maximizable == maximizable)
		return;

	this->maximizable = maximizable;
	invalidate();
}

void Window::setMovable(bool movable)
{
	ASSERTION_COBJECT(this);

	if (this->movable == movable)
		return;

	this->movable = movable;
	invalidate();
}

void Window::setPointerShown(bool pointerShown)
{
	ASSERTION_COBJECT(this);
	this->pointerShown = pointerShown;
}

void Window::setResizable(bool resizable)
{
	ASSERTION_COBJECT(this);

	if (this->resizable == resizable)
		return;

	this->resizable = resizable;
	invalidate();
}

void Window::setStatus(const String & status)
{
	ASSERTION_COBJECT(this);

	if (this->status == status)
		return;

	this->status = status;
	invalidate();

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Window::Listener * windowListener
			= dynamic_cast<Window::Listener *>(listeners.Get(i));
		if (windowListener != NULL)
			windowListener->onStatusChanged(this);
	}
}

void Window::setStatused(bool statused)
{
	ASSERTION_COBJECT(this);

	if (this->statused == statused)
		return;

	this->statused = statused;

	adjustClientArea();
	invalidate();

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Window::Listener * windowListener
			= dynamic_cast<Window::Listener *>(listeners.Get(i));
		if (windowListener != NULL)
			windowListener->onStatusChanged(this);
	}
}

void Window::setStayOnTop(bool stayOnTop)
{
	ASSERTION_COBJECT(this);

	if (this->stayOnTop == stayOnTop)
		return;

	this->stayOnTop = stayOnTop;

	if (!added)
		return;

	// If we reach this point, the stay-on-top property has changed.
	// We have to move the window from the on-top to the not-on-top windows or vice versa.
	// The least modifying way is to move it to the boundary between both types of windows.
	// This holds for both cases.

	Desktop * desktop = Desktop::getInstance();
	Window * previousFocusWindow = dynamic_cast<Window *>(desktop->getFocusWindowOrPopup());

	ArrayList<Window> & windows = desktop->windows;
	windows.Unlink(this);
	int targetPosition = desktop->getOnTopWindowsBoundary();
	windows.Insert(this, targetPosition);

	desktop->notifyPossibleFocusChange(previousFocusWindow);
}

void Window::setTitle(const String & title)
{
	ASSERTION_COBJECT(this);

	if (this->title == title)
		return;

	this->title = title;
	titleDeferred = title;

	invalidate();

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Window::Listener * windowListener
			= dynamic_cast<Window::Listener *>(listeners.Get(i));
		if (windowListener != NULL)
			windowListener->onTitleChanged(this);
	}
}

void Window::setTitleDeferred(const String & title)
{
	ASSERTION_COBJECT(this);

	Mutex * mutex = Desktop::getInstance()->getDeferralMutex();
	if (!mutex->lock())
		return;

	titleDeferred = title;
	Desktop::getInstance()->deferObject(this);

	mutex->release();
}

void Window::setTitled(bool titled)
{
	ASSERTION_COBJECT(this);

	if (this->titled == titled)
		return;

	this->titled = titled;

	adjustClientArea();
	invalidate();

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Window::Listener * windowListener
			= dynamic_cast<Window::Listener *>(listeners.Get(i));
		if (windowListener != NULL)
			windowListener->onTitleChanged(this);
	}
}

String Window::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Window(position:%s,size:%s)",
		(const char *) getPosition().toString(), (const char *) getSize().toString());
	return string;
}

void Window::unfocus()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	if (desktop->getFocusWindow() != this)
		return;

	const ArrayList<Window> & windows = desktop->getWindows();
	int windowsCount = windows.GetCount();
	for (int i = windowsCount - 1; i >= 0; i--)
	{
		Window * window = windows.Get(i);
		if (window->isVisible() && window != this)
		{
			desktop->setFocusWindow(window);
			return;
		}
	}

	desktop->setFocusWindow(NULL);
}


Window::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void Window::Listener::onClosed(Window * window)
{
	ASSERTION_COBJECT(this);
}

VetoMode Window::Listener::onClosing(Window * window)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}

void Window::Listener::onMaximized(Window * window)
{
	ASSERTION_COBJECT(this);
}

void Window::Listener::onRestored(Window * window)
{
	ASSERTION_COBJECT(this);
}

void Window::Listener::onStatusChanged(Window * window)
{
	ASSERTION_COBJECT(this);
}

void Window::Listener::onTitleChanged(Window * window)
{
	ASSERTION_COBJECT(this);
}
