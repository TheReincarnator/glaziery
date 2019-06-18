/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


FieldsArea::FieldsArea()
{
	ASSERTION_COBJECT(this);

	autoLayoutCentering = true;
	autoLayouting = false;
	autoLayoutResizing = true;
	defaultButton = NULL;
	defaultFieldBottomRightMargin = Vector(4, 4);
	defaultFieldBottomRightPadding = Vector(0, 0);
	defaultFieldTopLeftMargin = Vector(4, 4);
	defaultFieldTopLeftPadding = Vector(0, 0);
	focusField = NULL;
	layoutData = NULL;
	layouting = false;

	ContainerLayoutData * layoutData;
	if ((layoutData = new ContainerLayoutData) == NULL)
		throw EOUTOFMEMORY;
	setLayoutData(layoutData);

	if ((layouter = new GridLayouter()) == NULL)
		throw EOUTOFMEMORY;

	Desktop::getInstance()->getSkin()->setupFieldsArea(this);
}

FieldsArea::~FieldsArea()
{
	ASSERTION_COBJECT(this);

	// Explicit deletion of radio groups to ensure correct order of deletion
	radioGroups.DeleteAll();

	// Explicit deletion of fields to ensure correct order of deletion
	fields.DeleteAll();

	if (layoutData != NULL) {delete layoutData; layoutData = NULL;}
	if (layouter != NULL) {delete layouter; layouter = NULL;}
}

void FieldsArea::appendField(Field * field)
{
	ASSERTION_COBJECT(this);
	insertField(fields.GetCount(), field);
}

RadioGroup * FieldsArea::createRadioGroup()
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The fields area must be added to a parent"
			" before using it");

	RadioGroup * group;
	if ((group = new RadioGroup) == NULL)
		throw EOUTOFMEMORY;
	radioGroups.Append(group);

	return group;
}

void FieldsArea::drawAreaContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->getSkin()->drawFieldsArea(context, this);

	#ifdef GL_DEBUG_FIELDBOUNDS
		PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	#endif

	for (int i=0; i<fields.GetCount(); i++)
	{
		Field * field = fields.Get(i);
		if (field->isVisible() && field->getGroupField() == NULL)
		{
			DrawingContext fieldContext(context, field);

			#ifdef GL_DEBUG_FIELDBOUNDS
				adapter->drawRect(fieldContext, Vector(), fieldContext.getClippingSize() - Vector(1, 1),
					GL_TRANSPARENCY, GL_RGBA(128, 160, 128, 128));
			#endif

			field->draw(fieldContext);
		}
	}
}

bool FieldsArea::focusIfPossible(Field * field)
{
	ASSERTION_COBJECT(this);

	if (!field->isVisible() || !field->isActiveIncludingAncestors())
		return false;

	InputField * inputField = dynamic_cast<InputField *>(field);
	if (inputField == NULL)
		return false;

	bool focussable = false;
	RadioBox * radioBox = dynamic_cast<RadioBox *>(inputField);
	if (radioBox != NULL)
	{
		if (radioBox->isChecked())
			focussable = true;

		RadioGroup * radioGroup = radioBox->getRadioGroup();
		if (radioGroup == NULL || radioGroup->getChecked() == NULL)
			focussable = true;
	}
	else
		focussable = true;

	if (focussable)
	{
		setFocusField(inputField);
		return true;
	}

	return false;
}

EventTarget * FieldsArea::getContentEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = ScrollingArea::getContentEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	for (int i = fields.GetCount() - 1; i>=0; i--)
	{
		Field * field = fields.Get(i);
		if (field->isVisible() && field->getGroupField() == NULL
			&& position >= field->getPosition() && position < field->getPosition() + field->getSize())
			return field->getEventTargetAt(position - field->getPosition());
	}

	return this;
}

Button * FieldsArea::getDefaultButton()
{
	ASSERTION_COBJECT(this);
	return defaultButton;
}

Vector FieldsArea::getDefaultFieldBottomRightMargin()
{
	ASSERTION_COBJECT(this);
	return defaultFieldBottomRightMargin;
}

Vector FieldsArea::getDefaultFieldBottomRightPadding()
{
	ASSERTION_COBJECT(this);
	return defaultFieldBottomRightPadding;
}

Vector FieldsArea::getDefaultFieldTopLeftMargin()
{
	ASSERTION_COBJECT(this);
	return defaultFieldTopLeftMargin;
}

Vector FieldsArea::getDefaultFieldTopLeftPadding()
{
	ASSERTION_COBJECT(this);
	return defaultFieldTopLeftPadding;
}

const ArrayList<Field> & FieldsArea::getFields()
{
	ASSERTION_COBJECT(this);
	return fields;
}

