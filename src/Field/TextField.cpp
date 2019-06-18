/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


TextField::TextField()
{
	ASSERTION_COBJECT(this);

	cursorColumn = 0;
	cursorRow = 0;
	cursorX = 0;

	String * emptyLine;
	if ((emptyLine = new String) == NULL)
		throw EOUTOFMEMORY;
	lines.Append(emptyLine);

	lineSpacingBottom = 0;
	lineSpacingTop = 0;
	masked = false;
	multiLine = false;
	selectionStartColumn = 0;
	selectionStartRow = 0;

	setMinimumSize(Vector(64, getMinimumSize().y));

	Desktop::getInstance()->getSkin()->setupTextField(this);
}

TextField::~TextField()
{
	ASSERTION_COBJECT(this);
}

void TextField::addContextMenuItems(Menu * menu, Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::addContextMenuItems(menu, position, option1, option2);

	if (!isActiveIncludingAncestors())
		return;

	if (!menu->getItems().IsEmpty())
		menu->appendSeparator();

	Menu::Item * menuItem;
	menuItem = menu->appendItem(STRING_TEXTFIELD_CUT, &onContextMenuCut);
	if (!hasSelection() || masked)
		menuItem->setActive(false);
	menuItem = menu->appendItem(STRING_TEXTFIELD_COPY, &onContextMenuCopy);
	if (!hasSelection() || masked)
		menuItem->setActive(false);
	menu->appendItem(STRING_TEXTFIELD_PASTE, &onContextMenuPaste);
	menuItem = menu->appendItem(STRING_TEXTFIELD_DELETE, &onContextMenuDelete);
	if (!hasSelection())
		menuItem->setActive(false);

	menu->appendSeparator();

	menu->appendItem(STRING_TEXTFIELD_SELECTALL, &onContextMenuSelectAll);
}

void TextField::appendLine(const String & line)
{
	ASSERTION_COBJECT(this);
	insertLine(lines.GetCount(), line);
}

void TextField::constrainCursor()
{
	ASSERTION_COBJECT(this);

	if (cursorRow < 0)
		cursorRow = 0;
	if (cursorRow > lines.GetCount() - 1)
		cursorRow = lines.GetCount() - 1;

	if (cursorColumn < 0)
		cursorColumn = 0;
	if (cursorColumn > lines.Get(cursorRow)->GetLength())
		cursorColumn = lines.Get(cursorRow)->GetLength();

	if (selectionStartRow < 0)
		selectionStartRow = 0;
	if (selectionStartRow > lines.GetCount() - 1)
		selectionStartRow = lines.GetCount() - 1;

	if (selectionStartColumn < 0)
		selectionStartColumn = 0;
	if (selectionStartColumn > lines.Get(selectionStartRow)->GetLength())
		selectionStartColumn = lines.Get(selectionStartRow)->GetLength();

	cursorX = getFont()->getWidth(lines.Get(cursorRow)->Substring(0, cursorColumn), isCaps());

	ensureCursorVisible();
}

void TextField::clear()
{
	ASSERTION_COBJECT(this);

	if (lines.GetCount() == 0)
		return;
	if (lines.GetCount() == 1 && lines.GetFirst()->IsEmpty())
		return;

	if (notifyListenersTextChanging())
		return;

	lines.DeleteAll();
	String * emptyLine;
	if ((emptyLine = new String) == NULL)
		throw EOUTOFMEMORY;
	lines.Append(emptyLine);

	updateContentSize();
	constrainCursor();
	notifyListenersTextChanged();
	invalidate();
}

bool TextField::consumingEnter()
{
	ASSERTION_COBJECT(this);
	return multiLine;
}

bool TextField::deleteSelection()
{
	ASSERTION_COBJECT(this);

	if (!hasSelection())
		return true;

	if (notifyListenersTextChanging())
		return false;

	if (selectionStartRow > cursorRow || selectionStartRow == cursorRow
		&& selectionStartColumn > cursorColumn)
	{
		int tempRow = selectionStartRow;
		selectionStartRow = cursorRow;
		cursorRow = tempRow;

		int tempColumn = selectionStartColumn;
		selectionStartColumn = cursorColumn;
		cursorColumn = tempColumn;
	}

	while (cursorRow >= selectionStartRow + 2)
	{
		lines.Delete(selectionStartRow + 1);
		cursorRow--;
	}

	if (cursorRow > selectionStartRow)
	{
		lines.Get(selectionStartRow)->Cut(selectionStartColumn);
		lines.Get(cursorRow)->CutStart(cursorColumn);
		lines.Get(selectionStartRow)->Append(*lines.Get(cursorRow));
		lines.Delete(cursorRow);
	}
	else
	{
		lines.Get(selectionStartRow)->Cut(selectionStartColumn,
			cursorColumn - selectionStartColumn);
	}

	cursorRow = selectionStartRow;
	cursorColumn = selectionStartColumn;

	updateContentSize();
	constrainCursor();
	notifyListenersTextChanged();
	invalidate();

	return true;
}

