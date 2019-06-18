/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ListField::ListField()
{
	ASSERTION_COBJECT(this);

	setComponentParent(&rowsField);
	rowsField.setBorderSize(Vector());

	columnsMovable = false;
	columnsResizable = true;
	focusColumnNo = 0;
	focusRowNo = 0;
	headlined = true;
	horizontallyScrollable = true;
	rangeStartRowNo = 0;
	rowsRemovable = true;
	type = TYPE_MANY;
	verticallyScrollable = true;

	adjustRowsField();

	setMinimumSize(Vector(128, 64));

	Desktop::getInstance()->getSkin()->setupListField(this);
}

ListField::~ListField()
{
	ASSERTION_COBJECT(this);
	selectedRows.RemoveAll();
}

void ListField::addColumnContextMenuItems(Menu * menu, Column * column, Vector position,
	bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ListField::addContextMenuItems(Menu * menu, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	position += Vector(rowsField.getScrollPositionNow().x, 0);
	int columnNo;
	getHeadlineColumnForPosition(position, columnNo);

	Column * column = getColumn(columnNo);
	if (column != NULL)
		addColumnContextMenuItems(menu, column, position, option1, option2);
}

void ListField::addRowsContextMenuItems(Menu * menu, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
}

void ListField::adjustAutoColumns()
{
	ASSERTION_COBJECT(this);

	int contentViewWidth = rowsField.getContentViewSize().x;
	int autoColumns = 0;
	for (int i=0; i<columns.GetCount(); i++)
		if (columns.Get(i)->isVisible() && columns.Get(i)->isAutoResizing())
			autoColumns++;
		else
			contentViewWidth -= columns.Get(i)->getActualWidth();

	if (contentViewWidth < 0)
		contentViewWidth = 0;

	for (int i=0; i<columns.GetCount(); i++)
		if (columns.Get(i)->isVisible() && columns.Get(i)->isAutoResizing())
		{
			int width = contentViewWidth / autoColumns;
			columns.Get(i)->setWidth(width);

			contentViewWidth -= width;
			autoColumns--;
		}
}

void ListField::adjustRowsField()
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	int headingHeight = headlined ? skin->getListFieldHeadingHeight(this) : 0;

	Vector clientPosition = Vector(0, headingHeight);
	Vector clientSize = getSize() - clientPosition;
	rowsField.moveInternal(clientPosition, false);
	rowsField.resizeInternal(clientSize, false);
}

void ListField::appendColumn(ListField::Column * column)
{
	ASSERTION_COBJECT(this);
	insertColumn(columns.GetCount(), column);
}

void ListField::appendRow(ListField::Row * row)
{
	ASSERTION_COBJECT(this);
	insertRow(rows.GetCount(), row);
}

void ListField::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	skin->drawListField(DrawingContext(context, Vector(-rowsField.getScrollPositionNow().x, 0)), this);

	rowsField.draw(DrawingContext(context, &rowsField));
}

void ListField::ensureCellVisible(int rowNo, int columnNo, bool now)
{
	ASSERTION_COBJECT(this);

	if (columns.IsEmpty() || rows.IsEmpty())
		return;

	if (rowNo < 0)
		rowNo = 0;
	if (rowNo > rows.GetCount() - 1)
		rowNo = rows.GetCount() - 1;
	if (columnNo < 0)
		columnNo = 0;
	if (columnNo > columns.GetCount() - 1)
		columnNo = columns.GetCount() - 1;

	Skin * skin = Desktop::getInstance()->getSkin();
	int columnBorderWidth = skin->getListFieldColumnBorderWidth(this);
	int rowHeight = skin->getListFieldRowHeight(this);

	int columnPos = 0;
	for (int i = 0; i < columnNo; i++)
	{
		ListField::Column * column = columns.Get(i);
		if (column->isVisible())
			columnPos += column->getWidth() + columnBorderWidth;
	}
	int rowPos = rowNo * rowHeight;

	Vector topLeftVector(columnPos, rowPos);
	Vector bottomRightVector(columnPos + columns.Get(columnNo)->getActualWidth() - 1,
		rowPos + rowHeight - 1);

	int viewHeight = rowsField.getContentViewSize().y;
	int halfViewHeightWithoutFocus = (viewHeight - rowHeight) / 2;
	if (rowHeight * 4 > halfViewHeightWithoutFocus)
	{
		topLeftVector.y -= halfViewHeightWithoutFocus;
		bottomRightVector.y += halfViewHeightWithoutFocus;
	}
	else
	{
		topLeftVector.y -= rowHeight * 4;
		bottomRightVector.y += rowHeight * 4;
	}

	rowsField.ensureVisible(topLeftVector, bottomRightVector);
	if (now)
		rowsField.ensureVisibleNow(topLeftVector, bottomRightVector);
}

void ListField::ensureFocusVisible(bool now)
{
	ASSERTION_COBJECT(this);
	ensureCellVisible(getFocusRowNo(), getFocusColumnNo(), now);
}

void ListField::finishRowRemoval(bool selectionChanged)
{
	ASSERTION_COBJECT(this);

	if (rows.IsEmpty())
	{
		focusRowNo = 0;
		rangeStartRowNo = 0;
	}
	else
	{
		if (type == TYPE_ONE && selectedRows.IsEmpty())
		{
			Row * row = rows.Get(focusRowNo);
			if (row != NULL)
			{
				selectedRows.Add(row);
				selectionChanged = true;
			}
		}
	}

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			listFieldListener->onRowsRemoved(this);
	}

	rowsField.updateContentSize();
	ensureFocusVisible();
	if (selectionChanged)
		notifySelectionChanged();
	invalidate();
}

ListField::Cell * ListField::getCell(int rowNo, int cellNo)
{
	ASSERTION_COBJECT(this);

	ListField::Row * row = rows.Get(rowNo);
	return row != NULL ? row->getCell(cellNo) : NULL;
}

ListField::Column * ListField::getColumn(int columnNo)
{
	ASSERTION_COBJECT(this);
	return columns.Get(columnNo);
}

void ListField::getColumnAndRowForPosition(Vector position,
	int & columnNo, int & columnPos, int & rowNo)
{
	ASSERTION_COBJECT(this);

	if (rowsField.isVisible() && position >= rowsField.getPosition()
		&& position < rowsField.getPosition() + rowsField.getSize())
	{
		Vector positionInRowsField = position + rowsField.getScrollPositionNow();
		rowsField.getColumnAndRowForPosition(positionInRowsField, columnNo, columnPos, rowNo);
	}
	else
	{
		columnNo = -1;
		columnPos = -1;
		rowNo = -1;
	}
}

int ListField::getColumnCount()
{
	ASSERTION_COBJECT(this);
	return columns.GetCount();
}

int ListField::getColumnNo(ListField::Column * column)
{
	ASSERTION_COBJECT(this);

	for (int columnNo = 0; columnNo < columns.GetCount(); columnNo++)
		if (columns.Get(columnNo) == column)
			return columnNo;

	return -1;
}

const ArrayList<ListField::Column> & ListField::getColumns()
{
	ASSERTION_COBJECT(this);
	return columns;
}

EventTarget * ListField::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = InputField::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	if (rowsField.isVisible() && position >= rowsField.getPosition()
		&& position < rowsField.getPosition() + rowsField.getSize())
		return rowsField.getEventTargetAt(position - rowsField.getPosition());

	return this;
}

ListField::Cell * ListField::getFocusCell()
{
	ASSERTION_COBJECT(this);

	ListField::Row * focusRow = getFocusRow();
	if (focusRow == NULL)
		return NULL;

	return focusRow->getCell(focusColumnNo);
}

Component * ListField::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return &rowsField;
}

ListField::Column * ListField::getFocusColumn()
{
	ASSERTION_COBJECT(this);
	return columns.Get(focusColumnNo);
}

int ListField::getFocusColumnNo()
{
	ASSERTION_COBJECT(this);
	return focusColumnNo;
}

ListField::Row * ListField::getFocusRow()
{
	ASSERTION_COBJECT(this);
	return rows.Get(focusRowNo);
}

int ListField::getFocusRowNo()
{
	ASSERTION_COBJECT(this);
	return focusRowNo;
}

void ListField::getHeadlineColumnForPosition(Vector position, int & columnNo)
{
	ASSERTION_COBJECT(this);

	if (!headlined || position.y <= 0 || position.y >= rowsField.getPosition().y)
	{
		columnNo = -1;
		return;
	}

	Skin * skin = Desktop::getInstance()->getSkin();
	int columnBorderWidth = skin->getListFieldColumnBorderWidth(this);

	int columnPos = 0;
	for (columnNo = 0; columnNo < columns.GetCount(); columnNo++)
	{
		if (!columns.Get(columnNo)->isVisible())
			continue;

		int columnWidth = columns.Get(columnNo)->getWidth();
		if (position.x < columnPos + columnWidth + columnBorderWidth / 2)
			break;

		columnPos += columnWidth + columnBorderWidth;
	}

	if (columnNo < 0) columnNo = -1;
	if (columnNo > columns.GetCount()) columnNo = columns.GetCount();
}

