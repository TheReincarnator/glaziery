/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_TREEFIELD_H
#define __GLAZIERY_TREEFIELD_H


#define GLAZIERY_TREEFIELD(variable, area, groupField, \
	horizontallyScrollable, verticallyScrollable, type) \
	TreeField * variable; \
	if ((variable = new TreeField) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setHorizontallyScrollable(horizontallyScrollable); \
	variable->setVerticallyScrollable(verticallyScrollable); \
	variable->setType(TreeField::TYPE_##type);

#define GLAZIERY_TREEFIELD_P(variable, area, groupField, px, py, \
	horizontallyScrollable, verticallyScrollable, type) \
	GLAZIERY_TREEFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_TREEFIELD_S(variable, area, groupField, width, height, \
	horizontallyScrollable, verticallyScrollable, type) \
	GLAZIERY_TREEFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_TREEFIELD_PS(variable, area, groupField, px, py, width, height, \
	horizontallyScrollable, verticallyScrollable, type) \
	GLAZIERY_TREEFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_TREEFIELD_M(variable, area, groupField, minimumWidth, minimumHeight, \
	horizontallyScrollable, verticallyScrollable, type) \
	GLAZIERY_TREEFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_TREEFIELD_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	horizontallyScrollable, verticallyScrollable, type) \
	GLAZIERY_TREEFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_TREEFIELD_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	horizontallyScrollable, verticallyScrollable, type) \
	GLAZIERY_TREEFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_TREEFIELD_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	horizontallyScrollable, verticallyScrollable, type) \
	GLAZIERY_TREEFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_TREEFIELD_ITEM(variable, field, parentItem) \
	TreeField::Item * variable; \
	if ((variable = new TreeField::Item) == NULL) \
		throw EOUTOFMEMORY; \
	if ((parentItem) != NULL) \
		((TreeField::Item *) (parentItem))->appendChild(variable); \
	else \
		(field)->appendRoot(variable);

#define GLAZIERY_TREEFIELD_TEXTELEMENT(variable, item, text) \
	TextElement * variable; \
	if ((variable = new TextElement) == NULL) \
		throw EOUTOFMEMORY; \
	item->appendElement(variable); \
	variable->setText(text);

#define GLAZIERY_TREEFIELD_IMAGEELEMENT(variable, item, image) \
	ImageElement * variable; \
	if ((variable = new ImageElement) == NULL) \
		throw EOUTOFMEMORY; \
	item->appendElement(variable); \
	variable->setImage(image);

#define GLAZIERY_TREEFIELD_IMAGEELEMENT_I(variable, item, imageId) \
	ImageElement * variable; \
	if ((variable = new ImageElement) == NULL) \
		throw EOUTOFMEMORY; \
	item->appendElement(variable); \
	variable->setImageId(imageId);


namespace glaziery
{
	class TreeField : public ScrollingField
	{
		public:

			class Item;
			class Listener;
			enum Type;

			class Item : public GlazieryObject
			{
				friend ArrayList<Item>;
				friend TreeField;

				private:

					ArrayList<Item> children;

					Vector deepSize;

					ArrayList<FieldElement> elements;

					bool expanded;

					TreeField * field;

					Item * parent;

					/**
					 * A cache of the position of this tree item, relative to the tree field.
					 * This cache is automatically updated on tree field changes, e.g. item expansion.
					 */
					Vector position;

					/**
					 * A cache of the size of this tree item, including children, if expanded.
					 * This cache is automatically updated on tree field changes, e.g. item expansion.
					 */
					Vector size;

				protected:

					~Item();

					void collectBranch(HashSet & items, bool collapsedToo);

					Int * findChildPositionInPath(const ArrayList<Int> & path);

					void setExpandedInternal(bool expanded, bool deeply);

					void setFieldAndParent(TreeField * field, TreeField::Item * parent);

					/**
					 * Updates the size of the item including its children and element and
					 * item spaces. Also updates the position and size properties of the item.
					 * @param offset The start position for calculating positions.
					 * @return The deep size of the item.
					 */
					Vector updateDeepSize(Vector offset);

				public:

					Item();

					/**
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void appendChild(Item * child);

					/**
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void appendElement(FieldElement * Element);

					void collapse(bool deeply = false);

					/**
					 * Creates a list of positions within the tree field from a root to this item.
					 * @return A list of 0-indexed positions of this item.
					 * The first index is the position within the root items list,
					 * the second index is the position within the root's child items list, and so on,
					 * until this tree item is reached.
					 */
					ArrayList<Int> * createPath();

