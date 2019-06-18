/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


BottomRightListArea::BottomRightListArea()
{
	ASSERTION_COBJECT(this);
	listField = NULL;
}

Image * BottomRightListArea::getFemaleImage()
{
	ASSERTION_COBJECT(this);
	return femaleImage;
}

ListField * BottomRightListArea::getListField()
{
	ASSERTION_COBJECT(this);
	return listField;
}

Image * BottomRightListArea::getMaleImage()
{
	ASSERTION_COBJECT(this);
	return maleImage;
}

void BottomRightListArea::insertListFieldRow(Image * image,
	const String & firstName, const String & lastName, int position)
{
	ASSERTION_COBJECT(this);

	if (position < 0)
		position = listField->getRows().GetCount();

	GLAZIERY_LISTFIELD_ROW_P(row, listField, position)

	GLAZIERY_LISTFIELD_CELL(cell1, row)
	GLAZIERY_LISTFIELD_IMAGEELEMENT(element1, cell1, image)
	GLAZIERY_LISTFIELD_TEXTELEMENT(element2, cell1, firstName)

	GLAZIERY_LISTFIELD_CELL(cell2, row)
	GLAZIERY_LISTFIELD_TEXTELEMENT(element3, cell2, lastName)
}

void BottomRightListArea::initialize()
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	Image * femaleImage = adapter->getImage("images\\Female.png", false);
	Image * maleImage = adapter->getImage("images\\Male.png", false);

	GLAZIERY_LISTFIELD(listField, NULL, NULL, true, true, MANY)
	this->listField = listField;
	setContentField(listField);

	GLAZIERY_LISTFIELD_COLUMN_S(column1, listField, 118, true, LEFT, "First name")
	GLAZIERY_LISTFIELD_COLUMN_S(column2, listField, 100, true, LEFT, "Last name")

	insertListFieldRow(maleImage, "John", "Smith");
	insertListFieldRow(femaleImage, "Barbara", "Smith");
	insertListFieldRow(femaleImage, "Anna", "Bella");
	insertListFieldRow(maleImage, "Peter", "Dowell");
	insertListFieldRow(maleImage, "David", "Dowell");
	insertListFieldRow(femaleImage, "Anna", "Dowell");
	insertListFieldRow(maleImage, "Fred", "Smith");
	insertListFieldRow(maleImage, "Fred", "Dowell");
	insertListFieldRow(femaleImage, "Nicole", "Bella");
	insertListFieldRow(femaleImage, "Clarissa", "Bella");
	insertListFieldRow(femaleImage, "Barbara", "Bella");
}
