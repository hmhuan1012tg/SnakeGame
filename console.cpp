//File "console.cpp"

#include "console.h"

bool gamePaused = false;

void fixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void goToXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ci;
	ci.bVisible = FALSE;
	ci.dwSize = 100;
	SetConsoleCursorInfo(output, &ci);
}

void exitGame(HANDLE t) {
	system("cls");
	TerminateThread(t, 0);
}

void pauseGame(HANDLE t) {
	SuspendThread(t);
	gamePaused = true;
}

void resumeGame(HANDLE t) {
	ResumeThread(t);
	gamePaused = false;
}
