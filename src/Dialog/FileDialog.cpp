/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


FileDialog::FileDialog()
{
	ASSERTION_COBJECT(this);

	directoryOnly = false;
	directoryField = NULL;
	directoryTask = NULL;
	fileListField = NULL;
	fileNameField = NULL;
	fileTypeField = NULL;
	homePathToBeSelected = NULL;
	saveDialog = false;

	if ((treeElementListener = new TreeFieldElementListener) == NULL)
		throw EOUTOFMEMORY;
	treeElementListener->addReference();

	Path * rootPath = Path::CreateRootPath();
	setRootPath(rootPath);
	delete rootPath;

	Path * homePath = Path::CreateHomeFolderPath();
	setHomePath(homePath);
	delete homePath;
}

FileDialog::~FileDialog()
{
	ASSERTION_COBJECT(this);
	ASSERTION(directoryTask == NULL);

	if (treeElementListener != NULL) {treeElementListener->release(); treeElementListener = NULL;}
}

FileDialog::FileType * FileDialog::addFileType(const String & name)
{
	ASSERTION_COBJECT(this);

	if (isInitialized())
		throw EILLEGALSTATE("The dialog is already initialized");

	FileType * type;
	if ((type = new FileType(name)) == NULL)
		throw EOUTOFMEMORY;
	fileTypes.Append(type);

	return type;
}

