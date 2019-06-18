/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SCENE_H
#define __GLAZIERY_SCENE_H


namespace glaziery
{
	/**
	 * The scene is responsible for performing application operations
	 * (such as game state updates, collision calculations,
	 * network communication, etc.), and responsible for drawing
	 * the background of the desktop (e.g. rendering the 3D scene).
	 * @author Thomas Jacob
	 */
	class Scene : public GlazieryObject, public Configurable
	{
		friend InputManager;

		private:

			/**
			 * The acceptable formats for all graphical surfaces.
			 * Glaziery tries to find a matching resolution for a first
			 * format first, and if no resolution matches, it falls back to
			 * the second format, and so on.
			 * Defaults to FORMAT_24BIT, then FORMAT_15BIT, then FORMAT_8BIT.
			 */
			ArrayList<Int> acceptableFormats;

			/**
			 * Whether the configuration has been read successfully.
			 */
			bool configured;

			/**
			 * The desired height of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 600.
			 */
			unsigned int desiredHeight;

			/**
			 * The desired refresh rate (in hertz) of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 80.
			 */
			unsigned int desiredRefreshRate;

			/**
			 * The desired width of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 800.
			 */
			unsigned int desiredWidth;

			/**
			 * Whether the application (or game) shall run in exclusive mode.
			 * All system resources are used exclusively by the application.
			 * This flag also requires the fullScreen flag to be set,
			 * which is set automatically.
			 * Defaults to false.
			 * @warning Do not use exclusive mode before the application works fine.
			 * If it hangs, you have to reboot the computer.
			 */
			bool exclusive;

			/**
			 * Whether the application (or game) shall run in full screen mode,
			 * rather than in a window. It displays the graphics output
			 * to the full screen. If exclusive is not specified, the other
			 * system resources are still shared with the other applications.
			 * Defaults to false for debug and true for release builds.
			 */
			bool fullScreen;

			/**
			 * Whether the game uses transform and lighting while
			 * processing vertexes (3D games usually do), so that hardware
			 * TnL is desirable. If the game does not use it, the framework
			 * might choose software TnL to find a better resolution for
			 * the game.
			 * Defaults to true.
			 */
			bool hardwareTnLDesirable;

			/**
			 * The minimum acceptable height of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode below this value.
			 * Defaults to 400.
			 */
			unsigned int minimumHeight;

			/**
			 * The minimum acceptable width of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode below this value.
			 * Defaults to 600.
			 */
			unsigned int minimumWidth;

			/**
			 * The maximum acceptable height of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode above this value.
			 * Defaults to 768.
			 */
			unsigned int maximumHeight;

			/**
			 * The maximum acceptable width of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode above this value.
			 * Defaults to 1024.
			 */
			unsigned int maximumWidth;

			/**
			 * The scene attachments set. Global attachments are managed by the InputManager.
			 * @see InputManager
			 */
			InputAttachmentSet sceneAttachments;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Scene);
			#endif

		public:

			/**
			 * The color format that uses 24 bits for colors (typically 8/8/8 for R/G/B),
			 * but no bits for alpha information (blending).
			 */
			static const int FORMAT_24BIT;

			/**
			 * The color format that uses 24 bits for colors (typically 8/8/8 for R/G/B),
			 * and extra bits (typically 8) for alpha information (blending).
			 */
			static const int FORMAT_24BIT_WITH_ALPHA;

			/**
			 * The color format that uses 16 bits for colors (typically 5/6/5 for R/G/B),
			 * but no bits for alpha information (blending).
			 */
			static const int FORMAT_16BIT;

			/**
			 * The color format that uses 15 bits for colors (typically 5/5/5 for R/G/B),
			 * but no bits for alpha information (blending).
			 */
			static const int FORMAT_15BIT;

			/**
			 * The color format that uses 15 bits for colors (typically 5/5/5 for R/G/B),
			 * and extra bits (typically 1) for alpha information (blending).
			 */
			static const int FORMAT_15BIT_WITH_ALPHA;