ListField::Row * ListField::getRow(int rowNo)
{
	ASSERTION_COBJECT(this);
	return rows.Get(rowNo);
}

int ListField::getRowCount()
{
	ASSERTION_COBJECT(this);
	return rows.GetCount();
}

int ListField::getRowNo(ListField::Row * row)
{
	ASSERTION_COBJECT(this);

	if (row == NULL)
		return -1;

	for (int rowNo = 0; rowNo < rows.GetCount(); rowNo++)
		if (rows.Get(rowNo) == row)
			return rowNo;

	return -1;
}

int ListField::getRowNoByModel(void * model)
{
	ASSERTION_COBJECT(this);

	for (int rowNo = 0; rowNo < rows.GetCount(); rowNo++)
		if (rows.Get(rowNo)->getModel() == model)
			return rowNo;

	return -1;
}

const ArrayList<ListField::Row> & ListField::getRows()
{
	ASSERTION_COBJECT(this);
	return rows;
}

ListField::RowsField * ListField::getRowsField()
{
	ASSERTION_COBJECT(this);
	return &rowsField;
}

int ListField::getRowWidth()
{
	ASSERTION_COBJECT(this);

	if (columns.IsEmpty())
		return 0;

	Skin * skin = Desktop::getInstance()->getSkin();
	int columnBorderWidth = skin->getListFieldColumnBorderWidth(this);

	int rowWidth = 0;
	bool first = true;
	for (int i=0; i<columns.GetCount(); i++)
		if (columns.Get(i)->isVisible())
		{
			rowWidth += columns.Get(i)->getWidth();
			if (first)
				first = false;
			else
				rowWidth += columnBorderWidth;
		}

	return rowWidth;
}

ListField::Row * ListField::getSelectedRow()
{
	ASSERTION_COBJECT(this);

	if (selectedRows.GetCount() != 1)
		return NULL;

	PointeredList * rowsList = selectedRows.GetAll();
	Row * row = (Row *) rowsList->GetFirst()->GetData();
	delete rowsList;

	return row;
}

const HashSet & ListField::getSelectedRows()
{
	ASSERTION_COBJECT(this);
	return selectedRows;
}

ArrayList<ListField::Row> * ListField::getSelectedRowsInOrder()
{
	ASSERTION_COBJECT(this);

	ArrayList<ListField::Row> * selectedArray;
	if ((selectedArray = new ArrayList<ListField::Row>) == NULL)
		throw EOUTOFMEMORY;

	for (int rowNo=0; rowNo<rows.GetCount(); rowNo++)
	{
		ListField::Row * row = rows.Get(rowNo);
		if (selectedRows.Contains(row))
			selectedArray->Append(row);
	}

	return selectedArray;
}

ListField::Type ListField::getType()
{
	ASSERTION_COBJECT(this);
	return type;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ListField, ScrollingField);
#endif

void ListField::insertColumn(int position, ListField::Column * column)
{
	ASSERTION_COBJECT(this);

	if (column->getField() != NULL)
		throw EILLEGALSTATE("The column is already contained in a list field");
	if (position < 0 || position > columns.GetCount())
		throw EILLEGALSTATE("The column position is out of range");

	if (focusColumnNo >= position && focusColumnNo < columns.GetCount())
		focusColumnNo++;

	column->setField(this);
	columns.Insert(column, position);

	rowsField.updateContentSize();
	adjustAutoColumns();
	invalidate();

	notifyColumnAdded(position);
}

void ListField::insertRow(int position, ListField::Row * row)
{
	ASSERTION_COBJECT(this);

	if (row->getField() != NULL)
		throw EILLEGALSTATE("The row is already contained in a list field");
	if (position < 0 || position > rows.GetCount())
		throw EILLEGALSTATE("The row position is out of range");

	bool wasEmpty = rows.IsEmpty();

	if (focusRowNo >= position && focusRowNo < rows.GetCount())
		focusRowNo++;
	if (rangeStartRowNo >= position && rangeStartRowNo < rows.GetCount())
		rangeStartRowNo++;

	row->setField(this);
	rows.Insert(row, position);

	if (type == TYPE_ONE && wasEmpty)
		selectedRows.Add(row);

	rowsField.updateContentSize();
	invalidate();

	notifyRowAdded(position);

	if (type == TYPE_ONE && wasEmpty)
		notifySelectionChanged();
}

bool ListField::isColumnsMovable()
{
	ASSERTION_COBJECT(this);
	return columnsMovable;
}

bool ListField::isColumnsResizable()
{
	ASSERTION_COBJECT(this);
	return columnsResizable;
}

bool ListField::isEmpty()
{
	ASSERTION_COBJECT(this);
	return rows.IsEmpty();
}

bool ListField::isHeadlined()
{
	ASSERTION_COBJECT(this);
	return headlined;
}

bool ListField::isHorizontallyScrollable()
{
	ASSERTION_COBJECT(this);
	return horizontallyScrollable;
}

bool ListField::isRowSelected(int rowNo)
{
	ASSERTION_COBJECT(this);
	return selectedRows.Contains(rows.Get(rowNo));
}

bool ListField::isRowSelected(Row * row)
{
	ASSERTION_COBJECT(this);
	return selectedRows.Contains(row);
}

bool ListField::isRowsRemovable()
{
	ASSERTION_COBJECT(this);
	return rowsRemovable;
}

bool ListField::isVerticallyScrollable()
{
	ASSERTION_COBJECT(this);
	return verticallyScrollable;
}

void ListField::moveRow(int fromPosition, int toPosition)
{
	ASSERTION_COBJECT(this);

	if (fromPosition < 0 || fromPosition >= rows.GetCount()
		|| toPosition < 0 || toPosition > rows.GetCount()
		|| fromPosition == toPosition)
		return;

	rows.Insert(rows.Unlink(fromPosition), toPosition);
	invalidate();
}

void ListField::notifyColumnAdded(int columnNo)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			listFieldListener->onColumnAdded(this, columnNo);
	}
}

void ListField::notifyColumnRemoved()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			listFieldListener->onColumnRemoved(this);
	}
}

void ListField::notifyColumnRemoving(int columnNo)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			listFieldListener->onColumnRemoving(this, columnNo);
	}
}

void ListField::notifyRowAdded(int rowNo)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			listFieldListener->onRowAdded(this, rowNo);
	}
}

bool ListField::notifyRowsRemoving(int * rowNos, int rowCount)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			if (listFieldListener->onRowsRemoving(this, rowNos, rowCount) == VETO)
				return false;
	}

	return true;
}

void ListField::notifySelectionChanged()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			listFieldListener->onSelectionChanged(this);
	}
}

void ListField::onCellHover(int rowNo, int columnNo, int elementNo)
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			listFieldListener->onCellHover(this, rowNo, columnNo, elementNo);
	}
}

void ListField::onChildMoved(Component * child, Vector oldPosition)
{
	ASSERTION_COBJECT(this);

	InputField::onChildMoved(child, oldPosition);
	moveInternal(getPosition() + child->getPosition() - oldPosition, true);
	moveComponent(child, oldPosition);
}

void ListField::onChildMaximumSizeChanged(Component * child, Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	InputField::onChildMaximumSizeChanged(child, oldMaximumSize);
	setMaximumSize(getMaximumSize() + child->getMaximumSize() - oldMaximumSize);
}

void ListField::onChildMinimumSizeChanged(Component * child, Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	InputField::onChildMinimumSizeChanged(child, oldMinimumSize);
	setMinimumSize(getMinimumSize() + child->getMinimumSize() - oldMinimumSize);
}

void ListField::onChildResized(Component * child, Vector oldSize)
{
	ASSERTION_COBJECT(this);

	InputField::onChildResized(child, oldSize);
	Vector delta = child->getSize() - oldSize;
	resizeInternal(getSize() + delta, true);
}

void ListField::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onClick(position, option1, option2);

	position += Vector(rowsField.getScrollPositionNow().x, 0);
	int columnNo;
	getHeadlineColumnForPosition(position, columnNo);

	if (columnNo >= 0 && columnNo < columns.GetCount())
	{
		int listenersCount = listeners.GetCount();
		for (int i=0; i<listenersCount; i++)
		{
			ListField::Listener * listFieldListener
				= dynamic_cast<ListField::Listener *>(listeners.Get(i));
			if (listFieldListener != NULL)
				listFieldListener->onColumnClicked(this, columnNo);
		}
	}
}

