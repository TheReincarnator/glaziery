/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_HEADERS_H
#define __GLAZIERY_HEADERS_H


// Compiler options
#ifdef WIN32
	#pragma warning(disable: 4786)
#endif

// Toolbox
#include <ToolBox/Headers.h>
#include <Endpointvolume.h>
#include <Mmdeviceapi.h>

#ifdef D3D_OVERLOADS
	// Choosing DirectX versions
	#define DIRECTINPUT_VERSION 0x0800

	// DirectX headers, level 0
	#include <basetsd.h>

	// DirectX headers, level 1
	#include <d3d9.h>
	#include <d3d9types.h>
	#include <d3dx9core.h>
	#include <d3dx9math.h>
	#include <dinput.h>
	#include <dxdiag.h>

	// CxImage to dialog PNG images on CDialogs
	#include <CxImage/CxImage/ximapng.h>

	// CDialog resources
	#include <Glaziery/resources/Resources.h>

	// Windows includes
	#include <mmsystem.h>
	#include <shlobj.h>
#endif

// Load localized strings
#ifdef STRINGS_RESOURCE
	#include STRINGS_RESOURCE
#endif


// Color defines
#define GL_RGBA(r, g, b, a) (((unsigned long) (b)) + ((unsigned long) (g)) * 0x00000100UL \
	+ ((unsigned long) (r)) * 0x00010000UL + ((unsigned long) (a)) * 0x01000000UL)
#define GL_RGB(r, g, b) GL_RGBA(r, g, b, 0xFFUL)

#define GL_TRANSPARENCY 0x00000000UL
#define GL_BLACK 0xFF000000UL
#define GL_BLUE 0xFF0000FFUL
#define GL_GREEN 0xFF00FF00UL
#define GL_CYAN 0xFF00FFFFUL
#define GL_RED 0xFFFF0000UL
#define GL_PURPLE 0xFFFF00FFUL
#define GL_YELLOW 0xFFFFFF00UL
#define GL_GRAY 0xFF808080UL
#define GL_WHITE 0xFFFFFFFFUL

#define GL_ALPHA_OF(color) ((unsigned char) ((((unsigned long) (color)) >> 24) & 0xFFUL))
#define GL_RED_OF(color) ((unsigned char) ((((unsigned long) (color)) >> 16) & 0xFFUL))
#define GL_GREEN_OF(color) ((unsigned char) ((((unsigned long) (color)) >> 8) & 0xFFUL))
#define GL_BLUE_OF(color) ((unsigned char) (((unsigned long) (color)) & 0xFFUL))
#define GL_COLOR_OF(color) (((unsigned long) (color)) & 0x00FFFFFFUL)

#define GL_COLOR_A(color, a) (GL_COLOR_OF(color) + ((unsigned long) (a)) * 0x01000000UL)
#define GL_BLACK_A(a) GL_COLOR_A(GL_BLACK, a)
#define GL_WHITE_A(a) GL_COLOR_A(GL_WHITE, a)


// Input event defines
#define GL_KEYCODECOUNT 0x108


namespace glaziery
{
	// Common class declarations

	class ContainerLayoutData;
	class CPUProfiler;
	class DrawingContext;
	class Font;
	class FieldLayoutData;
	class GlazieryObject;
		class DraggingSurrogate;
		class Listenable;
			class EventTarget;
				class Component;
					class CachableComponent;
						class ClientArea;
							class ScrollingArea;
								class FieldsArea;
							class SingleFieldArea;
							class SplittingArea;
							class TabbingArea;
						class Popup;
							class BalloonPopup;
							class ClientAreaPopup;
							class MenuPopup;
							class SingleFieldPopup;
						class Window;
							class ModalWindow;
								class Dialog;
									class ColorDialog;
									class FileDialog;
									class InputDialog;
									class MessageDialog;
									class PasswordDialog;
									class ProgressDialog;
					class Field;
						class GroupField;
						class ImageField;
						class InputField;
							class AttachmentCheckListener;
								class AttachmentCheckBoxListener;
								class AttachmentRadioBoxListener;
							class Button;
							class CheckBox;
							class KeyStrokeField;
							class RadioBox;
							class ScrollingField;
								class ListField;
								class TextField;
								class TreeField;
							class Selector;
							class Slider;
						class Label;
						class ProgressBar;
						class Spacer;
						class TabbingField;
				class Desktop;
				class Widget;
			class FieldElement;
				class ImageElement;
				class TextElement;
			class Image;
		class Menu;
		class RadioGroup;
		class Scene;
		class SharedObject;
			class Effect;
				class EffectFork;
				class EffectSequence;
				class SaveConfigurationEffect;
				class SceneChangeEffect;
				class TimedEffect;
					class ComponentEffect;
						class CachableComponentEffect;
							class WindowEffect;
					class IdleEffect;
					class IntCycleEffect;
					class PointerEffect;
		class Tutorial;
	class InputEvent;
		class KeyEvent;
			class CharacterEvent;
			class HotKeyEvent;
			class KeyStrokeEvent;
			class SpecialEvent;
		class PointerButtonEvent;
		class PointerMoveEvent;
		class PointerWheelEvent;
	class InputManager;
	class JoystickHandler;
	class Key;
	class KeyboardHandler;
	class Layouter;
		class GridLayouter;
	class LongVector;
	class MouseHandler;
	class Mutex;
	class PlatformAdapter;
	class PointerType;
		class NoPointerType;
		class SimplePointerType;
	class Runnable;
	class Skin;
		class SimpleSkin;
	class SkinData;
	class Thread;
	class TimeCurve;
		class AccelerationTimeCurve;
		class LinearTimeCurve;
		class PolynomialTimeCurve;
	class Vector;