			/**
			 * The color format that uses 12 bits for colors (typically 4/4/4 for R/G/B),
			 * but no bits for alpha information (blending).
			 */
			static const int FORMAT_12BIT;

			/**
			 * The color format that uses 12 bits for colors (typically 4/4/4 for R/G/B),
			 * and extra bits (typically 4) for alpha information (blending).
			 */
			static const int FORMAT_12BIT_WITH_ALPHA;

			/**
			 * The color format that uses 8 bits for colors (typically 3/3/2 for R/G/B),
			 * but no bits for alpha information (blending).
			 */
			static const int FORMAT_8BIT;

			/**
			 * The color format that uses 8 bits for colors (typically 3/3/2 for R/G/B),
			 * and extra bits (typically 8) for alpha information (blending).
			 */
			static const int FORMAT_8BIT_WITH_ALPHA;

			/**
			 * Return value of onEnter(), issueing that the new scene continues
			 * to use the current adapter settings.
			 */
			static const int KEEP_ADAPTER;

			/**
			 * Return value of onEnter(), issueing that the platform adapter should shut down
			 * and initialize again, then using the new settings. Windows and other components
			 * are not closed automatically, but some sub-systems may have reset,
			 * depending on the implementation of the platform adapter.
			 */
			static const int RESTART_ADAPTER;

			/**
			 * Creates a new scene.
			 */
			Scene();

			/**
			 * Destroys the scene.
			 */
			virtual ~Scene();

			/**
			 * Sets, that x movements of the joystick shall increase
			 * or decrease certain event quantities.
			 * @param eventNumber The number of the event for the joystick x movement.
			 * @param factor The amount to increase the x event quantity for each x step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in this scene only.
			 * If you wish to set a global, use the InputManager's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void attachJoystickXMove(int eventNumber, long factor);

			/**
			 * Sets, that the x position of the joystick shall set certain event quantities.
			 * @param eventNumber The number of the event for the joystick x position.
			 * @param factor The multiplier for the x position. This number may be negative.
			 * @note This method sets attachments that are applied in this scene only.
			 * If you wish to set a global, use the InputManager's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void attachJoystickXPosition(int eventNumber, long factor);

			/**
			 * Sets, that y movements of the joystick shall increase
			 * or decrease certain event quantities.
			 * @param eventNumber The number of the event for the joystick y movement.
			 * @param factor The amount to increase the y event quantity for each y step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in this scene only.
			 * If you wish to set a global, use the InputManager's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void attachJoystickYMove(int eventNumber, long factor);

			/**
			 * Sets, that the y position of the joystick shall set certain event quantities.
			 * @param eventNumber The number of the event for the joystick y position.
			 * @param factor The multiplier for the y position. This number may be negative.
			 * @note This method sets attachments that are applied in this scene only.
			 * If you wish to set a global, use the InputManager's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void attachJoystickYPosition(int eventNumber, long factor);

			/**
			 * Sets, that a keyboard key shall raise a certain event if pressed, released
			 * or held. To raise an event means to increase its quantity by a certain amount.
			 * @param eventNumber The number of the event.
			 * @param keyCode The key code that raises the event. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 * @param type The type of key action to raise the event. Possible values are:
			 *
			 * <ul>
			 * <li>GFKC_HOLD: The event is raised while the key is held by factor for each
			 * milli second.</li>
			 * <li>GFKC_PRESS: The event is raised once when the key is pressed.</li>
			 * <li>GFKC_PRESSREPEAT: The event is raised when the key is pressed, and when held
			 * according to keyboard repetition (KeyboardRepeatDelay, KeyboardRepeatRate).</li>
			 * <li>GFKC_RELEASE: The event is raised once when the key is released.</li>
			 * <li>GFKC_PRESSRELEASE: The event is raised once when the key is pressed
			 * and once when released.</li>
			 * </ul>
			 *
			 * @param factor The amount to increase the event quantity in case of an event.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in this scene only.
			 * If you wish to set a global, use the InputManager's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void attachKeyOrButton(int eventNumber, int keyCode, int type, long factor);

			/**
			 * Sets, that the X movements of the mouse shall increase certain event quantities.
			 * @param eventNumber The number of the event for the mouse movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in this scene only.
			 * If you wish to set a global, use the InputManager's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void attachMouseXMove(int eventNumber, long factor);

			/**
			 * Sets, that the Y movements of the mouse shall increase certain event quantities.
			 * @param eventNumber The number of the event for the mouse movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in this scene only.
			 * If you wish to set a global, use the InputManager's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void attachMouseYMove(int eventNumber, long factor);

			/**
			 * Sets, that the mouse wheel movement shall increase a certain event quantity.
			 * @param eventNumber The number of the event for the mouse wheel movement.
			 * @param factor The amount to increase the event quantity for each step.
			 * This number may be negative.
			 * @note This method sets attachments that are applied in this scene only.
			 * If you wish to set a global, use the InputManager's attach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void attachMouseWheel(int eventNumber, long factor);

			/**
			 * Configures this configurable from a ConfigSection.
			 * Implement this method using the ConfigSection::Configure overloads.
			 * @param section The ConfigSection to be used.
			 * @param write Whether to write to the ConfigSection. false reads from it.
			 */
			virtual void Configure(ConfigSection * section, bool write);

