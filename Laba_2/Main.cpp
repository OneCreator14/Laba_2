#include <SDL.h>
#include <ctime>
#include <iostream>
#include <windows.h>
#include "CellMapClass.h"
#include "Settings.cpp"
#include "Module_tests.h"
#include "Algorithms.h"
#include "Greetings.h"
#include "Main.h"

// есть ли задержка
#define IS_DELAY 1
// задержка в милисекундах
#define TICK_RATE 20

using namespace std;

unsigned int seed;

// графика
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
unsigned int s_width = cellmap_width * cell_size;
unsigned int s_height = cellmap_height * cell_size;

void DrawCell(unsigned int x, unsigned int y, unsigned int colour)
{
	uint8_t* pixel_ptr = (uint8_t*)surface->pixels + (y * cell_size * s_width + x * cell_size) * 4;

	for (unsigned int i = 0; i < cell_size; i++)
	{
		for (unsigned int j = 0; j < cell_size; j++)
		{
			*(pixel_ptr + j * 4) = colour;
			*(pixel_ptr + j * 4 + 1) = colour;
			*(pixel_ptr + j * 4 + 2) = colour;
		}
		pixel_ptr += s_width * 4;
	}
}

int main(int argc, char* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	do {

		Greeting();

		cout << "Провести модульные тесты?\n1 - Да\n2 - Нет" << endl;
		int choice = Check_choice(TWO_ELEMENTS);
		if (choice == YES) {
			Module_tests();
		}


		// настройки графической библиотеки
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, s_width, s_height, SDL_WINDOW_SHOWN);
		surface = SDL_GetWindowSurface(window);

		// счетчик поколений
		unsigned long generation = 0;

		// инициаоизация карты
		CellMap current_map(cellmap_width, cellmap_height);
		current_map.Init(seed);

		// получаем зерно мира
		seed = (unsigned)time(NULL);

		SDL_Event e;

		bool quit = false;
		while (!quit)
		{
			while (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;

			generation++;

			current_map.NextGen();
			SDL_UpdateWindowSurface(window);

		#if IS_DELAY
			SDL_Delay(TICK_RATE);
		#endif
		}

		SDL_DestroyWindow(window);
		SDL_Quit();

		cout << "Total Generations: " << generation
			<< "\nSeed: " << seed << endl;
		cout << "Нажмите Enter, для того чтобы повторить программу.\n" "'0' - для завершения программы." << endl;
	} while (getchar() != '0');

	return 0;
}

