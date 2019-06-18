/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DIRECTXIMAGE_H
#define __GLAZIERY_DIRECTXIMAGE_H


namespace glaziery
{
	class DirectXImage : public Image
	{
		friend DirectXFont;
		friend DirectXPlatformAdapter;

		private:

			bool fatalErrors;
			IDirect3DSurface9 * surface;
			ID3DXRenderToSurface * surfaceRenderer;
			IDirect3DTexture9 * texture;
			Vector textureSize;

			void destroyTexture();
			bool ensureSurfaceAndHelpers();
			bool ensureTexture();
			D3DFORMAT getFormat();
			IDirect3DSurface9 * getSurface();
			ID3DXRenderToSurface * getSurfaceRenderer();
			IDirect3DTexture9 * getTexture();
			Vector getTextureSize();

		protected:

			virtual ~DirectXImage();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DirectXImage);
			#endif

		public:

			DirectXImage(const String & path, Type imageType);
			virtual void ensureLoaded();
			virtual Vector getSize();
			virtual void resize(Vector size);
			virtual void unload();
	};
}


#endif