void TextField::drawContent(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawTextField(context, this);
}

void TextField::ensureCursorVisible()
{
	ASSERTION_COBJECT(this);

	int lineHeight = lineSpacingTop + getFont()->getHeight() + lineSpacingBottom;
	ensureVisible(Vector(cursorX, cursorRow * lineHeight),
		Vector(cursorX + 1, (cursorRow + 1) * lineHeight - 1));
}

int TextField::getCursorColumn()
{
	ASSERTION_COBJECT(this);
	return cursorColumn;
}

int TextField::getCursorRow()
{
	ASSERTION_COBJECT(this);
	return cursorRow;
}

const ArrayList<String> & TextField::getLines()
{
	ASSERTION_COBJECT(this);
	return lines;
}

int TextField::getLineSpacingBottom()
{
	ASSERTION_COBJECT(this);
	return lineSpacingBottom;
}

int TextField::getLineSpacingTop()
{
	ASSERTION_COBJECT(this);
	return lineSpacingTop;
}

int TextField::getPageLineCount()
{
	ASSERTION_COBJECT(this);
	return getContentViewSize().y / getFont()->getHeight();
}

int TextField::getSelectionStartColumn()
{
	ASSERTION_COBJECT(this);
	return selectionStartColumn;
}

int TextField::getSelectionStartRow()
{
	ASSERTION_COBJECT(this);
	return selectionStartRow;
}

String TextField::getSelectionText()
{
	ASSERTION_COBJECT(this);

	int startRow = selectionStartRow < cursorRow ? selectionStartRow : cursorRow;
	int startColumn = selectionStartRow < cursorRow ? selectionStartColumn : cursorColumn;
	int endRow = selectionStartRow >= cursorRow ? selectionStartRow : cursorRow;
	int endColumn = selectionStartRow >= cursorRow ? selectionStartColumn : cursorColumn;
	if (startRow == endRow && startColumn > endColumn)
	{
		int temp = startColumn;
		startColumn = endColumn;
		endColumn = temp;
	}

	String text;
	for (int rowNo = startRow; rowNo <= endRow; rowNo++)
	{
		if (rowNo > startRow)
			text += "\n";

		if (rowNo == startRow && rowNo == endRow)
			text += lines.Get(rowNo)->Substring(startColumn, endColumn - startColumn);
		else if (rowNo == startRow)
			text += lines.Get(rowNo)->Substring(startColumn);
		else if (rowNo == endRow)
			text += lines.Get(rowNo)->Substring(0, endColumn);
		else
			text += *lines.Get(rowNo);
	}

	return text;
}

String TextField::getText()
{
	ASSERTION_COBJECT(this);

	String text;
	for (int rowNo=0; rowNo<lines.GetCount(); rowNo++)
	{
		if (rowNo > 0)
			text += "\n";
		text += *lines.Get(rowNo);
	}

	return text;
}

double TextField::getTextAsDouble()
{
	ASSERTION_COBJECT(this);

	String text = getTextTrimmed();
	return atof(text);
}

float TextField::getTextAsFloat()
{
	ASSERTION_COBJECT(this);

	String text = getTextTrimmed();
	char decimalPoint = Desktop::getInstance()->getPlatformAdapter()->getDecimalPoint();
	if (decimalPoint != '.')
		text.Replace(decimalPoint, '.');

	return (float) atof(text);
}

int TextField::getTextAsInt()
{
	ASSERTION_COBJECT(this);

	String text = getTextTrimmed();
	return atoi(text);
}

long TextField::getTextAsLong()
{
	ASSERTION_COBJECT(this);

	String text = getTextTrimmed();
	return atol(text);
}

String TextField::getTextTrimmed()
{
	ASSERTION_COBJECT(this);

	String text = getText();
	text.Trim();
	return text;
}

