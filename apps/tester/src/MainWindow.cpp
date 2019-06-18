/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include "Headers.h"


MainWindow::MainWindow()
{
	ASSERTION_COBJECT(this);
	bottomRightCustomArea = NULL;
}

void MainWindow::initialize()
{
	ASSERTION_COBJECT(this);

	// Main splitting area

	SplittingArea * mainSplittingArea;
	if ((mainSplittingArea = new SplittingArea(SplittingArea::VERTICAL)) == NULL)
		throw EOUTOFMEMORY;
	setClientArea(mainSplittingArea);

	// Top splitting area

	SplittingArea * topSplittingArea;
	if ((topSplittingArea = new SplittingArea(SplittingArea::HORIZONTAL)) == NULL)
		throw EOUTOFMEMORY;
	mainSplittingArea->appendPane(topSplittingArea);

	TopLeftArea * topLeftArea;
	if ((topLeftArea = new TopLeftArea) == NULL)
		throw EOUTOFMEMORY;
	topSplittingArea->appendPane(topLeftArea);
	topLeftArea->initialize();

	TopMiddleArea * topMiddleArea;
	if ((topMiddleArea = new TopMiddleArea) == NULL)
		throw EOUTOFMEMORY;
	topSplittingArea->appendPane(topMiddleArea);
	topMiddleArea->initialize(this);

	TopRightArea * topRightArea;
	if ((topRightArea = new TopRightArea) == NULL)
		throw EOUTOFMEMORY;
	topSplittingArea->appendPane(topRightArea);
	topRightArea->initialize();

	// Bottom splitting area

	SplittingArea * bottomSplittingArea;
	if ((bottomSplittingArea = new SplittingArea(SplittingArea::HORIZONTAL)) == NULL)
		throw EOUTOFMEMORY;
	mainSplittingArea->appendPane(bottomSplittingArea);

	BottomLeftArea * bottomLeftArea;
	if ((bottomLeftArea = new BottomLeftArea) == NULL)
		throw EOUTOFMEMORY;
	bottomSplittingArea->appendPane(bottomLeftArea);
	bottomLeftArea->initialize();

	TabbingArea * bottomRightArea;
	if ((bottomRightArea = new TabbingArea) == NULL)
		throw EOUTOFMEMORY;
	bottomSplittingArea->appendPane(bottomRightArea);

	BottomRightTreeArea * bottomRightTreeArea;
	if ((bottomRightTreeArea = new BottomRightTreeArea) == NULL)
		throw EOUTOFMEMORY;
	bottomRightArea->appendPane(bottomRightTreeArea, "&tree");
	bottomRightTreeArea->initialize();

	BottomRightListArea * bottomRightListArea;
	if ((bottomRightListArea = new BottomRightListArea) == NULL)
		throw EOUTOFMEMORY;
	bottomRightArea->appendPane(bottomRightListArea, "&list");
	bottomRightListArea->initialize();

	if ((bottomRightCustomArea = new BottomRightCustomArea) == NULL)
		throw EOUTOFMEMORY;
	bottomRightArea->appendPane(bottomRightCustomArea, "&custom");

	// Set sizes

	topLeftArea->resize(Vector(250, 150));
	topMiddleArea->resize(Vector(350, 250));
	topRightArea->resize(Vector(200, 150));
	bottomLeftArea->resize(Vector(400, 280));
	bottomRightArea->resize(Vector(300, 280));

	// Listeners

	ButtonCheckBoxListener * buttonCheckBoxListener;
	if ((buttonCheckBoxListener = new ButtonCheckBoxListener(topMiddleArea->getCheckBox1())) == NULL)
		throw EOUTOFMEMORY;
	topLeftArea->getButton1()->appendListener(buttonCheckBoxListener);

	CheckBoxListener * checkBoxListener;
	if ((checkBoxListener = new CheckBoxListener((TreeField *) bottomRightTreeArea->getContentField(),
		(ListField *) bottomRightListArea->getContentField(), this)) == NULL)
		throw EOUTOFMEMORY;
	topLeftArea->getCheckBox1()->appendListener(checkBoxListener);

	AppendButtonListener * appendButtonListener;
	if ((appendButtonListener = new AppendButtonListener(bottomRightListArea)) == NULL)
		throw EOUTOFMEMORY;
	topLeftArea->getButton4()->appendListener(appendButtonListener);

	PrependButtonListener * prependButtonListener;
	if ((prependButtonListener = new PrependButtonListener(bottomRightListArea)) == NULL)
		throw EOUTOFMEMORY;
	topLeftArea->getButton5()->appendListener(prependButtonListener);

	InsertButtonListener * insertButtonListener;
	if ((insertButtonListener = new InsertButtonListener(bottomRightListArea)) == NULL)
		throw EOUTOFMEMORY;
	topLeftArea->getButton6()->appendListener(insertButtonListener);

	DeleteSelectedButtonListener * deleteSelectedButtonListener;
	if ((deleteSelectedButtonListener = new DeleteSelectedButtonListener(bottomRightListArea)) == NULL)
		throw EOUTOFMEMORY;
	topLeftArea->getButton7()->appendListener(deleteSelectedButtonListener);

	DeleteUnselectedButtonListener * deleteUnselectedButtonListener;
	if ((deleteUnselectedButtonListener = new DeleteUnselectedButtonListener(bottomRightListArea)) == NULL)
		throw EOUTOFMEMORY;
	topLeftArea->getButton8()->appendListener(deleteUnselectedButtonListener);

	SliderAlphaListener * sliderAlphaListener;
	if ((sliderAlphaListener = new SliderAlphaListener(this)) == NULL)
		throw EOUTOFMEMORY;
	topMiddleArea->getSlider1()->appendListener(sliderAlphaListener);

	SliderProgressListener * sliderProgressListener;
	if ((sliderProgressListener = new SliderProgressListener(bottomLeftArea->getProgressBar1())) == NULL)
		throw EOUTOFMEMORY;
	topMiddleArea->getSlider2()->appendListener(sliderProgressListener);

	Window::Listener * windowListener;
	if ((windowListener = new WindowListener) == NULL)
		throw EOUTOFMEMORY;
	appendListener(windowListener);

	setStatus("Status bar message: Multiple selections allowed");
}


