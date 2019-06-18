/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DIRECTXFONT_H
#define __GLAZIERY_DIRECTXFONT_H


namespace glaziery
{
	class DirectXFont : public Font
	{
		private:

			static const int FONT_VERSION;

		protected:

			unsigned long getPixel(IDirect3DSurface9 * surface, int x, int y);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DirectXFont);
			#endif

		public:

			DirectXFont(const String & path);
			virtual ~DirectXFont();
	};
}


#endif