DraggingSurrogate * ListField::onDragStart(int rowNo)
{
	ASSERTION_COBJECT(this);
	return NULL;
}

DraggingSurrogate * ListField::onDragStart(int rowNo, int columnNo)
{
	ASSERTION_COBJECT(this);
	return NULL;
}

void ListField::onHover()
{
	ASSERTION_COBJECT(this);

	EventTarget::onHover();

	Vector position = Desktop::getInstance()->getPointerPosition() - getAbsolutePosition();
	int columnNo;
	getHeadlineColumnForPosition(position, columnNo);

	if (columnNo >= 0 && columnNo < columns.GetCount())
	{
		int listenersCount = listeners.GetCount();
		for (int i=0; i<listenersCount; i++)
		{
			ListField::Listener * listFieldListener
				= dynamic_cast<ListField::Listener *>(listeners.Get(i));
			if (listFieldListener != NULL)
				listFieldListener->onHeadlineHover(this, columnNo);
		}
	}
}

void ListField::onMaximumSizeChanged(Vector oldMaximumSize)
{
	ASSERTION_COBJECT(this);

	InputField::onMaximumSizeChanged(oldMaximumSize);

	Skin * skin = Desktop::getInstance()->getSkin();
	int headingHeight = headlined ? skin->getListFieldHeadingHeight(this) : 0;
	rowsField.setMaximumSizeInternal(getMaximumSize() - Vector(0, headingHeight), false);
}

void ListField::onMinimumSizeChanged(Vector oldMinimumSize)
{
	ASSERTION_COBJECT(this);

	InputField::onMinimumSizeChanged(oldMinimumSize);

	Skin * skin = Desktop::getInstance()->getSkin();
	int headingHeight = headlined ? skin->getListFieldHeadingHeight(this) : 0;
	rowsField.setMinimumSizeInternal(getMinimumSize() - Vector(0, headingHeight), false);
}

void ListField::prependColumn(ListField::Column * column)
{
	ASSERTION_COBJECT(this);
	insertColumn(0, column);
}

void ListField::prependRow(ListField::Row * row)
{
	ASSERTION_COBJECT(this);
	insertRow(0, row);
}

void ListField::removeColumn(int position)
{
	ASSERTION_COBJECT(this);

	notifyColumnRemoving(position);

	for (int i=0; i<rows.GetCount(); i++)
		rows.Get(i)->removeCell(position);

	columns.Delete(position);

	if (columns.IsEmpty())
		focusColumnNo = 0;
	else if (focusColumnNo > columns.GetCount() - 1)
		focusColumnNo = columns.GetCount() - 1;

	rowsField.updateContentSize();
	adjustAutoColumns();
	ensureFocusVisible();
	invalidate();

	notifyColumnRemoved();
}

void ListField::removeColumns()
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<columns.GetCount(); i++)
		notifyColumnRemoving(i);

	for (int i=0; i<rows.GetCount(); i++)
		rows.Get(i)->removeCells();

	columns.DeleteAll();

	focusColumnNo = 0;

	rowsField.updateContentSize();
	adjustAutoColumns();
	ensureFocusVisible();
	invalidate();

	notifyColumnRemoved();
}

bool ListField::removeRow(int position)
{
	ASSERTION_COBJECT(this);

	if (position < 0 || position >= rows.GetCount())
		return false;

	int positions[1];
	positions[0] = position;
	if (!notifyRowsRemoving(positions, 1))
		return false;

	Row * row = getRow(position);
	bool selectionChanged;
	if (selectedRows.Contains(row))
	{
		selectedRows.Remove(row);
		selectionChanged = true;
	}
	else
		selectionChanged = false;

	rows.Delete(position);
	if (focusRowNo > position)
		focusRowNo--;
	if (rangeStartRowNo > position)
		rangeStartRowNo--;
	finishRowRemoval(selectionChanged);

	return true;
}

bool ListField::removeRows()
{
	ASSERTION_COBJECT(this);

	int * positions;
	if ((positions = new int[rows.GetCount()]) == NULL)
		throw EOUTOFMEMORY;

	for (int i=0; i<rows.GetCount(); i++)
		positions[i] = i;

	if (!notifyRowsRemoving(positions, rows.GetCount()))
	{
		delete positions;
		return false;
	}
	delete positions;

	bool selectionChanged;
	if (!selectedRows.IsEmpty())
	{
		selectedRows.RemoveAll();
		selectionChanged = true;
	}
	else
		selectionChanged = false;

	rows.DeleteAll();
	finishRowRemoval(selectionChanged);

	return true;
}

bool ListField::removeSelectedRows()
{
	ASSERTION_COBJECT(this);

	int selectedRowCount = selectedRows.GetCount();

	int * selectedRowNos;
	if ((selectedRowNos = new int[selectedRowCount]) == NULL)
		throw EOUTOFMEMORY;

	PointeredList * selectedRowsList = selectedRows.GetAll();
	PointeredListItem * selectedRowsListItem = selectedRowsList->GetFirst();
	for (int i=0; selectedRowsListItem != NULL; i++)
	{
		Row * row = (Row *) selectedRowsListItem->GetData();
		selectedRowNos[i] = rows.Find(row);
		selectedRowsListItem = selectedRowsListItem->GetNext();
	}
	delete selectedRowsList;

	if (!notifyRowsRemoving(selectedRowNos, selectedRowCount))
	{
		delete selectedRowNos;
		return false;
	}

	bool selectionChanged = false;
	for (int i=0; i<selectedRowCount; i++)
	{
		int rowNo = selectedRowNos[i];
		Row * row = getRow(rowNo);
		if (selectedRows.Contains(row))
		{
			selectedRows.Remove(row);
			selectionChanged = true;
		}

		if (rowNo >= 0)
		{
			rows.Delete(rowNo);
			if (focusRowNo > rowNo)
				focusRowNo--;
			if (rangeStartRowNo > rowNo)
				rangeStartRowNo--;
		}
	}
	delete selectedRowNos;

	finishRowRemoval(selectionChanged);

	return true;
}

bool ListField::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	if (InputField::resizeInternal(size, notifyParent))
	{
		adjustRowsField();
		adjustAutoColumns();

		return true;
	}
	else
		return false;
}

BalloonPopup * ListField::showBalloonPopup(const String & text)
{
	ASSERTION_COBJECT(this);
	return InputField::showBalloonPopup(text);
}

BalloonPopup * ListField::showBalloonPopup(int rowNo, int columnNo, const String & text)
{
	ASSERTION_COBJECT(this);
	return showBalloonPopup(rowNo, columnNo, -1, text);
}

