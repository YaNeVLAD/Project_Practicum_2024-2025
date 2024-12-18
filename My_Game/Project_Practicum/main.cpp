#include "App.h"

#include <Windows.h>

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	App& app = App::Instance();
	app.Run();
	return EXIT_SUCCESS;
}