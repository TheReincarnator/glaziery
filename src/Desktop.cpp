/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int Desktop::DEFAULT_MAXIMUM_FRAME_TIME = 500;

Desktop * Desktop::singleton = NULL;

// Configurable properties
CF_STRINGLIST_IMPL(Desktop, OnceActions)

Desktop::Desktop(PlatformAdapter * platformAdapter, Skin * skin,
	const String & manufacturer, const String & applicationName)
{
	ASSERTION_COBJECT(this);

	// Initialize random

	srand((unsigned int) platformAdapter->getAbsoluteTime());

	// Set variables

	activeScene = NULL;
	applicationCloseReceived = false;
	this->applicationName = applicationName;
	deferralMutex = NULL;
	doubleClickDelay = 300;
	draggingSurrogate = NULL;
	draggingTested = false;
	dragStartDistance = 4;
	ending = false;
	extraFrameDuration = 0;
	focusWindow = NULL;
	frameTime = 0;
	hourGlassCount = 0;
	hoverFired = true;
	hoverTime = 0;
	initialized = false;
	initTime = 0;
	lastDrawTime = 0;
	lastRunOnceTime = 0;
	lastTime = 0;
	this->manufacturer = manufacturer;
	maximumFrameDuration = DEFAULT_MAXIMUM_FRAME_TIME;
	minimumFrameDuration = 0;
	modal = false;
	this->platformAdapter = platformAdapter;
	pointerMode = POINTER_AUTOMATIC;
	pointerType = NULL;
	primaryPressLastOnHoldTime = 0;
	primaryPressTarget = NULL;
	primaryPressTime = 0;
	secondaryPressTarget = NULL;
	this->skin = skin;
	title = "Glaziery Application";
	tutorial = NULL;
	waitingForTutorialStep = false;

	// Determine config folder, and read the configuration

	CommandLine commandLine;
	commandLine.declareOption("/config", 1);
	commandLine.readFromDefault();
	const ArrayList<String> * configFolderOption = commandLine.getOptionParameters("/config");
	if (configFolderOption != NULL && configFolderOption->GetCount() >= 1)
		configFolder = *configFolderOption->GetFirst();
	else
		configFolder = platformAdapter->getConfigFolder(manufacturer, applicationName);

	if (!config.ReadFromDirectory(configFolder))
		config.Read();
}

Desktop::~Desktop()
{
	ASSERTION_COBJECT(this);

	bool abnormal = false;
	const char * exceptionClassName = NULL;
	char * exceptionMessage = NULL;
	char * exceptionSource = NULL;
	int exceptionLineNumber = 0;

	try
	{
		// Delete any running drag-drop operation's surrogate.
		if (draggingSurrogate != NULL) {delete draggingSurrogate; draggingSurrogate = NULL;}

		if (tutorial != NULL)
			throw EILLEGALSTATE("Tutorial running while shutting down, this should not happen");

		// Cancel all active effects.

		DrawingContext context(this);
		while (!effects.IsEmpty())
		{
			Effect * effect = effects.UnlinkFirst();
			effect->onCancel(context);
			addDisposable(effect);
		}

		// End any running scene

		if (activeScene != NULL)
		{
			LOGPRINTF1(LOG_INFO, "Exiting scene '%s'", activeScene->getId());
			activeScene->onExit();
			activeScene = NULL;
		}

		// Force deletion before adapter is deleted,
		// because window deletion requires the adapter.

		while (!popups.IsEmpty())
			popups.GetFirst()->destroy();

		while (!windows.IsEmpty())
			windows.GetFirst()->destroy();

		PointeredList * scenesList = scenes.GetValues();
		PointeredListItem * scenesListItem = scenesList->GetFirst();
		while (scenesListItem != NULL)
		{
			Scene * scene = (Scene *) scenesListItem->GetData();
			LOGPRINTF1(LOG_INFO, "Shutting down scene '%s'", scene->getId());
			scene->onShutDown();
			LOGPRINTF1(LOG_INFO, "Scene '%s' shut down", scene->getId());
			scenesListItem = scenesListItem->GetNext();
		}
		delete scenesList;

		config.RemoveRootConfigurable("Glaziery");

		// Then delete all disposables explicitly

		processDisposableEffects();
		processDisposableComponents();
	}
	catch (EOutOfMemory & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
	}
	catch (Exception & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
		if ((exceptionMessage = new char[strlen(exception.GetMessage()) + 1]) != NULL)
			strcpy(exceptionMessage, exception.GetMessage());
		if ((exceptionSource = new char[strlen(exception.GetSource()) + 1]) != NULL)
			strcpy(exceptionSource, exception.GetSource());
		exceptionLineNumber = exception.GetLineNumber();
	}
	catch (...)
	{
		abnormal = true;
	}

	try
	{
		if (skin != NULL)
		{
			skin->shutDown();
			delete skin;
			skin = NULL;
		}
	}
	catch (EOutOfMemory & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
	}
	catch (Exception & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
		if ((exceptionMessage = new char[strlen(exception.GetMessage()) + 1]) != NULL)
			strcpy(exceptionMessage, exception.GetMessage());
		if ((exceptionSource = new char[strlen(exception.GetSource()) + 1]) != NULL)
			strcpy(exceptionSource, exception.GetSource());
		exceptionLineNumber = exception.GetLineNumber();
	}
	catch (...)
	{
		abnormal = true;
	}

	try
	{
		if (deferralMutex != NULL)
		{
			delete deferralMutex;
			deferralMutex = NULL;
		}
	}
	catch (EOutOfMemory & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
	}
	catch (Exception & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
		if ((exceptionMessage = new char[strlen(exception.GetMessage()) + 1]) != NULL)
			strcpy(exceptionMessage, exception.GetMessage());
		if ((exceptionSource = new char[strlen(exception.GetSource()) + 1]) != NULL)
			strcpy(exceptionSource, exception.GetSource());
		exceptionLineNumber = exception.GetLineNumber();
	}
	catch (...)
	{
		abnormal = true;
	}

	String configFolder;
	try
	{
		configFolder = getConfigFolder();
	}
	catch (EOutOfMemory & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
	}
	catch (Exception & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
		if ((exceptionMessage = new char[strlen(exception.GetMessage()) + 1]) != NULL)
			strcpy(exceptionMessage, exception.GetMessage());
		if ((exceptionSource = new char[strlen(exception.GetSource()) + 1]) != NULL)
			strcpy(exceptionSource, exception.GetSource());
		exceptionLineNumber = exception.GetLineNumber();
	}
	catch (...)
	{
		abnormal = true;
	}

	try
	{
		if (platformAdapter != NULL)
		{
			platformAdapter->shutDown();
			delete platformAdapter;
			platformAdapter = NULL;
		}
	}
	catch (EOutOfMemory & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
	}
	catch (Exception & exception)
	{
		abnormal = true;
		exceptionClassName = exception.GetClassName();
		if ((exceptionMessage = new char[strlen(exception.GetMessage()) + 1]) != NULL)
			strcpy(exceptionMessage, exception.GetMessage());
		if ((exceptionSource = new char[strlen(exception.GetSource()) + 1]) != NULL)
			strcpy(exceptionSource, exception.GetSource());
		exceptionLineNumber = exception.GetLineNumber();
	}
	catch (...)
	{
		abnormal = true;
	}

	try
	{
		if (!abnormal)
			config.WriteToDirectory(configFolder);
	}
	catch (...)
	{
		abnormal = true;
	}

	if (abnormal)
	{
		String report("Failed to shut down Glaziery Desktop, root cause: ");
		if (exceptionClassName != NULL)
		{
			report += exceptionClassName;
			if (exceptionMessage != NULL)
			{
				report += ": ";
				report += exceptionMessage;
				delete exceptionMessage;
			}
			if (exceptionSource != NULL)
			{
				report += " @ ";
				report += exceptionSource;
				delete exceptionSource;
				report += ":";
				report += exceptionLineNumber;
			}
		}

		throw EILLEGALSTATE(report);
	}
}

