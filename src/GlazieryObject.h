/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_GLAZIERYOBJECT_H
#define __GLAZIERY_GLAZIERYOBJECT_H


namespace glaziery
{
	/**
	 * A glaziery object is a parent super class for most of all classes within Glaziery.
	 * Its main purpose is to be the container for the application's model information and the skin data.
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class GlazieryObject : public CObject
	#else
		class GlazieryObject
	#endif
	{
		private:

			/**
			 * The application (or game) defined data. Typically, there is an object
			 * that contains application data that corresponds to this graphical element.
			 * This object (or raw data) is stored here. Glaziery will not perform
			 * any operation on it, but you can use it to identify application objects
			 * for given event targets (such as components).
			 */
			void * model;

			/**
			 * An optional static method that is called, whenever the
			 * glaziery object is destroyed or when a new model is set.
			 */
			void (*modelDestructor)(void * model);

			/**
			 * The skin specific settings and other skin data.
			 * The skin data is managed by the skin. If you want to implement a skin
			 * independent application (or game), do not use this object outside the skin.
			 * However, if you know which skin is in use and you require to read or write
			 * special property, you may cast this object to the respective type.
			 * This object may be NULL, if the skin does not need to store any data.
			 * @note If the object is not null when the component is destroyed,
			 * then the skin data is automatically destroyed.
			 */
			SkinData * skinData;

		public:

			/**
			 * Creates a new glaziery object.
			 */
			GlazieryObject();

			/**
			 * Destroys the glaziery object.
			 */
			virtual ~GlazieryObject();

			/**
			 * Destructor function for the setModel() method, when setting a model
			 * that is a glaziery object itself.
			 * @param model The model to be destroyed.
			 * @see setModel()
			 */
			static void GlazieryObject::destructor(void * model);

			/**
			 * Returns the application (or game) defined data. Typically, there is an object
			 * that contains application data that corresponds to this graphical element.
			 * This object (or raw data) is stored here. Glaziery will not perform
			 * any operation on it, but you can use it to identify application objects
			 * for given event targets (such as components).
			 * @return The application (or game) defined data.
			 */
			void * getModel();

			/**
			 * Returns the skin specific settings and other skin data.
			 * The skin data is managed by the skin. If you want to implement a skin
			 * independent application (or game), do not use this object outside the skin.
			 * However, if you know which skin is in use and you require to read or write
			 * special property, you may cast this object to the respective type.
			 * This object may be NULL, if the skin does not need to store any data.
			 * @return The skin specific settings and other skin data.
			 * @note If the object is not null when the component is destroyed,
			 * then the skin data is automatically destroyed.
			 */
			SkinData * getSkinData();

			/**
			 * Sets the application (or game) defined data. Typically, there is an object
			 * that contains application data that corresponds to this graphical element.
			 * This object (or raw data) is stored here. Glaziery will not perform
			 * any operation on it, but you can use it to identify application objects
			 * for given event targets (such as components).
			 * @param model The application (or game) defined data.
			 * @param modelDestructor An optional static method that is called, whenever the
			 * glaziery object is destroyed or when a new model is set.
			 * Specify this function only, if the model must be destroyed by this glaziery object,
			 * i.e. the object "owns" the model. If the model is a glaziery object itself,
			 * you may use the GlazieryObject::destructor() function.
			 * @see destructor()
			 */
			void setModel(void * model, void (*modelDestructor)(void * model) = NULL);

			/**
			 * Sets the skin specific settings and other skin data.
			 * The skin data is managed by the skin. If you want to implement a skin
			 * independent application (or game), do not use this object outside the skin.
			 * However, if you know which skin is in use and you require to read or write
			 * special property, you may cast this object to the respective type.
			 * This object may be NULL, if the skin does not need to store any data.
			 * @param skinData The skin specific settings and other skin data.
			 * @note If the object is not null when the component is destroyed,
			 * then the skin data is automatically destroyed.
			 * @note Any previously set skin data is automatically destroyed.
			 */
			virtual void setSkinData(SkinData * skinData);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
