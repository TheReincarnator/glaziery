/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_LISTFIELD_H
#define __GLAZIERY_LISTFIELD_H


#define GLAZIERY_LISTFIELD(variable, area, groupField, \
	horizontallyScrollable, verticallyScrollable, \
	type) \
	ListField * variable; \
	if ((variable = new ListField) == NULL) \
		throw EOUTOFMEMORY; \
	if (area != NULL) \
		((FieldsArea *) area)->appendField(variable); \
	if (groupField != NULL) \
		((GroupField *) groupField)->appendField(variable); \
	variable->setHorizontallyScrollable(horizontallyScrollable); \
	variable->setVerticallyScrollable(verticallyScrollable); \
	variable->setType(ListField::TYPE_##type);

#define GLAZIERY_LISTFIELD_P(variable, area, groupField, px, py, \
	horizontallyScrollable, verticallyScrollable, \
	type) \
	GLAZIERY_LISTFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->moveTo(Vector((px), (py))); \

#define GLAZIERY_LISTFIELD_S(variable, area, groupField, width, height, \
	horizontallyScrollable, verticallyScrollable, \
	type) \
	GLAZIERY_LISTFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_LISTFIELD_PS(variable, area, groupField, px, py, width, height, \
	horizontallyScrollable, verticallyScrollable, \
	type) \
	GLAZIERY_LISTFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->moveTo(Vector((px), (py))); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_LISTFIELD_M(variable, area, groupField, minimumWidth, minimumHeight, \
	horizontallyScrollable, verticallyScrollable, \
	type) \
	GLAZIERY_LISTFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_LISTFIELD_PM(variable, area, groupField, px, py, minimumWidth, minimumHeight, \
	horizontallyScrollable, verticallyScrollable, \
	type) \
	GLAZIERY_LISTFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resizeToMinimum();

#define GLAZIERY_LISTFIELD_SM(variable, area, groupField, width, height, minimumWidth, minimumHeight, \
	horizontallyScrollable, verticallyScrollable, \
	type) \
	GLAZIERY_LISTFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_LISTFIELD_PSM(variable, area, groupField, px, py, width, height, minimumWidth, minimumHeight, \
	horizontallyScrollable, verticallyScrollable, \
	type) \
	GLAZIERY_LISTFIELD(variable, area, groupField, \
		horizontallyScrollable, verticallyScrollable, type) \
	variable->moveTo(Vector((px), (py))); \
	variable->setMinimumSize(Vector(minimumWidth, minimumHeight)); \
	variable->resize(Vector((width) >= 0 ? (width) : variable->getSize().x, \
		(height) >= 0 ? (height) : variable->getSize().y));