bool TextField::hasSelection()
{
	ASSERTION_COBJECT(this);
	return selectionStartColumn != cursorColumn || selectionStartRow != cursorRow;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(TextField, ScrollingField);
#endif

void TextField::insertLine(int rowNo, const String & line)
{
	ASSERTION_COBJECT(this);

	if (!multiLine || rowNo < 0 || rowNo > lines.GetCount())
		return;

	String * newLine;
	if ((newLine = new String(line)) == NULL)
		throw EOUTOFMEMORY;
	lines.Insert(newLine, rowNo);

	if (cursorRow >= rowNo)
		cursorRow++;
	if (selectionStartRow >= rowNo)
		selectionStartRow++;

	updateContentSize();
	notifyListenersTextChanged();
	invalidate();
}

bool TextField::isHorizontallyScrollable()
{
	ASSERTION_COBJECT(this);
	return multiLine;
}

bool TextField::isMasked()
{
	ASSERTION_COBJECT(this);
	return masked;
}

bool TextField::isMultiLine()
{
	ASSERTION_COBJECT(this);
	return multiLine;
}

bool TextField::isOnWordStart()
{
	ASSERTION_COBJECT(this);

	String * line = lines.Get(cursorRow);
	if (cursorColumn <= 0 || cursorColumn >= line->GetLength())
		return true;

	char currentCharacter = (*line)[cursorColumn];
	if (String::IsWhiteSpace(currentCharacter))
		return false;

	char previousCharacter = (*line)[cursorColumn - 1];
	if (String::IsWhiteSpace(previousCharacter))
		return true;

	bool previousIsLetterOrDigit = String::IsLetter(previousCharacter) || String::IsDigit(previousCharacter);
	if (String::IsLetter(currentCharacter) || String::IsDigit(currentCharacter))
		return !previousIsLetterOrDigit;
	else
		return previousIsLetterOrDigit;
}

bool TextField::isVerticallyScrollable()
{
	ASSERTION_COBJECT(this);
	return multiLine;
}

bool TextField::moveDownInternal(bool option1, bool option2, int count)
{
	ASSERTION_COBJECT(this);

	if (!multiLine)
		return false;

	bool modified = false;

	if (hasSelection() && !option1)
	{
		if (selectionStartRow > cursorRow || selectionStartRow == cursorRow
			&& selectionStartColumn > cursorColumn)
		{
			cursorRow = selectionStartRow;
			cursorColumn = selectionStartColumn;
		}
		else
		{
			selectionStartRow = cursorRow;
			selectionStartColumn = cursorColumn;
		}

		modified = true;
	}

	if (cursorRow < lines.GetCount() - 1)
	{
		if (cursorRow + count >= lines.GetCount() - 1)
			cursorRow = lines.GetCount() - 1;
		else
			cursorRow += count;

		setCursorColumnFromX();
		modified = true;
	}

	if (!option1 && (selectionStartRow != cursorRow || selectionStartColumn != cursorColumn))
	{
		selectionStartRow = cursorRow;
		selectionStartColumn = cursorColumn;
		modified = true;
	}

	if (modified)
	{
		constrainCursor();
		invalidate();
	}

	return true;
}

bool TextField::moveUpInternal(bool option1, bool option2, int count)
{
	ASSERTION_COBJECT(this);

	if (!multiLine)
		return false;

	bool modified = false;

	if (hasSelection() && !option1)
	{
		if (selectionStartRow > cursorRow || selectionStartRow == cursorRow
			&& selectionStartColumn > cursorColumn)
		{
			selectionStartRow = cursorRow;
			selectionStartColumn = cursorColumn;
		}
		else
		{
			cursorRow = selectionStartRow;
			cursorColumn = selectionStartColumn;
		}

		modified = true;
	}

	if (cursorRow > 0)
	{
		if (count >= cursorRow)
			cursorRow = 0;
		else
			cursorRow -= count;

		setCursorColumnFromX();
		modified = true;
	}

	if (!option1 && (selectionStartRow != cursorRow || selectionStartColumn != cursorColumn))
	{
		selectionStartRow = cursorRow;
		selectionStartColumn = cursorColumn;
		modified = true;
	}

	if (modified)
	{
		constrainCursor();
		invalidate();
	}

	return true;
}

bool TextField::onBackSpace()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onBackSpace();

	if (!isEditable())
		return true;

	if (hasSelection())
		deleteSelection();
	else if (cursorRow > 0 || cursorColumn > 0)
	{
		if (notifyListenersTextChanging())
			return true;

		if (cursorColumn > 0)
		{
			cursorColumn--;
			lines.Get(cursorRow)->Cut(cursorColumn, 1);
		}
		else
		{
			cursorRow--;
			cursorColumn = lines.Get(cursorRow)->GetLength();
			lines.Get(cursorRow)->Append(*lines.Get(cursorRow+1));
			lines.Delete(cursorRow+1);
		}

		selectionStartRow = cursorRow;
		selectionStartColumn = cursorColumn;

		updateContentSize();
		constrainCursor();
		notifyListenersTextChanged();
		invalidate();
	}

	return true;
}

bool TextField::onCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onCharacter(character, option1, option2);

	if (!isEditable())
		return true;

	if (hasSelection())
	{
		if (!deleteSelection())
			return true;
	}
	else if (notifyListenersTextChanging())
		return true;

	lines.Get(cursorRow)->Insert(character, cursorColumn);
	cursorColumn++;
	selectionStartColumn = cursorColumn;

	updateContentSize();
	constrainCursor();
	notifyListenersTextChanged();
	invalidate();

	return true;
}

