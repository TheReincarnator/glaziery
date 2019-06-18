/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


TreeField::TreeField()
{
	ASSERTION_COBJECT(this);

	horizontallyScrollable = true;
	indentation = 16;
	type = TYPE_MANY;
	verticallyScrollable = true;

	setMinimumSize(Vector(128, 64));

	Desktop::getInstance()->getSkin()->setupTreeField(this);
}

TreeField::~TreeField()
{
	ASSERTION_COBJECT(this);

	focusItemPath.DeleteAll();
	selectedItems.RemoveAll();
}

void TreeField::addContextMenuItems(Menu * menu, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::addContextMenuItems(menu, position, option1, option2);

	if (!isActiveIncludingAncestors())
		return;

	TreeField::Item * item = getFocusItem();
	if (item != NULL)
		addItemContextMenuItems(menu, item, position + getScrollPositionNow() - item->getPosition(),
			option1, option2);
}

void TreeField::addItemContextMenuItems(Menu * menu, TreeField::Item * item, Vector position,
	bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (!menu->getItems().IsEmpty())
		menu->appendSeparator();

	if (item->isExpanded())
		menu->appendItem(STRING_TREEFIELD_COLLAPSE, &onContextMenuCollapse);
	else
		menu->appendItem(STRING_TREEFIELD_EXPAND, &onContextMenuExpand);

	menu->appendSeparator();

	menu->appendItem(STRING_TREEFIELD_EXPANDALL, &onContextMenuExpandAll);
	menu->appendItem(STRING_TREEFIELD_COLLAPSEALL, &onContextMenuCollapseAll);
}

void TreeField::appendRoot(TreeField::Item * root)
{
	ASSERTION_COBJECT(this);
	insertRoot(roots.GetCount(), root);
}

void TreeField::collapseAll(bool deeply)
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<roots.GetCount(); i++)
		roots.Get(i)->collapse(deeply);
}

void TreeField::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawTreeField(context, this);
}

void TreeField::ensureFocusVisible(bool deeply)
{
	ASSERTION_COBJECT(this);

	TreeField::Item * focusItem = getFocusItem();
	if (focusItem == NULL)
		return;

	ensureVisible(focusItem->getPosition(), focusItem->getPosition()
		+ (deeply ? focusItem->getDeepSize() : focusItem->getSize()) - Vector(1, 1));
}

void TreeField::ensureFocusVisibleNow(bool deeply)
{
	ASSERTION_COBJECT(this);

	TreeField::Item * focusItem = getFocusItem();
	if (focusItem == NULL)
		return;

	ensureVisibleNow(focusItem->getPosition(), focusItem->getPosition()
		+ (deeply ? focusItem->getDeepSize() : focusItem->getSize()) - Vector(1, 1));
}

void TreeField::expandAll(bool deeply)
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<roots.GetCount(); i++)
		roots.Get(i)->expand(deeply);
}

TreeField::Item * TreeField::getFocusItem()
{
	ASSERTION_COBJECT(this);
	return getItem(focusItemPath);
}

const ArrayList<Int> & TreeField::getFocusItemPath()
{
	ASSERTION_COBJECT(this);
	return focusItemPath;
}

int TreeField::getIndentation()
{
	ASSERTION_COBJECT(this);
	return indentation;
}

TreeField::Item * TreeField::getItem(const ArrayList<Int> & path)
{
	ASSERTION_COBJECT(this);

	const ArrayList<Item> * items = &roots;
	Item * item = NULL;
	for (int i=0; i<path.GetCount(); i++)
	{
		item = items->Get(path.Get(i)->Get());
		if (item == NULL)
			return NULL;

		items = &item->getChildren();
	}

	return item;
}

TreeField::Item * TreeField::getItemAt(Vector position)
{
	ASSERTION_COBJECT(this);
	return getItemAt(position, roots);
}

TreeField::Item * TreeField::getItemAt(Vector position, const ArrayList<TreeField::Item> & items)
{
	ASSERTION_COBJECT(this);

	if (items.IsEmpty())
		return NULL;

	int lower = 0;
	int upper = items.GetCount() - 1;
	int middle = (upper + lower) / 2;

	while (true)
	{
		Item * currentItem = items.Get(middle);
		int currentItemX = currentItem->getPosition().x;
		int currentItemY = currentItem->getPosition().y;

		if (position.y >= currentItemY && position.y < currentItemY + currentItem->getDeepSize().y)
		{
			if (position.y >= currentItemY && position.y < currentItemY + currentItem->getSize().y)
			{
				if (position.x >= currentItemX && position.x < currentItemX + currentItem->getSize().x)
					return currentItem;
				else
					return NULL;
			}
			else
				return getItemAt(position, currentItem->getChildren());
		}

		if (lower >= upper)
			return NULL;

		if (position.y >= currentItem->getPosition().y)
			lower = middle + 1;
		else
			upper = middle - 1;

		middle = (upper + lower) / 2;
	}
}

const ArrayList<TreeField::Item> & TreeField::getRoots()
{
	ASSERTION_COBJECT(this);
	return roots;
}

TreeField::Item * TreeField::getSelectedItem()
{
	ASSERTION_COBJECT(this);

	if (selectedItems.GetCount() != 1)
		return NULL;

	PointeredList * itemsList = selectedItems.GetAll();
	Item * item = (Item *) itemsList->GetFirst()->GetData();
	delete itemsList;

	return item;
}

const HashSet & TreeField::getSelectedItems()
{
	ASSERTION_COBJECT(this);
	return selectedItems;
}

