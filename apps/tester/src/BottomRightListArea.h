/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __BOTTOMRIGHTLISTAREA_H
#define __BOTTOMRIGHTLISTAREA_H


class BottomRightListArea : public SingleFieldArea
{
	private:

		Image * femaleImage;
		ListField * listField;
		Image * maleImage;

	public:

		BottomRightListArea();
		Image * getFemaleImage();
		ListField * getListField();
		Image * getMaleImage();
		void insertListFieldRow(Image * image, const String & firstName, const String & lastName,
			int position = -1);
		void initialize();
};


#endif
