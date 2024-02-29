// Chinh_Game_PACMAN.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include "math.h"
#include "windows.h"
#include "time.h"
#include "CType.h"
#include "string.h"
#include "console.h"

// Định nghĩa độ rộng màn hình trò chơi Console 
#define consoleW 85
#define consoleH 44

// Màn hình cho phép di chuyển bên trái và bên phải
const int consoleL = 20;
const int consoleR = 40;

int start = 0;
int numberCar = 4;
const int shapeW = 3; // độ rộng hình dạng 
const int shapeH = 3; // độ cao hình dạng

// tọa độ
struct Coordinate
{
	int x;
	int y;
};

// vận tốc
struct Velocity
{
	int x = 1;
	int y = 1;
};

struct Picture
{
	char **s = new char *[13];
};

struct Arrow
{
	char *arrow = "=>";
	int x = 48;
	int y = 12;
};

struct Shape
{
	// Allocate car
	// con trỏ cấp 2 cấp phát 3 con trỏ cấp 1
	char **car = new char*[shapeH];
	Velocity vantoc;
	Coordinate toado;
	int totalScore = 0;
	int score = 0;
	int live = 3;
	int save = 0;
	int map = 1;
	int time = 0;
	int direction = 3; // hướng mặc định của hình dạng
};

struct Cell
{
	int chr; // kí tự
	int color; // giá trị màu
	int status = 0; // trạng thái của Cell
};

Cell **MatrixCell; // con trỏ cấu trúc cấp 2

void(*selectedPointer)(Arrow &ar, Picture &pic, Shape &player, Shape *&computer, FILE *&f);
void(*playPointer)(Shape &player, Shape *&computer, Picture &pic, Arrow &ar, FILE *&f, int saves);

void Init(int posR, int posC, int kitu = 32, int color = 7)
{
	// tại (posR,posC) cập nhật lại cell
	MatrixCell[posR][posC].chr = kitu;
	MatrixCell[posR][posC].color = color;
}

void Allocate()
{
	MatrixCell = new Cell*[consoleH];
	for (int i = 0; i < consoleH; i++)
	{
		MatrixCell[i] = new Cell[consoleW + 1];
		for (int j = 0; j < consoleW; j++)
			Init(i, j);
	}
}

void ReadPicture(Picture &pic)
{
	FILE *f;
	errno_t err;
	err = fopen_s(&f, "data/Intro.txt", "rt");
	for (int i = 0; i < 14; i++)
	{
		pic.s[i] = new char[255];
		fgets(pic.s[i], 255, f);
	}
}

void DisplayPicture(Picture pic, int count)
{
	int color = (rand() % 14) + 1;
	gotoXY(0, 2);
	for (int i = 0; i < 13; i++)
	{
		if (count == 1 && i > 9)
	    	TextColor(7);
		else
			TextColor(color);
		printf("%s", pic.s[i]);
	}
}

void LoadIntro(Picture pic)
{
	system("cls");
	int count = 19;
	ReadPicture(pic);
	while (count > 0)
	{
		DisplayPicture(pic, count);
		count--;
		Sleep(100);
	}	
}

void DisplayArrow(Arrow ar)
{
	gotoXY(ar.x, ar.y);
	printf("%s", ar);
}

void OpenFile(FILE *&f, char *fileName, char *mode)
{
	errno_t err = fopen_s(&f, fileName, mode);
	if (err != 0)
		printf("The file was not opened\n");
}

void Delete(Shape hd)
{
	int x;
	int y;
	for (int kDong = -1; kDong <= 1; kDong++)
	{
		for (int kCot = -1; kCot <= 1; kCot++)
		{
			x = kCot + hd.toado.x;
			y = kDong + hd.toado.y;
			gotoXY(x, y);
			MatrixCell[y][x].chr = 32;
			putchar(MatrixCell[y][x].chr);
		}
	}
}

void Delete(Arrow ar)
{
	gotoXY(ar.x, ar.y);
	printf("  ");
}

void ControlsArrow(Arrow &ar)
{
	char c;
	while (1)
	{
		c = _getch();
		if (c == '\r')
			break;
		Delete(ar);
		switch (c)
		{
		case 'w':
		case 'W':
			if (ar.y > 12)
				ar.y -= 1;
			break;
		case 's':
		case 'S':
			if (ar.y < 14)
				ar.y += 1;
			break;
		}
		DisplayArrow(ar);
    }
}