FileDialog * FileDialog::createDirectoryDialog()
{
	FileDialog * dialog;
	if ((dialog = new FileDialog) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addWindow(dialog);

	dialog->setDirectoryOnly(true);
	dialog->setTitle(STRING_FILEDIALOG_TITLE_DIRECTORY);
	dialog->setButtons(BUTTONS_OKCANCEL);

	return dialog;
}

Path * FileDialog::createDirectoryPath()
{
	ASSERTION_COBJECT(this);

	TreeField::Item * item = directoryField->getSelectedItem();
	if (item == NULL)
		return NULL;

	Path * path = (Path *) item->getModel();
	return path->HeapClone();
}

Path * FileDialog::createFilePath()
{
	ASSERTION_COBJECT(this);

	if (fileNameField == NULL)
		return NULL;

	Path * directoryPath = createDirectoryPath();
	if (directoryPath == NULL)
		return NULL;

	Path * filePath = directoryPath->CreateTargetPath(fileNameField->getText());
	delete directoryPath;

	return filePath;
}

ListField::Row * FileDialog::createListRow(Path * path)
{
	ASSERTION_COBJECT(this);

	if (directoryOnly)
	{
		delete path;
		return NULL;
	}

	GLAZIERY_LISTFIELD_ROW(row, fileListField)
	row->setModel(path, &destroyPathModel);

	GLAZIERY_LISTFIELD_CELL(cell, row)
	GLAZIERY_LISTFIELD_IMAGEELEMENT(imageElement, cell, getPathImage(path))
	GLAZIERY_LISTFIELD_TEXTELEMENT(textElement, cell, getPathName(path))

	return row;
}

FileDialog * FileDialog::createLoadFileDialog()
{
	FileDialog * dialog;
	if ((dialog = new FileDialog) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addWindow(dialog);

	dialog->setTitle(STRING_FILEDIALOG_TITLE_LOAD);
	dialog->setButtons(BUTTONS_OKCANCEL);

	return dialog;
}

FileDialog * FileDialog::createSaveFileDialog()
{
	FileDialog * dialog;
	if ((dialog = new FileDialog) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addWindow(dialog);

	dialog->saveDialog = true;
	dialog->setTitle(STRING_FILEDIALOG_TITLE_SAVE);
	dialog->setButtons(BUTTONS_OKCANCEL);

	return dialog;
}

TreeField::Item * FileDialog::createTreeItem(Path * path, TreeField::Item * parentItem)
{
	ASSERTION_COBJECT(this);

	GLAZIERY_TREEFIELD_ITEM(item, directoryField, parentItem)
	item->setModel(path, &destroyPathModel);

	GLAZIERY_TREEFIELD_IMAGEELEMENT(imageElement, item, getPathImage(path))
	GLAZIERY_TREEFIELD_TEXTELEMENT(textElement, item, getPathName(path))
	textElement->setEditable(path->GetType() == Path::TYPE_FOLDER);
	textElement->appendListener(treeElementListener);

	GLAZIERY_TREEFIELD_ITEM(dummyChildItem, directoryField, item)
	GLAZIERY_TREEFIELD_TEXTELEMENT(dummyTextElement, dummyChildItem, STRING_FILEDIALOG_WAITITEM)
	item->collapse();

	return item;
}

void FileDialog::destroyPathModel(void * model)
{
	delete (Path *) model;
}

String FileDialog::getDirectoryPathString()
{
	ASSERTION_COBJECT(this);

	Path * directoryPath = createDirectoryPath();
	if (directoryPath == NULL)
		return String();

	char * directoryString = directoryPath->ToString();
	delete directoryPath;

	String directoryName(directoryString);
	delete directoryString;

	return directoryName;
}

String FileDialog::getFileName()
{
	ASSERTION_COBJECT(this);

	if (fileNameField == NULL)
		return String();

	return fileNameField->getText();
}

String FileDialog::getFilePathString()
{
	ASSERTION_COBJECT(this);

	Path * filePath = createFilePath();
	if (filePath == NULL)
		return String();

	char * fileString = filePath->ToString();
	delete filePath;

	String fileName(fileString);
	delete fileString;

	return fileName;
}

FileDialog::FileType * FileDialog::getFileType()
{
	ASSERTION_COBJECT(this);

	if (fileTypeField == NULL)
		return NULL;

	Selector::Option * option = fileTypeField->getSelection();
	if (option == NULL)
		return NULL;

	return (FileType *) option->getModel();
}

const ArrayList<FileDialog::FileType> & FileDialog::getFileTypes()
{
	ASSERTION_COBJECT(this);
	return fileTypes;
}

const String & FileDialog::getHomePath()
{
	ASSERTION_COBJECT(this);
	return homePath;
}

Image * FileDialog::getPathImage(Path * path)
{
	ASSERTION_COBJECT(this);
	return Desktop::getInstance()->getSkin()->getPathTypeImage(path->GetType());
}

String FileDialog::getPathName(Path * path)
{
	ASSERTION_COBJECT(this);

	if (path->IsElementsEmpty())
	{
		String fileSystem = path->GetFileSystem();

		#ifdef WIN32
			if (path->IsRoot())
				return String(STRING_FILEDIALOG_DESKTOP);
			else if (fileSystem == DU_WIN32GUID_MYDOCUMENTS)
				return String(STRING_FILEDIALOG_MYDOCUMENTS);
			else if (fileSystem == DU_WIN32GUID_MYCOMPUTER)
				return String(STRING_FILEDIALOG_MYCOMPUTER);
			else if (fileSystem == DU_WIN32GUID_NETWORKHOOD)
				return String(STRING_FILEDIALOG_NETWORKHOOD);
			else if (fileSystem.StartsWith("\\\\"))
			{
				int slashPos = fileSystem.Find("\\", 2);
				if (slashPos >= 0)
					return fileSystem.Substring(slashPos + 1);
				else
					return fileSystem.Substring(2);
			}
		#endif

		return String(fileSystem);
	}
	else
		return *path->GetLastElement();
}

const String & FileDialog::getRootPath()
{
	ASSERTION_COBJECT(this);
	return rootPath;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(FileDialog, Dialog);
#endif

bool FileDialog::isDirectoryOnly()
{
	ASSERTION_COBJECT(this);
	return directoryOnly;
}

VetoMode FileDialog::onDialogEnd(Buttons button)
{
	ASSERTION_COBJECT(this);

	VetoMode result = Dialog::onDialogEnd(button);

	if (directoryTask != NULL || button != Dialog::BUTTON_OK && button != Dialog::BUTTON_CANCEL)
		return VETO;

	if (fileListField != NULL && button == Dialog::BUTTON_OK)
	{
		ListField::Row * row = fileListField->getSelectedRow();
		if (row != NULL)
		{
			Path * path = (Path *) row->getModel();
			DirectoryEntry pathEntry(path);
			if ((pathEntry.GetFlags() & DU_FDIRECTORY) != 0)
			{
				TreeField::Item * item = directoryField->getSelectedItem();
				if (item != NULL)
				{
					const ArrayList<TreeField::Item> & children = item->getChildren();
					if (!item->isExpanded())
						item->expand();
					while (directoryTask != NULL)
					{
						Desktop::getInstance()->runOnce(20);
						onModalUpdate();
					}

					int childNo;
					for (childNo=0; childNo<children.GetCount(); childNo++)
					{
						Path * childPath = (Path *) children.Get(childNo)->getModel();
						if (childPath->Equals(path))
						{
							directoryField->setSelectedItem(children.Get(childNo));
							return VETO;
						}
					}
				}
			}
		}
	}

	if (fileNameField != NULL && button == Dialog::BUTTON_OK)
	{
		if (fileNameField->getText().IsEmpty())
		{
			MessageDialog::showDialog(STRING_FILEDIALOG_NOFILENAME_TITLE, STRING_FILEDIALOG_NOFILENAME_MESSAGE,
				MessageDialog::ICON_ERROR, MessageDialog::BUTTON_OK);
			return VETO;
		}

		FileType * selectedType = fileTypeField != NULL && fileTypeField->getSelection() != NULL
			? (FileType *) fileTypeField->getSelection()->getModel() : NULL;
		if (selectedType != NULL && !selectedType->getPatterns().IsEmpty())
		{
			String pattern = *selectedType->getPatterns().GetFirst();
			if (pattern.StartsWith('*'))
			{
				pattern.CutStart(1);
				if (!fileNameField->getText().EndsWith(pattern))
					fileNameField->setText(fileNameField->getText() + pattern);
			}
		}

		Path * filePath = createFilePath();
		bool exists = filePath->Exists();
		delete filePath;

		if (saveDialog && exists)
		{
			if (MessageDialog::showDialog(STRING_FILEDIALOG_SAVEEXISTS_TITLE, STRING_FILEDIALOG_SAVEEXISTS_MESSAGE,
				MessageDialog::ICON_WARNING, MessageDialog::BUTTONS_YESNO) != Dialog::BUTTON_YES)
				return VETO;
		}
		else if (!saveDialog && !exists)
		{
			MessageDialog::showDialog(STRING_FILEDIALOG_FILENOTFOUND_TITLE, STRING_FILEDIALOG_FILENOTFOUND_MESSAGE,
				MessageDialog::ICON_ERROR, MessageDialog::BUTTON_OK);
			return VETO;
		}
	}

	return result;
}

void FileDialog::onDialogStart()
{
	ASSERTION_COBJECT(this);

	Dialog::onDialogStart();

	Path * rootPath;
	if ((rootPath = new Path(this->rootPath)) == NULL)
		throw EOUTOFMEMORY;

	if (rootPath->IsRelative())
	{
		Path * relativePath = Path::CreateHomeFolderPath();
		rootPath->AppendPath(relativePath);
		delete relativePath;
	}

	if ((homePathToBeSelected = new Path(this->homePath)) == NULL)
		throw EOUTOFMEMORY;

	if (homePathToBeSelected->IsRelative())
	{
		Path * relativePath = homePathToBeSelected;
		homePathToBeSelected = Path::CreateHomeFolderPath();
		rootPath->AppendPath(relativePath);
		delete relativePath;
	}

	if (!rootPath->IsAncestorOf(homePathToBeSelected))
	{
		#ifdef _DEBUG
			throw EILLEGALSTATE("The home path is not within the root path's sub directories");
		#else
			LOGPRINTF2(LOG_ERROR, "The home path '%s' is not within the root path's sub directories ('%s')",
				homePathToBeSelected, rootPath);
		#endif

		delete homePathToBeSelected;
		if ((homePathToBeSelected = new Path(rootPath)) == NULL)
			throw EOUTOFMEMORY;
	}

	// Temporarily release homePathToBeSelected attribute to enable expansion
	Path * homePathToBeSelected = this->homePathToBeSelected;
	this->homePathToBeSelected = NULL;

	directoryField->removeRoots();
	TreeField::Item * selectedItem = createTreeItem(rootPath, NULL);
	directoryField->setSelectedItem(selectedItem);

	this->homePathToBeSelected = homePathToBeSelected;
}

void FileDialog::onModalUpdate()
{
	ASSERTION_COBJECT(this);

	Dialog::onModalUpdate();

	if (homePathToBeSelected != NULL && directoryTask == NULL)
	{
		TreeField::Item * item = directoryField->getSelectedItem();
		if (item != NULL && !((Path *) item->getModel())->Equals(homePathToBeSelected))
		{
			// Temporarily release homePathToBeSelected attribute to enable expansion/selection
			Path * homePathToBeSelected = this->homePathToBeSelected;
			this->homePathToBeSelected = NULL;

			const ArrayList<TreeField::Item> & children = item->getChildren();
			if (!item->isExpanded())
				item->expand();
			else
			{
				int childNo;
				for (childNo=0; childNo<children.GetCount(); childNo++)
				{
					Path * childPath = (Path *) children.Get(childNo)->getModel();
					if (childPath->IsAncestorOf(homePathToBeSelected))
					{
						directoryField->setSelectedItem(children.Get(childNo));
						break;
					}
				}

				if (childNo >= children.GetCount())
				{
					delete homePathToBeSelected;
					homePathToBeSelected = NULL;
				}
			}

			this->homePathToBeSelected = homePathToBeSelected;
		}
		else
		{
			delete homePathToBeSelected;
			homePathToBeSelected = NULL;
		}
	}

	if (directoryTask != NULL && directoryTask->isFinished())
	{
		if (directoryTask->expandItem != NULL)
		{
			directoryTask->expandItem->removeChildren();

			Directory & directory = directoryTask->directory;
			int entryCount = directory.GetEntryCount();
			for (int i=0; i<entryCount; i++)
			{
				Path * subPath = directory.GetEntry(i)->CreatePathObject();
				createTreeItem(subPath, directoryTask->expandItem);
			}
		}
		else
		{
			Directory & directory = directoryTask->directory;
			int entryCount = directory.GetEntryCount();

			// Add directories, first
			for (int entryNo=0; entryNo<entryCount; entryNo++)
			{
				DirectoryEntry * entry = directory.GetEntry(entryNo);
				if (entry->GetFlags() & DU_FDIRECTORY)
					createListRow(entry->CreatePathObject());
			}

			// Then, add files
			FileType * selectedType = fileTypeField != NULL && fileTypeField->getSelection() != NULL
				? (FileType *) fileTypeField->getSelection()->getModel() : NULL;
			const ArrayList<String> * patterns = selectedType != NULL ? &selectedType->getPatterns() : NULL;
			for (int entryNo=0; entryNo<entryCount; entryNo++)
			{
				DirectoryEntry * entry = directory.GetEntry(entryNo);
				if (entry->GetFlags() & DU_FDIRECTORY)
					continue;

				if (patterns != NULL && !patterns->IsEmpty())
				{
					String entryName = entry->GetPath();
					int lastSeparatorPos = entryName.FindLast(TB_PATH_SEPARATOR);
					if (lastSeparatorPos >= 0)
						entryName.CutStart(lastSeparatorPos);

					bool patternMatched = false;
					for (int i=0; i<patterns->GetCount(); i++)
						if (Directory::MatchesWildcard(entryName, *patterns->Get(i)))
						{
							patternMatched = true;
							break;
						}

					if (!patternMatched)
						continue;
				}

				createListRow(entry->CreatePathObject());
			}
		}

		Desktop::getInstance()->hideHourGlass();

		delete directoryTask;
		directoryTask = NULL;
	}
}

void FileDialog::onNewDirectory(Button * button)
{
	FileDialog * dialog = (FileDialog *) button->getWindow();

	TreeField::Item * item = dialog->directoryField->getSelectedItem();
	if (item == NULL)
		return;

	Path * path = (Path *) item->getModel();
	if (path == NULL)
		return;

	if (!item->isExpanded())
		item->expand();
	while (dialog->directoryTask != NULL)
	{
		Desktop::getInstance()->runOnce(20);
		dialog->onModalUpdate();
	}

	Path * pathClone;
	if ((pathClone = new Path(path)) == NULL)
		throw EOUTOFMEMORY;
	String directoryName(STRING_FILEDIALOG_NEWDIRECTORYNAME1);
	pathClone->AppendElement((const char *) directoryName);

	int newDirectoryNo = 1;
	while (pathClone->Exists())
	{
		pathClone->DeleteLastElement();
		directoryName.Format(STRING_FILEDIALOG_NEWDIRECTORYNAME2, ++newDirectoryNo);
		pathClone->AppendElement((const char *) directoryName);
	}

	if (!pathClone->MakeDirectory())
	{
		delete pathClone;
		return;
	}

	dialog->directoryField->setFocus();

	TreeField::Item * newItem = dialog->createTreeItem(pathClone, item);
	dialog->directoryField->setFocusItem(newItem);
	dialog->directoryField->setSelectedItem(newItem);
	dialog->directoryField->ensureFocusVisible();
	newItem->edit();
}

void FileDialog::setDirectoryOnly(bool directoryOnly)
{
	ASSERTION_COBJECT(this);

	if (isInitialized())
		throw EILLEGALSTATE("The dialog is already initialized");

	this->directoryOnly = directoryOnly;
}

void FileDialog::setFileName(const String & fileName)
{
	ASSERTION_COBJECT(this);

	setup();

	if (fileNameField == NULL)
		throw EILLEGALSTATE("The dialog has no file name field");

	fileNameField->setText(fileName);
}

void FileDialog::setHomePath(const String & homePath)
{
	ASSERTION_COBJECT(this);

	if (isInitialized())
		throw EILLEGALSTATE("The dialog is already initialized");

	this->homePath = homePath;
}

void FileDialog::setHomePath(const Path & homePath)
{
	ASSERTION_COBJECT(this);

	char * pathString = homePath.ToString(false);
	setHomePath(String(pathString));
	delete pathString;
}

void FileDialog::setHomePath(const Path * homePath)
{
	ASSERTION_COBJECT(this);

	char * pathString = homePath->ToString(false);
	setHomePath(String(pathString));
	delete pathString;
}

void FileDialog::setRootPath(const String & rootPath)
{
	ASSERTION_COBJECT(this);

	if (isInitialized())
		throw EILLEGALSTATE("The dialog is already initialized");

	this->rootPath = rootPath;
}

void FileDialog::setRootPath(const Path & rootPath)
{
	ASSERTION_COBJECT(this);

	char * pathString = rootPath.ToString(false);
	setRootPath(String(pathString));
	delete pathString;
}

void FileDialog::setRootPath(const Path * rootPath)
{
	ASSERTION_COBJECT(this);

	char * pathString = rootPath->ToString(false);
	setRootPath(String(pathString));
	delete pathString;
}

void FileDialog::setupContent(FieldsArea * area, GroupField * contentGroup)
{
	ASSERTION_COBJECT(this);

	GLAZIERY_GROUPFIELDLAYOUTDATA(contentGroup, 1)

	GLAZIERY_GROUPFIELD(choosersGroup, area, contentGroup)
	GLAZIERY_GROUPFIELDLAYOUTDATA(choosersGroup, directoryOnly ? 1 : 2)
	GLAZIERY_FIELDLAYOUTDATA(choosersGroup, FILL, FILL, 1)

	GLAZIERY_GROUPFIELD(directoryGroup, area, choosersGroup)
	GLAZIERY_GROUPFIELDLAYOUTDATA(directoryGroup, 0)
	GLAZIERY_FIELDLAYOUTDATA(directoryGroup, FILL, START, 1)
	GLAZIERY_LABEL(directoryLabel, area, directoryGroup,
		saveDialog ? STRING_FILEDIALOG_DIRECTORY_SAVE : STRING_FILEDIALOG_DIRECTORY)
	GLAZIERY_FIELDLAYOUTDATA(directoryLabel, FILL, START, 1)
	GLAZIERY_BUTTON(newDirectoryButton, area, directoryGroup, STRING_FILEDIALOG_DIRECTORY_NEW, NULL)
	GLAZIERY_FIELDLAYOUTDATA(newDirectoryButton, END, START, 1)
	newDirectoryButton->addPressedFunction(onNewDirectory);

	if (!directoryOnly)
	{
		GLAZIERY_LABEL(fileListLabel, area, choosersGroup, STRING_FILEDIALOG_FILELIST)
	}

	GLAZIERY_TREEFIELD_M(directoryField, area, choosersGroup, directoryOnly ? 600 : 400, 400, true, true, ONE)
	this->directoryField = directoryField;
	GLAZIERY_FIELDLAYOUTDATA(directoryField, FILL, FILL, 1)

	TreeFieldListener * treeFieldListener;
	if ((treeFieldListener = new TreeFieldListener) == NULL)
		throw EOUTOFMEMORY;
	directoryField->appendListener(treeFieldListener);

	if (!directoryOnly)
	{
		GLAZIERY_LISTFIELD_M(fileListField, area, choosersGroup, 350, 400, true, true, ONE_OR_NONE)
		this->fileListField = fileListField;
		GLAZIERY_FIELDLAYOUTDATA(fileListField, FILL, FILL, 1)
		fileListField->setHeadlined(false);

		GLAZIERY_LISTFIELD_COLUMN(fileListColumn, fileListField, false, LEFT, "")

		ListFieldListener * listFieldListener;
		if ((listFieldListener = new ListFieldListener) == NULL)
			throw EOUTOFMEMORY;
		fileListField->appendListener(listFieldListener);

		GLAZIERY_GROUPFIELD(textsGroup, area, contentGroup)
		GLAZIERY_GROUPFIELDLAYOUTDATA(textsGroup, 2)
		GLAZIERY_FIELDLAYOUTDATA(textsGroup, FILL, START, 1)

		GLAZIERY_LABEL(fileNameLabel, area, textsGroup, STRING_FILEDIALOG_FILENAME)

		GLAZIERY_TEXTFIELD(fileNameField, area, textsGroup, false)
		this->fileNameField = fileNameField;
		GLAZIERY_FIELDLAYOUTDATA(fileNameField, FILL, START, 1)

		if (!fileTypes.IsEmpty())
		{
			GLAZIERY_LABEL(fileTypeLabel, area, textsGroup, STRING_FILEDIALOG_FILETYPE)

			GLAZIERY_SELECTOR(fileTypeField, area, textsGroup)
			this->fileTypeField = fileTypeField;
			GLAZIERY_FIELDLAYOUTDATA(fileTypeField, FILL, START, 1)

			for (int i=0; i<fileTypes.GetCount(); i++)
			{
				FileType * fileType = fileTypes.Get(i);
				GLAZIERY_SELECTOR_TEXTOPTION(option, textElement, fileTypeField, fileType->getName(), false)
				option->setModel(fileType);
			}

			fileTypeField->setSelection(0);

			FileTypeListener * listener;
			if ((listener = new FileTypeListener) == NULL)
				throw EOUTOFMEMORY;
			fileTypeField->appendListener(listener);
		}

		if (saveDialog)
			fileNameField->setFocus();
		else
			fileListField->setFocus();
	}
	else
		directoryField->setFocus();
}

void FileDialog::updateFileList()
{
	if (fileListField == NULL || directoryTask != NULL)
		return;

	fileListField->removeRows();

	TreeField::Item * item = directoryField->getSelectedItem();
	if (item == NULL)
		return;

	Path * path = (Path *) item->getModel();
	if (path == NULL)
		return;

	if ((directoryTask = new DirectoryTask(path, NULL)) == NULL)
		throw EOUTOFMEMORY;

	directoryTask->run();
}


FileDialog::DirectoryTask::DirectoryTask(Path * path, TreeField::Item * expandItem)
{
	ASSERTION_COBJECT(this);

	this->expandItem = expandItem;
	this->path = path;
	thread = NULL;
}

FileDialog::DirectoryTask::~DirectoryTask()
{
	ASSERTION_COBJECT(this);
	if (thread != NULL) {delete thread; thread = NULL;}
}

bool FileDialog::DirectoryTask::isFinished()
{
	ASSERTION_COBJECT(this);
	return thread != NULL && thread->isFinished();
}

void FileDialog::DirectoryTask::run()
{
	ASSERTION_COBJECT(this);

	if (thread != NULL)
		return;

	thread = Desktop::getInstance()->createThread(&runInternal, false, this);
	thread->resume();

	Desktop::getInstance()->showHourGlass();
}

int FileDialog::DirectoryTask::runInternal(Thread & thread, void * parameter)
{
	DirectoryTask * task = (DirectoryTask *) parameter;

	FilterSettings filterSettings;
	if (task->expandItem != NULL)
		filterSettings.SetAttributeRequired(DU_FDIRECTORY);

	task->directory.ReadFrom(task->path, filterSettings);

	return 0;
}


void FileDialog::FileTypeListener::onSelectionChanged(Selector * selector, int oldSelectionNo)
{
	ASSERTION_COBJECT(this);

	FileDialog * dialog = (FileDialog *) selector->getWindow();
	dialog->updateFileList();
}


FileDialog::FileType::FileType(const String & name)
{
	ASSERTION_COBJECT(this);
	this->name = name;
}

void FileDialog::FileType::addPattern(const String & pattern)
{
	ASSERTION_COBJECT(this);
	patterns.Append(pattern.HeapClone());
}

const String & FileDialog::FileType::getName()
{
	ASSERTION_COBJECT(this);
	return name;
}

const ArrayList<String> & FileDialog::FileType::getPatterns()
{
	ASSERTION_COBJECT(this);
	return patterns;
}


void FileDialog::ListFieldListener::onCellDoubleClicked(ListField * field,
	int rowNo, int columnNo, int elementNo)
{
	ASSERTION_COBJECT(this);

	FileDialog * dialog = (FileDialog *) field->getWindow();
	dialog->endDialog(BUTTON_OK);
}

void FileDialog::ListFieldListener::onSelectionChanged(ListField * field)
{
	ASSERTION_COBJECT(this);

	ListField::Row * row = field->getSelectedRow();
	if (row == NULL)
		return;

	Path * path = (Path *) row->getModel();
	DirectoryEntry pathEntry(path);
	if ((pathEntry.GetFlags() & DU_FDIRECTORY) != 0)
		return;

	FileDialog * dialog = (FileDialog *) field->getWindow();
	dialog->fileNameField->setText(dialog->getPathName(path));
}


VetoMode FileDialog::TreeFieldElementListener::onTextChanging(TextElement * element, const String & newText)
{
	ASSERTION_COBJECT(this);

	TreeField::Item * item = (TreeField::Item *) element->getParent();
	Path * path = (Path *) item->getModel();
	if (path == NULL)
		return VETO;

	Path newPath(path);
	newPath.DeleteLastElement();
	newPath.AppendElement(newText);

	if (!path->MoveFileTo(&newPath, false, false))
	{
		MessageDialog::showDialog(STRING_FILEDIALOG_CANNOTRENAME_TITLE, STRING_FILEDIALOG_CANNOTRENAME_MESSAGE,
			MessageDialog::ICON_ERROR, Dialog::BUTTON_OK);

		return VETO;
	}

	path->DeleteLastElement();
	path->AppendElement(newText);

	return NO_VETO;
}

void FileDialog::TreeFieldListener::onItemExpanded(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);

	FileDialog * dialog = (FileDialog *) item->getField()->getWindow();
	if (dialog->directoryTask != NULL)
		return;

	// Readd 'please wait' item, because it may have been removed
	item->removeChildren();
	GLAZIERY_TREEFIELD_ITEM(dummyChildItem, item->getField(), item)
	GLAZIERY_TREEFIELD_TEXTELEMENT(dummyTextElement, dummyChildItem, STRING_FILEDIALOG_WAITITEM)

	Path * path = (Path *) item->getModel();
	if (path == NULL)
		return;

	if ((dialog->directoryTask = new DirectoryTask(path, item)) == NULL)
		throw EOUTOFMEMORY;

	dialog->directoryTask->run();
}

VetoMode FileDialog::TreeFieldListener::onItemExpanding(TreeField::Item * item)
{
	ASSERTION_COBJECT(this);

	FileDialog * dialog = (FileDialog *) item->getField()->getWindow();
	if (dialog->homePathToBeSelected != NULL || dialog->directoryTask != NULL)
		return VETO;

	return NO_VETO;
}

void FileDialog::TreeFieldListener::onSelectionChanged(TreeField * field)
{
	ASSERTION_COBJECT(this);

	FileDialog * dialog = (FileDialog *) field->getWindow();
	dialog->updateFileList();
}

VetoMode FileDialog::TreeFieldListener::onSelectionChanging(TreeField * field)
{
	ASSERTION_COBJECT(this);

	FileDialog * dialog = (FileDialog *) field->getWindow();
	if (dialog->homePathToBeSelected != NULL || dialog->directoryTask != NULL)
		return VETO;

	return NO_VETO;
}
