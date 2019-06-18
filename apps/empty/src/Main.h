/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __MAIN_H
#define __MAIN_H


/**
 * The application entry point.
 * @param instanceHandle A handle to the current instance
 * @param previousInstanceHandle A handle to the previous instance
 * @param commandLine A pointer to the command line
 * @param show The show state of the top window
 * @return The application exit code.
 */
int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE previousInstanceHandle,
	LPSTR commandLine, int show);


#endif
