/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_MENU_H
#define __GLAZIERY_MENU_H


namespace glaziery
{
	class Menu : public GlazieryObject
	{
		public:

			enum CheckState
			{
				NONE, CHECK, RADIO
			};

			class Item : public Listenable
			{
				friend Menu;
				friend MenuPopup;
				friend ArrayList<Item>;

				public:

					/**
					 * @note The listener concept has been designed for two clients:
					 * The main application (e.g. the game) and the skin.
					 * Do not create listeners for internal purposes.
					 * @warning Listeners are auto-deleting when they are not listening to any
					 * component anymore. So you should not hold any references to them.
					 */
					class Listener : public Listenable::Listener
					{
						friend Item;

						private:

							int components;

						protected:

							/**
							 * Destroys the listener.
							 * Since listeners are managed automatically,
							 * you do not need to destroy them by yourself.
							 */
							virtual ~Listener();

						public:

							Listener();

							virtual void onItemSelected(Item * item);
					};

					/**
					 * The listener implementation for simple function calls for onItemSelected().
					 */
					class SelectedFunctionListener : public Listener
					{
						private:

							/**
							 * The function to be invoked, when the menu item has been selected.
							 */
							void (*function)(Item * item);

						public:

							/**
							 * Creates a new function method listener.
							 * @param function The function to be invoked, when the menu item has been selected.
							 */
							SelectedFunctionListener(void (*function)(Item * item));

							/**
							 * Called when the menu item has been selected, using the user interface.
							 * @param item The item that has been selected.
							 */
							virtual void onItemSelected(Item * item);
					};

				private:

					bool active;

					CheckState checkState;

					Menu * parent;

					bool separator;

					Menu * subMenu;

					String text;

					Item(Menu * parent, const String & text, bool separator = false);

					~Item();

				public:

					SelectedFunctionListener * addSelectedFunction(void (*function)(Item * item));

					Menu * createSubMenu();

					CheckState getCheckState();

					Component * getContainer();

					Menu * getParent();

					Menu * getSubMenu();

					const String & getText();

					bool isActive();

					bool isSeparator();

					void removeSubMenu();

					void setActive(bool active);

					void setCheckState(CheckState checkState);

					void setText(const String & text);
			};

		friend Item;

		private:

			Component * container;

			bool horizontal;

			ArrayList<Item> items;

			Menu::Item * parent;

			Menu(Item * parent);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Menu);
			#endif

		public:

			Menu(Component * container);

			~Menu();

			Item * appendItem(const String & text, void (*function)(Item * item) = NULL);

			Item * appendSeparator();

			Component * getContainer();

			const ArrayList<Item> & getItems();

			Menu::Item * getParent();

			Item * insertItem(int position, const String & text, void (*function)(Item * item) = NULL);

			Item * insertSeparator(int position);

			bool isHorizontal();

			Item * prependItem(const String & text, void (*function)(Item * item) = NULL);

			Item * prependSeparator();

			void removeItem(int position);

			void setHorizontal(bool horizontal);
	};
}


#endif
