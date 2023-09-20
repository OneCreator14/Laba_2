#include "Main.h"
#include "CellMapClass.h"
#include <iostream>

using namespace std;

CellMap testMap(3, 3);
unsigned char* temp_cells;


bool Test1() {
	memcpy(temp_cells, testMap.cells, testMap.length_in_bytes);

	if (!(*(temp_cells+0) & 0x01)	&&   !(*(temp_cells+1) & 0x01)   &&	  !(*(temp_cells+2) & 0x01) &&
		 (*(temp_cells+3) & 0x01)   &&	  (*(temp_cells+4) & 0x01)	 &&	   (*(temp_cells+5) & 0x01) &&
		!(*(temp_cells+6) & 0x01)	&&	 !(*(temp_cells+7) & 0x01)	 &&	  !(*(temp_cells+8) & 0x01))

		{ return true; }

	else {
		cout << "Ошибка в тесте 1" << endl;
		return false;
	}
}

bool Test2() {
	testMap.TestNextGen();
	memcpy(temp_cells, testMap.cells, testMap.length_in_bytes);
	
	if ((*(temp_cells+0) & 0x01)	&&    (*(temp_cells+1) & 0x01)   &&	   (*(temp_cells+2) & 0x01) &&
		(*(temp_cells+3) & 0x01)    &&	  (*(temp_cells+4) & 0x01)	 &&	   (*(temp_cells+5) & 0x01) &&
		(*(temp_cells+6) & 0x01)	&&	  (*(temp_cells+7) & 0x01)	 &&	   (*(temp_cells+8) & 0x01))

		{ return true; }

	else {
		cout << "Ошибка в тесте 2" << endl;
		return false;
	}
}

bool Test3() {
	testMap.TestNextGen();
	memcpy(temp_cells, testMap.cells, testMap.length_in_bytes);

	if (!(*(temp_cells + 0) & 0x01) && !(*(temp_cells + 1) & 0x01) && !(*(temp_cells + 2) & 0x01) &&
		!(*(temp_cells + 3) & 0x01) && !(*(temp_cells + 4) & 0x01) && !(*(temp_cells + 5) & 0x01) &&
		!(*(temp_cells + 6) & 0x01) && !(*(temp_cells + 7) & 0x01) && !(*(temp_cells + 8) & 0x01))

	{
		return true;
	}

	else {
		cout << "Ошибка в тесте 3" << endl;
		return false;
	}
}

void Module_tests() {

	testMap.SetCell(0, 1);
	testMap.SetCell(1, 1);
	testMap.SetCell(2, 1);

	temp_cells = new unsigned char[testMap.length_in_bytes];

	if (Test1() && Test2() && Test3()) {
		cout << "Модульные тесты прошли успешно!" << endl;
	}
}

void CellMap::TestNextGen()
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
					//DrawCell(x, y, OFF_COLOUR);
				}
			}
			else {

				// рождение клетки
				if (count == 3) {
					SetCell(x, y);
					//DrawCell(x, y, ON_COLOUR);
				}
			}

			// двигаемся к следующей клетке
			cell_ptr++;

		} while (++x < w);
	}
}