					/**
					 * Opens a popup to edit the first editable field element of this cell.
					 * This method is typically called by the list field when receiving the onEdit() event.
					 * @return The field element that is being edited now, or NULL, if no field element is editable.
					 */
					FieldElement * edit();

					/**
					 * Ensure that this item is entirely visible by setting the scroll position.
					 * If the item cannot be shown entirely, then the top-left corner is ensured to be shown.
					 * @param deeply Whether to try to show the expanded children, too.
					 */
					void ensureVisible(bool deeply = false);

					/**
					 * Ensure that this item is entirely visible by setting the scroll position,
					 * bypassing the skin effects, setting the scroll position now.
					 * If the item cannot be shown entirely, then the top-left corner is ensured to be shown.
					 * @param deeply Whether to try to show the expanded children, too.
					 */
					void ensureVisibleNow(bool deeply = false);

					void expand(bool deeply = false);

					const ArrayList<Item> & getChildren();

					Vector getDeepSize();

					/**
					 * Returns the depth of this item from its root.
					 * The root items of a tree field have a depth of 0.
					 * @return The depth of this item.
					 */
					int getDepth();

					const ArrayList<FieldElement> & getElements();

					TreeField * getField();

					Item * getParent();

					Vector getPosition();

					Vector getSize();

					/**
					 * Returns whether this item has focus in its tree field.
					 * @return Whether this item has focus in its tree field.
					 */
					bool hasFocus();

					/**
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void insertChild(int position, Item * child);

					/**
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void insertElement(int position, FieldElement * element);

					bool isExpanded();

					/**
					 * Returns whether this item is selected in its tree field.
					 * @return Whether this item is selected.
					 */
					bool isSelected();

					/**
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void prependChild(Item * child);

					/**
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void prependElement(FieldElement * Element);

					void removeChild(int position);

					void removeChildren();

					void removeElement(int position);

					void removeElements();

					/**
					 * Adds this item to the set of selected items of the tree field,
					 * if the item is not yet selected.
					 * If it does not belong to a multi select tree field, all other items are unselected.
					 */
					void select();

					void setExpanded(bool expanded, bool deeply = false);

					/**
					 * Removes this item from the set of selected items.
					 */
					void unselect();
			};

			/**
			 * The listener interface for tree field events.
			 * @note The listener concept has been designed for two clients:
			 * The main application (e.g. the game) and the skin.
			 * Do not create listeners for internal purposes.
			 * @warning Listeners are auto-deleting when they are not listening to any
			 * component anymore. So you should not hold any references to them.
			 */
			class Listener : public ScrollingField::Listener
			{
				protected:

					/**
					 * Destroys the listener.
					 * Since listeners are managed automatically,
					 * you do not need to destroy them by yourself.
					 */
					virtual ~Listener();

				public:

					/**
					 * Called when an item of the tree field has been collapsed.
					 * @param item The tree field item that has been collapsed.
					 */
					virtual void onItemCollapsed(Item * item);

					/**
					 * Called when an item of the tree field is about to be collapsed.
					 * @param item The tree field item that is about to be collapsed.
					 */
					virtual VetoMode onItemCollapsing(Item * item);

					/**
					 * Called when an item of the tree field has been expanded.
					 * @param item The tree field item that has been expanded.
					 */
					virtual void onItemExpanded(Item * item);

					/**
					 * Called when an item of the tree field is about to be expanded.
					 * @param item The tree field item that is about to be expanded.
					 */
					virtual VetoMode onItemExpanding(Item * item);

					/**
					 * Called when the selection of a tree field has just changed.
					 * @param field The tree field that changed.
					 */
					virtual void onSelectionChanged(TreeField * field);

					/**
					 * Called when the selection of a tree field is about to change.
					 * @param field The tree field that is about to change.
					 */
					virtual VetoMode onSelectionChanging(TreeField * field);
			};

			enum Type
			{
				/**
				 * The user must choose exactly one item.
				 */
				TYPE_ONE,

				/**
				 * The user must choose one item or no item at all.
				 */
				TYPE_ONE_OR_NONE,

				/**
				 * The user may choose an arbitrary number of items.
				 */
				TYPE_MANY
			};

		private:

			/**
			 * A list of 0-indexed positions of the item that currently has focus.
			 * The first index is the position within the root items list,
			 * the second index is the position within the root's child items list, and so on,
			 * until the focus item is reached.<br>
			 * If the list is empty, then no item has currently focus.
			 */
			ArrayList<Int> focusItemPath;

			/**
			 * Whether the tree field can be scrolled horizontally. Defaults to true.
			 * If not, the content is resized together with the tree field.
			 */
			bool horizontallyScrollable;

			/**
			 * The width of the indentation of child items.
			 */
			int indentation;