void SetValueInCell(int kitu, int number, int row, int column)
{
	for (int i = 0; i < number; i++)
	{
		MatrixCell[row][column + i].chr = kitu;
		if (kitu == 42)
			MatrixCell[row][column + i].status = 1;
		if (kitu == 45)
			MatrixCell[row][column + i].status = 2;
	}
}

void ReadMap(FILE *&f, char *fileName, char *mode)
{
	int kitu;
	int number;

	OpenFile(f, fileName, mode);

	int row = 0;
	int column = 0;
	while (!feof(f))
	{
		fscanf_s(f, "%d", &kitu);
		fgetc(f);
		fscanf_s(f, "%d", &number);
		fgetc(f);

		SetValueInCell(kitu, number, row, column);

		if (fgetc(f) == '\n')
		{
			row++;
			column = 0;
		}
		else
			column += number;
	}
	fclose(f);
}

void ReadInformation(FILE *&f, Shape &player, Shape *&computer)
{
	int total;
	int score;
	int live;
	int map;
	int time;

	OpenFile(f, "data/save/SaveInformation.txt", "rt");

	while (!feof(f))
	{
		fscanf_s(f, "%d", &total);
		fgetc(f);
		fscanf_s(f, "%d", &score);
		fgetc(f);
		fscanf_s(f, "%d", &live);
		fgetc(f);
		fscanf_s(f, "%d", &map);
		fgetc(f);
		fscanf_s(f, "%d", &time);
		fgetc(f);

		player.totalScore = total;
		player.score = score;
		player.live = live;
		player.map = map;

		for (int i = 0; i < numberCar; i++)
	    	computer[i].map = map;
		player.time = time;
	}
	fclose(f);
}


void ReadPos(FILE *&f, Shape &player, Shape *&computer)
{
	int x;
	int y;
	int count = 1;
	int i = 0;

	OpenFile(f, "data/save/SavePos.txt", "rt");

	while (!feof(f))
	{
		fscanf_s(f, "%d", &x);
		fgetc(f);
		fscanf_s(f, "%d", &y);
		fgetc(f);

		if (count == 1)
		{
			player.toado.x = x;
			player.toado.y = y;
			count++;
		}
		else
		{
				computer[i].toado.x = x;
				computer[i].toado.y = y;
				i++;
		}
	

		if (fgetc(f) == '\n')
			continue;
	}
	fclose(f);
}

void ReadDirection(FILE *&f, Shape &player, Shape *&computer)
{
	int direction;
	int count = 1;
	int i = 0;

	OpenFile(f, "data/save/SaveDirection.txt", "rt");

	while (!feof(f))
	{
		fscanf_s(f, "%d", &direction);
		fgetc(f);

		if (count == 1)
		{
			player.direction = direction;
			count++;
		}
		else
		{
			computer[i].direction = direction;
			i++;
		}


		if (fgetc(f) == '\n')
			continue;
	}
	fclose(f);
}

void LoadMap()
{
	for (int i = 0; i < consoleH; i++)
	{
		for (int j = 0; j < consoleW; j++)
			putchar(MatrixCell[i][j].chr);		
		printf("\n");
	}
}

void TotalScore(Shape &player) 
{
	for (int i = 0; i < consoleH; i++)
	{
		for (int j = 0; j < consoleW; j++)
		{
			if (MatrixCell[i][j].chr == 42)
				player.totalScore += 10;
		}
	}
}

void LoadInstructionScreen(Shape &player, int flag)
{
	gotoXY(88, 0);
	printf("%c", 201);
	for (int i = 0; i < 25; i++)
		printf("%c", 205);
	printf("%c", 187);

	for (int i = 1; i < 20; i++)
	{
		gotoXY(88, i);
		printf("%c", 186);
	}
	
	gotoXY(88, 20);
	printf("%c", 200);
	for (int i = 0; i < 25; i++)
		printf("%c", 205);

	gotoXY(114, 20);
	printf("%c", 188);
	for (int i = 1; i < 20; i++)
	{
		gotoXY(114, i);
		printf("%c", 186);
	}

	gotoXY(88, 10);
	printf("%c", 204);
	for (int i = 0; i < 25; i++)
		printf("%c", 205);
	printf("%c", 185);

	gotoXY(96, 1);
	TextColor(9);
	printf("INFORMATION");

	TextColor(7);
	gotoXY(96, 3);
	printf("High Scores");

	gotoXY(100, 4);
	if (flag == 1)
    	TotalScore(player);
	printf(" %c %d", 92, player.totalScore);

	gotoXY(96, 11);
	TextColor(1);
	printf("GUIDE PLAY");

	TextColor(7);
	gotoXY(94, 12);
	printf("W: Go To Top");

	gotoXY(94, 13);
	printf("S: Go To Bottom");

	gotoXY(94, 14);
	printf("A: Go To Left");

	gotoXY(94, 15);
	printf("D: Go To Right");

	gotoXY(94, 16);
	printf("T: Save Game (Soon)");

	gotoXY(94, 17);
	printf("P: Pause");

	gotoXY(94, 18);
	printf("ESC: Exit");
}