void Desktop::addDisposable(Component * component)
{
	ASSERTION_COBJECT(this);
	disposableComponents.Append(component);
}

void Desktop::addDisposable(Effect * effect)
{
	ASSERTION_COBJECT(this);
	disposableEffects.Append(effect);
}

void Desktop::addEffect(Effect * effect)
{
	ASSERTION_COBJECT(this);

	effects.Append(effect);
	effect->addReference();

	effect->onAdded();
}

void Desktop::addEvent(InputEvent * event)
{
	ASSERTION_COBJECT(this);
	events.Append(event);
}

void Desktop::addPopup(Popup * popup)
{
	ASSERTION_COBJECT(this);

	if (dynamic_cast<BalloonPopup *>(popup) != NULL)
		cancelBalloonPopups();

	popups.Append(popup);
	skin->addPopupCreateEffect(popup);
}

void Desktop::addScene(Scene * scene)
{
	ASSERTION_COBJECT(this);

	if (scenes.ContainsKey(scene->getId()))
		throw EILLEGALARGUMENT("The scene's ID is already known to the Desktop");

	scenes.Set(scene->getId(), scene);
}

void Desktop::addWindow(Window * window, bool focus)
{
	ASSERTION_COBJECT(this);

	ASSERTION(window != NULL);

	cancelDragging();

	int targetPosition = window->isStayOnTop() ? windows.GetCount() : getOnTopWindowsBoundary();
	windows.Insert(window, targetPosition);
	window->setAdded();

	Dialog * dialog = dynamic_cast<Dialog *>(window);
	if (dialog != NULL)
		updateModal();

	skin->addWindowCreateEffect(window);

	if (focus)
		setFocusWindow(window);
}

bool Desktop::alreadyDone(const char * id, bool set)
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<OnceActions.GetCount(); i++)
		if (*OnceActions.Get(i) == id)
			return true;

	if (set)
	{
		String * idString;
		if ((idString = new String(id)) == NULL)
			throw EOUTOFMEMORY;
		OnceActions.Append(idString);
	}

	return false;
}

void Desktop::cancelAllPopups()
{
	ASSERTION_COBJECT(this);

	for (int i = popups.GetCount() - 1; i >= 0; i--)
	{
		Popup * popup = popups.Get(i);
		if (!popup->isDisposed())
			popup->cancel();
	}
}

void Desktop::cancelBalloonPopups()
{
	ASSERTION_COBJECT(this);

	for (int i = popups.GetCount() - 1; i >= 0; i--)
	{
		BalloonPopup * popup = dynamic_cast<BalloonPopup *>(popups.Get(i));
		if (popup != NULL && !popup->isDisposed())
			popup->cancel();
	}
}

void Desktop::cancelDragging()
{
	ASSERTION_COBJECT(this);

	if (draggingSurrogate == NULL)
		return;

	draggingSurrogate->onCanceled(primaryPressTarget);
	delete draggingSurrogate;
	draggingSurrogate = NULL;

	updatePointerType();
}

void Desktop::cancelTutorial()
{
	ASSERTION_COBJECT(this);

	if (tutorial != NULL)
		tutorial->cancel();
}

void Desktop::clearClipboard()
{
	ASSERTION_COBJECT(this);
	platformAdapter->clearClipboard();
}

