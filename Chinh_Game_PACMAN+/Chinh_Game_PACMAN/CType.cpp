#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include "math.h"
#include "windows.h"
#include "time.h"
#include "CType.h"
#include "string.h"

const int rows = 7;
const int columns = 7;
const int Max = 20;
const int max = 50;

void DrawBorder(int n, char c)
{
	n = (n * 8) + 10;
	printf("\n");
	for (int i = 1; i < n; i++)
		printf("%c", c);
	printf("\n");
}

void InputNumber(int &n, int column, char c[])
{
	do
	{
		printf("Number %s: ", c);
		scanf_s("%d", &n);
		DrawBorder(column, '=');
	} while (n < 1 || n > Max);
}

void Inputpos(int n, int &row, int column, char c[])
{
	do
	{
		printf("pos Number %s: ", c);
		scanf_s("%d", &row);
		row--;
		DrawBorder(column, '=');
	} while (row < 0 || row > n);
}


void Input(int arr[], int n, int column, char c)
{
	for (int i = 0; i < n; i++)
	{
		printf("arr%c[%d]: ", c, i);
		scanf_s("%d", &arr[i]);
	}
	DrawBorder(column, '=');
}

void InputValue(int &n)
{
	printf("Value:");
	scanf_s("%d", &n);
}

void Inputposition(int n, int &pos)
{
	do {
		printf("position:");
		scanf_s("%d", &pos);
	} while (pos < 0 || pos > n);
	pos = pos - 1;
}

void Input(int &n, int column, char c[])
{
	printf("Number %s: ", c);
	scanf_s("%d", &n);
	DrawBorder(column, '=');
	if (n < 5)
		do
		{
			printf("Number %s (Number > 5): ", c);
			scanf_s("%d", &n);
			DrawBorder(column, '=');
		} while (n < 5 || n > Max);
}

void Output(int arr[], int n, char c)
{
	printf("Arr%c = [", c);
	for (int i = 0; i < n; i++)
	{
		printf("%d", arr[i]);
		if (i == n - 1)
			printf("]");
		else
			printf("\t");
	}
}

int isOdd(int n)
{
	if (n % 2 != 0)
		return 1;
	return 0;
}

int isPrime(int n)
{
	if (n < 2)
		return 0;
	else
		for (int idx = 2; idx < sqrt(n); idx++)
			if (n % idx == 0)
				return 0;
	return 1;
}

// thêm phần tử value tại vị trí pos trong mảng a
void InsertAt(int arr[], int &n, int value, int pos)
{
	// arr: 1 2 3
	// value: 100, pos = 1
	// arr[3] = arr[2] = 3
	// arr[2] = arr[1] = 2

	// arr[pos] = value
	// 1 100 2 3 --> n = 4
	for (int i = n - 1; i >= pos; i--)
		arr[i + 1] = arr[i];
	n++;
	arr[pos] = value;
}

// xóa phần tử trong mảng a tại vị trí pos
void Delete(int arr[], int &n, int pos)
{
	for (int i = pos; i < n; i++)
	{
		arr[i] = arr[i + 1];
	}
	n--;
}

void InitializeValuesArray(int arr[], int &n, int value)
{
	arr[n] = value;
	n++;
}

int isExist(int arr[], int n, int value)
{
	for (int i = 0; i < n; i++)
	{
		if (arr[i] == value)
			return 1;
	}
	return 0;
}

void Swap(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}

void InterChange(int arr[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (arr[i] > arr[j])
				Swap(arr[i], arr[j]);
		}
		Output(arr, n, 'A');
	}
}

void SelectionSort(int arr[], int n)
{
	int posMin; // vị trí của phần tử nhỏ nhất(trong phần chưa sắp)
	for (int i = 0; i < n - 1; i++)
	{
		// tìm phần tử nhỏ nhất trong phần chưa sắp
		posMin = i;

		for (int j = i + 1; j < n; j++)
		{
			if (arr[j] < arr[posMin])
				//ghi nhận lại vị trí
				posMin = j;
		}
		// Swap element min được tìm thấy với phần tử đầu.
		Swap(arr[posMin], arr[i]);
	}
}

void BubbleSort(int arr[], int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = n - 1; j > i; j--)
		{
			if (arr[j] < arr[j - 1])
				Swap(arr[j], arr[j - 1]);
		}
	}
}

void Insert(int arr[], int pos, int value)
{
	while (arr[pos] > value)
	{
		arr[pos + 1] = arr[pos];
		pos--;
	}
	arr[pos + 1] = value;
}