			/**
			 * Removes all event attachments.
			 * @note This method removes attachments that are applied in this scene only.
			 * If you wish to remove a global attachment, use the InputManager's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void detachAll();

			/**
			 * Removes all attachments of an event.
			 * @param eventNumber The number of the event.
			 * @note This method removes attachments that are applied in this scene only.
			 * If you wish to remove a global attachment, use the InputManager's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void detach(int eventNumber);

			/**
			 * Removes any attachment of the joystick X axis to an event.
			 * @note This method removes attachments that are applied in this scene only.
			 * If you wish to remove a global attachment, use the InputManager's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void detachJoystickXAxis();

			/**
			 * Removes any attachment of the joystick Y axis to an event.
			 * @note This method removes attachments that are applied in this scene only.
			 * If you wish to remove a global attachment, use the InputManager's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void detachJoystickYAxis();

			/**
			 * Removes any attachment of a key to an event.
			 * @param keyCode The key code that raises the event. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 * @note This method removes attachments that are applied in this scene only.
			 * If you wish to remove a global attachment, use the InputManager's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void detachKeyOrButton(int keyCode);

			/**
			 * Removes any attachments of the mouse wheel to an event.
			 * @note This method removes attachments that are applied in this scene only.
			 * If you wish to remove a global attachment, use the InputManager's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void detachMouseWheel();

			/**
			 * Removes any attachment of the mouse X axis to an event.
			 * @note This method removes attachments that are applied in this scene only.
			 * If you wish to remove a global attachment, use the InputManager's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void detachMouseXMove();

			/**
			 * Removes any attachment of the mouse Y axis to an event.
			 * @note This method removes attachments that are applied in this scene only.
			 * If you wish to remove a global attachment, use the InputManager's detach method.
			 * Scene attachment always have a higher priority than global attachments.
			 * @see InputManager
			 */
			void detachMouseYMove();

			/**
			 * Returns the acceptable formats for all graphical surfaces.
			 * Glaziery tries to find a matching resolution for a first
			 * format first, and if no resolution matches, it falls back to
			 * the second format, and so on.
			 * Defaults to FORMAT_24BIT, then FORMAT_15BIT, then FORMAT_8BIT.
			 * @return The acceptable formats for all graphical surfaces.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			const ArrayList<Int> & getAcceptableFormats();

			/**
			 * Returns the key code of the attachment of a given event number.
			 * @param eventNumber The number of the event.
			 * @param positive Whether to search for attachments with positive factor.
			 * Defaults to true. May be combined with negative.
			 * @param negative Whether to search for attachments with positive factor.
			 * Defaults to true. May be combined with positive.
			 * @return The key code of the attachment found, or -1 if no attachment
			 * or more than one attachment was found.
			 * The key may include joystick and mouse buttons, specified by the key code constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 */
			int getAttachmentKeyOrButton(int eventNumber, bool positive = true, bool negative = true);

