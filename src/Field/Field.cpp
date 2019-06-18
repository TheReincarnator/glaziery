/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Field::Field()
{
	ASSERTION_COBJECT(this);

	active = true;
	attachmentFocussing = false;
	attachmentType = ACTIVE_IF_ANY_CHECKED;
	caps = false;
	fieldLayoutData = NULL;
	font = Desktop::getInstance()->getSkin()->getDefaultFieldFont();
	groupField = NULL;
	likeTitle = true;
	textDeferred = "";

	FieldLayoutData * fieldLayoutData;
	if ((fieldLayoutData = new FieldLayoutData) == NULL)
		throw EOUTOFMEMORY;
	setFieldLayoutData(fieldLayoutData);

	Desktop::getInstance()->getSkin()->setupField(this);
}

Field::~Field()
{
	ASSERTION_COBJECT(this);

	while (!attachmentCheckListeners.IsEmpty())
	{
		InputField * checkable = getAttachedCheckable(0);
		if (checkable != NULL)
			detachFrom(checkable);
		else
			attachmentCheckListeners.DeleteFirst();
	}

	if (fieldLayoutData != NULL) {delete fieldLayoutData; fieldLayoutData = NULL;}
}

void Field::addHotKey(char hotKey)
{
	ASSERTION_COBJECT(this);

	FieldsArea * area = getFieldsArea();
	if (area != NULL)
		area->addHotKey(hotKey, this);
}

void Field::attachTo(InputField * checkable)
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<attachmentCheckListeners.GetCount(); i++)
		if (getAttachedCheckable(i) == checkable)
			return;

	Field::Listener * attachmentCheckListener = NULL;
	if (dynamic_cast<CheckBox *>(checkable) != NULL)
	{
		if ((attachmentCheckListener = new AttachmentCheckBoxListener(this, (CheckBox *) checkable)) == NULL)
			throw EOUTOFMEMORY;
	}
	else if (dynamic_cast<RadioBox *>(checkable) != NULL)
	{
		if ((attachmentCheckListener = new AttachmentRadioBoxListener(this, (RadioBox *) checkable)) == NULL)
			throw EOUTOFMEMORY;
	}
	else
		return;

	checkable->appendListener(attachmentCheckListener);
	attachmentCheckListeners.Append(attachmentCheckListener);

	setActiveFromAttachments();
}

void Field::destroy()
{
	ASSERTION_COBJECT(this);

	if (groupField != NULL)
		groupField->fields.Unlink(this);

	Component::destroy();
}

void Field::detachFrom(InputField * checkable)
{
	ASSERTION_COBJECT(this);

	for (int i=0; i<attachmentCheckListeners.GetCount(); i++)
		if (getAttachedCheckable(i) == checkable)
		{
			Field::Listener * attachmentCheckListener = attachmentCheckListeners.Unlink(i);
			if (!checkable->isDisposed())
				checkable->removeListener(attachmentCheckListener);

			setActiveFromAttachments();

			return;
		}
}

String Field::escapeHotKeys(const String & text)
{
	String copy(text);
	copy.Replace("&", "&&");
	return copy;
}

void Field::executeDeferrals()
{
	ASSERTION_COBJECT(this);

	Component::executeDeferrals();

	if (textDeferred != getText())
		setText(textDeferred);
}

InputField * Field::getAttachedCheckable(int no)
{
	ASSERTION_COBJECT(this);

	Field::Listener * listener = attachmentCheckListeners.Get(no);
	if (dynamic_cast<AttachmentCheckBoxListener *>(listener) != NULL)
		return ((AttachmentCheckBoxListener *) listener)->getCheckBox();
	else if (dynamic_cast<AttachmentRadioBoxListener *>(listener) != NULL)
		return ((AttachmentRadioBoxListener *) listener)->getRadioBox();
	else
		return NULL;
}

Field::AttachmentType Field::getAttachmentType()
{
	ASSERTION_COBJECT(this);
	return attachmentType;
}

ClientArea * Field::getClientArea()
{
	ASSERTION_COBJECT(this);

	Component * ancestor = getParent();
	while (ancestor != NULL && dynamic_cast<ClientArea *>(ancestor) == NULL)
		ancestor = ancestor->getParent();

	return (ClientArea *) ancestor;
}

FieldsArea * Field::getFieldsArea()
{
	ASSERTION_COBJECT(this);
	return dynamic_cast<FieldsArea *>(getClientArea());
}

FieldLayoutData * Field::getFieldLayoutData()
{
	ASSERTION_COBJECT(this);
	return fieldLayoutData;
}

class Font * Field::getFont()
{
	ASSERTION_COBJECT(this);
	return font;
}

GroupField * Field::getGroupField()
{
	ASSERTION_COBJECT(this);
	return groupField;
}

Vector Field::getPositionOnClientArea()
{
	ASSERTION_COBJECT(this);

	Vector position = getPosition();

	Component * ancestor = getParent();
	while (ancestor != NULL && dynamic_cast<ClientArea *>(ancestor) == NULL)
	{
		position += ancestor->getPosition();
		ancestor = ancestor->getParent();
	}

	return position;
}

