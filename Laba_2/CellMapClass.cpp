#include <iostream>
#include "Settings.cpp"
#include "Main.h"

#define OFF_COLOUR 0x00
#define ON_COLOUR 0xFF

using namespace std;

// массив клеток и их состояний
class CellMap
{
public:
	CellMap(unsigned int w, unsigned int h);
	~CellMap();
	void SetCell(unsigned int x, unsigned int y);
	void ClearCell(unsigned int x, unsigned int y);
	int CellState(int x, int y);
	void NextGen();
	void Init(unsigned int seed);

	unsigned char* cells;
	int length_in_bytes;
private:
	unsigned char* temp_cells;
	int width;
	int height;
};

CellMap::CellMap(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
	length_in_bytes = w * h;
	cells = new unsigned char[length_in_bytes];  // массив клеток
	temp_cells = new unsigned char[length_in_bytes]; // временный массив клеток
	memset(cells, 0, length_in_bytes);  // заполняем нулями массив клеток
	memset(temp_cells, 0, length_in_bytes);  // заполняем нулями массив клеток
}

CellMap::~CellMap()
{
	delete[] cells;
	delete[] temp_cells;
}

void CellMap::SetCell(unsigned int x, unsigned int y)
{
	int w = width, h = height;
	int xoleft, xoright, yoabove, yobelow;
	unsigned char* cell_ptr = cells + (y * w) + x;

	// вычисляем, как добраться до клеток соседей
	// учитываем, не на краю ли карты клетка
	xoleft = (x == 0) ? w - 1 : -1;
	xoright = (x == (w - 1)) ? -(w - 1) : 1;
	yoabove = (y == 0) ? length_in_bytes - w : -w;
	yobelow = (y == (h - 1)) ? -(length_in_bytes - w) : w;

	*(cell_ptr) |= 0x01; // Set first bit to 1

	// увеличиваем на 1 параметр "количество соседей" у соседей 
	*(cell_ptr + yoabove + xoleft) += 0x02;
	*(cell_ptr + yoabove) += 0x02;
	*(cell_ptr + yoabove + xoright) += 0x02;
	*(cell_ptr + xoleft) += 0x02;
	*(cell_ptr + xoright) += 0x02;
	*(cell_ptr + yobelow + xoleft) += 0x02;
	*(cell_ptr + yobelow) += 0x02;
	*(cell_ptr + yobelow + xoright) += 0x02;
}

void CellMap::ClearCell(unsigned int x, unsigned int y)
{
	int w = width, h = height;
	int xoleft, xoright, yoabove, yobelow;
	unsigned char* cell_ptr = cells + (y * w) + x;

	// Calculate the offsets to the eight neighboring cells,
	// accounting for wrapping around at the edges of the cell map
	xoleft = (x == 0) ? w - 1 : -1;
	xoright = (x == (w - 1)) ? -(w - 1) : 1;
	yoabove = (y == 0) ? length_in_bytes - w : -w;
	yobelow = (y == (h - 1)) ? -(length_in_bytes - w) : w;


	*(cell_ptr) &= ~0x01; // Set first bit to 0

	// Change successive bits for neighbour counts
	*(cell_ptr + yoabove + xoleft) -= 0x02;
	*(cell_ptr + yoabove) -= 0x02;
	*(cell_ptr + yoabove + xoright) -= 0x02;
	*(cell_ptr + xoleft) -= 0x02;
	*(cell_ptr + xoright) -= 0x02;
	*(cell_ptr + yobelow + xoleft) -= 0x02;
	*(cell_ptr + yobelow) -= 0x02;
	*(cell_ptr + yobelow + xoright) -= 0x02;
}

int CellMap::CellState(int x, int y)
{
	unsigned char* cell_ptr = cells + (y * width) + x;

	// возвращаем первый бит с сотоянием клетки
	return *cell_ptr & 0x01;
}

void CellMap::NextGen()
{
	unsigned int x, y, count;
	unsigned int h = height, w = width;
	unsigned char* cell_ptr;

	// заполняем временный массив
	memcpy(temp_cells, cells, length_in_bytes);

	cell_ptr = temp_cells;
	for (y = 0; y < h; y++) {

		x = 0;
		do {
			count = *cell_ptr >> 1; // количество соседей клетки 
			if (*cell_ptr & 0x01) {

				// смерть клетки
				if ((count != 2) && (count != 3)) {
					ClearCell(x, y);
					DrawCell(x, y, OFF_COLOUR);
				}
			}
			else {

				// рождение клетки
				if (count == 3) {
					SetCell(x, y);
					DrawCell(x, y, ON_COLOUR);
				}
			}

			// двигаемся к следующей клетке
			cell_ptr++;

		} while (++x < w);
	}
}

void CellMap::Init(unsigned int seed)
{
	unsigned int x, y, init_length;

	cout << "Initializing" << endl;

	// случайным образом заполняем около половины пространства живыми клетками
	srand(seed);
	init_length = (width * height) / 2;
	do
	{
		x = rand() % (width - 1);
		y = rand() % (height - 1);
		if (CellState(x, y) == 0)
			SetCell(x, y);
	} while (--init_length);
}