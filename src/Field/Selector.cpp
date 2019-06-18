/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int Selector::NONE = -1;

Selector::Selector()
{
	ASSERTION_COBJECT(this);

	autoResizing = true;
	popupHeight = 256;
	popupListener = NULL;
	selectionNo = NONE;

	popupField.setColumnsResizable(false);
	popupField.setHeadlined(false);
	popupField.setHorizontallyScrollable(false);
	popupField.setType(ListField::TYPE_ONE);
	popupField.setVerticallyScrollable(true);

	ListField::Column * column;
	if ((column = new ListField::Column) == NULL)
		throw EOUTOFMEMORY;
	popupField.appendColumn(column);
	column->setEditable(false);

	ListFieldRowsFieldListener * rowsFieldListener;
	if ((rowsFieldListener = new ListFieldRowsFieldListener(this)) == NULL)
		throw EOUTOFMEMORY;
	popupField.getRowsField()->appendListener(rowsFieldListener);

	setMinimumSize(Vector(64, getMinimumSize().y));

	Desktop::getInstance()->getSkin()->setupSelector(this);
	resizeInternal(Vector(getSize().x, Desktop::getInstance()->getSkin()->getSelectorHeight(this)), true);
}

Selector::~Selector()
{
	ASSERTION_COBJECT(this);

	if (popupListener != NULL)
	{
		SingleFieldPopup * popup = dynamic_cast<SingleFieldPopup *>(popupField.getParent());
		if (popup != NULL)
		{
			popup->removeListener(popupListener);
			popupListener = NULL;
		}
	}

	options.UnlinkAll();
}

void Selector::appendOption(Option * option)
{
	ASSERTION_COBJECT(this);
	insertOption(options.GetCount(), option);
}

void Selector::autoResize()
{
	ASSERTION_COBJECT(this);
	resizeInternal(Vector(getSize().x, Desktop::getInstance()->getSkin()->getSelectorHeight(this)), true);
}

void Selector::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawSelector(context, this);
}

const ArrayList<Selector::Option> & Selector::getOptions()
{
	ASSERTION_COBJECT(this);
	return options;
}

ListField * Selector::getPopupField()
{
	ASSERTION_COBJECT(this);
	return &popupField;
}

int Selector::getPopupHeight()
{
	ASSERTION_COBJECT(this);
	return popupHeight;
}

Selector::Option * Selector::getSelection()
{
	ASSERTION_COBJECT(this);
	return options.Get(selectionNo);
}

int Selector::getSelectionNo()
{
	ASSERTION_COBJECT(this);
	return selectionNo;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Selector, InputField);
#endif

void Selector::insertOption(int position, Option * option)
{
	ASSERTION_COBJECT(this);

	if (getClientArea() == NULL)
		throw EILLEGALARGUMENT("The selector must be added to a client area before using it");
	if (option->getSelector() != NULL)
		throw EILLEGALARGUMENT("The option is already contained in a selector");
	if (position < 0 || position > options.GetCount())
		throw EILLEGALSTATE("The option position is out of range");

	option->setSelector(this);
	options.Insert(option, position);

	ListField::Row * row;
	if ((row = new ListField::Row) == NULL)
		throw EOUTOFMEMORY;
	popupField.insertRow(position, row);
	row->appendCell(option);

	if (selectionNo >= position)
		selectionNo++;

	invalidate();
}

bool Selector::isAutoResizing()
{
	ASSERTION_COBJECT(this);
	return autoResizing;
}

void Selector::onDestroying()
{
	ASSERTION_COBJECT(this);

	SingleFieldPopup * popup = dynamic_cast<SingleFieldPopup *>(popupField.getParent());
	if (popup != NULL)
	{
		popup->setContentField(NULL);
		popup->destroy();
	}

	InputField::onDestroying();
}

bool Selector::onMoveDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveDown(option1, option2);

	if (!isActiveIncludingAncestors())
		return true;

	if (option2)
		togglePopup();
	else
		if (selectionNo < options.GetCount() - 1)
			setSelection(selectionNo + 1);

	return true;
}

bool Selector::onMoveToEnd(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveToEnd(option1, option2);

	if (!isActiveIncludingAncestors())
		return true;

	if (option2)
		InputField::onMoveToEnd(option1, option2);
	else
		setSelection(options.IsEmpty() ? -1 : options.GetCount() - 1);

	return true;
}

bool Selector::onMoveToStart(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveToStart(option1, option2);

	if (!isActiveIncludingAncestors())
		return true;

	if (option2)
		InputField::onMoveToStart(option1, option2);
	else
		setSelection(options.IsEmpty() ? -1 : 0);

	return true;
}

bool Selector::onMoveUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onMoveUp(option1, option2);

	if (!isActiveIncludingAncestors())
		return true;

	if (option2)
		togglePopup();
	else
		if (selectionNo < 0 && !options.IsEmpty())
			setSelection(options.GetCount() - 1);
		else if (selectionNo > 0)
			setSelection(selectionNo - 1);

	return true;
}

void Selector::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	InputField::onPress(position, option1, option2);

	if (isActiveIncludingAncestors())
		togglePopup();
}

