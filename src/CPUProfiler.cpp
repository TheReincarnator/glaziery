/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


// Main header file
#include <Glaziery/src/Headers.h>


const int CPUProfiler::SAMPLE_COUNT = 4;

const int CPUProfiler::SAMPLE_LENGTH = 250;

const unsigned long CPUProfiler::USAGE_COLORS[COUNT] =
{
	GL_RGBA(255, 255, 255, 192), // OTHER
	GL_RGBA(255, 64, 128, 192), // GRAPHICS
	GL_RGBA(96, 255, 96, 192), // IO
	GL_RGBA(128, 128, 255, 192), // APPLICATION
	GL_RGBA(64, 64, 64, 128) // IDLE
};

CPUProfiler::CPUProfiler()
{
	ASSERTION_COBJECT(this);

	if ((averageUsageTime = new unsigned long[COUNT]) == NULL)
		throw EOUTOFMEMORY;

	font = NULL;

	if ((frameCounts = new long[SAMPLE_COUNT + 1]) == NULL)
		throw EOUTOFMEMORY;

	samplesTaken = 0;
	sampleStartTime = 0;

	if ((usageTimes = new unsigned long * [SAMPLE_COUNT + 1]) == NULL)
		throw EOUTOFMEMORY;
	for (int sampleNo = 0; sampleNo < SAMPLE_COUNT + 1; sampleNo++)
		if ((usageTimes[sampleNo] = new unsigned long[COUNT]) == NULL)
			throw EOUTOFMEMORY;

	usageType = OTHER;
}

CPUProfiler::~CPUProfiler()
{
	ASSERTION_COBJECT(this);

	if (averageUsageTime != NULL) {delete averageUsageTime; averageUsageTime = NULL;}
	if (frameCounts != NULL) {delete frameCounts; frameCounts = NULL;}

	if (usageTimes != NULL)
	{
		for (int sampleNo = 0; sampleNo < SAMPLE_COUNT + 1; sampleNo++)
			delete usageTimes[sampleNo];

		delete usageTimes;
		usageTimes = NULL;
	}
}

void CPUProfiler::drawProfile()
{
	ASSERTION_COBJECT(this);

	if (font == NULL)
		return;

	updateSamples();

	if (samplesTaken < SAMPLE_COUNT)
		return;

	// Prepare profile information (release current usage)

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();
	unsigned long currentTime = adapter->getTime();

	usageTimes[SAMPLE_COUNT][usageType] += currentTime;

	// Calculate values to be displayed

	long framesAverage = 0;
	for (int sampleNo = 0; sampleNo < SAMPLE_COUNT; sampleNo++)
		framesAverage += frameCounts[sampleNo];
	framesAverage = (long) (1000.0 / SAMPLE_LENGTH * framesAverage
		/ SAMPLE_COUNT + 0.5);

	unsigned long usageTimeTotal = 0;
	for (int usageNo = 0; usageNo < COUNT; usageNo++)
	{
		averageUsageTime[usageNo] = 0;
		for (int sampleNo = 0; sampleNo < SAMPLE_COUNT; sampleNo++)
			averageUsageTime[usageNo] += usageTimes[sampleNo][usageNo];
		averageUsageTime[usageNo] = (unsigned long) (1000.0 / SAMPLE_LENGTH * averageUsageTime[usageNo]
			/ SAMPLE_COUNT + 0.5);

		usageTimeTotal += averageUsageTime[usageNo];
	}

	int usagePercentage;
	if (usageTimeTotal > 0)
		usagePercentage = (int) (100.0 * (usageTimeTotal - averageUsageTime[IDLE]) / usageTimeTotal);
	else
		usagePercentage = 0;

	// Render profile background

	DrawingContext context(DrawingContext(desktop), Vector(desktop->getSize().x - 8 - 159, 8), Vector(159, 40));
	adapter->drawRect(context, Vector(), context.getClippingSize() - Vector(1, 1),
		GL_RGBA(0, 0, 0, 96));

	// Render frames per second information

	String text;
	text.Format("%ld FPS", framesAverage);
	adapter->drawMonoText(context, font, Vector(4, 3), text, GL_WHITE_A(192), false, false);

	text.Format("CPU %d%%", usagePercentage);
	int textWidth = font->getWidth(text);
	adapter->drawMonoText(context, font, Vector(context.getClippingSize().x - textWidth - 3, 3),
		text, GL_WHITE_A(192), false, false);

	// Render CPU usage distribution

	adapter->draw3DRect(context, Vector(4, 16), Vector(155, 23), GL_TRANSPARENCY,
		GL_RGBA(128, 128, 128, 128), GL_RGBA(192, 192, 192, 128));
	int x = 5;
	if (usageTimeTotal > 0)
		for (int usageNo = 0; usageNo < COUNT; usageNo++)
		{
			int percentage = (int) (150.0 * averageUsageTime[usageNo] / usageTimeTotal);
			if (averageUsageTime[usageNo] > 0 && usageNo != IDLE)
			{
				adapter->drawRect(context, Vector(x, 17), Vector(x + percentage - 1, 22),
					USAGE_COLORS[usageNo]);
				x += percentage;
			}
		}

	if (x < 155)
		adapter->drawRect(context, Vector(x, 17), Vector(154, 22), USAGE_COLORS[IDLE]);

	// Render legend

	x = 4;

	text = "OTHER";
	textWidth = font->getWidth(text);
	adapter->drawMonoText(context, font, Vector(x, 24), text,
		USAGE_COLORS[OTHER], false, false);
	x += textWidth + 4;

	text = "GFX";
	textWidth = font->getWidth(text);
	adapter->drawMonoText(context, font, Vector(x, 24), text,
		USAGE_COLORS[GRAPHICS], false, false);
	x += textWidth + 4;

	text = "IO";
	textWidth = font->getWidth(text);
	adapter->drawMonoText(context, font, Vector(x, 24), text,
		USAGE_COLORS[IO], false, false);
	x += textWidth + 4;

	text = "APP";
	textWidth = font->getWidth(text);
	adapter->drawMonoText(context, font, Vector(x, 24), text,
		USAGE_COLORS[APPLICATION], false, false);
	x += textWidth + 4;

	// Engage current usage again

	usageTimes[SAMPLE_COUNT][usageType] -= currentTime;

	// Log profile

	if (desktop->getTimeStepScaled(1000) > 0)
		LOGPRINTF2(LOG_DEBUG, "%ld frames per second, %d%% CPU usage", framesAverage, usagePercentage);
}

