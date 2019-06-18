/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


GroupField::GroupField()
{
	ASSERTION_COBJECT(this);

	bordered = false;
	groupLayoutData = NULL;
	setMinimumSize(Vector());
	resize(Vector());

	ContainerLayoutData * groupLayoutData;
	if ((groupLayoutData = new ContainerLayoutData) == NULL)
		throw EOUTOFMEMORY;
	setGroupLayoutData(groupLayoutData);

	Desktop::getInstance()->getSkin()->setupGroupField(this);
}

GroupField::~GroupField()
{
	ASSERTION_COBJECT(this);

	while (!fields.IsEmpty())
	{
		Field * field = fields.UnlinkFirst();
		field->groupField = getGroupField();

		if (field->groupField != NULL)
			field->parent = field->groupField;
		else
			field->parent = getClientArea();
	}

	if (groupLayoutData != NULL) {delete groupLayoutData; groupLayoutData = NULL;}
}

void GroupField::appendField(Field * field)
{
	ASSERTION_COBJECT(this);
	insertField(fields.GetCount(), field);
}

void GroupField::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);

	Desktop::getInstance()->getSkin()->drawGroupField(context, this);

	#ifdef GL_DEBUG_FIELDBOUNDS
		PlatformAdapter * adapter = Desktop::getInstance()->getPlatformAdapter();
	#endif

	for (int i=0; i<fields.GetCount(); i++)
	{
		Field * field = fields.Get(i);
		if (field->isVisible())
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

EventTarget * GroupField::getEventTargetAt(Vector position)
{
	ASSERTION_COBJECT(this);

	EventTarget * superTarget = Field::getEventTargetAt(position);
	if (superTarget != this)
		return superTarget;

	if (position >= Vector() && position < getSize())
		for (int i = fields.GetCount() - 1; i>=0; i--)
		{
			Field * field = fields.Get(i);
			if (field->isVisible() && position >= field->getPosition()
				&& position < field->getPosition() + field->getSize())
				return field->getEventTargetAt(position - field->getPosition());
		}

	return this;
}

const ArrayList<Field> & GroupField::getFields()
{
	ASSERTION_COBJECT(this);
	return fields;
}

ContainerLayoutData * GroupField::getGroupLayoutData()
{
	ASSERTION_COBJECT(this);
	return groupLayoutData;
}

String * GroupField::getTextAttribute()
{
	ASSERTION_COBJECT(this);
	return &text;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(GroupField, Field);
#endif

void GroupField::insertField(int position, Field * field)
{
	ASSERTION_COBJECT(this);

	if (getClientArea() == NULL)
		throw EILLEGALSTATE("The group field must be added to a client area before using it");

	if (getClientArea() != field->getClientArea())
		throw EILLEGALARGUMENT("The field is not a member of the group field's client area");

	if (field->getGroupField() != NULL)
		throw EILLEGALARGUMENT("The field has already been added to another group field");

	fields.Insert(field, position);
	field->groupField = this;
	unsetComponentParent(field);
	setComponentParent(field);

	FieldsArea * fieldsArea = getFieldsArea();
	if (fieldsArea != NULL)
		fieldsArea->layoutIfAutomatic();

	invalidate();
}

bool GroupField::isAcceptingPressesToFocus()
{
	ASSERTION_COBJECT(this);
	return false;
}

bool GroupField::isBordered()
{
	ASSERTION_COBJECT(this);
	return bordered;
}

void GroupField::prependField(Field * field)
{
	ASSERTION_COBJECT(this);
	insertField(0, field);
}

void GroupField::setBordered(bool bordered)
{
	ASSERTION_COBJECT(this);

	this->bordered = bordered;
	invalidate();
}

void GroupField::setGroupLayoutData(ContainerLayoutData * groupLayoutData)
{
	ASSERTION_COBJECT(this);

	if (this->groupLayoutData != NULL) {delete this->groupLayoutData; this->groupLayoutData = NULL;}
	this->groupLayoutData = groupLayoutData;
	groupLayoutData->container = this;

	FieldsArea * fieldsArea = getFieldsArea();
	if (fieldsArea != NULL)
		fieldsArea->layoutIfAutomatic();
}

String GroupField::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("GroupField(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}