BalloonPopup * ListField::showBalloonPopup(int rowNo, int columnNo, int elementNo, const String & text)
{
	ASSERTION_COBJECT(this);

	if (columns.IsEmpty() || rows.IsEmpty())
		return NULL;

	Cell * cell = getCell(rowNo, columnNo);
	if (cell == NULL)
		return NULL;

	ensureCellVisible(rowNo, columnNo);

	Skin * skin = Desktop::getInstance()->getSkin();
	int columnBorderWidth = skin->getListFieldColumnBorderWidth(this);
	int rowHeight = skin->getListFieldRowHeight(this);

	int columnPos = 0;
	if (columnNo > 0)
	{
		columnPos += columnBorderWidth * (columnNo - 1);
		for (int i = 0; i < columnNo; i++)
			columnPos += columns.Get(i)->getActualWidth();
	}
	int rowPos = rowNo * rowHeight;

	const ArrayList<FieldElement> & elements = cell->getElements();
	Vector fromPosition = rowsField.getAbsolutePosition() - rowsField.getScrollPosition() + Vector(columnPos, rowPos);
	Vector toPosition;
	if (elementNo >= 0 && elementNo < elements.GetCount())
	{
		fromPosition += Vector(skin->getListFieldColumnPaddingWidth(this), 0);
		for (int i=0; i<elementNo; i++)
			fromPosition += Vector(elements.Get(i)->getSize().x, 0);

		toPosition = fromPosition + Vector(elements.Get(elementNo)->getSize().x - 1, rowHeight - 1);
	}
	else
		toPosition = fromPosition + Vector(columns.Get(columnNo)->getActualWidth() - 1, rowHeight - 1);

	BalloonPopup * popup;
	if ((popup = new BalloonPopup) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addPopup(popup);

	popup->setMessage(text);
	popup->pointTo(fromPosition, toPosition);

	return popup;
}

void ListField::selectAllRows()
{
	ASSERTION_COBJECT(this);

	if (type != TYPE_MANY)
		return;

	selectedRows.RemoveAll();
	for (int i=0; i<rows.GetCount(); i++)
		selectedRows.Add(rows.Get(i));

	notifySelectionChanged();
	invalidate();
}

void ListField::selectRow(int rowNo)
{
	ASSERTION_COBJECT(this);

	Row * row = rows.Get(rowNo);
	if (row != NULL)
		selectRow(row);
}

void ListField::selectRow(Row * row)
{
	ASSERTION_COBJECT(this);

	if (!selectedRows.Contains(row))
	{
		if (type != TYPE_MANY)
		{
			selectedRows.RemoveAll();
			this->focusRowNo = getRowNo(row);
			ensureFocusVisible();
		}

		selectedRows.Add(row);

		notifySelectionChanged();
		invalidate();
	}
}

void ListField::setActive(bool active)
{
	ASSERTION_COBJECT(this);

	InputField::setActive(active);

	// Propagate to rows field
	rowsField.setActive(active);
}

void ListField::setColumnsMovable(bool columnsMovable)
{
	ASSERTION_COBJECT(this);

	this->columnsMovable = columnsMovable;
	invalidate();
}

void ListField::setColumnsResizable(bool columnsResizable)
{
	ASSERTION_COBJECT(this);

	this->columnsResizable = columnsResizable;
	invalidate();
}

void ListField::setFocusColumnNo(int focusColumnNo)
{
	ASSERTION_COBJECT(this);

	if (focusColumnNo < 0)
		focusColumnNo = 0;
	else if (focusColumnNo >= columns.GetCount() - 1)
		focusColumnNo = columns.GetCount() - 1;

	if (this->focusColumnNo == focusColumnNo)
		return;

	this->focusColumnNo = focusColumnNo;

	ensureFocusVisible();
	invalidate();
}

void ListField::setFocusRowNo(int focusRowNo)
{
	ASSERTION_COBJECT(this);

	if (focusRowNo < 0)
		focusRowNo = 0;
	else if (focusRowNo >= rows.GetCount() - 1)
		focusRowNo = rows.GetCount() - 1;

	if (this->focusRowNo == focusRowNo)
		return;

	this->focusRowNo = focusRowNo;

	if (type == TYPE_ONE)
	{
		selectedRows.RemoveAll();

		Row * row = rows.Get(focusRowNo);
		if (row != NULL)
		{
			selectedRows.Add(row);
			notifySelectionChanged();
		}
	}

	ensureFocusVisible();
	invalidate();
}

void ListField::setHeadlined(bool headlined)
{
	ASSERTION_COBJECT(this);

	this->headlined = headlined;
	adjustRowsField();
	ensureFocusVisible();
	invalidate();
}

void ListField::setHorizontallyScrollable(bool horizontallyScrollable)
{
	ASSERTION_COBJECT(this);

	this->horizontallyScrollable = horizontallyScrollable;
	rowsField.updateContentSize();
	ensureFocusVisible();
	invalidate();
}

void ListField::setRowsRemovable(bool rowsRemovable)
{
	ASSERTION_COBJECT(this);
	this->rowsRemovable = rowsRemovable;
}

void ListField::setSelectedRow(int rowNo)
{
	ASSERTION_COBJECT(this);
	setSelectedRow(rows.Get(rowNo));
}

void ListField::setSelectedRow(Row * row)
{
	ASSERTION_COBJECT(this);

	if (selectedRows.GetCount() <= 1)
	{
		Row * selectedRow = NULL;
		if (!selectedRows.IsEmpty())
		{
			PointeredList * selectedRowsList = selectedRows.GetAll();
			PointeredListItem * selectedRowsListItem = selectedRowsList->GetFirst();
			Row * selectedRow = selectedRowsListItem != NULL ? (Row *) selectedRowsListItem->GetData() : NULL;
			delete selectedRowsList;
		}

		if (selectedRow == row)
			return;
	}

	selectedRows.RemoveAll();
	if (row != NULL && rows.Contains(row))
	{
		selectedRows.Add(row);

		if (type != TYPE_MANY)
		{
			int focusRowNo = getRowNo(row);
			if (this->focusRowNo != focusRowNo)
			{
				this->focusRowNo = focusRowNo;
				ensureFocusVisible();
			}
		}
	}

	notifySelectionChanged();
	invalidate();
}

void ListField::setSelectedRows(const HashSet & selectedRows)
{
	ASSERTION_COBJECT(this);

	if (this->selectedRows.GetCount() == selectedRows.GetCount())
	{
		PointeredList * selectedRowsList = this->selectedRows.GetAll();
		PointeredListItem * selectedRowsListItem = selectedRowsList->GetFirst();
		while (selectedRowsListItem != NULL)
		{
			Row * selectedRow = (Row *) selectedRowsListItem->GetData();
			if (!selectedRows.Contains(selectedRow))
				break;

			selectedRowsListItem = selectedRowsListItem->GetNext();
		}
		delete selectedRowsList;

		if (selectedRowsListItem == NULL)
			return;
	}

	this->selectedRows.RemoveAll();

	if (!selectedRows.IsEmpty())
	{
		PointeredList * selectedRowsList = selectedRows.GetAll();
		PointeredListItem * selectedRowsListItem = selectedRowsList->GetFirst();
		while (selectedRowsListItem != NULL)
		{
			Row * selectedRow = (Row *) selectedRowsListItem->GetData();
			this->selectedRows.Add(selectedRow);

			// Use only one, if type is not many, but focus it
			if (type != TYPE_MANY)
			{
				this->focusRowNo = getRowNo(selectedRow);
				ensureFocusVisible();

				break;
			}

			selectedRowsListItem = selectedRowsListItem->GetNext();
		}
		delete selectedRowsList;
	}

	notifySelectionChanged();
	invalidate();
}

void ListField::setType(Type type)
{
	ASSERTION_COBJECT(this);

	if (this->type == type)
		return;

	this->type = type;

	if (type == TYPE_ONE)
	{
		selectedRows.RemoveAll();

		Row * focusRow = getFocusRow();
		if (focusRow != NULL)
			selectedRows.Add(focusRow);

		notifySelectionChanged();
	}
	else if (type == TYPE_ONE_OR_NONE)
	{
		if (selectedRows.GetCount() > 1)
		{
			selectedRows.RemoveAll();
			notifySelectionChanged();
		}
		else if (selectedRows.GetCount() == 1)
		{
			this->focusRowNo = getRowNo(getSelectedRow());
			ensureFocusVisible();
		}
	}

	invalidate();
}

void ListField::setVerticallyScrollable(bool verticallyScrollable)
{
	ASSERTION_COBJECT(this);

	this->verticallyScrollable = verticallyScrollable;
	rowsField.updateContentSize();
	ensureFocusVisible();
	invalidate();
}

void ListField::sortRows(int (*compare)(Row * row1, Row * row2))
{
	ASSERTION_COBJECT(this);

	rows.Sort((int (*)(const Row * row1, const Row * row2)) compare);
	invalidate();
}

String ListField::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("ListField(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}

void ListField::tutorialClick(int rowNo, int columnNo,
	PointerEffect::ButtonEffect buttonEffect, bool option1, bool option2, long time)
{
	ASSERTION_COBJECT(this);
	tutorialClick(rowNo, columnNo, -1, buttonEffect, option1, option2, time);
}

void ListField::tutorialClick(int rowNo, int columnNo, int elementNo,
	PointerEffect::ButtonEffect buttonEffect, bool option1, bool option2, long time)
{
	ASSERTION_COBJECT(this);

	if (columns.IsEmpty() || rows.IsEmpty())
		return;

	Cell * cell = getCell(rowNo, columnNo);
	if (cell == NULL)
		return;

   if (!Desktop::getInstance()->isTutorialMode())
		throw EILLEGALSTATE("Use the tutorial methods in Tutorial::run() implementations only");

	if (buttonEffect == PointerEffect::BUTTONEFFECT_DRAGDROP)
		throw EILLEGALARGUMENT("tutorialClick cannot have a drag-drop button effect. Use tutorialDragDropTo instead.");

	if (time < 0)
		time = 1000;

	ensureCellVisible(rowNo, columnNo);

	while (rowsField.getScrollPosition() != rowsField.getScrollPositionNow())
		Desktop::getInstance()->runOnce();

	Skin * skin = Desktop::getInstance()->getSkin();
	int columnBorderWidth = skin->getListFieldColumnBorderWidth(this);
	int rowHeight = skin->getListFieldRowHeight(this);

	int columnPos = 0;
	if (columnNo > 0)
	{
		columnPos += columnBorderWidth * (columnNo - 1);
		for (int i = 0; i < columnNo; i++)
			columnPos += columns.Get(i)->getActualWidth();
	}
	int rowPos = rowNo * rowHeight;

	const ArrayList<FieldElement> & elements = cell->getElements();
	Vector relativePosition = Vector(columnPos, rowPos + rowHeight / 2);
	if (elementNo >= 0 && elementNo < elements.GetCount())
	{
		relativePosition += Vector(skin->getListFieldColumnPaddingWidth(this), 0);
		for (int i=0; i<elementNo; i++)
			relativePosition += Vector(elements.Get(i)->getSize().x, 0);

		relativePosition += Vector(elements.Get(elementNo)->getSize().x / 2, 0);
	}
	else
		relativePosition += Vector(columns.Get(columnNo)->getActualWidth() / 2, 0);

	Vector absolutePosition = rowsField.getAbsolutePosition() - rowsField.getScrollPosition() + relativePosition;
	if (Desktop::getInstance()->getPointerPosition() == absolutePosition)
		time = 0;

	PointerEffect * effect;
	if ((effect = new PointerEffect(time)) == NULL) \
		throw EOUTOFMEMORY;
	effect->setPositionEnd(absolutePosition);
	effect->setTimeCurveToAcceleration();
	effect->setButtonEffect(buttonEffect);
	effect->setButtonOption1(option1);
	effect->setButtonOption2(option2);

	Desktop::getInstance()->addEffect(effect);
	effect->waitFor();
}

void ListField::updateSelectionFromFocus(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (!option1 || type != TYPE_MANY)
		rangeStartRowNo = focusRowNo;

	if (!option2 || type != TYPE_MANY)
	{
		HashSet rowsToSelect;
		if (rangeStartRowNo <= focusRowNo)
			for (int i=rangeStartRowNo; i<=focusRowNo; i++)
				rowsToSelect.Add(rows.Get(i));
		else
			for (int i=focusRowNo; i<=rangeStartRowNo; i++)
				rowsToSelect.Add(rows.Get(i));

		setSelectedRows(rowsToSelect);
	}
}

void ListField::unselectRow(Row * row)
{
	ASSERTION_COBJECT(this);

	if (selectedRows.Contains(row))
	{
		selectedRows.Remove(row);
		notifySelectionChanged();
		invalidate();
	}
}

void ListField::unselectRows()
{
	ASSERTION_COBJECT(this);

	if (!selectedRows.IsEmpty())
	{
		selectedRows.RemoveAll();
		notifySelectionChanged();
		invalidate();
	}
}


ListField::Cell::Cell()
{
	ASSERTION_COBJECT(this);

	column = NULL;
	row = NULL;
}

ListField::Cell::~Cell()
{
	ASSERTION_COBJECT(this);
}

void ListField::Cell::appendElement(FieldElement * element)
{
	ASSERTION_COBJECT(this);

	if (row == NULL)
		throw EILLEGALSTATE("The cell must be added to a row before using it");

	if (element->getField() != NULL)
		throw EILLEGALSTATE("The element is already contained in a field");

	elements.Append(element);
	element->setFieldAndParent(row->getField(), this);
	row->getField()->invalidate();
}

FieldElement * ListField::Cell::edit()
{
	ASSERTION_COBJECT(this);

	if (!column->isVisible() || !column->isEditable())
		return NULL;

	int elementNo;
	for (elementNo=0; elementNo < elements.GetCount(); elementNo++)
		if (elements.Get(elementNo)->isEditable())
			break;

	if (elementNo >= elements.GetCount())
		return NULL;
	FieldElement * element = elements.Get(elementNo);

	Desktop * desktop = Desktop::getInstance();
	Skin * skin = desktop->getSkin();
	ListField * field = getField();
	int rowHeight = skin->getListFieldRowHeight(field);

	Vector elementPosition(0, rowHeight * field->focusRowNo);
	int i;
	for (i=0; i<field->focusColumnNo; i++)
		elementPosition.x += field->getColumns().Get(i)->getActualWidth();

	ListField::RowsField * rowsField = field->getRowsField();
	Vector editPosition = rowsField->getAbsolutePosition() + elementPosition - rowsField->getScrollPositionNow();
	Vector editSize = Vector(column->getActualWidth(), rowHeight);

	for (i=0; i<elementNo; i++)
	{
		int elementSize = elements.Get(i)->getSize().x;
		editPosition.x += elementSize;
		editSize.x -= elementSize;
	}

	element->edit(editPosition, editSize, rowsField);

	return element;
}

ListField::Column * ListField::Cell::getColumn()
{
	ASSERTION_COBJECT(this);
	return column;
}

const ArrayList<FieldElement> & ListField::Cell::getElements()
{
	ASSERTION_COBJECT(this);
	return elements;
}

ListField * ListField::Cell::getField()
{
	ASSERTION_COBJECT(this);
	return row->getField();
}

ListField::Row * ListField::Cell::getRow()
{
	ASSERTION_COBJECT(this);
	return row;
}

String ListField::Cell::getText(char separator)
{
	ASSERTION_COBJECT(this);

	bool first = true;
	String text;
	for (int i=0; i<elements.GetCount(); i++)
	{
		TextElement * element = dynamic_cast<TextElement *>(elements.Get(i));
		if (element != NULL)
		{
			if (separator != 0)
			{
				if (first)
					first = false;
				else
					text += separator;
			}

			text += element->getText();
		}
	}

	return text;
}

bool ListField::Cell::hasFocus()
{
	ASSERTION_COBJECT(this);
	return getField()->getFocusCell() == this;
}

void ListField::Cell::insertElement(int position, FieldElement * element)
{
	ASSERTION_COBJECT(this);

	if (row == NULL)
		throw EILLEGALSTATE("The cell must be added to a row before using it");

	if (element->getField() != NULL)
		throw EILLEGALSTATE("The element is already contained in a field");

	elements.Insert(element, position);
	element->setFieldAndParent(getField(), this);
	getField()->invalidate();
}

void ListField::Cell::prependElement(FieldElement * element)
{
	ASSERTION_COBJECT(this);

	if (row == NULL)
		throw EILLEGALSTATE("The cell must be added to a row before using it");

	if (element->getField() != NULL)
		throw EILLEGALSTATE("The element is already contained in a field");

	elements.Prepend(element);
	element->setFieldAndParent(getField(), this);
	getField()->invalidate();
}

void ListField::Cell::removeElement(int position)
{
	ASSERTION_COBJECT(this);

	if (row == NULL)
		throw EILLEGALSTATE("The cell must be added to a row before using it");

	elements.Delete(position);
	getField()->invalidate();
}

void ListField::Cell::removeElements()
{
	ASSERTION_COBJECT(this);

	if (row == NULL)
		throw EILLEGALSTATE("The cell must be added to a row before using it");

	elements.DeleteAll();
	getField()->invalidate();
}

void ListField::Cell::setColumn(Column * column)
{
	ASSERTION_COBJECT(this);

	if (this->column != NULL)
		throw EILLEGALARGUMENT("The cell is already contained in another column");
	this->column = column;
}

void ListField::Cell::setRow(Row * row)
{
	ASSERTION_COBJECT(this);

	if (this->row != NULL)
		throw EILLEGALARGUMENT("The cell is already contained in another row");
	this->row = row;
}


ListField::Column::Column()
{
	ASSERTION_COBJECT(this);

	alignment = ListField::ALIGN_LEFT;
	autoEditing = false;
	autoResizing = false;
	editable = true;
	field = NULL;
	heading = "";
	maximumWidth = 0;
	minimumWidth = 16;
	visible = true;
	width = 64;
}

ListField::Column::~Column()
{
	ASSERTION_COBJECT(this);
}

int ListField::Column::getActualWidth()
{
	ASSERTION_COBJECT(this);
	return visible ? width : 0;
}

ListField::Alignment ListField::Column::getAlignment()
{
	ASSERTION_COBJECT(this);
	return alignment;
}

ListField * ListField::Column::getField()
{
	ASSERTION_COBJECT(this);
	return field;
}

const String & ListField::Column::getHeading()
{
	ASSERTION_COBJECT(this);
	return heading;
}

int ListField::Column::getMaximumWidth()
{
	ASSERTION_COBJECT(this);
	return maximumWidth;
}

int ListField::Column::getMinimumWidth()
{
	ASSERTION_COBJECT(this);
	return minimumWidth;
}

int ListField::Column::getWidth()
{
	ASSERTION_COBJECT(this);
	return width;
}

bool ListField::Column::isAutoEditing()
{
	ASSERTION_COBJECT(this);
	return autoEditing;
}

bool ListField::Column::isAutoResizing()
{
	ASSERTION_COBJECT(this);
	return autoResizing;
}

bool ListField::Column::isEditable()
{
	ASSERTION_COBJECT(this);
	return editable;
}

bool ListField::Column::isVisible()
{
	ASSERTION_COBJECT(this);
	return visible;
}

void ListField::Column::setAlignment(ListField::Alignment alignment)
{
	ASSERTION_COBJECT(this);

	if (this->alignment == alignment)
		return;

	this->alignment = alignment;

	if (field != NULL)
		field->invalidate();
}

void ListField::Column::setAutoEditing(bool autoEditing)
{
	ASSERTION_COBJECT(this);
	this->autoEditing = autoEditing;
}

void ListField::Column::setAutoResizing(bool autoResizing)
{
	ASSERTION_COBJECT(this);

	if (this->autoResizing == autoResizing)
		return;

	this->autoResizing = autoResizing;

	if (field != NULL)
		field->adjustAutoColumns();
}

void ListField::Column::setEditable(bool editable)
{
	ASSERTION_COBJECT(this);

	if (this->editable == editable)
		return;

	this->editable = editable;

	if (field != NULL)
		field->invalidate();
}

void ListField::Column::setField(ListField * field)
{
	ASSERTION_COBJECT(this);

	if (this->field != NULL)
		throw EILLEGALARGUMENT("The column is already contained in another list field");

	this->field = field;
}

void ListField::Column::setHeading(const String & heading)
{
	ASSERTION_COBJECT(this);

	if (this->heading == heading)
		return;

	this->heading = heading;

	if (field != NULL)
		field->invalidate();
}

void ListField::Column::setMaximumWidth(int maximumWidth)
{
	ASSERTION_COBJECT(this);

	this->maximumWidth = maximumWidth;

	if (maximumWidth > 0 && width > maximumWidth)
		setWidth(maximumWidth);
}

void ListField::Column::setMinimumWidth(int minimumWidth)
{
	ASSERTION_COBJECT(this);

	this->minimumWidth = minimumWidth;

	if (width < minimumWidth)
		setWidth(minimumWidth);
}

void ListField::Column::setVisible(bool visible)
{
	ASSERTION_COBJECT(this);

	if (this->visible == visible)
		return;

	this->visible = visible;

	if (field != NULL)
	{
		field->adjustAutoColumns();
		field->getRowsField()->updateContentSize();
		field->invalidate();
	}
}

void ListField::Column::setWidth(int width)
{
	ASSERTION_COBJECT(this);

	if (width < minimumWidth)
		width = minimumWidth;
	if (maximumWidth > 0 && width > maximumWidth)
		width = maximumWidth;

	if (this->width == width)
		return;

	int oldWidth = this->width;
	this->width = width;

	if (field != NULL)
	{
		field->adjustAutoColumns();
		field->getRowsField()->updateContentSize();

		int listenersCount = field->getListeners().GetCount();
		if (listenersCount > 0)
		{
			int i; // loop variable
			int columnNo = 0;
			for (i=0; i<field->getColumnCount(); i++)
				if (field->getColumns().Get(i) == this)
					columnNo = i;
			for (i=0; i<listenersCount; i++)
			{
				ListField::Listener * listFieldListener
					= dynamic_cast<ListField::Listener *>(field->getListeners().Get(i));
				if (listFieldListener != NULL)
					listFieldListener->onColumnResized(field, columnNo, oldWidth);
			}
		}

		field->invalidate();
	}
}


ListField::EditEffect::EditEffect(ListField::RowsField * rowsField, long time)
	: TimedEffect(time)
{
	ASSERTION_COBJECT(this);
	this->rowsField = rowsField;
}

bool ListField::EditEffect::onEnd(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	rowsField->editEffect = NULL;
	rowsField->onEdit();

	return false;
}


ListField::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

bool ListField::Listener::onCellClicked(ListField * field, int rowNo, int columnNo, int elementNo)
{
	ASSERTION_COBJECT(this);
	return false;
}

void ListField::Listener::onCellDoubleClicked(ListField * field, int rowNo, int columnNo, int elementNo)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onCellHover(ListField * field, int rowNo, int columnNo, int elementNo)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onColumnAdded(ListField * field, int columnNo)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onColumnClicked(ListField * field, int columnNo)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onColumnRemoved(ListField * field)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onColumnRemoving(ListField * field, int columnNo)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onColumnResized(ListField * field, int columnNo, int oldWidth)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onHeadlineHover(ListField * field, int columnNo)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onRowAdded(ListField * field, int rowNo)
{
	ASSERTION_COBJECT(this);
}

void ListField::Listener::onRowsRemoved(ListField * field)
{
	ASSERTION_COBJECT(this);
}

VetoMode ListField::Listener::onRowsRemoving(ListField * field, int * rowNos, int rowCount)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}

void ListField::Listener::onSelectionChanged(ListField * field)
{
	ASSERTION_COBJECT(this);
}


ListField::Row::Row()
{
	ASSERTION_COBJECT(this);
	active = true;
	field = NULL;
}

ListField::Row::~Row()
{
	ASSERTION_COBJECT(this);
	field = NULL;
}

void ListField::Row::appendCell(ListField::Cell * cell)
{
	ASSERTION_COBJECT(this);
	insertCell(cells.GetCount(), cell);
}

ListField::Cell * ListField::Row::getCell(int cellNo)
{
	ASSERTION_COBJECT(this);
	return cells.Get(cellNo);
}

const ArrayList<ListField::Cell> & ListField::Row::getCells()
{
	ASSERTION_COBJECT(this);
	return cells;
}

ListField * ListField::Row::getField()
{
	ASSERTION_COBJECT(this);
	return field;
}

void ListField::Row::insertCell(int position, ListField::Cell * cell)
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The row must be added to a list field before using it");

	if (cell->getRow() != NULL)
		throw EILLEGALSTATE("The cell is already contained in another field");

	if (cells.GetCount() >= field->getColumnCount())
		throw EILLEGALSTATE("You cannot add more cells to a list field row than the number of columns");

	Column * column = field->getColumns().Get(position);
	ASSERTION(column != NULL);

	cells.Insert(cell, position);
	cell->setColumn(column);
	cell->setRow(this);
	field->invalidate();
}