long TextField::onContentHold(Vector position, long holdTime)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onContentHold(position, holdTime);

	int row;
	int column;
	Desktop::getInstance()->getSkin()->getTextFieldRowAndColumnAtPosition(this, row, column, position);

	bool modified = false;
	if (cursorRow != row || cursorColumn != column)
	{
		cursorRow = row;
		cursorColumn = column;

		constrainCursor();
		invalidate();
	}

	return 0;
}

void TextField::onContentPress(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onContentPress(position, option1, option2);

	int row;
	int column;
	Desktop::getInstance()->getSkin()->getTextFieldRowAndColumnAtPosition(this, row, column, position);

	bool modified = false;
	if (cursorRow != row || cursorColumn != column)
	{
		cursorRow = row;
		cursorColumn = column;
		modified = true;
	}

	if (!option1)
		if (selectionStartRow != cursorRow || selectionStartColumn != cursorColumn)
		{
			selectionStartRow = cursorRow;
			selectionStartColumn = cursorColumn;
			modified = true;
		}

	if (modified)
	{
		constrainCursor();
		invalidate();
	}
}

bool TextField::onCopy()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onCopy();

	String selectionText = getSelectionText();
	if (selectionText.IsEmpty() || masked)
		return true;

	Desktop::getInstance()->getPlatformAdapter()->setClipboardText(selectionText);

	return true;
}

void TextField::onContextMenuCut(Menu::Item * item)
{
	TextField * field = (TextField *) item->getContainer();
	field->onCut();
}

void TextField::onContextMenuCopy(Menu::Item * item)
{
	TextField * field = (TextField *) item->getContainer();
	field->onCopy();
}

void TextField::onContextMenuDelete(Menu::Item * item)
{
	TextField * field = (TextField *) item->getContainer();
	field->deleteSelection();
}

void TextField::onContextMenuPaste(Menu::Item * item)
{
	TextField * field = (TextField *) item->getContainer();
	field->onPaste();
}

void TextField::onContextMenuSelectAll(Menu::Item * item)
{
	TextField * field = (TextField *) item->getContainer();
	field->selectAll();
}

bool TextField::onCut()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onCut();

	String selectionText = getSelectionText();
	if (selectionText.IsEmpty() || masked)
		return true;

	Desktop::getInstance()->getPlatformAdapter()->setClipboardText(selectionText);
	deleteSelection();

	return true;
}

bool TextField::onDelete()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onDelete();

	if (!isEditable())
		return true;

	if (hasSelection())
		deleteSelection();
	else
	{
		if (notifyListenersTextChanging())
			return true;

		if (cursorRow < lines.GetCount() - 1
			|| cursorColumn < lines.Get(cursorRow)->GetLength())
		{
			if (cursorColumn < lines.Get(cursorRow)->GetLength())
				lines.Get(cursorRow)->Cut(cursorColumn, 1);
			else
			{
				lines.Get(cursorRow)->Append(*lines.Get(cursorRow+1));
				lines.Delete(cursorRow+1);
			}

			selectionStartRow = cursorRow;
			selectionStartColumn = cursorColumn;

			updateContentSize();
			constrainCursor();
			notifyListenersTextChanged();
			invalidate();
		}
	}

	return true;
}

bool TextField::onEnter(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingField::onEnter(option1, option2);

	if (!isEditable())
		return true;

	if (multiLine)
	{
		if (hasSelection())
		{
			if (!deleteSelection())
				return true;
		}
		else if (notifyListenersTextChanging())
			return true;

		String * newLine;
		if ((newLine = new String(lines.Get(cursorRow)->Substring(cursorColumn))) == NULL)
			throw EOUTOFMEMORY;
		lines.Get(cursorRow)->Cut(cursorColumn);
		lines.Insert(newLine, cursorRow + 1);

		cursorRow++;
		cursorColumn = 0;
		selectionStartRow = cursorRow;
		selectionStartColumn = cursorColumn;

		updateContentSize();
		constrainCursor();
		notifyListenersTextChanged();
		invalidate();

		consumed = true;
	}

	return consumed;
}

void TextField::onGotFocus(bool byParent)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onGotFocus(byParent);

	if (!byParent)
		selectAll();
}

bool TextField::onHotKeyByCharacter(char character, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	// Text fields should not treat characters as hot keys
	return true;
}

void TextField::onLostFocus()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onLostFocus();
	scrollTo(Vector());
}

bool TextField::onMoveDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingField::onMoveDown(option1, option2);
	if (moveDownInternal(option1, option2, 1))
		consumed = true;

	return consumed;
}

