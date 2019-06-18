/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SKINDATA_H
#define __GLAZIERY_SKINDATA_H


namespace glaziery
{
	/**
	 * The skin specific settings and other skin data contained in each component.
	 * The skin data is managed by the skin. If you want to implement a skin
	 * independent application (or game), do not use this object outside the skin.
	 * However, if you know which skin is in use and you require to read or write
	 * special property, you may cast this object to the respective type.
	 * This object may be NULL, if the skin does not need to store any data.
	 *
	 * @see Component
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class SkinData : public CObject
	#else
		class SkinData
	#endif
	{
		friend Component;
		friend GlazieryObject;

		private:

			/**
			 * The component this skin data belongs to or NULL,
			 * if this skin data does not belong to a component.
			 */
			Component * component;

		protected:

			/**
			 * Destroys the skin data.
			 */
			virtual ~SkinData();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(SkinData);
			#endif

		public:

			/**
			 * Creates a new skin data.
			 */
			SkinData();

			/**
			 * Returns the component this skin data belongs to or NULL,
			 * if this skin data does not belong to a component.
			 * @return The component this skin data belongs to or NULL.
			 */
			Component * getComponent();
	};
}


#endif