void CreateMap(FILE *&f, Shape &player, char *file, int flag)
{
	ReadMap(f, file, "rt");
	LoadMap();
	LoadInstructionScreen(player, flag);
}

void Create(Shape &hd)
{
#define pHD hd.car
#define pTD hd.toado

	for (int i = 0; i < shapeH; i++)
		pHD[i] = new char[shapeW];

	pHD[0][0] = 201;
	pHD[0][2] = 187;
	pHD[2][0] = 200;
	pHD[2][2] = 188;
	pHD[1][0] = 204;
	pHD[1][2] = 185;
	pHD[1][1] = 206;
	switch (hd.direction)
	{
	case 1:
		pHD[0][1] = 203;
		pHD[2][1] = 202;
		pHD[1][2] = 16;
		break;
	case 2:
		pHD[0][1] = 203;
		pHD[2][1] = 202;
		pHD[1][0] = 17;
		break;
	case 3:
		pHD[0][1] = 30;
		pHD[2][1] = 202;
		break;
	case 4:
		pHD[0][1] = 203;
		pHD[2][1] = 31;
		break;
	}
}

void Display(Shape hd, int color)
{
	int x;
	int y;
	for (int kDong = -1; kDong <= 1; kDong++) 
	{
		for (int kCot = -1; kCot <= 1; kCot++) 
		{
			x = kCot + hd.toado.x;
			y = kDong + hd.toado.y;
			gotoXY(x, y);
			Init(y, x, hd.car[kDong + 1][kCot + 1]);
		   	TextColor(color);
			putchar(MatrixCell[y][x].chr);
		}
	}
}

int isOutOfBoundTop(Shape hd, int chr)
{
	if (MatrixCell[hd.toado.y - 2][hd.toado.x].chr == chr
		|| MatrixCell[hd.toado.y - 2][hd.toado.x - 1].chr == chr
		|| MatrixCell[hd.toado.y - 2][hd.toado.x + 1].chr == chr)
		return 1;
	return 0;
}

int isOutOfBoundBottom(Shape hd, int chr)
{
	if (MatrixCell[hd.toado.y + 2][hd.toado.x].chr == chr
		|| MatrixCell[hd.toado.y + 2][hd.toado.x - 1].chr == chr
		|| MatrixCell[hd.toado.y + 2][hd.toado.x + 1].chr == chr)
		return 1;
	return 0;
}

int isOutOfBoundRight(Shape hd, int chr)
{
	// o^o  hd.toado.y - 1, hd.toado.x + 2
	// | |  hd.toado.y, hd.toado.x + 2
	// o-o  hd.toado.y + 1, hd.toado.x + 2
	if (MatrixCell[hd.toado.y][hd.toado.x + 2].chr == chr
		|| MatrixCell[hd.toado.y - 1][hd.toado.x + 2].chr == chr
		|| MatrixCell[hd.toado.y + 1][hd.toado.x + 2].chr == chr)
		return 1;
	return 0;
}

int isOutOfBoundLeft(Shape hd, int chr)
{
	if (MatrixCell[hd.toado.y][hd.toado.x - 2].chr == chr
		|| MatrixCell[hd.toado.y - 1][hd.toado.x - 2].chr == chr
		|| MatrixCell[hd.toado.y + 1][hd.toado.x - 2].chr == chr)
		return 1;
	return 0;
}

int getOutRight(Shape &hd)
{
	if (hd.toado.y == 10 && hd.toado.x + 2 >= 85
		|| hd.toado.y == 10 && hd.toado.x + 2 >= 85
		|| hd.toado.y == 10 && hd.toado.x + 2 >= 85)
	{
		hd.toado.y = 10;
		hd.toado.x = 1;	
		return 0;
	}

	else if (hd.toado.y == 26 && hd.toado.x + 2 >= 85
		|| hd.toado.y == 26 && hd.toado.x + 2 >= 85
		|| hd.toado.y == 26 && hd.toado.x + 2 >= 85)
	{
		hd.toado.y = 26;
		hd.toado.x = 1;
		return 0;
	}
	return 1;
}

