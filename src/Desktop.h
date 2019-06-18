/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DESKTOP_H
#define __GLAZIERY_DESKTOP_H


namespace glaziery
{
	/**
	 * The desktop is the main class of the glaziery.
	 * It represents the background element that contains
	 * all windows and that receives and dispatches events.
	 * It is also responsible for rendering to the display surface.
	 * It is the entry class for the game or application (facade pattern).<br>
	 *
	 * To use Glaziery, the client (the game or application) should
	 * (e.g. in the application's main, WinMain, etc. method):
	 *
	 * <ul>
	 * <li>Choose and instantiate an appropriate platform adapter.
	 * The platform adapter typically results from the platform (PC, Mac, ...)
	 * and the rendering technology (DirectX, OpenGL, ...).
	 * You can also implement your own one, and you can use macros to choose
	 * the appropriate adapter in a multi-platform environment.</li>
	 * <li>Choose and instantiate an appropriate skin.
	 * The skin determines how components (windows, client areas, fields, ...)
	 * look like (colors, images, fonts, layout, etc.), but it also defines
	 * some UI related behavior, i.e. what happens if the user double-clicks
	 * the title bar of a window. Glaziery provides a reference skin, the
	 * SimpleSkin, and it encourages you to implement your own one.</li>
	 * <li>Provide at least one Scene derivative that performs the application
	 * regular operations and that renders the Desktop background (e.g. the game scene).
	 * It is also used to recursively call the game or application in modal dialogs
	 * initiated by the game or application itself.</li>
	 * <li>Initialize Glaziery using these objects.</li>
	 * <li>Call the desktop's run method. This method returns when the scene
	 * issues to end Glaziery (usually then ending the whole application).</li>
	 * <li>After run() has returned, shut down Glaziery, then the application.</li>
	 * </ul>
	 *
	 * Within the scene, perform your application's operations,
	 * and render the background surface of the Desktop.<br>
	 *
	 * Please note that Glaziery is a single-thread framework.
	 * Unless you create threads for your own, there is no concurrency,
	 * so you do not need to implement your classes in a thread-safe way.
	 * Nevertheless, Glaziery provides pseudo-concurrency by executing
	 * effects step by step in the main run loop, together with all other
	 * operations. Also, Glaziery provides modal windows with a reentrance
	 * of Glaziery's and your methods. In this case, you should implement
	 * e.g. your Scene derivatives at least reentrant-safe.<br>
	 *
	 * Since Glaziery is a framework, it has control of the single thread.
	 * The current scene and all registered listeners are called by
	 * Glaziery whenever appropriate. The main method of Glaziery, run(),
	 * does not leave until the system is about to shut down.
	 *
	 * @see PlatformAdapter
	 * @see Skin
	 * @see SimpleSkin
	 * @see Scene
	 * @see run()
	 */
	class Desktop : public EventTarget, public Configurable
	{
		private:

			/**
			 * The way the pointer is displayed.
			 * @see Desktop::hidePointer()
			 * @see Desktop::showPointerAlways()
			 * @see Desktop::showPointerAutomatically()
			 */
			enum PointerMode
			{
				/**
				 * The pointer (mouse, joystick etc.) is hidden and events are disabled.
				 * The game or application may react and process mouse etc. events by itself,
				 * e.g. passing them to the game instead of glaziery.
				 * @see pointerMode
				 */
				POINTER_HIDDEN,

				/**
				 * The pointer (mouse, joystick etc.) is always shown.
				 * @see pointerMode
				 */
				POINTER_SHOWN,

				/**
				 * The pointer (mouse, joystick etc.) is shown if any pointer event processing
				 * component exists, in terms of hasFocusWindowOrPopup() (the default).
				 * @see pointerMode
				 * @see hasFocusWindowOrPopup()
				 */
				POINTER_AUTOMATIC
			};

		public:

			/**
			 * The desktop event listener.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * event target anymore. So you should not hold any references to them.
			 */
			class Listener : public EventTarget::Listener
			{
				public:

					/**
					 * Called when the user was signalled to end the application by using OS commands (like Alt-F4 on Windows).
					 * @return Whether the desktop may end on this event. Veto if you want to perform a special handling.
					 */
					virtual VetoMode onApplicationCloseReceived();

					/**
					 * Called when a tutorial ends (or gets canceled).
					 * @param tutorial The tutorial that ended.
					 * @param canceled Whether the tutorial has been canceled (otherwise it ended regularly).
					 */
					virtual void onTutorialEnded(Tutorial * tutorial, bool canceled);

					/**
					 * Called when a new tutorial starts.
					 * @param tutorial The tutorial that started.
					 */
					virtual void onTutorialStarted(Tutorial * tutorial);
			};

		friend CharacterEvent;
		friend Component;
		friend EffectFork;
		friend EffectSequence;
		friend EventTarget;
		friend HotKeyEvent;
		friend KeyStrokeEvent;
		friend ModalWindow;
		friend PointerButtonEvent;
		friend PointerMoveEvent;
		friend PointerWheelEvent;
		friend Popup;
		friend SpecialEvent;
		friend Tutorial;
		friend Window;

		// Configurable properties
		CF_STRINGLIST_DECL(OnceActions)

		protected:

			/**
			 * The currently active scene, responsible for performing
			 * application operations (such as game state updates,
			 * collision calculations, network communication, etc.),
			 * and responsible for drawing the background of the desktop
			 * (e.g. rendering the 3D scene).
			 */
			Scene * activeScene;

			/**
			 * The user was signalled to end the application by using OS commands (like Alt-F4 on Windows).
			 */
			bool applicationCloseReceived;

			/**
			 * The technical name of the application (used e.g. to build configuration folder names).
			 */
			String applicationName;

			/**
			 * The configuration of the application (or game).
			 */
			Config config;

			/**
			 * The configuration root folder path. May be overridden with the command-line option "/config <path>".
			 */
			String configFolder;

			/**
			 * The time of the current frame since the program start in milli seconds.
			 */
			unsigned long frameTime;

			/**
			 * The mutex to synchronize on the list of deferred operations.
			 */
			Mutex * deferralMutex;

			/**
			 * The list of event targets to call executeDeferrals on in the main thread.
			 */
			ArrayList<EventTarget> deferredObjects;

			/**
			 * The list of components scheduled for deletion.
			 * Listeners are already notified about destruction.
			 * The components are added by the Component::destroy() method.
			 * @see Component::destroy()
			 */
			ArrayList<Component> disposableComponents;

