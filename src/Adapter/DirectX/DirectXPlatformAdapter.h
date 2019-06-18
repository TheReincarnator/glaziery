/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DIRECTXPLATFORMADAPTER_H
#define __GLAZIERY_DIRECTXPLATFORMADAPTER_H


namespace glaziery
{
	class DirectXPlatformAdapter : public PlatformAdapter
	{
		protected:

			struct Point
			{
				float x, y, z;
				float rhw;
				unsigned long diffuse;
				float tu, tv;
			};

		friend DirectXFont;
		friend DirectXImage;
		friend DirectXJoystickHandler;
		friend DirectXKeyboardHandler;
		friend DirectXMouseHandler;

		protected:

			static const int CIRCLE_VERTEX_COUNT;
			static const int VERTEX_BUFFER_SIZE;

			bool alphaFullySupported;
			int applicationIcon;
			D3DFORMAT backBufferFormat;
			IDirect3DSurface9 * backBufferSurface;
			Vector backBufferSize;
			float * cosineTable;
			BlendType currentBlendType;
			DrawingContext currentContext;
			ID3DXRenderToSurface * currentRenderer;
			IDirect3DTexture9 * currentTexture;
			Vector currentTextureSize;
			Point * currentVertices;
			char decimalPoint;
			IDirect3D9 * direct3D;
			IDirect3DDevice9 * direct3DDevice;
			bool direct3DProblems;
			IDirectInput8 * directInput;
			IAudioEndpointVolume * endpointVolume;
			bool exclusive;
			bool fullScreen;
			bool inScene;
			HINSTANCE instanceHandle;
			DirectXJoystickHandler joystickHandler;
			DirectXKeyboardHandler keyboardHandler;
			HMIXER mixer;
			MIXERLINE mixerLine;
			DirectXMouseHandler mouseHandler;
			double performanceTimerFactor;
			__int64 performanceTimerStart;
			bool pow2Required;
			D3DPRESENT_PARAMETERS presentParameters;
			float * sineTable;
			bool squareRequired;
			int vertexNo;
			IDirect3DVertexBuffer9 * vertexBuffer;
			ATOM windowClassNameAtom;
			HWND windowHandle;

			inline void addTextureRectangle(Vector sourcePosition, Vector sourceSize,
				Vector targetPosition, Vector targetSize, unsigned long modulationColor);
			inline void addTextureRectangle(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
				float targetX, float targetY, float targetWidth, float targetHeight, unsigned long modulationColor);
			inline void DirectXPlatformAdapter::addTextureRectangle(Vector sourcePosition, Vector sourceSize,
				Vector targetPosition0, Vector targetPosition1, Vector targetPosition2, Vector targetPosition3,
				unsigned long modulationColor);
			inline void DirectXPlatformAdapter::addTextureRectangle(int sourceX, int sourceY,
				int sourceWidth, int sourceHeight,
				float target0X, float target0Y, float target1X, float target1Y,
				float target2X, float target2Y, float target3X, float target3Y, unsigned long modulationColor);
			Vector alignSize(Vector size);
			bool checkAndChooseMode(D3DFORMAT d3dFormat,
				unsigned int minimumWidth, unsigned int minimumHeight,
				unsigned int maximumWidth, unsigned int maximumHeight,
				unsigned int desiredWidth, unsigned int desiredHeight,
				unsigned int desiredRefreshRate);
			virtual Image * createImage(const String & path, Image::Type imageType);
			virtual Font * createFont(const String & path);
			inline void drawImageNoTextureChange(DrawingContext & context, Image * image,
				Vector sourcePosition, Vector sourceSize, Vector targetPosition,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);
			virtual void drawTextInternal(DrawingContext & context, Font * font, Vector position,
				const String & text, unsigned long modulationColor, BlendType blendType, bool caps,
				bool withHotKeys, bool ellipse);
			bool ensureScene(DrawingContext & context);
			inline bool failedObserving(HRESULT result, const char * message);
			inline bool flushVertexBuffer();
			D3DFORMAT getBackBufferFormat();
			IDirect3DSurface9 * getCurrentSurface();
			D3DFORMAT getD3DFormat(int format);
			IDirect3DDevice9 * getDirect3DDevice();
			IDirectInput8 * getDirectInput();
			static LRESULT CALLBACK getWindowProc(HWND windowHandle, UINT message,
				WPARAM wParam, LPARAM lParam);
			void handleFailure(HRESULT result, const char * message);
			virtual void initializeAudio();
			virtual void initializeBuffers();
			virtual void initializeDirect3DDevice(Scene * scene);
			virtual void initializeDirectInput();
			virtual void initializeWindow(Scene * scene, const String & applicationName,
				const String & windowTitle);
			bool prepareViewport(DrawingContext & context, D3DVIEWPORT9 & viewport);
			void processMessages();
			void recoverFromDirect3DProblems();
			void releaseBuffers();
			void releaseDirect3DDevice();
			void releaseScene();
			inline bool setTexture(Image * image, BlendType blendType = BLENDTYPE_NORMAL);
			virtual void shutDownDirectInput();
			virtual void shutDownWindow();
			void updatePresentParameters();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DirectXPlatformAdapter);
			#endif