void Desktop::Configure(ConfigSection * section, bool write)
{
	ASSERTION_COBJECT(this);

	section->Configure("InputManager", inputManager, write);
	section->Configure("OnceActions", OnceActions, write);
	section->Configure("PlatformAdapter", platformAdapter, write);

	// Scene configuration happens when they are initialized
	if (write)
	{
		PointeredList * scenesList = scenes.GetValues();
		PointeredListItem * scenesListItem = scenesList->GetFirst();
		while (scenesListItem != NULL)
		{
			Scene * scene = (Scene *) scenesListItem->GetData();
			if (scene->isConfigured())
            scene->Configure(section->GetSection(scene->getId()), true);

			scenesListItem = scenesListItem->GetNext();
		}
		delete scenesList;
	}

	skin->Configure(section->GetSection("Skin"), write);
}

Desktop * Desktop::createInstance(PlatformAdapter * platformAdapter, Skin * skin,
	const String & manufacturer, const String & applicationName)
{
	if (singleton != NULL)
		throw EILLEGALSTATE("The Desktop instance already exists. Use destroyInstance first().");

	LOGPRINTF0(LOG_INFO, "Creating Glaziery desktop instance");

	if ((singleton = new Desktop(platformAdapter, skin, manufacturer, applicationName)) == NULL)
		throw EOUTOFMEMORY;
	singleton->getConfig().AddRootConfigurable("Glaziery", singleton);

	return singleton;
}

Thread * Desktop::createThread(int (*function)(Thread & thread, void * parameter),
	bool progressDialog, void * parameter)
{
	ASSERTION_COBJECT(this);

	Thread * thread = platformAdapter->createThread(function, parameter);

	if (progressDialog)
	{
		ProgressDialog * progressDialog = ProgressDialog::createDialog(thread);
		thread->setProgressDialog(progressDialog);
	}

	return thread;
}

void Desktop::deferObject(EventTarget * object)
{
	ASSERTION_COBJECT(this);

	if (!deferralMutex->lock())
		throw EILLEGALSTATE("Locking did not succeed");
	SET_OWNER_THREAD(&deferredObjects);

	deferredObjects.Append(object);

	deferralMutex->release();
}

void Desktop::destroyInstance()
{
	if (singleton != NULL)
	{
		try
		{
			delete singleton;
		}
		catch (...)
		{
			singleton = NULL;
			throw;
		}

		singleton = NULL;
	}
}

void Desktop::draw()
{
	ASSERTION_COBJECT(this);

	unsigned long start = platformAdapter->getTime();

	DrawingContext context(this);

	// Let the skin draw the desktop background
	skin->drawDesktop(context);

	// Then let the scene draw it, too
	if (activeScene != NULL)
		activeScene->renderBackground(context);

	// Now draw regular windows in back-to-front order
	int windowsCount = windows.GetCount();
	int i;
	for (i=0; i<windowsCount; i++)
	{
		Window * window = windows.Get(i);
		if (window->isVisible())
			window->draw(DrawingContext(context, window));
	}

	// Now draw popup windows in back-to-front order
	int popupsCount = popups.GetCount();
	for (i=0; i<popupsCount; i++)
	{
		Popup * popup = popups.Get(i);
		if (popup->isVisible())
			popup->draw(DrawingContext(context, popup));
	}

	// Let the scene draw the desktop foreground
	if (activeScene != NULL)
		activeScene->renderForeground(context);

	// Draw the splash text/image
	skin->drawSplash(context);

	// Draw any effects
	int effectCount = effects.GetCount();
	for (i=0; i<effectCount; i++)
		effects.Get(i)->draw(context);

	// Draw the optional drag-drop surrogate
	if (draggingSurrogate != NULL)
		draggingSurrogate->draw(context);

	// Draw the pointer
	if (isPointerCurrentlyShown() && pointerType != NULL)
		pointerType->draw(context, platformAdapter->getMouseHandler()->getPointerPosition());

	// Finally, draw the CPU profile, if active
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::OTHER);
		profiler.drawProfile();
	#endif

	// Show the frame
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::IDLE);
	#endif
	platformAdapter->showFrame();

	unsigned long end = platformAdapter->getTime();
	lastDrawTime = end - start;
}

void Desktop::end()
{
	ASSERTION_COBJECT(this);
	ending = true;
}

void Desktop::executeDeferrals()
{
	ASSERTION_COBJECT(this);

	if (!deferralMutex->lock(100))
		return;
	SET_OWNER_THREAD(&deferredObjects);

	while (!deferredObjects.IsEmpty())
	{
		EventTarget * object = deferredObjects.UnlinkFirst();
		object->executeDeferrals();
	}

	deferralMutex->release();
}

void Desktop::executeEffects()
{
	ASSERTION_COBJECT(this);

	DrawingContext context(this);

	// First, remove last frame's disposable effects
	processDisposableEffects();

	// Execute effects, if not currently executing modally
	for (int i=0; i<effects.GetCount(); i++)
	{
		Effect * effect = effects.Get(i);
		if (effect->isExecuting())
			continue;

		effect->setExecuting(true);

		if (effect->isCanceled())
		{
			effect->onCancel(context);
			effect->setEnded();

			// Search effect in list, because the above method may have changed the list
			int newPos = effects.Find(effect);
			if (newPos >= 0)
			{
				i = newPos;
				effects.Unlink(i--);
			}

			addDisposable(effect);
			continue;
		}

		if (!effect->execute(context, getTimeStep()))
		{
			// Search effect in list, because the above method may have changed the list
			int newPos = effects.Find(effect);
			if (newPos >= 0)
			{
				i = newPos;
				effects.Unlink(i--);
			}

			effect->setEnded();
			addDisposable(effect);
			continue;
		}

		effect->setExecuting(false);

		// Search effect in list, because the above method may have changed the list
		int newPos = effects.Find(effect);
		if (newPos >= 0)
			i = newPos;
	}
}

