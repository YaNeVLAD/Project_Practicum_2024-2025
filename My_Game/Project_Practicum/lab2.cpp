#include <iostream>
#include "lab2.h"

using namespace std;

void InputNumbers();
void LinearEquasion1();
void LinearEquasion2();
void SquareEquasion();

int Lab2()
{
	setlocale(LC_ALL, "rus");

	cout << "Hello World!" << endl;

	puts("Hello World!");

	InputNumbers();

	LinearEquasion1();
	LinearEquasion2();

	SquareEquasion();

	return EXIT_SUCCESS;
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
	//D = b^2 - 4ac
	// if sqrt(D) > 0
	//x(1) = (-b+sqrt(D))/2a
	//x(2) = (-b-sqrt(D))/2a
	// if sqrt(D) = 0
	// x = -b/2a
	// else
	// 0 roots
	float a, b, c;
	float D = 0.0;
	cout << "ax^2 + bx + c = 0" << endl;
	cout << "Введите коэфициенты a, b, c" << endl;
	cin >> a >> b >> c;
	D = b * b - 4 * a * c;
	if (D > 0)
	{
		cout << "Первый корень: " << (-b + sqrt(D)) / 2 * a << endl;
		cout << "Второй корень: " << (-b - sqrt(D)) / 2 * a<< endl;
	}
	else if (D == 0)
	{
		cout << "Единственный Корень: " << - b / 2 * a << endl; 
	}
	else
	{
		cout << "Корень из дискриминанта < 0. Корней нет." << endl;
	}
}
