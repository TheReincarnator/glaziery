// Main header file
#include "Headers.h"


BEGIN_MESSAGE_MAP(MainDialog, SizableDialog)
	ON_COMMAND(IDC_MAIN_FONT_BROWSE, OnFontBrowse)
	ON_COMMAND(IDC_MAIN_GRADIENTS_ADD, OnGradientsAdd)
	ON_COMMAND(IDC_MAIN_GRADIENTS_REMOVE, OnGradientsRemove)
	ON_COMMAND(IDC_MAIN_PREVIEW_REFRESH, OnPreviewRefresh)
	ON_COMMAND(IDC_MAIN_SAVEFONT, OnSaveFont)
END_MESSAGE_MAP()


MainDialog::MainDialog() : SizableDialog(IDD_MAIN)
{
	ASSERTION_COBJECT(this);

	ASSERT_POINTER(this, MainDialog);
	Initialized = false;
}

void MainDialog::CreateFont(Font & font, REAL * brushPositions, Color * brushColors,
	int brushColorsCount, float spaceWidthHeightRatio, WCHAR * pathNameWString, bool usAsciiOnly)
{
	ASSERTION_COBJECT(this);

	// Measure glyph sizes

	FontFamily family;
	font.GetFamily(&family);
	unsigned int ascent = family.GetCellAscent(FontStyleBold);
	unsigned int ascentPixels = (int) (font.GetSize() * ascent
		/ family.GetEmHeight(FontStyleRegular) + 0.5f);
	int widths[256];
	int offsets[256];
	int totalGlyphWidth = 0;
	int minimumY = 255;
	int maximumY = 0;
	PointF origin(1.25, 1.25);

	for (WCHAR character = 0; character < (usAsciiOnly ? 128 : 256); character++)
	{
		if (character % 128 < 32)
		{
			widths[character] = 1;
			offsets[character] = 0;
			continue;
		}

		Bitmap measureBitmap(256, 256);
		Graphics measureGraphics(&measureBitmap);
		measureGraphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
		measureGraphics.SetTextContrast(0);

		SolidBrush brush(Color(255, 255, 255, 255));

		RectF layout;
		measureGraphics.DrawString((WCHAR *) &character, 1, &font, origin, &brush);
		measureGraphics.MeasureString((WCHAR *) &character, 1, &font, origin, &layout);

		int measuredWidth = (int) (layout.Width + origin.X + 1.0) + 2;
		if (measuredWidth > 256)
			measuredWidth = 256;
		int measuredHeight = (int) (layout.Height + origin.Y + 1.0) + 2;
		if (measuredHeight > 256)
			measuredHeight = 256;

		int minimumX = measuredWidth - 1;
		int maximumX = 0;
		for (int y=0; y<measuredHeight; y++)
			for (int x=0; x<measuredWidth; x++)
			{
				Color color;
				measureBitmap.GetPixel(x, y, &color);
				if (color.GetAlpha() >= 8)
				{
					if (x < minimumX) minimumX = x;
					if (x > maximumX) maximumX = x;
					if (y < minimumY) minimumY = y;
					if (y > maximumY) maximumY = y;
				}
			}

		int width = maximumX - minimumX + 1;
		if (width < 1)
			width = 1;
		int offsetX = minimumX - (int) origin.X;

		widths[character] = width;
		offsets[character] = offsetX;
		totalGlyphWidth += width;
	}

	int maximumGlyphHeight = maximumY - minimumY + 1;
	if (maximumGlyphHeight < 3)
		maximumGlyphHeight = 3;
	int minimumGlyphOffset = (int) (minimumY - origin.Y);

	widths[' '] = (int) (maximumGlyphHeight * spaceWidthHeightRatio + 0.5f);

	// Distribute glyphs to create nealy a square bitmap

	int aimedRowCount = (int) sqrt((double) totalGlyphWidth / (double) maximumGlyphHeight);
	int bitmapWidth = totalGlyphWidth / aimedRowCount;
	if (bitmapWidth > 1)
		bitmapWidth = (int) (2 * pow(2, floor(log((double) bitmapWidth - 1.0) / log(2.0))));

	int x = 2;
	int rowCount = 1;
	WCHAR character;
	for (character = 0; character < (usAsciiOnly ? 128 : 256); character++)
	{
		if (character % 128 < 32 || character == ' ')
			continue;

		if (x + widths[character] > bitmapWidth)
		{
			rowCount++;
			x = 0;
		}

		x += widths[character];
	}
	int bitmapHeight = (maximumGlyphHeight + 1) * rowCount;

	// Create empty bitmap

	Bitmap * bitmap;
	if ((bitmap = new Bitmap(bitmapWidth, bitmapHeight)) == NULL)
		AfxThrowMemoryException();
	Graphics graphics(bitmap);
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics.SetTextContrast(0);

	x = 0;
	int y = 0;

	// Render Glaziery Font identifier (GLF), version, and properties

	bitmap->SetPixel(x, y, Color('G', 'L', 'F', 1));
	bitmap->SetPixel(x, y + 1, Color(maximumGlyphHeight / 256, maximumGlyphHeight % 256,
		ascentPixels / 256, ascentPixels % 256));
	x++;

	// Render underline glyph

	character = '_';

	Rect clippingRect(Point(x, y), Size(1, maximumGlyphHeight + 1));
	graphics.SetClip(clippingRect);

	Rect brushRect(Point(y, x), Size(maximumGlyphHeight + 1, widths[character]));
	LinearGradientBrush brush(brushRect, Color::Black, Color::Black, 0, false);
	brush.SetInterpolationColors(brushColors, brushPositions, brushColorsCount);
	brush.RotateTransform(90.0f);

	origin = PointF((REAL) (x - offsets[character] - widths[character] / 2 + 0.25),
		(REAL) (y - minimumGlyphOffset + 1 + 0.25));
	graphics.DrawString(&character, 1, &font, origin, &brush);

	x++;

	// Render character glyphs

	for (character = 0; character < (usAsciiOnly ? 128 : 256); character++)
	{
		if (character % 128 < 32)
			continue;

		if (x + widths[character] > bitmapWidth)
		{
			y += maximumGlyphHeight + 1;
			x = 0;
		}

		Rect clippingRect(Point(x, y), Size(widths[character], maximumGlyphHeight + 1));
		graphics.SetClip(clippingRect);

		Rect brushRect(Point(y, x), Size(maximumGlyphHeight + 1, widths[character]));
		LinearGradientBrush brush(brushRect, Color::Black, Color::Black, 0, false);
		brush.SetInterpolationColors(brushColors, brushPositions, brushColorsCount);
		brush.RotateTransform(90.0f);

		PointF origin((REAL) (x - offsets[character] + 0.25), (REAL) (y - minimumGlyphOffset + 1 + 0.25));
		graphics.DrawString(&character, 1, &font, origin, &brush);

		x += widths[character];
		bitmap->SetPixel(x - 1, y, Color::White);
	}
	graphics.ResetClip();

	// Save bitmap

	CLSID encoderId;
	if (!GetEncoderClsid(L"image/png", &encoderId))
	{
		MessageBox("Cannot save image: A PNG encoder is not available", "FontCreator", MB_OK | MB_ICONERROR);
		return;
	}

	//WCHAR pathNameWString[255];
	//MultiByteToWideChar(CP_ACP, 0, (const char *) dialog.GetPathName(), -1, pathNameWString, 255);
	bitmap->Save(pathNameWString, &encoderId, NULL);

	delete bitmap;
}