			/**
			 * The list of effects scheduled for deletion.
			 * The components are added by the executeEffects() method.
			 */
			ArrayList<Effect> disposableEffects;

			/**
			 * The maximum number of milli seconds to accept two presses as a double click.
			 */
			long doubleClickDelay;

			/**
			 * The dragging surrogate to draw next to the pointer, while a drag-drop operation
			 * is running, or NULL, if no drag-drop is active.
			 */
			DraggingSurrogate * draggingSurrogate;

			/**
			 * The user has pressed and moved an event target at the dragStartDistance
			 * so that the desktop has already checked whether dragging is possible.
			 * @see dragStartDistance
			 * @see isDragging()
			 */
			bool draggingTested;

			/**
			 * The minimum number of pixels an event target has to be moved in both
			 * directions to start a dragging operation (if it supports dragging).
			 * This also serves as double click guard distance.
			 */
			int dragStartDistance;

			/**
			 * Whether the Desktop is about to end.
			 * If set to true, the run() method and all windows' runModal() methods
			 * should leave. This is typically issued by a Scene's update() method
			 * by calling end().
			 * @see end()
			 */
			bool ending;

			/**
			 * The effects managed and executed by the Desktop.
			 * Completed effects are automatically destroyed.
			 */
			ArrayList<Effect> effects;

			/**
			 * The list of pending events. The events are added by the scene
			 * using the addEvent method, and they are processed by run().
			 * @see addEvent()
			 * @see run()
			 */
			ArrayList<InputEvent> events;

			/**
          * The extra time to relief the CPU each frame, if no effects run. Defaults to 0.
			 */
			unsigned long extraFrameDuration;

			/**
			 * The window that has focus or NULL, if no window has focus.
			 */
			Window * focusWindow;

			/**
			 * The number of times showHourGlass() has been called without hideHourGlass().
			 * A value greater than 0 shows the hour glass (background operation).
			 */
			int hourGlassCount;

			/**
			 * Whether the hover operation has already been fired as event.
			 */
			bool hoverFired;

			/**
			 * The start position of a hover operation (the pointer has not moved for a while).
			 */
			Vector hoverPosition;

			/**
			 * The time when the hover operation started (the pointer has not moved for a while).
			 */
			unsigned long hoverTime;

			/**
			 * Whether the desktop is initialized already.
			 * This is used in the run() method to determine
			 * whether components have to be initalized.
			 */
			bool initialized;

			/**
			 * Absolute time at the program start in milli seconds.
			 */
			unsigned long initTime;

			/**
			 * The input manager responsible for all input devices
			 * and their event handlings during the application (or game).
			 */
			InputManager inputManager;

			/**
			 * The time, in milli-seconds, of the last draw() operation.
			 */
			unsigned long lastDrawTime;

			/**
			 * The time the last runOnce operation was performed.
			 * runOnce checks whether it has been called recently and skips too often calls.
			 */
			unsigned long lastRunOnceTime;

			/**
			 * Time before the lass call of setFrameTime().
			 */
			unsigned long lastTime;

			/**
			 * The technical name of the manufacturer (used e.g. to build configuration folder names).
			 */
			String manufacturer;

			/**
			 * The maximum time a frame shall take in milli seconds.
			 * If a frame takes longer than this time, the Timing class issues
			 * that only maximumFrameDuration milli seconds elapsed.
			 * This is just to make sure that if any real time calculation
			 * takes longer than real time, the whole thing cannot build up.
			 * Defaults to the DEFAULT_MAXIMUM_FRAME_TIME.
			 * @see DEFAULT_MAXIMUM_FRAME_TIME
			 */
			unsigned long maximumFrameDuration;

			/**
			 * The minimum time a frame shall take. After all other frame
			 * routines (such as the desktop update and rendering routines,
			 * as well as the methods of Scene) have been executed,
			 * the Timing waits so that the whole time (including the routines)
			 * of the frame is at least this amount.
			 * Use a positive value larger than the maximum time of your
			 * routines to choose the deterministic model: All frames will
			 * take that long then. Use 0 for e.g. 3D games, as they
			 * strive for maximum frame rates.
			 */
			unsigned long minimumFrameDuration;

			/**
			 * Whether any modal window is open.
			 */
			bool modal;

			/**
			 * The platform adapter configured for Glaziery.
			 * The adapter is used by the skin to render primitives.
			 * It may be set only once (on initialization).
			 * During operations, it cannot be changed.
			 * @see createInstance()
			 */
			PlatformAdapter * platformAdapter;

			/**
			 * The pointer (mouse, joystick etc.) mode, determining whether the
			 * pointer is shown (and maybe how the game or application handles pointer events).
			 * One of POINTER_HIDDEN, POINTER_SHOWN, and POINTER_AUTOMATIC.
			 * @see PointerMode
			 */
			PointerMode pointerMode;

			/**
			 * The current pointer type, defining the pointer's design.
			 */
			PointerType * pointerType;

			/**
			 * The popup windows currently registered to the desktop.
			 * Popup windows are used as context menu windows or
			 * selector choice windows. They are displayed on top of
			 * the regular windows. Within the popup windows,
			 * the last window is displayed on top.
			 * When the user causes a loss of focus (e.g. by clicking
			 * elsewhere), the popup window is automatically removed.
			 * @see windows
			 */
			ArrayList<Popup> popups;

			/**
			 * The time the onHold() method was last called on the primary press target.
			 * Valid only if primaryPressTarget is not NULL.
			 * @see primaryPressTarget
			 */
			unsigned long primaryPressLastOnHoldTime;

			/**
			 * The currently pressed event target using the primary button.
			 */
			EventTarget * primaryPressTarget;

			/**
			 * The time the event target was pressed using the primary button.
			 * Valid only if primaryPressTarget is not NULL.
			 * @see primaryPressTarget
			 */
			unsigned long primaryPressTime;

			/**
			 * The absolute position where the event target was pressed
			 * using the primary button.
			 * Valid only if primaryPressTarget is not NULL.
			 * @see primaryPressTarget
			 */
			Vector primaryPressPosition;

			#ifdef GL_DEBUG_CPUPROFILER
				/**
				 * The CPU usage times, for debugging purposes.
				 */
				CPUProfiler profiler;
			#endif

			/**
			 * The registered scenes. Add scenes during initialization of the
			 * application (or game), typically in the main method.
			 */
			StringKeyHashMap<Scene> scenes;

