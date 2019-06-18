/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SHAREDOBJECT_H
#define __GLAZIERY_SHAREDOBJECT_H


namespace glaziery
{
	/**
	 * A shared object knows the number of references to it.
	 * It supports reference increment and decrement operations.
	 * If no references remain, it is automatically destroying itself.
	 */
	class SharedObject : public GlazieryObject
	{
		private:

			/**
			 * The number of open references to this object. Starts with zero.
			 * If no references remain, the object is automatically destroying itself.
			 */
			int references;

		protected:

			/**
			 * Destroys the shared object. Protected, because it may not be destroyed manually.
			 */
			virtual ~SharedObject();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(SharedObject);
			#endif

		public:

			/**
			 * Creates a new shared object.
			 */
			SharedObject();

			/**
			 * Increases a reference count by one.
			 */
			void addReference();

			/**
			 * Returns the number of open references to this object. Starts with zero.
			 * If no references remain, the object is automatically destroying itself.
			 * @return The number of references.
			 */
			int getReferences();

			/**
			 * Decreases a reference count by one.
			 * If no references remain, the object is automatically destroying itself.
			 */
			void release();
	};
}


#endif
