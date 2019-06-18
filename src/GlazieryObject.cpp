/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


GlazieryObject::GlazieryObject()
{
	ASSERTION_COBJECT(this);

	model = NULL;
	modelDestructor = NULL;
	skinData = NULL;
}

GlazieryObject::~GlazieryObject()
{
	ASSERTION_COBJECT(this);

	if (skinData != NULL) {delete skinData; skinData = NULL;}

	if (model != NULL && modelDestructor != NULL)
		modelDestructor(model);
}

void GlazieryObject::destructor(void * model)
{
	delete (GlazieryObject *) model;
}

void * GlazieryObject::getModel()
{
	ASSERTION_COBJECT(this);
	return model;
}

SkinData * GlazieryObject::getSkinData()
{
	ASSERTION_COBJECT(this);
	return skinData;
}

void GlazieryObject::setModel(void * model, void (*modelDestructor)(void * model))
{
	ASSERTION_COBJECT(this);

	if (this->model != NULL && this->modelDestructor != NULL)
		this->modelDestructor(this->model);

	this->model = model;
	this->modelDestructor = modelDestructor;
}

void GlazieryObject::setSkinData(SkinData * skinData)
{
	ASSERTION_COBJECT(this);

	if (this->skinData != NULL) {delete this->skinData; this->skinData = NULL;}
	this->skinData = skinData;
}

String GlazieryObject::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("GlazieryObject@%lX", this);
	return string;
}
