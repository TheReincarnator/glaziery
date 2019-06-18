/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DIRECTXMOUSEINPUTHANDLER_H
#define __GLAZIERY_DIRECTXMOUSEINPUTHANDLER_H


namespace glaziery
{
	class DirectXMouseHandler : public MouseHandler
	{
		protected:

			static const int BUFFER_SIZE;

			bool mouseAcquired;
			IDirectInputDevice8 * mouseDevice;
			Vector pointerPosition;
			long wheelDeltaFractions;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(DirectXMouseHandler);
			#endif

		public:

			DirectXMouseHandler();
			virtual Vector getPointerPosition();
			virtual void initialize();
			void setPointerPosition(Vector position);
			virtual void shutDown();
			virtual void update();
			virtual void updateExclusive();
	};
}


#endif
