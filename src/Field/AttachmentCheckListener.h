/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_ATTACHMENTCHECKLISTENER_H
#define __GLAZIERY_ATTACHMENTCHECKLISTENER_H


namespace glaziery
{
	/**
	 * The attachment check listener is a special check box listener
	 * that toggles the active flag of a field according to the box'es state.
	 * @see Field::attachTo()
	 */
	class AttachmentCheckBoxListener : public CheckBox::Listener
	{
		friend ArrayList<AttachmentCheckBoxListener>;

		private:

			/**
			 * The check box the listener is attached to.
			 */
			CheckBox * checkBox;

			/**
			 * The field to create the listener for.
			 */
			Field * field;

		public:

			/**
			 * Creates a new state to activation listener.
			 * @param field The field to create the listener for.
			 * @param checkBox The check box the listener is attached to.
			 */
			AttachmentCheckBoxListener(Field * field, CheckBox * checkBox);

			/**
			 * Returns the check box the listener is attached to.
			 * @return The check box the listener is attached to.
			 */
			CheckBox * getCheckBox();

			/**
			 * Called when the component is about to be destroyed (its memory
			 * is released). This method is called before the component is deleted,
			 * but its parent container references may have been released already.
			 * @param component The component that received the event.
			 */
			virtual void onDestroying(Component * component);

			/**
			 * Called when the check box'es state has been changed,
			 * either using the user interface or using the setChecked()
			 * or setState() methods.
			 * @param checkBox The check box that has been changed.
			 * @see onStateChanging()
			 * @see CheckBox::setChecked()
			 * @see CheckBox::setState()
			 */
			virtual void onStateChanged(CheckBox * checkBox);
	};

	/**
	 * The attachment radio listener is a special radio box listener
	 * that toggles the active flag of a field according to the box'es state.
	 * @see Field::attachTo()
	 */
	class AttachmentRadioBoxListener : public RadioBox::Listener
	{
		friend ArrayList<AttachmentRadioBoxListener>;

		private:

			/**
			 * The radio box the listener is attached to.
			 */
			RadioBox * radioBox;

			/**
			 * The field to create the listener for.
			 */
			Field * field;

		public:

			/**
			 * Creates a new state to activation listener.
			 * @param field The field to create the listener for.
			 * @param radioBox The radio box the listener is attached to.
			 */
			AttachmentRadioBoxListener(Field * field, RadioBox * radioBox);

			/**
			 * Returns the radio box the listener is attached to.
			 * @return The radio box the listener is attached to.
			 */
			RadioBox * getRadioBox();

			/**
			 * Called when the component is about to be destroyed (its memory
			 * is released). This method is called before the component is deleted,
			 * but its parent container references may have been released already.
			 * @param component The component that received the event.
			 */
			virtual void onDestroying(Component * component);

			/**
			 * Called when the radio box'es state has been changed,
			 * either using the user interface or using the setChecked()
			 * or setState() methods.
			 * @param radioBox The radio box that has been changed.
			 * @see onStateChanging()
			 * @see RadioBox::setChecked()
			 * @see RadioBox::setState()
			 */
			virtual void onStateChanged(RadioBox * radioBox);
	};
}


#endif