#define GLAZIERY_LISTFIELD_COLUMN(variable, listField, editable, alignment, heading) \
	ListField::Column * variable; \
	if ((variable = new ListField::Column) == NULL) \
		throw EOUTOFMEMORY; \
	listField->appendColumn(variable); \
	variable->setAutoResizing(true); \
	variable->setEditable(editable); \
	variable->setAlignment(ListField::ALIGN_##alignment); \
	variable->setHeading(heading);

#define GLAZIERY_LISTFIELD_COLUMN_S(variable, listField, width, editable, alignment, heading) \
	ListField::Column * variable; \
	if ((variable = new ListField::Column) == NULL) \
		throw EOUTOFMEMORY; \
	listField->appendColumn(variable); \
	variable->setWidth(width); \
	variable->setEditable(editable); \
	variable->setAlignment(ListField::ALIGN_##alignment); \
	variable->setHeading(heading);

#define GLAZIERY_LISTFIELD_ROW(variable, listField) \
	ListField::Row * variable; \
	if ((variable = new ListField::Row) == NULL) \
		throw EOUTOFMEMORY; \
	listField->appendRow(variable);

#define GLAZIERY_LISTFIELD_ROW_P(variable, listField, position) \
	ListField::Row * variable; \
	if ((variable = new ListField::Row) == NULL) \
		throw EOUTOFMEMORY; \
	listField->insertRow(position, variable);

#define GLAZIERY_LISTFIELD_CELL(variable, row) \
	ListField::Cell * variable; \
	if ((variable = new ListField::Cell) == NULL) \
		throw EOUTOFMEMORY; \
	row->appendCell(variable);

#define GLAZIERY_LISTFIELD_TEXTELEMENT(variable, cell, text) \
	TextElement * variable; \
	if ((variable = new TextElement) == NULL) \
		throw EOUTOFMEMORY; \
	cell->appendElement(variable); \
	variable->setText(text);

#define GLAZIERY_LISTFIELD_IMAGEELEMENT(variable, cell, image) \
	ImageElement * variable; \
	if ((variable = new ImageElement) == NULL) \
		throw EOUTOFMEMORY; \
	cell->appendElement(variable); \
	variable->setImage(image);

#define GLAZIERY_LISTFIELD_IMAGEELEMENT_I(variable, cell, imageId) \
	ImageElement * variable; \
	if ((variable = new ImageElement) == NULL) \
		throw EOUTOFMEMORY; \
	cell->appendElement(variable); \
	variable->setImageId(imageId);


namespace glaziery
{
	/**
	 * The list field is an input field that displays items in rows,
	 * each row consisting of one or more columns of text or image type.
	 * All rows have the same columns, so the content is displayed
	 * like a table. The list field also has a heading, a row that displays
	 * the name of each column.
	 * The list field supports scrolling, which can be disabled by setting
	 * the respective properties.
	 */
	class ListField : public InputField
	{
		public:

			class Cell;
			class Column;
			class EditEffect;
			class Listener;
			class Row;
			class RowsField;

			enum Alignment
			{
				/**
				 * Center the whole cell's content.
				 */
				ALIGN_CENTER,

				/**
				 * Center the whole cell's content, use the entire column (do no pad at the left and the right).
				 */
				ALIGN_CENTER_NO_PADDING,

				/**
				 * Align the whole cell's content to the left.
				 */
				ALIGN_LEFT,

				/**
				 * Align the whole cell's content to the right.
				 */
				ALIGN_RIGHT
			};

			class Cell : public GlazieryObject
			{
				friend ArrayList<Cell>;
				friend Row;

				private:

					/**
					 * The field elements of this cell.
					 */
					ArrayList<FieldElement> elements;

					/**
					 * The row this cell belongs to.
					 */
					Row * row;

					/**
					 * The column this cell belongs to.
					 */
					Column * column;

					/**
					 * Sets the column this cell belongs to.
					 * An exception is thrown, if the property has already been set.
					 * @param column The column this cell belongs to.
					 */
					 void setColumn(Column * column);

					/**
					 * Sets the row this cell belongs to.
					 * An exception is thrown, if the property has already been set.
					 * @param row The row this cell belongs to.
					 */
					 void setRow(Row * row);

				protected:

					/**
					 * Destroys the cell.
					 */
					~Cell();

				public:

					/**
					 * Creates a new cell.
					 */
					Cell();

					/**
					 * Appends a field element at the end of the cell.
					 * @param element The element to be added.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void appendElement(FieldElement * element);

					/**
					 * Opens a popup to edit the first editable field element of this cell.
					 * This method is typically called by the list field when receiving the onEdit() event.
					 * @return The field element that is being edited now, or NULL, if no field element is editable.
					 */
					FieldElement * edit();

					/**
					 * Returns the column this cell belongs to.
					 * @return The column this cell belongs to.
					 */
					Column * getColumn();

					/**
					 * Returns the field elements of this cell
					 * @return The content elements of this cell.
					 */
					const ArrayList<FieldElement> & getElements();

					/**
					 * Returns the list field this cell belongs to.
					 * @return The list field this cell belongs to.
					 */
					ListField * getField();

					/**
					 * Returns the row this cell belongs to.
					 * @return The row this cell belongs to.
					 */
					Row * getRow();

					/**
					 * Returns the concatenation of all text element's texts of this cell.
					 * @param separator The separator character to use between the texts.
					 * Defaults to 0, which adds no separator character at all.
					 * @return The text.
					 */
					String getText(char separator = 0);

					/**
					 * Returns whether this cell has focus in its list field.
					 * @return Whether this cell has focus in its list field.
					 */
					bool hasFocus();

					/**
					 * Inserts a field element at a given position.
					 * @param position the 0-indexed column position to insert the element at.
					 * @param element The element to be added.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void insertElement(int position, FieldElement * element);

					/**
					 * Prepends a field element at the beginning of the cell.
					 * @param element The element to be added.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 */
					void prependElement(FieldElement * element);

					/**
					 * Removes a field element at a given position.
					 * @param position the 0-indexed column position of the element to be removed.
					 */
					void removeElement(int position);

					/**
					 * Removes all field elements.
					 */
					void removeElements();
			};

			/**
			 * A column of a list field. Defines the column heading and width.
			 * The content type is determined by the row itself.
			 */
			class Column : public GlazieryObject
			{
				friend ArrayList<Column>;
				friend ListField;

				private:

					/**
					 * How to align the whole cell's content in this column.
					 * Defaults to LEFT.
					 */
					Alignment alignment;

					/**
					 * Whether any character starts editing the column's cell. Defaults to false.
					 */
					bool autoEditing;

					/**
					 * Whether this column (together with all the other auto-resizing columns)
					 * automatically consumes all content view space after substracting the width
					 * of the non-auto-resizing columns.
					 */
					bool autoResizing;

					/**
					 * Whether the cells of this column can be edited.
					 */
					bool editable;

					/**
					 * The list field this column belongs to.
					 */
					ListField * field;

					/**
					 * The heading text describing this column.
					 * It is displayed above all rows in the heading row.
					 */
					String heading;

					/**
					 * The maximum width of this column in pixels, or 0, if there is no limit.
					 */
					int maximumWidth;

					/**
					 * The minimum width of this column in pixels.
					 */
					int minimumWidth;

					/**
					 * Whether this column is currently visible. Defaults to true.
					 */
					bool visible;

					/**
					 * The width of this column in pixels.
					 */
					int width;

					/**
					 * Destroys the column.
					 */
					~Column();

					/**
					 * Sets the field property of the column to a given value.
					 * An exception is thrown, if the column field is not NULL.
					 * @param field The new field value.
					 */
					 void setField(ListField * field);

				public:

					/**
					 * Creates a new column.
					 */
					Column();

					/**
					 * Returns the visible width of this column, i.e. if it is visible,
					 * returns the width in pixels, otherwise returns 0.
					 * @return The actual width of this column.
					 */
					int getActualWidth();

					/**
					 * Returns how to align the whole cell's content in this column.
					 * Defaults to LEFT.
					 * @return How to align the whole cell's content in this column.
					 */
					Alignment getAlignment();

					/**
					 * Returns the list field this column belongs to.
					 * @return The list field this column belongs to.
					 */
					ListField * getField();

					/**
					 * Returns the heading text describing this column.
					 * It is displayed above all rows in the heading row.
					 * @return The heading.
					 */
					const String & getHeading();

					/**
					 * Returns the maximum width of this column in pixels, or 0, if there is no limit.
					 * @return The maximum width of this column.
					 */
					int getMaximumWidth();

					/**
					 * Returns the minimum width of this column in pixels.
					 * @return The minimum width of this column.
					 */
					int getMinimumWidth();

					/**
					 * Returns the width of this column in pixels.
					 * @return The width of this column.
					 */
					int getWidth();

					/**
					 * Returns whether any character starts editing the column's cell. Defaults to false.
					 * @return Whether the column is auto-editing.
					 */
					bool isAutoEditing();

					/**
					 * Returns whether this column (together with all the other auto-resizing columns)
					 * automatically consumes all content view space after substracting the width
					 * of the non-auto-resizing columns.
					 * @return Whether this column is auto resizing.
					 */
					bool isAutoResizing();

					/**
					 * Returns whether the cells of this column can be edited.
					 * @return Whether the cells of this column can be edited.
					 */
					bool isEditable();

					/**
					 * Returns whether this column is currently visible. Defaults to true.
					 * @return Whether this column is currently visible.
					 */
					bool isVisible();

					/**
					 * Sets how to align the whole cell's content in this column.
					 * Defaults to LEFT.
					 * @param alignment How to align the whole cell's content in this column.
					 */
					void setAlignment(Alignment alignment);

					/**
					 * Sets whether any character starts editing the column's cell. Defaults to false.
					 * @param autoEditing Whether the column is auto-editing.
					 */
					void setAutoEditing(bool autoEditing);

					/**
					 * Sets whether this column (together with all the other auto-resizing columns)
					 * automatically consumes all content view space after substracting the width
					 * of the non-auto-resizing columns.
					 * @param autoResizing Whether this column is auto resizing.
					 */
					void setAutoResizing(bool autoResizing);

					/**
					 * Returns whether the cells of this column can be edited.
					 * @param editable Whether the cells of this column can be edited.
					 */
					void setEditable(bool editable);

					/**
					 * Sets the heading text describing this column.
					 * It is displayed above all rows in the heading row.
					 * @param heading The heading.
					 */
					void setHeading(const String & heading);

					/**
					 * Sets the maximum width of this column in pixels, or 0, if there is no limit.
					 * @param maximumWidth The maximum width of this column.
					 */
					void setMaximumWidth(int maximumWidth);

					/**
					 * Sets the minimum width of this column in pixels.
					 * @param minimumWidth The minimum width of this column.
					 */
					void setMinimumWidth(int minimumWidth);

					/**
					 * Sets whether this column is currently visible. Defaults to true.
					 * @param visible Whether this column is currently visible.
					 */
					void setVisible(bool visible);

					/**
					 * Sets the width of this column in pixels.
					 * @param width The width of this column.
					 */
					void setWidth(int width);
			};

			/**
			 * An effect that opens the edit popup for an element, if no double-click cancels it.
			 */
			class EditEffect : public TimedEffect
			{
				private:

					/**
					 * The rows field to call the onEdit() method on.
					 */
					RowsField * rowsField;

				protected:

					/**
					 * Performs operations when the total time of the effect has elapsed.
					 * The method can decide whether to restart the effect (loop).
					 * The default implementation does not.
					 * @param context The drawing context that specifies the surface and clipping
					 * rectangle for the drawing operation.
					 * @return Whether to restart the effect. In this case, the onStart()
					 * and execute() methods are called again, and the time of the execute()
					 * method is the remainder of the time (i.e. the frame time minus the
					 * time passed to the execute() method of the previous loop.
					 * @note This method is called by the Desktop. Do not call it directly.
					 * To cancel the effect, use cancel().
					 */
					virtual bool onEnd(DrawingContext & context);

				public:

					/**
					 * Creates a new edit effect.
					 * @param rowsField The rows field to call the onEdit() method on.
					 * @param time The total time of the effect in milli seconds.
					 */
					EditEffect(RowsField * rowsField, long time);
			};

			/**
			 * The listener interface for list field events.
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
					 * Called when a cell has been clicked.
					 * @param field The list field the field element belongs to.
					 * @param rowNo The number of the row the field element belongs to.
					 * @param columnNo The number of the column the field element belongs to.
					 * @param elementNo The number of the field element that has been clicked.
					 * May be -1, if no element has been clicked directly.
					 * @return Whether the listener has handled the event, so that the row
					 * should not change its selection.
					 */
					virtual bool onCellClicked(ListField * field, int rowNo, int columnNo, int elementNo);

					/**
					 * Called when a cell has been double-clicked.
					 * @param field The list field the field element belongs to.
					 * @param rowNo The number of the row the field element belongs to.
					 * @param columnNo The number of the column the field element belongs to.
					 * @param elementNo The number of the field element that has been clicked.
					 * May be -1, if no element has been clicked directly.
					 */
					virtual void onCellDoubleClicked(ListField * field, int rowNo, int columnNo, int elementNo);

					/**
					 * Called when the pointer is hovered over a cell of the list field for a while.
					 * @param field The list field that received the event.
					 * @param rowNo The 0-indexed number of the hovered row. May be -1 for out-of-range hovers.
					 * @param columnNo The 0-indexed number of the hovered column. May be -1 for out-of-range hovers.
					 * @param elementNo The 0-indexed number of the hovered element within the cell.
					 * May be -1 for out-of-range hovers.
					 */
					virtual void onCellHover(ListField * field, int rowNo, int columnNo, int elementNo);

					/**
					 * Called when a column of a list field has just been added.
					 * @param field The list field that received the event.
					 * @param columnNo The number of the column that has been added.
					 */
					virtual void onColumnAdded(ListField * field, int columnNo);

					/**
					 * Called when a column header has been clicked.
					 * @param field The list field that received the event.
					 * @param columnNo The number of the column that has been clicked.
					 */
					virtual void onColumnClicked(ListField * field, int columnNo);

					/**
					 * Called when a column of a list field has just been removed.
					 * @param field The list field that received the event.
					 */
					virtual void onColumnRemoved(ListField * field);

					/**
					 * Called when a column of a list field is about to be removed.
					 * @param field The list field that received the event.
					 * @param columnNo The number of the column that will be removed.
					 */
					virtual void onColumnRemoving(ListField * field, int columnNo);

					/**
					 * Called when a column of a list field has just been resized.
					 * @param field The list field that received the event.
					 * @param columnNo The number of the column that has been resized.
					 * @param oldWidth The old width of the column, before resizing.
					 */
					virtual void onColumnResized(ListField * field, int columnNo, int oldWidth);

					/**
					 * Called when the pointer is hovered over the headline of the list field for a while.
					 * @param field The list field that received the event.
					 * @param columnNo The 0-indexed number of the hovered column. May be -1 for out-of-range hovers.
					 */
					virtual void onHeadlineHover(ListField * field, int columnNo);

					/**
					 * Called when a row of a list field has just been added.
					 * @param field The list field that received the event.
					 * @param rowNo The number of the row that has been added.
					 */
					virtual void onRowAdded(ListField * field, int rowNo);

					/**
					 * Called when one or more rows have been removed.
					 * @param field The list field that received the event.
					 * @see onRowsRemoving()
					 */
					virtual void onRowsRemoved(ListField * field);

					/**
					 * Called when a row is about to be removed.
					 * The listener may cancel this event by vetoing it.
					 * @param field The list field that received the event.
					 * @param rowNos The positions of the rows to be removed.
					 * @param rowCount The number of rows to be removed.
					 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
					 * @see VetoMode
					 * @see onRowsRemoved()
					 */
					virtual VetoMode onRowsRemoving(ListField * field, int * rowNos, int rowCount);

					/**
					 * Called when the selection of a list field has just changed.
					 * @param field The list field that changed.
					 */
					virtual void onSelectionChanged(ListField * field);
			};

			class Row : public GlazieryObject
			{
				friend ArrayList<Row>;
				friend ListField;
				friend RowsField;

				private:

					/**
					 * Whether the row is active, i.e. it may be selected. Defaults to true.
					 */
					bool active;

					/**
					 * The cells of this row, one cell for each column,
					 * ordered the same way the columns of the list field are.
					 * @note If the number of columns is larger than a row's number of cells,
					 * the remaining columns are displayed emptily.
					 */
					ArrayList<Cell> cells;

					/**
					 * The list field this row belongs to.
					 */
					ListField * field;

					/**
					 * Destroys the row.
					 */
					~Row();

					/**
					 * Sets the list field this row belongs to.
					 * An exception is thrown, if the property has already been set.
					 * @param field The list field this row belongs to.
					 */
					 void setField(ListField * field);

				public:

					/**
					 * Creates a new row.
					 */
					Row();

					/**
					 * Appends a cell at the end of the row.
					 * The cells define the content of this row, one cell for each column,
					 * ordered the same way the columns of the list field are.
					 * @param cell The cell to be appended.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 * @note If the number of columns is larger than a row's number of cells,
					 * the remaining columns are displayed emptily.
					 */
					void appendCell(Cell * cell);

					/**
					 * Returns a cell of this row.
					 * The cells are ordered the same way the columns of the list field are.
					 * @param The 0-indexed cell (or column) number.
					 * @return The cell or NULL, if the index is out of bounds.
					 */
					Cell * getCell(int cellNo);

					/**
					 * Returns the cells of this row, one cell for each column,
					 * ordered the same way the columns of the list field are.
					 * @return The cells of this row.
					 * @note If the number of columns is larger than a row's number of cells,
					 * the remaining columns are displayed emptily.
					 */
					const ArrayList<Cell> & getCells();

					/**
					 * Returns the list field this row belongs to.
					 * @return The list field this row belongs to.
					 */
					ListField * getField();

					/**
					 * Inserts a cell at a given position.
					 * The cells define the content of this row, one cell for each column,
					 * ordered the same way the columns of the list field are.
					 * @param position the 0-indexed column position to insert the cell at.
					 * @param cell The cell to be inserted.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 * @note If the number of columns is larger than a row's number of cells,
					 * the remaining columns are displayed emptily.
					 */
					void insertCell(int position, Cell * cell);

					/**
					 * Returns whether the row is active, i.e. it may be selected. Defaults to true.
					 * @return Whether the row is active.
					 */
					bool isActive();

					/**
					 * Returns whether this row is selected in its list field.
					 * @return Whether this row is selected.
					 */
					bool isSelected();

					/**
					 * Prepends a cell at the beginning of the row.
					 * The cells define the content of this row, one cell for each column,
					 * ordered the same way the columns of the list field are.
					 * @param cell The cell to be added.
					 * @note After the call, the memory is owned by this object.
					 * You should not delete it, it will be done automatically.
					 * @note If the number of columns is larger than a row's number of cells,
					 * the remaining columns are displayed emptily.
					 */
					void prependCell(Cell * cell);

					/**
					 * Removes a cell at a given position.
					 * The cells define the content of this row, one cell for each column,
					 * ordered the same way the columns of the list field are.
					 * @param position the 0-indexed column position of the cell to be removed.
					 * @note If the number of columns is larger than a row's number of cells,
					 * the remaining columns are displayed emptily.
					 */
					void removeCell(int position);

					/**
					 * Removes all cells of this row.
					 */
					void removeCells();

					/**
					 * Adds this row to the set of selected rows of the list field,
					 * if the row is not yet selected.
					 * If it does not belong to a multi select list field, all other rows are unselected.
					 */
					void select();

					/**
					 * Sets whether the row is active, i.e. it may be selected. Defaults to true.
					 * @param active Whether the row is active.
					 */
					void setActive(bool active);

					/**
					 * Removes this row from the set of selected rows.
					 */
					void unselect();
			};

			/**
			 * The rows field is the scrolling field that displays the rows of
			 * the list field. It is a child of the list field.
			 */
			class RowsField : public ScrollingField
			{
				friend EditEffect;
				friend ListField;

				protected:

					/**
					 * The edit effect, if an element editing is about to come
					 * (and may be canceled by a double-click). NULL if no effect is running.
					 */
					EditEffect * editEffect;

					/**
					 * Whether the last content press issues an onEdit operation on content click.
					 */
					bool editOnClick;

					/**
					 * The number of the column that was clicked last (for double-click operations),
					 * or -1 if none was clicked.
					 */
					int lastClickColumnNo;

					/**
					 * The number of the field element that was clicked last (for double-click operations),
					 * or -1 if none was clicked.
					 */
					int lastClickElementNo;

					/**
					 * The number of the row that was clicked last (for double-click operations),
					 * or -1 if none was clicked.
					 */
					int lastClickRowNo;

					/**
					 * Whether the last content press issues an selection update operation on content click.
					 */
					bool updateSelectionOnClick;

					/**
					 * Creates a new list field rows field.
					 */
					RowsField();

					/**
					 * Destroys the list field rows field.
					 */
					virtual ~RowsField();

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
					 * Handles the event that a the scroll position now value has changed.
					 * @param previousPosition The old scroll position now value.
					 */
					virtual void onScrollPositionNowChanged(Vector previousPosition);

					/**
					 * Selects the current row internally, e.g. when the space key has been pressed.
					 */
					void toggleSelection();

				public:

					/**
					 * Adds items for a context-clock menu. If you leave the menu empty, none appears.
					 * This is the default. Override this method to define your own menu items.
					 * Super-call to include the default menu items of the overridden parent.
					 * @param clickPosition The click position relative to the top-left corner of the component.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @return The new context menu to show, or NULL if no menu shall exists.
					 */
					virtual void addContextMenuItems(Menu * menu, Vector clickPosition, bool option1, bool option2);

					/**
					 * Fills a column and row variable with the indexes of the element at a given position.
					 * If the position is out of range, both variables are filled with -1.
					 * Additionally, fills a variable with the start x position of the column.
					 * @param position The position relative to the list field.
					 * @param columnNo The variable of the column index.
					 * @param columnPos The variable of the column x position.
					 * @param rowNo The variable of the column index.
					 */
					void getColumnAndRowForPosition(Vector position,
						int & columnNo, int & columnPos, int & rowNo);

					/**
					 * Returns the list field this rows field belongs to.
					 * @return The list field this rows field belongs to.
					 */
					ListField * getListField();

					/**
					 * Returns the number of lines to move the focus for one page up or down.
					 * @return The number of page lines.
					 */
					int getPageLinesCount();

					/**
					 * Returns whether the field accepts onPress events to focus it. Defaults to true.
					 * @return Whether the field accepts onPress events to focus it.
					 */
					virtual bool isAcceptingPressesToFocus();

					/**
					 * Returns whether the list field displays a horizontal scroller.
					 * The skin's created widgets return act and the scroller sizes
					 * should adjust according to this information. It may change during run.
					 * @return Whether the list field displays a horizontal scroller.
					 */
					virtual bool isHorizontallyScrollable();

					/**
					 * Returns whether the list field displays a horizontal scroller.
					 * The skin's created widgets return act and the scroller sizes
					 * should adjust according to this information. It may change during run.
					 * @return Whether the list field displays a horizontal scroller.
					 */
					virtual bool isVerticallyScrollable();

					/**
					 * Handles the event that a regular character has been entered
					 * on the event target.
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
					 * Called by the scrolling field to notify the content when the field
					 * is double-clicked (usually using the left mouse button).
					 * @param position The position within the content where the double click
					 * occurred, relative to the upper-left corner of the content.
					 * @param option1 Whether the option button 1 is currently pressed,
					 *	e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onContentDoubleClick(Vector position, bool option1, bool option2);

					/**
					 * Called by the scrolling field to notify the content when the field
					 * is pressed down (usually using the left mouse button).
					 * @param position The position within the content where the press
					 * occurred, relative to the upper-left corner of the content.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onContentPress()
					 * @see onContentRelease()
					 */
					virtual void onContentPress(Vector position, bool option1, bool option2);

					/**
					 * Handles the event that the event target is context-clicked
					 * (usually using the right mouse button).
					 * @param position The position where the context click occurred
					 * relative to the upper-left corner of the event target's origin.
					 * For components, this is their absolute position,
					 * for widgets, this is the absolute position of their component.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 */
					virtual void onContextClick(Vector position, bool option1, bool option2);
					
					/**
					 * Handles the event that the DELETE key (forward delete)
					 * is pressed on the event target.
					 * @return Whether the event target has consumed the event.
					 * If false, the event is first passed to the desktop
					 * and then to the input manager to increase event quantities.
					 */
					virtual bool onDelete();

					/**
					 * Handles the event that a dragging operation started.
					 * This is when the component was pressed using the primary pointer
					 * button and then the pointer has been moved a little.
					 * @param startPosition The absolute position where the dragging started.
					 * @return The dragging surrogate to be drawn next to the pointer
					 * while the dragging operation is running. This surrogate is a representative
					 * of the dragged object (event target, list field row, etc.).
					 * NULL indicates that dragging is not supported, which is the default.
					 * The DraggingSurrogate class itself can be used to support dragging
					 * without drawing a different pointer etc.
					 */
					virtual DraggingSurrogate * onDragStart(const Vector & startPosition);

					/**
					 * Handles the event that the EDIT key is pressed on the event target.
					 * On Windows systems, this is the F2 key.
					 * @return Whether the event target has consumed the event.
					 * If false, the event is first passed to the desktop
					 * and then to the input manager to increase event quantities.
					 */
					virtual bool onEdit();

					/**
					 * Handles the event that the pointer is hovered over the event target for a while.
					 */
					virtual void onHover();

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
					 * Handles the event that the event target is pressed down
					 * (usually using the left mouse button).
					 * @param position The position where the click occurred
					 * relative to the upper-left corner of the event target.
					 * @param option1 Whether the option button 1 is currently pressed,
					 * e.g. on Windows systems, this is the SHIFT key.
					 * @param option2 Whether the option button 2 is currently pressed,
					 * e.g. on Windows systems, this is the CTRL key.
					 * @see onRelease()
					 */
					virtual void onPress(Vector position, bool option1, bool option2);

					/**
					 * Handles the event that the user requested to select all
					 * (e.g. by pressing Ctrl+A on Windows systems).
					 * @return Whether the event target has consumed the event.
					 * If false, the event is first passed to the desktop
					 * and then to the input manager to increase event quantities.
					 */
					virtual bool onSelectAll();

					/**
					 * Handles the event that a pointer wheel (mouse, joystick, etc.)
					 * is changed while the mouse is over this event target.
					 * @param delta The relative wheel movement.
					 */
					virtual void onWheel(int delta);

					/**
					 * Returns a string reprensentation of this object.
					 * @return The string representation.
					 */
					virtual String toString();

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

			enum Type
			{
				/**
				 * The user must choose exactly one row.
				 */
				TYPE_ONE,

				/**
				 * The user must choose one row or no row at all.
				 */
				TYPE_ONE_OR_NONE,

				/**
				 * The user may choose an arbitrary number of rows.
				 */
				TYPE_MANY
			};

		friend Selector;
		friend RowsField;

		private:

			/**
			 * Whether the user can resize the columns, e.g. dragging the heading column border.
			 * The actual UI is defined by the skin. Defaults to false.
			 */
			bool columnsMovable;

			/**
			 * Whether the user can resize the columns, e.g. dragging the heading column border.
			 * The actual UI is defined by the skin. Defaults to true.
			 */
			bool columnsResizable;

			/**
			 * The columns of this list field, defining width and heading.
			 * @note If the number of columns is larger than a row's number of cells,
			 * the remaining columns are displayed emptily.
			 */
			ArrayList<Column> columns;

			/**
			 * The 0-indexed column number that has the focus (the cursor is own).
			 */
			int focusColumnNo;

			/**
			 * The 0-indexed row number that has the focus (the cursor is own).
			 */
			int focusRowNo;

			/**
			 * Whether to display the column headers line. Defaults to true.
			 */
			bool headlined;

			/**
			 * Whether the list field can be scrolled horizontally. Defaults to true.
			 * If not, the content is resized together with the list field.
			 */
			bool horizontallyScrollable;

			/**
			 * The 0-indexed row number where the range selection starts.
			 */
			int rangeStartRowNo;

			/**
			 * Whether the user may remove rows (e.g. by pressing the delete key).
			 * Defaults to true.
			 */
			bool rowsRemovable;

			/**
			 * The rows of this list field.
			 */
			ArrayList<Row> rows;

			/**
			 * The scrolling field that displays the rows of
			 * the list field. It is a child of the list field.
			 */
			RowsField rowsField;

			/**
			 * The currently selected rows of this list field.
			 */
			HashSet selectedRows;

			/**
			 * The selection type (e.g. whether to select multiple rows). Defaults to TYPE_MANY.
			 */
			Type type;

			/**
			 * Whether the list field can be scrolled vertically. Defaults to true.
			 * If not, the content is resized together with the list field.
			 */
			bool verticallyScrollable;

		protected:

			/**
			 * Destroys the list field.
			 */
			virtual ~ListField();

			/**
			 * Adjusts all auto-resizing columns according to the width of the field,
			 * after having substracted the width of the not auto-resizing columns.
			 */
			void adjustAutoColumns();

			/**
			 * Performs final operations when removing rows.
			 * @param selectionChanged Whether the selection changed.
			 */
			void finishRowRemoval(bool selectionChanged);

			/**
			 * Fills a column and row variable with the indexes of the element at a given position.
			 * If the position is out of range, both variables are filled with -1.
			 * Additionally, fills a variable with the start x position of the column.
			 * @param position The position in the rows field content area.
			 * @param columnNo The variable of the column index.
			 * @param columnPos The variable of the column x position.
			 * @param rowNo The variable of the column index.
			 */
			void getColumnAndRowForPosition(Vector position,
				int & columnNo, int & columnPos, int & rowNo);

			/**
			 * Notifies all listeners about that a column has just been added.
			 * @param columnNo The number of the column that has been added.
			 */
			void notifyColumnAdded(int columnNo);

			/**
			 * Notifies all listeners about that a column has just been removed.
			 */
			void notifyColumnRemoved();

			/**
			 * Notifies all listeners about that a column is about to be removed.
			 * @param columnNo The number of the column that will be removed.
			 */
			void notifyColumnRemoving(int columnNo);

			/**
			 * Notifies all listeners about that a row has just been added.
			 * @param rowNo The number of the row that has been added.
			 */
			void notifyRowAdded(int rowNo);

			/**
			 * Notifies all listeners about rows about to be removed.
			 * @param rowNos The positions of the rows to be removed.
			 * @param rowCount The number of rows to be removed.
			 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
			 * @see VetoMode
			 */
			bool notifyRowsRemoving(int * rowNos, int rowCount);

			/**
			 * Notifies all list field listeners that the selection of the field has changed.
			 */
			void notifySelectionChanged();

			/**
			 * Handles the event that the pointer is hovered over a cell or element for a while.
			 * @param rowNo The 0-indexed number of the hovered row.
			 * @param columnNo The 0-indexed number of the hovered column.
			 * @param elementNo The 0-indexed number of the hovered element within the cell.
			 * May be -1, if the pointer is hovered over a cell, but not over a cell element.
			 */
			virtual void onCellHover(int rowNo, int columnNo, int elementNo);

			/**
			 * Handles the event that the event target is clicked
			 * (usually using the left mouse button).
			 * A click is raised after the mouse has been pressed and
			 * released, and no dragging operation has started.
			 * @param position The position where the click occurred
			 * relative to the upper-left corner of the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @see onPress()
			 * @see onRelease()
			 * @see onDoubleClick()
			 */
			virtual void onClick(Vector position, bool option1, bool option2);

			/**
			 * Notifies the component that the maximum size of one of its children has changed.
			 * The default implementation does nothing.
			 * @param child The child that changed.
			 * @param oldMaximumSize The old maximum size.
			 */
			virtual void onChildMaximumSizeChanged(Component * child, Vector oldMaximumSize);

			/**
			 * Notifies the component that the minimum size of one of its children has changed.
			 * The default implementation does nothing.
			 * @param child The child that changed.
			 * @param oldMaximumSize The old minimum size.
			 */
			virtual void onChildMinimumSizeChanged(Component * child, Vector oldMinimumSize);

			/**
			 * Notifies the component that the maximum size has changed.
			 * The default implementation does nothing.
			 * @param oldMinimumSize The old maximum size.
			 */
			virtual void onMaximumSizeChanged(Vector oldMaximumSize);

			/**
			 * Notifies the component that the minimum size has changed.
			 * The default implementation does nothing.
			 * @param oldMinimumSize The old minimum size.
			 */
			virtual void onMinimumSizeChanged(Vector oldMinimumSize);

			/**
			 * Resizes the component and handles listeners.
			 * This method is called internally by the resize overload,
			 * and by parents resizing a child.
			 * @param size The new size.
			 * @param notifyParent false, if the method has been called
			 * from the parent (e.g. onChildMoved() or onChildResized()).
			 * In this case, the child event handlers are not called again,
			 * to prevent an endless recursion or other inappropriate behavior.
			 * @return Whether the component was actually resized.
			 * @see resize()
			 * @see onChildMoved()
			 * @see onChildResized()
			 */
			virtual bool resizeInternal(Vector size, bool notifyParent);

			/**
			 * Updates the selected rows and range selection start after the
			 * focus row having moved.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 */
			void updateSelectionFromFocus(bool option1, bool option2);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ListField);
			#endif

		public:

			/**
			 * Creates a new list field.
			 */
			ListField();

			/**
			 * Creates a new context menu for a context-click on a column.
			 * If you return NULL, no context menu appears. This is the default.
			 * Override this method to define your own menu items.
			 * Super-call to include the default menu items of the overridden parent.
			 * @param menu The context menu.
			 * @param column The clicked column. May the NULL, in this case
			 * the column header was clicked, but no column in the header.
			 * @param position The position relative to the top-left position of the component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return The new context menu to show, or NULL if no menu shall exists.
			 */
			virtual void addColumnContextMenuItems(Menu * menu, Column * column, Vector position,
				bool option1, bool option2);

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
			 * Creates a new context menu for a context-click on the rows.
			 * If you return NULL, no context menu appears. This is the default.
			 * Override this method to define your own menu items.
			 * Super-call to include the default menu items of the overridden parent.
			 * @param menu The context menu.
			 * @param position The position relative to the top-left position of the component.
			 * @param option1 Whether the option button 1 is currently pressed,
			 * e.g. on Windows systems, this is the SHIFT key.
			 * @param option2 Whether the option button 2 is currently pressed,
			 * e.g. on Windows systems, this is the CTRL key.
			 * @return The new context menu to show, or NULL if no menu shall exists.
			 */
			virtual void addRowsContextMenuItems(Menu * menu, Vector position, bool option1, bool option2);

			/**
			 * This method adjusts the position and size of the rows field
			 * according to the list field's position, size, and skin properties.
			 */
			void adjustRowsField();

			/**
			 * Appends a column, defining width and heading, at the end of the columns.
			 * @param column The column to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 * @note If the number of columns is larger than a row's number of cells,
			 * the remaining columns are displayed emptily.
			 */
			void appendColumn(Column * column);

			/**
			 * Appends a row at the end of the rows.
			 * @param row The row to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void appendRow(Row * row);

			/**
			 * Draws the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The caller is responsible for not calling this method
			 * if the component is invisible.
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Ensures that a given cell is entirely visible by setting the scroll position.
			 * If the focus cannot be shown entirely, then the top-left corner is ensured to be shown.
			 * @param now Whether to by-pass the skin effects, setting the focus now. Defaults to false.
			 */
			void ensureCellVisible(int rowNo, int columnNo, bool now = false);

			/**
			 * Ensures that the focus is entirely visible by setting the scroll position.
			 * If the focus cannot be shown entirely, then the top-left corner is ensured to be shown.
			 * @param now Whether to by-pass the skin effects, setting the focus now. Defaults to false.
			 */
			void ensureFocusVisible(bool now = false);

			/**
			 * Returns a cell at a given position.
			 * @param rowNo The 0-indexed row number.
			 * @param columnNo The 0-indexed column number.
			 * @return The cell or NULL, if the row and/or column number is out of range.
			 */
			Cell * getCell(int rowNo, int cellNo);

			/**
			 * Returns a column at a given position.
			 * @param columnNo The 0-indexed column number.
			 * @return The row or NULL, if the column number is out of range.
			 */
			Column * getColumn(int columnNo);

			/**
			 * Returns the number of columns currently in this list.
			 * @return The number of columns currently in this list.
			 */
			int getColumnCount();

			/**
			 * Returns the 0-indexed number of a given column by linearily searching through the columns.
			 * @param column The column to be searched.
			 * @return The 0-indexed column number or -1, if the column cannot be found.
			 * @warn The method searched linearily through the columns. Avoid on large lists.
			 */
			int getColumnNo(Column * column);

			/**
			 * Returns the columns of this list field, defining width and heading.
			 * @note If the number of columns is larger than a row's number of cells,
			 * the remaining columns are displayed emptily.
			 * @return The columns of this list field
			 */
			const ArrayList<Column> & getColumns();

			/**
			 * Returns the event target at a given position that is setup to
			 * receive pointer events. For popup components, this is always true,
			 * for windows, this is true if and only if the pointer event mode is
			 * active or inactive, but bypassing windows are treated as not
			 * existing (transparent for events), and for widgets, this depends on
			 * the widget's hit test (isHitAt()).<br>
			 *
			 * This is a deep search: The desktop delegates to the found
			 * window or popup component, which delegates to the client area
			 * and so on.
			 * @param position The position relative to the event target's origin.
			 * For components, this is their absolute position,
			 * for widgets, this is the absolute position of their component.
			 * @return The event receiving event target at the position or
			 * <code>NULL</code>, if there is no target.
			 */
			virtual EventTarget * getEventTargetAt(Vector position);

			/**
			 * Returns the child component that has focus within this parent.
			 * @return The child component that has focus within this parent
			 * or NULL, if this component has no children or if focusing is
			 * not subject of this component.
			 */
			virtual Component * getFocusChild();

			/**
			 * Returns the column that currently has focus or NULL, if no column has focus.
			 * @return The column that currently has focus or NULL.
			 */
			Column * getFocusColumn();

			/**
			 * Returns the 0-indexed column number that has the focus (the cursor is own).
			 * @return The 0-indexed column number that has the focus.
			 */
			int getFocusColumnNo();

			/**
			 * Returns the cell that currently has focus or NULL, if no cell has focus.
			 * @return The cell that currently has focus or NULL.
			 */
			Cell * getFocusCell();

			/**
			 * Returns the row that currently has focus or NULL, if no row has focus.
			 * @return The row that currently has focus or NULL.
			 */
			Row * getFocusRow();

			/**
			 * Returns the 0-indexed row number that has the focus (the cursor is own).
			 * @return The 0-indexed row number that has the focus.
			 */
			int getFocusRowNo();

			/**
			 * Fills a column variable with the index of the headline column at a given position.
			 * If the position is not within the headline, the variable is filled with -1.
			 * @param position The position relative to the list field.
			 * @param columnNo The variable of the column index.
			 */
			void getHeadlineColumnForPosition(Vector position, int & columnNo);

			/**
			 * Returns a row at a given position.
			 * @param rowNo The 0-indexed row number.
			 * @return The row or NULL, if the row number is out of range.
			 */
			Row * getRow(int rowNo);

			/**
			 * Returns the number of rows currently in this list.
			 * @return The number of rows currently in this list.
			 */
			int getRowCount();

			/**
			 * Returns the 0-indexed number of a given row by linearily searching through the rows.
			 * @param row The row to be searched.
			 * @return The 0-indexed row number or -1, if the row cannot be found.
			 * @warn The method searched linearily through the rows. Avoid on large lists.
			 */
			int getRowNo(Row * row);

			/**
			 * Returns the 0-indexed number of a row with a given model by linearily searching through the rows.
			 * @param model The row's model to be searched.
			 * @return The 0-indexed row number or -1, if the model cannot be found.
			 * @warn The method searched linearily through the rows. Avoid on large lists.
			 */
			int getRowNoByModel(void * model);

			/**
			 * Returns the rows of this list field.
			 * @return The rows of this list field.
			 */
			const ArrayList<Row> & getRows();

			/**
			 * Returns the scrolling field that displays the rows of
			 * the list field. It is a child of the list field.
			 * It is usually not required to operate on the rows field.
			 * @return The rows scrolling field.
			 */
			RowsField * getRowsField();

			/**
			 * Returns the total width of all columns, including the border width
			 * between the columns, as defines by the skin.
			 * @return The total width of all columns.
			 */
			int getRowWidth();

			/**
			 * Returns the currently selected row, if exactly one row is selected.
			 * Otherwise, returns NULL.
			 * @return The currently selected row or NULL.
			 */
			Row * getSelectedRow();

			/**
			 * Returns the currently selected rows of this list field in no particular order.
			 * @note This method is fast, as it returns the internal set of selected rows.
			 * Use getSelectedRowsInOrder() to get the rows in the list field's order.
			 * @return The currently selected rows of this list field.
			 */
			const HashSet & getSelectedRows();

			/**
			 * Returns the currently selected rows of this list field ordered by the list field rows' order.
			 * @note This method is slow, as it iterates over the list field rows.
			 * Use getSelectedRows() on large lists, where no order is necessary.
			 * @warn The method creates a new ArrayList and expects that the caller unlinks all elements
			 * and then destroys the list after use.
			 * @return The currently selected rows of this list field.
			 * UnlinkAll elements and delete the ArrayList after use.
			 */
			ArrayList<ListField::Row> * getSelectedRowsInOrder();

			/**
			 * Returns the selection type (e.g. whether to select multiple rows). Defaults to TYPE_MANY.
			 * @return The selection type.
			 */
			Type getType();

			/**
			 * Inserts a column at a given position.
			 * @param position The 0-indexed position of the column.
			 * @param column The column to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void insertColumn(int position, Column * column);

			/**
			 * Inserts a row at a given position.
			 * @param position The 0-indexed position of the row.
			 * @param row The row to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void insertRow(int position, Row * row);

			/**
			 * Returns whether the user can move the columns, e.g. dragging a column heading.
			 * The actual UI is defined by the skin. Defaults to false.
			 * @return Whether the user can move the columns.
			 */
			bool isColumnsMovable();

			/**
			 * Returns whether the user can resize the columns, e.g. dragging the heading column border.
			 * The actual UI is defined by the skin. Defaults to true.
			 * @return Whether the user can resize the columns.
			 */
			bool isColumnsResizable();

			/**
			 * Returns whether the list currently is empty (has no rows).
			 * @return Whether the list currently is empty (has no rows).
			 */
			bool isEmpty();

			/**
			 * Returns whether to display the column headers line. Defaults to true.
			 * @return Whether to display the column headers line.
			 */
			bool isHeadlined();

			/**
			 * Returns whether the list field displays a horizontal scroller.
			 * The skin's created widgets return act and the scroller sizes
			 * should adjust according to this information. It may change during run.
			 * @return Whether the list field displays a horizontal scroller.
			 */
			bool isHorizontallyScrollable();

			/**
			 * Returns whether a given row is currently selected.
			 * @param rowNo The number of the row to be checked.
			 * @return Whether the row is selected.
			 */
			bool isRowSelected(int rowNo);

			/**
			 * Returns whether a given row is currently selected.
			 * @param row The row to be checked.
			 * @return Whether the row is selected.
			 */
			bool isRowSelected(Row * row);

			/**
			 * Whether the user may remove rows (e.g. by pressing the delete key).
			 * Defaults to true.
			 * @return Whether the user may remove rows.
			 */
			bool isRowsRemovable();

			/**
			 * Returns whether the list field displays a horizontal scroller.
			 * The skin's created widgets return act and the scroller sizes
			 * should adjust according to this information. It may change during run.
			 * @return Whether the list field displays a horizontal scroller.
			 */
			bool isVerticallyScrollable();

			/**
			 * Moves a row from a given position to another position.
			 * @param fromPosition The from position of the row.
			 * @param toPosition The to position of the row.
			 */
			void moveRow(int fromPosition, int toPosition);

			/**
			 * Performs operations necessary when a child component
			 * has been moved within this parent. E.g. move the parent,
			 * too, if the child uses all the area of the parent, or
			 * ignore the event if the child can move freely.
			 * @param child The child component that moved.
			 * @param oldPosition The position of the child before the move.
			 */
			virtual void onChildMoved(Component * child, Vector oldPosition);

			/**
			 * Performs operations necessary when a child component
			 * has been resized within this parent. E.g. resize the parent,
			 * too, if the child uses all the area of the parent, or
			 * ignore the event if the child can resize freely.
			 * @param child The child component that resized.
			 * @param oldSize The size of the child before the resize.
			 */
			virtual void onChildResized(Component * child, Vector oldSize);

			/**
			 * Returns the dragging surrogate to be used if the list field supports dragging of a particular row.
			 * Defaults to NULL (not supporting dragging) for all rows.
			 * @param rowNo The number of the row to be dragged.
			 * @warn Beware of changing list fields while dragging!
			 * Either copy the dragged object when dragging starts,
			 * or memorize the row number only amd register as listener to the list field,
			 * adjusting the row number if rows before this row are added/removed,
			 * and cancelling the dragging if this row is removed (Desktop::cancelDragging()).
			 * @note If the list field supports both cell and row dragging, cell dragging wins.
			 * Returns whether the list field supports dragging of a particular row.
			 * @note Override this method to change the behavior.
			 * @return The dragging surrogate to be drawn next to the pointer
			 * while the dragging operation is running. This surrogate is a representative
			 * of the dragged object (event target, list field row, etc.).
			 * NULL indicates that dragging is not supported, which is the default.
			 * The DraggingSurrogate class itself can be used to support dragging
			 * without drawing a different pointer etc.
			 * @see Desktop::cancelDragging()
			 */
			virtual DraggingSurrogate * onDragStart(int rowNo);

			/**
			 * Returns the dragging surrogate to be used if the list field supports dragging of a particular cell.
			 * Defaults to NULL (not supporting dragging) for all cells.
			 * @param rowNo The row number of the cell to be dragged.
			 * @param columnNo The column number of the cell to be dragged.
			 * @warn Beware of changing list fields while dragging!
			 * Either copy the dragged object when dragging starts,
			 * or memorize the row number only amd register as listener to the list field,
			 * adjusting the row number if rows before this row are added/removed,
			 * and cancelling the dragging if this row is removed (Desktop::cancelDragging()).
			 * @note If the list field supports both cell and row dragging, cell dragging wins.
			 * Returns whether the list field supports dragging of a particular row.
			 * @note Override this method to change the behavior.
			 * @return The dragging surrogate to be drawn next to the pointer
			 * while the dragging operation is running. This surrogate is a representative
			 * of the dragged object (event target, list field row, etc.).
			 * NULL indicates that dragging is not supported, which is the default.
			 * The DraggingSurrogate class itself can be used to support dragging
			 * without drawing a different pointer etc.
			 * @see Desktop::cancelDragging()
			 */
			virtual DraggingSurrogate * onDragStart(int rowNo, int columnNo);

			/**
			 * Handles the event that the pointer is hovered over the event target for a while.
			 */
			virtual void onHover();

			/**
			 * Prepends a column at the beginning of the columns.
			 * @param column The column to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void prependColumn(Column * column);

			/**
			 * Prepends a row at the beginning of the rows.
			 * @param row The row to be added.
			 * @note After the call, the memory is owned by this object.
			 * You should not delete it, it will be done automatically.
			 */
			void prependRow(Row * row);

			/**
			 * Removes a column at a given position.
			 * @param position The position of the column to be deleted.
			 */
			void removeColumn(int position);

			/**
			 * Removes all columns from the list field.
			 */
			void removeColumns();

			/**
			 * Removes a row at a given position from the list field.
			 * @param position The position of the row to be deleted.
			 * @return Whether the removal was performed. Any veto results in false.
			 */
			bool removeRow(int position);

			/**
			 * Removes all rows from the list field.
			 * @return Whether the removal was performed. Any veto results in false.
			 */
			bool removeRows();

			/**
			 * Removes all currently selected rows from the list field.
			 * @return Whether the removal was performed. Any veto results in false.
			 */
			bool removeSelectedRows();

			/**
			 * Adds all rows to the set of selected rows.
			 * If this is no multi select list field, the method does nothing.
			 */
			void selectAllRows();

			/**
			 * Adds a row to the set of selected rows, if the row is not yet selected.
			 * If this is no multi select list field, all other rows are unselected.
			 * @param rowNo The number of the row.
			 */
			void selectRow(int rowNo);

			/**
			 * Adds a row to the set of selected rows, if the row is not yet selected.
			 * If this is no multi select list field, all other rows are unselected.
			 * @param row The row to be selected.
			 * This row must be part of the rows of the list field,
			 * otherwise it will not be selected.
			 */
			void selectRow(Row * row);

			/**
			 * Sets whether this input field can get focus and therefore
			 * can be edited or selected. Not active fields are usually grayed out.
			 * @param active Whether this input field is active.
			 */
			virtual void setActive(bool active);

			/**
			 * Sets whether the user can move the columns, e.g. dragging a column heading.
			 * The actual UI is defined by the skin. Defaults to false.
			 * @param columnsMovable Whether the user can move the columns.
			 */
			void setColumnsMovable(bool columnsMovable);

			/**
			 * Sets whether the user can resize the columns, e.g. dragging the heading column border.
			 * The actual UI is defined by the skin. Defaults to true.
			 * @param columnsResizable Whether the user can resize the columns.
			 */
			void setColumnsResizable(bool columnsResizable);

			/**
			 * Sets the 0-indexed column number that has the focus (the cursor is own).
			 * @param focusColumnNo The 0-indexed column number that has the focus.
			 */
			void setFocusColumnNo(int focusColumnNo);

			/**
			 * Sets the 0-indexed row number that has the focus (the cursor is own).
			 * @param focusRowNo The 0-indexed row number that has the focus.
			 */
			void setFocusRowNo(int focusRowNo);

			/**
			 * Sets whether to display the column headers line. Defaults to true.
			 * @param headlined Whether to display the column headers line.
			 */
			void setHeadlined(bool headlined);

			/**
			 * Sets whether the list field can be scrolled horizontally. Defaults to true.
			 * If not, the content is resized together with the list field.
			 * @param horizontallyScrollable Whether the list field can be scrolled horizontally.
			 */
			void setHorizontallyScrollable(bool horizontallyScrollable);

			/**
			 * Sets whether the user may remove rows (e.g. by pressing the delete key).
			 * Defaults to true.
			 * @param rowsRemovable Whether the user may remove rows.
			 */
			void setRowsRemovable(bool rowsRemovable);

			/**
			 * Sets that a given row is solely selected.
			 * @param rowNo The number of the row.
			 */
			void setSelectedRow(int rowNo);

			/**
			 * Sets that a given row is solely selected.
			 * @param row The row to be selected.
			 * This row must be part of the rows of the list field,
			 * otherwise it will not be selected.
			 */
			void setSelectedRow(Row * row);

			/**
			 * Sets the selected rows of this list field.
			 * @param selectedRows The selected rows.
			 * These rows must be part of the rows of the list field,
			 * otherwise it will not be selected.
			 */
			void setSelectedRows(const HashSet & selectedRows);

			/**
			 * Sets the selection type (e.g. whether to select multiple rows). Defaults to TYPE_MANY.
			 * @param type The selection type.
			 */
			void setType(Type type);

			/**
			 * Sets whether the list field can be scrolled vertically. Defaults to true.
			 * If not, the content is resized together with the list field.
			 * @param verticallyScrollable Whether the list field can be scrolled vertically.
			 */
			void setVerticallyScrollable(bool verticallyScrollable);

			/**
			 * Shows a balloon popup for this field, containing a given text.
			 * @param text The text to show.
			 * @return The balloon popup.
			 */
			BalloonPopup * showBalloonPopup(const String & text);

			/**
			 * Shows a balloon popup for this list field, containing a given text.
			 * The popup points at a given cell.
			 * @param rowNo The 0-indexed number of the row.
			 * @param columnNo The 0-indexed number of the column.
			 * @param text The text to show.
			 * @return The balloon popup.
			 */
			BalloonPopup * showBalloonPopup(int rowNo, int columnNo, const String & text);

			/**
			 * Shows a balloon popup for this list field, containing a given text.
			 * The popup points at a given cell element.
			 * @param rowNo The 0-indexed number of the row.
			 * @param columnNo The 0-indexed number of the column.
			 * @param text The text to show.
			 * @return The balloon popup.
			 */
			BalloonPopup * showBalloonPopup(int rowNo, int columnNo, int elementNo, const String & text);

			/**
			 * Sorts the rows by a given compare function.
			 * @param compare A functions that compares two rows and returns -1,
			 * if the first is before the second, 0, if both rows are equal and 1,
			 * if the first is after than the second.
			 */
			void sortRows(int (*compare)(Row * row1, Row * row2));

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();

			/**
			 * For tutorials: Creates a pointer effect that moves the pointer to a cell of this list
			 * and clicks the cell, and waits until the effect has ended.
			 * @param rowNo The 0-indexed number of the row.
			 * @param columnNo The 0-indexed number of the column.
			 * @param buttonEffect The button effect (see PointerEffect::ButtonEffect) to be added
			 * (clicking, double-clicking, right-clicking the component). Only BUTTONEFFECT_LEFTCLICK,
			 * BUTTONEFFECT_DOUBLECLICK, and BUTTONEFFECT_RIGHTCLICK is allowed. To drag this component,
			 * use tutorialDragDropTo. Defaults to BUTTONEFFECT_LEFTCLICK;
			 * @param option1 Whether option1 (e.g. Shift) is held during the button effect. Defaults to false.
			 * @param option2 Whether option2 (e.g. Ctrl) is held during the button effect. Defaults to false.
			 * @param time The time in milli seconds for the effect, or leave empty for a default.
			 * @return The newly created tutorial effect.
			 * @note Use this method in Tutorial::run() implementations only.
			 * @see Tutorial
			 */
			void tutorialClick(int rowNo, int columnNo,
				PointerEffect::ButtonEffect buttonEffect = PointerEffect::BUTTONEFFECT_LEFTCLICK,
				bool option1 = false, bool option2 = false, long time = -1);

			/**
			 * For tutorials: Creates a pointer effect that moves the pointer to a cell element of this list
			 * and clicks the element, and waits until the effect has ended.
			 * @param rowNo The 0-indexed number of the row.
			 * @param columnNo The 0-indexed number of the column.
			 * @param elementNo The 0-indexed number of the element.
			 * @param buttonEffect The button effect (see PointerEffect::ButtonEffect) to be added
			 * (clicking, double-clicking, right-clicking the component). Only BUTTONEFFECT_LEFTCLICK,
			 * BUTTONEFFECT_DOUBLECLICK, and BUTTONEFFECT_RIGHTCLICK is allowed. To drag this component,
			 * use tutorialDragDropTo. Defaults to BUTTONEFFECT_LEFTCLICK;
			 * @param option1 Whether option1 (e.g. Shift) is held during the button effect. Defaults to false.
			 * @param option2 Whether option2 (e.g. Ctrl) is held during the button effect. Defaults to false.
			 * @param time The time in milli seconds for the effect, or leave empty for a default.
			 * @return The newly created tutorial effect.
			 * @note Use this method in Tutorial::run() implementations only.
			 * @see Tutorial
			 */
			void tutorialClick(int rowNo, int columnNo, int elementNo,
				PointerEffect::ButtonEffect buttonEffect = PointerEffect::BUTTONEFFECT_LEFTCLICK,
				bool option1 = false, bool option2 = false, long time = -1);

			/**
			 * Removes a row from the set of selected rows.
			 * @param row The row to be unselected.
			 */
			void unselectRow(Row * row);

			/**
			 * Removes all rows from the set of selected rows, so no row is selected anymore.
			 */
			void unselectRows();
	};
}


#endif