int getOutLeft(Shape &hd)
{
	if (hd.toado.y == 10 && hd.toado.x - 2 < 0
		|| hd.toado.y == 10 && hd.toado.x - 2 < 0
		|| hd.toado.y == 10 && hd.toado.x - 2 < 0)
	{
		hd.toado.y = 10;
		hd.toado.x = 83;	
		return 0;
	}

	else if (hd.toado.y == 26 && hd.toado.x - 2 < 0
		|| hd.toado.y == 26 && hd.toado.x - 2 < 0
		|| hd.toado.y == 26 && hd.toado.x - 2 < 0)
	{
		hd.toado.y = 26;
		hd.toado.x = 83;
		return 0;
	}
	return 1;
}

int getOutRightCom(Shape &hd)
{
	if (hd.toado.y > 10 && hd.toado.x + 2 >= 82
		|| hd.toado.y - 1 > 10 && hd.toado.x + 2 >= 82
		|| hd.toado.y + 1 > 10 && hd.toado.x + 2 >= 82)
		return 0;
	else if (hd.toado.y > 26 && hd.toado.x + 2 >= 81
		|| hd.toado.y - 1 > 26 && hd.toado.x + 2 >= 81
		|| hd.toado.y + 1 > 26 && hd.toado.x + 2 >= 81)
		return 0;
	return 1;
}

int getOutLeftCom(Shape &hd)
{
	if (hd.toado.y > 10 && hd.toado.x - 2 < 3
		|| hd.toado.y - 1 > 10 && hd.toado.x - 2 < 3
		|| hd.toado.y + 1 > 10 && hd.toado.x - 2 < 3)
		return 0;
	else if (hd.toado.y > 26 && hd.toado.x - 2 < 3
		|| hd.toado.y - 1 > 26 && hd.toado.x - 2 < 3
		|| hd.toado.y + 1 > 26 && hd.toado.x - 2 < 3)
		return 0;
	return 1;
}

// kiểm tra va chạm giữa 2 ma trận hình chữ nhật
int isIntersect(Shape *com, Shape player)
{
	for (int i = 0; i < numberCar; i++)
	{
		if (com[i].toado.x < player.toado.x + shapeW && com[i].toado.x + shapeW > player.toado.x
			&& com[i].toado.y < player.toado.y + shapeH && com[i].toado.y + shapeH > player.toado.y)
			return 1;
	}
	return 0;
}

void checkDirection(Shape &hd, int *&temp, int &value)
{
	value = 0;
	if (!isOutOfBoundTop(hd, 219))
	{
		if (value > 0)
			value *= 10;
		value += 3;
		temp[3] = 3;
	}

	if (!isOutOfBoundBottom(hd, 219))
	{
		if (value > 0)
			value *= 10;
		value += 4;
		temp[4] = 4;
	}

	if (!isOutOfBoundRight(hd, 219))
	{
		if (value > 0)
			value *= 10;
		value += 1;
		temp[1] = 1;
	}

	if (!isOutOfBoundLeft(hd, 219))
	{
		if (value > 0)
			value *= 10;
		value += 2;
		temp[2] = 2;
	}
}

int getDirection(int *temp)
{
	int n ;
	do
	{
		n = (rand() % 4) + 1;
	} while (temp[n] < 1 || temp[n] > 4);
	return n;
}

void DisplayComputer(Shape *computer)
{
	for (int i = 0; i < numberCar; i++)
	{
		if (i > 5)
			Display(computer[i], i + 2);
		else
			Display(computer[i], i + 1);
	}
    	
}

void DisplayLives(Shape hd, int color, int x)
{
	int y = 6;
	for (int kDong = -1; kDong <= 1; kDong++)
	{
		for (int kCot = -1; kCot <= 1; kCot++)
		{
			TextColor(color);
			putchar(hd.car[kDong + 1][kCot + 1]);		
		}
		y++;
		gotoXY(x, y);
	}
}

void DisplayLives(Shape player)
{
	int x = 96;
	player.direction = 3;
	Create(player);
	for (int i = 0; i < player.live; i++)
	{
		gotoXY(x, 6);
		DisplayLives(player, 7, x);
		x += 4;
	}
}

void DeleteLives()
{
	for (int x = 96; x <= 106; x++)
	{
		for (int y = 6; y <= 8; y++)
		{
			gotoXY(x, y);
			printf(" ");
		}
	}
}