unsigned long Desktop::getAbsoluteTime()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->getAbsoluteTime();
}

Scene * Desktop::getActiveScene()
{
	ASSERTION_COBJECT(this);
	return activeScene;
}

const char * Desktop::getActiveSceneId()
{
	ASSERTION_COBJECT(this);
	return activeScene != NULL ? activeScene->getId() : NULL;
}

const String & Desktop::getApplicationName()
{
	ASSERTION_COBJECT(this);
	return applicationName;
}

String Desktop::getClipboardText()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->getClipboardText();
}

Config & Desktop::getConfig()
{
	ASSERTION_COBJECT(this);
	return config;
}

String Desktop::getConfigFolder()
{
	ASSERTION_COBJECT(this);
	return configFolder;
}

unsigned long Desktop::getCurrentTime()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->getTime();
}

Mutex * Desktop::getDeferralMutex()
{
	ASSERTION_COBJECT(this);
	return deferralMutex;
}

long Desktop::getDoubleClickDelay()
{
	ASSERTION_COBJECT(this);
	return doubleClickDelay;
}

EventTarget * Desktop::getDraggedObject()
{
	ASSERTION_COBJECT(this);
	return draggingSurrogate != NULL ? primaryPressTarget : NULL;
}

DraggingSurrogate * Desktop::getDraggingSurrogate()
{
	ASSERTION_COBJECT(this);
	return draggingSurrogate;
}

int Desktop::getDragStartDistance()
{
	ASSERTION_COBJECT(this);
	return dragStartDistance;
}

const ArrayList<Effect> & Desktop::getEffects()
{
	ASSERTION_COBJECT(this);
	return effects;
}

EventTarget * Desktop::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	if (!isPointerCurrentlyShown())
		return NULL;

	int popupsCount = popups.GetCount();
	int i;
	for (i = popupsCount - 1; i >= 0; i--)
	{
		Popup * popup = popups.Get(i);
		if (popup->isVisible() && position >= popup->getPosition()
			&& position < popup->getPosition() + popup->getSize())
			return popup->getEventTargetAt(position - popup->getPosition());
	}

	int windowsCount = windows.GetCount();
	for (i = windowsCount - 1; i >= 0; i--)
	{
		Window * window = windows.Get(i);
		if (window->isVisible() && position >= window->getPosition()
			&& position < window->getPosition() + window->getSize())
		{
			if (!window->isActive())
				return NULL;

			return window->getEventTargetAt(position - window->getPosition());
		}

		// If we are in modal mode, use only the top window
		if (modal)
			return NULL;
	}

	return NULL;
}

Component * Desktop::getFocusComponent()
{
	ASSERTION_COBJECT(this);

	Component * component = getFocusWindowOrPopup();
	if (component == NULL)
		return NULL;

	Component * child = component->getFocusChild();
	while (child != NULL)
	{
		component = child;
		child = child->getFocusChild();
	}

	return component;
}

Window * Desktop::getFocusWindow()
{
	ASSERTION_COBJECT(this);
	return focusWindow;
}

CachableComponent * Desktop::getFocusWindowOrPopup()
{
	ASSERTION_COBJECT(this);

	int popupCount = popups.GetCount();
	for (int i = popupCount - 1; i>=0; i--)
	{
		Popup * popup = popups.Get(i);
		if (popup->isVisible())
			return popup;
	}

	if (focusWindow != NULL && focusWindow->isVisible())
		return focusWindow;

	return NULL;
}

unsigned long Desktop::getExtraFrameDuration()
{
	ASSERTION_COBJECT(this);
	return extraFrameDuration;
}

unsigned long Desktop::getFrameTime()
{
	ASSERTION_COBJECT(this);
	return frameTime;
}

InputManager & Desktop::getInputManager()
{
	ASSERTION_COBJECT(this);
	return inputManager;
}

Desktop * Desktop::getInstance()
{
	return singleton;
}

unsigned long Desktop::getLastDrawTime()
{
	ASSERTION_COBJECT(this);
	return lastDrawTime;
}

unsigned long Desktop::getLastTime()
{
	ASSERTION_COBJECT(this);
	return lastTime;
}

const String & Desktop::getManufacturer()
{
	ASSERTION_COBJECT(this);
	return manufacturer;
}

int Desktop::getMasterVolume()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->getMasterVolume();
}

unsigned long Desktop::getMaximumFrameDuration()
{
	ASSERTION_COBJECT(this);
	return maximumFrameDuration;
}

unsigned long Desktop::getMinimumFrameDuration()
{
	ASSERTION_COBJECT(this);
	return minimumFrameDuration;
}

int Desktop::getOnTopWindowsBoundary()
{
	ASSERTION_COBJECT(this);

	int boundary;
	for (boundary = 0; boundary < windows.GetCount(); boundary++)
		if (windows.Get(boundary)->isStayOnTop())
			break;

	return boundary;
}

Vector Desktop::getOrigin()
{
	ASSERTION_COBJECT(this);
	return Vector();
}

Component * Desktop::getParent()
{
	ASSERTION_COBJECT(this);
	return NULL;
}

class PlatformAdapter * Desktop::getPlatformAdapter()
{
	ASSERTION_COBJECT(this);
	return platformAdapter;
}

Vector Desktop::getPointerPosition()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->getMouseHandler()->getPointerPosition();
}

PointerType * Desktop::getPointerType()
{
	ASSERTION_COBJECT(this);
	return pointerType;
}

const ArrayList<Popup> & Desktop::getPopups()
{
	ASSERTION_COBJECT(this);
	return popups;
}

