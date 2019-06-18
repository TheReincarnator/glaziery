/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


ContainerLayoutData::ContainerLayoutData()
{
	ASSERTION_COBJECT(this);

	bottomRightMargin = Vector(8, 8);
	columns = 1;
	container = NULL;
	topLeftMargin = Vector(8, 8);
}

ContainerLayoutData::~ContainerLayoutData()
{
	ASSERTION_COBJECT(this);
}

Component * ContainerLayoutData::getContainer()
{
	ASSERTION_COBJECT(this);
	return container;
}

Vector ContainerLayoutData::getBottomRightMargin()
{
	ASSERTION_COBJECT(this);
	return bottomRightMargin;
}

int ContainerLayoutData::getColumns()
{
	ASSERTION_COBJECT(this);
	return columns;
}

Vector ContainerLayoutData::getTopLeftMargin()
{
	ASSERTION_COBJECT(this);
	return topLeftMargin;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(ContainerLayoutData, CObject);
#endif

void ContainerLayoutData::layoutIfAutomatic()
{
	ASSERTION_COBJECT(this);

	FieldsArea * fieldsArea = dynamic_cast<FieldsArea *>(container);

	if (fieldsArea == NULL)
	{
		GroupField * groupField = dynamic_cast<GroupField *>(container);
		if (groupField != NULL)
			fieldsArea = groupField->getFieldsArea();
	}

	if (fieldsArea != NULL)
		fieldsArea->layoutIfAutomatic();
}

void ContainerLayoutData::setBottomRightMargin(Vector bottomRightMargin)
{
	ASSERTION_COBJECT(this);

	this->bottomRightMargin = bottomRightMargin;
	layoutIfAutomatic();
}

void ContainerLayoutData::setColumns(int columns)
{
	ASSERTION_COBJECT(this);

	if (this->columns == columns)
		return;

	if (columns < 0)
		throw EILLEGALARGUMENT("The number of columns must not be less than 0");

	this->columns = columns;
	layoutIfAutomatic();
}

void ContainerLayoutData::setTopLeftMargin(Vector topLeftMargin)
{
	ASSERTION_COBJECT(this);

	this->topLeftMargin = topLeftMargin;
	layoutIfAutomatic();
}