void AllocateNew(Shape &player, Shape *&computer, Picture &pic, Arrow &ar, FILE *&f)
{
	if (player.live == 0)
	{
		player.score = 0;
		player.live = 3;
	}  

	if (player.live == 3)
	{
		system("cls");
		if (player.map == 1)
	    	CreateMap(f, player, "data/Map1.txt", 1);
		if (player.map == 2)
			CreateMap(f, player, "data/Map2.txt", 1);

		if (player.map == 2)
		{
			numberCar = 4;
			player.time = 15;
		}			
		else
		{
			numberCar = 4;
			player.time = 20;
		}
		
		for (int i = 0; i < numberCar; i++)
		{
			if (i > 3 && player.map == 2)
			{
				computer[i].toado.y = 26;
				computer[i].toado.x = 42;
			}
			else
			{
				computer[i].toado.y = 10;
				computer[i].toado.x = 42;
			}
			
			computer[i].direction = 1;

			Create(computer[i]);
		}
	}

	if (player.live < 3)
	{
		Delete(player);

		for (int i = 0; i < numberCar; i++)
			Delete(computer[i]);

		DeleteLives();
	}

	if (player.map == 1)
	{
		player.direction = 1;
		player.toado.y = 10;
		player.toado.x = 1;
	}
	else
	{
		player.direction = 1;
		player.toado.y = 34;
		player.toado.x = 43;
	}
	
	Create(player);
	DisplayLives(player);
}

void AllocateSave(Shape &player, Shape *&computer, Picture &pic, Arrow &ar, FILE *&f)
{
	if (player.live == 0)
	{
		player.score = 0;
		player.live = 3;
	}

	if (player.live < 3)
	{
		Delete(player);

		for (int i = 0; i < numberCar; i++)
			Delete(computer[i]);

		DeleteLives();
	}


	if (player.live == 3)
	{
		player.totalScore = 0;

		ReadInformation(f, player, computer);
		ReadPos(f, player, computer);
		ReadDirection(f, player, computer);

		if (player.map == 2)
		{
			numberCar = 8;
			player.time = 10;
		}

		else
		{
			numberCar = 4;
			player.time = 25;
		}

		for (int i = 0; i < numberCar; i++)
		{
			Create(computer[i]);
		}
	}

	if (player.save == 1 || player.live == 3)
	{
		system("cls");
		CreateMap(f, player, "data/save/SaveMap.txt", 0);
	}

	DisplayLives(player);

}

int ExitSelected(Arrow &ar, Picture &pic, Shape &player, Shape *&computer, FILE *&f, char ch)
{
	ch = _getch();
	if (ch == 27)
		selectedPointer(ar, pic, player, computer, f);
	if (ch == 49 || ch == 80 || ch == 112)
		return 0;
}

void goCom(Shape &computer)
{
	int direction;
	int value;
	int *temp = new int[4];

	checkDirection(computer, temp, value);
	direction = getDirection(temp);

	switch (direction)
	{
	case 1:
		if (!isOutOfBoundRight(computer, 219))
		{
			if (getOutRightCom(computer))
			{
				computer.direction = 1;
				computer.toado.x += 1;
			}	
		}
		break;
	case 2:
		if (!isOutOfBoundLeft(computer, 219))
		{
			if (getOutLeftCom(computer))
			{
				computer.direction = 2;
				computer.toado.x -= 1;
			}
		}
		break;	
	case 3:
		if (!isOutOfBoundTop(computer, 219))
		{
			computer.direction = 3;
			computer.toado.y -= 1;
		}
		break;
	case 4:
		if (!isOutOfBoundBottom(computer, 219) 
			&& MatrixCell[computer.toado.y + 2][computer.toado.x].status != 2)
		{
			computer.direction = 4;
			computer.toado.y += 1;
		}
		break;
	}
}