TreeField::Type TreeField::getType()
{
	ASSERTION_COBJECT(this);
	return type;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(TreeField, ScrollingField);
#endif

void TreeField::insertRoot(int position, TreeField::Item * root)
{
	ASSERTION_COBJECT(this);

	if (root->getField() != NULL)
		throw EILLEGALSTATE("The item is already contained in another item or field");

	root->setFieldAndParent(this, NULL);
	roots.Insert(root, position);

	bool refocus = false;
	if (!focusItemPath.IsEmpty() && focusItemPath.GetFirst()->Get() >= position)
	{
		focusItemPath.GetFirst()->Set(focusItemPath.GetFirst()->Get() + 1);
		refocus = true;
	}
	else if (focusItemPath.IsEmpty() && type == TYPE_ONE)
		setFocusItem(root);

	if (selectedItems.IsEmpty() && type == TYPE_ONE)
		selectItem(root);

	updateContentSize();
	if (refocus)
		ensureFocusVisible();
	invalidate();
}

bool TreeField::isHorizontallyScrollable()
{
	ASSERTION_COBJECT(this);
	return horizontallyScrollable;
}

bool TreeField::isVerticallyScrollable()
{
	ASSERTION_COBJECT(this);
	return verticallyScrollable;
}

bool TreeField::moveDownPath(ArrayList<Int> & path)
{
	ASSERTION_COBJECT(this);

	if (roots.IsEmpty())
	{
		if (path.IsEmpty())
			return false;

		path.DeleteAll();
		return true;
	}

	TreeField::Item * item = getItem(path);
	if (item == NULL || item != NULL && item->isExpanded() && !item->getChildren().IsEmpty())
	{
		Int * pathElement;
		if ((pathElement = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		path.Append(pathElement);
	}
	else
	{
		int depth;
		for (depth = path.GetCount() - 1; depth >= 0; depth--)
		{
			const ArrayList<TreeField::Item> & siblings = item->getParent() != NULL
				? item->getParent()->getChildren() : roots;
			if (path.Get(depth)->Get() < siblings.GetCount() - 1)
				break;

			item = item->getParent();
		}

		if (depth < 0)
			return false;

		for (int i = path.GetCount() - 1; i > depth; i--)
			path.DeleteLast();

		path.Get(depth)->Add(1);
	}

	return true;
}

bool TreeField::moveUpPath(ArrayList<Int> & path)
{
	ASSERTION_COBJECT(this);

	if (roots.IsEmpty())
	{
		if (path.IsEmpty())
			return false;

		path.DeleteAll();
		return true;
	}

	if (path.IsEmpty())
	{
		Int * pathElement;
		if ((pathElement = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		path.Append(pathElement);
	}
	else if (path.GetLast()->Get() > 0)
	{
		path.GetLast()->Add(-1);
		TreeField::Item * item = getItem(path);
		while (item->isExpanded() && !item->getChildren().IsEmpty())
		{
			Int * pathElement;
			if ((pathElement = new Int(item->getChildren().GetCount() - 1)) == NULL)
				throw EOUTOFMEMORY;
			path.Append(pathElement);

			item = item->getChildren().GetLast();
		}
	}
	else if (path.GetCount() > 1)
		path.DeleteLast();
	else
		return false;

	return true;
}

void TreeField::notifyItemCollapsed(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		TreeField::Listener * treeFieldListener
			= dynamic_cast<TreeField::Listener *>(listeners.Get(i));
		if (treeFieldListener != NULL)
			treeFieldListener->onItemCollapsed(item);
	}
}

bool TreeField::notifyItemCollapsing(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		TreeField::Listener * treeFieldListener
			= dynamic_cast<TreeField::Listener *>(listeners.Get(i));
		if (treeFieldListener != NULL)
			if (treeFieldListener->onItemCollapsing(item) == VETO)
				return true;
	}

	return false;
}

void TreeField::notifyItemExpanded(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		TreeField::Listener * treeFieldListener
			= dynamic_cast<TreeField::Listener *>(listeners.Get(i));
		if (treeFieldListener != NULL)
			treeFieldListener->onItemExpanded(item);
	}
}

bool TreeField::notifyItemExpanding(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		TreeField::Listener * treeFieldListener
			= dynamic_cast<TreeField::Listener *>(listeners.Get(i));
		if (treeFieldListener != NULL)
			if (treeFieldListener->onItemExpanding(item) == VETO)
				return true;
	}

	return false;
}

void TreeField::notifySelectionChanged()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		TreeField::Listener * treeFieldListener
			= dynamic_cast<TreeField::Listener *>(listeners.Get(i));
		if (treeFieldListener != NULL)
			treeFieldListener->onSelectionChanged(this);
	}
}

bool TreeField::notifySelectionChanging()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		TreeField::Listener * treeFieldListener
			= dynamic_cast<TreeField::Listener *>(listeners.Get(i));
		if (treeFieldListener != NULL)
			if (treeFieldListener->onSelectionChanging(this) == VETO)
				return true;
	}

	return false;
}

bool TreeField::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingField::onCharacter(character, option1, option2);

	if (character == ' ' && option2)
	{
		selectCurrentItem();
		return true;
	}
	else if (character == '+')
	{
		TreeField::Item * focusItem = getFocusItem();
		if (focusItem != NULL)
		{
			focusItem->expand();
			updateSelectionFromFocus(false, false);
		}

		return true;
	}
	else if (character == '-')
	{
		TreeField::Item * focusItem = getFocusItem();
		if (focusItem != NULL)
		{
			focusItem->collapse();
			updateSelectionFromFocus(false, false);
		}

		return true;
	}
	else if (character == '*')
	{
		TreeField::Item * focusItem = getFocusItem();
		if (focusItem != NULL)
		{
			focusItem->expand(true);
			updateSelectionFromFocus(false, false);
		}

		return true;
	}
	else if (character == '/')
	{
		collapseAll(true);
		updateSelectionFromFocus(false, false);

		return true;
	}

	return consumed;
}