MainWindow::AppendButtonListener::AppendButtonListener(BottomRightListArea * listArea)
{
	ASSERTION_COBJECT(this);
	this->listArea = listArea;
}

void MainWindow::AppendButtonListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);
	listArea->insertListFieldRow(listArea->getMaleImage(), "Appended", "Row");
}


MainWindow::ButtonCheckBoxListener::ButtonCheckBoxListener(CheckBox * checkBox)
{
	ASSERTION_COBJECT(this);
	this->checkBox = checkBox;
}

void MainWindow::ButtonCheckBoxListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);
	checkBox->toggle();
}


MainWindow::CheckBoxListener::CheckBoxListener(TreeField * treeField, ListField * listField, Window * window)
{
	ASSERTION_COBJECT(this);

	this->treeField = treeField;
	this->listField = listField;
	this->window = window;
}

void MainWindow::CheckBoxListener::onStateChanged(CheckBox * checkBox)
{
	ASSERTION_COBJECT(this);

	switch (checkBox->getState())
	{
		case CheckBox::CHECKED:
			listField->setType(ListField::TYPE_MANY);
			treeField->setType(TreeField::TYPE_MANY);
		break;

		case CheckBox::TRI_STATE:
			listField->setType(ListField::TYPE_ONE_OR_NONE);
			treeField->setType(TreeField::TYPE_ONE_OR_NONE);
		break;

		default:
			listField->setType(ListField::TYPE_ONE);
			treeField->setType(TreeField::TYPE_ONE);
		break;
	}

	window->setStatused(checkBox->isChecked());
}


MainWindow::DeleteSelectedButtonListener::DeleteSelectedButtonListener(BottomRightListArea * listArea)
{
	ASSERTION_COBJECT(this);
	this->listArea = listArea;
}

void MainWindow::DeleteSelectedButtonListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);

	ListField * listField = listArea->getListField();
	for (int i=0; i<listField->getRows().GetCount();)
		if (listField->isRowSelected(i))
			listField->removeRow(i);
		else
			i++;
}


MainWindow::DeleteUnselectedButtonListener::DeleteUnselectedButtonListener(BottomRightListArea * listArea)
{
	ASSERTION_COBJECT(this);
	this->listArea = listArea;
}

void MainWindow::DeleteUnselectedButtonListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);

	ListField * listField = listArea->getListField();
	for (int i=0; i<listField->getRows().GetCount();)
		if (!listField->isRowSelected(i))
			listField->removeRow(i);
		else
			i++;
}


MainWindow::InsertButtonListener::InsertButtonListener(BottomRightListArea * listArea)
{
	ASSERTION_COBJECT(this);
	this->listArea = listArea;
}

void MainWindow::InsertButtonListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);

	listArea->insertListFieldRow(listArea->getMaleImage(), "Inserted", "Row",
		listArea->getListField()->getRows().GetCount() >= 4 ? 4 : 0);
}


MainWindow::PrependButtonListener::PrependButtonListener(BottomRightListArea * listArea)
{
	ASSERTION_COBJECT(this);
	this->listArea = listArea;
}

void MainWindow::PrependButtonListener::onPressed(Button * button)
{
	ASSERTION_COBJECT(this);
	listArea->insertListFieldRow(listArea->getMaleImage(), "Appended", "Row", 0);
}


MainWindow::SliderAlphaListener::SliderAlphaListener(Window * window)
{
	ASSERTION_COBJECT(this);
	this->window = window;
}

void MainWindow::SliderAlphaListener::onCurrentChanged(Slider * field)
{
	ASSERTION_COBJECT(this);

	window->setFocusAlpha((unsigned char) field->getConstrainedCurrent());
	window->setNonFocusAlpha((unsigned char) (field->getConstrainedCurrent() * 3 / 4));
}


MainWindow::SliderProgressListener::SliderProgressListener(ProgressBar * progressBar)
{
	ASSERTION_COBJECT(this);
	this->progressBar = progressBar;
}

void MainWindow::SliderProgressListener::onCurrentChanged(Slider * field)
{
	ASSERTION_COBJECT(this);
	progressBar->setCurrent(field->getCurrent());
}


VetoMode MainWindow::WindowListener::onClosing(Window * window)
{
	ASSERTION_COBJECT(this);

	if (MessageDialog::showDialog("GlazieryTester - Close window",
		"Are you sure to close this window?", MessageDialog::ICON_QUESTION,
		Dialog::BUTTONS_OKCANCEL) == Dialog::BUTTON_OK)
		return NO_VETO;
	else
		return VETO;
}