void AutoCom(Shape &computer)
{
	int *temp = new int[4];
	int value;
	checkDirection(computer, temp, value);
	Delete(computer);

	if (computer.direction == 1)
	{
		if (isOutOfBoundRight(computer, 219) || value > 100)
			goCom(computer);
		else
		{
			if (getOutRightCom(computer))
				computer.toado.x += 1;
		}
			
		
		if (MatrixCell[computer.toado.y][computer.toado.x - 2].status == 1)
		{
			MatrixCell[computer.toado.y][computer.toado.x - 2].chr = 42;
			gotoXY(computer.toado.x - 2, computer.toado.y);
			TextColor(7);
			putchar(MatrixCell[computer.toado.y][computer.toado.x - 2].chr);
		}
	}
	else if (computer.direction == 2)
	{
		if (isOutOfBoundLeft(computer, 219) || value > 100)
			goCom(computer);
		else
		{
			if (getOutLeftCom(computer))
				computer.toado.x -= 1;
		}
			
		if (MatrixCell[computer.toado.y][computer.toado.x + 2].status == 1)
		{
			MatrixCell[computer.toado.y][computer.toado.x + 2].chr = 42;
			gotoXY(computer.toado.x + 2, computer.toado.y);
			TextColor(7);
			putchar(MatrixCell[computer.toado.y][computer.toado.x + 2].chr);
		}
	}

	else if (computer.direction == 3)
	{
		if (isOutOfBoundTop(computer, 219) || value > 100)
			goCom(computer);
		else
			computer.toado.y -= 1;

		if (MatrixCell[computer.toado.y + 2][computer.toado.x].status == 1)
		{
			MatrixCell[computer.toado.y + 2][computer.toado.x].chr = 42;
			gotoXY(computer.toado.x, computer.toado.y + 2);
			TextColor(7);
			putchar(MatrixCell[computer.toado.y + 2][computer.toado.x].chr);
		}

		if (MatrixCell[computer.toado.y + 2][computer.toado.x - 1].status == 2
			&& MatrixCell[computer.toado.y + 2][computer.toado.x - 1].chr == 32)
		{
			int n = computer.toado.x - 1;
			int m = computer.toado.x - 1 + 2;
			for (int i = n ; n <= m; n++)
				MatrixCell[computer.toado.y + 2][n].chr = 45;
			
				gotoXY(computer.toado.x - 1, computer.toado.y + 2);
				TextColor(7);
				printf("---");
		}
	}

	else if (computer.direction == 4)
	{
		if (isOutOfBoundBottom(computer, 219) || value > 100)
			goCom(computer);
		else
			computer.toado.y += 1;
		if (MatrixCell[computer.toado.y - 2][computer.toado.x].status == 1)
		{
			MatrixCell[computer.toado.y - 2][computer.toado.x].chr = 42;
			gotoXY(computer.toado.x, computer.toado.y - 2);
			TextColor(7);
			putchar(MatrixCell[computer.toado.y - 2][computer.toado.x].chr);
		}
	}
}

void changeStatus(Shape &player, int direction)
{
	switch (direction)
	{
	case 1:
		if (MatrixCell[player.toado.y][player.toado.x].status == 1)
		{
			MatrixCell[player.toado.y][player.toado.x].status = 0;
			player.score += 10;
		}		
		break;
	case 2:
		if (MatrixCell[player.toado.y][player.toado.x].status == 1)
		{
			MatrixCell[player.toado.y][player.toado.x].status = 0;
			player.score += 10;
		}			
		break;
	case 3:
		if (MatrixCell[player.toado.y][player.toado.x].status == 1)
		{
			MatrixCell[player.toado.y][player.toado.x].status = 0;
			player.score += 10;
		}			
		break;
	case 4:
		if (MatrixCell[player.toado.y][player.toado.x].status == 1)
		{
			MatrixCell[player.toado.y][player.toado.x].status = 0;
			player.score += 10;
		}			
		break;
	}
}

void DisplayScore(Shape &player)
{
	gotoXY(96, 4);
	printf("    ");
	gotoXY(96, 4);
	printf("%d", player.score);
}

void AutoControlPlayer(Shape &player)
{
	int *temp = new int[4];
	int value;
	checkDirection(player, temp, value);
	Delete(player);

	if (player.direction == 1)
	{
		if (isOutOfBoundRight(player, 219) || value > 100)
		{
			if (!isOutOfBoundRight(player, 219))
				player.toado.x += 1;
		}		
		else
			if (getOutRight(player))
				player.toado.x += 1;	
	}
	else if (player.direction == 2)
	{
		if (isOutOfBoundLeft(player, 219) || value > 100)
		{
			if (!isOutOfBoundLeft(player, 219))
				player.toado.x -= 1;
		}		
		else
			if (getOutLeft(player))
				player.toado.x -= 1;	
	}

	else if (player.direction == 3)
	{
		if (isOutOfBoundTop(player, 219) || value > 100)
		{
			if (!isOutOfBoundTop(player, 219))
				player.toado.y -= 1;
		}			
		else
			player.toado.y -= 1;
	}

	else if (player.direction == 4)
	{
		if (isOutOfBoundBottom(player, 219) || value > 100)
		{
			if (!isOutOfBoundBottom(player, 219))
				player.toado.y += 1;
		}		
		else
			player.toado.y += 1;
	}
}