#ifdef GL_DEBUG_CPUPROFILER
	CPUProfiler & Desktop::getProfiler()
	{
		return profiler;
	}
#endif

Scene * Desktop::getScene(const char * id)
{
	ASSERTION_COBJECT(this);
	return scenes.Get(id);
}

const StringKeyHashMap<Scene> & Desktop::getScenes()
{
	ASSERTION_COBJECT(this);
	return scenes;
}

Vector Desktop::getSize()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->getScreenSize();
}

class Skin * Desktop::getSkin()
{
	ASSERTION_COBJECT(this);
	return skin;
}

unsigned long Desktop::getTimeModulo(unsigned long modulo)
{
	ASSERTION_COBJECT(this);
	return frameTime % modulo;
}

unsigned long Desktop::getTimeScaled(unsigned long milliSecs)
{
	ASSERTION_COBJECT(this);
	return frameTime / milliSecs;
}

unsigned long Desktop::getTimeScaledModulo(unsigned long milliSecs, unsigned long modulo)
{
	ASSERTION_COBJECT(this);
	return (frameTime / milliSecs) % modulo;
}

unsigned long Desktop::getTimeStep()
{
	ASSERTION_COBJECT(this);
	return frameTime - lastTime;
}

unsigned long Desktop::getTimeStepScaled(unsigned long milliSecs)
{
	ASSERTION_COBJECT(this);
	return frameTime / milliSecs - lastTime / milliSecs;
}

const String & Desktop::getTitle()
{
	ASSERTION_COBJECT(this);
	return title;
}

Tutorial * Desktop::getTutorial()
{
	ASSERTION_COBJECT(this);
	return tutorial;
}

const ArrayList<Window> & Desktop::getWindows()
{
	ASSERTION_COBJECT(this);
	return windows;
}

bool Desktop::hasFocusWindowOrPopup()
{
	ASSERTION_COBJECT(this);
	return getFocusWindowOrPopup() != NULL;
}

bool Desktop::hasPopup()
{
	ASSERTION_COBJECT(this);
	return !popups.IsEmpty();
}

void Desktop::hideHourGlass()
{
	ASSERTION_COBJECT(this);

	if (hourGlassCount > 0)
		hourGlassCount--;

	if (hourGlassCount == 0)
		updatePointerType();
}

void Desktop::hidePointer()
{
	ASSERTION_COBJECT(this);
	pointerMode = POINTER_HIDDEN;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Desktop, EventTarget);
#endif

void Desktop::initialize()
{
	ASSERTION_COBJECT(this);

	if (initialized)
		return;

	if (activeScene == NULL)
		throw EILLEGALSTATE("Before calling run(), you must set an active scene using setActiveScene");

	LOGPRINTF0(LOG_INFO, "Initializing Glaziery");

	// Initialize platform adapter, deferral mutex, and skin

	platformAdapter->initialize();
	deferralMutex = platformAdapter->createMutex();
	skin->initialize();
	skin->setupDesktop();

	// Initialize timing

	singleton->initTime = platformAdapter->getTime();
	singleton->setFrameTime(false);

	// Initialize scenes

	ConfigSection * glazierySection = config.GetContent()->GetSection("Glaziery");
	PointeredList * scenesList = scenes.GetValues();
	PointeredListItem * scenesListItem = scenesList->GetFirst();
	while (scenesListItem != NULL)
	{
		Scene * scene = (Scene *) scenesListItem->GetData();
		LOGPRINTF1(LOG_INFO, "Initializing scene '%s'", (const char *) scene->getId());

		scene->Configure(glazierySection->GetSection(scene->getId()), false);

		String sceneFirstRunId;
		sceneFirstRunId.Format("glaziery.sceneFirstRun.%s", (const char *) scene->getId());
		scene->onInitialize(!alreadyDone(sceneFirstRunId));

		LOGPRINTF1(LOG_INFO, "Scene '%s' initialized", scene->getId());
		scenesListItem = scenesListItem->GetNext();
	}
	delete scenesList;

	// Initialize variables after adapter, skin, and scenes are up

	singleton->pointerType = skin->getSelectPointerType();

	LOGPRINTF0(LOG_INFO, "Glaziery initialized");

	initialized = true;

	// Reset the profiler

	#ifdef GL_DEBUG_CPUPROFILER
		profiler.reset();
	#endif

	// Enter initial scene

	LOGPRINTF1(LOG_INFO, "Entering scene '%s'", activeScene->getId());
	activeScene->onEnter();
}

bool Desktop::isApplicationFocused()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->isApplicationFocused();
}

bool Desktop::isDragging()
{
	ASSERTION_COBJECT(this);
	return draggingSurrogate != NULL;
}

bool Desktop::isEnding()
{
	ASSERTION_COBJECT(this);
	return ending;
}

bool Desktop::isExclusive()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->isExclusive();
}

bool Desktop::isFullScreen()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->isFullScreen();
}

bool Desktop::isInitialized()
{
	ASSERTION_COBJECT(this);
	return initialized;
}

bool Desktop::isKeyStrokeMode()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	Component * component = desktop->getFocusComponent();
	return dynamic_cast<KeyStrokeField *>(component) != NULL;
}

bool Desktop::isModal()
{
	ASSERTION_COBJECT(this);
	return modal;
}

bool Desktop::isMute()
{
	ASSERTION_COBJECT(this);
	return platformAdapter->isMute();
}

bool Desktop::isPointerHidden()
{
	ASSERTION_COBJECT(this);
	return pointerMode == POINTER_HIDDEN;
}

bool Desktop::isPointerAlwaysShown()
{
	ASSERTION_COBJECT(this);
	return pointerMode == POINTER_SHOWN;
}