void InsertionSort(int arr[], int n)
{
	int pos;
	// lưu giá trị a[i] tránh bị ghi đè khi dời chổ các phần tử  
	int value;

	for (int i = 1; i < n; i++)
	{
		value = arr[i];
		pos = i - 1;
		Insert(arr, pos, value);
	}
}

// Tìm kiếm nhị phân
int SearchBinary(int a[], int n, int value)
{
	int left = 0;
	int right = n - 1;
	int mid;
	while (left <= right)
	{
		mid = (left + right) / 2;

		if (a[mid] == value)
			return mid;
		else if (a[mid] > value)
			right = mid - 1;
		else
			left = mid + 1;
	}
	return -1;
}

// 2 mảng
void Input2D(int arr[][columns], int row, int column)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			printf("a[%d][%d]: ", i, j);
			scanf_s("%d", &arr[i][j]);
		}
	}
}

void Output2D(int arr[][columns], int row, int column)
{
	DrawBorder(column, '=');
	if (row == column)
		printf("Block Square:\n");

	else
		printf("Block Rectangle:\n");
	printf("\t");
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
			printf("\t%d", arr[i][j]);
		printf("\n\n\n");
		printf("\t");
	}
	printf("\n");
}

void InputRandom(int a[][columns], int &dong, int &cot)
{
	// Nhập biến dong và biến cot như bình thường
	srand((unsigned int)time(0));
	for (int i = 0; i < dong; i++)
	{
		for (int j = 0; j < cot; j++)
		{
			// (rand() % [b – a + 1]) + a
			a[i][j] = (rand() % 601) + 0;
		}
	}
}

// trường hợp đặc biệt: matrix square
// for (int i = 0; i < dong * cot; i++) 
// {
//		// a[i/cot][i%cot]
// }

void SelectInput(int arr2D[][columns], int &row, int &column, int flag)
{
	do
	{
		printf("Select the input method:\n");
		printf("1. User input!");
		printf("\n");
		printf("2. Random input!");
		DrawBorder(column, '=');

		scanf_s("%d", &flag);

	} while (flag > 2 || flag < 1);

	if (flag == 1)
		Input2D(arr2D, row, column);
	DrawBorder(column, '=');
	if (flag == 2)
		InputRandom(arr2D, row, column);
	DrawBorder(column, '=');
}

void SET_COLOR(int color)
{
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
}

void Draw(char c)
{
	printf("%c", c);
}


void NumberColumn(int n, int row)
{
	if (row == -1)
	{
		for (int idxA = 0; idxA < n; idxA++)
			printf("\t[%d]", idxA + 1);

		printf("\t");
	}
}

void NumberRow(int n)
{
	printf("[%d]", n + 1);
	printf("\t");
}


void InputNumberBlock(int &n, char c[])
{
	do
	{
		printf("Number %s Block (> 4): ", c);
		scanf_s("%d", &n);
	} while (n < 5);
}

void InputArrPointer(int *&arr, int n, int column)
{
	arr = new int[n];
	for (int i = 0; i < n; i++)
	{
		printf("Number Value of Block %d:", i + 1);
		scanf_s("%d", &arr[i]);
	}
	DrawBorder(column, '=');
}

void InputArrPointerV2(int *&arr, int n)
{
	arr = new int[n];
	do
	{
		printf("Number Column Block (> 4):");
		scanf_s("%d", &arr[0]);
	} while (n < 5);

	for (int i = 1; i < n; i++)
		arr[i] = arr[0];
}


void InputRandomPointer(int **&arr, int &row, int column, int *list)
{
	// Nhập biến dong và biến cot như bình thường
	srand((unsigned int)time(0));
	// cấp phát mảng con trỏ vùng HEAP
	arr = new int *[row];
	for (int idxA = 0; idxA < row; idxA++)
	{
		// mỗi con trỏ cấp phát 1 vùng nhớ Heap
		arr[idxA] = new int[list[idxA]];
		for (int idxB = 0; idxB < list[idxA]; idxB++)
		{
			arr[idxA][idxB] = (rand() % 601) + 0;
		}
	}
	DrawBorder(column, '=');
}

void InputPointer(int **&arr, int row, int column, int *list)
{
	// cấp phát mảng con trỏ vùng HEAP
	arr = new int *[row];
	for (int idxA = 0; idxA < row; idxA++)
	{
		// mỗi con trỏ cấp phát 1 vùng nhớ Heap
		arr[idxA] = new int[list[idxA]];
		for (int idxB = 0; idxB < list[idxA]; idxB++)
		{
			printf("a[%d][%d]:", idxA, idxB);
			scanf_s("%d", &arr[idxA][idxB]);
		}
	}
	DrawBorder(column, '=');
}

