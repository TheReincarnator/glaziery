/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


FieldElement::FieldElement()
{
	ASSERTION_COBJECT(this);

	alignment = ALIGN_CENTER;
	editable = true;
	field = NULL;
	parent = NULL;
}

FieldElement::~FieldElement()
{
	ASSERTION_COBJECT(this);
}

void FieldElement::drawElements(DrawingContext & elementContext, const ArrayList<FieldElement> & elements,
	int height, int elementSpaceWidth, bool textElements, bool nonTextElements)
{
	int x = 0;
	for (int i=0; i<elements.GetCount(); i++)
	{
		FieldElement * element = elements.Get(i);
		Vector elementSize = element->getSize();

		bool textElement = dynamic_cast<TextElement *>(element) != NULL;
		if (textElement && textElements || !textElement && nonTextElements)
		{
			if (element->getAlignment() == FieldElement::ALIGN_CENTER)
				element->draw(DrawingContext(elementContext, Vector(x, (height - elementSize.y) / 2)));
			else if (element->getAlignment() == FieldElement::ALIGN_TOP)
				element->draw(DrawingContext(elementContext, Vector(x, 0)));
			else
				element->draw(DrawingContext(elementContext, Vector(x, height - elementSize.y)));
		}

		x += elementSize.x;
		if (i > 0)
			x += elementSpaceWidth;
	}
}

FieldElement::Alignment FieldElement::getAlignment(void)
{
	ASSERTION_COBJECT(this);
	return alignment;
}

Field * FieldElement::getField()
{
	ASSERTION_COBJECT(this);
	return field;
}

void * FieldElement::getParent()
{
	ASSERTION_COBJECT(this);
	return parent;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(FieldElement, Listenable);
#endif

bool FieldElement::isEditable()
{
	ASSERTION_COBJECT(this);
	return editable;
}

void FieldElement::setAlignment(FieldElement::Alignment alignment)
{
	ASSERTION_COBJECT(this);

	if (this->alignment == alignment)
		return;

	this->alignment = alignment;
	getField()->onElementChanged(this);
}

void FieldElement::setEditable(bool editable)
{
	ASSERTION_COBJECT(this);
	this->editable = editable;
}

void FieldElement::setFieldAndParent(Field * field, void * parent)
{
	ASSERTION_COBJECT(this);

	if (this->field != NULL)
		throw EILLEGALARGUMENT("The field element is already contained in another field");

	this->field = field;
	this->parent = parent;
}
