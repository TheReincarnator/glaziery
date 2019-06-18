/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Menu::Menu(Component * container)
{
	ASSERTION_COBJECT(this);

	this->container = container;
	horizontal = false;
	parent = NULL;
}

Menu::Menu(Menu::Item * parent)
{
	ASSERTION_COBJECT(this);

	this->container = parent->getContainer();
	horizontal = false;
	this->parent = parent;
}

Menu::~Menu()
{
	ASSERTION_COBJECT(this);
}

Menu::Item * Menu::appendItem(const String & text, void (*function)(Item * item))
{
	ASSERTION_COBJECT(this);

	Menu::Item * item;
	if ((item = new Menu::Item(this, text)) == NULL)
		throw EOUTOFMEMORY;

	items.Append(item);

	if (function != NULL)
		item->addSelectedFunction(function);

	return item;
}

Menu::Item * Menu::appendSeparator()
{
	ASSERTION_COBJECT(this);

	Menu::Item * item;
	if ((item = new Menu::Item(this, "", true)) == NULL)
		throw EOUTOFMEMORY;

	items.Append(item);
	return item;
}

Component * Menu::getContainer()
{
	ASSERTION_COBJECT(this);
	return container;
}

const ArrayList<Menu::Item> & Menu::getItems()
{
	ASSERTION_COBJECT(this);
	return items;
}

Menu::Item * Menu::getParent()
{
	ASSERTION_COBJECT(this);
	return parent;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Menu, GlazieryObject);
#endif

Menu::Item * Menu::insertItem(int position, const String & text, void (*function)(Item * item))
{
	ASSERTION_COBJECT(this);

	Menu::Item * item;
	if ((item = new Menu::Item(this, text)) == NULL)
		throw EOUTOFMEMORY;

	items.Insert(item, position);

	if (function != NULL)
		item->addSelectedFunction(function);

	return item;
}

Menu::Item * Menu::insertSeparator(int position)
{
	ASSERTION_COBJECT(this);

	Menu::Item * item;
	if ((item = new Menu::Item(this, "", true)) == NULL)
		throw EOUTOFMEMORY;

	items.Insert(item, position);
	return item;
}

bool Menu::isHorizontal()
{
	ASSERTION_COBJECT(this);
	return horizontal;
}

Menu::Item * Menu::prependItem(const String & text, void (*function)(Item * item))
{
	ASSERTION_COBJECT(this);

	Menu::Item * item;
	if ((item = new Menu::Item(this, text)) == NULL)
		throw EOUTOFMEMORY;

	items.Prepend(item);

	if (function != NULL)
		item->addSelectedFunction(function);

	return item;
}

Menu::Item * Menu::prependSeparator()
{
	ASSERTION_COBJECT(this);

	Menu::Item * item;
	if ((item = new Menu::Item(this, "", true)) == NULL)
		throw EOUTOFMEMORY;

	items.Prepend(item);
	return item;
}

void Menu::removeItem(int position)
{
	ASSERTION_COBJECT(this);
	items.Delete(position);
}

void Menu::setHorizontal(bool horizontal)
{
	ASSERTION_COBJECT(this);
	this->horizontal = horizontal;
}


Menu::Item::Item(Menu * parent, const String & text, bool separator)
{
	ASSERTION_COBJECT(this);

	active = true;
	checkState = NONE;
	this->parent = parent;
	this->separator = separator;
	subMenu = NULL;
	this->text = text;
}

Menu::Item::~Item()
{
	ASSERTION_COBJECT(this);

	if (subMenu != NULL)
		delete subMenu;
}

Menu::Item::SelectedFunctionListener * Menu::Item::addSelectedFunction(void (*function)(Item * item))
{
	ASSERTION_COBJECT(this);

	SelectedFunctionListener * listener;
	if ((listener = new SelectedFunctionListener(function)) == NULL)
		throw EOUTOFMEMORY;

	appendListener(listener);
	return listener;
}

Menu * Menu::Item::createSubMenu()
{
	ASSERTION_COBJECT(this);

	if (subMenu != NULL) {delete subMenu; subMenu = NULL;}

	if ((subMenu = new Menu(this)) == NULL)
		throw EOUTOFMEMORY;

	return subMenu;
}

Menu::CheckState Menu::Item::getCheckState()
{
	ASSERTION_COBJECT(this);
	return checkState;
}

Component * Menu::Item::getContainer()
{
	ASSERTION_COBJECT(this);
	return parent != NULL ? parent->getContainer() : NULL;
}

const String & Menu::Item::getText()
{
	ASSERTION_COBJECT(this);
	return text;
}

Menu * Menu::Item::getParent()
{
	ASSERTION_COBJECT(this);
	return parent;
}

Menu * Menu::Item::getSubMenu()
{
	ASSERTION_COBJECT(this);
	return subMenu;
}

bool Menu::Item::isActive()
{
	ASSERTION_COBJECT(this);
	return active;
}

bool Menu::Item::isSeparator()
{
	ASSERTION_COBJECT(this);
	return separator;
}

void Menu::Item::removeSubMenu()
{
	ASSERTION_COBJECT(this);

	if (subMenu != NULL) {delete subMenu; subMenu = NULL;}
}

void Menu::Item::setActive(bool active)
{
	ASSERTION_COBJECT(this);
	this->active = active;
}

void Menu::Item::setCheckState(Menu::CheckState checkState)
{
	ASSERTION_COBJECT(this);
	this->checkState = checkState;
}

void Menu::Item::setText(const String & text)
{
	ASSERTION_COBJECT(this);

	if (this->text == text)
		return;

	this->text = text;
}


Menu::Item::Listener::Listener()
{
	ASSERTION_COBJECT(this);
	components = 0;
}

Menu::Item::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void Menu::Item::Listener::onItemSelected(Item * item)
{
	ASSERTION_COBJECT(this);
}


Menu::Item::SelectedFunctionListener::SelectedFunctionListener(void (*function)(Item * item))
{
	ASSERTION_COBJECT(this);
	this->function = function;
}

void Menu::Item::SelectedFunctionListener::onItemSelected(Item * item)
{
	ASSERTION_COBJECT(this);
	(*function)(item);
}