CPUProfiler::UsageType CPUProfiler::getUsageType()
{
	ASSERTION_COBJECT(this);
	return usageType;
}

#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
	IMPLEMENT_DYNAMIC(CPUProfiler, CObject);
#endif

void CPUProfiler::onFrame()
{
	ASSERTION_COBJECT(this);

	updateSamples();

	frameCounts[SAMPLE_COUNT]++;
}

void CPUProfiler::reset()
{
	ASSERTION_COBJECT(this);

	Desktop * desktop = Desktop::getInstance();
	PlatformAdapter * adapter = desktop->getPlatformAdapter();
	unsigned long currentTime = adapter->getTime();
	int sampleNo; // loop variable

	if (font == NULL)
		font = adapter->getFont("fonts\\Arial-8-bold-white.png");

	for (sampleNo = 0; sampleNo < SAMPLE_COUNT + 1; sampleNo++)
		frameCounts[sampleNo] = 0;

	samplesTaken = 0;
	sampleStartTime = 0;

	for (sampleNo = 0; sampleNo < SAMPLE_COUNT + 1; sampleNo++)
		for (int usageNo = 0; usageNo < COUNT; usageNo++)
			usageTimes[sampleNo][usageNo] = 0;

	usageType = OTHER;
	usageTimes[SAMPLE_COUNT][usageType] -= currentTime;
}

void CPUProfiler::setUsageType(UsageType usageType)
{
	ASSERTION_COBJECT(this);

	updateSamples();

	if (this->usageType == usageType)
		return;

	unsigned long currentTime = Desktop::getInstance()->getCurrentTime();

	usageTimes[SAMPLE_COUNT][this->usageType] += currentTime;
	this->usageType = usageType;
	usageTimes[SAMPLE_COUNT][this->usageType] -= currentTime;
}

void CPUProfiler::updateSamples()
{
	ASSERTION_COBJECT(this);

	unsigned long currentTime = Desktop::getInstance()->getCurrentTime();

	usageTimes[SAMPLE_COUNT][usageType] += currentTime;
	for (int pass = 0; currentTime >= sampleStartTime + SAMPLE_LENGTH && pass < SAMPLE_COUNT + 1; pass++)
	{
		memmove(frameCounts, &frameCounts[1], sizeof(long) * SAMPLE_COUNT);
		frameCounts[SAMPLE_COUNT] = 0;

		unsigned long * oldUsageTimes = usageTimes[0];
		memmove(usageTimes, &usageTimes[1], sizeof(unsigned long *) * SAMPLE_COUNT);
		usageTimes[SAMPLE_COUNT] = oldUsageTimes;

		for (int usageNo = 0; usageNo < COUNT; usageNo++)
			usageTimes[SAMPLE_COUNT][usageNo] = 0;

		sampleStartTime += SAMPLE_LENGTH;
		samplesTaken++;
	}
	usageTimes[SAMPLE_COUNT][usageType] -= currentTime;

	if (currentTime >= sampleStartTime + SAMPLE_LENGTH)
		sampleStartTime = currentTime;
}