void TreeField::onContentClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	TreeField::Item * item = getItemAt(position);
	if (item != NULL)
	{
		Skin * skin = Desktop::getInstance()->getSkin();
		if (position.x < item->getPosition().x + skin->getTreeFieldExpansionWidgetSize(item).x)
		{
			item->setExpanded(!item->isExpanded());
			return;
		}

		bool itemWasFocused = item->hasFocus();
		bool itemWasSelected = item->isSelected();

		setFocusItem(item);

		updateSelectionFromFocus(option1, option2);
		if (!option1 && option2)
			selectCurrentItem();

		if (itemWasFocused && itemWasSelected && !option1 && !option2)
			onEdit();
	}
	else if (!option1 && !option2 && type != TreeField::TYPE_ONE)
	{
		if (!selectedItems.IsEmpty())
		{
			if (!notifySelectionChanging())
			{
				selectedItems.RemoveAll();
				notifySelectionChanged();
			}
		}
	}

	ScrollingField::onContentClick(position, option1, option2);
}

void TreeField::onContentContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	TreeField::Item * item = getItemAt(position);
	if (item != NULL)
	{
		Skin * skin = Desktop::getInstance()->getSkin();
		if (position.x < item->getPosition().x + skin->getTreeFieldExpansionWidgetSize(item).x)
			return;

		bool itemWasFocused = item->hasFocus();
		bool itemWasSelected = item->isSelected();

		setFocusItem(item);

		updateSelectionFromFocus(option1, option2);
		if (!option1 && option2)
			selectCurrentItem();
	}
	else if (!option1 && !option2 && type != TreeField::TYPE_ONE)
	{
		if (!selectedItems.IsEmpty())
		{
			if (!notifySelectionChanging())
			{
				selectedItems.RemoveAll();
				notifySelectionChanged();
			}
		}
	}

	ScrollingField::onContentContextClick(position, option1, option2);
}

void TreeField::onContextMenuCollapse(Menu::Item * item)
{
	TreeField * field = (TreeField *) item->getContainer();
	TreeField::Item * focusItem = field != NULL ? field->getFocusItem() : NULL;
	if (focusItem == NULL)
		return;

	focusItem->collapse();
}

void TreeField::onContextMenuCollapseAll(Menu::Item * item)
{
	TreeField * field = (TreeField *) item->getContainer();
	TreeField::Item * focusItem = field != NULL ? field->getFocusItem() : NULL;
	if (focusItem == NULL)
		return;

	focusItem->collapse(true);
}

void TreeField::onContextMenuExpand(Menu::Item * item)
{
	TreeField * field = (TreeField *) item->getContainer();
	TreeField::Item * focusItem = field != NULL ? field->getFocusItem() : NULL;
	if (focusItem == NULL)
		return;

	focusItem->expand();
}

void TreeField::onContextMenuExpandAll(Menu::Item * item)
{
	TreeField * field = (TreeField *) item->getContainer();
	TreeField::Item * focusItem = field != NULL ? field->getFocusItem() : NULL;
	if (focusItem == NULL)
		return;

	focusItem->expand(true);
}

bool TreeField::onEdit()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onEdit();

	if (!isEditable())
		return true;

	Item * item = getFocusItem();
	if (item == NULL)
		return true;

	item->edit();

	return true;
}

void TreeField::onElementChanged(FieldElement * element)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onElementChanged(element);
	updateContentSize();
}