bool Desktop::isPointerAutomatic()
{
	ASSERTION_COBJECT(this);
	return pointerMode == POINTER_AUTOMATIC;
}

bool Desktop::isPointerCurrentlyShown()
{
	ASSERTION_COBJECT(this);

	if (pointerMode == POINTER_SHOWN)
		return true;
	else if (pointerMode == POINTER_HIDDEN)
		return false;
	else if (tutorial != NULL)
		return true;

	int popupCount = popups.GetCount();
	for (int i = popupCount - 1; i>=0; i--)
	{
		Popup * popup = popups.Get(i);
		if (popup->isVisible())
			return true;
	}

	if (focusWindow != NULL && focusWindow->isVisible())
		return focusWindow->isPointerShown();

	return false;
}

bool Desktop::isShowingHourGlass()
{
	ASSERTION_COBJECT(this);
	return hourGlassCount > 0;
}

bool Desktop::isTutorialMode()
{
	ASSERTION_COBJECT(this);
	return tutorial != NULL;
}

void Desktop::minimize()
{
	ASSERTION_COBJECT(this);

	inputManager.resetHelds();
	platformAdapter->minimizeDesktop();
}

void Desktop::movePointer(Vector position)
{
	ASSERTION_COBJECT(this);
	platformAdapter->getMouseHandler()->setPointerPosition(position);
}

void Desktop::notifyPossibleFocusChange(Window * previousFocusWindow)
{
	ASSERTION_COBJECT(this);

	Window * currentFocusWindow = dynamic_cast<Window *>(getFocusWindowOrPopup());

	if (previousFocusWindow != currentFocusWindow)
	{
		if (previousFocusWindow != NULL)
		{
			previousFocusWindow->onLostFocus();
			previousFocusWindow->invalidate();
		}

		if (currentFocusWindow != NULL)
		{
			currentFocusWindow->onGotFocus(false);
			currentFocusWindow->invalidate();
		}
	}
}

void Desktop::onApplicationCloseReceived()
{
	ASSERTION_COBJECT(this);
	applicationCloseReceived = true;
}

void Desktop::onTutorialStep(bool cancel)
{
	ASSERTION_COBJECT(this);

	if (!isModal())
		waitingForTutorialStep = false;

	if (cancel)
		cancelTutorial();
}

void Desktop::openUrl(const String & url)
{
	ASSERTION_COBJECT(this);
	platformAdapter->openUrl(url);
}

void Desktop::processDisposableComponents()
{
	ASSERTION_COBJECT(this);

	while (!disposableComponents.IsEmpty())
	{
		Component * component = disposableComponents.UnlinkFirst();
		ASSERTION_COBJECT(component);
		delete component;
	}
}

void Desktop::processDisposableEffects()
{
	ASSERTION_COBJECT(this);

	while (!disposableEffects.IsEmpty())
		disposableEffects.UnlinkFirst()->release();
}

void Desktop::processEvents()
{
	ASSERTION_COBJECT(this);

	// Process the event queue
	while (!events.IsEmpty())
	{
		InputEvent * event = events.UnlinkFirst();
		event->execute();
		delete event;
	}

	Vector pointerPosition = platformAdapter->getMouseHandler()->getPointerPosition();
	if (tutorial == NULL && isPointerCurrentlyShown())
	{
		// Update and issue pseudo event onHover()
		Vector hoverDistance = (pointerPosition - hoverPosition).absolute();
		if (!(hoverDistance <= Vector(4, 4)))
		{
			hoverPosition = pointerPosition;
			hoverFired = false;
			hoverTime = frameTime;
		}
		else if (!hoverFired && frameTime >= hoverTime + 600)
		{
			bool anyNonBalloonPopup = false;
			for (int i=0; i<popups.GetCount(); i++)
				if (dynamic_cast<BalloonPopup *>(popups.Get(i)) == NULL)
				{
					anyNonBalloonPopup = true;
					break;
				}

			if (!anyNonBalloonPopup)
			{
				EventTarget * eventTarget = getEventTargetAt(pointerPosition);
				if (eventTarget != NULL)
					eventTarget->onHover();
			}

			hoverFired = true;
		}
	}

	// Issue pseudo event onHold() if a primaryPressTarget is set
	if (primaryPressTarget != NULL && frameTime > primaryPressLastOnHoldTime)
	{
		Vector position = pointerPosition - primaryPressTarget->getOrigin();
		unsigned long notConsumedTime = primaryPressTarget->onHold(position,
			frameTime - primaryPressLastOnHoldTime);
		primaryPressLastOnHoldTime = frameTime - notConsumedTime;
	}

	// Handle application close
	if (applicationCloseReceived)
	{
		applicationCloseReceived = false;

		bool veto = false;
		int listenersCount = listeners.GetCount();
		for (int i=0; i<listenersCount; i++)
		{
			Desktop::Listener * desktopListener
				= dynamic_cast<Desktop::Listener *>(listeners.Get(i));
			if (desktopListener != NULL)
				if (desktopListener->onApplicationCloseReceived() == VETO)
				{
					veto = true;
					break;
				}
		}

		if (!veto)
			end();
	}
}

void Desktop::releaseReferences(EventTarget * eventTarget)
{
	ASSERTION_COBJECT(this);

	if (primaryPressTarget == eventTarget)
		primaryPressTarget = NULL;
	if (secondaryPressTarget == eventTarget)
		secondaryPressTarget = NULL;
}

void Desktop::restore()
{
	ASSERTION_COBJECT(this);
	platformAdapter->restoreDesktop();
}