ContainerLayoutData * FieldsArea::getLayoutData()
{
	ASSERTION_COBJECT(this);
	return layoutData;
}

Layouter * FieldsArea::getLayouter()
{
	ASSERTION_COBJECT(this);
	return layouter;
}

Component * FieldsArea::getFocusChild()
{
	ASSERTION_COBJECT(this);
	return focusField;
}

InputField * FieldsArea::getFocusField()
{
	ASSERTION_COBJECT(this);
	return focusField;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(FieldsArea, ScrollingArea);
#endif

void FieldsArea::insertField(int position, Field * field)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The fields area must be added to a parent before using it");

	fields.Insert(field, position);
	setComponentParent(field);

	if (focusField == NULL)
		setFocusToFirstField();

	FieldLayoutData * layoutData = field->getFieldLayoutData();
	layoutData->setBottomRightMargin(defaultFieldBottomRightMargin);
	layoutData->setBottomRightPadding(defaultFieldBottomRightPadding);
	layoutData->setTopLeftMargin(defaultFieldTopLeftMargin);
	layoutData->setTopLeftPadding(defaultFieldTopLeftPadding);

	layoutIfAutomatic();
	invalidate();
}

bool FieldsArea::isAutoLayoutCentering()
{
	ASSERTION_COBJECT(this);
	return autoLayoutCentering;
}

bool FieldsArea::isAutoLayouting()
{
	ASSERTION_COBJECT(this);
	return autoLayouting;
}

bool FieldsArea::isAutoLayoutResizing()
{
	ASSERTION_COBJECT(this);
	return autoLayoutResizing;
}

void FieldsArea::layout(bool layoutResizing, bool layoutCentering)
{
	ASSERTION_COBJECT(this);

	if (!layouting && layouter != NULL)
	{
		layouting = true;

		layouter->layout(this, layoutResizing);
		if (layoutResizing && layoutCentering)
		{
			Window * window = getWindow();
			if (window != NULL)
				window->center();
		}

		layouting = false;
	}
}

void FieldsArea::layoutIfAutomatic()
{
	ASSERTION_COBJECT(this);

	if (autoLayouting)
		layout(autoLayoutResizing, autoLayoutCentering);
}

bool FieldsArea::onBackTab(bool secondary)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingArea::onBackTab(secondary);
	if (!secondary)
	{
		setFocusToPreviousField();
		return true;
	}
	else
		return consumed;
}

void FieldsArea::onDestroying()
{
	ASSERTION_COBJECT(this);

	ScrollingArea::onDestroying();
	for (int i=0; i<fields.GetCount(); i++)
	{
		Field * field = fields.Get(i);
		field->setDisposed();
		field->onDestroying();
	}
}

bool FieldsArea::onHotKey(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingArea::onHotKey(character, option1, option2);
	Field * field = (Field *) getHotKeyComponent(character);
	if (field == NULL)
		return consumed;

	bool direct = true;
	InputField * inputField = dynamic_cast<InputField *>(field);
	if (inputField == NULL)
	{
		direct = false;
		int thisFieldNo = fields.Find(field);
		int fieldNo = thisFieldNo;
		do
		{
			fieldNo = (fieldNo + 1) % fields.GetCount();
			inputField = dynamic_cast<InputField *>(fields.Get(fieldNo));
		}
		while (inputField == NULL && fieldNo != thisFieldNo);
	}

	if (inputField != NULL && inputField->isVisible() && inputField->isActiveIncludingAncestors())
	{
		inputField->setFocus();
		inputField->onMatchedHotKey(direct);
	}

	return true;
}

void FieldsArea::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
	setFocusField(NULL);
}

bool FieldsArea::onTab(bool secondary)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingArea::onTab(secondary);
	if (!secondary)
	{
		setFocusToNextField();
		return true;
	}
	else
		return consumed;
}

void FieldsArea::prependField(Field * field)
{
	ASSERTION_COBJECT(this);
	insertField(0, field);
}

void FieldsArea::removeField(Field * field)
{
	ASSERTION_COBJECT(this);

	if (focusField == field)
		setFocusField(NULL);

	fields.Unlink(field);
	field->destroy();

	if (focusField == NULL)
		setFocusToFirstField();

	layoutIfAutomatic();
	invalidate();
}

bool FieldsArea::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	if (ScrollingArea::resizeInternal(size, notifyParent))
	{
		if (autoLayouting)
			layout(false);

		return true;
	}
	else
		return false;
}

