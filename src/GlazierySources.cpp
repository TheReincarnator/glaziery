/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>

// Source files
#ifdef D3D_OVERLOADS
	#include <Glaziery/src/Adapter/DirectX/DirectXFont.cpp>
	#include <Glaziery/src/Adapter/DirectX/DirectXImage.cpp>
	#include <Glaziery/src/Adapter/DirectX/DirectXJoystickHandler.cpp>
	#include <Glaziery/src/Adapter/DirectX/DirectXKeyboardHandler.cpp>
	#include <Glaziery/src/Adapter/DirectX/DirectXMouseHandler.cpp>
	#include <Glaziery/src/Adapter/DirectX/DirectXPlatformAdapter.cpp>
	#include <Glaziery/src/Adapter/DirectX/Win32CrashDialog.cpp>
	#include <Glaziery/src/Adapter/DirectX/Win32Mutex.cpp>
	#include <Glaziery/src/Adapter/DirectX/Win32Thread.cpp>
#endif
#include <Glaziery/src/Adapter/Font.cpp>
#include <Glaziery/src/Adapter/Image.cpp>
#include <Glaziery/src/Adapter/JoystickHandler.cpp>
#include <Glaziery/src/Adapter/KeyboardHandler.cpp>
#ifdef TODO_MACOS_KEYWORD
	#include <Glaziery/src/Adapter/MacOS/MacOSFolder.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSFont.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSImage.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSJoystickHandler.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSKey.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSKeyboardHandler.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSMouseHandler.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSMutex.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSPlatformAdapter.cpp>
	#include <Glaziery/src/Adapter/MacOS/MacOSThread.cpp>
#endif
#include <Glaziery/src/Adapter/MouseHandler.cpp>
#include <Glaziery/src/Adapter/Mutex.cpp>
#include <Glaziery/src/Adapter/PlatformAdapter.cpp>
#include <Glaziery/src/Adapter/Thread.cpp>
#include <Glaziery/src/ClientArea/ClientArea.cpp>
#include <Glaziery/src/ClientArea/FieldsArea.cpp>
#include <Glaziery/src/ClientArea/ScrollingArea.cpp>
#include <Glaziery/src/ClientArea/SingleFieldArea.cpp>
#include <Glaziery/src/ClientArea/SplittingArea.cpp>
#include <Glaziery/src/ClientArea/TabbingArea.cpp>
#include <Glaziery/src/Component/CachableComponent.cpp>
#include <Glaziery/src/Component/Component.cpp>
#ifdef GL_DEBUG_CPUPROFILER
	#include <Glaziery/src/CPUProfiler.cpp>