			/**
			 * The currently pressed event target using the secondary button.
			 */
			EventTarget * secondaryPressTarget;

			/**
			 * The desktop singleton.
			 */
			static Desktop * singleton;

			/**
			 * The skin configured for Glaziery.
			 * The skin is used to define the UI for components,
			 * including graphical representation and behavior.
			 * It may be set only once (on initialization).
			 * During operations, it cannot be changed.
			 * @see createInstance()
			 */
			Skin * skin;

			/**
			 * The current title of the entire desktop (e.g. displayed in the OS window).
			 */
			String title;

			/**
			 * The currently showing tutorial or NULL, if no tutorial is running.
			 */
			Tutorial * tutorial;

			/**
			 * Whether the desktop is currently waiting for a SPACE or ESC to continue the tutorial.
			 */
			bool waitingForTutorialStep;

			/**
			 * The regular windows currently registered to the desktop.
			 * The last window in this array is displayed on top.
			 * @note Popup windows are displayed on top the regular windows.
			 * @see popups
			 */
			ArrayList<Window> windows;

			/**
			 * Creates a new desktop.
			 * @param platformAdapter The platform adapter to be used.
			 * @param skin The skin to be used.
			 * @param manufacturer The technical name of the manufacturer
			 * (used e.g. to build configuration folder names).
			 * @param applicationName The technical name of the application
			 * (used e.g. to build configuration folder names).
			 */
			Desktop(PlatformAdapter * platformAdapter, Skin * skin,
				const String & manufacturer, const String & applicationName);

			/**
			 * Destroys the desktop.
			 */
			~Desktop();

			/**
			 * Adds a component to the list of disposableComponents scheduled for deletion.
			 * Listeners must already have been notified about destruction.
			 * The components are added by the Component::destroy() method.
			 * @param component The component to be added.
			 * @see Component::destroy()
			 */
			void addDisposable(Component * component);

			/**
			 * Adds an effect to the list of disposableComponents scheduled for release.
			 * @param effect The effect to be added.
			 */
			void addDisposable(Effect * effect);

			/**
			 * Draws the whole desktop to the display surface.
			 * @note Call the method only if the scene's rendering is not suspended.
			 * This method does not check this condition.
			 * @see Scene::isRenderingSuspended()
			 */
			void draw();

			/**
			 * Executes deferred operations.
			 */
			void executeDeferrals();

			/**
			 * Executes effects, if not currently executing modally.
			 */
			void executeEffects();

			/**
			 * Returns the position of the first stay-on-top window.
			 * If none exists, returns the number of windows.
			 * @return The position of the first stay-on-top window.
			 */
			int getOnTopWindowsBoundary();

			/**
			 * Compares a given previous focus window with the current one
			 * and sends focus change notifications.
			 * @param previousFocusWindow The previous focus window.
			 */
			void notifyPossibleFocusChange(Window * previousFocusWindow);

			/**
			 * Deletes all disposable components added by the Component::destroy() method.
			 */
			void processDisposableComponents();

			/**
			 * Deletes all disposable effects added by the executeEffects() method.
			 */
			void processDisposableEffects();

			/**
			 * Executes and deletes events issued by the platform
			 * adapter's input handlers.
			 */
			void processEvents();

			/**
			 * Forgets all references to a given event target when it dies.
			 * @param eventTarget The event target.
			 */
			void releaseReferences(EventTarget * eventTarget);

			/**
			 * Memorizes the current frame time.
			 * @param noWarnings Normally, if there is much time between
			 * two calls to setFrameTime(), a warning message is produced.
			 * If you know, that there may be a delay, or if you are in
			 * the initialization, set this parameter to true, otherwise,
			 * set it to false.
			 * @see getTimeStep()
			 */
			void setFrameTime(bool noWarnings);

			/**
			 * Updates the modal flag.
			 */
			void updateModal();

			/**
			 * Updates the pointer type.
			 */
			void updatePointerType();

			/**
			 * Updates internal variables each frame and checks whether
			 * to wait for the minimum frame time to complete.
			 */
			void updateTimingAndWait();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Desktop);
			#endif

		public:

			/**
			 * The default maximum time for a frame. This is just to make sure that,
			 * if any real time calculation takes longer than real time,
			 * the whole thing cannot build up.
			 */
			static const int DEFAULT_MAXIMUM_FRAME_TIME;

			/**
			 * Adds an effect to the Desktop.
			 * @note Some effects are added to components.
			 * Consider calling Component::cancelEffects() in this case first,
			 * if you do not want any running effect to continue and concur
			 * with your new effect.
			 * @note After the call, the memory is owned by the desktop.
			 * You should not delete it, it will be done automatically.
			 * To remove a running effect, use its cancel() method.
			 * @see Component::cancelEffects()
			 */
			void addEffect(Effect * effect);

			/**
			 * Adds an event to the event queue.
			 * Instantiate Event derivatives in the scene and add them
			 * using this method.
			 * See the Desktop's class documentation for details.
			 * @see Desktop
			 */
			void addEvent(InputEvent * event);

			/**
			 * Adds a popup window to the desktop.
			 * Popup windows are used as context menu windows or
			 * selector choice windows. They are displayed on top of
			 * the regular windows. Within the popup windows,
			 * the last window is displayed on top.
			 * When the user causes a loss of focus (e.g. by clicking
			 * elsewhere), the popup window is automatically removed.
			 * @param popup The popup window to be added.
			 * @note After the call, the memory is owned by the desktop.
			 * You should not delete it, it will be done automatically.
			 */
			void addPopup(Popup * popup);

			/**
			 * Adds a scene to the desktop. Do this during initialization
			 * of the application (or game), typically in the main method.
			 * After adding all scenes, choose one as the initial scene,
			 * using the setScene method.
			 * @param scene The scene to be added.
			 * @note After the call, the memory is owned by the desktop.
			 * You should not delete it, it will be done automatically.
			 * @see setScene()
			 */
			void addScene(Scene * scene);

			/**
			 * Adds a regular window to the desktop.
			 * The last added window is displayed on top,
			 * but this can be changed using bringToFront() and sendToBack().
			 * @param window The window to be added.
			 * @param focus Whether this window should initially get focus. Defaults to true.
			 * @note After the call, the memory is owned by the desktop.
			 * You should not delete it, it will be done automatically.
			 * @see Window::bringToFront()
			 * @see Window::sendToBack()
			 */
			void addWindow(Window * window, bool focus = true);

