// Main header file
#include "Headers.h"


const char * mfctriext::GetTechnicalApplicationName()
{
	return "FontCreator";
}


class FontCreator Application;


BEGIN_MESSAGE_MAP(FontCreator, CWinApp)
END_MESSAGE_MAP()


// TODO: Replace MainFrame icon.

FontCreator::FontCreator()
{
	ASSERTION_COBJECT(this);
	ASSERT_POINTER(this, FontCreator);
}

FontCreator::~FontCreator()
{
	ASSERTION_COBJECT(this);
	ASSERT_POINTER(this, FontCreator);
}

int FontCreator::InitInstance()
{
	ASSERTION_COBJECT(this);

	ASSERT_POINTER(this, FontCreator);

	GdiplusStartupInput gdiPlusStartupInput;
	ULONG_PTR gdiPlusToken;
   GdiplusStartup(&gdiPlusToken, &gdiPlusStartupInput, NULL);

	MainDialog Dialog;
	Dialog.DoModal();

	return 0;
}