			/**
			 * A list of 0-indexed positions of the item where the range selection starts.
			 * The first index is the position within the root items list,
			 * the second index is the position within the root's child items list, and so on,
			 * until the range selection start item is reached.<br>
			 * If the list is empty, then no range exists.
			 */
			ArrayList<Int> rangeStartItemPath;

			/**
			 * The root items of this tree field.
			 */
			ArrayList<Item> roots;

			/**
			 * The currently selected items of this tree field.
			 */
			HashSet selectedItems;

			/**
			 * The selection type (e.g. whether to select multiple items). Defaults to TYPE_MANY.
			 */
			Type type;

			/**
			 * Whether the tree field can be scrolled vertically. Defaults to true.
			 * If not, the content is resized together with the tree field.
			 */
			bool verticallyScrollable;

		protected:

			virtual ~TreeField();

			/**
			 * Draws the content of the scrolling field to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation. This context is already translated
			 * for the current scroll position and scroll field size, so you should
			 * draw as if there is no scrolling field, and use the field size you also
			 * return in getContentSize().
			 * @see Desktop::getSkin()
			 */
			virtual void drawContent(DrawingContext & context);

			/**
			 * Returns the item found at a given position or NULL, if no item matches.
			 * @param position The position relative to the top-left corner of content field.
			 * @param items The list of items to be scanned.
			 * @return The item at the given position or NULL.
			 */
			Item * getItemAt(Vector position, const ArrayList<Item> & items);

			bool moveDownPath(ArrayList<Int> & path);

			bool moveUpPath(ArrayList<Int> & path);

			/**
			 * Notifies all listeners about an item that has collapsed.
			 * @param item The tree field item that has been collapsed.
			 */
			void notifyItemCollapsed(Item * item);

			/**
			 * Notifies all listeners about an item that has collapsed.
			 * @param item The tree field item that has been collapsed.
			 * @return Whether any listener vetoed.
			 */
			bool notifyItemCollapsing(Item * item);

			/**
			 * Notifies all listeners about an item that has expanded.
			 * @param item The tree field item that has been expanded.
			 */
			void notifyItemExpanded(Item * item);

			/**
			 * Notifies all listeners about an item that has expanded.
			 * @param item The tree field item that has been expanded.
			 * @return Whether any listener vetoed.
			 */
			bool notifyItemExpanding(Item * item);

			/**
			 * Notifies all tree field listeners that the selection of the field has changed.
			 */
			void notifySelectionChanged();

			/**
			 * Notifies all tree field listeners that the selection of the field has changed.
			 * @return Whether any listener vetoed.
			 */
			bool notifySelectionChanging();

			/**
			 * Called by the scrolling field to notify the content when the field
			 * is clicked (usually using the left mouse button).
			 * A click is raised after the mouse has been pressed and
			 * released, and no dragging operation has started.
			 * @param position The position within the content where the click
			 * occurred, relative to the upper-left corner of the content.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onContentPress()
			 * @see onContentRelease()
			 */
			virtual void onContentClick(Vector position, bool option1, bool option2);

			/**
			 * Handles a context menu click for 'collapse'.
			 * @param item The selected item.
			 */
			static void onContextMenuCollapse(Menu::Item * item);

			/**
			 * Handles a context menu click for 'collapse all'.
			 * @param item The selected item.
			 */
			static void onContextMenuCollapseAll(Menu::Item * item);

			/**
			 * Handles a context menu click for 'expand'.
			 * @param item The selected item.
			 */
			static void onContextMenuExpand(Menu::Item * item);

			/**
			 * Handles a context menu click for 'expand all'.
			 * @param item The selected item.
			 */
			static void onContextMenuExpandAll(Menu::Item * item);

			/**
			 * Internal method to add a given item and all its decendents to the selected items.
			 * @param root The root item of the branch to be added to the selection.
			 * @param collapsedToo Whether to select collapsed children, too. Defaults to false.
			 * @return Whether the selection was changed in any way.
			 */
			bool selectBranchInternal(Item * root, bool collapsedToo);

			/**
			 * Selects the current item internally, e.g. when the space key has been pressed.
			 */
			void selectCurrentItem();

			/**
			 * Internal method to remove a given item and all its decendents from the selected items.
			 * @param item The root item of the branch to be removed from the selection.
			 * @return Whether the selection was changed in any way.
			 */
			bool unselectBranchInternal(Item * root);

			void updateFocusAndRangeStartPaths();

			void updateSelectionFromFocus(bool option1, bool option2);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(TreeField);
			#endif

		public:

			TreeField();

