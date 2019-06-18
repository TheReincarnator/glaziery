/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_FILEDIALOG_H
#define __GLAZIERY_FILEDIALOG_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class FileDialog : public Dialog
	{
		protected:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				class DirectoryTask : public CObject
			#else
				class DirectoryTask
			#endif
			{
				public:

					Directory directory;

					TreeField::Item * expandItem;

					Path * path;

					Thread * thread;

					DirectoryTask(Path * path, TreeField::Item * expandItem);

					~DirectoryTask();

					bool isFinished();

					void run();

					static int runInternal(Thread & thread, void * parameter);
			};

			class FileTypeListener : public Selector::Listener
			{
				public:

					virtual void onSelectionChanged(Selector * selector, int oldSelectionNo);
			};

			class ListFieldListener : public ListField::Listener
			{
				public:

					virtual void onCellDoubleClicked(ListField * field,
						int rowNo, int columnNo, int elementNo);

					virtual void onSelectionChanged(ListField * field);
			};

			class TreeFieldListener : public TreeField::Listener
			{
				public:

					virtual void onItemExpanded(TreeField::Item * item);

					virtual VetoMode onItemExpanding(TreeField::Item * item);

					virtual void onSelectionChanged(TreeField * field);

					virtual VetoMode onSelectionChanging(TreeField * field);
			};

			class TreeFieldElementListener : public TextElement::Listener
			{
				public:

					VetoMode onTextChanging(TextElement * element, const String & newText);
			};

		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				class FileType : public CObject
			#else
				class FileType
			#endif
			{
				friend FileDialog;

				private:

					String name;

					ArrayList<String> patterns;

					FileType(const String & name);

				public:

					void addPattern(const String & pattern);

					const String & getName();

					const ArrayList<String> & getPatterns();
			};

		protected:

			bool directoryOnly;

			TreeField * directoryField;

			DirectoryTask * directoryTask;

			ListField * fileListField;

			TextField * fileNameField;

			Selector * fileTypeField;

			ArrayList<FileType> fileTypes;

			String homePath;

			Path * homePathToBeSelected;

			String rootPath;

			bool saveDialog;

			TreeFieldElementListener * treeElementListener;

			/**
			 * Creates a new message window.
			 */
			FileDialog();

			/**
			 * Destroys the message dialog.
			 */
			virtual ~FileDialog();

			ListField::Row * createListRow(Path * path);

			TreeField::Item * createTreeItem(Path * path, TreeField::Item * parentItem);

			static void destroyPathModel(void * model);

			Image * getPathImage(Path * path);

			String getPathName(Path * path);

			/**
			 * Performs operations when the dialog has been dismissed.
			 * @param The button ID that has been pressed or NO_BUTTON,
			 * if no button has been pressed when showing the message dialog.
			 * This means the window has been closed or the Desktop shuts down.
			 * @return The veto mode, <code>VETO</code> or <code>NO_VETO</code>.
			 * Vetoing this method will not close the dialog.
			 * @see VetoMode
			 * @note Implement this method in derivatives, if necessary.
			 */
			virtual VetoMode onDialogEnd(Buttons button);

			/**
			 * Performs final operations before the dialog start, e.g. fills or adjusts fields
			 * according to dialog property set after setupContent().
			 */
			virtual void onDialogStart();

			/**
			 * Performs operations for each frame while the window is running modally.
			 */
			virtual void onModalUpdate();

			static void onNewDirectory(Button * button);

			/**
			 * Initializes the content group:
			 * @param area The fields area of this dialog.
			 * @param contentGroup The content group of this dialog.
			 */
			virtual void setupContent(FieldsArea * area, GroupField * contentGroup);

			void updateFileList();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(FileDialog);
			#endif

		public:

			FileType * addFileType(const String & name);

			/**
			 * @note The dialog is automatically added to the desktop.
			 * You should neither add it manually, nor delete it.
			 */
			static FileDialog * createDirectoryDialog();

			/**
			 * @warn This method allocates memory and expects that you delete it after use.
			 */
			Path * createDirectoryPath();

			/**
			 * @warn This method allocates memory and expects that you delete it after use.
			 */
			Path * createFilePath();

			/**
			 * @note The dialog is automatically added to the desktop.
			 * You should neither add it manually, nor delete it.
			 */
			static FileDialog * createLoadFileDialog();

			/**
			 * @note The dialog is automatically added to the desktop.
			 * You should neither add it manually, nor delete it.
			 */
			static FileDialog * createSaveFileDialog();

			String getDirectoryPathString();

			String getFileName();

			String getFilePathString();

			FileType * getFileType();

			const ArrayList<FileType> & getFileTypes();

			const String & getHomePath();

			const String & getRootPath();

			bool isDirectoryOnly();

			void setDirectoryOnly(bool directoryOnly);

			void setFileName(const String & fileName);

			void setHomePath(const String & homePath);

			void setHomePath(const Path & homePath);

			void setHomePath(const Path * homePath);

			void setRootPath(const String & rootPath);

			void setRootPath(const Path & rootPath);

			void setRootPath(const Path * rootPath);
	};
}


#endif
