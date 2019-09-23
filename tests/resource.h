#pragma once

#include <stdio.h>

#ifdef WIN32
#include <conio.h>
#include <windows.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif
#define mygetch	getch

#elif defined(LINUX)
#include <stdint.h>
#include <termios.h>
#include <unistd.h>

int mygetch(void)
{
	struct termios oldt,
		newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#endif