bool TreeField::onMoveDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveDown(option1, option2);

	if (moveDownPath(focusItemPath))
	{
		updateSelectionFromFocus(option1, option2);
		ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool TreeField::onMoveLeft(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveLeft(option1, option2);

	TreeField::Item * focusItem = getFocusItem();
	if (focusItem != NULL)
	{
		if (focusItem->isExpanded() && !focusItem->getChildren().IsEmpty())
			focusItem->collapse();
		else if (focusItemPath.GetCount() > 1)
			focusItemPath.DeleteLast();

		updateSelectionFromFocus(option1, option2);
		ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool TreeField::onMoveRight(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveRight(option1, option2);

	TreeField::Item * focusItem = getFocusItem();
	const ArrayList<Item> & children = focusItem != NULL ? focusItem->getChildren() : roots;
	if (!children.IsEmpty())
	{
		if (focusItem != NULL && !focusItem->isExpanded())
			focusItem->expand();
		else
		{
			Int * pathElement;
			if ((pathElement = new Int(0)) == NULL)
				throw EOUTOFMEMORY;
			focusItemPath.Append(pathElement);
		}

		updateSelectionFromFocus(option1, option2);
		ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool TreeField::onMoveToEnd(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveToEnd(option1, option2);

	if (roots.IsEmpty())
		return true;

	bool modified = false;
	TreeField::Item * parent = NULL;
	int i;
	for (i=0; i<focusItemPath.GetCount() && (parent == NULL || parent->isExpanded()
		&& !parent->getChildren().IsEmpty()); i++)
	{
		const ArrayList<TreeField::Item> & parents = parent != NULL ? parent->getChildren() : roots;
		Int * position = focusItemPath.Get(i);
		if (position->Get() < parents.GetCount() - 1)
		{
			modified = true;
			position->Set(parents.GetCount() - 1);
		}

		parent = parents.GetLast();
	}

	while (i < focusItemPath.GetCount())
	{
		focusItemPath.DeleteLast();
		modified = true;
	}

	while (parent == NULL || parent->isExpanded() && !parent->getChildren().IsEmpty())
	{
		const ArrayList<TreeField::Item> & parents = parent != NULL ? parent->getChildren() : roots;
		Int * pathElement;
		if ((pathElement = new Int(parents.GetCount() - 1)) == NULL)
			throw EOUTOFMEMORY;
		focusItemPath.Append(pathElement);

		modified = true;

		parent = parents.GetLast();
	}

	if (modified)
	{
		updateSelectionFromFocus(option1, option2);
		ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool TreeField::onMoveToStart(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveToStart(option1, option2);

	if (roots.IsEmpty() || focusItemPath.GetCount() == 1 && focusItemPath.GetFirst()->Get() == 0)
		return true;

	focusItemPath.DeleteAll();
	if (!roots.IsEmpty())
	{
		Int * pathElement;
		if ((pathElement = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		focusItemPath.Append(pathElement);
	}

	updateSelectionFromFocus(option1, option2);
	ensureFocusVisible();
	invalidate();

	return true;
}

bool TreeField::onMoveUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveUp(option1, option2);

	if (moveUpPath(focusItemPath))
	{
		updateSelectionFromFocus(option1, option2);
		ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool TreeField::onPageDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onPageDown(option1, option2);

	int remaining = getContentViewSize().y;
	TreeField::Item * focusItem = getFocusItem();
	if (focusItem == NULL)
		return true;
	remaining -= focusItem->getSize().y;

	if (!moveDownPath(focusItemPath))
		return true;

	while (true)
	{
		TreeField::Item * focusItem = getFocusItem();
		if (focusItem == NULL)
			break;
		remaining -= focusItem->getSize().y;

		if (remaining < 0)
			break;
		if (!moveDownPath(focusItemPath))
			break;
	}

	updateSelectionFromFocus(option1, option2);
	ensureFocusVisible();
	invalidate();

	return true;
}

bool TreeField::onPageUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onPageUp(option1, option2);

	int remaining = getContentViewSize().y;
	TreeField::Item * focusItem = getFocusItem();
	if (focusItem == NULL)
		return true;
	remaining -= focusItem->getSize().y;

	if (!moveUpPath(focusItemPath))
		return true;

	while (true)
	{
		TreeField::Item * focusItem = getFocusItem();
		if (focusItem == NULL)
			break;
		remaining -= focusItem->getSize().y;

		if (remaining < 0)
			break;
		if (!moveUpPath(focusItemPath))
			break;
	}

	updateSelectionFromFocus(option1, option2);
	ensureFocusVisible();
	invalidate();

	return true;
}

bool TreeField::onSelectAll()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onSelectAll();
	selectAll(false);
	return true;
}

void TreeField::prependRoot(TreeField::Item * root)
{
	ASSERTION_COBJECT(this);

	if (root->getField() != NULL)
		throw EILLEGALSTATE("The item is already contained in another item or field");

	root->setFieldAndParent(this, NULL);
	roots.Prepend(root);

	bool refocus = false;
	if (focusItemPath.GetCount() >= 1)
	{
		focusItemPath.GetFirst()->Set(focusItemPath.GetFirst()->Get() + 1);
		refocus = true;
	}
	else if (focusItemPath.IsEmpty() && type == TYPE_ONE)
		setFocusItem(root);

	updateContentSize();
	if (refocus)
		ensureFocusVisible();
	invalidate();
}

void TreeField::removeRoot(int position)
{
	ASSERTION_COBJECT(this);

	if (position < 0 || position >= roots.GetCount())
		return;

	bool selectionChanged = unselectBranchInternal(roots.Get(position));

	bool refocus = false;
	if (focusItemPath.GetCount() >= 1)
		if (focusItemPath.GetFirst()->Get() == position)
		{
			focusItemPath.DeleteAll();
			refocus = true;
		}
		else if (focusItemPath.GetFirst()->Get() > position)
		{
			focusItemPath.GetFirst()->Set(focusItemPath.GetFirst()->Get() - 1);
			refocus = true;
		}

	if (rangeStartItemPath.GetCount() >= 1)
		if (rangeStartItemPath.GetFirst()->Get() == position)
			rangeStartItemPath.DeleteAll();
		else if (rangeStartItemPath.GetFirst()->Get() > position)
			rangeStartItemPath.GetFirst()->Set(rangeStartItemPath.GetFirst()->Get() - 1);

	roots.Delete(position);

	updateContentSize();

	if (refocus)
		ensureFocusVisible();

	if (selectionChanged)
	{
		notifySelectionChanging();
		notifySelectionChanged();
	}

	invalidate();
}

void TreeField::removeRoots()
{
	ASSERTION_COBJECT(this);

	bool selectionChanged = !selectedItems.IsEmpty();
	selectedItems.RemoveAll();
	focusItemPath.DeleteAll();
	rangeStartItemPath.DeleteAll();
	roots.DeleteAll();

	updateContentSize();
	ensureFocusVisible();

	if (selectionChanged)
	{
		notifySelectionChanging();
		notifySelectionChanged();
	}

	invalidate();
}

void TreeField::selectAll(bool collapsedToo)
{
	ASSERTION_COBJECT(this);

	if (notifySelectionChanging())
		return;

	bool selectionChanged = false;
	for (int i=0; i<roots.GetCount(); i++)
		if (selectBranchInternal(roots.Get(i), collapsedToo))
			selectionChanged = true;

	if (selectionChanged)
	{
		notifySelectionChanged();
		invalidate();
	}
}

void TreeField::selectBranch(Item * root, bool collapsedToo)
{
	ASSERTION_COBJECT(this);

	if (notifySelectionChanging())
		return;

	if (selectBranchInternal(root, collapsedToo))
	{
		notifySelectionChanged();
		invalidate();
	}
}

bool TreeField::selectBranchInternal(Item * root, bool collapsedToo)
{
	ASSERTION_COBJECT(this);

	bool selectionChanged = !selectedItems.Contains(root);
	selectedItems.Add(root);

	if (collapsedToo || root->isExpanded())
	{
		const ArrayList<Item> & children = root->getChildren();
		for (int i=0; i<children.GetCount(); i++)
			if (selectBranchInternal(children.Get(i), collapsedToo))
				selectionChanged = true;
	}

	return selectionChanged;
}

void TreeField::selectCurrentItem()
{
	ASSERTION_COBJECT(this);

	TreeField::Item * focusItem = getFocusItem();
	if (focusItem == NULL)
		return;

	if (notifySelectionChanging())
		return;

	if (getType() == ListField::TYPE_ONE)
	{
		selectedItems.RemoveAll();
		selectedItems.Add(focusItem);
	}
	else if (getType() == ListField::TYPE_ONE_OR_NONE)
	{
		bool focusEqualsSelection = selectedItems.GetCount() == 1 && selectedItems.Contains(focusItem);
		selectedItems.RemoveAll();
		if (!focusEqualsSelection)
			selectedItems.Add(focusItem);
	}
	else
	{
		if (selectedItems.Contains(focusItem))
			selectedItems.Remove(focusItem);
		else
			selectedItems.Add(focusItem);
	}

	notifySelectionChanged();
	invalidate();
}

void TreeField::selectItem(Item * item)
{
	ASSERTION_COBJECT(this);

	if (item->getField() != this)
		throw EILLEGALARGUMENT("The item is not part of this tree field");

	if (selectedItems.Contains(item))
		return;

	if (notifySelectionChanging())
		return;

	if (type != TYPE_MANY)
		selectedItems.RemoveAll();

	selectedItems.Add(item);

	if (type == TYPE_ONE)
		setFocusItem(item);

	notifySelectionChanged();
	invalidate();
}

void TreeField::setFocusItem(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);

	if (item == NULL)
	{
		if (!focusItemPath.IsEmpty())
		{
			focusItemPath.DeleteAll();
			invalidate();
		}

		return;
	}

	if (item->getField() != this)
		throw EILLEGALARGUMENT("The item is not part of this tree field");

	ArrayList<Int> * newFocusItemPath = item->createPath();
	if (focusItemPath.Equals(newFocusItemPath))
	{
		delete newFocusItemPath;
		return;
	}

	focusItemPath.DeleteAll();
	while (!newFocusItemPath->IsEmpty())
		focusItemPath.Append(newFocusItemPath->UnlinkFirst());
	delete newFocusItemPath;

	ensureFocusVisible();
	invalidate();
}

void TreeField::setHorizontallyScrollable(bool horizontallyScrollable)
{
	ASSERTION_COBJECT(this);

	if (this->horizontallyScrollable == horizontallyScrollable)
		return;

	this->horizontallyScrollable = horizontallyScrollable;

	updateContentSize();
	invalidate();
}

void TreeField::setIndentation(int indentation)
{
	ASSERTION_COBJECT(this);

	if (this->indentation == indentation)
		return;

	this->indentation = indentation;

	updateContentSize();
	invalidate();
}

void TreeField::setSelectedItem(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);

	if (item != NULL && item->getField() != this)
		throw EILLEGALARGUMENT("The item is not part of this tree field");

	if (getSelectedItem() == item)
		return;

	if (item != NULL)
	{
		selectedItems.RemoveAll();
		selectItem(item);
	}
	else
		unselectItems();
}

void TreeField::setSelectedItems(const HashSet & selectedItems)
{
	ASSERTION_COBJECT(this);

	if (this->selectedItems.GetCount() == selectedItems.GetCount())
	{
		PointeredList * selectedItemsList = this->selectedItems.GetAll();
		PointeredListItem * selectedItemsListItem = selectedItemsList->GetFirst();
		while (selectedItemsListItem != NULL)
		{
			Item * selectedItem = (Item *) selectedItemsListItem->GetData();
			if (!selectedItems.Contains(selectedItem))
				break;

			selectedItemsListItem = selectedItemsListItem->GetNext();
		}
		delete selectedItemsList;

		if (selectedItemsListItem == NULL)
			return;
	}

	if (notifySelectionChanging())
		return;

	this->selectedItems.RemoveAll();

	if (!selectedItems.IsEmpty())
	{
		PointeredList * selectedItemsList = selectedItems.GetAll();
		PointeredListItem * selectedItemsListItem = selectedItemsList->GetFirst();
		while (selectedItemsListItem != NULL)
		{
			Item * selectedItem = (Item *) selectedItemsListItem->GetData();
			this->selectedItems.Add(selectedItem);

			// Use only one, if type is not many, but focus it
			if (type != TYPE_MANY)
			{
				setFocusItem(selectedItem);
				break;
			}

			selectedItemsListItem = selectedItemsListItem->GetNext();
		}
		delete selectedItemsList;
	}

	notifySelectionChanged();
	invalidate();
}

void TreeField::setType(TreeField::Type type)
{
	ASSERTION_COBJECT(this);

	if (this->type == type)
		return;

	this->type = type;

	if (type != TYPE_MANY && selectedItems.GetCount() > 1)
		unselectItems();
	else if (type == TYPE_ONE)
		setSelectedItem(getFocusItem());

	invalidate();
}

void TreeField::setVerticallyScrollable(bool verticallyScrollable)
{
	ASSERTION_COBJECT(this);

	if (this->verticallyScrollable == verticallyScrollable)
		return;

	this->verticallyScrollable = verticallyScrollable;

	updateContentSize();
	invalidate();
}

String TreeField::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("TreeField(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}

void TreeField::unselectBranch(Item * root)
{
	ASSERTION_COBJECT(this);

	if (notifySelectionChanging())
		return;

	if (unselectBranchInternal(root))
	{
		notifySelectionChanged();
		invalidate();
	}
}

bool TreeField::unselectBranchInternal(Item * root)
{
	ASSERTION_COBJECT(this);

	bool selectionChanged = selectedItems.Contains(root);
	selectedItems.Remove(root);

	const ArrayList<Item> & children = root->getChildren();
	for (int i=0; i<children.GetCount(); i++)
		if (unselectBranchInternal(children.Get(i)))
			selectionChanged = true;

	return selectionChanged;
}

void TreeField::unselectItem(Item * item)
{
	ASSERTION_COBJECT(this);

	if (item->getField() != this)
		throw EILLEGALARGUMENT("The item is not part of this tree field");

	if (!selectedItems.Contains(item) || type == TYPE_ONE)
		return;

	if (notifySelectionChanging())
		return;

	selectedItems.Remove(item);

	notifySelectionChanged();
	invalidate();
}

void TreeField::unselectItems()
{
	ASSERTION_COBJECT(this);

	if (selectedItems.IsEmpty() || type == TYPE_ONE)
		return;

	if (notifySelectionChanging())
		return;

	selectedItems.RemoveAll();

	notifySelectionChanged();
	invalidate();
}

void TreeField::updateContentSize()
{
	ASSERTION_COBJECT(this);

	contentSize = Vector();
	for (int i=0; i<roots.GetCount(); i++)
	{
		Vector rootSize = roots.Get(i)->updateDeepSize(Vector(0, contentSize.y));
		contentSize.y += rootSize.y;
		if (contentSize.x < rootSize.x)
			contentSize.x = rootSize.x;
	}
}

void TreeField::updateFocusAndRangeStartPaths()
{
	ASSERTION_COBJECT(this);

	Item * item = NULL;
	int depth;
	for (depth = 0; depth < focusItemPath.GetCount() - 1; depth++)
	{
		int position = focusItemPath.Get(depth)->Get();
		item = item != NULL ? item->getChildren().Get(position) : roots.Get(position);
		if (!item->isExpanded())
			break;
	}

	for (int i = focusItemPath.GetCount() - 1; i > depth; i--)
		focusItemPath.DeleteLast();

	item = NULL;
	for (depth = 0; depth < rangeStartItemPath.GetCount() - 1; depth++)
	{
		int position = rangeStartItemPath.Get(depth)->Get();
		item = item != NULL ? item->getChildren().Get(position) : roots.Get(position);
		if (!item->isExpanded())
			break;
	}

	for (int i = rangeStartItemPath.GetCount() - 1; i > depth; i--)
		rangeStartItemPath.DeleteLast();
}

void TreeField::updateSelectionFromFocus(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (!option1 || type != TYPE_MANY)
	{
		rangeStartItemPath.DeleteAll();
		for (int i=0; i<focusItemPath.GetCount(); i++)
		{
			Int * pathElement;
			if ((pathElement = new Int(focusItemPath.Get(i)->Get())) == NULL)
				throw EOUTOFMEMORY;
			rangeStartItemPath.Append(pathElement);
		}
	}

	if (focusItemPath.IsEmpty() || rangeStartItemPath.IsEmpty())
		return;

	if (!option2 || type != TYPE_MANY)
	{
		const ArrayList<Int> * firstPath = &rangeStartItemPath;
		const ArrayList<Int> * secondPath = &focusItemPath;

		int maxPathLength = focusItemPath.GetCount() > rangeStartItemPath.GetCount()
			? focusItemPath.GetCount() : rangeStartItemPath.GetCount();
		for (int i = 0; i < maxPathLength; i++)
		{
			int focusItemPos = i < focusItemPath.GetCount()
				? focusItemPath.Get(i)->Get() : -1;
			int rangeStartItemPos = i < rangeStartItemPath.GetCount()
				? rangeStartItemPath.Get(i)->Get() : -1;

			if (rangeStartItemPos > focusItemPos)
			{
				firstPath = &focusItemPath;
				secondPath = &rangeStartItemPath;
				break;
			}
			else if (rangeStartItemPos < focusItemPos)
				break;
		}

		HashSet itemsToSelect;

		ArrayList<Int> path;
		for (int i=0; i<firstPath->GetCount(); i++)
		{
			Int * pathElement;
			if ((pathElement = new Int(firstPath->Get(i)->Get())) == NULL)
				throw EOUTOFMEMORY;
			path.Append(pathElement);
		}

		TreeField::Item * endItem = getItem(*secondPath);
		TreeField::Item * item = getItem(path);
		itemsToSelect.Add(item);
		while (item != endItem)
		{
			if (!moveDownPath(path))
				break;

			item = getItem(path);
			itemsToSelect.Add(item);
		}

		setSelectedItems(itemsToSelect);
	}
}


TreeField::Item::Item()
{
	ASSERTION_COBJECT(this);

	expanded = false;
	field = NULL;
	parent = NULL;
}

TreeField::Item::~Item()
{
	ASSERTION_COBJECT(this);
}

void TreeField::Item::appendChild(TreeField::Item * child)
{
	ASSERTION_COBJECT(this);
	insertChild(children.GetCount(), child);
}

void TreeField::Item::appendElement(FieldElement * element)
{
	ASSERTION_COBJECT(this);
	insertElement(elements.GetCount(), element);
}

void TreeField::Item::collapse(bool deeply)
{
	ASSERTION_COBJECT(this);
	setExpanded(false, deeply);
}

void TreeField::Item::collectBranch(HashSet & items, bool collapsedToo)
{
	ASSERTION_COBJECT(this);

	items.Add(this);
	if (expanded || collapsedToo)
		for (int i=0; i<children.GetCount(); i++)
			children.Get(i)->collectBranch(items, collapsedToo);
}

ArrayList<Int> * TreeField::Item::createPath()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	ArrayList<Int> * path;
	if ((path = new ArrayList<Int>) == NULL)
		throw EOUTOFMEMORY;

	Item * item = this;
	while (item != NULL)
	{
		Item * parent = item->getParent();
		const ArrayList<Item> & parentChildren = parent != NULL ? parent->getChildren() : field->getRoots();

		int position = parentChildren.Find(item);
		if (position < 0)
			throw EILLEGALSTATE("Cannot find tree item in parent (internal error)");

		Int * pathElement;
		if ((pathElement = new Int(position)) == NULL)
			throw EOUTOFMEMORY;
		path->Prepend(pathElement);

		item = parent;
	}

	return path;
}

FieldElement * TreeField::Item::edit()
{
	ASSERTION_COBJECT(this);

	int elementNo;
	for (elementNo=0; elementNo < elements.GetCount(); elementNo++)
		if (elements.Get(elementNo)->isEditable())
			break;

	if (elementNo >= elements.GetCount())
		return NULL;
	FieldElement * element = elements.Get(elementNo);

	Desktop * desktop = Desktop::getInstance();
	Skin * skin = desktop->getSkin();
	TreeField * field = getField();

	Vector editPosition = field->getAbsolutePosition() + position - field->getScrollPositionNow();
	editPosition.x += skin->getTreeFieldExpansionWidgetSize(this).x;
	for (int i=0; i<elementNo; i++)
		editPosition.x += elements.Get(i)->getSize().x;

	element->edit(editPosition, element->getSize(), field);

	return element;
}

void TreeField::Item::ensureVisible(bool deeply)
{
	ASSERTION_COBJECT(this);
	field->ensureVisible(getPosition(), getPosition() + (deeply ? getDeepSize() : getSize()) - Vector(1, 1));
}

void TreeField::Item::ensureVisibleNow(bool deeply)
{
	ASSERTION_COBJECT(this);
	field->ensureVisibleNow(getPosition(), getPosition() + (deeply ? getDeepSize() : getSize()) - Vector(1, 1));
}

void TreeField::Item::expand(bool deeply)
{
	ASSERTION_COBJECT(this);
	setExpanded(true, deeply);
}

Int * TreeField::Item::findChildPositionInPath(const ArrayList<Int> & path)
{
	ASSERTION_COBJECT(this);

	if (path.IsEmpty())
		return NULL;

	Item * item = field->getRoots().Get(path.GetFirst()->Get());
	int count = path.GetCount();
	int position;
	for (position=1; item != this && position < count; position++)
		item = item->getChildren().Get(path.Get(position)->Get());

	return path.Get(position);
}

const ArrayList<TreeField::Item> & TreeField::Item::getChildren()
{
	ASSERTION_COBJECT(this);
	return children;
}

Vector TreeField::Item::getDeepSize()
{
	ASSERTION_COBJECT(this);
	return deepSize;
}

int TreeField::Item::getDepth()
{
	ASSERTION_COBJECT(this);

	int depth = 0;
	Item * item = getParent();
	while (item != NULL)
	{
		item = item->getParent();
		depth++;
	}

	return depth;
}

const ArrayList<FieldElement> & TreeField::Item::getElements()
{
	ASSERTION_COBJECT(this);
	return elements;
}

TreeField * TreeField::Item::getField()
{
	ASSERTION_COBJECT(this);
	return field;
}

TreeField::Item * TreeField::Item::getParent()
{
	ASSERTION_COBJECT(this);
	return parent;
}

Vector TreeField::Item::getPosition()
{
	ASSERTION_COBJECT(this);
	return position;
}

Vector TreeField::Item::getSize()
{
	ASSERTION_COBJECT(this);
	return size;
}

bool TreeField::Item::hasFocus()
{
	ASSERTION_COBJECT(this);
	return field->getFocusItem() == this;
}

void TreeField::Item::insertChild(int position, TreeField::Item * child)
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	if (child->getField() != NULL)
		throw EILLEGALSTATE("The item is already contained in another item or field");

	child->setFieldAndParent(field, this);
	children.Insert(child, position);

	Int * pathPosition = findChildPositionInPath(field->focusItemPath);
	if (pathPosition != NULL && pathPosition->Get() >= position)
		pathPosition->Add(1);

	pathPosition = findChildPositionInPath(field->rangeStartItemPath);
	if (pathPosition != NULL && pathPosition->Get() >= position)
		pathPosition->Add(1);

	field->updateContentSize();
	field->invalidate();
}

void TreeField::Item::insertElement(int position, FieldElement * element)
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	if (element->getField() != NULL)
		throw EILLEGALSTATE("The element is already contained in a field");

	element->setFieldAndParent(field, this);
	elements.Insert(element, position);

	field->updateContentSize();
	field->invalidate();
}

bool TreeField::Item::isExpanded()
{
	ASSERTION_COBJECT(this);
	return expanded;
}

bool TreeField::Item::isSelected()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	return field->getSelectedItems().Contains(this);
}

void TreeField::Item::prependChild(TreeField::Item * child)
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	if (child->getField() != NULL)
		throw EILLEGALSTATE("The item is already contained in another item or field");

	child->setFieldAndParent(field, this);
	children.Prepend(child);

	Int * pathPosition = findChildPositionInPath(field->focusItemPath);
	if (pathPosition != NULL)
		pathPosition->Add(1);

	pathPosition = findChildPositionInPath(field->rangeStartItemPath);
	if (pathPosition != NULL)
		pathPosition->Add(1);

	field->updateContentSize();
	field->invalidate();
}

void TreeField::Item::prependElement(FieldElement * element)
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	if (element->getField() != NULL)
		throw EILLEGALSTATE("The element is already contained in a field");

	element->setFieldAndParent(field, this);
	elements.Prepend(element);

	field->updateContentSize();
	field->invalidate();
}