bool ListField::Row::isActive()
{
	ASSERTION_COBJECT(this);
	return active;
}

bool ListField::Row::isSelected()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The row must be added to a list field before using it");

	return field->getSelectedRows().Contains(this);
}

void ListField::Row::prependCell(ListField::Cell * cell)
{
	ASSERTION_COBJECT(this);
	insertCell(0, cell);
}

void ListField::Row::removeCell(int position)
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The row must be added to a list field before using it");

	cells.Delete(position);
	field->invalidate();
}

void ListField::Row::removeCells()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The row must be added to a list field before using it");

	cells.DeleteAll();
	field->invalidate();
}

void ListField::Row::select()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The row must be added to a list field before using it");

	field->selectRow(this);
}

void ListField::Row::setActive(bool active)
{
	ASSERTION_COBJECT(this);

	if (this->active == active)
		return;

	this->active = active;

	while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo < field->rows.GetCount() - 1)
	   field->focusRowNo++;
	while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo > 0)
	   field->focusRowNo--;

	field->updateSelectionFromFocus(false, false);
	field->ensureFocusVisible();
	field->getRowsField()->invalidate();
}

void ListField::Row::setField(ListField * field)
{
	ASSERTION_COBJECT(this);

	if (this->field != NULL)
		throw EILLEGALARGUMENT("The row is already contained in another list field");
	this->field = field;
}