bool TextField::onMoveLeft(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveLeft(option1, option2);

	bool modified = false;

	if (hasSelection() && !option1)
	{
		if (selectionStartRow > cursorRow || selectionStartRow == cursorRow
			&& selectionStartColumn > cursorColumn)
		{
			selectionStartRow = cursorRow;
			selectionStartColumn = cursorColumn;
		}
		else
		{
			cursorRow = selectionStartRow;
			cursorColumn = selectionStartColumn;
		}

		modified = true;
	}
	else
	{
		do
		{
			if (cursorColumn > 0)
			{
				cursorColumn--;
				modified = true;
			}
			else if (cursorRow > 0)
			{
				cursorRow--;
				cursorColumn = lines.Get(cursorRow)->GetLength();
				modified = true;
			}
		} while (option2 && !isOnWordStart());

		if (!option1 && (selectionStartRow != cursorRow || selectionStartColumn != cursorColumn))
		{
			selectionStartRow = cursorRow;
			selectionStartColumn = cursorColumn;
			modified = true;
		}
	}

	if (modified)
	{
		constrainCursor();
		invalidate();
	}

	return true;
}

bool TextField::onMoveRight(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveRight(option1, option2);

	bool modified = false;

	if (hasSelection() && !option1)
	{
		if (selectionStartRow > cursorRow || selectionStartRow == cursorRow
			&& selectionStartColumn > cursorColumn)
		{
			cursorRow = selectionStartRow;
			cursorColumn = selectionStartColumn;
		}
		else
		{
			selectionStartRow = cursorRow;
			selectionStartColumn = cursorColumn;
		}

		modified = true;
	}
	else
	{
		do
		{
			if (cursorColumn < lines.Get(cursorRow)->GetLength())
			{
				cursorColumn++;
				modified = true;
			}
			else if (cursorRow < lines.GetCount() - 1)
			{
				cursorRow++;
				cursorColumn = 0;
				modified = true;
			}
		} while (option2 && !isOnWordStart());

		if (!option1 && (selectionStartRow != cursorRow || selectionStartColumn != cursorColumn))
		{
			selectionStartRow = cursorRow;
			selectionStartColumn = cursorColumn;
			modified = true;
		}
	}

	if (modified)
	{
		constrainCursor();
		invalidate();
	}

	return true;
}

bool TextField::onMoveToEnd(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveToEnd(option1, option2);

	bool modified = false;

	if (option2)
	{
		if (cursorRow < lines.GetCount() - 1)
		{
			cursorRow = lines.GetCount() - 1;
			modified = true;
		}
	}
	else
	{
		if (cursorColumn < lines.Get(cursorRow)->GetLength())
		{
			cursorColumn = lines.Get(cursorRow)->GetLength();
			modified = true;
		}
	}

	if (!option1 && (selectionStartRow != cursorRow || selectionStartColumn != cursorColumn))
	{
		selectionStartRow = cursorRow;
		selectionStartColumn = cursorColumn;
		modified = true;
	}

	if (modified)
	{
		constrainCursor();
		invalidate();
	}

	return true;
}

bool TextField::onMoveToStart(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	ScrollingField::onMoveToStart(option1, option2);

	bool modified = false;

	if (option2)
	{
		if (cursorRow > 0)
		{
			cursorRow = 0;
			modified = true;
		}
	}
	else
	{
		if (cursorColumn > 0)
		{
			cursorColumn = 0;
			modified = true;
		}
	}

	if (!option1 && (selectionStartRow != cursorRow || selectionStartColumn != cursorColumn))
	{
		selectionStartRow = cursorRow;
		selectionStartColumn = cursorColumn;
		modified = true;
	}

	if (modified)
	{
		constrainCursor();
		invalidate();
	}

	return true;
}

bool TextField::onMoveUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingField::onMoveUp(option1, option2);
	if (moveUpInternal(option1, option2, 1))
		consumed = true;

	return consumed;
}

bool TextField::onPageDown(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingField::onPageDown(option1, option2);

	int pageLineCount = getPageLineCount();
	if (pageLineCount < 1)
		pageLineCount = 1;
	if (moveDownInternal(option1, option2, pageLineCount))
		consumed = true;

	return consumed;
}

bool TextField::onPageUp(bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	bool consumed = ScrollingField::onPageUp(option1, option2);

	int pageLineCount = getPageLineCount();
	if (pageLineCount < 1)
		pageLineCount = 1;
	if (moveUpInternal(option1, option2, pageLineCount))
		consumed = true;

	return consumed;
}