void MainDialog::OnWindowPosChanged(WINDOWPOS * windowPosition)
{
	ASSERTION_COBJECT(this);

	ASSERT_POINTER(this, MainDialog);

	SizableDialog::OnWindowPosChanged(windowPosition);

	if (!Initialized)
		return;
}

void MainDialog::DoDataExchange(CDataExchange * DDXContext)
{
	ASSERTION_COBJECT(this);

	ASSERT_POINTER(this, MainDialog);

	SizableDialog::DoDataExchange(DDXContext);

	if (DDXContext->m_bSaveAndValidate)
	{
	}
	else
	{
	}
}

bool MainDialog::GetEncoderClsid(const WCHAR * format, CLSID * classId)
{
	ASSERTION_COBJECT(this);

	unsigned int count = 0;
	unsigned int size = 0;
	::GetImageEncodersSize(&count, &size);
	if (size == 0)
		return false;

	ImageCodecInfo * imageCodecInfo;
	if ((imageCodecInfo = (ImageCodecInfo *) malloc(size)) == NULL)
		AfxThrowMemoryException();

	::GetImageEncoders(count, size, imageCodecInfo);
	for (unsigned int i=0; i<count; i++)
		if (wcscmp(imageCodecInfo[i].MimeType, format) == 0)
		{
			*classId = imageCodecInfo[i].Clsid;
			free(imageCodecInfo);
			return true;
		}

	free(imageCodecInfo);
	return false;
}

