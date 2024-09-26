#include <iostream>
#include "lab2.h"

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
	std::cout << "Hello World!" << std::endl;

	puts("Hello Vlad!");
}

void InputNumbers()
{
	int x, y;
	scanf_s("%d %d", &x, &y);
	std::cout << std::endl;
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
	std::cout << "please enter a and b for ax + b" << std::endl;
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
	std::cout << "ax^2 + bx + c = 0" << std::endl;
	std::cout << "Введите коэфициенты a, b, c" << std::endl;
	std::cin >> a >> b >> c;
	D = b * b - 4 * a * c;
	if (D > 0)
	{
		std::cout << "Первый корень: " << (-b + sqrt(D)) / (2 * a) << std::endl;
		std::cout << "Второй корень: " << (-b - sqrt(D)) / (2 * a) << std::endl;
	}
	else if (D == 0)
	{
		std::cout << "Единственный Корень: " << - b / (2 * a) << std::endl;
	}
	else
	{
		std::cout << "Корень из дискриминанта < 0. Корней нет." << std::endl;
	}
}