	// Platform dependent class declarations
	#ifdef D3D_OVERLOADS
		class DirectXFont;
		class DirectXImage;
		class DirectXJoystickHandler;
		class DirectXKey;
		class DirectXKeyboardHandler;
		class DirectXMouseHandler;
		class DirectXPlatformAdapter;
		class Win32CrashDialog;
		class Win32Mutex;
		class Win32Thread;
	#elif defined(TODO_MACOS_KEYWORD)
		class MacOSFolder;
		class MacOSFont;
		class MacOSImage;
		class MacOSJoystickHandler;
		class MacOSKey;
		class MacOSKeyboardHandler;
		class MacOSMouseHandler;
		class MacOSMutex;
		class MacOSPlatformAdapter;
		class MacOSThread;
	#else
		#error Unknown platform
	#endif
}
using namespace glaziery;


// Glaziery headers, group 1
#include <Glaziery/src/GlazieryObject.h>
#include <Glaziery/src/Effect/TimeCurve/TimeCurve.h>
#include <Glaziery/src/Input/InputAttachment.h>
#include <Glaziery/src/LongVector.h>
#include <Glaziery/src/Vector.h>

// Glaziery headers, group 2
#include <Glaziery/src/Effect/TimeCurve/AccelerationTimeCurve.h>
#include <Glaziery/src/Effect/TimeCurve/LinearTimeCurve.h>
#include <Glaziery/src/Effect/TimeCurve/PolynomialTimeCurve.h>
#include <Glaziery/src/Input/DraggingSurrogate.h>
#include <Glaziery/src/Input/InputAttachmentSet.h>
#include <Glaziery/src/Layout/Layouter.h>
#include <Glaziery/src/SharedObject.h>
#include <Glaziery/src/Skin/ScrollerInfo.h>
#include <Glaziery/src/Skin/SkinData.h>
#include <Glaziery/src/Tutorial.h>
#include <Glaziery/src/VetoMode.h>

// Glaziery headers, group 3
#include <Glaziery/src/Adapter/Font.h>
#include <Glaziery/src/Listenable.h>
	#include <Glaziery/src/Adapter/Image.h>
#include <Glaziery/src/Adapter/Mutex.h>
#include <Glaziery/src/Adapter/Thread.h>
#ifdef GL_DEBUG_CPUPROFILER
	#include <Glaziery/src/CPUProfiler.h>
#endif
#include <Glaziery/src/DrawingContext.h>
#include <Glaziery/src/Input/InputManager.h>
#include <Glaziery/src/Layout/ContainerLayoutData.h>
#include <Glaziery/src/Layout/FieldLayoutData.h>
#include <Glaziery/src/Layout/GridLayouter.h>
#include <Glaziery/src/Menu/Menu.h>
#include <Glaziery/src/Runnable.h>

// Glaziery headers, group 4
#include <Glaziery/src/Adapter/PlatformAdapter.h>
#include <Glaziery/src/Adapter/JoystickHandler.h>
#include <Glaziery/src/Adapter/KeyboardHandler.h>
#include <Glaziery/src/Adapter/MouseHandler.h>
#include <Glaziery/src/Effect/Effect.h>
	#include <Glaziery/src/Effect/TimedEffect.h>
		#include <Glaziery/src/Effect/PointerEffect.h>
#include <Glaziery/src/Field/RadioGroup.h>
#include <Glaziery/src/Input/InputEvent.h>
	#include <Glaziery/src/Input/KeyEvent.h>
		#include <Glaziery/src/Input/CharacterEvent.h>
		#include <Glaziery/src/Input/HotKeyEvent.h>
		#include <Glaziery/src/Input/KeyStrokeEvent.h>
		#include <Glaziery/src/Input/SpecialEvent.h>
	#include <Glaziery/src/Input/PointerButtonEvent.h>
	#include <Glaziery/src/Input/PointerMoveEvent.h>
	#include <Glaziery/src/Input/PointerWheelEvent.h>
