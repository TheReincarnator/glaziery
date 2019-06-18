/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


GridLayouter::GridLayouter()
{
	ASSERTION_COBJECT(this);
}

GridLayouter::~GridLayouter()
{
	ASSERTION_COBJECT(this);
}

int GridLayouter::getMaximumSpacing(ArrayList<Int> * spacings, int columnNo, int columnSpan)
{
	ASSERTION_COBJECT(this);

	int maximumSpacing = 0;
	for (int i=0; i<columnSpan; i++)
	{
		int currentSpacing = spacings->Get(columnNo + i)->Get();
		if (maximumSpacing == 0 || maximumSpacing < currentSpacing)
			maximumSpacing = currentSpacing;
	}

	return maximumSpacing;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(GridLayouter, Layouter);
#endif

bool GridLayouter::isLayouting(Field * field)
{
	ASSERTION_COBJECT(this);

	if (!field->isVisible())
		return false;

	FieldLayoutData * layoutData = field->getFieldLayoutData();
	if (layoutData != NULL && layoutData->isExcluded())
		return false;

	return true;
}

bool GridLayouter::layout(FieldsArea * fieldsArea, bool resizeArea)
{
	ASSERTION_COBJECT(this);

	ContainerLayoutData * containerLayoutData = fieldsArea->getLayoutData();

	const ArrayList<Field> & fields = fieldsArea->getFields();
	ArrayList<Field> topLevelFields;
	for (int i=0; i<fields.GetCount(); i++)
	{
		Field * field = fields.Get(i);
		if (isLayouting(field) && field->getGroupField() == NULL)
			topLevelFields.Append(fields.Get(i));
	}

	bool modified = layoutGroupsAndFields(topLevelFields, fieldsArea,
		containerLayoutData->getTopLeftMargin(),
		containerLayoutData->getBottomRightMargin(),
		containerLayoutData->getColumns(), resizeArea);

	topLevelFields.UnlinkAll();

	return modified;
}

bool GridLayouter::layout(GroupField * groupField, bool resizeGroupField)
{
	ASSERTION_COBJECT(this);

	ContainerLayoutData * containerLayoutData = groupField->getGroupLayoutData();

	Vector topLeftSize;
	Vector bottomRightSize;
	Desktop::getInstance()->getSkin()->getGroupFieldMarginSizes(groupField, topLeftSize, bottomRightSize);

	return layoutGroupsAndFields(groupField->getFields(), groupField,
		containerLayoutData->getTopLeftMargin() + topLeftSize,
		containerLayoutData->getBottomRightMargin() + bottomRightSize,
		containerLayoutData->getColumns(), resizeGroupField);
}

bool GridLayouter::layoutFields(const ArrayList<Field> & fields, Component * container,
	Vector topLeftMargins, Vector bottomRightMargins, int columns, bool resizeContainer)
{
	ASSERTION_COBJECT(this);

	if (fields.IsEmpty())
		return false;

	int fieldCount = fields.GetCount();

	ScrollingArea * scrollingArea = dynamic_cast<ScrollingArea *>(container);
	Vector containerSize;
	if (scrollingArea != NULL)
		containerSize = scrollingArea->getContentViewSize();
	else
		containerSize = container->getSize();

	bool modified = false;

	// Correct requested number of columns to actually required number

	bool oneRowOnly;
	if (columns == 0)
	{
		// If no requested number of column is provided, calculate all fields into one row
		for (int fieldNo=0; fieldNo<fieldCount; fieldNo++)
		{
			Field * field = fields.Get(fieldNo);
			if (!isLayouting(field))
				continue;

			FieldLayoutData * fieldLayoutData = field->getFieldLayoutData();
			if (fieldLayoutData->getColumnSpan() < 1)
				columns++;
			else
				columns += fieldLayoutData->getColumnSpan();
		}

		oneRowOnly = true;
	}
	else
	{
		// Otherwise check whether requested number of column is too big
		int actualMinimumColumns = 0;
		int fieldNo;
		for (fieldNo=0; fieldNo < fieldCount && actualMinimumColumns < columns; fieldNo++)
		{
			Field * field = fields.Get(fieldNo);
			if (!isLayouting(field))
				continue;

			FieldLayoutData * fieldLayoutData = field->getFieldLayoutData();
			if (fieldLayoutData->getColumnSpan() < 1)
				break;
			else
				actualMinimumColumns += fieldLayoutData->getColumnSpan();
		}

		if (fieldNo >= fieldCount && actualMinimumColumns < columns)
			columns = actualMinimumColumns;

		oneRowOnly = false;
	}

	// Initialize the columns and rows lists

	int i;

	ArrayList<Int> columnWidths;
	ArrayList<Int> columnMinimumWidths;
	ArrayList<Int> columnMaximumWidths;
	ArrayList<Int> rowHeights;
	ArrayList<Int> rowMinimumHeights;
	ArrayList<Int> rowMaximumHeights;

	ArrayList<Bool> columnResizings;
	ArrayList<Bool> rowResizings;

	ArrayList<ArrayList<Int> > leftSpacings;
	ArrayList<ArrayList<Int> > rightSpacings;
	ArrayList<ArrayList<Int> > topSpacings;
	ArrayList<ArrayList<Int> > bottomSpacings;

	int columnNo;
	for (columnNo = 0; columnNo < columns; columnNo++)
	{
		Int * columnWidth;
		if ((columnWidth = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		columnWidths.Append(columnWidth);
		if ((columnWidth = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		columnMinimumWidths.Append(columnWidth);
		if ((columnWidth = new Int(4096)) == NULL)
			throw EOUTOFMEMORY;
		columnMaximumWidths.Append(columnWidth);

		Bool * columnResizing;
		if ((columnResizing = new Bool(false)) == NULL)
			throw EOUTOFMEMORY;
		columnResizings.Append(columnResizing);
	}

	int rows = 1;
	Int * rowHeight;
	if ((rowHeight = new Int(0)) == NULL)
		throw EOUTOFMEMORY;
	rowHeights.Append(rowHeight);
	if ((rowHeight = new Int(0)) == NULL)
		throw EOUTOFMEMORY;
	rowMinimumHeights.Append(rowHeight);
	if ((rowHeight = new Int(4096)) == NULL)
		throw EOUTOFMEMORY;
	rowMaximumHeights.Append(rowHeight);

	Bool * rowResizing;
	if ((rowResizing = new Bool(false)) == NULL)
		throw EOUTOFMEMORY;
	rowResizings.Append(rowResizing);

	ArrayList<Int> * spacingsRow;

	if ((spacingsRow = new ArrayList<Int>) == NULL)
		throw EOUTOFMEMORY;
	leftSpacings.Append(spacingsRow);

	if ((spacingsRow = new ArrayList<Int>) == NULL)
		throw EOUTOFMEMORY;
	rightSpacings.Append(spacingsRow);

	if ((spacingsRow = new ArrayList<Int>) == NULL)
		throw EOUTOFMEMORY;
	topSpacings.Append(spacingsRow);

	if ((spacingsRow = new ArrayList<Int>) == NULL)
		throw EOUTOFMEMORY;
	bottomSpacings.Append(spacingsRow);

	// Build spacings arrays out of margins specification of fields layout data

	int rowNo = 0;
	int fieldNo;
	columnNo = 0;

	for (fieldNo=0; fieldNo<fieldCount; fieldNo++)
	{
		Field * field = fields.Get(fieldNo);
		if (!isLayouting(field))
			continue;

		bool last = true;
		for (int fieldNo2 = fieldNo + 1; fieldNo2 < fieldCount; fieldNo2++)
			if (isLayouting(fields.Get(fieldNo2)))
			{
				last = false;
				break;
			}

		FieldLayoutData * fieldLayoutData = field->getFieldLayoutData();

		int columnSpan = fieldLayoutData->getColumnSpan();
		if (oneRowOnly && columnSpan < 1)
			columnSpan = 1;
		if (columnSpan < 1 || columnNo + columnSpan >= columns)
			columnSpan = columns - columnNo;

		for (i=0; i<columnSpan; i++)
		{
			Int * spacing;

			int leftMargin = i > 0 ? 0 : fieldLayoutData->getTopLeftMargin().x;
			int rightMargin = i < columnSpan - 1 ? 0 : fieldLayoutData->getBottomRightMargin().x;

			if ((spacing = new Int(leftMargin)) == NULL)
				throw EOUTOFMEMORY;
			leftSpacings.GetLast()->Append(spacing);

			if ((spacing = new Int(rightMargin)) == NULL)
				throw EOUTOFMEMORY;
			rightSpacings.GetLast()->Append(spacing);

			if ((spacing = new Int(fieldLayoutData->getTopLeftMargin().y)) == NULL)
				throw EOUTOFMEMORY;
			topSpacings.GetLast()->Append(spacing);

			if ((spacing = new Int(fieldLayoutData->getBottomRightMargin().y)) == NULL)
				throw EOUTOFMEMORY;
			bottomSpacings.GetLast()->Append(spacing);
		}

		// Move column and/or row number according to column span

		columnNo += columnSpan;
		if (columnNo >= columns && !last)
		{
			columnNo = 0;
			rowNo++;

			if ((rowHeight = new Int(0)) == NULL)
				throw EOUTOFMEMORY;
			rowHeights.Append(rowHeight);
			if ((rowHeight = new Int(0)) == NULL)
				throw EOUTOFMEMORY;
			rowMinimumHeights.Append(rowHeight);
			if ((rowHeight = new Int(4096)) == NULL)
				throw EOUTOFMEMORY;
			rowMaximumHeights.Append(rowHeight);
			rows++;

			if ((rowResizing = new Bool(false)) == NULL)
				throw EOUTOFMEMORY;
			rowResizings.Append(rowResizing);

			if ((spacingsRow = new ArrayList<Int>) == NULL)
				throw EOUTOFMEMORY;
			leftSpacings.Append(spacingsRow);

			if ((spacingsRow = new ArrayList<Int>) == NULL)
				throw EOUTOFMEMORY;
			rightSpacings.Append(spacingsRow);

			if ((spacingsRow = new ArrayList<Int>) == NULL)
				throw EOUTOFMEMORY;
			topSpacings.Append(spacingsRow);

			if ((spacingsRow = new ArrayList<Int>) == NULL)
				throw EOUTOFMEMORY;
			bottomSpacings.Append(spacingsRow);
		}
	}

	// Fill empty spaces with 0 spacings

	while (columnNo < columns)
	{
		Int * spacing;

		if ((spacing = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		leftSpacings.GetLast()->Append(spacing);

		if ((spacing = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		rightSpacings.GetLast()->Append(spacing);

		if ((spacing = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		topSpacings.GetLast()->Append(spacing);

		if ((spacing = new Int(0)) == NULL)
			throw EOUTOFMEMORY;
		bottomSpacings.GetLast()->Append(spacing);

		columnNo++;
	}

	// Correct spacings by analyzing adjacent cells and considering container margins specification

	for (rowNo = 0; rowNo < rows; rowNo++)
		for (columnNo = 0; columnNo < columns; columnNo++)
		{
			Int * leftSpacing = leftSpacings.Get(rowNo)->Get(columnNo);
			Int * rightSpacing = rightSpacings.Get(rowNo)->Get(columnNo);
			Int * topSpacing = topSpacings.Get(rowNo)->Get(columnNo);
			Int * bottomSpacing = bottomSpacings.Get(rowNo)->Get(columnNo);

			// Correct outer margins (container margin always overrides)

			if (columnNo == 0)
				leftSpacing->Set(topLeftMargins.x);

			if (columnNo == columns - 1)
				rightSpacing->Set(bottomRightMargins.x);

			if (rowNo == 0)
				topSpacing->Set(topLeftMargins.y);

			if (rowNo == rows - 1)
				bottomSpacing->Set(bottomRightMargins.y);

			// Correct inner margins (use maximum of both cells, then distribute among both)

			if (columnNo > 0)
			{
				Int * leftCellRightSpacing = rightSpacings.Get(rowNo)->Get(columnNo - 1);
				int maximum = leftCellRightSpacing->Get() > leftSpacing->Get()
					? leftCellRightSpacing->Get() : leftSpacing->Get();
				leftCellRightSpacing->Set(maximum / 2);
				leftSpacing->Set(maximum - leftCellRightSpacing->Get());
			}

			if (rowNo > 0)
			{
				Int * topCellBottomSpacing = bottomSpacings.Get(rowNo - 1)->Get(columnNo);
				int maximum = topCellBottomSpacing->Get() > topSpacing->Get()
					? topCellBottomSpacing->Get() : topSpacing->Get();
				topCellBottomSpacing->Set(maximum / 2);
				topSpacing->Set(maximum - topCellBottomSpacing->Get());
			}
		}

	// Add padding to spacings

	rowNo = 0;
	columnNo = 0;

	for (fieldNo=0; fieldNo<fieldCount; fieldNo++)
	{
		Field * field = fields.Get(fieldNo);
		if (!isLayouting(field))
			continue;

		bool last = true;
		for (int fieldNo2 = fieldNo + 1; fieldNo2 < fieldCount; fieldNo2++)
			if (isLayouting(fields.Get(fieldNo2)))
			{
				last = false;
				break;
			}

		FieldLayoutData * fieldLayoutData = field->getFieldLayoutData();

		int columnSpan = fieldLayoutData->getColumnSpan();
		if (oneRowOnly && columnSpan < 1)
			columnSpan = 1;
		if (columnSpan < 1 || columnNo + columnSpan >= columns)
			columnSpan = columns - columnNo;

		Int * leftSpacing = leftSpacings.Get(rowNo)->Get(columnNo);
		Int * rightSpacing = rightSpacings.Get(rowNo)->Get(columnNo + columnSpan - 1);
		leftSpacing->Add(fieldLayoutData->getTopLeftPadding().x);
		rightSpacing->Add(fieldLayoutData->getBottomRightPadding().x);

		for (i=0; i<columnSpan; i++)
		{
			Int * topSpacing = topSpacings.Get(rowNo)->Get(columnNo + i);
			Int * bottomSpacing = bottomSpacings.Get(rowNo)->Get(columnNo + i);
			topSpacing->Add(fieldLayoutData->getTopLeftPadding().y);
			bottomSpacing->Add(fieldLayoutData->getBottomRightPadding().y);
		}

		// Move column and/or row number according to column span

		columnNo += columnSpan;
		if (columnNo >= columns && !last)
		{
			columnNo = 0;
			rowNo++;
		}
	}

	// Find minimum, maximum, and desired column widths, use column span 1 first, then 2, and so on

	for (int investigatedColumnSpan = 1; investigatedColumnSpan <= columns; investigatedColumnSpan++)
	{
		rowNo = 0;
		columnNo = 0;

		for (int fieldNo=0; fieldNo<fieldCount; fieldNo++)
		{
			Field * field = fields.Get(fieldNo);
			if (!isLayouting(field))
				continue;

			FieldLayoutData * fieldLayoutData = field->getFieldLayoutData();

			int columnSpan = fieldLayoutData->getColumnSpan();
			if (oneRowOnly && columnSpan < 1)
				columnSpan = 1;
			if (columnSpan < 1 || columnNo + columnSpan >= columns)
				columnSpan = columns - columnNo;

			// If the field has the currently investigated column span,
			// analyze field's current, minimum, and maximum widths

			if (columnSpan == investigatedColumnSpan)
			{
				// Find desired columns width

				int currentColumnsWidth = 0;
				for (i=0; i<investigatedColumnSpan; i++)
					currentColumnsWidth += columnWidths.Get(columnNo + i)->Get();

				int leftSpacing = leftSpacings.Get(rowNo)->Get(columnNo)->Get();
				int rightSpacing = rightSpacings.Get(rowNo)->Get(columnNo + investigatedColumnSpan - 1)->Get();

				int desiredColumnsWidth = leftSpacing + rightSpacing;
				if (resizeContainer || fieldLayoutData->getHorizontalAlignment() != FieldLayoutData::ALIGN_FILL)
					desiredColumnsWidth += field->getSize().x;
				else
					desiredColumnsWidth += field->getMinimumSize().x;

				int additionalSpace = desiredColumnsWidth - currentColumnsWidth;
				if (additionalSpace > 0)
					for (i=0; i<investigatedColumnSpan; i++)
					{
						Int * columnWidth = columnWidths.Get(columnNo + i);
						columnWidth->Add(additionalSpace / (investigatedColumnSpan - i));
						additionalSpace -= additionalSpace / (investigatedColumnSpan - i);
					}

				if (fieldLayoutData->getHorizontalAlignment() == FieldLayoutData::ALIGN_FILL)
				{
					// If none of the columns the field spans is marked as resizing, mark them all
					for (i=0; i<investigatedColumnSpan; i++)
						if (columnResizings.Get(columnNo + i)->Get())
							break;
					if (i >= investigatedColumnSpan)
						for (i=0; i<investigatedColumnSpan; i++)
							columnResizings.Get(columnNo + i)->Set(true);
				}

				// Update minimum and maximum column widths

				int minimumWidth;
				int maximumWidth;

				if (fieldLayoutData->getHorizontalAlignment() == FieldLayoutData::ALIGN_FILL)
				{
					// Filling fields can be sized to their minimum and maximum sizes
					minimumWidth = field->getMinimumSize().x + leftSpacing + rightSpacing;
					maximumWidth = field->getMaximumSize().x + leftSpacing + rightSpacing;
				}
				else
				{
					// Non-filling fields cannot be sized, use the size for the minimum size
					minimumWidth = field->getSize().x + leftSpacing + rightSpacing;
					maximumWidth = 0;
				}

				// Update the minimum width, if the current minimum width is bigger
				int additionalMinimumWidth = minimumWidth;
				for (i=0; i<investigatedColumnSpan; i++)
					additionalMinimumWidth -= columnMinimumWidths.Get(columnNo + i)->Get();
				if (additionalMinimumWidth > 0)
					for (i=0; i<investigatedColumnSpan; i++)
					{
						Int * columnMinimumWidth = columnMinimumWidths.Get(columnNo + i);
						columnMinimumWidth->Add(additionalMinimumWidth / (investigatedColumnSpan - i));
						additionalMinimumWidth -= additionalMinimumWidth / (investigatedColumnSpan - i);
					}

				// Update the maximum width, if the current maximum width is bigger
				// (this is correct, we are looking for the biggest maximum width)
				int additionalMaximumWidth = maximumWidth;
				for (i=0; i<investigatedColumnSpan; i++)
					additionalMaximumWidth -= columnMaximumWidths.Get(columnNo + i)->Get();
				if (additionalMaximumWidth > 0)
					for (i=0; i<investigatedColumnSpan; i++)
					{
						Int * columnMaximumWidth = columnMaximumWidths.Get(columnNo + i);
						columnMaximumWidth->Add(additionalMaximumWidth / (investigatedColumnSpan - i));
						additionalMaximumWidth -= additionalMaximumWidth / (investigatedColumnSpan - i);
					}
			}

			// Move column and/or row number according to column span

			columnNo += columnSpan;
			if (columnNo >= columns)
			{
				columnNo = 0;
				rowNo++;
			}
		}
	}

	// Find minimum, maximum, and desired row heights

	rowNo = 0;
	columnNo = 0;

	for (fieldNo=0; fieldNo<fieldCount; fieldNo++)
	{
		Field * field = fields.Get(fieldNo);
		if (!isLayouting(field))
			continue;

		FieldLayoutData * fieldLayoutData = field->getFieldLayoutData();

		int columnSpan = fieldLayoutData->getColumnSpan();
		if (oneRowOnly && columnSpan < 1)
			columnSpan = 1;
		if (columnSpan < 1 || columnNo + columnSpan >= columns)
			columnSpan = columns - columnNo;

		// Find desired row height

		Int * currentRowHeight = rowHeights.Get(rowNo);

		int topSpacing = getMaximumSpacing(topSpacings.Get(rowNo), columnNo, columnSpan);
		int bottomSpacing = getMaximumSpacing(bottomSpacings.Get(rowNo), columnNo, columnSpan);

		int desiredRowHeight = topSpacing + bottomSpacing;
		if (resizeContainer || fieldLayoutData->getVerticalAlignment() != FieldLayoutData::ALIGN_FILL)
			desiredRowHeight += field->getSize().y;
		else
			desiredRowHeight += field->getMinimumSize().y;

		if (desiredRowHeight > currentRowHeight->Get())
			currentRowHeight->Set(desiredRowHeight);

		if (fieldLayoutData->getVerticalAlignment() == FieldLayoutData::ALIGN_FILL)
		{
			// Filling fields mark the row to be resizing
			rowResizings.Get(rowNo)->Set(true);
		}

		// Update minimum and maximum column widths

		int minimumHeight;
		int maximumHeight;

		if (fieldLayoutData->getVerticalAlignment() == FieldLayoutData::ALIGN_FILL)
		{
			// Filling fields can be sized to their minimum and maximum sizes
			minimumHeight = field->getMinimumSize().y + topSpacing + bottomSpacing;
			maximumHeight = field->getMaximumSize().y + topSpacing + bottomSpacing;
		}
		else
		{
			// Non-filling fields cannot be sized, use the size for the minimum size
			minimumHeight = field->getSize().y + topSpacing + bottomSpacing;
			maximumHeight = 0;
		}

		// Update the maximum height, if the current maximum height is bigger
		Int * rowMinimumHeight = rowMinimumHeights.Get(rowNo);
		if (minimumHeight > rowMinimumHeight->Get())
			rowMinimumHeight->Set(minimumHeight);

		// Update the maximum height, if the current maximum height is bigger
		// (this is correct, we are looking for the biggest maximum height)
		Int * rowMaximumHeight = rowMaximumHeights.Get(rowNo);
		if (maximumHeight > rowMaximumHeight->Get())
			rowMaximumHeight->Set(maximumHeight);

		// Move column and/or row number according to column span

		columnNo += columnSpan;
		if (columnNo >= columns)
		{
			columnNo = 0;
			rowNo++;
		}
	}

	// Measure the columns

	int totalWidth = 0;
	int resizingColumns = 0;
	for (i=0; i<columns; i++)
	{
		totalWidth += columnWidths.Get(i)->Get();
		if (columnResizings.Get(i)->Get())
			resizingColumns++;
	}

	// Enlarge/shrink filling columns if not resizing the area

	int additionalSpace = containerSize.x - totalWidth;
	if (additionalSpace != 0 && !resizeContainer && resizingColumns > 0)
		for (i=0; i<columns; i++)
			if (columnResizings.Get(i)->Get())
			{
				Int * columnWidth = columnWidths.Get(i);
				columnWidth->Add(additionalSpace / resizingColumns);
				totalWidth += additionalSpace / resizingColumns;
				additionalSpace -= additionalSpace / resizingColumns;
				resizingColumns--;
			}

	// Measure the rows

	int totalHeight = 0;
	int resizingRows = 0;
	for (i=0; i<rows; i++)
	{
		totalHeight += rowHeights.Get(i)->Get();
		if (rowResizings.Get(i)->Get())
			resizingRows++;
	}

	// Enlarge/shrink filling rows

	additionalSpace = containerSize.y - totalHeight;
	if (additionalSpace != 0 && !resizeContainer && resizingRows > 0)
		for (i=0; i<rows; i++)
			if (rowResizings.Get(i)->Get())
			{
				Int * rowHeight = rowHeights.Get(i);
				rowHeight->Add(additionalSpace / resizingRows);
				totalHeight += additionalSpace / resizingRows;
				additionalSpace -= additionalSpace / resizingRows;
				resizingRows--;
			}

	// Correct columns exceeding the minimum or maximum widths

	for (columnNo=0; columnNo<columns; columnNo++)
	{
		Int * targetColumnWidth = columnWidths.Get(columnNo);
		int missingWidth = columnMinimumWidths.Get(columnNo)->Get() - targetColumnWidth->Get();
		while (missingWidth > 0)
		{
			int possibleTargets = 0;
			for (i=0; i<columns; i++)
			{
				Int * sourceColumnWidth = columnWidths.Get(i);
				if (columnNo == i || sourceColumnWidth->Get() <= columnMinimumWidths.Get(i)->Get())
					continue;

				possibleTargets++;
			}

			if (possibleTargets == 0)
			{
				// No source found: This is a problem, but we have to stop distributing anyway
				break;
			}

			int sourceNo = 0;
			for (i=0; i<columns; i++)
			{
				Int * sourceColumnWidth = columnWidths.Get(i);
				if (columnNo == i || sourceColumnWidth->Get() <= columnMinimumWidths.Get(i)->Get())
					continue;

				int possibleAcceptance = sourceColumnWidth->Get() - columnMinimumWidths.Get(i)->Get();
				int actualAcceptance = missingWidth / (possibleTargets - sourceNo);
				if (actualAcceptance > possibleAcceptance)
					actualAcceptance = possibleAcceptance;

				sourceColumnWidth->Add(-actualAcceptance);
				targetColumnWidth->Add(actualAcceptance);
				missingWidth -= actualAcceptance;

				sourceNo++;
			}
		}

		targetColumnWidth = columnWidths.Get(columnNo);
		int excessiveWidth = targetColumnWidth->Get() - columnMaximumWidths.Get(columnNo)->Get();
		while (excessiveWidth > 0)
		{
			int possibleTargets = 0;
			for (i=0; i<columns; i++)
			{
				Int * sourceColumnWidth = columnWidths.Get(i);
				if (columnNo == i || sourceColumnWidth->Get() >= columnMaximumWidths.Get(i)->Get())
					continue;

				possibleTargets++;
			}

			if (possibleTargets == 0)
			{
				// No source found: This is a problem, but we have to stop distributing anyway
				break;
			}

			int sourceNo = 0;
			for (i=0; i<columns; i++)
			{
				Int * sourceColumnWidth = columnWidths.Get(i);
				if (columnNo == i || sourceColumnWidth->Get() >= columnMaximumWidths.Get(i)->Get())
					continue;

				int possibleAcceptance = columnMaximumWidths.Get(i)->Get() - sourceColumnWidth->Get();
				int actualAcceptance = excessiveWidth / (possibleTargets - sourceNo);
				if (actualAcceptance > possibleAcceptance)
					actualAcceptance = possibleAcceptance;

				sourceColumnWidth->Add(actualAcceptance);
				targetColumnWidth->Add(-actualAcceptance);
				excessiveWidth -= actualAcceptance;

				sourceNo++;
			}
		}
	}

	// Correct rows exceeding the minimum or maximum heights

	for (rowNo=0; rowNo<rows; rowNo++)
	{
		Int * targetRowHeight = rowHeights.Get(rowNo);
		int missingHeight = rowMinimumHeights.Get(rowNo)->Get() - targetRowHeight->Get();
		while (missingHeight > 0)
		{
			int possibleTargets = 0;
			for (i=0; i<rows; i++)
			{
				Int * sourceRowHeight = rowHeights.Get(i);
				if (rowNo == i || sourceRowHeight->Get() <= rowMinimumHeights.Get(i)->Get())
					continue;

				possibleTargets++;
			}

			if (possibleTargets == 0)
			{
				// No source found: This is a problem, but we have to stop distributing anyway
				break;
			}

			int sourceNo = 0;
			for (i=0; i<rows; i++)
			{
				Int * sourceRowHeight = rowHeights.Get(i);
				if (rowNo == i || sourceRowHeight->Get() <= rowMinimumHeights.Get(i)->Get())
					continue;

				int possibleAcceptance = sourceRowHeight->Get() - rowMinimumHeights.Get(i)->Get();
				int actualAcceptance = missingHeight / (possibleTargets - sourceNo);
				if (actualAcceptance > possibleAcceptance)
					actualAcceptance = possibleAcceptance;

				sourceRowHeight->Add(-actualAcceptance);
				targetRowHeight->Add(actualAcceptance);
				missingHeight -= actualAcceptance;

				sourceNo++;
			}
		}

		targetRowHeight = rowHeights.Get(rowNo);
		int excessiveHeight = targetRowHeight->Get() - rowMaximumHeights.Get(rowNo)->Get();
		while (excessiveHeight > 0)
		{
			int possibleTargets = 0;
			for (i=0; i<rows; i++)
			{
				Int * sourceRowHeight = rowHeights.Get(i);
				if (rowNo == i || sourceRowHeight->Get() >= rowMaximumHeights.Get(i)->Get())
					continue;

				possibleTargets++;
			}

			if (possibleTargets == 0)
			{
				// No source found: This is a problem, but we have to stop distributing anyway
				break;
			}

			int sourceNo = 0;
			for (i=0; i<rows; i++)
			{
				Int * sourceRowHeight = rowHeights.Get(i);
				if (rowNo == i || sourceRowHeight->Get() >= rowMaximumHeights.Get(i)->Get())
					continue;

				int possibleAcceptance = rowMaximumHeights.Get(i)->Get() - sourceRowHeight->Get();
				int actualAcceptance = excessiveHeight / (possibleTargets - sourceNo);
				if (actualAcceptance > possibleAcceptance)
					actualAcceptance = possibleAcceptance;

				sourceRowHeight->Add(actualAcceptance);
				targetRowHeight->Add(-actualAcceptance);
				excessiveHeight -= actualAcceptance;

				sourceNo++;
			}
		}
	}

	// Apply columns and row size information to fields (actual alignment and resizing)

	rowNo = 0;
	columnNo = 0;

	Vector cellPosition;
	for (fieldNo=0; fieldNo<fieldCount; fieldNo++)
	{
		Field * field = fields.Get(fieldNo);
		if (!isLayouting(field))
			continue;

		// Collect information about the field's columns and row, and about the field itself

		FieldLayoutData * fieldLayoutData = field->getFieldLayoutData();
		FieldLayoutData::Alignment horizontalAlignment = fieldLayoutData->getHorizontalAlignment();
		FieldLayoutData::Alignment verticalAlignment = fieldLayoutData->getVerticalAlignment();

		Vector position = field->getPosition();
		Vector size = field->getSize();
		Vector minimumSize = field->getMinimumSize();
		Vector maximumSize = field->getMaximumSize();

		int columnSpan = fieldLayoutData->getColumnSpan();
		if (oneRowOnly && columnSpan < 1)
			columnSpan = 1;
		if (columnSpan < 1 || columnNo + columnSpan >= columns)
			columnSpan = columns - columnNo;

		int currentColumnsWidth = 0;
		for (i=0; i<columnSpan; i++)
			currentColumnsWidth += columnWidths.Get(columnNo + i)->Get();

		int rowHeight = rowHeights.Get(rowNo)->Get();

		int leftSpacing = leftSpacings.Get(rowNo)->Get(columnNo)->Get();
		int rightSpacing = rightSpacings.Get(rowNo)->Get(columnNo + columnSpan - 1)->Get();
		int topSpacing = getMaximumSpacing(topSpacings.Get(rowNo), columnNo, columnSpan);
		int bottomSpacing = getMaximumSpacing(bottomSpacings.Get(rowNo), columnNo, columnSpan);

		// Set new size of field, if layout says to fill

		if (horizontalAlignment == FieldLayoutData::ALIGN_FILL)
			size.x = currentColumnsWidth - leftSpacing - rightSpacing;

		if (verticalAlignment == FieldLayoutData::ALIGN_FILL)
			size.y = rowHeight - topSpacing - bottomSpacing;

		// If there is no extra space (among others because of a fill), move field to beginning of cell

		if (currentColumnsWidth <= size.x + leftSpacing + rightSpacing)
			horizontalAlignment = FieldLayoutData::ALIGN_START;
		if (rowHeight <= size.y + topSpacing + bottomSpacing)
			verticalAlignment = FieldLayoutData::ALIGN_START;

		// Set new position of field, if layout says to move

		switch (horizontalAlignment)
		{
			case FieldLayoutData::ALIGN_START:
				position.x = cellPosition.x + leftSpacing;
			break;

			case FieldLayoutData::ALIGN_CENTER:
				position.x = cellPosition.x + leftSpacing
					+ (currentColumnsWidth - leftSpacing - rightSpacing - size.x) / 2;
			break;

			case FieldLayoutData::ALIGN_END:
				position.x = cellPosition.x + currentColumnsWidth - rightSpacing - size.x;
			break;
		}

		switch (verticalAlignment)
		{
			case FieldLayoutData::ALIGN_START:
				position.y = cellPosition.y + topSpacing;
			break;

			case FieldLayoutData::ALIGN_CENTER:
				position.y = cellPosition.y + topSpacing
					+ (rowHeight - topSpacing - bottomSpacing - size.y) / 2;
			break;

			case FieldLayoutData::ALIGN_END:
				position.y = cellPosition.y + rowHeight - bottomSpacing - size.y;
			break;
		}

		// Apply new position and size

		if (field->moveTo(position))
			modified = true;
		if (field->resize(size))
			modified = true;

		// Move column and/or row number, as well as current position according to column span

		for (i=0; i<columnSpan; i++)
			cellPosition.x += columnWidths.Get(columnNo + i)->Get();
		columnNo += columnSpan;
		if (columnNo >= columns)
		{
			cellPosition.x = 0;
			cellPosition.y += rowHeights.Get(rowNo)->Get();

			columnNo = 0;
			rowNo++;
		}
	}

	// Calculate the minimum and maximum size of the container

	Vector minimumSize;
	Vector maximumSize;
	for (i=0; i<columns; i++)
	{
		minimumSize.x += columnMinimumWidths.Get(i)->Get();
		maximumSize.x += columnMaximumWidths.Get(i)->Get();
	}
	for (i=0; i<rows; i++)
	{
		minimumSize.y += rowMinimumHeights.Get(i)->Get();
		maximumSize.y += rowMaximumHeights.Get(i)->Get();
	}

	// Scrollable scrolling areas do not set the minimum and maximum size

	if (scrollingArea != NULL)
	{
		if (scrollingArea->isHorizontallyScrollable())
		{
			minimumSize.x = scrollingArea->getMinimumSize().x;
			maximumSize.x = scrollingArea->getMaximumSize().x;
		}
		else
		{
			minimumSize.x += scrollingArea->getSize().x - scrollingArea->getContentViewSize().x;
			maximumSize.x += scrollingArea->getSize().x - scrollingArea->getContentViewSize().x;
		}

		if (scrollingArea->isVerticallyScrollable())
		{
			minimumSize.y = scrollingArea->getMinimumSize().y;
			maximumSize.y = scrollingArea->getMaximumSize().y;
		}
		else
		{
			minimumSize.y += scrollingArea->getSize().y - scrollingArea->getContentViewSize().y;
			maximumSize.y += scrollingArea->getSize().y - scrollingArea->getContentViewSize().y;
		}
	}

	// Apply the new minimum and maximum sizes

	Vector totalSize = Vector(totalWidth, totalHeight);
	if (scrollingArea != NULL)
		scrollingArea->setContentSize(totalSize);

	if (resizeContainer)
	{
		container->setMaximumSize(maximumSize);
		container->setMinimumSize(minimumSize);

		// Scrolling areas with scroller do not resize the area itself
		// Scrolling areas without scroller resize the area that the view has the target size
		// All other containers area resizes directly

		if (scrollingArea != NULL)
		{
			if (scrollingArea->isHorizontallyScrollable())
				totalSize.x = scrollingArea->getSize().x;
			else
				totalSize.x += scrollingArea->getSize().x - scrollingArea->getContentViewSize().x;

			if (scrollingArea->isVerticallyScrollable())
				totalSize.y = scrollingArea->getSize().y;
			else
				totalSize.y += scrollingArea->getSize().y - scrollingArea->getContentViewSize().y;
		}

		if (container->resize(totalSize))
			modified = true;
	}

	return modified;
}

bool GridLayouter::layoutGroupsAndFields(const ArrayList<Field> & fields, Component * container,
	Vector topLeftMargins, Vector bottomRightMargins, int columns, bool resizeContainer)
{
	ASSERTION_COBJECT(this);

	bool modified = false;

	// First, layout the groups' child fields
	// Let the layouter change the group's size according to the content
	for (int i=0; i<fields.GetCount(); i++)
	{
		GroupField * groupField = dynamic_cast<GroupField *>(fields.Get(i));
		if (groupField != NULL && isLayouting(groupField))
			if (layout(groupField, true))
				modified = true;
	}

	// Now, layout the fields, including the group fields
	// The previously layouted group fields' sizes are taken into account
	if (layoutFields(fields, container, topLeftMargins, bottomRightMargins, columns, resizeContainer))
		modified = true;

	// Finally, propagated the new group field sizes to the children
	for (int i=0; i<fields.GetCount(); i++)
	{
		GroupField * groupField = dynamic_cast<GroupField *>(fields.Get(i));
		if (groupField != NULL && isLayouting(groupField))
			if (layout(groupField, false))
				modified = true;
	}

	return modified;
}