#endif
#include <Glaziery/src/Desktop.cpp>
#include <Glaziery/src/Dialog/ColorDialog.cpp>
#include <Glaziery/src/Dialog/Dialog.cpp>
#include <Glaziery/src/Dialog/FileDialog.cpp>
#include <Glaziery/src/Dialog/InputDialog.cpp>
#include <Glaziery/src/Dialog/MessageDialog.cpp>
#include <Glaziery/src/Dialog/PasswordDialog.cpp>
#include <Glaziery/src/Dialog/ProgressDialog.cpp>
#include <Glaziery/src/DrawingContext.cpp>
#include <Glaziery/src/Effect/CachableComponentEffect.cpp>
#include <Glaziery/src/Effect/ComponentEffect.cpp>
#include <Glaziery/src/Effect/DesktopEndEffect.cpp>
#include <Glaziery/src/Effect/Effect.cpp>
#include <Glaziery/src/Effect/EffectFork.cpp>
#include <Glaziery/src/Effect/EffectSequence.cpp>
#include <Glaziery/src/Effect/IdleEffect.cpp>
#include <Glaziery/src/Effect/IntCycleEffect.cpp>
#include <Glaziery/src/Effect/PointerEffect.cpp>
#include <Glaziery/src/Effect/SaveConfigurationEffect.cpp>
#include <Glaziery/src/Effect/SceneChangeEffect.cpp>
#include <Glaziery/src/Effect/TimeCurve/AccelerationTimeCurve.cpp>
#include <Glaziery/src/Effect/TimeCurve/LinearTimeCurve.cpp>
#include <Glaziery/src/Effect/TimeCurve/PolynomialTimeCurve.cpp>
#include <Glaziery/src/Effect/TimeCurve/TimeCurve.cpp>
#include <Glaziery/src/Effect/TimedEffect.cpp>
#include <Glaziery/src/Effect/WindowEffect.cpp>
#include <Glaziery/src/EventTarget.cpp>
#include <Glaziery/src/Field/AttachmentCheckListener.cpp>
#include <Glaziery/src/Field/Button.cpp>
#include <Glaziery/src/Field/CheckBox.cpp>
#include <Glaziery/src/Field/Element/FieldElement.cpp>
#include <Glaziery/src/Field/Element/ImageElement.cpp>
#include <Glaziery/src/Field/Element/TextElement.cpp>
#include <Glaziery/src/Field/Field.cpp>
#include <Glaziery/src/Field/GroupField.cpp>
#include <Glaziery/src/Field/ImageField.cpp>
#include <Glaziery/src/Field/InputField.cpp>
#include <Glaziery/src/Field/KeyStrokeField.cpp>
#include <Glaziery/src/Field/Label.cpp>
#include <Glaziery/src/Field/ListField.cpp>
#include <Glaziery/src/Field/ProgressBar.cpp>
#include <Glaziery/src/Field/RadioBox.cpp>
#include <Glaziery/src/Field/RadioGroup.cpp>
#include <Glaziery/src/Field/ScrollingField.cpp>
#include <Glaziery/src/Field/Selector.cpp>
#include <Glaziery/src/Field/Slider.cpp>
#include <Glaziery/src/Field/Spacer.cpp>
#include <Glaziery/src/Field/TabbingField.cpp>
#include <Glaziery/src/Field/TextField.cpp>
#include <Glaziery/src/Field/TreeField.cpp>
#include <Glaziery/src/GlazieryObject.cpp>
#include <Glaziery/src/Input/CharacterEvent.cpp>
#include <Glaziery/src/Input/DraggingSurrogate.cpp>
#include <Glaziery/src/Input/HotKeyEvent.cpp>
#include <Glaziery/src/Input/InputAttachment.cpp>
#include <Glaziery/src/Input/InputAttachmentSet.cpp>
#include <Glaziery/src/Input/InputEvent.cpp>
#include <Glaziery/src/Input/InputManager.cpp>
#include <Glaziery/src/Input/KeyEvent.cpp>
#include <Glaziery/src/Input/KeyStrokeEvent.cpp>
#include <Glaziery/src/Input/PointerButtonEvent.cpp>
#include <Glaziery/src/Input/PointerMoveEvent.cpp>
#include <Glaziery/src/Input/PointerWheelEvent.cpp>
#include <Glaziery/src/Input/SpecialEvent.cpp>
#include <Glaziery/src/Menu/Menu.cpp>
#include <Glaziery/src/Menu/MenuPopup.cpp>
#include <Glaziery/src/Layout/ContainerLayoutData.cpp>
#include <Glaziery/src/Layout/FieldLayoutData.cpp>
#include <Glaziery/src/Layout/GridLayouter.cpp>
#include <Glaziery/src/Layout/Layouter.cpp>
#include <Glaziery/src/Listenable.cpp>
#include <Glaziery/src/LongVector.cpp>
#include <Glaziery/src/Pointer/NoPointerType.cpp>
#include <Glaziery/src/Pointer/PointerType.cpp>
#include <Glaziery/src/Pointer/SimplePointerType.cpp>
#include <Glaziery/src/Popup/BalloonPopup.cpp>
#include <Glaziery/src/Popup/ClientAreaPopup.cpp>
#include <Glaziery/src/Popup/Popup.cpp>
#include <Glaziery/src/Popup/SingleFieldPopup.cpp>
#include <Glaziery/src/Runnable.cpp>
#include <Glaziery/src/Scene.cpp>
#include <Glaziery/src/SharedObject.cpp>
#include <Glaziery/src/Skin/ScrollerInfo.cpp>
#include <Glaziery/src/Skin/SimpleSkin/SimpleSkin.cpp>
#include <Glaziery/src/Skin/Skin.cpp>
#include <Glaziery/src/Skin/SkinData.cpp>
#include <Glaziery/src/Tutorial.cpp>
#include <Glaziery/src/Vector.cpp>
#include <Glaziery/src/Window/Widget.cpp>
#include <Glaziery/src/Window/ModalWindow.cpp>
#include <Glaziery/src/Window/Window.cpp>
