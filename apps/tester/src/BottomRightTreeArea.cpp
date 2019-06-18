/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


TreeField::Item * BottomRightTreeArea::createTreeFieldItem(TreeField * treeField, TreeField::Item * parent,
	Image * image, const String & firstName, const String & lastName, bool expanded)
{
	ASSERTION_COBJECT(this);

	TreeField::Item * item;
	if ((item = new TreeField::Item) == NULL)
		throw EOUTOFMEMORY;

	if (parent == NULL)
		treeField->appendRoot(item);
	else
		parent->appendChild(item);

	ImageElement * element1;
	if ((element1 = new ImageElement) == NULL)
		throw EOUTOFMEMORY;
	item->appendElement(element1);
	element1->setImage(image);

	TextElement * element2;
	if ((element2 = new TextElement) == NULL)
		throw EOUTOFMEMORY;
	item->appendElement(element2);
	element2->setText(firstName);

	TextElement * element3;
	if ((element3 = new TextElement) == NULL)
		throw EOUTOFMEMORY;
	item->appendElement(element3);
	element3->setText(lastName);

	item->setExpanded(expanded);

	return item;
}

void BottomRightTreeArea::initialize()
{
	ASSERTION_COBJECT(this);

	PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	Image * femaleImage = adapter->getImage("images\\Female.png", false);
	Image * maleImage = adapter->getImage("images\\Male.png", false);

	TreeField * treeField5a;
	if ((treeField5a = new TreeField) == NULL)
		throw EOUTOFMEMORY;
	setContentField(treeField5a);
	treeField5a->setType(TreeField::TYPE_ONE);

	TreeField::Item * item1 = createTreeFieldItem(treeField5a, NULL, maleImage, "Adam", "Smith", true);
	TreeField::Item * item2 = createTreeFieldItem(treeField5a, item1, maleImage, "Peter", "Smith", true);
	TreeField::Item * item3 = createTreeFieldItem(treeField5a, item1, maleImage, "Paul", "Smith", true);
	TreeField::Item * item4 = createTreeFieldItem(treeField5a, item1, maleImage, "Fred", "Smith", false);
	TreeField::Item * item5 = createTreeFieldItem(treeField5a, item4, femaleImage, "Nicole", "Smith", true);
	TreeField::Item * item6 = createTreeFieldItem(treeField5a, item4, femaleImage, "Clarissa", "Smith", true);
	TreeField::Item * item7 = createTreeFieldItem(treeField5a, item6, maleImage, "Barbara", "Smith", true);
	TreeField::Item * item8 = createTreeFieldItem(treeField5a, item4, maleImage, "Adam", "Dowell", true);
	TreeField::Item * item9 = createTreeFieldItem(treeField5a, item8, maleImage, "Peter", "Dowell", true);
	TreeField::Item * item10 = createTreeFieldItem(treeField5a, item8, maleImage, "Paul", "Dowell", true);
	TreeField::Item * item11 = createTreeFieldItem(treeField5a, item8, maleImage, "Fred", "Dowell", true);
	TreeField::Item * item12 = createTreeFieldItem(treeField5a, item11, femaleImage, "Nicole", "Dowell", true);
	TreeField::Item * item13 = createTreeFieldItem(treeField5a, item8, femaleImage, "Clarissa", "Dowell", true);
	TreeField::Item * item14 = createTreeFieldItem(treeField5a, item8, maleImage, "Barbara", "Dowell", true);
	TreeField::Item * item15 = createTreeFieldItem(treeField5a, item4, maleImage, "Adam", "Sandell", true);
	TreeField::Item * item16 = createTreeFieldItem(treeField5a, item15, maleImage, "Peter", "Sandell", true);
	TreeField::Item * item17 = createTreeFieldItem(treeField5a, item16, maleImage, "Paul", "Sandell", true);
	TreeField::Item * item18 = createTreeFieldItem(treeField5a, item17, maleImage, "Fred", "Sandell", true);
	TreeField::Item * item19 = createTreeFieldItem(treeField5a, item18, femaleImage, "Nicole", "Sandell", true);
	TreeField::Item * item20 = createTreeFieldItem(treeField5a, item19, femaleImage, "Clarissa", "Sandell", true);
	TreeField::Item * item21 = createTreeFieldItem(treeField5a, item4, maleImage, "Barbara", "Sandell", true);
	TreeField::Item * item22 = createTreeFieldItem(treeField5a, item21, maleImage, "Adam", "Fork", true);
	TreeField::Item * item23 = createTreeFieldItem(treeField5a, item21, maleImage, "Peter", "Fork", true);
	TreeField::Item * item24 = createTreeFieldItem(treeField5a, item21, maleImage, "Paul", "Fork", true);
	TreeField::Item * item25 = createTreeFieldItem(treeField5a, item1, maleImage, "Fred", "Fork", true);
	TreeField::Item * item26 = createTreeFieldItem(treeField5a, item25, femaleImage, "Nicole", "Fork", true);
	TreeField::Item * item27 = createTreeFieldItem(treeField5a, item25, femaleImage, "Clarissa", "Fork", true);
	TreeField::Item * item28 = createTreeFieldItem(treeField5a, item27, maleImage, "Barbara", "Fork", true);
	TreeField::Item * item29 = createTreeFieldItem(treeField5a, item25, maleImage, "Adam", "Pratt", true);
	TreeField::Item * item30 = createTreeFieldItem(treeField5a, item29, maleImage, "Peter", "Pratt", true);
	TreeField::Item * item31 = createTreeFieldItem(treeField5a, item30, maleImage, "Paul", "Pratt", true);
	TreeField::Item * item32 = createTreeFieldItem(treeField5a, item30, maleImage, "Fred", "Pratt", true);
	TreeField::Item * item33 = createTreeFieldItem(treeField5a, item32, femaleImage, "Nicole", "Pratt", true);
	TreeField::Item * item34 = createTreeFieldItem(treeField5a, item33, femaleImage, "Clarissa", "Pratt", true);
	TreeField::Item * item35 = createTreeFieldItem(treeField5a, item32, maleImage, "Barbara", "Pratt", true);
	TreeField::Item * item36 = createTreeFieldItem(treeField5a, item35, maleImage, "Adam", "Powers", true);
	TreeField::Item * item37 = createTreeFieldItem(treeField5a, item29, maleImage, "Peter", "Powers", true);
	TreeField::Item * item38 = createTreeFieldItem(treeField5a, item25, maleImage, "Paul", "Powers", true);
	TreeField::Item * item39 = createTreeFieldItem(treeField5a, item1, maleImage, "Fred", "Powers", true);
	TreeField::Item * item40 = createTreeFieldItem(treeField5a, item39, femaleImage, "Nicole", "Powers", true);
	TreeField::Item * item41 = createTreeFieldItem(treeField5a, item40, femaleImage, "Clarissa", "Powers", true);
	TreeField::Item * item42 = createTreeFieldItem(treeField5a, item40, maleImage, "Barbara", "Powers", true);
	TreeField::Item * item43 = createTreeFieldItem(treeField5a, item42, maleImage, "Adam", "Tott", true);
	TreeField::Item * item44 = createTreeFieldItem(treeField5a, item43, maleImage, "Peter", "Tott", true);
	TreeField::Item * item45 = createTreeFieldItem(treeField5a, item43, maleImage, "Paul", "Tott", true);
	TreeField::Item * item46 = createTreeFieldItem(treeField5a, NULL, maleImage, "Fred", "Tott", false);
	TreeField::Item * item47 = createTreeFieldItem(treeField5a, item46, femaleImage, "Nicole", "Tott", true);
	TreeField::Item * item48 = createTreeFieldItem(treeField5a, item46, femaleImage, "Clarissa", "Tott", true);
	TreeField::Item * item49 = createTreeFieldItem(treeField5a, item48, maleImage, "Barbara", "Tott", true);
	TreeField::Item * item50 = createTreeFieldItem(treeField5a, item48, maleImage, "Adam", "Krains", true);
	TreeField::Item * item51 = createTreeFieldItem(treeField5a, item46, maleImage, "Peter", "Krains", true);
	TreeField::Item * item52 = createTreeFieldItem(treeField5a, item51, maleImage, "Paul", "Krains", true);
	TreeField::Item * item53 = createTreeFieldItem(treeField5a, item46, maleImage, "Fred", "Krains", true);
	TreeField::Item * item54 = createTreeFieldItem(treeField5a, item53, femaleImage, "Nicole", "Krains", true);
	TreeField::Item * item55 = createTreeFieldItem(treeField5a, item54, femaleImage, "Clarissa", "Krains", true);
	TreeField::Item * item56 = createTreeFieldItem(treeField5a, item46, maleImage, "Barbara", "Krains", true);
}
