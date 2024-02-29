#pragma once
#pragma once
#include "stdafx.h"

const int column = 7;

void DrawBorder(int n, char c);
void InputNumber(int &n, int column, char c[]);
void Inputpos(int n, int &row, int column, char c[]);
void Input(int arr[], int n, int column, char c);
void Input(int &n, int column, char c[]);
void InputValue(int &n);
void Inputposition(int n, int &pos);

void Output(int arr[], int n, char c);
int isOdd(int n);
int isPrime(int n);
void InsertAt(int arr[], int &n, int value, int pos);
void Delete(int arr[], int &n, int pos);
void InitializeValuesArray(int arr[], int &n, int value);
int isExist(int arr[], int n, int value);
void Swap(int &a, int &b);
void InterChange(int arr[], int n);
void SelectionSort(int arr[], int n);
void BubbleSort(int arr[], int n);
void Insert(int arr[], int pos, int value);
void InsertionSort(int arr[], int n);
void Input2D(int arr[][column], int row, int column);
void Output2D(int arr[][column], int row, int column);
void InputRandom(int a[][column], int &dong, int &cot);
void SelectInput(int arr2D[][column], int &row, int &column, int flag);
void SET_COLOR(int color);
void Draw(char c);
void NumberColumn(int n, int row);
void NumberRow(int n);

// Chuong 6.
void SelectInputPointer(int **&arr, int &row, int column, int *list, int flag);
void InputNumberBlock(int &n, char c[]);
void InputArrPointer(int *&arr, int n, int column);
void InputArrPointerV2(int *&arr, int n);
void InputPointer(int **&arr, int row, int column, int *list);
void OutputPointer(char **arr, int row, int column, int *list);
void checkFlag(int flag);
void Menu(int &flag);
void Delete(char *dest, int &length, int pos, int value);

// Chuong 7.

int isDigit(char c);
int isCheck(char *source);
// con trỏ hàm là tham số của hàm isCheck
int isCheck(char *source, int(*condition)(char c));
void Output(char *name, int length, char c[]);
int getLength(char *c);
int isLower(char c);
int isUpper(char c);
void ToUpper(char *&p);
int isMax(int a, int b);
int Compare(char *str1, char *str2);
char toLower(char &c);
int CompareImprove(char *str1, char *str2);
void InputPass(char *&password, char c[]);