			/**
			 * Returns the desired height of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 0, which means to try the current OS mode.
			 * @return The desired height.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			unsigned int getDesiredHeight();

			/**
			 * Returns the desired refresh rate (in hertz) of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 0, which means to try the current OS mode.
			 * @return the desired refresh rate (in hertz).
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			unsigned int getDesiredRefreshRate();

			/**
			 * Returns the desired width of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 0, which means to try the current OS mode.
			 * @return The desired width.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			unsigned int getDesiredWidth();

			/**
			 * Returns the unique ID of this scene.
			 * This ID is used to select the scene when invoking Desktop::setScene()
			 * from any other operation, e.g. another scene's update() method.
			 * @return The unique ID of this scene.
			 * @note Implement this method by return a constant int.
			 * This value should not change during the application's run.
			 * It is reasonable to provide a constant class field for other classes.
			 * @see Desktop::setScene()
			 */
			virtual const char * getId() = NULL;

			/**
			 * Returns the scene joystick X axis attachment.
			 * @return The scene attachment.
			 */
			InputAttachment & getJoystickXAxisAttachment();

			/**
			 * Returns the scene joystick Y axis attachment.
			 * @return The scene attachment.
			 */
			InputAttachment & getJoystickYAxisAttachment();

			/**
			 * Returns the scene key or button attachment.
			 * @param keyCode The key code to return the attachment of. This may include joystick
			 * and mouse buttons, specified using the key constants
			 * (e.g. KEY_JOYSTICK_BUTTON1 or KEY_MOUSE_LEFT).
			 * @return The scene attachment.
			 */
			InputAttachment & getKeyOrButtonAttachment(int keyCode);

			/**
			 * Returns the maximum acceptable height of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode above this value.
			 * Defaults to 768.
			 * @return The maximum acceptable height.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			unsigned int getMaximumHeight();

			/**
			 * Returns the maximum acceptable width of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode above this value.
			 * Defaults to 1024.
			 * @return The maximum acceptable width.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			unsigned int getMaximumWidth();

			/**
			 * Returns the minimum acceptable height of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode below this value.
			 * Defaults to 400.
			 * @return The minimum acceptable height.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			unsigned int getMinimumHeight();

			/**
			 * Returns the minimum acceptable width of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode below this value.
			 * Defaults to 600.
			 * @return The minimum acceptable width.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			unsigned int getMinimumWidth();

			/**
			 * Returns the scene mouse wheel attachment.
			 * @return The scene attachment.
			 */
			InputAttachment & getMouseWheelAttachment();

			/**
			 * Returns the scene mouse Y axis attachment.
			 * @return The scene attachment.
			 */
			InputAttachment & getMouseXAxisAttachment();

			/**
			 * Returns the scene mouse X axis attachment.
			 * @return The scene attachment.
			 */
			InputAttachment & getMouseYAxisAttachment();

			/**
			 * Returns whether the configuration has been read successfully.
			 * @return Whether the configuration has been read successfully.
			 */
			bool isConfigured();