#include <Glaziery/src/EventTarget.h>
	#include <Glaziery/src/Component/Component.h>
		#include <Glaziery/src/Component/CachableComponent.h>
			#include <Glaziery/src/ClientArea/ClientArea.h>
				#include <Glaziery/src/ClientArea/ScrollingArea.h>
					#include <Glaziery/src/ClientArea/FieldsArea.h>
				#include <Glaziery/src/ClientArea/SingleFieldArea.h>
				#include <Glaziery/src/ClientArea/SplittingArea.h>
				#include <Glaziery/src/ClientArea/TabbingArea.h>
			#include <Glaziery/src/Popup/Popup.h>
				#include <Glaziery/src/Popup/BalloonPopup.h>
				#include <Glaziery/src/Popup/ClientAreaPopup.h>
				#include <Glaziery/src/Menu/MenuPopup.h>
				#include <Glaziery/src/Popup/SingleFieldPopup.h>
			#include <Glaziery/src/Window/Window.h>
				#include <Glaziery/src/Window/ModalWindow.h>
	#include <Glaziery/src/Desktop.h>
	#include <Glaziery/src/Window/Widget.h>
#include <Glaziery/src/Pointer/PointerType.h>
#include <Glaziery/src/Scene.h>

// Glaziery headers, group 5
#include <Glaziery/src/Effect/DesktopEndEffect.h>
#include <Glaziery/src/Effect/EffectFork.h>
#include <Glaziery/src/Effect/EffectSequence.h>
#include <Glaziery/src/Effect/SaveConfigurationEffect.h>
#include <Glaziery/src/Effect/SceneChangeEffect.h>
	#include <Glaziery/src/Effect/ComponentEffect.h>
		#include <Glaziery/src/Effect/CachableComponentEffect.h>
			#include <Glaziery/src/Effect/WindowEffect.h>
	#include <Glaziery/src/Effect/IdleEffect.h>
	#include <Glaziery/src/Effect/IntCycleEffect.h>
#include <Glaziery/src/Field/Field.h>
	#include <Glaziery/src/Field/GroupField.h>
	#include <Glaziery/src/Field/ImageField.h>
	#include <Glaziery/src/Field/InputField.h>
		#include <Glaziery/src/Field/Button.h>
		#include <Glaziery/src/Field/CheckBox.h>
		#include <Glaziery/src/Field/KeyStrokeField.h>
		#include <Glaziery/src/Field/RadioBox.h>
		#include <Glaziery/src/Field/ScrollingField.h>
			#include <Glaziery/src/Field/ListField.h>
			#include <Glaziery/src/Field/TextField.h>
			#include <Glaziery/src/Field/TreeField.h>
		#include <Glaziery/src/Field/Selector.h>
		#include <Glaziery/src/Field/Slider.h>
	#include <Glaziery/src/Field/Label.h>
	#include <Glaziery/src/Field/ProgressBar.h>
	#include <Glaziery/src/Field/Spacer.h>
	#include <Glaziery/src/Field/TabbingField.h>
#include <Glaziery/src/Pointer/NoPointerType.h>
#include <Glaziery/src/Pointer/SimplePointerType.h>

// Glaziery headers, group 6
#include <Glaziery/src/Dialog/Dialog.h>
#include <Glaziery/src/Field/AttachmentCheckListener.h>
#include <Glaziery/src/Field/Element/FieldElement.h>
	#include <Glaziery/src/Field/Element/ImageElement.h>
	#include <Glaziery/src/Field/Element/TextElement.h>

// Glaziery headers, group 7
#include <Glaziery/src/Dialog/ColorDialog.h>
#include <Glaziery/src/Dialog/FileDialog.h>
#include <Glaziery/src/Dialog/InputDialog.h>
#include <Glaziery/src/Dialog/MessageDialog.h>
#include <Glaziery/src/Dialog/PasswordDialog.h>
#include <Glaziery/src/Dialog/ProgressDialog.h>
#include <Glaziery/src/Skin/Skin.h>
	#include <Glaziery/src/Skin/SimpleSkin/SimpleSkin.h>

// Glaziery headers, group 8
#ifdef D3D_OVERLOADS
	#include <Glaziery/src/Adapter/DirectX/DirectXFont.h>
	#include <Glaziery/src/Adapter/DirectX/DirectXImage.h>
	#include <Glaziery/src/Adapter/DirectX/DirectXJoystickHandler.h>
	#include <Glaziery/src/Adapter/DirectX/DirectXKeyboardHandler.h>
	#include <Glaziery/src/Adapter/DirectX/DirectXMouseHandler.h>
	#include <Glaziery/src/Adapter/DirectX/DirectXPlatformAdapter.h>
	#include <Glaziery/src/Adapter/DirectX/Win32CrashDialog.h>
	#include <Glaziery/src/Adapter/DirectX/Win32Mutex.h>
	#include <Glaziery/src/Adapter/DirectX/Win32Thread.h>
#elif defined(TODO_MACOS_KEYWORD)
	#include <Glaziery/src/Adapter/MacOS/MacOSFolder.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSFont.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSImage.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSJoystickHandler.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSKey.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSKeyboardHandler.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSMouseHandler.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSMutex.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSPlatformAdapter.h>
	#include <Glaziery/src/Adapter/MacOS/MacOSThread.h>
#else
	#error Unknown platform
#endif

// Glaziery inlines
#include <Glaziery/src/Adapter/PlatformAdapter.inl>
#ifdef D3D_OVERLOADS
	#include <Glaziery/src/Adapter/DirectX/DirectXPlatformAdapter.inl>
#endif
#include <Glaziery/src/DrawingContext.inl>


#endif
