/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


Label::Label()
{
	ASSERTION_COBJECT(this);

	allClickable = false;
	autoResizing = true;
	text = "";
	urlsClickable = true;

	Desktop::getInstance()->getSkin()->setupLabel(this);
	resizeInternal(getFont()->getSize(text, isCaps(), true), true);
}

Label::~Label()
{
	ASSERTION_COBJECT(this);
	removeHotKey(ClientArea::findHotKey(text));
}

void Label::autoResize()
{
	ASSERTION_COBJECT(this);
	resizeInternal(getFont()->getSize(text, isCaps(), true), true);
}

void Label::draw(DrawingContext & context)
{
	ASSERTION_COBJECT(this);
	Desktop::getInstance()->getSkin()->drawLabel(context, this);
}

Label::UrlPosition * Label::getUrlPositionAt(Vector position)
{
	if (urlPositions.IsEmpty())
		return NULL;

	for (int i=0; i<urlPositions.GetCount(); i++)
	{
		UrlPosition * urlPosition = urlPositions.Get(i);
		if (urlPosition->isHitAt(position))
			return urlPosition;
	}

	return NULL;
}

const ArrayList<Label::UrlPosition> & Label::getUrlPositions()
{
	ASSERTION_COBJECT(this);
	return urlPositions;
}

String * Label::getTextAttribute()
{
	ASSERTION_COBJECT(this);
	return &text;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(Label, Field);
#endif

bool Label::isAllClickable()
{
	ASSERTION_COBJECT(this);
	return allClickable;
}

bool Label::isAutoResizing()
{
	ASSERTION_COBJECT(this);
	return autoResizing;
}

bool Label::isLikeTitle(Vector position)
{
	ASSERTION_COBJECT(this);

	UrlPosition * urlPosition = getUrlPositionAt(position);
	if (urlPosition != NULL)
		return false;

	return Field::isLikeTitle(position);
}

bool Label::isUrlsClickable()
{
	ASSERTION_COBJECT(this);
	return urlsClickable;
}

void Label::onClick(Vector position, bool option1, bool option2)
{
	ASSERTION_COBJECT(this);

	Field::onClick(position, option1, option2);

	if (isActiveIncludingAncestors())
	{
		if (allClickable)
			Desktop::getInstance()->getPlatformAdapter()->openUrl(text);
		else
		{
			UrlPosition * urlPosition = getUrlPositionAt(position);
			if (urlPosition != NULL)
				Desktop::getInstance()->getPlatformAdapter()->openUrl(urlPosition->getUrl());
		}
	}
}

void Label::setAllClickable(bool allClickable)
{
	ASSERTION_COBJECT(this);

	if (this->allClickable == allClickable)
		return;

	this->allClickable = allClickable;
	if (allClickable)
		urlsClickable = false;

	updateUrlPositions();
	invalidate();
}

void Label::setAutoResizing(bool autoResizing)
{
	ASSERTION_COBJECT(this);

	if (this->autoResizing == autoResizing)
		return;

	this->autoResizing = autoResizing;

	if (autoResizing)
		autoResize();
}

void Label::setCaps(bool caps)
{
	ASSERTION_COBJECT(this);

	if (isCaps() == caps)
		return;

	Field::setCaps(caps);

	if (autoResizing)
		autoResize();
}

void Label::setFont(Font * font)
{
	ASSERTION_COBJECT(this);

	if (getFont() == font)
		return;

	Field::setFont(font);

	if (autoResizing)
		autoResize();
}

void Label::setText(const String & text)
{
	ASSERTION_COBJECT(this);

	if (this->text == text)
		return;

	Field::setText(text);
	updateUrlPositions();

	if (autoResizing)
		autoResize();
}

void Label::setUrlsClickable(bool urlsClickable)
{
	ASSERTION_COBJECT(this);

	if (this->urlsClickable == urlsClickable)
		return;

	this->urlsClickable = urlsClickable;
	if (urlsClickable)
		allClickable = false;

	updateUrlPositions();
	invalidate();
}

String Label::toString()
{
	ASSERTION_COBJECT(this);

	String string;
	string.Format("Label(position:%s,size:%s,text:\"%s\")",
		(const char *) getPosition().toString(), (const char *) getSize().toString(), (const char *) getText());
	return string;
}

void Label::updateUrlPositions()
{
	ASSERTION_COBJECT(this);

	urlPositions.DeleteAll();
	if (!urlsClickable)
		return;

	Font * font = getFont();
	ArrayList<String> * lines = text.Split("\n");
	for (int lineNo = 0; lineNo < lines->GetCount(); lineNo++)
	{
		String * line = lines->Get(lineNo);
		int lineLength = line->GetLength();
		for (int charNo = 0; charNo < lineLength; charNo++)
		{
			int end = charNo;
			bool match = false;
			if (line->Substring(charNo, 7) == "http://")
			{
				match = true;
				end = charNo + 7;
			}
			else if (line->Substring(charNo, 8) == "https://")
			{
				match = true;
				end = charNo + 8;
			}
			else if (line->Substring(charNo, 4) == "www.")
			{
				match = true;
				end = charNo + 4;
			}
			else if (line->Substring(charNo, 6) == "ftp://")
			{
				match = true;
				end = charNo + 6;
			}
			else if (line->Substring(charNo, 7) == "mailto:")
			{
				match = true;
				end = charNo + 7;
			}

			if (match)
			{
				char prefixChar = charNo == 0 ? 0 : (*line)[charNo - 1];
				if (!String::IsLetter(prefixChar) && !String::IsDigit(prefixChar))
				{
					for (; end < lineLength; end++)
					{
						char character = (*line)[end];
						if (!String::IsLetter(character) && !String::IsDigit(character)
							&& strchr(".:-+=_/@%?&#~", character) == NULL)
							break;
					}

					char lastCharacter = (*line)[end - 1];
					if (lastCharacter == '.' || lastCharacter == ':' || lastCharacter == '?')
						end--;

					String prefix = line->Substring(0, charNo);
					int prefixWidth = font->getWidth(prefix, isCaps(), true);

					String url = line->Substring(charNo, end - charNo);
					int urlWidth = font->getWidth(url, isCaps(), true);

					Vector position(prefixWidth, lineNo * font->getHeight());
					Vector size(urlWidth, font->getHeight());

					UrlPosition * urlPosition;
					if ((urlPosition = new UrlPosition(position, size, url)) == NULL)
						throw EOUTOFMEMORY;
					urlPositions.Append(urlPosition);

					charNo = end - 1;
				}
			}
		}
	}

	delete lines;
}


Label::UrlPosition::UrlPosition(Vector position, Vector size, const String & url)
{
	ASSERTION_COBJECT(this);

	this->position = position;
	this->size = size;
	this->url = url;
}

Vector Label::UrlPosition::getPosition()
{
	ASSERTION_COBJECT(this);
	return position;
}

Vector Label::UrlPosition::getSize()
{
	ASSERTION_COBJECT(this);
	return size;
}

const String & Label::UrlPosition::getUrl()
{
	ASSERTION_COBJECT(this);
	return url;
}

bool Label::UrlPosition::isHitAt(Vector position)
{
	ASSERTION_COBJECT(this);
	return position >= this->position && position < this->position + this->size;
}