void SelectInputPointer(int **&arr, int &row, int column, int *list, int flag)
{
	do
	{
		printf("Select the input method:\n");
		printf("1. User input!");
		printf("\n");
		printf("2. Random input!");
		DrawBorder(column, '=');

		scanf_s("%d", &flag);

	} while (flag > 2 || flag < 1);

	if (flag == 1)
		InputPointer(arr, row, column, list);
	DrawBorder(column, '=');
	if (flag == 2)
		InputRandomPointer(arr, row, column, list);
	DrawBorder(column, '=');
}

void OutputPointer(char **arr, int row, int column, int *list)
{
	for (int idxA = -1; idxA < row; idxA++)
	{
		if (idxA == -1)
			NumberColumn(row, idxA);
		else
			NumberRow(idxA);
		for (int idxB = 0; idxB < list[idxA]; idxB++)
			printf("%d\t", arr[idxA][idxB]);
		printf("\n");
		printf("\n");
		printf("\n");
	}
	DrawBorder(column, '=');
}

void checkFlag(int flag)
{
	switch (flag)
	{
	case 1:
		system("cls");
		break;
	case 2:
		exit(0);
		break;
	}
}

void Menu(int &flag)
{
	do
	{
		printf("1. Test again!");
		printf("\n");
		printf("2. Quits!");
		printf("\n");
		scanf_s("%d", &flag);

	} while (flag != 1 && flag != 2);

	checkFlag(flag);
}

void Delete(char *dest, int &length, int pos, int value)
{
	for (int i = pos; i < length; i++)
		dest[i] = dest[i + 1];
	length--;
}

int isDigit(char c)
{
	if (c >= 48 && c <= 57)
		return 1; // là kí tự số
	return 0; // không phải kí tự số
}

int isCheck(char *source)
{
	for (int i = 0; i < strlen(source); i++)
	{
		if (isDigit(source[i]))
		{
			puts("Not found!");
			return 1;
		}
	}
	return 0;
}

// con trỏ hàm là tham số của hàm isCheck
int isCheck(char *source, int(*condition)(char c))
{
	for (int i = 0; i < strlen(source); i++) {
		if (condition(source[i]))
			return 1;
	}
	return 0;
}

void Output(char *name, int length, char c[])
{
	printf("%s: ", c);
	printf("'");
	printf("%s", name);
	printf("'");
}

int getLength(char *c)
{
	int count = 0;
	while (*c != '\0')
	{
		count++;
		c++;
	}
	return count;
}

int isLower(char c)
{
	if (c >= 97 && c <= 122)
		return 1;
	return 0;
}

int isUpper(char c)
{
	if (c >= 65 && c <= 90)
		return 1;
	return 0;
}

void ToUpper(char *&p)
{
	char *temp = p;
	while (*temp != '\0')
	{
		if (!isUpper(*temp))
			*temp = *temp - 32;
		temp++;
	}
}

int isMax(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

int Compare(char *str1, char *str2)
{
	int lengthStr1 = getLength(str1);
	int lengthStr2 = getLength(str2);

	int max = isMax(lengthStr1, lengthStr2);
	for (int i = 0; i < max; i++)
	{
		if (str1[i] > str2[i])
			return 1;
		else if (str1[i] < str2[i])
			return -1;
	}
	return 0;
}

char toLower(char &c)
{
	char ch = c;
	if (isUpper(c))
		ch = c + 32;
	return ch;
}

int CompareImprove(char *str1, char *str2) {
	int lengthStr1 = getLength(str1);
	int lengthStr2 = getLength(str2);

	int max = isMax(lengthStr1, lengthStr2);

	for (int i = 0; i < max; i++)
	{
		char ch1 = toLower(str1[i]);
		char ch2 = toLower(str2[i]);
		if (ch1 > ch2)
			return 1;
		else if (ch1 < ch2)
			return -1;
	}
	return 0;
}



void InputPass(char *&password, char c[])
{
	char ch;
	int idx = 0;
	password = new char[10];
	printf("Password: ");
	while (1)
	{
		ch = _getch();
		if (ch == '\r')
		{
			password[idx] = '\0';
			break;
		}

		if (ch == '\b')
		{
			if (idx == 0)
				continue;
			printf("\b");
			printf("%c", 32);
			printf("\b \b");

			idx--;
			password[idx] = '\0';

			continue;
		}

		printf("*");
		password[idx] = ch;
		idx++;
	}
}

