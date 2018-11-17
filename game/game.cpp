#include "pch.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>

const int size = 21;

std::string maps[21][21][size];
HANDLE hStdOut;
int houses = 0;
int wood = 0;
int iPosX = size / 2;
int iPosY = size / 2;
int oiPosX = size / 2;
int oiPosY = size / 2;
int iLevX = 10;
int iLevY = 10;

void cls();
void gen();
void init();
void move(char c);
void setCursorPosition(int x, int y);
void ShowConsoleCursor(bool showFlag);
void updateCharacterPosition();
void updateWoodAndHouses();
void setColor(int color);

//1 - Blue, 2 - Light Blue, 3 - Red, 4 - Green, 5 - White
void setColor(int color)
{
	switch (color) {
		case 1:
			SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE);
			break;
		case 2:
			SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case 3:
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			break;
		case 4:
			SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
			break;
		default:
			SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			break;
	}
}

void ShowConsoleCursor(bool showFlag)
{
	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(hStdOut, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(hStdOut, &cursorInfo);
}

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void cls()
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	std::cout.flush();

	if (!GetConsoleScreenBufferInfo(hOut, &csbi))
	{
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(hOut, topLeft);
}

void gen()
{
	if (maps[iLevY][iLevX][0] == std::string(" "))
	{
		srand((unsigned int)time(NULL));
		char land[] = { '.', 'Y' };
		for (int i = 0; i < size; i++)
		{
			maps[iLevY][iLevX][i] = "";
			for (int j = 0; j < size; j++)
			{
				char cLand = (rand() % 15 == 0) ? land[1] : land[0];
				maps[iLevY][iLevX][i] += cLand;
			}
		}
	}
	cls();
	for (int i = 0; i < size; i++)
	{
		std::string sTemp = maps[iLevY][iLevX][i];
		for (int j = 0; j < size; j++)
		{
			switch (sTemp[j])
			{
				case 'Y':
					setColor(4);
					printf("%c", sTemp[j]);
					break;
				case 'A':
					setColor(3);
					printf("%c", sTemp[j]);
					break;
				default:
					setColor(2);
					printf("%c", sTemp[j]);
					break;
			}
		}
		printf("\n");
	}
	setColor(5);
	printf("\nLocation: X-%d Y-%d\n", iLevX, iLevY);
	updateWoodAndHouses();
	updateCharacterPosition();
}

void updateCharacterPosition()
{
	setCursorPosition(oiPosX, oiPosY);
	if (maps[iLevY][iLevX][oiPosY][oiPosX] != 'A')
	{
		setColor(2);
		printf(".");
	}
	else
	{
		setColor(3);
		printf("A");
	}
	setCursorPosition(iPosX, iPosY);
	setColor(1);
	printf("@");
}

void updateWoodAndHouses()
{
	setColor(5);
	setCursorPosition(0, size + 2);
	printf("Wood: %d       \nHouses: %d       ", wood, houses);
}

void init() {
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	ShowConsoleCursor(false);
	printf("wasd or arrow keys - move\nspace - build house(100 wood)\nq - quit\npress any key to start");
	_getch(); //Stops the exectution of the program until a key is pressed
	cls();
	for (int i = 0; i < 21; i++)     // Fills all of the maps with a space to check if it is empty.
	{                                // Without this, the values would be random and I would not be able
		for (int j = 0; j < 21; j++) // to check if it is empty        
		{
			for (int k = 0; k < size; k++)
			{
				maps[i][j][k] = " ";
			}
		}
	}
	gen();
}

void move(char c)
{
	char charOn = maps[iLevY][iLevX][iPosY][iPosX];
	oiPosX = iPosX;
	oiPosY = iPosY;

	if (iPosY != 0 && (c == 'w' || c == 72))
	{
		iPosY--;
		updateCharacterPosition();
	}
	else if ((c == 'w' || c == 72) && iLevY < 20)
	{
		iLevY++;
		iPosY = size - 1;
		gen();
	}

	if (iPosX != 0 && (c == 'a' || c == 75))
	{
		iPosX--;
		updateCharacterPosition();
	}
	else if ((c == 'a' || c == 75) && iLevX > 0)
	{
		iLevX--;
		iPosX = size - 1;
		gen();
	}

	if (iPosY != size - 1 && (c == 's' || c == 80))
	{
		iPosY++;
		updateCharacterPosition();
	}
	else if ((c == 's' || c == 80) && iLevY > 0)
	{
		iLevY--;
		iPosY = 0;
		gen();
	}

	if (iPosX != size - 1 && (c == 'd' || c == 77))
	{
		iPosX++;
		updateCharacterPosition();
	}
	else if ((c == 'd' || c == 77) && iLevX < 20)
	{
		iLevX++;
		iPosX = 0;
		gen();
	}

	if (charOn == 'Y')
	{
		maps[iLevY][iLevX][iPosY][iPosX] = '.';
		wood += 10 + houses;
		updateWoodAndHouses();
	}
	else if (c == ' ' && charOn != 'A' && charOn != 'Y' && wood > 99)
	{
		maps[iLevY][iLevX][iPosY][iPosX] = 'A';
		houses++;
		wood -= 100;
		updateWoodAndHouses();
	}
}

int main()
{
	init();
	char c = '0';
	while (c != 'q') {
		c = _getch();
		move(c);
	}
}