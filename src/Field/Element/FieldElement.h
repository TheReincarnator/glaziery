/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_FIELDELEMENT_H
#define __GLAZIERY_FIELDELEMENT_H


namespace glaziery
{
	/**
	 * Field elements are parts of structural fields like lists or trees.
	 * This super class abstracts from the type of element.
	 * Concrete elements include text and image elements.
	 */
	class FieldElement : public Listenable
	{
		friend ArrayList<FieldElement>;

		public:

			/**
			 * The vertical alignment of the element within the list or tree field cell.
			 */
			enum Alignment
			{
				/**
				 * Center the element to the top.
				 */
				ALIGN_TOP,

				/**
				 * Center the element to the center.
				 */
				ALIGN_CENTER,

				/**
				 * Center the element to the bottom.
				 */
				ALIGN_BOTTOM
			};

		private:

			/**
			 * The vertical alignment of the element within the list or tree field cell.
			 * Defaults to ALIGN_CENTER.
			 */
			Alignment alignment;

			/**
			 * Whether the user can edit this field element.
			 */
			bool editable;

			/**
			 * The field this element belongs to.
			 */
			Field * field;

			/**
			 * The parent of this element. E.g. for list fields, this is the cell,
			 * and for tree fields, this is the item.
			 */
			void * parent;

		protected:

			/**
			 * Creates a new field element.
			 */
			FieldElement();

			/**
			 * Destroys the field element.
			 */
			virtual ~FieldElement();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(FieldElement);
			#endif

		public:

			/**
			 * Draws the content of the field element to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation. This context is already translated
			 * for the current field element position and available size
			 * (e.g. list field column).
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context) = NULL;

			/**
			 * Draws a number of field elements, respecting their vertical alignments.
			 * This is a helper you can use in your field implementations.
			 * @param context The current drawing context. Drawing starts at the top-left corner of this context.
			 * @param elements The field elements.
			 * @param height The available height of the field element container. Used for vertical alignment.
			 * @param elementSpaceWidth The space between two field elements.
			 * @param textElements Whether to draw text elements, false skips them, but leaves a space.
			 * @param nonTextElements Whether to draw non-text elements, false skips them, but leaves a space.
			 */
			static void drawElements(DrawingContext & context, const ArrayList<FieldElement> & elements,
				int height, int elementSpaceWidth = 0, bool textElements = true, bool nonTextElements = true);

			/**
			 * Opens a popup to edit the field element.
			 * This method is typically called by the field (e.g. list or tree field)
			 * when receiving the onEdit() event.
			 * @param position The absolute position of the top-left corner
			 * of the inner edit field of the popup.
			 * @param size The size of the inner edit field of the popup.
			 * @param scrollingField The scrolling field the popup is created for,
			 * or NULL, if the popup is not created for any scrolling field.
			 * Setting a scrolling field enables the popup to move if the scrolling field is scrolled.
			 * Defaults to NULL.
			 */
			virtual void edit(Vector position, Vector size, ScrollingField * scrollingField) = NULL;

			/**
			 * Returns the vertical alignment of the element within the list or tree field cell.
			 * Defaults to ALIGN_CENTER.
			 * @return The vertical alignment.
			 */
			Alignment getAlignment(void);

			/**
			 * Returns the field this element belongs to.
			 * @return The field this element belongs to.
			 */
			Field * getField();

			/**
			 * Returns the parent of this element. E.g. for list fields, this is the cell,
			 * and for tree fields, this is the item.
			 * @return The parent of this element.
			 */
			void * getParent();

			/**
			 * Returns the size of the field element.
			 * @return The size of the field element.
			 */
			virtual Vector getSize() = NULL;

			/**
			 * Returns whether the user can edit this field element.
			 * @return Whether the user can edit this field element.
			 */
			bool isEditable();

			/**
			 * Sets the vertical alignment of the element within the list or tree field cell.
			 * Defaults to ALIGN_CENTER.
			 * @param alignment The vertical alignment.
			 */
			void setAlignment(Alignment alignment);

			/**
			 * Sets whether the user can edit this field element.
			 * @param editable Whether the user can edit this field element.
			 */
			void setEditable(bool editable);

			/**
			 * Informs the field element about the field that contains it.
			 * Also informs about ts parent. E.g. for list fields, this is the cell,
			 * and for tree fields, this is the item.
			 * This method should not be called by client, but by the field
			 * providing adder methods for field elements.
			 * @note This method may be called only once.
			 * @param field The field that adds the field element.
			 * @param parent The parent of the field element.
			 */
			void setFieldAndParent(Field * field, void * parent);
	};
}


#endif