		public:

			DirectXPlatformAdapter();
			virtual ~DirectXPlatformAdapter();
			virtual void clear(DrawingContext & context, unsigned long fillColor = GL_TRANSPARENCY);
			virtual void clearClipboard();
			virtual Mutex * createMutex();
			virtual Thread * createThread(int (*function)(Thread & thread, void * parameter),
				void * parameter);
			virtual void drawEllipse(DrawingContext & context, Vector center, Vector radius,
				unsigned long fillColor, unsigned long borderColor);
			virtual void drawImage(DrawingContext & context, Image * image,
				Vector sourcePosition, Vector sourceSize, Vector targetPosition, Vector targetSize,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);
			virtual void drawLine(DrawingContext & context, Vector from, Vector to,
				unsigned long color);
			virtual void drawPixel(DrawingContext & context, Vector position,
				unsigned long color);
			virtual void drawRect(DrawingContext & context, Vector from, Vector to,
				unsigned long fillColor, unsigned long borderColor);
			virtual void drawRotatedImage(DrawingContext & context, Image * image,
				Vector sourcePosition, Vector sourceSize, Vector targetPosition, Vector targetSize,
				Vector rotationAnchor, double rotationAngle,
				unsigned long modulationColor = GL_WHITE, BlendType blendType = BLENDTYPE_NORMAL);
			virtual void drawTriangle(DrawingContext & context, Vector * vertices,
				unsigned long fillColor, unsigned long borderColor);
			virtual unsigned long getAbsoluteTime();
			int getApplicationIcon();
			virtual unsigned long getAvailableVideoMemory();
			virtual String getClipboardText();
			String getConfigFolder(const String & manufacturer, const String & applicationName);
			virtual char getDecimalPoint();
			bool getDirectXVersion(int * major, int * minor, char * letter);
			virtual JoystickHandler * getJoystickHandler();
			virtual KeyboardHandler * getKeyboardHandler();
			virtual const char * getKeyName(int keyCode);
			virtual int getMasterVolume();
			virtual MouseHandler * getMouseHandler();
			virtual unsigned long getPixel(DrawingContext & context, Vector position);
			virtual Vector getScreenSize();
			virtual String getSystemInformation();
			virtual unsigned long getTime();
			HWND getWindowHandle();
			virtual void initialize();
			virtual bool isAlphaFullySupported();
			virtual bool isApplicationFocused();
			virtual bool isDeleteKey(int keyCode);
			virtual bool isExclusive();
			virtual bool isFullScreen();
			virtual bool isMute();
			virtual bool isPow2Required();
			virtual bool isSquareRequired();
			virtual void minimizeDesktop();
			virtual void onDesktopTitleChanged();
			virtual void openUrl(const String & url);
			virtual void restoreDesktop();
			void setApplicationIcon(int applicationIcon);
			virtual void setClipboardText(const String & text);
			virtual void setMasterVolume(int masterVolume);
			virtual void setMute(bool mute);
			virtual void showFrame();
			virtual void shutDown();
			virtual void sleep(unsigned long msecs);
			virtual void update();
	};
}


#endif