			/**
			 * Adds items for a context-clock menu. If you leave the menu empty, none appears.
			 * This is the default. Override this method to define your own menu items.
			 * Super-call to include the default menu items of the overridden parent.
			 * @param menu The context menu.
			 * @param clickPosition The click position relative to the top-left corner of the component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return The new context menu to show, or NULL if no menu shall exists.
			 */
			virtual void addContextMenuItems(Menu * menu, Vector clickPosition, bool option1, bool option2);

			/**
			 * Adds items for a context-clock menu. If you leave the menu empty, none appears.
			 * This is the default. Override this method to define your own menu items.
			 * Super-call to include the default menu items of the overridden parent.
			 * @param menu The context menu.
			 * @param item The clicked item.
			 * @param clickPosition The click position relative to the top-left corner of the component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return The new context menu to show, or NULL if no menu shall exists.
			 */
			virtual void addItemContextMenuItems(Menu * menu, TreeField::Item * item, Vector clickPosition,
				bool option1, bool option2);

			/**
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void appendRoot(Item * Root);

			void collapseAll(bool deeply = false);

			/**
			 * Called by the scrolling field to notify the content when the field
			 * is context-clicked (usually using the right mouse button).
			 * @param position The position within the content where the context click
			 * occurred, relative to the upper-left corner of the content.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			virtual void onContentContextClick(Vector position, bool option1, bool option2);

			/**
			 * Ensure that the focus is entirely visible by setting the scroll position.
			 * If the focus cannot be shown entirely, then the top-left corner is ensured to be shown.
			 * @param deeply Whether to try to show the expanded children, too.
			 */
			void ensureFocusVisible(bool deeply = false);

			/**
			 * Ensure that the focus is entirely visible by setting the scroll position,
			 * bypassing the skin effects, setting the scroll position now.
			 * If the focus cannot be shown entirely, then the top-left corner is ensured to be shown.
			 * @param deeply Whether to try to show the expanded children, too.
			 */
			void ensureFocusVisibleNow(bool deeply = false);

			void expandAll(bool deeply = false);

			/**
			 * Returns the item that currently has focus or NULL, if no item has focus.
			 * @return The item that currently has focus or NULL.
			 */
			Item * getFocusItem();

			/**
			 * Returns a list of 0-indexed positions of the item that currently has focus.
			 * The first index is the position within the root items list,
			 * the second index is the position within the root's child items list, and so on,
			 * until the focus item is reached.<br>
			 * If the list is empty, then no item has currently focus.
			 * @return The focus item positions.
			 */
			const ArrayList<Int> & getFocusItemPath();

			/**
			 * Returns the width of the indentation of child items.
			 * @return The width of the indentation.
			 */
			int getIndentation();

			/**
			 * Returns the item found at a given path of position, starting from the root.
			 * If any index is out of range, or if the given path is empty, the method returns NULL.
			 * @param path The position relative to the top-left corner of content field.
			 * A list of 0-indexed positions of the item that is selected.
			 * The first index is the position within the root items list,
			 * the second index is the position within the root's child items list, and so on,
			 * until the requested tree item is reached.
			 * @return The item at the given position or NULL.
			 */
			Item * getItem(const ArrayList<Int> & path);

			/**
			 * Returns the item found at a given position or NULL, if no item matches.
			 * @param position The position relative to the top-left corner of content field.
			 * @return The item at the given position or NULL.
			 */
			Item * getItemAt(Vector position);

			/**
			 * Returns the root items of this tree field.
			 * @return The root items of this tree field.
			 */
			const ArrayList<Item> & getRoots();

			/**
			 * Returns the currently selected item, if exactly one item is selected.
			 * Otherwise, returns NULL.
			 * @return The currently selected item or NULL.
			 */
			Item * getSelectedItem();

			/**
			 * Returns the currently selected items of this tree field.
			 * @return The currently selected items of this tree field.
			 */
			const HashSet & getSelectedItems();

			/**
			 * Returns the selection type (e.g. whether to select multiple items). Defaults to TYPE_MANY.
			 * @return The selection type.
			 */
			Type getType();

			/**
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void insertRoot(int position, Item * root);

			/**
			 * Returns whether the tree field displays a horizontal scroller.
			 * The skin's created widgets return act and the scroller sizes
			 * should adjust according to this information. It may change during run.
			 * @return Whether the tree field displays a horizontal scroller.
			 */
			virtual bool isHorizontallyScrollable();

			/**
			 * Returns whether the tree field displays a horizontal scroller.
			 * The skin's created widgets return act and the scroller sizes
			 * should adjust according to this information. It may change during run.
			 * @return Whether the tree field displays a horizontal scroller.
			 */
			virtual bool isVerticallyScrollable();

