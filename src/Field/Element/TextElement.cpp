/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


TextElement::TextElement()
{
	ASSERTION_COBJECT(this);
	editPopup = NULL;
}

TextElement::~TextElement()
{
	ASSERTION_COBJECT(this);

	if (editPopup != NULL)
	{
		editPopup->destroy();
		editPopup = NULL;
	}
}

void TextElement::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawTextElement(context, this);
}

void TextElement::edit(Vector position, Vector size, ScrollingField * scrollingField)
{
	ASSERTION_COBJECT(this);

	if (editPopup == NULL)
	{
		if ((editPopup = new EditPopup(this, scrollingField)) == NULL)
			throw EOUTOFMEMORY;
		Desktop::getInstance()->addPopup(editPopup);

		TextField * popupTextField;
		if ((popupTextField = new TextField) == NULL)
			throw EOUTOFMEMORY;
		editPopup->setContentField(popupTextField);

		popupTextField->setText(text);
		popupTextField->selectAll();

		Desktop::getInstance()->getSkin()->setupTextElementEditPopup(editPopup, popupTextField, this,
			position, size);

		TextFieldListener * textFieldListener;
		if ((textFieldListener = new TextFieldListener(editPopup)) == NULL)
			throw EOUTOFMEMORY;
		popupTextField->appendListener(textFieldListener);
	}
}

TextElement::EditPopup * TextElement::getEditPopup()
{
	ASSERTION_COBJECT(this);
	return editPopup;
}

Vector TextElement::getSize()
{
	ASSERTION_COBJECT(this);
	return getField()->getFont()->getSize(text, getField()->isCaps());
}

const String & TextElement::getText()
{
	ASSERTION_COBJECT(this);
	return text;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(TextElement, FieldElement);
#endif

bool TextElement::setText(const String & text)
{
	ASSERTION_COBJECT(this);

	if (this->text == text)
		return true;

	String oldText = this->text;

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		TextElement::Listener * textElementListener
			= dynamic_cast<TextElement::Listener *>(listeners.Get(i));
		if (textElementListener != NULL)
			if (textElementListener->onTextChanging(this, text) == VETO)
				return false;
	}

	this->text = text;

	for (int i=0; i<listenersCount; i++)
	{
		TextElement::Listener * textElementListener
			= dynamic_cast<TextElement::Listener *>(listeners.Get(i));
		if (textElementListener != NULL)
			textElementListener->onTextChanged(this, oldText);
	}

	getField()->onElementChanged(this);

	return true;
}


TextElement::EditPopup::EditPopup(TextElement * element, ScrollingField * scrollingField)
{
	ASSERTION_COBJECT(this);

	this->element = element;
	this->scrollingField = scrollingField;
	scrollingFieldListener = NULL;

	if (scrollingField != NULL)
	{
		if ((scrollingFieldListener = new ScrollingFieldListener(this)) == NULL)
			throw EOUTOFMEMORY;
		scrollingField->appendListener(scrollingFieldListener);
		scrollingFieldListener->addReference();
	}
}

void TextElement::EditPopup::apply()
{
	TextField * field = (TextField *) getContentField();
	String newText = field->getText();
	Field * elementField = element->getField();

	// Commit any changes into the element
	element->setText(newText);

	// Invalidate the field, because the text may not have changed,
	// but we require the invalidation, if the popup has been canceled,
	// otherwise the focus would not be drawn correctly.
	elementField->invalidate();
}

void TextElement::EditPopup::cancel()
{
	if (isDestroying())
		return;

	SingleFieldPopup::cancel();
	apply();
}

void TextElement::EditPopup::moveDown()
{
	ASSERTION_COBJECT(this);

	if (isDisposed())
		return;

	ListField * listField = dynamic_cast<ListField *>(element->getField());
	if (listField != NULL)
	{
		ListField::Cell * cell = (ListField::Cell *) element->getParent();
		ListField::Column * column = (ListField::Column *) cell->getColumn();
		int columnNo = listField->getColumnNo(column);
		ListField::Row * row = (ListField::Row *) cell->getRow();
		int rowNo = listField->getRowNo(row);

		if (rowNo < listField->getRows().GetCount() - 1)
			rowNo++;
		else
			return;

		cancel();

		listField->setSelectedRow(rowNo);
		listField->setFocusRowNo(rowNo);
		listField->onEdit();
	}
}