bool TextField::onPaste()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onPaste();

	if (!isEditable())
		return true;

	if (hasSelection())
	{
		if (!deleteSelection())
			return true;
	}
	else if (notifyListenersTextChanging())
		return true;

	String clipboardText = Desktop::getInstance()->getPlatformAdapter()->getClipboardText();
	ArrayList<String> lines;
	splitTextIntoLines(lines, clipboardText);

	for (int lineNo = 0; lineNo < lines.GetCount(); lineNo++)
	{
		String * line = lines.Get(lineNo);
		this->lines.Get(cursorRow)->Insert(line, cursorColumn);
		cursorColumn += line->GetLength();

		if (lineNo < lines.GetCount() - 1)
		{
			String * newLine;
			if ((newLine = new String(this->lines.Get(cursorRow)->Substring(cursorColumn))) == NULL)
				throw EOUTOFMEMORY;
			this->lines.Get(cursorRow)->Cut(cursorColumn);
			this->lines.Insert(newLine, cursorRow + 1);

			cursorRow++;
			cursorColumn = 0;
		}
	}

	selectionStartRow = cursorRow;
	selectionStartColumn = cursorColumn;

	updateContentSize();
	constrainCursor();
	notifyListenersTextChanged();
	invalidate();

	return true;
}

bool TextField::onSelectAll()
{
	ASSERTION_COBJECT(this);

	ScrollingField::onSelectAll();

	selectAll();
	return true;
}


void TextField::prependLine(const String & line)
{
	ASSERTION_COBJECT(this);
	insertLine(0, line);
}

void TextField::removeLine(int rowNo)
{
	ASSERTION_COBJECT(this);

	if (!multiLine || rowNo < 0 || rowNo >= lines.GetCount())
		return;

	if (rowNo == 0 && lines.GetCount() == 1)
	{
		lines.GetFirst()->Clear();

		cursorRow = 0;
		cursorColumn = 0;
		selectionStartRow = 0;
		selectionStartColumn = 0;
	}
	else
	{
		lines.Delete(rowNo);

		if (cursorRow == rowNo)
			cursorColumn = 0;
		else if (cursorRow > rowNo)
			cursorRow--;

		if (selectionStartRow == rowNo)
			selectionStartColumn = 0;
		else if (selectionStartRow > rowNo)
			selectionStartRow--;
	}

	updateContentSize();
	notifyListenersTextChanged();
	invalidate();
}

void TextField::selectAll()
{
	ASSERTION_COBJECT(this);

	int lineCount = lines.GetCount();
	int lastLineLength = lineCount > 0 ? lines.Get(lineCount - 1)->GetLength() : 0;

	if (selectionStartRow == 0 && selectionStartColumn == 0
		&& cursorRow == lineCount - 1 && cursorColumn == lastLineLength)
		return;

	selectionStartRow = 0;
	selectionStartColumn = 0;
	cursorRow = lineCount - 1;
	cursorColumn = lastLineLength;

	constrainCursor();
	invalidate();
}

void TextField::selectCharacters(int startRow, int startColumn, int characters)
{
	ASSERTION_COBJECT(this);

	if (startRow < 0)
		startRow = 0;
	if (startColumn < 0)
		startColumn = 0;
	if (startRow > lines.GetCount() - 1)
		startRow = lines.GetCount() - 1;
	if (startColumn > lines.Get(startRow)->GetLength())
		startColumn = lines.Get(startRow)->GetLength();

	selectionStartRow = startRow;
	selectionStartColumn = startColumn;
	cursorRow = startRow;
	cursorColumn = startColumn;

	while (characters > 0)
	{
		if (characters <= lines.Get(cursorRow)->GetLength() - cursorColumn)
		{
			cursorColumn += characters;
			characters = 0;
		}
		else
		{
			if (cursorRow >= lines.GetCount() - 1)
			{
				characters = 0;
				cursorColumn = lines.Get(cursorRow)->GetLength();
			}
			else
			{
				characters -= lines.Get(cursorRow)->GetLength() - cursorColumn + 1;
				cursorRow++;
				cursorColumn = 0;
			}
		}
	}

	constrainCursor();
	invalidate();
}

void TextField::selectRows(int startRow, int rows)
{
	ASSERTION_COBJECT(this);

	if (startRow < 0)
		startRow = 0;
	if (startRow > lines.GetCount() - 1)
		startRow = lines.GetCount() - 1;

	selectionStartRow = startRow;
	selectionStartColumn = 0;

	if (startRow + rows >= lines.GetCount() - 1)
	{
		cursorRow = lines.GetCount() - 1;
		cursorColumn = lines.Get(cursorRow)->GetLength();
	}
	else
	{
		cursorRow = startRow + rows;
		cursorColumn = 0;
	}

	constrainCursor();
	invalidate();
}

void TextField::setCursorColumnFromX()
{
	ASSERTION_COBJECT(this);

	Font * font = getFont();
	String * line = lines.Get(cursorRow);
	int lineLength = line->GetLength();

	int x = 0;
	for (cursorColumn = 0; cursorColumn < lineLength; cursorColumn++)
	{
		int width = font->getWidth(line->Substring(cursorColumn, 1), isCaps());
		if (x + width / 2 >= cursorX)
			break;
		x += width;
	}
}

void TextField::setCursorPosition(int row, int column)
{
	ASSERTION_COBJECT(this);
	selectCharacters(row, column, 0);
}

