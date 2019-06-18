/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_FIELDLAYOUTDATA_H
#define __GLAZIERY_FIELDLAYOUTDATA_H


#define GLAZIERY_FIELDLAYOUTDATA(field, horizontalAlignment, verticalAlignment, columnSpan) \
	{ \
		FieldLayoutData * layoutData = field->getFieldLayoutData(); \
		layoutData->setHorizontalAlignment(FieldLayoutData::ALIGN_##horizontalAlignment); \
		layoutData->setVerticalAlignment(FieldLayoutData::ALIGN_##verticalAlignment); \
		layoutData->setColumnSpan(columnSpan); \
		layoutData->setExcluded(false); \
	}

#define GLAZIERY_FIELDLAYOUTDATA_EXCLUDE(field, horizontalAlignment, verticalAlignment, columnSpan) \
	{ \
		FieldLayoutData * layoutData = field->getFieldLayoutData(); \
		layoutData->setExcluded(true); \
	}

#define GLAZIERY_FIELDLAYOUTDATA_MP(field, leftMargin, topMargin, rightMargin, bottomMargin, \
	leftPadding, topPadding, rightPadding, bottomPadding, \
	horizontalAlignment, verticalAlignment, columnSpan) \
	GLAZIERY_FIELDLAYOUTDATA(field, horizontalAlignment, verticalAlignment, columnSpan) \
	{ \
		FieldLayoutData * layoutData = field->getFieldLayoutData(); \
		layoutData->setTopLeftMargin(Vector((leftMargin), (topMargin))); \
		layoutData->setBottomRightMargin(Vector((rightMargin), (bottomMargin))); \
		layoutData->setTopLeftPadding(Vector((leftPadding), (topPadding))); \
		layoutData->setBottomRightPadding(Vector((rightPadding), (bottomPadding))); \
		layoutData->setExcluded(false); \
	}


namespace glaziery
{
	/**
	 * @author Thomas Jacob
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class FieldLayoutData : public CObject
	#else
		class FieldLayoutData
	#endif
	{
		friend Field;

		public:

			enum Alignment
			{
				/**
				 * <ul>
				 * <li>Move the field to the left resp. top within the grid cell.</li>
				 * <li>Do not resize the field.</li>
				 * <li>Minimum requirement for the column resp. row is the current size of the field.</li>
				 * </ul>
				 */
				ALIGN_START,

				/**
				 * <ul>
				 * <li>Move the field to the center within the grid cell.</li>
				 * <li>Do not resize the field.</li>
				 * <li>Minimum requirement for the column resp. row is the current size of the field.</li>
				 * </ul>
				 */
				ALIGN_CENTER,

				/**
				 * <ul>
				 * <li>Move the field to the right resp. bottom within the grid cell.</li>
				 * <li>Do not resize the field.</li>
				 * <li>Minimum requirement for the column resp. row is the current size of the field.</li>
				 * </ul>
				 */
				ALIGN_END,

				/**
				 * <ul>
				 * <li>Move the field to the left resp. top within the grid cell.</li>
				 * <li>Resize the field to fill the entire cell.</li>
				 * <li>Participate in using all available container's space
				 * by enlarging the column resp. row to the maximum.</li>
				 * </ul>
				 * @note Do not specify ALIGN_FILL for group fields with resizeToChildren turned on.
				 * The result will be unpredictable.
				 */
				ALIGN_FILL,
			};

		private:

			/**
			 * The bottom and right margin sizes in pixels.
			 * If multiple inner margin specifications apply (e.g. the right margin of one field
			 * and the left margin of the field next to it), the maximum margin specification is used,
			 * not the sum of both. Exception: The fields area's margin specification always overrides
			 * the outer margin specifications of outer fields.
			 * The margin can be used for spacing considering neighbors as well,
			 * while the padding defines "fixed" size spacing.
			 * @see bottomRightPadding
			 */
			Vector bottomRightMargin;

			/**
			 * The bottom and right padding sizes in pixels.
			 * The padding is the space between the margin and the field.
			 * It always reduces the space available for the field and is not related
			 * to the padding or margin of any adjacent field or the fields area.
			 * It can be used for "fixed" size spacing, while the margin defines spacing
			 * considering neighbors as well.
			 * @see bottomRightMargin
			 */
			Vector bottomRightPadding;

			/**
			 * The number of consecutive cells the field uses within its row.
			 * 0 specifies that is should use all cells up to the end of the row.
			 * Same holds if the span is too large, in this case it also uses at last the remaining cells.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 */
			int columnSpan;

			/**
			 * Whether this field is excluded from layout entirely.
			 */
			bool excluded;

			/**
			 * The field this layout data belongs to.
			 */
			Field * field;

			/**
			 * The horizontal alignment. This value specifies how the field should
			 * be positioned and resized within its cell.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 */
			Alignment horizontalAlignment;

			/**
			 * The top and left margin sizes in pixels.
			 * If multiple inner margin specifications apply (e.g. the right margin of one field
			 * and the left margin of the field next to it), the maximum margin specification is used,
			 * not the sum of both. Exception: The fields area's margin specification always overrides
			 * the outer margin specifications of outer fields.
			 * The margin can be used for spacing considering neighbors as well,
			 * while the padding defines "fixed" size spacing.
			 * @see topLeftPadding
			 */
			Vector topLeftMargin;

			/**
			 * The top and left padding sizes in pixels.
			 * The padding is the space between the margin and the field.
			 * It always reduces the space available for the field and is not related
			 * to the padding or margin of any adjacent field or the fields area.
			 * It can be used for "fixed" size spacing, while the margin defines spacing
			 * considering neighbors as well.
			 * @see topLeftMargin
			 */
			Vector topLeftPadding;

			/**
			 * The vertical alignment. This value specifies how the field should
			 * be positioned and resized within its cell.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 */
			Alignment verticalAlignment;

		protected:

			/**
			 * Relayouts the fields area, if automatic layouting is enabled.
			 */
			void layoutIfAutomatic();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(FieldLayoutData);
			#endif

		public:

			/**
			 * Creates a new field layout data.
			 */
			FieldLayoutData();

			/**
			 * Destroys the field layout data.
			 */
			virtual ~FieldLayoutData();

			/**
			 * Returns the bottom and right margin sizes in pixels.
			 * If multiple inner margin specifications apply (e.g. the right margin of one field
			 * and the left margin of the field next to it), the maximum margin specification is used,
			 * not the sum of both. Exception: The fields area's margin specification always overrides
			 * the outer margin specifications of outer fields.
			 * The margin can be used for spacing considering neighbors as well,
			 * while the padding defines "fixed" size spacing.
			 * @return The bottom and right margin sizes in pixels.
			 * @see getBottomRightPadding()
			 */
			Vector getBottomRightMargin();

			/**
			 * Returns the bottom and right padding sizes in pixels.
			 * The padding is the space between the margin and the field.
			 * It always reduces the space available for the field and is not related
			 * to the padding or margin of any adjacent field or the fields area.
			 * It can be used for "fixed" size spacing, while the margin defines spacing
			 * considering neighbors as well.
			 * @return The bottom and right padding sizes in pixels.
			 * @see getBottomRightMargin()
			 */
			Vector getBottomRightPadding();

			/**
			 * Returns the number of consecutive cells the field uses within its row.
			 * 0 specifies that is should use all cells up to the end of the row.
			 * Same holds if the span is too large, in this case it also uses at last the remaining cells.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 * @return The number of consecutive cells the field uses within its row.
			 */
			int getColumnSpan();

			/**
			 * Returns the field this layout data belongs to.
			 * @return The field this layout data belongs to.
			 */
			Field * getField();

			/**
			 * Returns the horizontal alignment. This value specifies how the field should
			 * be positioned and resized within its cell.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 * @note Do not specify ALIGN_FILL for group fields with resizeToChildren turned on.
			 * The result will be unpredictable.
			 * @return The horizontal alignment.
			 */
			Alignment getHorizontalAlignment();

			/**
			 * Returns the top and left margin sizes in pixels.
			 * If multiple inner margin specifications apply (e.g. the right margin of one field
			 * and the left margin of the field next to it), the maximum margin specification is used,
			 * not the sum of both. Exception: The fields area's margin specification always overrides
			 * the outer margin specifications of outer fields.
			 * The margin can be used for spacing considering neighbors as well,
			 * while the padding defines "fixed" size spacing.
			 * @return The top and left margin sizes in pixels.
			 * @see getTopLeftPadding()
			 */
			Vector getTopLeftMargin();

			/**
			 * Returns the top and left padding sizes in pixels.
			 * The padding is the space between the margin and the field.
			 * It always reduces the space available for the field and is not related
			 * to the padding or margin of any adjacent field or the fields area.
			 * It can be used for "fixed" size spacing, while the margin defines spacing
			 * considering neighbors as well.
			 * @return The top and left padding sizes in pixels.
			 * @see getTopLeftMargin()
			 */
			Vector getTopLeftPadding();

			/**
			 * Returns the vertical alignment. This value specifies how the field should
			 * be positioned and resized within its cell.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 * @note Do not specify ALIGN_FILL for group fields with resizeToChildren turned on.
			 * The result will be unpredictable.
			 * @return The vertical alignment.
			 */
			Alignment getVerticalAlignment();

			/**
			 * Returns whether this field is excluded from layout entirely.
			 * @return Whether this field is excluded from layout entirely.
			 */
			bool isExcluded();

			/**
			 * Sets the bottom and right margin sizes in pixels.
			 * If multiple inner margin specifications apply (e.g. the right margin of one field
			 * and the left margin of the field next to it), the maximum margin specification is used,
			 * not the sum of both. Exception: The fields area's margin specification always overrides
			 * the outer margin specifications of outer fields.
			 * The margin can be used for spacing considering neighbors as well,
			 * while the padding defines "fixed" size spacing.
			 * @param bottomRightMargin The bottom and right margin sizes in pixels.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 * @see setBottomRightPadding()
			 */
			void setBottomRightMargin(Vector bottomRightMargin);

			/**
			 * Sets the bottom and right padding sizes in pixels.
			 * The padding is the space between the margin and the field.
			 * It always reduces the space available for the field and is not related
			 * to the padding or margin of any adjacent field or the fields area.
			 * It can be used for "fixed" size spacing, while the margin defines spacing
			 * considering neighbors as well.
			 * @param bottomRightPadding The bottom and right padding sizes in pixels.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 * @see setBottomRightMargin()
			 */
			void setBottomRightPadding(Vector bottomRightPadding);

			/**
			 * Sets the number of consecutive cells the field uses within its row.
			 * 0 specifies that is should use all cells up to the end of the row.
			 * Same holds if the span is too large, in this case it also uses at last the remaining cells.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 * @param columnSpan The number of consecutive cells the field uses within its row.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 */
			void setColumnSpan(int columnSpan);

			/**
			 * Sets whether this field is excluded from layout entirely.
			 * @param excluded Whether this field is excluded from layout entirely.
			 */
			void setExcluded(bool excluded);

			/**
			 * Sets the horizontal alignment. This value specifies how the field should
			 * be positioned and resized within its cell.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 * @param horizontalAlignment The horizontal alignment.
			 * @note Do not specify ALIGN_FILL for group fields with resizeToChildren turned on.
			 * The result will be unpredictable.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 */
			void setHorizontalAlignment(Alignment horizontalAlignment);

			/**
			 * Sets the top and left margin sizes in pixels.
			 * If multiple inner margin specifications apply (e.g. the right margin of one field
			 * and the left margin of the field next to it), the maximum margin specification is used,
			 * not the sum of both. Exception: The fields area's margin specification always overrides
			 * the outer margin specifications of outer fields.
			 * The margin can be used for spacing considering neighbors as well,
			 * while the padding defines "fixed" size spacing.
			 * @param topLeftMargin The top and left margin sizes in pixels.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 * @see setTopLeftPadding()
			 */
			void setTopLeftMargin(Vector topLeftMargin);

			/**
			 * Sets the top and left padding sizes in pixels.
			 * The padding is the space between the margin and the field.
			 * It always reduces the space available for the field and is not related
			 * to the padding or margin of any adjacent field or the fields area.
			 * It can be used for "fixed" size spacing, while the margin defines spacing
			 * considering neighbors as well.
			 * @param topLeftPadding The top and left padding sizes in pixels.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 * @see setTopLeftMargin()
			 */
			void setTopLeftPadding(Vector topLeftPadding);

			/**
			 * Sets the vertical alignment. This value specifies how the field should
			 * be positioned and resized within its cell.
			 * The cells are built from the container layout data's column specification,
			 * and the spans of each field. If a row's span sum exceeds the columns specification,
			 * the next row starts.
			 * @param verticalAlignment The vertical alignment.
			 * @note Do not specify ALIGN_FILL for group fields with resizeToChildren turned on.
			 * The result will be unpredictable.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 */
			void setVerticalAlignment(Alignment verticalAlignment);
	};
}


#endif
