/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_CPUPROFILER_H
#define __GLAZIERY_CPUPROFILER_H


namespace glaziery
{
	/**
	 * The CPU profiler records and displays information about frames
	 * per second, CPU usage type distribution and other statistic information.
	 */
	#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
		class CPUProfiler : public CObject
	#else
		class CPUProfiler
	#endif
	{
		public:

			enum UsageType
			{
				/**
				 * The CPU has been used for other purposes than stated below.
				 */
				OTHER = 0,

				/**
				 * The CPU has been used for graphics operations (e.g. rendering).
				 */
				GRAPHICS,

				/**
				 * The CPU has been used for I/O communication (input devices, network, etc.).
				 */
				IO,

				/**
				 * The CPU has been used by the application for various purposes.
				 */
				APPLICATION,

				/**
				 * The CPU has been idle.
				 */
				IDLE,

				/**
				 * The total number of different usage types.
				 */
				COUNT
			};

		protected:

			/**
			 * The number of samples to remember. The profiler will take the average
			 * of all samples in history when rendering the CPU profile.
			 */
			static const int SAMPLE_COUNT;

			/**
			 * The number of milliseconds of one samples. The profiler will take the average
			 * of all samples in history when rendering the CPU profile.
			 */
			static const int SAMPLE_LENGTH;

			/**
			 * The colors for the usage types when rendering the CPU profile.
			 */
			static const unsigned long USAGE_COLORS[COUNT];

			/**
			 * A temporary array of usage times when rendering the CPU profile.
			 */
			unsigned long * averageUsageTime;

			/**
			 * The font usage for rendering profile information.
			 */
			Font * font;

			/**
			 * An array of samples, each containing the number of frames rendered in that sample.
			 */
			long * frameCounts;

			/**
			 * The number of samples taken since the last call to reset().
			 */
			long samplesTaken;

			/**
			 * The start time of the current sample.
			 */
			unsigned long sampleStartTime;

			/**
			 * An array of samples, each containing an array of CPU usage times, one for each usage type.
			 */
			unsigned long ** usageTimes;

			/**
			 * The currently set usage type.
			 */
			UsageType usageType;

			/**
			 * Checks whether the current sample is over and shifts all samples accordingly.
			 */
			void updateSamples();

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(CPUProfiler);
			#endif

		public:

			/**
			 * Creates a new CPU profiler.
			 */
			CPUProfiler();

			/**
			 * Destroys the CPU profiler.
			 */
			~CPUProfiler();

			/**
			 * Renders the profile information on the desktop.
			 */
			void drawProfile();

			/**
			 * Returns the currently set usage type.
			 * @return The currently set usage type.
			 */
			UsageType getUsageType();

			/**
			 * Notifies the profiler about a rendered frame (updates frame count).
			 */
			void onFrame();

			/**
			 * Resets all profiling information.
			 * @note Call this method at least once before calling any notification method.
			 */
			void reset();

			/**
			 * Notifies the profiler about the beginning of a certain type of CPU usage
			 * (update usage type times).
			 * @param usageType The new usage type.
			 */
			void setUsageType(UsageType usageType);
	};
}


#endif