			/**
			 * Handles the event that a regular character has been entered
			 * on the component.
			 * This is used for text input fields to enter text.
			 * @param character The character pressed, using ISO-8859-1 encoding.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onCharacter(char character, bool option1, bool option2);

			/**
			 * Handles the event that the EDIT key is pressed on the event target.
			 * On Windows systems, this is the F2 key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onEdit();

			/**
			 * Handles the event this an element of this field has changed.
			 * By default, invalidates the field.
			 * @param element The element that changed.
			 */
			virtual void onElementChanged(FieldElement * element);

			/**
			 * Handles the event that the DOWN key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveDown(bool option1, bool option2);

			/**
			 * Handles the event that the LEFT key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveLeft(bool option1, bool option2);

			/**
			 * Handles the event that the RIGHT key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveRight(bool option1, bool option2);

			/**
			 * Handles the event that the END key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveToEnd(bool option1, bool option2);

			/**
			 * Handles the event that the START (HOME, POS1, ...) key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveToStart(bool option1, bool option2);

			/**
			 * Handles the event that the UP key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onMoveUp(bool option1, bool option2);

			/**
			 * Handles the event that the PAGE DOWN key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onPageDown(bool option1, bool option2);

			/**
			 * Handles the event that the PAGE UP key is pressed on the event target.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onPageUp(bool option1, bool option2);

			/**
			 * Handles the event that the user requested to select all
			 * (e.g. by pressing Ctrl+A on Windows systems).
			 * @return Whether the event target has consumed the event.
			 * If false, the event is first passed to the desktop
			 * and then to the input manager to increase event quantities.
			 */
			virtual bool onSelectAll();

			/**
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void prependRoot(Item * Root);

			void removeRoot(int position);

			void removeRoots();

			void selectAll(bool collapsedToo = false);

			/**
			 * Adds a given item and all its decendents to the selected items.
			 * @param root The root item of the branch to be added to the selection.
			 * @param collapsedToo Whether to select collapsed children, too. Defaults to false.
			 */
			void selectBranch(Item * root, bool collapsedToo = false);

			/**
			 * Adds an item to the set of selected items, if the item is not yet selected.
			 * If this is no multi select tree field, all other rows are unselected.
			 * @param item The item to be selected.
			 * This item must be part of the items of the tree field.
			 */
			void selectItem(Item * item);

			/**
			 * Sets the tree item that has focus.
			 * @param item The item that has focus.
			 */
			void setFocusItem(Item * item);

			/**
			 * Sets whether the tree field can be scrolled horizontally. Defaults to true.
			 * If not, the content is resized together with the tree field.
			 * @param horizontallyScrollable Whether the tree field can be scrolled horizontally.
			 */
			void setHorizontallyScrollable(bool horizontallyScrollable);

			/**
			 * Sets the width of the indentation of child items.
			 * @param indentation The width of the indentation.
			 */
			void setIndentation(int indentation);

			/**
			 * Sets that a given item is solely selected.
			 * @param item The item to be selected.
			 * This item must be part of the item of the tree field,
			 * otherwise it will not be selected.
			 */
			void setSelectedItem(Item * item);

			/**
			 * Sets the selected items of this tree field.
			 * @param selectedItems The selected items.
			 * These items must be part of the items of the tree field,
			 * otherwise it will not be selected.
			 */
			void setSelectedItems(const HashSet & selectedItems);

			/**
			 * Sets the selection type (e.g. whether to select multiple items). Defaults to TYPE_MANY.
			 * @param type The selection type.
			 */
			void setType(Type type);

			/**
			 * Sets whether the tree field can be scrolled vertically. Defaults to true.
			 * If not, the content is resized together with the tree field.
			 * @param verticallyScrollable Whether the tree field can be scrolled vertically.
			 */
			void setVerticallyScrollable(bool verticallyScrollable);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();

			/**
			 * Removes a given item and all its decendents from the selected items.
			 * @param item The root item of the branch to be removed from the selection.
			 */
			void unselectBranch(Item * root);

			/**
			 * Removes an item from the set of selected items.
			 * @param item The item to be unselected.
			 */
			void unselectItem(Item * item);

			/**
			 * Removes all item from the set of selected items.
			 */
			void unselectItems();

			/**
			 * Calculates the size of the content, as if there was no scrolling field
			 * and the field must display its entire content at once, and puts the
			 * value into the contentSize attributes.
			 * The scrolling field automatically adjusts the scrollers according to
			 * this value.
			 * @note This method should be called when the content changes in a way
			 * that might change the its size.
			 * @see contentSize
			 */
			virtual void updateContentSize();
	};
}


#endif