void MainDialog::OnFontBrowse()
{
	ASSERTION_COBJECT(this);

	ASSERT_POINTER(this, MainDialog);

	LOGFONT font;
	ZeroMemory(&font, sizeof(font));
	font.lfHeight = -50;
	CFontDialog Dialog(&font);
	Dialog.DoModal();
}

void MainDialog::OnGradientsAdd()
{
	ASSERTION_COBJECT(this);
	ASSERT_POINTER(this, MainDialog);
}

void MainDialog::OnGradientsRemove()
{
	ASSERTION_COBJECT(this);
	ASSERT_POINTER(this, MainDialog);
}

int MainDialog::OnInitDialog()
{
	ASSERTION_COBJECT(this);

	ASSERT_POINTER(this, MainDialog);

	if (!SizableDialog::OnInitDialog())
		return 0;

	SetIcon(Application.LoadIcon(IDR_MAINFRAME), true);

	// TODO: Setup dialog controls

	Initialized = true;

	// TODO:
	// IDC_MAIN_FONT
	// IDC_MAIN_GRADIENTS
	// IDC_MAIN_GRADIENTANGLE
	// IDC_MAIN_PREVIEW

	return 1;
}

void MainDialog::OnPreviewRefresh()
{
	ASSERTION_COBJECT(this);
	ASSERT_POINTER(this, MainDialog);
}