void FieldsArea::setAutoLayouting(bool autoLayouting, bool autoLayoutResizing, bool autoLayoutCentering)
{
	ASSERTION_COBJECT(this);

	if (this->autoLayouting == autoLayouting
		&& this->autoLayoutResizing == autoLayoutResizing
		&& this->autoLayoutCentering == autoLayoutCentering)
		return;

	this->autoLayouting = autoLayouting;
	this->autoLayoutResizing = autoLayoutResizing;
	this->autoLayoutCentering = autoLayoutCentering;

	layoutIfAutomatic();
}

void FieldsArea::setDefaultButton(Button * defaultButton)
{
	ASSERTION_COBJECT(this);
	this->defaultButton = defaultButton;
}

void FieldsArea::setDefaultFieldBottomRightMargin(Vector defaultFieldBottomRightMargin)
{
	ASSERTION_COBJECT(this);
	this->defaultFieldBottomRightMargin = defaultFieldBottomRightMargin;
}

void FieldsArea::setDefaultFieldBottomRightPadding(Vector defaultFieldBottomRightPadding)
{
	ASSERTION_COBJECT(this);
	this->defaultFieldBottomRightPadding = defaultFieldBottomRightPadding;
}

void FieldsArea::setDefaultFieldTopLeftMargin(Vector defaultFieldTopLeftMargin)
{
	ASSERTION_COBJECT(this);
	this->defaultFieldTopLeftMargin = defaultFieldTopLeftMargin;
}

void FieldsArea::setDefaultFieldTopLeftPadding(Vector defaultFieldTopLeftPadding)
{
	ASSERTION_COBJECT(this);
	this->defaultFieldTopLeftPadding = defaultFieldTopLeftPadding;
}

void FieldsArea::setFocusField(InputField * focusField)
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The fields area must be added to a parent"
			" before using it");

	if (this->focusField == focusField)
		return;

	if (this->focusField != NULL)
		this->focusField->onLostFocus();

	this->focusField = focusField;

	if (this->focusField != NULL)
		this->focusField->onGotFocus(false);

	invalidate();
}

void FieldsArea::setFocusToFirstField()
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The fields area must be added to a parent"
			" before using it");

	int fieldCount = fields.GetCount();
	for (int i = 0; i < fieldCount; i++)
	{
		Field * field = fields.Get(i);
		if (focusIfPossible(field))
			return;
	}

	setFocusField(NULL);
}

void FieldsArea::setFocusToLastField()
{
	ASSERTION_COBJECT(this);

	if (getParent() == NULL)
		throw EILLEGALSTATE("The fields area must be added to a parent"
			" before using it");

	int fieldCount = fields.GetCount();
	for (int i = fieldCount - 1; i >= 0; i--)
	{
		Field * field = fields.Get(i);
		if (focusIfPossible(field))
			return;
	}

	setFocusField(NULL);
}

bool FieldsArea::setFocusToNextField()
{
	ASSERTION_COBJECT(this);

	int fieldCount = fields.GetCount();
	if (fieldCount == 0)
		return true;

	int focusFieldNo = fieldCount - 1;
	if (focusField != NULL)
	{
		focusFieldNo = fields.Find(focusField);
		if (focusFieldNo < 0)
			focusFieldNo = fieldCount - 1;
	}

	for (int step = 1; step < fieldCount; step++)
	{
		int fieldNo = (focusFieldNo + step) % fieldCount;
		if (focusIfPossible(fields.Get(fieldNo)))
			return fieldNo <= focusFieldNo;
	}

	return true;
}

bool FieldsArea::setFocusToPreviousField()
{
	ASSERTION_COBJECT(this);

	int fieldCount = fields.GetCount();
	if (fieldCount == 0)
		return true;

	int focusFieldNo = 0;
	if (focusField != NULL)
	{
		focusFieldNo = fields.Find(focusField);
		if (focusFieldNo < 0)
			focusFieldNo = 0;
	}

	for (int step = fieldCount - 1; step > 0; step--)
	{
		int fieldNo = (focusFieldNo + step) % fieldCount;
		if (focusIfPossible(fields.Get(fieldNo)))
			return fieldNo >= focusFieldNo;
	}

	return true;
}

void FieldsArea::setLayoutData(ContainerLayoutData * layoutData)
{
	ASSERTION_COBJECT(this);

	if (this->layoutData != NULL) {delete this->layoutData; this->layoutData = NULL;}
	this->layoutData = layoutData;
	layoutData->container = this;

	if (autoLayouting)
		layout();
}

void FieldsArea::setLayouter(Layouter * layouter)
{
	ASSERTION_COBJECT(this);

	if (this->layouter != NULL) {delete this->layouter; this->layouter = NULL;}
	this->layouter = layouter;

	if (autoLayouting)
		layout();
}