void ListField::Row::unselect()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		throw EILLEGALSTATE("The row must be added to a list field before using it");

	field->unselectRow(this);
}


ListField::RowsField::RowsField()
{
	ASSERTION_COBJECT(this);

	editEffect = NULL;
	editOnClick = false;
	lastClickColumnNo = -1;
	lastClickElementNo = -1;
	lastClickRowNo = -1;
	updateSelectionOnClick = false;
}

ListField::RowsField::~RowsField()
{
	ASSERTION_COBJECT(this);

	if (editEffect != NULL) {editEffect->cancel(); editEffect = NULL;}
}

void ListField::RowsField::addContextMenuItems(Menu * menu, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
	getListField()->addRowsContextMenuItems(menu, position, option1, option2);
}

void ListField::RowsField::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	skin->drawListFieldRowsField(context, this);
}

void ListField::RowsField::getColumnAndRowForPosition(Vector position,
	int & columnNo, int & columnPos, int & rowNo)
{
	ASSERTION_COBJECT(this);

	ListField * field = getListField();
	Skin * skin = Desktop::getInstance()->getSkin();
	int columnBorderWidth = skin->getListFieldColumnBorderWidth(field);
	int rowHeight = skin->getListFieldRowHeight(field);
	const ArrayList<Column> & columns = field->columns;
	const ArrayList<Row> & rows = field->rows;

	columnPos = 0;
	for (columnNo = 0; columnNo < columns.GetCount(); columnNo++)
	{
		if (!columns.Get(columnNo)->isVisible())
			continue;

		int columnWidth = columns.Get(columnNo)->getWidth();
		if (position.x < columnPos + columnWidth + columnBorderWidth / 2)
			break;

		columnPos += columnWidth + columnBorderWidth;
	}
	rowNo = position.y / rowHeight;

	if (columnNo < 0) columnNo = -1;
	if (columnNo > columns.GetCount()) columnNo = columns.GetCount();
	if (rowNo < 0) rowNo = -1;
	if (rowNo > rows.GetCount()) rowNo = rows.GetCount();
}