void TreeField::Item::removeChild(int position)
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	if (position < 0 || position >= children.GetCount())
		return;

	bool selectionChanged = field->unselectBranchInternal(children.Get(position));

	Int * pathPosition = findChildPositionInPath(field->focusItemPath);
	if (pathPosition != NULL)
		if (pathPosition->Get() == position)
		{
			int depth = getDepth();
			while (field->focusItemPath.GetCount() > depth + 1)
				field->focusItemPath.DeleteLast();
		}
		else if (pathPosition->Get() > position)
			pathPosition->Add(-1);

	pathPosition = findChildPositionInPath(field->rangeStartItemPath);
	if (pathPosition != NULL)
		if (pathPosition->Get() == position)
		{
			int depth = getDepth();
			while (field->rangeStartItemPath.GetCount() > depth + 1)
				field->rangeStartItemPath.DeleteLast();
		}
		else if (pathPosition->Get() > position)
			pathPosition->Add(-1);

	children.Delete(position);

	field->updateContentSize();

	if (selectionChanged)
	{
		field->notifySelectionChanging();
		field->notifySelectionChanged();
	}

	field->invalidate();
}

void TreeField::Item::removeChildren()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	bool selectionChanged = false;
	for (int i=0; i<children.GetCount(); i++)
		if (field->unselectBranchInternal(children.Get(i)))
			selectionChanged = true;

	children.DeleteAll();

	field->updateFocusAndRangeStartPaths();
	field->updateContentSize();

	if (selectionChanged)
	{
		field->notifySelectionChanging();
		field->notifySelectionChanged();
	}

	field->invalidate();
}