void Desktop::run()
{
	ASSERTION_COBJECT(this);

	if (!initialized)
		initialize();

	while (!ending)
	{
		// First, remove last frame's disposables
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::OTHER);
		#endif
		processDisposableComponents();

		// Update timing and wait if necessary
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::IDLE);
		#endif
		updateTimingAndWait();

		// Now handle platform adapter's operations
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::OTHER);
		#endif
		platformAdapter->update();

		// Perform regular skin operations
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::OTHER);
		#endif
		skin->update();

		// And update the input manager
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::IO);
		#endif
		inputManager.update();

		// Execute pending events now
		// (issued by the platform adapter's input handlers)
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::APPLICATION);
		#endif
		processEvents();

		// Update the scene
		if (activeScene != NULL)
		{
			#ifdef GL_DEBUG_CPUPROFILER
				profiler.setUsageType(CPUProfiler::APPLICATION);
			#endif
			activeScene->update();
		}

		// Execute deferred operations
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::APPLICATION);
		#endif
		executeDeferrals();

		// Execute effects
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::OTHER);
		#endif
		executeEffects();

		// Finally draw the entire desktop, if the scene is not suspended
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::GRAPHICS);
		#endif
		draw();
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::OTHER);
		#endif

		// Calculate frame times
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.onFrame();
		#endif
	}

	// End any running tutorial
	if (tutorial != NULL)
	{
		tutorial->cancel();
		while (tutorial != NULL)
			runOnce();
	}
}

void Desktop::runOnce(unsigned long minimumTime)
{
	ASSERTION_COBJECT(this);

	// Skip too often calls
	unsigned long now = platformAdapter->getTime();
	if (minimumTime > 0 && now - lastRunOnceTime < minimumTime)
		return;
	lastRunOnceTime = now;

	if (!initialized)
		throw EILLEGALSTATE("Desktop::runOnce(20) is allowed only after (while) calling run()");

	// Execute deferred operations
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::APPLICATION);
	#endif
	executeDeferrals();

	// Execute effects
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::OTHER);
	#endif
	executeEffects();

	// Now draw the entire desktop, if the scene is not suspended
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::GRAPHICS);
	#endif
	draw();
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::OTHER);
	#endif

	// Calculate frame times
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.onFrame();
	#endif

	// Remove disposable components issued above
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::OTHER);
	#endif
	processDisposableComponents();

	// Update timing and wait if necessary
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::IDLE);
	#endif
	updateTimingAndWait();

	// Now handle platform adapter's operations
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::OTHER);
	#endif
	platformAdapter->update();

	// Perform regular skin operations
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::OTHER);
	#endif
	skin->update();

	// And update the input manager
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::IO);
	#endif
	getInputManager().update();

	// Execute pending events now
	// (issued by the platform adapter's input handlers)
	#ifdef GL_DEBUG_CPUPROFILER
		profiler.setUsageType(CPUProfiler::APPLICATION);
	#endif
	processEvents();

	// Finally, update the scene
	if (activeScene != NULL)
	{
		#ifdef GL_DEBUG_CPUPROFILER
			profiler.setUsageType(CPUProfiler::APPLICATION);
		#endif
		activeScene->update();
	}
}

void Desktop::saveConfig()
{
	ASSERTION_COBJECT(this);
	config.WriteToDirectory(getConfigFolder());
}

void Desktop::setActiveScene(const char * id)
{
	ASSERTION_COBJECT(this);
	setActiveScene(getScene(id));
}

void Desktop::setActiveScene(Scene * scene)
{
	ASSERTION_COBJECT(this);

	if (scene == NULL)
		throw EILLEGALARGUMENT("Unknown scene ID");

	if (activeScene != NULL)
	{
		LOGPRINTF1(LOG_INFO, "Exiting scene '%s'", activeScene->getId());
		activeScene->onExit();
	}

	activeScene = scene;

	if (initialized)
	{
		LOGPRINTF1(LOG_INFO, "Entering scene '%s'", activeScene->getId());
		if (activeScene->onEnter() == Scene::RESTART_ADAPTER)
		{
			LOGPRINTF0(LOG_INFO, "Scene issues an adapter restart");
			platformAdapter->shutDown();
			platformAdapter->initialize();
			LOGPRINTF0(LOG_INFO, "Adapter restarted");
		}
	}
}

void Desktop::setClipboardText(const String & text)
{
	ASSERTION_COBJECT(this);
	platformAdapter->setClipboardText(text);
}

void Desktop::setDoubleClickDelay(long doubleClickDelay)
{
	ASSERTION_COBJECT(this);
	this->doubleClickDelay = doubleClickDelay;
}

void Desktop::setDragStartDistance(int dragStartDistance)
{
	ASSERTION_COBJECT(this);
	this->dragStartDistance = dragStartDistance;
}

void Desktop::setExtraFrameDuration(unsigned long extraFrameDuration)
{
	ASSERTION_COBJECT(this);
	this->extraFrameDuration = extraFrameDuration;
}

void Desktop::setFocusWindow(Window * focusWindow)
{
	ASSERTION_COBJECT(this);

	Window * previousFocusWindow = dynamic_cast<Window *>(getFocusWindowOrPopup());
	this->focusWindow = focusWindow;
	notifyPossibleFocusChange(previousFocusWindow);
}

void Desktop::setFrameTime(bool noWarnings)
{
	ASSERTION_COBJECT(this);

	lastTime = frameTime;
	frameTime = platformAdapter->getTime() - initTime;

	// Just to make sure, if any real frameTime calculation takes
	// longer than real frameTime, so that it cannot build up.
	if (getTimeStep() > maximumFrameDuration)
	{
		#ifdef _DEBUG
			if (!noWarnings)
			{
				LOGPRINTF1(LOG_WARN, "Loop took more than %d millisecs", maximumFrameDuration);
			}
		#endif

		initTime += getTimeStep() - maximumFrameDuration;
		frameTime -= getTimeStep() - maximumFrameDuration;
	}
}

