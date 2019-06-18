/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


MenuPopup::MenuPopup(Menu * menu, bool destroyMenu)
{
	ASSERTION_COBJECT(this);

	this->menu = menu;
	this->destroyMenu = destroyMenu;

	Desktop::getInstance()->getSkin()->setupMenuPopup(this);
	resizeToMenu();
}

MenuPopup::~MenuPopup()
{
	ASSERTION_COBJECT(this);
}

void MenuPopup::destroy()
{
	if (destroyMenu && menu != NULL)
	{
		delete menu;
		menu = NULL;
	}

	Popup::destroy();
}

void MenuPopup::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawMenuPopup(context, this);
}

Menu * MenuPopup::getMenu()
{
	ASSERTION_COBJECT(this);
	return menu;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(MenuPopup, Popup);
#endif

void MenuPopup::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Popup::onClick(position, option1, option2);

	Skin * skin = Desktop::getInstance()->getSkin();
	Vector topLeftSize;
	Vector bottomRightSize;
	skin->getMenuPopupBorderSizes(this, topLeftSize, bottomRightSize);
	if (!(position >= topLeftSize) || !(position < getSize() - bottomRightSize))
		return;

	int rest = menu->isHorizontal() ? position.x - topLeftSize.x : position.y - topLeftSize.y;
	const ArrayList<Menu::Item> & items = menu->getItems();
	for (int i=0; i<items.GetCount(); i++)
	{
		Menu::Item * item = items.Get(i);

		Vector itemSize = skin->getMenuItemSize(item);
		if (menu->isHorizontal())
			rest -= itemSize.x;
		else
			rest -= itemSize.y;

		if (rest < 0)
		{
			if (!item->isActive() || item->isSeparator())
				return;

			cancel();

			const ArrayList<Listenable::Listener> & itemListeners = item->getListeners();
			int listenersCount = itemListeners.GetCount();
			for (int i=0; i<listenersCount; i++)
			{
				Menu::Item::Listener * menuItemListener
					= dynamic_cast<Menu::Item::Listener *>(itemListeners.Get(i));
				if (menuItemListener != NULL)
					menuItemListener->onItemSelected(item);
			}

			return;
		}
	}
}

void MenuPopup::resizeToMenu()
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();

	Vector size;
	const ArrayList<Menu::Item> & items = menu->getItems();
	for (int i=0; i<items.GetCount(); i++)
	{
		Vector itemSize = skin->getMenuItemSize(items.Get(i));
		if (menu->isHorizontal())
		{
			size.x += itemSize.x;
			if (size.y < itemSize.y)
				size.y = itemSize.y;
		}
		else
		{
			size.y += itemSize.y;
			if (size.x < itemSize.x)
				size.x = itemSize.x;
		}
	}

	Vector topLeftSize;
	Vector bottomRightSize;
	skin->getMenuPopupBorderSizes(this, topLeftSize, bottomRightSize);
	size += topLeftSize + bottomRightSize;

	resize(size);
}