ListField * ListField::RowsField::getListField()
{
	ASSERTION_COBJECT(this);
	return (ListField *) getParent();
}

int ListField::RowsField::getPageLinesCount()
{
	ASSERTION_COBJECT(this);

	Skin * skin = Desktop::getInstance()->getSkin();
	int rowHeight = skin->getListFieldRowHeight(getListField());

	int lines = getContentViewSize().y / rowHeight;
	if (lines < 1)
		lines = 1;

	return lines;
}

bool ListField::RowsField::isAcceptingPressesToFocus()
{
	ASSERTION_COBJECT(this);
	return false;
}

bool ListField::RowsField::isHorizontallyScrollable()
{
	ASSERTION_COBJECT(this);
	return getListField()->isHorizontallyScrollable();
}

bool ListField::RowsField::isVerticallyScrollable()
{
	ASSERTION_COBJECT(this);
	return getListField()->isVerticallyScrollable();
}

bool ListField::RowsField::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingField::onCharacter(character, option1, option2);

	if (option2)
	{
		if (character == ' ')
		{
			toggleSelection();
			return true;
		}
	}
	else
	{
		if (getListField()->isEditable())
		{
			if (editEffect != NULL) {editEffect->cancel(); editEffect = NULL;}

			ListField * field = getListField();
			if (!field->isEditable())
				return true;

			Cell * cell = field->getFocusCell();
			if (cell == NULL || field->focusColumnNo >= field->columns.GetCount())
				return true;

			if (!cell->getColumn()->isAutoEditing())
				return true;

			TextElement * element = dynamic_cast<TextElement *>(cell->edit());
			if (element == NULL)
				return true;

			TextField * popupField = dynamic_cast<TextField *>(element->getEditPopup()->getContentField());
			if (popupField == NULL)
				return true;

			popupField->setText(character);
			popupField->selectCharacters(0, 1, 0);
		}
	}

	return consumed;
}

void ListField::RowsField::onContentClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (updateSelectionOnClick)
	{
		ListField * field = getListField();
		field->updateSelectionFromFocus(option1, option2);
		if (!option1 && option2)
			toggleSelection();
	}

	if (editOnClick)
	{
		Window * window = getWindow();
		if (window == NULL || !window->isLastPressFocused())
		{
			if (editEffect != NULL) {editEffect->cancel(); editEffect = NULL;}

			if ((editEffect = new EditEffect(this, Desktop::getInstance()->getDoubleClickDelay())) == NULL)
				throw EOUTOFMEMORY;
			Desktop::getInstance()->addEffect(editEffect);
		}
	}

	ScrollingField::onContentClick(position, option1, option2);
}

void ListField::RowsField::onContentDoubleClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onContentDoubleClick(position, option1, option2);

	if (!isActiveIncludingAncestors())
		return;

	if (editEffect != NULL) {editEffect->cancel(); editEffect = NULL;}

	ListField * field = getListField();
	Row * row = field->rows.Get(lastClickRowNo);
	if (row == NULL)
		return;

	Cell * cell = row->getCells().Get(lastClickColumnNo);
	if (cell == NULL)
		return;

	Column * column = field->columns.Get(lastClickColumnNo);
	if (column == NULL || !column->isVisible())
		return;

	if (lastClickElementNo < -1 || lastClickElementNo >= cell->getElements().GetCount())
		return;

	const ArrayList<Listenable::Listener> & listeners = field->getListeners();
	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		ListField::Listener * listFieldListener
			= dynamic_cast<ListField::Listener *>(listeners.Get(i));
		if (listFieldListener != NULL)
			listFieldListener->onCellDoubleClicked(field,
				lastClickRowNo, lastClickColumnNo, lastClickElementNo);
	}
}

void ListField::RowsField::onContentPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onContentPress(position, option1, option2);

	if (!isActiveIncludingAncestors())
		return;

	if (editEffect != NULL) {editEffect->cancel(); editEffect = NULL;}

	lastClickColumnNo = -1;
	lastClickElementNo = -1;
	lastClickRowNo = -1;
	updateSelectionOnClick = false;
	editOnClick = false;

	int columnNo;
	int columnPos;
	int rowNo;
	getColumnAndRowForPosition(position, columnNo, columnPos, rowNo);

	ListField * field = getListField();
	Skin * skin = Desktop::getInstance()->getSkin();

	if (columnNo >= 0 && columnNo < field->getColumnCount()
		&& rowNo >= 0 && rowNo < field->getRowCount())
	{
		bool cellWasFocused = field->focusColumnNo == columnNo && field->focusRowNo == rowNo;
		bool cellWasSelected = field->isRowSelected(rowNo);

		bool handled = false;
		if (!option1 && !option2)
		{
			int elementNo;
			Cell * cell = field->getCell(rowNo, columnNo);
			if (cell != NULL)
			{
				const ArrayList<FieldElement> & elements = cell->getElements();
				int elementPos = columnPos + skin->getListFieldColumnPaddingWidth(field);
				for (elementNo = 0; elementNo < elements.GetCount(); elementNo++)
				{
					FieldElement * element = elements.Get(elementNo);
					if (position.x < elementPos + element->getSize().x)
						break;
					elementPos += element->getSize().x;
				}

				if (elementNo >= elements.GetCount())
					elementNo = -1;
			}
			else
				elementNo = -1;

			lastClickRowNo = rowNo;
			lastClickColumnNo = columnNo;
			lastClickElementNo = elementNo;

			const ArrayList<Listenable::Listener> & listeners = field->getListeners();
			int listenersCount = listeners.GetCount();
			for (int i=0; i<listenersCount; i++)
			{
				ListField::Listener * listFieldListener
					= dynamic_cast<ListField::Listener *>(listeners.Get(i));
				if (listFieldListener != NULL)
					if (listFieldListener->onCellClicked(field, rowNo, columnNo, elementNo))
						handled = true;
			}
		}

		if (!handled)
		{
			field->focusColumnNo = columnNo;
			field->focusRowNo = rowNo;
			while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo < field->rows.GetCount() - 1)
	     		field->focusRowNo++;
			while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo > 0)
	     		field->focusRowNo--;

			field->ensureFocusVisible();

			if (!field->isRowSelected(rowNo))
			{
				field->updateSelectionFromFocus(option1, option2);
				if (!option1 && option2)
					toggleSelection();
			}
			else
				updateSelectionOnClick = true;

			if (cellWasFocused && cellWasSelected && !option1 && !option2)
				editOnClick = true;
		}
	}
	else if (!option1 && !option2 && field->getType() != ListField::TYPE_ONE)
	{
		if (!field->selectedRows.IsEmpty())
		{
			field->selectedRows.RemoveAll();
			field->notifySelectionChanged();
		}
	}

	invalidate();
}

void ListField::RowsField::onContextClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	// Note: Need to intercept onContextClick instead of onContentContextClick
	// to be able to focus and select before the context menu is built.

	if (!isActiveIncludingAncestors())
	{
		ScrollingField::onContextClick(position, option1, option2);
		return;
	}

	Vector positionWithinContent = getPositionWithinContent(position);

	if (editEffect != NULL) {editEffect->cancel(); editEffect = NULL;}

	lastClickColumnNo = -1;
	lastClickElementNo = -1;
	lastClickRowNo = -1;
	updateSelectionOnClick = false;
	editOnClick = false;

	int columnNo;
	int columnPos;
	int rowNo;
	getColumnAndRowForPosition(positionWithinContent, columnNo, columnPos, rowNo);

	ListField * field = getListField();
	Skin * skin = Desktop::getInstance()->getSkin();

	if (columnNo >= 0 && columnNo < field->getColumnCount()
		&& rowNo >= 0 && rowNo < field->getRowCount())
	{
		field->focusColumnNo = columnNo;
		field->focusRowNo = rowNo;
		field->ensureFocusVisible();

		if (!field->isRowSelected(rowNo))
		{
			field->rangeStartRowNo = rowNo;
			field->setSelectedRow(rowNo);
		}
	}
	else if (field->getType() != ListField::TYPE_ONE)
	{
		if (!field->selectedRows.IsEmpty())
		{
			field->selectedRows.RemoveAll();
			field->notifySelectionChanged();
		}
	}

	invalidate();

	ScrollingField::onContextClick(position, option1, option2);
}

