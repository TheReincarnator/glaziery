#ifndef __MAINDIALOG_H
#define __MAINDIALOG_H


/**
 * The main dialog.
 * @author Thomas Jacob
 */
class MainDialog : public SizableDialog
{
	private:

		/**
		 * Whether the OnInitDialog method has completed.
		 */
		bool Initialized;

		void CreateFont(Font & font, REAL * brushPositions, Color * brushColors,
			int brushColorsCount, float spaceWidthHeightRatio, WCHAR * pathNameWString,
			bool usAsciiOnly = false);

		void OnFontBrowse();
		void OnGradientsAdd();
		void OnGradientsRemove();
		void OnPreviewRefresh();
		void OnSaveFont();

		bool GetEncoderClsid(const WCHAR * format, CLSID * classId);

	// Declaration macros require their own protected section.
	protected:

		/**
		 * Message map declaration.
		 */
		DECLARE_MESSAGE_MAP()

	protected:

		/**
		 * Performs a dialog data exchange.
		 * @param DDXContext Dialog data exchange context information.
		 */
		virtual void DoDataExchange(CDataExchange * DDXContext);

		/**
		 * Initializes the dialog after all controls have been placed,
		 * but before the dialog is displayed.
		 * @return 0 if the method has set the focus on a control,
		 * 1 if the framework should do that.
		 */
		virtual int OnInitDialog();

		/**
		 * Message handler for the position/size event. This method is
		 * called by the framework when the window position and/or size
		 * have changed.
		 * @param windowPosition Information about the position and size.
		 */
		virtual void OnWindowPosChanged(WINDOWPOS * windowPosition);

	public:

		/**
		 * Creates a new main dialog.
		 */
		MainDialog();
};


#endif