void Selector::prependOption(Option * option)
{
	ASSERTION_COBJECT(this);
	insertOption(0, option);
}

void Selector::removeOption(int position)
{
	ASSERTION_COBJECT(this);

	if (getClientArea() == NULL)
		throw EILLEGALARGUMENT("The selector must be added to a client area before using it");

	options.Unlink(position);
	popupField.removeRow(position);

	if (selectionNo == position)
		setSelection(NONE);
	else if (selectionNo > position)
		selectionNo--;

	invalidate();
}

void Selector::removeOptions()
{
	ASSERTION_COBJECT(this);

	if (getClientArea() == NULL)
		throw EILLEGALARGUMENT("The selector must be added to a client area before using it");

	options.UnlinkAll();
	popupField.removeRows();

	setSelection(NONE);

	invalidate();
}

void Selector::setAutoResizing(bool autoResizing)
{
	ASSERTION_COBJECT(this);

	this->autoResizing = autoResizing;
	if (autoResizing)
		autoResize();
}

void Selector::setCaps(bool caps)
{
	ASSERTION_COBJECT(this);

	if (isCaps() == caps)
		return;

	Field::setCaps(caps);
	if (autoResizing)
		autoResize();
}

void Selector::setFont(Font * font)
{
	ASSERTION_COBJECT(this);

	if (getFont() == font)
		return;

	Field::setFont(font);
	if (autoResizing)
		autoResize();
}

void Selector::setPopupHeight(int popupHeight)
{
	ASSERTION_COBJECT(this);
	this->popupHeight = popupHeight;
}

void Selector::setSelection(Selector::Option * selection)
{
	ASSERTION_COBJECT(this);
	setSelection(options.Find(selection));
}

void Selector::setSelection(int selectionNo)
{
	ASSERTION_COBJECT(this);

	if (getClientArea() == NULL)
		throw EILLEGALARGUMENT("The selector must be added to a client area before using it");

	if (selectionNo < 0 || selectionNo >= options.GetCount())
		selectionNo = NONE;

	if (this->selectionNo == selectionNo)
		return;

	int listenersCount = listeners.GetCount();
	int i;
	for (i=0; i<listenersCount; i++)
	{
		Selector::Listener * selectorListener
			= dynamic_cast<Selector::Listener *>(listeners.Get(i));
		if (selectorListener != NULL)
			if (selectorListener->onSelectionChanging(this, selectionNo) == VETO)
				return;
	}

	int oldSelectionNo = this->selectionNo;
	this->selectionNo = selectionNo;

	for (i=0; i<listenersCount; i++)
	{
		Selector::Listener * selectorListener
			= dynamic_cast<Selector::Listener *>(listeners.Get(i));
		if (selectorListener != NULL)
			selectorListener->onSelectionChanged(this, oldSelectionNo);
	}

	invalidate();
}

void Selector::setSelectionByModel(void * model)
{
	ASSERTION_COBJECT(this);

	for (int optionNo=0; optionNo<options.GetCount(); optionNo++)
		if (options.Get(optionNo)->getModel() == model)
		{
			setSelection(optionNo);
			return;
		}

	setSelection(NONE);
}

void Selector::togglePopup()
{
	ASSERTION_COBJECT(this);

	SingleFieldPopup * popup = dynamic_cast<SingleFieldPopup *>(popupField.getParent());
	if (popup != NULL)
	{
		popup->destroy();
		return;
	}

	if (!isActiveIncludingAncestors())
		return;

	Desktop * desktop = Desktop::getInstance();
	ListField::RowsField * rowsField = popupField.getRowsField();

	if ((popup = new SingleFieldPopup) == NULL)
		throw EOUTOFMEMORY;
	desktop->addPopup(popup);

	popup->setContentField(&popupField);
	popup->setAutoDestroying(false);

	int height = popupField.getRowsField()->getContentSize().y;
	if (height > popupHeight)
	{
		height = popupHeight;
		popupField.setVerticallyScrollable(true);
	}
	else
		popupField.setVerticallyScrollable(false);

	popup->resize(Vector(getSize().x, height));
	popup->resize(Vector(popup->getSize().x, popup->getSize().y + height - rowsField->getContentViewSize().y));

	Vector absolutePosition = getAbsolutePosition();
	if (absolutePosition.y + getSize().y + height > desktop->getSize().y)
		if (popup->getSize().y > absolutePosition.y)
	      popup->moveTo(Vector(absolutePosition.x, 0));
		else
	      popup->moveTo(absolutePosition - Vector(0, popup->getSize().y));
	else
		popup->moveTo(absolutePosition + Vector(0, getSize().y));

	popupField.getColumns().GetFirst()->setWidth(rowsField->getContentViewSize().x);

	desktop->getSkin()->setupSelectorPopup(this, popup);

	popupField.unselectRows();
	Option * selection = getSelection();
	if (selection != NULL)
	{
		popupField.selectRow(selection->getRow());
		popupField.setFocusColumnNo(0);
		popupField.setFocusRowNo(selectionNo);
	}

	if ((popupListener = new PopupListener(this)) == NULL)
		throw EOUTOFMEMORY;
	popup->appendListener(popupListener);

	invalidate();
}