void goPlayer(Shape &player, int &flags, int &save, char &exit)
{
	if (_kbhit())
	{
		char c = _getch();
		Delete(player);
		switch (c)
		{
		case 'd':
		case 'D':
			if (!isOutOfBoundRight(player, 219))
			{
				player.direction = 1;
				player.toado.x += 1;
			}
			break;
		case 'a':
		case 'A':
			if (!isOutOfBoundLeft(player, 219))
			{
				player.direction = 2;
				player.toado.x -= 1;
			}
			break;
		case 'w':
		case 'W':
			if (!isOutOfBoundTop(player, 219))
			{
				player.direction = 3;
				player.toado.y -= 1;
			}
			break;
		case 's':
		case 'S':
			if (!isOutOfBoundBottom(player, 219)
				&& MatrixCell[player.toado.y + 2][player.toado.x].status != 2)
			{
				player.direction = 4;
				player.toado.y += 1;
			}
			break;
		case 'p':
		case 'P':
			flags = 0;
			break;
		case 't':
		case 'T':
			save = 1;
			break;
		case 27:
			exit = 27;
			break;
		}
	}
	else
		AutoControlPlayer(player);
	changeStatus(player, player.direction);
}

void editStatus(Shape &player)
{
	for (int i = 0; i < consoleH; i++)
	{
		for (int j = 0; j < consoleW; j++)
		{
			if (MatrixCell[i][j].chr == 32)
			{
				if (MatrixCell[i][j].status == 1)
				{
					player.score += 10;
					MatrixCell[i][j].status = 0;
					DisplayScore(player);
				}
			}
		}
	}
}

void WriteMap(FILE *f)
{
	char *fileNameCate = "data/save/SaveMap.txt";
	// Code here ĐỌC TẬP TIN, CHẾ ĐỘ GHI TẬP TIN LÀ WT (WRITE TEXT)
	OpenFile(f, fileNameCate, "wt");
	
	int values;
	int count = 1;
	int n = consoleH - 1;

	for (int i = 0; i < n; i++)
	{
		values = MatrixCell[i][0].chr;
		count = 1;
		for (int j = 1; j < consoleW; j++)
		{
			if (MatrixCell[i][j].chr < 0 
				|| MatrixCell[i][j].chr == 16
				|| MatrixCell[i][j].chr == 17
				|| MatrixCell[i][j].chr == 30
				|| MatrixCell[i][j].chr == 31)
			{
				if (MatrixCell[i][j].status == 1)
					MatrixCell[i][j].chr = 42;
				else
					MatrixCell[i][j].chr = 32;
			}
			if (values == MatrixCell[i][j].chr)
				count++;

			if (values != MatrixCell[i][j].chr || j + 1 == consoleW)
			{
				fprintf_s(f, "%d:%d:", values, count);

				values = MatrixCell[i][j].chr;
				count = 1;
				if (j + 1 < consoleW)
					fprintf_s(f, " ");
			}
		}	
		fprintf_s(f, "\n");
	}
	fclose(f);
}

void WriteInformation(FILE *f, Shape player)
{
	char *fileNameCate = "data/save/SaveInformation.txt";
	// Code here ĐỌC TẬP TIN, CHẾ ĐỘ GHI TẬP TIN LÀ WT (WRITE TEXT)
	OpenFile(f, fileNameCate, "wt");

	fprintf_s(f, "%d:%d:%d:%d:%d", player.totalScore, player.score, player.live, player.map, player.time);
	
	fclose(f);
}

void WritePos(FILE *f, Shape player, Shape *computer)
{
	char *fileNameCate = "data/save/SavePos.txt";
	// Code here ĐỌC TẬP TIN, CHẾ ĐỘ GHI TẬP TIN LÀ WT (WRITE TEXT)
	OpenFile(f, fileNameCate, "wt");

	fprintf_s(f, "%d:%d:", player.toado.x, player.toado.y);
	fprintf_s(f, "\n");

	for (int i = 0; i < numberCar; i++)
	{
		fprintf_s(f, "%d:%d:", computer[i].toado.x, computer[i].toado.y);
		if (i + 1 < numberCar)
	    	fprintf_s(f, "\n");
	}
	fclose(f);
}

