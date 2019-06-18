/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_LAYOUTER_H
#define __GLAZIERY_LAYOUTER_H


namespace glaziery
{
	/**
	 * @author Thomas Jacob
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class Layouter : public CObject
	#else
		class Layouter
	#endif
	{
		private:

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(Layouter);
			#endif

		public:

			/**
			 * Creates a new layouter.
			 */
			Layouter();

			/**
			 * Destroys the layouter.
			 */
			virtual ~Layouter();

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
			virtual bool layout(FieldsArea * fieldsArea, bool resizeArea) = NULL;

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
			virtual bool layout(GroupField * groupField, bool resizeGroupField) = NULL;
	};
}


#endif