bool ListField::RowsField::onDelete()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onDelete();

	ListField * field = getListField();
	if (!field->isEditable() || !field->isRowsRemovable())
		return true;

	if (field->selectedRows.IsEmpty() && !field->rows.IsEmpty())
		field->removeRow(field->focusRowNo);
	else
		field->removeSelectedRows();

	return true;
}

DraggingSurrogate * ListField::RowsField::onDragStart(const Vector & startPosition)
{
	ASSERTION_COBJECT(this);

	int columnNo;
	int columnPos;
	int rowNo;
	getColumnAndRowForPosition(startPosition - getAbsolutePosition(), columnNo, columnPos, rowNo);

	ListField * field = getListField();
	if (columnNo < 0 || columnNo >= field->getColumnCount()
		|| rowNo < 0 || rowNo >= field->getRowCount())
		return NULL;

	DraggingSurrogate * surrogate = field->onDragStart(rowNo, columnNo);
	if (surrogate == NULL)
		surrogate = field->onDragStart(rowNo);

	return surrogate;
}

bool ListField::RowsField::onEdit()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onEdit();

	if (editEffect != NULL) {editEffect->cancel(); editEffect = NULL;}

	ListField * field = getListField();
	if (!field->isEditable())
		return true;

	Cell * cell = field->getFocusCell();
	if (cell == NULL || field->focusColumnNo >= field->columns.GetCount())
		return true;

	cell->edit();

	return true;
}

void ListField::RowsField::onHover()
{
	ASSERTION_COBJECT(this);

	EventTarget::onHover();

	ListField * field = getListField();
	Vector position = Desktop::getInstance()->getPointerPosition() - getAbsolutePosition() + getScrollPositionNow();
	int columnNo;
	int columnPos;
	int rowNo;
	getColumnAndRowForPosition(position, columnNo, columnPos, rowNo);

	Cell * cell = field->getCell(rowNo, columnNo);
	if (cell == NULL)
		return;

	int elementNo;
	const ArrayList<FieldElement> & elements = cell->getElements();
	int elementPos = columnPos + Desktop::getInstance()->getSkin()->getListFieldColumnPaddingWidth(field);
	for (elementNo = 0; elementNo < elements.GetCount(); elementNo++)
	{
		FieldElement * element = elements.Get(elementNo);
		if (position.x < elementPos + element->getSize().x)
			break;
		elementPos += element->getSize().x;
	}

	if (elementNo >= elements.GetCount())
		elementNo = -1;

	field->onCellHover(rowNo, columnNo, elementNo);
}

bool ListField::RowsField::onMoveDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveDown(option1, option2);

	ListField * field = getListField();
	if (field->focusRowNo < field->rows.GetCount() - 1)
	{
		field->focusRowNo++;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo < field->rows.GetCount() - 1)
	     	field->focusRowNo++;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo > 0)
	     	field->focusRowNo--;

		field->updateSelectionFromFocus(option1, option2);
		field->ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool ListField::RowsField::onMoveLeft(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveLeft(option1, option2);

	ListField * field = getListField();
	for (int i = field->focusColumnNo - 1; i >= 0; i--)
		if (field->columns.Get(i)->isVisible())
		{
			field->focusColumnNo = i;
			field->ensureFocusVisible();
			invalidate();
			return true;
		}

	return true;
}

bool ListField::RowsField::onMoveRight(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveRight(option1, option2);

	ListField * field = getListField();
	for (int i = field->focusColumnNo + 1; i < field->columns.GetCount(); i++)
		if (field->columns.Get(i)->isVisible())
		{
			field->focusColumnNo = i;
			field->ensureFocusVisible();
			invalidate();
			return true;
		}

	return true;
}

bool ListField::RowsField::onMoveToEnd(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveToEnd(option1, option2);

	ListField * field = getListField();
	if (field->focusRowNo < field->rows.GetCount() - 1)
	{
		field->focusRowNo = field->rows.GetCount() - 1;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo > 0)
	     	field->focusRowNo--;

		field->updateSelectionFromFocus(option1, option2);
		field->ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool ListField::RowsField::onMoveToStart(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveToStart(option1, option2);

	ListField * field = getListField();
	if (field->focusRowNo > 0)
	{
		field->focusRowNo = 0;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo < field->rows.GetCount() - 1)
	     	field->focusRowNo++;

		field->updateSelectionFromFocus(option1, option2);
		field->ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool ListField::RowsField::onMoveUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveUp(option1, option2);

	ListField * field = getListField();
	if (field->focusRowNo > 0)
	{
     	field->focusRowNo--;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo > 0)
	     	field->focusRowNo--;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo < field->rows.GetCount() - 1)
	     	field->focusRowNo++;

		field->updateSelectionFromFocus(option1, option2);
		field->ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool ListField::RowsField::onPageDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onPageDown(option1, option2);

	ListField * field = getListField();
	if (field->focusRowNo < field->rows.GetCount() - 1)
	{
		field->focusRowNo += getPageLinesCount();
		if (field->focusRowNo > field->rows.GetCount() - 1)
			field->focusRowNo = field->rows.GetCount() - 1;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo < field->rows.GetCount() - 1)
	     	field->focusRowNo++;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo > 0)
	     	field->focusRowNo--;

		field->updateSelectionFromFocus(option1, option2);
		field->ensureFocusVisible();
		invalidate();
	}

	return true;
}

bool ListField::RowsField::onPageUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onPageUp(option1, option2);

	ListField * field = getListField();
	if (field->focusRowNo > 0)
	{
		field->focusRowNo -= getPageLinesCount();
		if (field->focusRowNo < 0)
			field->focusRowNo = 0;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo > 0)
	     	field->focusRowNo--;
		while (!field->getRow(field->focusRowNo)->isActive() && field->focusRowNo < field->rows.GetCount() - 1)
	     	field->focusRowNo++;

		field->updateSelectionFromFocus(option1, option2);
		field->ensureFocusVisible();
		invalidate();
	}

	return true;
}

void ListField::RowsField::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ListField * listField = getListField();
	if (listField->isActiveIncludingAncestors())
		listField->setFocus();

	ScrollingField::onPress(position, option1, option2);
}

void ListField::RowsField::onScrollPositionNowChanged(Vector previousPosition)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onScrollPositionNowChanged(previousPosition);

	if (previousPosition.x != getScrollPositionNow().x)
		getListField()->invalidate();
}

bool ListField::RowsField::onSelectAll()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onSelectAll();

	ListField * field = getListField();
	field->selectAllRows();

	return true;
}

void ListField::RowsField::onWheel(int delta)
{
	ASSERTION_COBJECT(this);

	InputField::onWheel(delta);

	if (isActiveIncludingAncestors())
	{
		int wheelLines = Desktop::getInstance()->getInputManager().GetWheelLines();
		if (wheelLines < 0)
		{
			Vector contentViewSize = getContentViewSize();
			scroll(Vector(0, -contentViewSize.y * delta));
		}
		else
		{
			Skin * skin = Desktop::getInstance()->getSkin();
			int rowHeight = skin->getListFieldRowHeight(getListField());
			scroll(Vector(0, -rowHeight * wheelLines * delta));
		}
	}
}

void ListField::RowsField::toggleSelection()
{
	ASSERTION_COBJECT(this);

	ListField * field = getListField();
	if (field->rows.IsEmpty())
		return;

	Row * row = field->rows.Get(field->focusRowNo);

	if (field->getType() == ListField::TYPE_ONE)
	{
		field->selectedRows.RemoveAll();
		field->selectedRows.Add(row);
	}
	else if (field->getType() == ListField::TYPE_ONE_OR_NONE)
	{
		bool focusEqualsSelection = field->selectedRows.GetCount() == 1 && field->selectedRows.Contains(row);
		field->selectedRows.RemoveAll();
		if (!focusEqualsSelection)
			field->selectedRows.Add(row);
	}
	else
	{
		if (field->selectedRows.Contains(row))
			field->selectedRows.Remove(row);
		else
			field->selectedRows.Add(row);
	}

	field->notifySelectionChanged();
	invalidate();
}

String ListField::RowsField::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("ListField::RowsField(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}

void ListField::RowsField::updateContentSize()
{
	ASSERTION_COBJECT(this);

	ListField * field = getListField();
	Skin * skin = Desktop::getInstance()->getSkin();
	int columnBorderWidth = skin->getListFieldColumnBorderWidth(field);
	int rowHeight = skin->getListFieldRowHeight(field);

	contentSize = Vector(field->getRowWidth(),
		rowHeight * field->rows.GetCount());
}