void WriteDirection(FILE *f, Shape player, Shape *computer)
{
	char *fileNameCate = "data/save/SaveDirection .txt";
	// Code here ĐỌC TẬP TIN, CHẾ ĐỘ GHI TẬP TIN LÀ WT (WRITE TEXT)
	OpenFile(f, fileNameCate, "wt");

	fprintf_s(f, "%d:", player.direction);
	fprintf_s(f, "\n");

	for (int i = 0; i < numberCar; i++)
	{
		fprintf_s(f, "%d:", computer[i].direction);
		if (i + 1 < 4)
			fprintf_s(f, "\n");
	}
	fclose(f);
}

void DisplayWin()
{
	char *temp = "PLAYER WIN";
	int color;
	for (int i = 0; i < 7; i++)
	{
		color = (rand() % 14) + 2;
		gotoXY(38, 10);
		TextColor(color);
		puts(temp);
		TextColor(7);
		Sleep(150);
	}
	
}

void Play(Shape &player, Shape *&computer, Picture &pic, Arrow &ar, FILE *&f, int saves)
{
	if (saves == 0)
		AllocateNew(player, computer, pic, ar, f);
	if (saves == 1)
		AllocateSave(player, computer, pic, ar, f);
	int isFirstTime = 0;
	int flag = 0;
	char c = 1;
	char chr;
	int flags = 1;
	char exit = 0;
	int save = 0;

	DisplayComputer(computer);
	Display(player, 7);

	do
	{
		while (1)
		{
			save = 0;
			flags = 1;
			for (int i = 0; i < numberCar; i++)
			{
				AutoCom(computer[i]);
				Create(computer[i]);
				DisplayComputer(computer);

				if (flag = isIntersect(computer, player))
					break;
				Sleep(player.time);
			}

			goPlayer(player, flags, save, exit);
			Create(player);
			Display(player, 7);
			DisplayScore(player);
			editStatus(player);

			if (flag == 1)
			{
				player.live--;
				if (player.live == 0)
				{
					DeleteLives();
					char *temp = "GAME OVER";
					gotoXY(38, 10);
					TextColor(4);
					puts(temp);
					TextColor(7);
					while (flags)
					{
						c = _getch();
						if (c == 27)
							flags = 0;
					}
				}
				else
				{
					DisplayLives(player);
					playPointer(player, computer, pic, ar, f, saves);
				}
			}

			if (save == 1)
			{
				char *temp = "SAVE SUCCESS";
				gotoXY(37, 10);
				TextColor(12);
				puts(temp);
				TextColor(7);
				player.save = 1;
				WriteMap(f);
				WriteInformation(f, player);
				WritePos(f, player, computer);
				WriteDirection(f, player, computer);
			}

			if (player.totalScore == 1900)
			{
				if (player.map == 1)
				{
					DisplayWin();
					player.totalScore = 0;
					player.score = 0;
					player.live = 3;
					player.map = 2;
					playPointer(player, computer, pic, ar, f, saves);
				}
				if (player.map == 2)
				{

					char *temp = "PLAYER WIN";
					gotoXY(38, 10);
					TextColor(1);
					puts(temp);
					TextColor(7);
					while (flags)
					{
						c = _getch();
						if (c == 27)
							flags = 0;
					}
				}	
			}

			if (flags == 0 || exit == 27)
			{
				if (exit == 27)
					selectedPointer(ar, pic, player, computer, f);
				break;
			}
		}
	} while (ExitSelected(ar, pic, player, computer, f, c) == 0);
}

int checkSelect(Arrow &ar)
{
	if (ar.y == 12)
		return 1;
	if (ar.y == 13)
		return 2;
	if (ar.y == 14)
		return 3;
}

void SelectedView(Arrow &ar, Picture &pic, Shape &player, Shape *&computer, FILE *&f)
{
	Allocate();
	LoadIntro(pic);
	DisplayArrow(ar);
	ControlsArrow(ar);

	switch (checkSelect(ar))
	{
	case 1:
		Play(player, computer, pic, ar, f, 0);
		break;
	case 2:	
		Play(player, computer, pic, ar, f, 1);
		break;
	case 3:
		exit(0);
		break;
	}
}

int main()
{
	FILE *f;
	Picture pic;
	Arrow ar;
	Shape player;
	Shape *computer = new Shape[8];

	selectedPointer = SelectedView;
	playPointer = Play;

	SetConsoleCP(437);
	SetConsoleOutputCP(437);
	SelectedView(ar, pic, player, computer, f);

    _getch();
}