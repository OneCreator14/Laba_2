class CellMap
{
public:
	CellMap(unsigned int w, unsigned int h);
	~CellMap();
	void SetCell(unsigned int x, unsigned int y);
	void ClearCell(unsigned int x, unsigned int y);
	int CellState(int x, int y);
	void NextGen();
	void TestNextGen();
	void Init(unsigned int seed);

	unsigned char* cells;
	int length_in_bytes;
private:
	unsigned char* temp_cells;
	int width;
	int height;
};