			/**
			 * Returns whether the application (or game) shall run in exclusive mode.
			 * All system resources are used exclusively by the application.
			 * This flag also requires the fullScreen flag to be set,
			 * which is set automatically.
			 * Defaults to false.
			 * @warning Do not use exclusive mode before the application works fine.
			 * If it hangs, you have to reboot the computer.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			bool isExclusive();

			/**
			 * Returns whether the application (or game) shall run in full screen mode,
			 * rather than in a window. It displays the graphics output
			 * to the full screen. If exclusive is not specified, the other
			 * system resources are still shared with the other applications.
			 * Defaults to false for debug and true for release builds.
			 * @return Whether the application (or game) shall run in full screen mode.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			bool isFullScreen();

			/**
			 * Returns whether the game uses transform and lighting while
			 * processing vertexes (3D games usually do), so that hardware
			 * TnL is desirable. If the game does not use it, the framework
			 * might choose software TnL to find a better resolution for
			 * the game.
			 * Defaults to true.
			 * @return Whether the game uses transform and lighting.
			 * @note This property is read only when activating a scene for the first time,
			 * unless onEnter() returns RESTART_ADAPTER.
			 */
			bool isHardwareTnLDesirable();

			/**
			 * Performs operations when the scene has been set, before render() or update()
			 * is next called. This method is called each time the scene is set.
			 * The method is called by the desktop, so do not call it manually.
			 * @return Whether to restart the platform adapter,
			 * because the new scene requires a different graphics setup.
			 * If KEEP_ADAPTER is returned, the new scene continues to use the current settings.
			 * If RESTART_ADAPTER is returned, the platform adapter is shut down and initialized
			 * again, then using the new settings. Windows and other components
			 * are not closed automatically, but some sub-systems may have reset,
			 * depending on the implementation of the platform adapter.
			 * @note Perform all each-time initialization operations here when the scene is set.
			 * The default implementation does nothing and returns KEEP_ADAPTER.
			 * @see onExit()
			 * @see Desktop::setScene()
			 */
			virtual int onEnter();

			/**
			 * Performs operations when the scene is unset, e.g. because another scene is set,
			 * or because the Desktop shuts down. In the latter case, this method is called
			 * before onShutDown.
			 * The method is called by the desktop, so do not call it manually.
			 * @note Perform all each-time shut down operations here when the scene is unset.
			 * The default implementation does nothing.
			 * @see onEnter()
			 * @see onShutDown()
			 * @see Desktop::setScene()
			 */
			virtual void onExit();

			/**
			 * Performs operations to initialize the scene on Desktop initialization.
			 * This method is called after the skin and platform adapter have been initialized,
			 * so they are already available, and it is called only once.
			 * The method is called by the desktop, so do not call it manually.
			 * @note Perform all one-time initialization operations here that require
			 * an initialized platform adapter (such as image or font retrieval).
			 * The default implementation does nothing.
			 * @see onShutDown()
			 * @see Desktop::createInstance()
			 * @see PlatformAdapter
			 * @see Skin
			 */
			virtual void onInitialize(bool firstRun);

			/**
			 * Performs operations to shut down the scene on Desktop shut down.
			 * This method is called before the skin and platform adapter are shut down,
			 * so they are still available, and it is called only once.
			 * The method is called by the desktop, so do not call it manually.
			 * @note Perform all one-time shut down operations here that require
			 * an initialized platform adapter.
			 * The default implementation does nothing.
			 * @see onInitialize(bool firstRun)
			 * @see Desktop::destroyInstance()
			 * @see PlatformAdapter
			 * @see Skin
			 */
			virtual void onShutDown();

			/**
			 * Displays the desktop background for the current frame,
			 * behind all windows and popups. The default implementation clears the device.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note Implement this method to write your main displaying
			 * routines. These routine shall draw to the Direct3D device.
			 * Glaziery displays windows on top of this background
			 * and then performs the Direct3DDevice::Present().
			 * BeginScene(), Clear(), and EndScene() is also handled
			 * by Glaziery.
			 */
			virtual void renderBackground(DrawingContext & context);

			/**
			 * Displays the desktop foreground for the current frame,
			 * in front of all windows and popups. The default implementation does nothing.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note Implement this method to write your main displaying
			 * routines. These routine shall draw to the Direct3D device.
			 * Glaziery displays windows on top of this background
			 * and then performs the Direct3DDevice::Present().
			 * BeginScene(), Clear(), and EndScene() is also handled
			 * by Glaziery.
			 */
			virtual void renderForeground(DrawingContext & context);

