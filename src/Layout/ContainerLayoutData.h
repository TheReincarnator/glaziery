/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_CONTAINERLAYOUTDATA_H
#define __GLAZIERY_CONTAINERLAYOUTDATA_H


#define GLAZIERY_FIELDSAREALAYOUTDATA(area, columns) \
	{ \
		ContainerLayoutData * layoutData = area->getLayoutData(); \
		layoutData->setColumns(columns); \
	}

#define GLAZIERY_FIELDSAREALAYOUTDATA_M(area, \
	leftMargin, topMargin, rightMargin, bottomMargin, columns) \
	GLAZIERY_FIELDSAREALAYOUTDATA(area, columns) \
	{ \
		ContainerLayoutData * layoutData = area->getLayoutData(); \
		layoutData->setTopLeftMargin(Vector((leftMargin), (topMargin))); \
		layoutData->setBottomRightMargin(Vector((rightMargin), (bottomMargin))); \
	}

#define GLAZIERY_GROUPFIELDLAYOUTDATA(field, columns) \
	{ \
		ContainerLayoutData * layoutData = field->getGroupLayoutData(); \
		layoutData->setColumns(columns); \
	}

#define GLAZIERY_GROUPFIELDLAYOUTDATA_M(field, \
	leftMargin, topMargin, rightMargin, bottomMargin, columns) \
	GLAZIERY_GROUPFIELDLAYOUTDATA(field, columns) \
	{ \
		ContainerLayoutData * layoutData = field->getGroupLayoutData(); \
		layoutData->setTopLeftMargin(Vector((leftMargin), (topMargin))); \
		layoutData->setBottomRightMargin(Vector((rightMargin), (bottomMargin))); \
	}


namespace glaziery
{
	/**
	 * @author Thomas Jacob
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class ContainerLayoutData : public CObject
	#else
		class ContainerLayoutData
	#endif
	{
		friend FieldsArea;
		friend GroupField;

		private:

			/**
			 * The bottom and right margin sizes in pixels.
			 * This specification always overrides the outer margin specifications of outer fields.
			 */
			Vector bottomRightMargin;

			/**
			 * The number of columns for fields within this container (fields area or group field).
			 * The cells are built from this column specification, and the spans of each field.
			 * If a row's span sum exceeds the columns specification, the next row starts.
			 * If this field is 0, this container has only one row which collects all fields.
			 */
			int columns;

			/**
			 * The fields area or group field this layout data belongs to.
			 */
			Component * container;

			/**
			 * The top and left margin sizes in pixels.
			 * This specification always overrides the outer margin specifications of outer fields.
			 */
			Vector topLeftMargin;

		protected:

			/**
			 * Relayouts the fields area, if automatic layouting is enabled.
			 */
			void layoutIfAutomatic();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(ContainerLayoutData);
			#endif

		public:

			/**
			 * Creates a new container layout data.
			 */
			ContainerLayoutData();

			/**
			 * Destroys the container layout data.
			 */
			virtual ~ContainerLayoutData();

			/**
			 * Returns the bottom and right margin sizes in pixels.
			 * This specification always overrides the outer margin specifications of outer fields.
			 * @return The bottom and right margin sizes in pixels.
			 */
			Vector getBottomRightMargin();

			/**
			 * Returns the number of columns for fields within this container (fields area or group field).
			 * The cells are built from this column specification, and the spans of each field.
			 * If a row's span sum exceeds the columns specification, the next row starts.
			 * @return The number of columns for fields within this container or 0,
			 * if this container has only one row which collects all fields.
			 */
			int getColumns();

			/**
			 * Returns the top and left margin sizes in pixels.
			 * This specification always overrides the outer margin specifications of outer fields.
			 * @return The top and left margin sizes in pixels.
			 */
			Vector getTopLeftMargin();

			/**
			 * Returns the fields area or group field this layout data belongs to.
			 * @return The fields area or group field this layout data belongs to.
			 */
			Component * getContainer();

			/**
			 * Sets the bottom and right margin sizes in pixels.
			 * This specification always overrides the outer margin specifications of outer fields.
			 * @param bottomRightMargin The bottom and right margin sizes in pixels.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 */
			void setBottomRightMargin(Vector bottomRightMargin);

			/**
			 * Sets the number of columns for fields within this container (fields area or group field).
			 * The cells are built from this column specification, and the spans of each field.
			 * If a row's span sum exceeds the columns specification, the next row starts.
			 * @param columns The number of columns for fields within this container or 0,
			 * if this container has only one row which collects all fields.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 */
			void setColumns(int columns);

			/**
			 * Sets the top and left margin sizes in pixels.
			 * This specification always overrides the outer margin specifications of outer fields.
			 * @param topLeftMargin The top and left margin sizes in pixels.
			 * @note Calling this method relayouts the fields area, if automatic layouting is enabled,
			 * so consider turning off the automatic layout when doing major changes.
			 */
			void setTopLeftMargin(Vector topLeftMargin);
	};
}


#endif