void TextElement::EditPopup::moveLeft()
{
	ASSERTION_COBJECT(this);

	if (isDisposed())
		return;

	ListField * listField = dynamic_cast<ListField *>(element->getField());
	if (listField != NULL)
	{
		ListField::Cell * cell = (ListField::Cell *) element->getParent();
		ListField::Column * column = (ListField::Column *) cell->getColumn();
		int columnNo = listField->getColumnNo(column);
		ListField::Row * row = (ListField::Row *) cell->getRow();
		int rowNo = listField->getRowNo(row);
		const ArrayList<ListField::Column> & columns = listField->getColumns();

		int steps = 0;
		do
		{
			if (columnNo > 0)
				columnNo--;
			else if (rowNo > 0)
			{
				rowNo--;
				columnNo = columns.GetCount() - 1;
			}
			else
				return;

			steps++;
		} while (steps < columns.GetCount() && !columns.Get(columnNo)->isEditable());

		cancel();

		listField->setSelectedRow(rowNo);
		listField->setFocusRowNo(rowNo);
		listField->setFocusColumnNo(columnNo);
		listField->onEdit();
	}
}

void TextElement::EditPopup::moveRight()
{
	ASSERTION_COBJECT(this);

	if (isDisposed())
		return;

	ListField * listField = dynamic_cast<ListField *>(element->getField());
	if (listField != NULL)
	{
		ListField::Cell * cell = (ListField::Cell *) element->getParent();
		ListField::Column * column = (ListField::Column *) cell->getColumn();
		int columnNo = listField->getColumnNo(column);
		ListField::Row * row = (ListField::Row *) cell->getRow();
		int rowNo = listField->getRowNo(row);
		const ArrayList<ListField::Column> & columns = listField->getColumns();

		int steps = 0;
		do
		{
			if (columnNo < columns.GetCount() - 1)
				columnNo++;
			else if (rowNo < listField->getRows().GetCount() - 1)
			{
				rowNo++;
				columnNo = 0;
			}
			else
				return;

			steps++;
		} while (steps < columns.GetCount() && !columns.Get(columnNo)->isEditable());

		cancel();

		listField->setSelectedRow(rowNo);
		listField->setFocusRowNo(rowNo);
		listField->setFocusColumnNo(columnNo);
		listField->onEdit();
	}
}

void TextElement::EditPopup::moveUp()
{
	ASSERTION_COBJECT(this);

	if (isDisposed())
		return;

	ListField * listField = dynamic_cast<ListField *>(element->getField());
	if (listField != NULL)
	{
		ListField::Cell * cell = (ListField::Cell *) element->getParent();
		ListField::Column * column = (ListField::Column *) cell->getColumn();
		int columnNo = listField->getColumnNo(column);
		ListField::Row * row = (ListField::Row *) cell->getRow();
		int rowNo = listField->getRowNo(row);

		if (rowNo > 0)
			rowNo--;
		else
			return;

		cancel();

		listField->setSelectedRow(rowNo);
		listField->setFocusRowNo(rowNo);
		listField->onEdit();
	}
}

bool TextElement::EditPopup::onCancel()
{
	ASSERTION_COBJECT(this);

	// Reset any changes made to the field
	TextField * field = (TextField *) getContentField();
	field->setText(element->getText());

	SingleFieldPopup::onCancel();

	return true;
}

void TextElement::EditPopup::onDestroying()
{
	ASSERTION_COBJECT(this);

	element->editPopup = NULL;
	if (scrollingFieldListener != NULL)
	{
		if (scrollingField != NULL)
			scrollingField->removeListener(scrollingFieldListener);

		ASSERTION(scrollingFieldListener->getReferences() == 1);
		scrollingFieldListener->release();
		scrollingFieldListener = NULL;
	}

	SingleFieldPopup::onDestroying();
}

bool TextElement::EditPopup::onEnter(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	cancel();

	return true;
}


TextElement::EditPopup::ScrollingFieldListener::ScrollingFieldListener(EditPopup * popup)
{
	ASSERTION_COBJECT(this);
	this->popup = popup;
}

void TextElement::EditPopup::ScrollingFieldListener::onScrollPositionNowSet(ScrollingField * field,
	Vector previousPosition)
{
	ASSERTION_COBJECT(this);

	Vector delta = previousPosition - field->getScrollPositionNow();
	popup->moveRelative(delta);
}


TextElement::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void TextElement::Listener::onTextChanged(TextElement * element, const String & oldText)
{
	ASSERTION_COBJECT(this);
}

VetoMode TextElement::Listener::onTextChanging(TextElement * element,
	const String & newText)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}


TextElement::TextFieldListener::TextFieldListener(EditPopup * popup)
{
	ASSERTION_COBJECT(this);
	this->popup = popup;
}

void TextElement::TextFieldListener::onBackTab(EventTarget * target, bool secondary)
{
	ASSERTION_COBJECT(this);

	if (!secondary)
		popup->moveLeft();
}

void TextElement::TextFieldListener::onMoveDown(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
	popup->moveDown();
}

void TextElement::TextFieldListener::onMoveUp(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
	popup->moveUp();
}

void TextElement::TextFieldListener::onTab(EventTarget * target, bool secondary)
{
	ASSERTION_COBJECT(this);

	if (!secondary)
		popup->moveRight();
}