void MainDialog::OnSaveFont()
{
	ASSERTION_COBJECT(this);

	ASSERT_POINTER(this, MainDialog);

	//CFileDialog dialog(false, "png", NULL, OFN_HIDEREADONLY,
	//	"Portable Network Graphics Files (*.png)|*.png|All Files (*.*)|*.*||");
	//dialog.DoModal();

	REAL brushPositions[4];
	Color brushColors[4];

	// Arial-8-bold-white
	Font font1(L"Arial", 8, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 255, 255, 0);
	brushColors[1] = Color(255, 255, 255, 0);
	CreateFont(font1, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Components\\Glaziery\\resources\\common\\fonts\\Arial-8-bold-white.png");

	// Arial-9-bold-white
	Font font1b(L"Arial", 9, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 255, 255, 0);
	brushColors[1] = Color(255, 255, 255, 0);
	CreateFont(font1b, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Components\\Glaziery\\resources\\common\\fonts\\Arial-9-bold-white.png");

	// Arial-10-bold-white
	Font font1c(L"Arial", 10, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 255, 255, 0);
	brushColors[1] = Color(255, 255, 255, 0);
	CreateFont(font1c, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Components\\Glaziery\\resources\\common\\fonts\\Arial-10-bold-white.png");

	// Verdana-10-bold-blue
	Font font2(L"Verdana", 10, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 7, 0, 102);
	brushColors[1] = Color(255, 7, 0, 102);
	CreateFont(font2, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Verdana-10-bold-blue.png");

	// Verdana-10-bold-gray
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 250, 250, 255);
	brushColors[1] = Color(255, 205, 205, 210);
	CreateFont(font2, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Verdana-10-bold-gray.png");

	// Verdana-10-bold-lightblue-brown
	Font font3(L"Verdana", 10, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 0.55f;
	brushPositions[2] = 0.65f;
	brushPositions[3] = 1.0f;
	brushColors[0] = Color(255, 153, 153, 204);
	brushColors[1] = Color(255, 226, 226, 249);
	brushColors[2] = Color(255, 245, 241, 201);
	brushColors[3] = Color(255, 215, 190, 140);
	CreateFont(font3, brushPositions, brushColors, 4, 0.2f,
		L"E:\\Work\\C\\Components\\Glaziery\\resources\\simpleskin\\fonts\\Verdana-10-bold-lightblue-brown.png");

	// Bauhaus-Md-BT-12-pink
	Font font4(L"Bauhaus Md BT", 12, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 255, 157, 175);
	brushColors[1] = Color(255, 255, 107, 125);
	CreateFont(font4, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Bauhaus-Md-BT-12-pink.png");

	// Bauhaus-Md-BT-12-gray
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 250, 250, 255);
	brushColors[1] = Color(255, 205, 205, 210);
	CreateFont(font4, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Bauhaus-Md-BT-12-gray.png");

	// Bauhaus-Md-BT-12-blue
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 7, 0, 102);
	brushColors[1] = Color(255, 7, 0, 102);
	CreateFont(font4, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Bauhaus-Md-BT-12-blue.png");

	// Verdana-16-bold-blue
	Font font5(L"Verdana", 16, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 7, 0, 102);
	brushColors[1] = Color(255, 7, 0, 102);
	CreateFont(font5, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Verdana-16-bold-blue.png");

	// Verdana-16-bold-gray
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 250, 250, 255);
	brushColors[1] = Color(255, 205, 205, 210);
	CreateFont(font5, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Verdana-16-bold-gray.png");

	// Verdana-16-bold-lightblue-brown
	Font font6(L"Verdana", 16, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 0.53f;
	brushPositions[2] = 0.59f;
	brushPositions[3] = 1.0f;
	brushColors[0] = Color(255, 153, 153, 204);
	brushColors[1] = Color(255, 226, 226, 249);
	brushColors[2] = Color(255, 245, 241, 201);
	brushColors[3] = Color(255, 215, 190, 140);
	CreateFont(font6, brushPositions, brushColors, 4, 0.2f,
		L"E:\\Work\\C\\Components\\Glaziery\\resources\\simpleskin\\fonts\\Verdana-16-bold-lightblue-brown.png");

	// Verdana-14-bold-yellow
	Font font7(L"Verdana", 14, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 255, 248, 224);
	brushColors[1] = Color(255, 252, 226, 96);
	CreateFont(font7, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Verdana-14-bold-yellow.png");

	// Verdana-18-bold-yellow
	Font font8(L"Verdana", 18, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 255, 248, 224);
	brushColors[1] = Color(255, 252, 226, 96);
	CreateFont(font8, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Verdana-18-bold-yellow.png");

	// Verdana-24-bold-green
	Font font9(L"Verdana", 24, FontStyleBold, UnitPoint);
	brushPositions[0] = 0.0f;
	brushPositions[1] = 1.0f;
	brushColors[0] = Color(255, 224, 248, 224);
	brushColors[1] = Color(255, 96, 226, 96);
	CreateFont(font9, brushPositions, brushColors, 2, 0.2f,
		L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Verdana-24-bold-green.png");

	// Swis721-BlkRnd-BT-36-bold-lightblue
	//Font font10(L"Swis721 BlkRnd BT", 36, FontStyleBold, UnitPoint);
	//brushPositions[0] = 0.0f;
	//brushPositions[1] = 1.0f;
	//brushColors[0] = Color(255, 207, 237, 239);
	//brushColors[1] = Color(255, 207, 237, 239);
	//CreateFont(font10, brushPositions, brushColors, 2, 0.2f,
	//	L"E:\\Work\\C\\Projects\\JukeThat\\resources\\fonts\\Swis721-BlkRnd-BT-36-bold-lightblue.png");
}