String Selector::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Selector(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}

void Selector::tutorialSelect(int selectionNo, long time)
{
	ASSERTION_COBJECT(this);

	SingleFieldPopup * popup = dynamic_cast<SingleFieldPopup *>(popupField.getParent());
	if (popup == NULL)
	{
		if (time > 0)
			time /= 2;

		tutorialClick(PointerEffect::BUTTONEFFECT_LEFTCLICK, false, false, time);
	}

	popup = dynamic_cast<SingleFieldPopup *>(popupField.getParent());
	if (popup != NULL)
		popupField.tutorialClick(selectionNo, 0, PointerEffect::BUTTONEFFECT_LEFTCLICK, false, false, time);
}

bool Selector::validateRequired(bool nonNullModelRequired)
{
	ASSERTION_COBJECT(this);

	Option * selection = getSelection();
	if (selection != NULL && nonNullModelRequired && selection->getModel() == NULL)
		selection = NULL;

	if (selection == NULL)
	{
		showBalloonPopup(STRING_GLAZIERY_VALIDATE_SELECTOR_REQUIRED);
		return false;
	}

	return true;
}


Selector::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void Selector::Listener::onSelectionChanged(Selector * selector, int oldSelectionNo)
{
	ASSERTION_COBJECT(this);
}

VetoMode Selector::Listener::onSelectionChanging(Selector * selector, int newSelectionNo)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}


Selector::ListFieldRowsFieldListener::ListFieldRowsFieldListener(Selector * selector)
{
	ASSERTION_COBJECT(this);
	this->selector = selector;
}

void Selector::ListFieldRowsFieldListener::commitSelection(EventTarget * target)
{
	ASSERTION_COBJECT(this);

	if (selector->popupListener != NULL)
	{
		ListField * listField = (ListField *) ((ListField::RowsField *) target)->getParent();
		selector->setSelection(listField->getFocusRowNo());
		listField->getParent()->destroy();
	}
}

void Selector::ListFieldRowsFieldListener::onContentClick(Component * component, Vector position,
	bool option1, bool option2)
{
	ASSERTION_COBJECT(this);
	commitSelection(component);
}

void Selector::ListFieldRowsFieldListener::onEnter(EventTarget * target, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (selector->popupListener != NULL)
	commitSelection(target);
}


Selector::Option::Option()
{
	ASSERTION_COBJECT(this);
	selector = NULL;
}

void Selector::Option::appendElement(FieldElement * element)
{
	ASSERTION_COBJECT(this);

	if (selector == NULL)
		throw EILLEGALSTATE("The option must be added to a selector before using it");

	((ListField::Cell *) this)->appendElement(element);
}

const ArrayList<FieldElement> & Selector::Option::getElements()
{
	ASSERTION_COBJECT(this);

	if (selector == NULL)
		throw EILLEGALSTATE("The option must be added to a selector before using it");

	return ((ListField::Cell *) this)->getElements();
}

Selector * Selector::Option::getSelector()
{
	ASSERTION_COBJECT(this);
	return selector;
}

void Selector::Option::insertElement(int position, FieldElement * element)
{
	ASSERTION_COBJECT(this);

	if (selector == NULL)
		throw EILLEGALSTATE("The option must be added to a selector before using it");

	((ListField::Cell *) this)->insertElement(position, element);
}

bool Selector::Option::isSelected()
{
	ASSERTION_COBJECT(this);

	if (selector == NULL)
		throw EILLEGALSTATE("The option must be added to a selector before using it");

	return selector->getSelection() == this;
}

void Selector::Option::prependElement(FieldElement * element)
{
	ASSERTION_COBJECT(this);

	if (selector == NULL)
		throw EILLEGALSTATE("The option must be added to a selector before using it");

	((ListField::Cell *) this)->prependElement(element);
}

void Selector::Option::removeElement(int position)
{
	ASSERTION_COBJECT(this);

	if (selector == NULL)
		throw EILLEGALSTATE("The option must be added to a selector before using it");

	((ListField::Cell *) this)->removeElement(position);
}

void Selector::Option::select()
{
	ASSERTION_COBJECT(this);

	if (selector == NULL)
		throw EILLEGALSTATE("The option must be added to a selector before using it");

	selector->setSelection(this);
}

void Selector::Option::setSelector(Selector * selector)
{
	ASSERTION_COBJECT(this);

	if (this->selector != NULL)
		throw EILLEGALARGUMENT("The option is already contained in another selector");
	this->selector = selector;
}


Selector::PopupListener::PopupListener(Selector * selector)
{
	ASSERTION_COBJECT(this);
	this->selector = selector;
}

void Selector::PopupListener::onDestroying(Component * component)
{
	ASSERTION_COBJECT(this);

	// Invalidate the selector, because the selection may not have changed,
	// but we require the invalidation, if the popup has been canceled,
	// otherwise the focus would not be drawn correctly.
	selector->invalidate();
	selector->popupListener = NULL;
}