void TreeField::Item::removeElement(int position)
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	elements.Delete(position);

	field->updateContentSize();
	field->invalidate();
}

void TreeField::Item::removeElements()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	elements.DeleteAll();

	field->updateContentSize();
	field->invalidate();
}

void TreeField::Item::select()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	field->selectItem(this);
}

void TreeField::Item::setExpanded(bool expanded, bool deeply)
{
	ASSERTION_COBJECT(this);

	setExpandedInternal(expanded, deeply);

	field->updateFocusAndRangeStartPaths();
	field->updateContentSize();
	ensureVisible(true);
	field->invalidate();
}

void TreeField::Item::setExpandedInternal(bool expanded, bool deeply)
{
	ASSERTION_COBJECT(this);

	if (this->expanded != expanded)
	{
		if (expanded)
		{
			if (field->notifyItemExpanding(this))
				return;
		}
		else
		{
			if (field->notifyItemCollapsing(this))
				return;
		}

		this->expanded = expanded;

		if (expanded)
			field->notifyItemExpanded(this);
		else
			field->notifyItemCollapsed(this);
	}

	if (deeply)
		for (int i=0; i<children.GetCount(); i++)
			children.Get(i)->setExpandedInternal(expanded, true);
}

void TreeField::Item::setFieldAndParent(TreeField * field, TreeField::Item * parent)
{
	ASSERTION_COBJECT(this);

	this->field = field;
	this->parent = parent;
}