void Desktop::setMasterVolume(int masterVolume)
{
	ASSERTION_COBJECT(this);
	platformAdapter->setMasterVolume(masterVolume);
}

void Desktop::setMaximumFrameDuration(unsigned long maximumFrameDuration)
{
	ASSERTION_COBJECT(this);
	this->maximumFrameDuration = maximumFrameDuration;
}

void Desktop::setMinimumFrameDuration(unsigned long minimumFrameDuration)
{
	ASSERTION_COBJECT(this);
	this->minimumFrameDuration = minimumFrameDuration;
}

void Desktop::setMute(bool mute)
{
	ASSERTION_COBJECT(this);
	platformAdapter->setMute(mute);
}

void Desktop::setTitle(const String & title)
{
	ASSERTION_COBJECT(this);

	this->title = title;

	if (platformAdapter != NULL)
		platformAdapter->onDesktopTitleChanged();
}

void Desktop::showHourGlass()
{
	ASSERTION_COBJECT(this);

	hourGlassCount++;
	if (hourGlassCount == 1)
		updatePointerType();
}

void Desktop::showPointerAlways()
{
	ASSERTION_COBJECT(this);
	pointerMode = POINTER_SHOWN;
}

void Desktop::showPointerAutomatically()
{
	ASSERTION_COBJECT(this);
	pointerMode = POINTER_AUTOMATIC;
}

bool Desktop::showTutorial(Tutorial * tutorial)
{
	ASSERTION_COBJECT(this);

	ASSERTION(this->tutorial == NULL);
	if (this->tutorial != NULL)
	{
		// There already is a tutorial running, so it is not allowed to run another one.
		// To replace the tutorial, use cancelTutorial first.
		delete tutorial;
		return false;
	}

	this->tutorial = tutorial;

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Desktop::Listener * desktopListener
			= dynamic_cast<Desktop::Listener *>(listeners.Get(i));
		if (desktopListener != NULL)
			desktopListener->onTutorialStarted(tutorial);
	}

	tutorial->run();
	bool canceled = tutorial->isCanceled();

	cancelBalloonPopups();

	listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Desktop::Listener * desktopListener
			= dynamic_cast<Desktop::Listener *>(listeners.Get(i));
		if (desktopListener != NULL)
			desktopListener->onTutorialEnded(tutorial, canceled);
	}

	delete tutorial;
	this->tutorial = NULL;

	return !canceled;
}

void Desktop::sleep(unsigned long msecs)
{
	ASSERTION_COBJECT(this);
	platformAdapter->sleep(msecs);
}

void Desktop::sleepWithCheatProtection(unsigned long time)
{
	ASSERTION_COBJECT(this);
	platformAdapter->sleepWithCheatProtection(time);
}

String Desktop::toString()
{
	ASSERTION_COBJECT(this);
	return "Desktop";
}

void Desktop::updateModal()
{
	ASSERTION_COBJECT(this);

	int windowsCount = windows.GetCount();
	for (int i=0; i<windowsCount; i++)
	{
		ModalWindow * modalWindow = dynamic_cast<ModalWindow *>(windows.Get(i));
		if (modalWindow != NULL && !modalWindow->isDestroying())
		{
			modal = true;
			return;
		}
	}

	modal = false;
}

void Desktop::updatePointerType()
{
	ASSERTION_COBJECT(this);

	Vector pointerPosition = platformAdapter->getMouseHandler()->getPointerPosition();

	if (hourGlassCount > 0)
		pointerType = skin->getHourGlassPointerType();
	else if (draggingSurrogate != NULL && primaryPressTarget != NULL)
	{
		EventTarget * target = getEventTargetAt(pointerPosition);
		if (target != NULL && draggingSurrogate->supportsDropping(primaryPressTarget,
			target, pointerPosition - target->getOrigin()))
		{
			pointerType = draggingSurrogate->getSupportingPointerType();
			if (pointerType == NULL)
				pointerType = skin->getDefaultDraggingPointerType();
		}
		else
		{
			pointerType = draggingSurrogate->getUnsupportingPointerType();
			if (pointerType == NULL)
				pointerType = skin->getDefaultImpossiblePointerType();
		}
	}
	else
	{
		EventTarget * target = getEventTargetAt(pointerPosition);
		if (dynamic_cast<Widget *>(target))
		{
			Widget * widget = ((Widget *) target);
			PointerType * widgetType = widget->getPointerType(pointerPosition
				- widget->getParent()->getPosition());

			if (widgetType != NULL)
				pointerType = widgetType;
			else
				pointerType = skin->getSelectPointerType();
		}
		else
			pointerType = skin->getSelectPointerType();
	}
}

void Desktop::updateTimingAndWait()
{
	ASSERTION_COBJECT(this);

	#ifndef GL_DEBUG_NOSLEEP
		if (extraFrameDuration > 0 && effects.IsEmpty())
			platformAdapter->sleep(extraFrameDuration);

		if (minimumFrameDuration > 0)
		{
			unsigned long frameDuration = platformAdapter->getTime() - initTime - frameTime;
			if (frameDuration < minimumFrameDuration)
				platformAdapter->sleep(minimumFrameDuration - (long) frameDuration);
		}
	#endif

	setFrameTime(true);
}


VetoMode Desktop::Listener::onApplicationCloseReceived()
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}

void Desktop::Listener::onTutorialEnded(Tutorial * tutorial, bool canceled)
{
	ASSERTION_COBJECT(this);
}

void Desktop::Listener::onTutorialStarted(Tutorial * tutorial)
{
	ASSERTION_COBJECT(this);
}