void TextField::setLines(const ArrayList<String> & lines)
{
	ASSERTION_COBJECT(this);

	// Check whether nothing changed

	if (this->lines.GetCount() == lines.GetCount())
	{
		int lineNo;
		for (lineNo=0; lineNo<lines.GetCount(); lineNo++)
			if (this->lines.Get(lineNo) != lines.Get(lineNo))
				break;

		if (lineNo >= lines.GetCount())
			return;
	}

	if (notifyListenersTextChanging())
		return;

	// Build a clone of the lines provided

	this->lines.DeleteAll();

	if (lines.IsEmpty())
	{
		String * emptyLine;
		if ((emptyLine = new String) == NULL)
			throw EOUTOFMEMORY;
		this->lines.Append(emptyLine);
	}
	else if (multiLine)
	{
		for (int i=0; i<lines.GetCount(); i++)
		{
			String * clone;
			if ((clone = new String(lines.Get(i))) == NULL)
				throw EOUTOFMEMORY;
			this->lines.Append(clone);
		}
	}
	else
	{
		String * clone;
		if ((clone = new String(lines.GetFirst())) == NULL)
			throw EOUTOFMEMORY;
		this->lines.Append(clone);
	}

	// Update field properties

	updateContentSize();
	constrainCursor();
	notifyListenersTextChanged();
	invalidate();
}

void TextField::setLineSpacingBottom(int lineSpacingBottom)
{
	ASSERTION_COBJECT(this);

	if (this->lineSpacingBottom == lineSpacingBottom)
		return;

	this->lineSpacingBottom = lineSpacingBottom;
	invalidate();
}

void TextField::setLineSpacingTop(int lineSpacingTop)
{
	ASSERTION_COBJECT(this);

	if (this->lineSpacingTop == lineSpacingTop)
		return;

	this->lineSpacingTop = lineSpacingTop;
	invalidate();
}

void TextField::setMasked(bool masked)
{
	ASSERTION_COBJECT(this);

	this->masked = masked;

	updateContentSize();
	constrainCursor();
	invalidate();
}

void TextField::setMultiLine(bool multiLine)
{
	ASSERTION_COBJECT(this);

	this->multiLine = multiLine;

	if (!multiLine)
		while (lines.GetCount() > 1)
			lines.Delete(1);

	updateContentSize();
	constrainCursor();
	invalidate();
}

void TextField::setText(const String & text)
{
	ASSERTION_COBJECT(this);

	ArrayList<String> lines;
	splitTextIntoLines(lines, text);
	setLines(lines);
}

BalloonPopup * TextField::showBalloonPopup(const String & text)
{
	ASSERTION_COBJECT(this);
	return Field::showBalloonPopup(text);
}

BalloonPopup * TextField::showBalloonPopup(int row, int column, const String & text,
	bool includeSpacing, bool positionCursor)
{
	ASSERTION_COBJECT(this);

	if (row < 0)
		row = 0;
	if (row > lines.GetCount() - 1)
		row = lines.GetCount() - 1;

	String * line = lines.Get(row);
	if (column < 0)
		column = 0;
	if (column > line->GetLength())
		column = line->GetLength();

	if (positionCursor)
		setCursorPosition(row, column);

	BalloonPopup * popup;
	if ((popup = new BalloonPopup) == NULL)
		throw EOUTOFMEMORY;
	Desktop::getInstance()->addPopup(popup);

	popup->setMessage(text);

	Font * font = getFont();
	int lineHeight = lineSpacingTop + font->getHeight() + lineSpacingBottom;
	Vector positionFrom = getAbsolutePosition() + getBorderSize() + getOuterPaddingSize();
	positionFrom -= getScrollPosition();
	positionFrom += Vector(font->getWidth(line->Substring(0, column)), row * lineHeight);

	Vector positionTo = positionFrom + Vector(font->getWidth((*line)[column]) - 1, lineHeight - 1);

	if (!includeSpacing)
	{
		positionFrom += Vector(0, lineSpacingTop);
		positionTo -= Vector(0, lineSpacingBottom);
	}

	popup->pointTo(positionFrom, positionTo);

	return popup;
}

void TextField::splitTextIntoLines(ArrayList<String> & lines, const String & text)
{
	ASSERTION_COBJECT(this);

	int startPos = 0;
	int enterPos;
	do
	{
		enterPos = text.Find("\n", startPos);
		if (enterPos < 0)
			enterPos = text.GetLength();

		String * clone;
		if ((clone = new String(text.Substring(startPos, enterPos - startPos))) == NULL)
			throw EOUTOFMEMORY;
		lines.Append(clone);

		startPos = enterPos + 1;
	}
	while (multiLine && enterPos < text.GetLength());
}