void TreeField::Item::unselect()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The item must be added to a tree field before using it");

	field->unselectItem(this);
}

Vector TreeField::Item::updateDeepSize(Vector offset)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	Vector widgetSize = skin->getTreeFieldExpansionWidgetSize(this);
	int elementsSpace = skin->getTreeFieldElementSpaceWidth(this);
	int itemsSpace = skin->getTreeFieldItemSpaceHeight(getField());

	Vector deepSize(widgetSize.x, 0);
	for (int i=0; i<elements.GetCount(); i++)
	{
		if (i > 0)
			deepSize.x += elementsSpace;

		Vector elementSize = elements.Get(i)->getSize();
		deepSize.x += elementSize.x;
		if (deepSize.y < elementSize.y)
			deepSize.y = elementSize.y;
	}
	this->position = offset;
	this->size = deepSize;
	deepSize.y += itemsSpace;

	if (expanded)
	{
		Vector position = offset + Vector(field->getIndentation(), deepSize.y);
		for (int i=0; i<children.GetCount(); i++)
		{
			Vector childSize = children.Get(i)->updateDeepSize(position) + Vector(field->getIndentation(), 0);
			position.y += childSize.y;
			deepSize.y += childSize.y;
			if (deepSize.x < childSize.x)
				deepSize.x = childSize.x;
		}
	}

	this->deepSize = deepSize;

	return deepSize;
}


TreeField::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void TreeField::Listener::onItemCollapsed(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);
}

VetoMode TreeField::Listener::onItemCollapsing(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}

void TreeField::Listener::onItemExpanded(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);
}

VetoMode TreeField::Listener::onItemExpanding(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}

void TreeField::Listener::onSelectionChanged(TreeField * field)
{
	ASSERTION_COBJECT(this);
}

VetoMode TreeField::Listener::onSelectionChanging(TreeField * field)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}
