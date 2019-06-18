/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_LISTENABLE_H
#define __GLAZIERY_LISTENABLE_H


namespace glaziery
{
	/**
	 * A listenable object is an object that supports registering listeners for certain events.
	 * Among others, pointer targets like components, widgets, etc, as well as images are listenable.
	 * @see EventTarget
	 * @see Component
	 * @see Window
	 * @see Image
	 */
	class Listenable : public GlazieryObject
	{
		public:

			/**
			 * The base listener class for all events of listable objects.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * event target anymore. So you should not hold any references to them.
			 */
			class Listener : public SharedObject
			{
				friend ArrayList<Listener>;

				protected:

					/**
					 * Destroys the listener.
					 * Since listeners are managed automatically,
					 * you do not need to destroy them by yourself.
					 */
					virtual ~Listener();
			};

		friend ArrayList<Listenable>;

		protected:

			/**
			 * The currently registered listeners.
			 */
			ArrayList<Listener> listeners;

			/**
			 * Destroys the listenable object.
			 */
			virtual ~Listenable();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Listenable);
			#endif

		public:

			/**
			 * Creates a new listenable object.
			 */
			Listenable();

			/**
			 * Adds a listener at the end of the listeners of this listenable object.
			 * Appended listeners are called last.
			 * @param listener The listener to be added.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * listenable object anymore. So you should not hold any references to them
			 * and you should not manually delete them.
			 */
			void appendListener(Listener * listener);

			/**
			 * Returns all currently registered listeners.
			 * Listeners are called in the order they have been appended.
			 * @return The listeners.
			 */
			const ArrayList<Listener> & getListeners();

			/**
			 * Inserts a listener at a given position in the listeners
			 * of this listenable object.
			 * @param position The 0-indexed position of the listener.
			 * @param listener The listener to be added.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * listenable object anymore. So you should not hold any references to them
			 * and you should not manually delete them.
			 */
			void insertListener(int position, Listener * listener);

			/**
			 * Adds a listener at the beginning of the listeners of this listenable object.
			 * Prepended listeners are called first.
			 * @param listener The listener to be added.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * listenable object anymore. So you should not hold any references to them
			 * and you should not manually delete them.
			 */
			void prependListener(Listener * listener);

			/**
			 * Removes a listener from the list of listeners and then deletes it,
			 * if it is no longer used.
			 * @param listener The listener to be removed and deleted.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * listenable object anymore. So you should not hold any references to them.
			 */
			void removeListener(Listener * listener);
	};
}


#endif