			/**
			 * Sets the acceptable formats for all graphical surfaces.
			 * Glaziery tries to find a matching resolution for a first
			 * format first, and if no resolution matches, it falls back to
			 * the second format, and so on.
			 * Defaults to FORMAT_24BIT, then FORMAT_15BIT, then FORMAT_8BIT.
			 * @param acceptableFormats An array of acceptable formats.
			 * @param length The length of the acceptableFormats array.
			 */
			void setAcceptableFormats(int * acceptableFormats, int length);

			/**
			 * Sets the desired height of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 600.
			 * @param desiredHeight The desired height.
			 */
			void setDesiredHeight(unsigned int desiredHeight);

			/**
			 * Sets the desired refresh rate (in hertz) of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 80.
			 * @param desiredRefreshRate the desired refresh rate (in hertz).
			 */
			void setDesiredRefreshRate(unsigned int desiredRefreshRate);

			/**
			 * Sets the desired width of the full screen resolution
			 * or the display window. The framework tries to get
			 * as close as possible to this value.
			 * Defaults to 800.
			 * @param desiredWidth The desired width.
			 */
			void setDesiredWidth(unsigned int desiredWidth);

			/**
			 * Sets whether the application (or game) shall run in exclusive mode.
			 * All system resources are used exclusively by the application.
			 * This flag also requires the fullScreen flag to be set,
			 * which is set automatically.
			 * Defaults to false.
			 * @warning Do not use exclusive mode before the application works fine.
			 * If it hangs, you have to reboot the computer.
			 * @param exclusive Whether the application shall run in exclusive mode.
			 */
			void setExclusive(bool exclusive);

			/**
			 * Sets whether the application (or game) shall run in full screen mode,
			 * rather than in a window. It displays the graphics output
			 * to the full screen. If exclusive is not specified, the other
			 * system resources are still shared with the other applications.
			 * Defaults to false for debug and true for release builds.
			 * @param fullScreen Whether the application shall run in full screen mode.
			 */
			void setFullScreen(bool fullScreen);

			/**
			 * Sets whether the game uses transform and lighting while
			 * processing vertexes (3D games usually do), so that hardware
			 * TnL is desirable. If the game does not use it, the framework
			 * might choose software TnL to find a better resolution for
			 * the game.
			 * Defaults to true.
			 * @param hardwareTnLDesirable Whether the game uses transform and lighting.
			 */
			void setHardwareTnLDesirable(bool hardwareTnLDesirable);

			/**
			 * Sets the minimum acceptable height of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode below this value.
			 * Defaults to 400.
			 * @param minimumHeight The minimum acceptable height.
			 */
			void setMinimumHeight(unsigned int minimumHeight);

			/**
			 * Sets the minimum acceptable width of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode below this value.
			 * Defaults to 600.
			 * @param minimumWidth The minimum acceptable width.
			 */
			void setMinimumWidth(unsigned int minimumWidth);

			/**
			 * Sets the maximum acceptable height of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode above this value.
			 * Defaults to 768.
			 * @param maximumHeight The maximum acceptable height.
			 */
			void setMaximumHeight(unsigned int maximumHeight);

			/**
			 * Sets the maximum acceptable width of the full screen resolution
			 * or the display window. The framework will not open a
			 * mode above this value.
			 * Defaults to 1024.
			 * @param maximumWidth The maximum acceptable width.
			 */
			void setMaximumWidth(unsigned int maximumWidth);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();

			/**
			 * Updates internal variables and performs application operations,
			 * such as game state updates, collision calculations,
			 * network communication, etc.
			 * @note Implement this method to write your main operational
			 * routines. The default implementation does nothing.
			 */
			virtual void update();
	};
}


#endif