			/**
			 * Returns whether this method was called with the given ID before.
			 * That means, it returns false for a given ID for the first time,
			 * and then it returns true for consecutive calls.
			 * This method can be used to display messages only once, for example.
			 * @param id The ID string. This string is used to identify
			 * the action type and as the configuration parameter name.
			 * @param set Whether to set the already-done state, defaults to true.
			 * Setting it to false allows for querying the state without setting it.
			 * @return Whether the method was called with the given ID before.
			 */
			bool alreadyDone(const char * id, bool set = true);

			/**
			 * Cancels and destroys all popups from the front to the back.
			 */
			void cancelAllPopups();

			/**
			 * Cancels and destroys all balloon popups from the front to the back.
			 */
			void cancelBalloonPopups();

			/**
			 * Cancels the current drag-drop operation, if any.
			 */
			void cancelDragging();

			/**
			 * Issues to cancel any running tutorial. Ignored if no tutorial runs.
			 */
			void cancelTutorial();

			/**
			 * Clears the contents of the system's clipboard or at least the text version of it,
			 * so that getClipboardText() will return an empty string.
			 */
			void clearClipboard();

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param Section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * Section, bool write);

			/**
			 * Creates the singleton instance of the Glaziery windows system.
			 * Call this method before any other operations with Glaziery.
			 * @param platformAdapter The platform adapter configured for Glaziery.
			 * The adapter is used by the skin to render primitives.
			 * It may be set only once. During operations, it cannot be changed.
			 * @param skin The skin configured for Glaziery.
			 * The skin is used to define the UI for components,
			 * including graphical representation and behavior.
			 * It may be set only once. During operations, it cannot be changed.
			 * @param manufacturer The technical name of the manufacturer
			 * (used e.g. to build configuration folder names).
			 * @param applicationName The technical name of the application
			 * (used e.g. to build configuration folder names).
			 * @return The created desktop. This object can also be retrieved
			 * using getInstance().
			 * @note Shut down Glaziery using the destroyInstance() method.
			 * @note For more information about Glaziery's model and the way
			 * to use it, please see the class documentation.
			 * @see destroyInstance()
			 * @see getInstance()
			 * @see Desktop
			 */
			static Desktop * createInstance(PlatformAdapter * platformAdapter, Skin * skin,
				const String & manufacturer, const String & applicationName);

			/**
			 * Creates a new, concurrent thread for a given function.
			 * Concurrent threads should be used rarely, but they are required
			 * if a function call (system function or other third-party function)
			 * takes too long, resulting in the Desktop becoming inresponsive.
			 * Use this method to run the long-term function in a new thread,
			 * waiting for its completion, while executing event loop cycles in the
			 * main thread in the mean-time.
			 * @param function The function to run in the new thread.
			 * Note that this function is not allowed to perform any regular Glaziery operations
			 * (see warning note).
			 * The function should check the Thread's isCanceled() method on a regular basis
			 * and quit if it returns true. If the wait model is dialog, the user has
			 * the chance to cancel the function's operation this way, as well.
			 * @param parameter A parameter to pass to the function. Defaults to NULL.
			 * @param progressDialog Whether to create a progress dialog for the thread.
			 * Progress dialogs may show a progress and/or a message, or a generic candy bar.
			 * Setup the dialog after calling createThread(), using Thread::getProgressDialog().
			 * The dialog is actually shown when Thread::runAndWait() is called.
			 * Thread::runAndWait() shows an hour glass pointer, if no progress dialog has been created.
			 * @return The thread object created to access the new native thread and to
			 * communicate between the main thread (including Glaziery and the application)
			 * and the new native thread. The thread starts in suspended state, so you have to
			 * call Thread::runAndWait() for a blocking thread executing (recommended), or
			 * Thread::resume() to start it concurrently, which requires more experience
			 * in the thread handling with Glaziery.
			 * @warn Glaziery is not thread-safe in the most part of it. All glaziery objects
			 * (including windows, fields, etc.) may not be accessed by any other thread
			 * than the main thread. Concurrent threads created using the platform adapter's
			 * createThread method must therefore place operations on Glaziery objects into
			 * a deferral queue. Use Desktop::deferOperation() from any concurrent thread.
			 * Some classes also provide convenience methods, usually suffixed by 'Deferred'.
			 * Also note that the thread's methods are safe to use from both the main and
			 * the concurrent thread.
			 * @see Thread
			 * @see Thread::isCanceled()
			 * @see Thread::runAndWait()
			 * @see Thread::resume()
			 * @see Desktop::deferOperation()
			 */
			Thread * createThread(int (*function)(Thread & thread, void * parameter),
				bool progressDialog, void * parameter = NULL);

			/**
			 * Places a given event target in the list of objects to execute operations on
			 * in the main application thread. Glaziery is not thread-safe
			 * in the most part of it. All glaziery objects (including windows, fields,
			 * etc.) may not be accessed by any other thread than the main thread.
			 * Concurrent threads created using the platform adapter's createThread method
			 * must therefore place operations on Glaziery objects into a deferral queue.
			 * Some implementations (like Label, ProgressBar, etc.) provide convenience
			 * methods for operation deferral.
			 * @param object The event target to call executeDeferrals on.
			 * @note Use the deferObject method only from concurrent threads,
			 * and always do this within a deferral mutex lock.
			 * @see getDeferralMutex()
			 * @see executeDeferrals()
			 */
			void deferObject(EventTarget * object);

			/**
			 * Shuts down the whole Glaziery windows system and destroys the
			 * singleton instance. Call this method at the very end of your program.
			 * After calling destroyInstance(), no operations are allowed anymore,
			 * that use Glaziery in any way.
			 * But remember to call destroyInstance(), it must be called or
			 * memory leaks occur.
			 * @note For more information about Glaziery's model and the way
			 * to use it, please see the class documentation.
			 * @see createInstance()
			 * @see Desktop
			 */
			static void destroyInstance();

			/**
			 * Issues Glaziery to leave the run() method and all windows' runModal() methods.
			 * Call this method in the Scene's update() method to end the application (or game).
			 */
			void end();

			/**
			 * Returns the current time since the beginning of the ERA
			 * (since 1st of January 1970, 0:00) in seconds.
			 * @return The absolute time in seconds.
			 */
			unsigned long getAbsoluteTime();

			/**
			 * Returns the currently active scene, responsible for performing
			 * application operations (such as game state updates,
			 * collision calculations, network communication, etc.),
			 * and responsible for drawing the background of the desktop
			 * (e.g. rendering the 3D scene).
			 * @return The currently active scene or NULL, if no scene is set yet.
			 * During run, a scene is guaranteed to be set.
			 */
			Scene * getActiveScene();

			/**
			 * Returns the ID of the currently active scene, responsible for performing
			 * application operations (such as game state updates,
			 * collision calculations, network communication, etc.),
			 * and responsible for drawing the background of the desktop
			 * (e.g. rendering the 3D scene).
			 * @return The ID of the currently active scene or NULL, if no scene is set yet.
			 * During run, a scene is guaranteed to be set.
			 */
			const char * getActiveSceneId();

			/**
			 * Returns the technical name of the application (used e.g. to build configuration folder names).
			 * @return The technical name of the application.
			 */
			const String & getApplicationName();

			/**
			 * Returns a text version of the current content of the system's clipboard.
			 * @return The clipboard text.
			 */
			String getClipboardText();

			/**
			 * Returns the configuration of the application (or game).
			 * @return The configuration of the application (or game).
			 */
			Config & getConfig();

			/**
			 * Returns the path to the configuration folder for this application on the OS.
			 * This may be the application path itself, or another folder designated to configurations.
			 * @return The path to the configuration folder.
			 */
			String getConfigFolder();

			/**
			 * Returns the current time since the program start in milli seconds.
			 * @return The current time in milli seconds.
			 * @see getFrameTime()
			 */
			unsigned long getCurrentTime();

			/**
			 * Returns the mutex to synchronize on the list of deferred operations.
			 * @return The mutex to synchronize on the list of deferred operations.
			 */
			Mutex * getDeferralMutex();

			/**
			 * Returns the maximum number of milli seconds to accept two presses
			 * as a double click.
			 * @return The maximum number of double click delay milli seconds.
			 */
			long getDoubleClickDelay();

			/**
			 * The dragged object, while a drag-drop operation is running,
			 * or NULL, if no drag-drop is active.
			 * @return The dragged object or NULL.
			 */
			EventTarget * getDraggedObject();

			/**
			 * The dragging surrogate to draw next to the pointer, while a drag-drop operation
			 * is running, or NULL, if no drag-drop is active.
			 * @return The dragging surrogate or NULL.
			 */
			DraggingSurrogate * getDraggingSurrogate();

			/**
			 * Returns the minimum number of pixels an event target has to be moved in both
			 * directions to start a dragging operation (if it supports dragging).
			 * This also serves as double click guard distance.
			 * @return The minimum number of drag pixels.
			 */
			int getDragStartDistance();

			/**
			 * Returns the effects managed and executed by the Desktop.
			 * Completed effects are automatically destroyed.
			 * @return The effects managed and executed by the Desktop.
			 */
			const ArrayList<Effect> & getEffects();

			/**
			 * Returns the event target at a given position that is setup to
			 * receive events. For popup components, this is always true,
			 * for windows, this is true if and only if the event mode is
			 * active or inactive, but bypassing windows are treated as not
			 * existing (transparent for events), and for widgets, this depends on
			 * the widget's hit test (isHitAt()).
			 * @note This is a deep search: The desktop delegates to the found
			 * window or popup component, which delegates to the client area
			 * and so on.
			 * @param position The absolute position within the desktop.
			 * @return The event receiving event target at the position or
			 * <code>NULL</code>, if there is no target.
			 */
			EventTarget * getEventTargetAt(Vector position);

			/**
			 * Returns the extra time to relief the CPU each frame, if no effects run. Defaults to 0.
			 * @return The extra sleep time in milli seconds.
			 */
			unsigned long getExtraFrameDuration();

			/**
			 * Returns the deepest focus child, traversing the focus chain to the
			 * grandest child, using getFocusWindowOrPopup() and getFocusChild().
			 * @return The deepest focus child component.
			 * @see getFocusWindowOrPopup()
			 * @see Component::getFocusChild()
			 */
			Component * getFocusComponent();

			/**
			 * Returns the window set to have focus or NULL, if no window it set.
			 * @note This method does not return the window actually having focus.
			 * Popups have a higher priority on having focus, and invisible windows
			 * never have focus. See getFocusWindowOrPopup() to get this information.
			 * @return The window set to have focus or NULL.
			 * @see getFocusWindowOrPopup()
			 */
			Window * getFocusWindow();

			/**
			 * Returns the visible window or popup that currently has focus.
			 * If a visible popup exists, it is the top-most one.
			 * If no visible popup exists, the focus window is returned, if it is visible.
			 * If no visible popup and no visible focus window exists,
			 * the method returns NULL.
			 * @return The focus window, focus popup, or NULL.
			 * @see getFocusWindow()
			 */
			CachableComponent * getFocusWindowOrPopup();

			/**
			 * Returns the time of the current frame since the program start in milli seconds.
			 * @return The frame time in milli seconds.
			 * @see getCurrentTime()
			 */
			unsigned long getFrameTime();

			/**
			 * Returns the input manager responsible for all input devices
			 * and their event handlings during the application (or game).
			 * @return The input manager.
			 */
			InputManager & getInputManager();

			/**
			 * Returns the desktop singleton.
			 * @return The desktop singleton.
			 */
			static Desktop * getInstance();

			/**
			 * Returns the time, in milli-seconds, of the last draw() operation.
			 * @return The time, in milli-seconds, of the last draw() operation.
			 */
			unsigned long getLastDrawTime();

			/**
			 * Returns the last frame's time since the program start in milli seconds.
			 * @return The time in milli seconds.
			 */
			unsigned long getLastTime();

			/**
			 * Returns the technical name of the manufacturer (used e.g. to build configuration folder names).
			 * return The technical name of the manufacturer.
			 */
			const String & getManufacturer();

			/**
			 * Returns the current master volume, which ranges from 0 to 255.
			 * @return The current master volume.
			 */
			int getMasterVolume();

			/**
			 * Returns the maximum time a frame shall take in milli seconds.
			 * If a frame takes longer than this time, the Timing class issues
			 * that only maximumFrameDuration milli seconds elapsed.
			 * This is just to make sure that if any real time calculation
			 * takes longer than real time, the whole thing cannot build up.
			 * Defaults to the DEFAULT_MAXIMUM_FRAME_TIME.
			 * @return The maximum frame time in milli seconds.
			 * @see DEFAULT_MAXIMUM_FRAME_TIME
			 */
			unsigned long getMaximumFrameDuration();

			/**
			 * Returns the minimum time a frame shall take. After all other frame
			 * routines (such as InputManager::handle and Renderer::frame) have
			 * been executed, the Timing waits so that the whole time
			 * (including the routines) of the frame is at least this amount.
			 * Use a positive value larger than the maximum time of your
			 * routines to choose the deterministic model: All frames will
			 * take that long then. Use 0 for e.g. 3D games, as they
			 * strive for maximum frame rates.
			 * @returns The minimum time a frame shall take.
			 */
			unsigned long getMinimumFrameDuration();

			/**
			 * Returns the origin for relative position values.
			 * E.g. for components, this is their absolute position,
			 * and for widgets, it is the absolute position of their component.
			 */
			virtual Vector getOrigin();

			/**
			 * Returns the parent component, usually the container
			 * (e.g. the window for the client area).
			 * @return The parent component.
			 */
			virtual Component * getParent();

			/**
			 * Returns the platform adapter configured for Glaziery.
			 * The adapter is used by the skin to render primitives.
			 * It may be set only once (on initialization).
			 * During operations, it cannot be changed.
			 * @return The platform adapter.
			 * @see createInstance()
			 */
			PlatformAdapter * getPlatformAdapter();

			/**
			 * Returns the current absolute position of the pointer (mouse, joystick etc.).
			 * @return The current absolute position.
			 */
			Vector getPointerPosition();

			/**
			 * Returns the current pointer type, defining the pointer's design.
			 * @return The current pointer type.
			 */
			PointerType * getPointerType();

			/**
			 * Returns the popup windows currently registered to the desktop.
			 * Popup windows are used as context menu windows or
			 * selector choice windows. They are displayed on top of
			 * the regular windows. Within the popup windows,
			 * the last window is displayed on top.
			 * Popup windows automatically close themselves when they do not
			 * have the focus anymore and the focus is not owned by a popup
			 * window lying on top of this popup window. I.e. if no popup
			 * window has focus, all popup windows are destroyed. If a popup
			 * window gains focus, all popup windows lying on top of the popup
			 * window are destroyed, too.
			 * @return The popup windows.
			 * @see getWindows()
			 */
			const ArrayList<Popup> & getPopups();

			#ifdef GL_DEBUG_CPUPROFILER
				/**
				 * Returns the CPU usage times, for debugging purposes.
				 * @return The CPU usage times, for debugging purposes.
				 */
				CPUProfiler & getProfiler();
			#endif

			/**
			 * Returns a scene by its ID.
			 * @param id The ID to return the scene for.
			 * @return The scene or NULL, if no scene can be found for this ID.
			 */
			Scene * getScene(const char * id);

			/**
			 * Returns all registered scenes as a map of scene IDs to scenes.
			 * @return All registered scenes.
			 */
			const StringKeyHashMap<Scene> & getScenes();

			/**
			 * Returns the desktop's size (the screen size).
			 * @return The desktop's size (the screen size).
			 */
			Vector getSize();

			/**
			 * Returns the skin configured for Glaziery.
			 * The skin is used to define the UI for components,
			 * including graphical representation and behavior.
			 * It may be set only once (on initialization).
			 * During operations, it cannot be changed.
			 * @return The skin.
			 * @see createInstance()
			 */
			Skin * getSkin();

			/**
			 * Returns a modulo of the time since the program start in milli seconds.
			 * @param modulo The modulo divisor.
			 * @return The modulo of the time since the program start.
			 */
			unsigned long getTimeModulo(unsigned long modulo);

			/**
			 * Returns the time since the program start
			 * in steps of a certain amount of milli seconds.
			 * @param milliSecs The time scale step in milliSecs.
			 * @return The steps of the time since the program start.
			 */
			unsigned long getTimeScaled(unsigned long milliSecs);

			/**
			 * Returns a modulo of steps of a certain amount
			 * of milli seconds of the time since the program start.
			 * @param milliSecs The time scale step in milliSecs.
			 * @param modulo The modulo divisor.
			 * @return The moduloed steps of the time since the program start.
			 */
			unsigned long getTimeScaledModulo(unsigned long milliSecs, unsigned long modulo);

			/**
			 * Returns the amount of milli seconds
			 * between the last two calls to setFrameTime().
			 * @return The amount of milli seconds.
			 */
			unsigned long getTimeStep();

			/**
			 * Returns the amount of milli seconds between
			 * the last two calls to setFrameTime() in steps of a certain size.
			 * @param milliSecs The time scale step in milliSecs.
			 * @return The scaled amount of milli seconds.
			 */
			unsigned long getTimeStepScaled(unsigned long milliSecs);

			/**
			 * Returns the current title of the entire desktop (e.g. displayed in the OS window).
			 * @return The current title of the entire desktop.
			 */
			const String & getTitle();

			/**
			 * Returns the currently showing tutorial or NULL, if no tutorial runs.
			 * @return The currently showing tutorial or NULL.
			 * @see showTutorial()
			 * @see Tutorial
			 */
			Tutorial * getTutorial();

			/**
			 * Returns the regular windows currently registered to the desktop.
			 * The last window in this array is displayed on top.
			 * @note Popup windows are displayed on top the regular windows.
			 * @return The regular windows.
			 * @see popups
			 */
			const ArrayList<Window> & getWindows();

			/**
			 * Returns whether a focus window or popup exists.
			 * If a popup exists, this is true.
			 * If no popup exists, it is true if and only if a window
			 * with event mode active or inactive exists.
			 * If no popup and no active and no inactive window exists,
			 * the method returns false. Among others, if only the desktop
			 * would receive an event, the method returns false, too.
			 * @return Whether a focus window or popup exists.
			 */
			bool hasFocusWindowOrPopup();

			/**
			 * Returns whether a popup exists.
			 * @return Whether a popup exists.
			 */
			bool hasPopup();

			/**
			 * Sets not to show the hour glass pointer anymore.
			 * Calling showHourGlass multiple times requires hideHourGlass multiple times, as well.
			 * @see showHourGlass()
			 * @see isShowingHourGlass()
			 */
			void hideHourGlass();

			/**
			 * Sets that the pointer (mouse, joystick etc.) is hidden and events are disabled.
			 * The game or application may react and process mouse etc. events by itself,
			 * e.g. passing them to the game instead of glaziery.
			 * @see showPointerAlways()
			 * @see showPointerAutomatically()
			 */
			void hidePointer();

			/**
			 * Initializes the Glaziery system components.
			 * Called from the run() method, if Glaziery is not initialized yet.
			 * You may call it manually to have more control on the initialization,
			 * but you don't need to.
			 * @see run()
			 */
			void initialize();

			/**
			 * Returns whether the application is currently focused.
			 * @return Whether the application is currently focused.
			 */
			bool isApplicationFocused();

			/**
			 * Returns whether the user is currently dragging an event target using the pointer.
			 * @return Whether the user is currently dragging.
			 */
			bool isDragging();

			/**
			 * Returns whether the desktop is about to end.
			 * If set to true, the run() method and all windows' runModal() methods
			 * should leave. This is typically issued by a Scene's update() method
			 * by calling end().
			 * @return Whether the Desktop is about to end.
			 * @see end()
			 */
			bool isEnding();

			/**
			 * Returns whether the application (or game) is run in exclusive mode.
			 * All system resources are used exclusively by the application.
			 * This is controlled by the last scene that issued an adapter restart.
			 * @return Whether the application is in exclusive mode.
			 * @see Scene::isExclusive()
			 */
			bool isExclusive();

			/**
			 * Returns whether the desktop is full screen.
			 * This is controlled by the last scene that issued an adapter restart.
			 * @return Whether the desktop is full screen.
			 * @see Scene::isFullScreen()
			 */
			bool isFullScreen();

			/**
			 * Returns whether the desktop is initialized already.
			 * This is used in the run() method to determine
			 * whether components have to be initalized.
			 * @return Whether the desktop is initialized already.
			 */
			bool isInitialized();

			/**
			 * Returns whether the desktop is in key stroke mode.
			 * Key strokes are usually received by key stroke fields.
			 * When the desktop is in key stroke mode, the next key pressed
			 * should be sent to the onKeyStroke() method.
			 * This is useful e.g. to define keyboard layouts for games.
			 * @return Whether the desktop is in key stroke mode.
			 * @see onKeyStroke()
			 */
			bool isKeyStrokeMode();

			/**
			 * Returns whether any modal window is open.
			 * @return Whether any modal window is open.
			 */
			bool isModal();

			/**
			 * Returns whether the speakers are mute (which is independent from the master volume).
			 * @return Whether the speakers are mute.
			 */
			bool isMute();

			/**
			 * Returns true if the pointer (mouse, joystick etc.) is always shown,
			 * as set by showPointer(). This does not include automatic showing.
			 * @return Whether the pointer is always shown.
			 * @see showPointer()
			 * @see isPointerCurrentlyShown
			 */
			bool isPointerAlwaysShown();

			/**
			 * Returns true if the pointer (mouse, joystick etc.) is in automatic mode
			 * (the default). The pointer is shown if any event processing
			 * component exists, in terms of hasFocusWindowOrPopup().
			 * @return Whether the pointer is automatic.
			 * @see showPointer()
			 * @see showPointerAutomatically()
			 * @see hasFocusWindowOrPopup()
			 */
			bool isPointerAutomatic();

			/**
			 * Returns whether the pointer (mouse, joystick etc.) is currently
			 * shown. This is true if the mode is always shown or it is automatic
			 * and hasPointerReceivingComponent() returns true.
			 * @return Whether the pointer is currently shown.
			 * @see isPointerShown()
			 * @see showPointer()
			 * @see showPointerAutomatically()
			 * @see hasPointerReceivingComponent()
			 */
			bool isPointerCurrentlyShown();

			/**
			 * Returns true if the pointer (mouse, joystick etc.) is hidden,
			 * as set by hidePointer(). This does not include automatic hiding.
			 * The game or application may react and process mouse etc. events by itself then,
			 * e.g. passing them to the game instead of glaziery.
			 * @return Whether the pointer is hidden.
			 * @see hidePointer()
			 * @see isPointerCurrentlyShown
			 */
			bool isPointerHidden();

			/**
			 * Returns whether the Desktop is currently showing the hour glass.
			 * @return Whether the pointer is currently showing the hour glass.
			 * @see showHourGlass()
			 * @see hideHourGlass()
			 */
			bool isShowingHourGlass();

			/**
			 * Returns whether the Desktop is currently in tutorial mode.
			 * @return Whether the Desktop is currently in tutorial mode.
			 * @see showTutorial()
			 * @see Tutorial
			 */
			bool isTutorialMode();

			/**
			 * Minimizes the entire application.
			 */
			void minimize();

			/**
			 * Positions the position of the pointer (mouse, joystick etc.) absolutely.
			 * You do not need to ensure that the position stays within the screen,
			 * this method does that for you.
			 * @param position The new absolute position for the pointer.
			 */
			void movePointer(Vector position);

			/**
			 * Tells the Desktop that the user was signalled to end the application
			 * by using OS commands (like Alt-F4 on Windows).
			 */
			void onApplicationCloseReceived();

			/**
			 * Continues the currently running waitForTutorialStep() when SPACE or ESC is pressed.
			 * @param cancel Whether to cancel.
			 */
			void onTutorialStep(bool cancel);

			/**
			 * Opens a given URL in a web browser.
			 * @param url The URL to open.
			 */
			void openUrl(const String & url);

			/**
			 * Restores the entire application.
			 */
			void restore();

			/**
			 * Runs the application (or game). This is the main entry point for
			 * Glaziery. Call this method from the program's main method
			 * (e.g. <code>main</code> or <code>WinMain</code>), after having
			 * initialized it. The method returns when Glaziery should leave.
			 * Call destroyInstance() then to shut Glaziery down again.
			 * @note For more information about Glaziery's model and the way
			 * to use it, please see the class documentation.
			 * @see createInstance()
			 * @see destroyInstance()
			 * @see Desktop
			 */
			void run();

			/**
			 * Performs one frame of the application (or game). If you implement
			 * time-consuming operations, you should call this method regularly
			 * (multiple times within a second), so that Glaziery still responds
			 * to user input, it still renders your scene and windows, and your
			 * application (or game) continues to run.
			 * @param minimumTime The minimum time (milli seconds) since the last call to runOnce.
			 * If less time has passed, the method does nothing. So it is safe to call often.
			 * Defaults to 0, i.e. always perform one frame.
			 * @warn runOnce() may call the code again from where it has been
			 * called, since regular event queue handling takes place.
			 * Beware of this fact when using this method. Maybe you require
			 * guard flags to prevent duplicate (inner) execution of your code.
			 * @note Window::runModally() uses this method to continue Glaziery
			 * while blocking until the user has completed the window.
			 * @see Desktop
			 * @see Window::runModally()
			 */
			void runOnce(unsigned long minimumTime = 0);

			/**
			 * Saves the configuration to hard disk while the application is running.
			 * @note You do not need to call this method to save the configuration
			 * at the end of the application run. This is done automatically.
			 */
			void saveConfig();

			/**
			 * Sets the currently active scene, responsible for performing
			 * application operations (such as game state updates,
			 * collision calculations, network communication, etc.),
			 * and responsible for drawing the background of the desktop
			 * (e.g. rendering the 3D scene).
			 * @param id The ID of the new scene to be set.
			 * @see Scene::onEnter()
			 * @see Scene::onExit()
			 */
			void setActiveScene(const char * id);

			/**
			 * Sets the currently active scene, responsible for performing
			 * application operations (such as game state updates,
			 * collision calculations, network communication, etc.),
			 * and responsible for drawing the background of the desktop
			 * (e.g. rendering the 3D scene).
			 * @param scene The new scene to be set.
			 * @see Scene::onEnter()
			 * @see Scene::onExit()
			 */
			void setActiveScene(Scene * scene);

			/**
			 * Sets a given text into system's clipboard.
			 * @param text The new clipboard text.
			 */
			void setClipboardText(const String & text);

			/**
			 * Sets the maximum number of milli seconds to accept two presses
			 * as a double click.
			 * @param doubleClickDelay The maximum number of double click delay milli seconds.
			 */
			void setDoubleClickDelay(long doubleClickDelay);

			/**
			 * Sets the minimum number of pixels an event target has to be moved in both
			 * directions to start a dragging operation (if it supports dragging).
			 * This also serves as double click guard distance.
			 * @param dragStartDistance The minimum number of drag pixels.
			 */
			void setDragStartDistance(int dragStartDistance);

			/**
			 * Sets an extra time to relief the CPU each frame, if no effects run. Defaults to 0.
			 * @param extraFrameDuration The extra sleep time in milli seconds.
			 */
			void setExtraFrameDuration(unsigned long extraFrameDuration);

			/**
			 * Sets the window that has focus.
			 * @param focusWindow The window that has focus.
			 * @note This method does not check whether the window is visible and active.
			 */
			void setFocusWindow(Window * focusWindow);

			/**
			 * Sets the current master volume, which ranges from 0 to 255.
			 * @param volume The current master volume.
			 */
			void setMasterVolume(int masterVolume);

			/**
			 * Sets the maximum time a frame shall take in milli seconds.
			 * If a frame takes longer than this time, the Timing class issues
			 * that only maximumFrameDuration milli seconds elapsed.
			 * This is just to make sure that if any real time calculation
			 * takes longer than real time, the whole thing cannot build up.
			 * Defaults to the DEFAULT_MAXIMUM_FRAME_TIME.
			 * @param maximumFrameDuration The maximum frame time in milli seconds.
			 * @see DEFAULT_MAXIMUM_FRAME_TIME
			 */
			void setMaximumFrameDuration(unsigned long maximumFrameDuration);

			/**
			 * Sets the minimum time a frame shall take. After all other frame
			 * routines (such as InputManager::handle and Renderer::frame) have
			 * been executed, the Timing waits so that the whole time
			 * (including the routines) of the frame is at least this amount.
			 * Use a positive value larger than the maximum time of your
			 * routines to choose the deterministic model: All frames will
			 * take that long then. Use 0 for e.g. 3D games, as they
			 * strive for maximum frame rates.
			 * @param minimumFrameDuration The minimum time a frame shall take.
			 */
			void setMinimumFrameDuration(unsigned long minimumFrameDuration);

			/**
			 * Sets whether the speakers are mute (which is independent from the master volume).
			 * @param mute Whether the speakers are mute.
			 */
			void setMute(bool mute);

			/**
			 * Sets the current title of the entire desktop (e.g. displayed in the OS window).
			 * @param title The current title of the entire desktop.
			 */
			void setTitle(const String & title);

			/**
			 * Sets to show the hour glass pointer due to a background operation.
			 * Does not prevent the user from clicking etc., you must take care of
			 * unwanted recursion or operations while the background operation is running.
			 * hideHourGlass shows the regular pointer again. Calling showHourGlass multiple
			 * times requires hideHourGlass multiple times, as well.
			 * @see hideHourGlass()
			 * @see isShowingHourGlass()
			 */
			void showHourGlass();

			/**
			 * Sets that the pointer (mouse, joystick etc.) is always shown.
			 * @see hidePointer()
			 * @see showPointerAutomatically()
			 */
			void showPointerAlways();

			/**
			 * Sets that the pointer (mouse, joystick etc.) is shown if any event
			 * processing component exists, in terms of
			 * hasFocusWindowOrPopup() (the default).
			 * @see hasFocusWindowOrPopup()
			 * @see hidePointer()
			 * @see showPointerAlways()
			 */
			void showPointerAutomatically();

			/**
			 * Starts a given tutorial, disabling regular pointer and keyboard input in the meantime.
			 * Tutorials help the user to understand the front-end by showing use-cases step-by-step.
			 * Tutorial use pointer and other effects to visualize the use, and pause after a step,
			 * prompting the user to press SPACE to continue. The user can stop the tutorial using ESC.
			 * @param tutorial The tutorial to be displayed by calling the run method, having the desktop
			 * in a special tutorial mode.
			 * @return Whether the tutorial ended regularly. false if canceled.
			 * @warn This method expects that the given tutorial is newly created,
			 * and it destroys the tutorial when its finished or canceled.
			 */
			bool showTutorial(Tutorial * tutorial);

			/**
			 * Suspends the current thread for a given number of milli seconds.
			 * @param msecs The number of milli seconds to wait.
			 */
			void sleep(unsigned long msecs);

			/**
			 * Suspends the current thread for a given number of milli seconds,
			 * splitting the time into slices to prevent cheating by shifting the clock.
			 * On the other hand, this is a little less accurate.
			 * @param time The number of milli seconds to wait.
			 */
			void sleepWithCheatProtection(unsigned long time);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