String TextField::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("TextField(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}

void TextField::updateContentSize()
{
	ASSERTION_COBJECT(this);
	contentSize = Desktop::getInstance()->getSkin()->getTextFieldContentSize(this);
}

bool TextField::validateEqual(TextField * other)
{
	ASSERTION_COBJECT(this);

	if (getText() != other->getText())
	{
		showBalloonPopup(STRING_GLAZIERY_VALIDATE_TEXTFIELD_NOTEQUAL);
		return false;
	}

	return true;
}

bool TextField::validateFloat()
{
	ASSERTION_COBJECT(this);

	String text = getTextTrimmed();
	if (text.IsEmpty())
		return true;

	int textLength = text.GetLength();
	char decimalPoint = Desktop::getInstance()->getPlatformAdapter()->getDecimalPoint();
	bool foundDecimalPoint = false;
	bool foundDigit = false;
	int pos;
	for (pos=0; pos<textLength; pos++)
	{
		char character = text[pos];
		if (character == '+' || character == '-')
		{
			if (pos > 0)
				break;
		}
		else if (character == decimalPoint)
		{
			if (foundDecimalPoint)
				break;
			else
				foundDecimalPoint = true;
		}
		else if (character >= '0' && character <= '9')
			foundDigit = true;
		else
			break;
	}

	if (pos < textLength || !foundDigit)
	{
		showBalloonPopup(STRING_GLAZIERY_VALIDATE_TEXTFIELD_INVALIDFLOAT);
		return false;
	}

	return true;
}

bool TextField::validateFloatRange(double from, double to)
{
	ASSERTION_COBJECT(this);

	String text = getTextTrimmed();
	if (text.IsEmpty())
		return true;

	if (!validateFloat())
		return false;

	double value = getTextAsDouble();
	if (value < from)
	{
		String message;
		message.Format(STRING_GLAZIERY_VALIDATE_TEXTFIELD_TOOSMALL_FLOAT, from);
		showBalloonPopup(message);
		return false;
	}
	else if (value > to)
	{
		String message;
		message.Format(STRING_GLAZIERY_VALIDATE_TEXTFIELD_TOOLARGE_FLOAT, to);
		showBalloonPopup(message);
		return false;
	}

	return true;
}

bool TextField::validateInt()
{
	ASSERTION_COBJECT(this);

	String text = getTextTrimmed();
	if (text.IsEmpty())
		return true;

	int textLength = text.GetLength();
	bool foundDigit = false;
	int pos;
	for (pos=0; pos<textLength; pos++)
	{
		char character = text[pos];
		if (character == '+' || character == '-')
		{
			if (pos > 0)
				break;
		}
		else if (character >= '0' && character <= '9')
			foundDigit = true;
		else
			break;
	}

	if (pos < textLength || !foundDigit)
	{
		showBalloonPopup(STRING_GLAZIERY_VALIDATE_TEXTFIELD_INVALIDINT);
		return false;
	}

	return true;
}

bool TextField::validateIntRange(long from, long to)
{
	ASSERTION_COBJECT(this);

	String text = getTextTrimmed();
	if (text.IsEmpty())
		return true;

	if (!validateInt())
		return false;

	long value = getTextAsLong();
	if (value < from)
	{
		String message;
		message.Format(STRING_GLAZIERY_VALIDATE_TEXTFIELD_TOOSMALL_INT, from);
		showBalloonPopup(message);
		return false;
	}
	else if (value > to)
	{
		String message;
		message.Format(STRING_GLAZIERY_VALIDATE_TEXTFIELD_TOOLARGE_INT, to);
		showBalloonPopup(message);
		return false;
	}

	return true;
}

bool TextField::validateMaxLength(int maxLength, bool trim)
{
	ASSERTION_COBJECT(this);

	String text = trim ? getTextTrimmed() : getText();
	if (text.GetLength() > maxLength)
	{
		String message;
		message.Format(STRING_GLAZIERY_VALIDATE_TEXTFIELD_TOOLONG, maxLength);
		showBalloonPopup(message);
		return false;
	}

	return true;
}

bool TextField::validateMinLength(int minLength, bool trim)
{
	ASSERTION_COBJECT(this);

	String text = trim ? getTextTrimmed() : getText();
	if (!text.IsEmpty() && text.GetLength() < minLength)
	{
		String message;
		message.Format(STRING_GLAZIERY_VALIDATE_TEXTFIELD_TOOSHORT, minLength);
		showBalloonPopup(message);
		return false;
	}

	return true;
}

bool TextField::validateRequired(bool trim)
{
	ASSERTION_COBJECT(this);

	String text = trim ? getTextTrimmed() : getText();
	if (text.IsEmpty())
	{
		showBalloonPopup(STRING_GLAZIERY_VALIDATE_TEXTFIELD_REQUIRED);
		return false;
	}

	return true;
}
