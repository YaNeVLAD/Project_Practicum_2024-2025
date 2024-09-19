#include <iostream>
#include "lab2.h"

using namespace std;

void WriteHelloWorld();
void InputNumbers();
void LinearEquasion1();
void LinearEquasion2();
void SquareEquasion();

int Lab2()
{
	setlocale(LC_ALL, "rus");

	WriteHelloWorld();

	InputNumbers();

	LinearEquasion1();
	LinearEquasion2();

	SquareEquasion();

	return EXIT_SUCCESS;
}

void WriteHelloWorld()
{
	cout << "Hello World!" << endl;

	puts("Hello Vlad!");
}

void InputNumbers()
{
	int x, y;
	scanf_s("%d %d", &x, &y);
	cout << endl;
	printf("%d %d\n", x, y);
}

void LinearEquasion1()
{
	puts("please enter a and b for ax + b");
	int a = 0;
	int b = 0;
	scanf_s("%d %d", &a, &b);

	int x = -b / a;
	printf("solution: %d\n", x);
}

void LinearEquasion2()
{
	cout << "please enter a and b for ax + b" << endl;
	float a = 0;
	float b = 0;
	scanf_s("%d %d", &a, &b);

	int x = -b / a;
	printf("solution: %d\n", x);
}

void SquareEquasion()
{
	float a, b, c;
	float D = 0.0;
	cout << "ax^2 + bx + c = 0" << endl;
	cout << "������� ����������� a, b, c" << endl;
	cin >> a >> b >> c;
	D = b * b - 4 * a * c;
	if (D > 0)
	{
		cout << "������ ������: " << (-b + sqrt(D)) / (2 * a) << endl;
		cout << "������ ������: " << (-b - sqrt(D)) / (2 * a) << endl;
	}
	else if (D == 0)
	{
		cout << "������������ ������: " << - b / (2 * a) << endl; 
	}
	else
	{
		cout << "������ �� ������������� < 0. ������ ���." << endl;
	}
}