Vector Field::getPositionOnWindow()
{
	ASSERTION_COBJECT(this);

	Vector position = getPosition();

	Component * ancestor = getParent();
	while (ancestor != NULL && dynamic_cast<Window *>(ancestor) == NULL)
	{
		position += ancestor->getPosition();
		ancestor = ancestor->getParent();
	}

	return position;
}

String Field::getText()
{
	ASSERTION_COBJECT(this);

	String * textAttribute = getTextAttribute();
	if (textAttribute == NULL)
		return "";

	return *textAttribute;
}

String * Field::getTextAttribute()
{
	ASSERTION_COBJECT(this);
	return NULL;
}

Window * Field::getWindow()
{
	ASSERTION_COBJECT(this);

	Component * ancestor = getParent();
	while (ancestor != NULL && dynamic_cast<Window *>(ancestor) == NULL)
		ancestor = ancestor->getParent();

	return (Window *) ancestor;
}

bool Field::hasFocus()
{
	ASSERTION_COBJECT(this);

	ClientArea * area = getClientArea();
	if (area != NULL)
		return area->getFocusChild() == this && area->hasFocus();
	else
		return Component::hasFocus();
}

void Field::hide()
{
	ASSERTION_COBJECT(this);

	Component::hide();

	FieldsArea * area = getFieldsArea();
	if (area != NULL)
		area->layoutIfAutomatic();
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Field, Component);
#endif

bool Field::isAcceptingPressesToFocus()
{
	ASSERTION_COBJECT(this);
	return true;
}

bool Field::isActive()
{
	ASSERTION_COBJECT(this);
	return active;
}

bool Field::isActiveIncludingAncestors()
{
	ASSERTION_COBJECT(this);

	if (!active)
		return false;

	if (groupField != NULL)
		if (!groupField->isActiveIncludingAncestors())
			return false;

	Field * parentField = dynamic_cast<Field *>(getParent());
	if (parentField != NULL)
		if (!parentField->isActiveIncludingAncestors())
			return false;

	return true;
}

bool Field::isAttachmentFocussing()
{
	ASSERTION_COBJECT(this);
	return attachmentFocussing;
}

bool Field::isCaps()
{
	ASSERTION_COBJECT(this);
	return caps;
}

bool Field::isLikeTitle(Vector position)
{
	ASSERTION_COBJECT(this);
	return likeTitle;
}

bool Field::moveInternal(Vector position, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	if (Component::moveInternal(position, notifyParent))
	{
		FieldsArea * area = getFieldsArea();
		if (area != NULL)
			area->layoutIfAutomatic();

		return true;
	}
	else
		return false;
}

void Field::notifyListenersTextChanged()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Field::Listener * fieldListener
			= dynamic_cast<Field::Listener *>(listeners.Get(i));
		if (fieldListener != NULL)
			fieldListener->onTextChanged(this);
	}
}

bool Field::notifyListenersTextChanging()
{
	ASSERTION_COBJECT(this);

	int listenersCount = listeners.GetCount();
	for (int i=0; i<listenersCount; i++)
	{
		Field::Listener * fieldListener
			= dynamic_cast<Field::Listener *>(listeners.Get(i));
		if (fieldListener != NULL)
			if (fieldListener->onTextChanging(this) == VETO)
				return true;
	}

	return false;
}

bool Field::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = Component::onCharacter(character, option1, option2);

	if (onHotKeyByCharacter(character, option1, option2))
		consumed = true;

	return consumed;
}

void Field::onElementChanged(FieldElement * element)
{
	ASSERTION_COBJECT(this);
	invalidate();
}

bool Field::onHotKeyByCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	FieldsArea * area = getFieldsArea();
	if (area != NULL && area->getHotKeyComponent(character) != NULL)
	{
		area->onHotKey(character, option1, option2);
		return true;
	}

	return false;
}

void Field::onPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	if (!isAcceptingPressesToFocus() || !isActiveIncludingAncestors())
		return;

	FieldsArea * fieldArea = getFieldsArea();
	if (fieldArea != NULL)
	{
		const ArrayList<Field> & fields = fieldArea->getFields();
		int fieldCount = fields.GetCount();

		int fieldNo = fields.Find(this);
		for (int step = 0; step < fieldCount; step++)
		{
			InputField * field = dynamic_cast<InputField *>(
				fields.Get((fieldNo + step) % fieldCount));
			if (field != NULL && field->isVisible())
			{
				if (field->isActiveIncludingAncestors())
					field->setFocus();
				return;
			}
		}
	}
}

void Field::onWheel(int delta)
{
	ASSERTION_COBJECT(this);

	Component::onWheel(delta);

	ClientArea * area = getClientArea();
	if (area != NULL)
		area->onWheel(delta);
}

void Field::removeHotKey(unsigned char hotKey)
{
	ASSERTION_COBJECT(this);

	FieldsArea * area = getFieldsArea();
	if (area != NULL)
		area->removeHotKey(hotKey);
}

