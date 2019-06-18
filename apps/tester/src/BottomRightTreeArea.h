/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __BOTTOMRIGHTTREEAREA_H
#define __BOTTOMRIGHTTREEAREA_H


class BottomRightTreeArea : public SingleFieldArea
{
	private:

		TreeField::Item * createTreeFieldItem(TreeField * treeField, TreeField::Item * parent,
			Image * image, const String & firstName, const String & lastName, bool expanded);

	public:

		void initialize();
};


#endif
