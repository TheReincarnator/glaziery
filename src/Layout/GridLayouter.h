/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_GRIDLAYOUTER_H
#define __GLAZIERY_GRIDLAYOUTER_H


namespace glaziery
{
	/**
	 * @author Thomas Jacob
	 */
	class GridLayouter : public Layouter
	{
		protected:

			int getMaximumSpacing(ArrayList<Int> * spacings, int columnNo, int columnSpan);

			bool isLayouting(Field * field);

			bool layoutFields(const ArrayList<Field> & fields, Component * container,
				Vector topLeftBorders, Vector bottomRightBorders, int columns, bool resizeContainer);

			bool layoutGroupsAndFields(const ArrayList<Field> & fields, Component * container,
				Vector topLeftBorders, Vector bottomRightBorders, int columns, bool resizeContainer);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(GridLayouter);
			#endif

		public:

			/**
			 * Creates a new grid layouter.
			 */
			GridLayouter();

			/**
			 * Destroys the grid layouter.
			 */
			virtual ~GridLayouter();

			/**
			 * Layouts all fields of a fields area.
			 * @param fieldArea The fields area to be layouted.
			 * @param resizeArea Whether to resize the fields area according to the total size of the fields.<br>
			 * If true, all ALIGN_FILL groups and fields are filled only to the maximum size of
			 * all groups resp. fields in the resp column or row. Then, the fields area is resized.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.<br>
			 * If false, all ALIGN_FILL groups and fields are filled to the maximum size of the parent
			 * fields area or group. The fields area is not resized in this case.
			 * Same holds for the groups in the fields area, containing groups and fields themselves.
			 * @return Whether any component was moved or resized.
			 */
			virtual bool layout(FieldsArea * fieldsArea, bool resizeArea);

			/**
			 * Layouts the fields of a group field and resizes the group field.
			 * @param groupField The group field to be layouted.
			 * @param resizeGroupField Whether to resize the group field according to the total size of the fields.<br>
			 * If true, all ALIGN_FILL groups and fields are filled only to the maximum size of
			 * all groups resp. fields in the resp column or row. Then, the group field is resized.
			 * Same holds for the groups in the group field, containing groups and fields themselves.<br>
			 * If false, all ALIGN_FILL groups and fields are filled to the maximum size of the parent
			 * fields area or group. The group field is not resized in this case.
			 * Same holds for the groups in the group field, containing groups and fields themselves.
			 * @return Whether any component was moved or resized.
			 */
			virtual bool layout(GroupField * groupField, bool resizeGroupField);
	};
}


#endif