bool Field::resizeInternal(Vector size, bool notifyParent)
{
	ASSERTION_COBJECT(this);

	if (Component::resizeInternal(size, notifyParent))
	{
		FieldsArea * area = getFieldsArea();
		if (area != NULL)
			area->layoutIfAutomatic();

		invalidate();

		return true;
	}
	else
		return false;
}

void Field::setActive(bool active)
{
	ASSERTION_COBJECT(this);

	if (this->active == active)
		return;

	this->active = active;

	if (!active && hasFocus())
	{
		FieldsArea * area = getFieldsArea();
		if (area != NULL)
			area->setFocusToNextField();
	}

	invalidate();
}

void Field::setActiveFromAttachments()
{
	ASSERTION_COBJECT(this);

	if (attachmentCheckListeners.IsEmpty())
		return;

	bool mustBeAll = attachmentType == ACTIVE_IF_ALL_CHECKED || attachmentType == ACTIVE_IF_ALL_UNCHECKED;
	bool mustBeChecked = attachmentType == ACTIVE_IF_ANY_CHECKED || attachmentType == ACTIVE_IF_ALL_CHECKED;

	bool active = mustBeAll;
	for (int i=0; i<attachmentCheckListeners.GetCount(); i++)
	{
		InputField * checkable = getAttachedCheckable(i);
		bool checked = false;
		if (dynamic_cast<CheckBox *>(checkable) != NULL)
			checked = ((CheckBox *) checkable)->isChecked();
		else if (dynamic_cast<RadioBox *>(checkable) != NULL)
			checked = ((RadioBox *) checkable)->isChecked();

		if (mustBeAll && mustBeChecked != checked || !mustBeAll && mustBeChecked == checked)
			active = !mustBeAll;
	}

	setActive(active);

	if (attachmentFocussing && active && dynamic_cast<InputField *>(this) != NULL)
		((InputField *) this)->setFocus();
}

void Field::setAttachmentFocussing(bool attachmentFocussing)
{
	ASSERTION_COBJECT(this);
	this->attachmentFocussing = attachmentFocussing;
	setActiveFromAttachments();
}

void Field::setAttachmentType(Field::AttachmentType attachmentType)
{
	ASSERTION_COBJECT(this);
	this->attachmentType = attachmentType;
	setActiveFromAttachments();
}

void Field::setCaps(bool caps)
{
	ASSERTION_COBJECT(this);

	if (this->caps == caps)
		return;

	this->caps = caps;
	invalidate();
}

void Field::setFieldLayoutData(FieldLayoutData * fieldLayoutData)
{
	ASSERTION_COBJECT(this);

	if (this->fieldLayoutData != NULL) {delete this->fieldLayoutData; this->fieldLayoutData = NULL;}
	this->fieldLayoutData = fieldLayoutData;
	fieldLayoutData->field = this;

	FieldsArea * area = getFieldsArea();
	if (area != NULL)
		area->layoutIfAutomatic();
}

void Field::setFont(Font * font)
{
	ASSERTION_COBJECT(this);

	if (this->font == font)
		return;

	this->font = font;
	invalidate();
}

void Field::setFontId(const String & fontId)
{
	ASSERTION_COBJECT(this);
	setFont(Desktop::getInstance()->getPlatformAdapter()->getFont(fontId));
}

void Field::setFontId(const char * fontId)
{
	ASSERTION_COBJECT(this);
	setFont(Desktop::getInstance()->getPlatformAdapter()->getFont(String(fontId)));
}

void Field::setLikeTitle(bool likeTitle)
{
	ASSERTION_COBJECT(this);

	if (this->likeTitle == likeTitle)
		return;

	this->likeTitle = likeTitle;
	invalidate();
}

void Field::setText(const String & text)
{
	ASSERTION_COBJECT(this);

	String * textAttribute = getTextAttribute();
	if (textAttribute == NULL)
		return;

	if (*textAttribute == text)
	{
		textDeferred = text;
		return;
	}

	if (notifyListenersTextChanging())
		return;

	removeHotKey(ClientArea::findHotKey(*textAttribute));
	*textAttribute = text;
	textDeferred = text;
	addHotKey(ClientArea::findHotKey(text));

	notifyListenersTextChanged();

	invalidate();
}

void Field::setTextDeferred(const String & text)
{
	ASSERTION_COBJECT(this);

	Mutex * mutex = Desktop::getInstance()->getDeferralMutex();
	if (!mutex->lock())
		return;

	textDeferred = text;
	Desktop::getInstance()->deferObject(this);

	mutex->release();
}

void Field::show()
{
	ASSERTION_COBJECT(this);

	Component::show();

	FieldsArea * area = getFieldsArea();
	if (area != NULL)
		area->layoutIfAutomatic();
}

String Field::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Field(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}


Field::Listener::~Listener()
{
	ASSERTION_COBJECT(this);
}

void Field::Listener::onTextChanged(Field * field)
{
	ASSERTION_COBJECT(this);
}

VetoMode Field::Listener::onTextChanging(Field * field)
{
	ASSERTION_COBJECT(this);
	return NO_VETO;
}
