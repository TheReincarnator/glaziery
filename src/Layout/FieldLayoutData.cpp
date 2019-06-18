/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


FieldLayoutData::FieldLayoutData()
{
	ASSERTION_COBJECT(this);

	columnSpan = 1;
	excluded = false;
	field = NULL;
	horizontalAlignment = ALIGN_START;
	verticalAlignment = ALIGN_START;
}

FieldLayoutData::~FieldLayoutData()
{
	ASSERTION_COBJECT(this);
}

Vector FieldLayoutData::getBottomRightMargin()
{
	ASSERTION_COBJECT(this);
	return bottomRightMargin;
}

Vector FieldLayoutData::getBottomRightPadding()
{
	ASSERTION_COBJECT(this);
	return bottomRightPadding;
}

int FieldLayoutData::getColumnSpan()
{
	ASSERTION_COBJECT(this);
	return columnSpan;
}

Field * FieldLayoutData::getField()
{
	ASSERTION_COBJECT(this);
	return field;
}

FieldLayoutData::Alignment FieldLayoutData::getHorizontalAlignment()
{
	ASSERTION_COBJECT(this);
	return horizontalAlignment;
}

Vector FieldLayoutData::getTopLeftMargin()
{
	ASSERTION_COBJECT(this);
	return topLeftMargin;
}

Vector FieldLayoutData::getTopLeftPadding()
{
	ASSERTION_COBJECT(this);
	return topLeftPadding;
}

FieldLayoutData::Alignment FieldLayoutData::getVerticalAlignment()
{
	ASSERTION_COBJECT(this);
	return verticalAlignment;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(FieldLayoutData, CObject);
#endif

bool FieldLayoutData::isExcluded()
{
	ASSERTION_COBJECT(this);
	return excluded;
}

void FieldLayoutData::layoutIfAutomatic()
{
	ASSERTION_COBJECT(this);

	if (field == NULL)
		return;

	FieldsArea * fieldsArea = field->getFieldsArea();
	if (fieldsArea != NULL)
		fieldsArea->layoutIfAutomatic();
}

void FieldLayoutData::setBottomRightMargin(Vector bottomRightMargin)
{
	ASSERTION_COBJECT(this);

	this->bottomRightMargin = bottomRightMargin;
	layoutIfAutomatic();
}

void FieldLayoutData::setBottomRightPadding(Vector bottomRightPadding)
{
	ASSERTION_COBJECT(this);

	this->bottomRightPadding = bottomRightPadding;
	layoutIfAutomatic();
}

void FieldLayoutData::setColumnSpan(int columnSpan)
{
	ASSERTION_COBJECT(this);

	this->columnSpan = columnSpan;
	layoutIfAutomatic();
}

void FieldLayoutData::setHorizontalAlignment(FieldLayoutData::Alignment horizontalAlignment)
{
	ASSERTION_COBJECT(this);

	this->horizontalAlignment = horizontalAlignment;
	layoutIfAutomatic();
}

void FieldLayoutData::setExcluded(bool excluded)
{
	ASSERTION_COBJECT(this);
	this->excluded = excluded;
}

void FieldLayoutData::setTopLeftMargin(Vector topLeftMargin)
{
	ASSERTION_COBJECT(this);

	this->topLeftMargin = topLeftMargin;
	layoutIfAutomatic();
}

void FieldLayoutData::setTopLeftPadding(Vector topLeftPadding)
{
	ASSERTION_COBJECT(this);

	this->topLeftPadding = topLeftPadding;
	layoutIfAutomatic();
}

void FieldLayoutData::setVerticalAlignment(FieldLayoutData::Alignment verticalAlignment)
{
	ASSERTION_COBJECT(this);

	this->verticalAlignment = verticalAlignment;
	layoutIfAutomatic();
}
