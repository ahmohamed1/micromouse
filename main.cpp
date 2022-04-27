#include<iostream>
#include<queue>
#include <chrono>

using namespace std;

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

//const int Neighbours[4][2] = {{0,1},
//							  {-1,0},
//							  {0,-1},
//							  {1,0} };

const int Neighbours[4][2] = { {-1,0},
							  {0,1},
							  {1,0},
							  {0,-1} };
struct Cell
{
	int row;
	int column;
	int idea;
	Cell(int _row, int _column)
	{
		row = _row;
		column = _column;
	}
};

template <int ROWS, int COLUMNS>
class Maze
{
private:
	bool vertical_walls[ROWS][COLUMNS + 1];
	bool horizontal_walls[ROWS + 1][COLUMNS];
public:
	int maze[ROWS][COLUMNS];

	int robotRow;
	int robotColumn;
	int robotHeading;

	int goalRow;
	int goalColumns;

	Maze()
	{
		// initilize wall at vertical array
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLUMNS + 1; j++)
			{
				vertical_walls[i][j] = false;
			}
		}

		//initialize wall at horizontial array
		for (int i = 0; i < ROWS + 1; i++)
		{
			for (int j = 0; j < COLUMNS; j++)
			{
				horizontal_walls[i][j] = false;
			}
		}

		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLUMNS; j++)
			{
				maze[i][j] = 255;
			}
		}
	}

	void addRobotPosition(int row, int column, int heading)
	{
		robotColumn = column;
		robotRow = row;
		robotHeading = heading;
	}

	void addGoal(int row, int column)
	{
		goalRow = row;
		goalColumns = column;
		//auto start = std::chrono::high_resolution_clock::now();
		floodFill(goalRow, goalColumns);
		//auto end = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double, std::milli> float_ms = end - start;

		//std::cout << "elapsed time is " << float_ms.count() << " milli seconds )" << std::endl;
	}

	void floodFillUnkown(int row, int column, int newValue)
	{
		Cell currentCell(row, column);
		currentCell.idea = 0;
		queue<Cell> list;
		list.push(currentCell);
		int currntValue = maze[row][column];
		while (!list.empty())
		{
			currentCell = list.front();
			list.pop();
			if (currentCell.row < 0 || currentCell.row >= ROWS || currentCell.column < 0 || currentCell.column >= COLUMNS)
				continue;
			if (maze[currentCell.row][currentCell.column] != currntValue)
				continue;

			for (auto neighbor : Neighbours)
			{
				Cell neighborCell(currentCell.row + neighbor[0], currentCell.column + neighbor[1]);
				neighborCell.idea = currentCell.idea + 1;
				list.push(neighborCell);
			}
			maze[currentCell.row][currentCell.column] = currentCell.idea;
		}

	}

	bool CheckWallExisting(int row, int column, int direction)
	{
		switch (direction)
		{
			case(NORTH):
				//cout << "NORTH\n";
				return horizontal_walls[row][column];
				break;
			case(EAST):
				//cout << "EAST\n";
				return vertical_walls[row][column + 1];
				break;
			case(SOUTH):
				//cout << "SOUTH\n";
				return horizontal_walls[row + 1][column];
				break;
			case(WEST):
				//cout << "WEST\n";
				return vertical_walls[row][column];
			default:
				break;
		}


	}

	// This function used queu to solve the maze
	void floodFill(int row, int column, int newValue)
	{
		Cell currentCell(row, column);
		currentCell.idea = 0;
		queue<Cell> list;
		list.push(currentCell);
		int currntValue = maze[row][column];
		while (!list.empty())
		{
			currentCell = list.front();
			list.pop();
			
			if (currentCell.row < 0 || currentCell.row >= ROWS || currentCell.column < 0 || currentCell.column >= COLUMNS)
				continue;
			if (maze[currentCell.row][currentCell.column] != currntValue)
				continue;
			for (int i = 0; i < 4; i++)
			{
				auto neighbor = Neighbours[i];
				if (CheckWallExisting(currentCell.row, currentCell.column, i) == false)
				{
					Cell neighborCell(currentCell.row + neighbor[0], currentCell.column + neighbor[1]);
					neighborCell.idea = currentCell.idea + 1;
					list.push(neighborCell);
				}
			}
			maze[currentCell.row][currentCell.column] = currentCell.idea;
		}
	}

	void floodFill(int row, int column)
	{
		maze[row][column] = 0;

		bool FinishLoop = true;
		while (FinishLoop)
		{
			FinishLoop = false;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLUMNS; j++)
				{
					if (maze[i][j] < 255)
					{
						for (int k = 0; k < 4; k++)
						{
							bool wallExist = CheckWallExisting(i, j, k);
							auto neiborCell = Neighbours[k];
							if (maze[i + neiborCell[0]][j + neiborCell[1]] ==255 && !wallExist)
							{
								maze[i + neiborCell[0]][j + neiborCell[1]] = maze[i][j] + 1;
								FinishLoop = true;
							}
						}
					}
				}
			}
		}
	}

	// This function use recusion approch to do the flood fill
	void floodFill_1(int row, int column, int currntValue, int newValue)
	{
		if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS)
			return;
		if (maze[row][column] != currntValue)
			return;

		maze[row][column] = newValue;
		floodFill(row + 1, column, currntValue, newValue + 1);
		floodFill(row - 1, column, currntValue, newValue + 1);
		floodFill(row, column + 1, currntValue, newValue + 1);
		floodFill(row, column - 1, currntValue, newValue + 1);
	}

	int FindBestNeighbor()
	{
		int directionToGo = 0;
		int bestValue = 255;
		for (int k = 0; k < 4; k++)
		{
			auto neibour = Neighbours[k];
			bool wallExist = CheckWallExisting(robotRow, robotColumn, k);
			if (!wallExist && maze[robotRow + neibour[0]][robotColumn + neibour[1]] < bestValue)
			{
				directionToGo = k;
				bestValue = maze[robotRow + neibour[0]][robotColumn + neibour[1]];
			}
			if (!wallExist && maze[robotRow + neibour[0]][robotColumn + neibour[1]] == bestValue && robotHeading == k)
			{
				directionToGo = robotHeading;
				bestValue = maze[robotRow + neibour[0]][robotColumn + neibour[1]];
			}
		}
		return directionToGo;
	}

	void addWall(int direction)
	{
		switch (direction)
		{
		case(NORTH):
			horizontal_walls[robotRow][robotColumn] = true;
			break;
		case(EAST):
			vertical_walls[robotRow][robotColumn + 1] = true;
			break;
		case(SOUTH):
			horizontal_walls[robotRow + 1][robotColumn] = true;
			break;
		case(WEST):
			vertical_walls[robotRow][robotColumn] = true;
			break;
		default:
			break;
		}
	}

	void addVirtualWall(int row, int column, int direction)
	{
		switch (direction)
		{
		case(NORTH):
			horizontal_walls[row][column] = true;
			break;
		case(EAST):
			vertical_walls[row][column + 1] = true;
			break;
		case(SOUTH):
			horizontal_walls[row + 1][column] = true;
			break;
		case(WEST):
			vertical_walls[row][column] = true;
			break;
		default:
			break;
		}
	}

	void CreateMaze()
	{
		for (int i = 0; i < 4; i++)
		{
			addVirtualWall(i, 0, WEST);
			addVirtualWall(i, 5, EAST);
		}
		for (int i = 0; i < 6; i++)
		{
			addVirtualWall(0, i, NORTH);
			addVirtualWall(3, i, SOUTH);
		}
		addVirtualWall(0, 0, EAST);
		addVirtualWall(1, 0, EAST);
		addVirtualWall(3, 0, EAST);

		addVirtualWall(1, 1, EAST);
		addVirtualWall(2, 1, EAST);
		addVirtualWall(2, 2, EAST);
		addVirtualWall(2, 3, EAST);
		addVirtualWall(2, 4, EAST);

		addVirtualWall(2, 4, SOUTH);
		addVirtualWall(2, 3, NORTH);
		addVirtualWall(0, 2, SOUTH);
		addVirtualWall(0, 3, SOUTH);
		addVirtualWall(0, 4, SOUTH);
	}

	void print()
	{
		for (int i = 0; i < ROWS * 2 + 1; i++)
		{
			for (int j = 0; j < COLUMNS * 2 + 1; j++)
			{
				// We are looping at the line that represent the vertical walls 
				if (i % 2)
				{
					if (j % 2 == 0) {
						if (vertical_walls[i / 2][j / 2] == true)
							cout << "|";
						else
							cout << " ";

					}
					else
					{
						if (robotColumn == j / 2 && robotRow == i / 2)
						{
							switch (robotHeading)
							{
							case(NORTH):
								cout << " ^ ";
								break;
							case(EAST):
								cout << " > ";
								break;
							case(SOUTH):
								cout << " v ";
								break;
							case(WEST):
								cout << " < ";
								break;
							default:
								break;
							}

						}
						else if (goalColumns == j / 2 && goalRow == i / 2)
							cout << " G ";
						else
							cout << " " << maze[i / 2][j / 2] << " ";
					}
				}
				else {
					if (j % 2 == 1) {
						if (horizontal_walls[i / 2][j / 2] == true)
							cout << "---";
						else
							cout << "   ";
					}
					else
					{
						cout << "*";
					}
				}
			}
			cout << endl;
		}
	}
};


struct CellWall
{
	bool northWall;
	bool eastWall;
	bool southWall;
	bool westWall;
	int value;

	CellWall() {}
	CellWall(bool n, bool e, bool s, bool w)
	{
		northWall = n;
		eastWall = e;
		southWall = s;
		westWall = w;
	}
};

template <int ROWS, int COLUMNS>
class Maze2
{
private:

public:
	CellWall maze[ROWS][COLUMNS];

	int robotRow;
	int robotColumn;
	int robotHeading;

	int goalRow;
	int goalColumns;

	Maze2()
	{
		// initilize wall at vertical array
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLUMNS + 1; j++)
			{
				maze[i][j].northWall = false;
				maze[i][j].eastWall = false;
				maze[i][j].southWall = false;
				maze[i][j].westWall = false;
				maze[i][j].value = 255;
			}
		}

	}
	~Maze2()
	{
		delete[] maze;
	}
	void addRobotPosition(int row, int column, int heading)
	{
		robotColumn = column;
		robotRow = row;
		robotHeading = heading;
	}

	void addGoal(int row, int column)
	{
		goalRow = row;
		goalColumns = column;
		//auto start = std::chrono::high_resolution_clock::now();
		floodFill(goalRow, goalColumns);
		//auto end = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double, std::milli> float_ms = end - start;

		//std::cout << "elapsed time is " << float_ms.count() << " milli seconds )" << std::endl;
	}

	void floodFillUnkown(int row, int column, int newValue)
	{
		Cell currentCell(row, column);
		currentCell.idea = 0;
		queue<Cell> list;
		list.push(currentCell);
		int currntValue = maze[row][column];
		while (!list.empty())
		{
			currentCell = list.front();
			list.pop();
			if (currentCell.row < 0 || currentCell.row >= ROWS || currentCell.column < 0 || currentCell.column >= COLUMNS)
				continue;
			if (maze[currentCell.row][currentCell.column] != currntValue)
				continue;

			for (auto neighbor : Neighbours)
			{
				Cell neighborCell(currentCell.row + neighbor[0], currentCell.column + neighbor[1]);
				neighborCell.idea = currentCell.idea + 1;
				list.push(neighborCell);
			}
			maze[currentCell.row][currentCell.column] = currentCell.idea;
		}

	}

	bool CheckWallExisting(int row, int column, int direction)
	{
		switch (direction)
		{
		case(NORTH):

			return maze[row][column].northWall;
			break;
		case(EAST):
			return maze[row][column].eastWall;
			break;
		case(SOUTH):
			return maze[row][column].southWall;
			break;
		case(WEST):
			return maze[row][column].westWall;
		default:
			break;
		}
	}

	// This function used queu to solve the maze
	void floodFill(int row, int column, int newValue)
	{
		Cell currentCell(row, column);
		currentCell.idea = 0;
		queue<Cell> list;
		list.push(currentCell);
		int currntValue = maze[row][column];
		while (!list.empty())
		{
			currentCell = list.front();
			list.pop();

			if (currentCell.row < 0 || currentCell.row >= ROWS || currentCell.column < 0 || currentCell.column >= COLUMNS)
				continue;
			if (maze[currentCell.row][currentCell.column] != currntValue)
				continue;
			for (int i = 0; i < 4; i++)
			{
				auto neighbor = Neighbours[i];
				if (CheckWallExisting(currentCell.row, currentCell.column, i) == false)
				{
					Cell neighborCell(currentCell.row + neighbor[0], currentCell.column + neighbor[1]);
					neighborCell.idea = currentCell.idea + 1;
					list.push(neighborCell);
				}
			}
			maze[currentCell.row][currentCell.column] = currentCell.idea;
		}
	}

	void floodFill(int row, int column)
	{
		maze[row][column].value = 0;

		bool FinishLoop = true;
		while (FinishLoop)
		{
			FinishLoop = false;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLUMNS; j++)
				{
					if (maze[i][j].value < 255)
					{
						for (int k = 0; k < 4; k++)
						{
							bool wallExist = CheckWallExisting(i, j, k);
							auto neiborCell = Neighbours[k];
							if (maze[i + neiborCell[0]][j + neiborCell[1]].value == 255 && !wallExist)
							{
								maze[i + neiborCell[0]][j + neiborCell[1]].value = maze[i][j].value + 1;
								FinishLoop = true;
							}
						}
					}
				}
			}
		}
	}

	// This function use recusion approch to do the flood fill
	void floodFill_1(int row, int column, int currntValue, int newValue)
	{
		if (row < 0 || row >= ROWS || column < 0 || column >= COLUMNS)
			return;
		if (maze[row][column] != currntValue)
			return;

		maze[row][column] = newValue;
		floodFill(row + 1, column, currntValue, newValue + 1);
		floodFill(row - 1, column, currntValue, newValue + 1);
		floodFill(row, column + 1, currntValue, newValue + 1);
		floodFill(row, column - 1, currntValue, newValue + 1);
	}

	int FindBestNeighbor()
	{
		int directionToGo = 0;
		int bestValue = 255;
		for (int k = 0; k < 4; k++)
		{
			auto neibour = Neighbours[k];
			bool wallExist = CheckWallExisting(robotRow, robotColumn, k);
			if (!wallExist && maze[robotRow + neibour[0]][robotColumn + neibour[1]].value < bestValue)
			{
				directionToGo = k;
				bestValue = maze[robotRow + neibour[0]][robotColumn + neibour[1]].value;
			}
			if (!wallExist && maze[robotRow + neibour[0]][robotColumn + neibour[1]].value == bestValue && robotHeading == k)
			{
				directionToGo = robotHeading;
				bestValue = maze[robotRow + neibour[0]][robotColumn + neibour[1]].value;
			}
		}
		return directionToGo;
	}

	void addWall(int direction)
	{
		switch (direction)
		{
		case(NORTH):
			maze[robotRow][robotColumn].northWall = true;
			maze[robotRow - 1][robotColumn].southWall = true;
			break;
		case(EAST):
			maze[robotRow][robotColumn].eastWall = true;
			maze[robotRow][robotColumn+1].westWall = true;
			break;
		case(SOUTH):
			maze[robotRow][robotColumn].southWall = true;
			maze[robotRow + 1][robotColumn].northWall = true;
			break;
		case(WEST):
			maze[robotRow][robotColumn].westWall = true;
			maze[robotRow][robotColumn-1].eastWall = true;
			break;
		default:
			break;
		}
	}

	void addVirtualWall(int row, int column, int direction)
	{
		switch (direction)
		{
		case(NORTH):
			maze[row][column].northWall = true;
			maze[row - 1][column].southWall = true;
			break;
		case(EAST):
			maze[row][column].eastWall = true;
			maze[row][column + 1].westWall = true;
			break;
		case(SOUTH):
			maze[row][column].southWall = true;
			maze[row + 1][column].northWall = true;
			break;
		case(WEST):
			maze[row][column].westWall = true;
			maze[row][column - 1].eastWall = true;
			break;
		default:
			break;
		}
	}

	void CreateMaze()
	{
		for (int i = 0; i < 4; i++)
		{
			addVirtualWall(i, 0, WEST);
			addVirtualWall(i, 5, EAST);
		}
		for (int i = 0; i < 6; i++)
		{
			addVirtualWall(0, i, NORTH);
			addVirtualWall(3, i, SOUTH);
		}
		addVirtualWall(0, 0, EAST);
		addVirtualWall(1, 0, EAST);
		addVirtualWall(3, 0, EAST);

		addVirtualWall(1, 1, EAST);
		addVirtualWall(2, 1, EAST);
		addVirtualWall(2, 2, EAST);
		addVirtualWall(2, 3, EAST);
		addVirtualWall(2, 4, EAST);

		addVirtualWall(2, 4, SOUTH);
		addVirtualWall(2, 3, NORTH);
		addVirtualWall(0, 2, SOUTH);
		addVirtualWall(0, 3, SOUTH);
		addVirtualWall(0, 4, SOUTH);
	}

	void print()
	{
		for (int i = 0; i < ROWS * 2 +1; i++)
		{
			for (int j = 0; j < COLUMNS * 2+1; j++)
			{
				// We are looping at the line that represent the vertical walls 
				if (i % 2)
				{
					if (j % 2 == 0) {
						if (maze[i / 2][j / 2].westWall == true)
							cout <<"|";
						else
							cout << " ";
					}
					else
					{
						if (robotColumn == j / 2 && robotRow == i / 2)
						{
							switch (robotHeading)
							{
							case(NORTH):
								cout << " ^ ";
								break;
							case(EAST):
								cout << " > ";
								break;
							case(SOUTH):
								cout << " v ";
								break;
							case(WEST):
								cout << " < ";
								break;
							default:
								break;
							}

						}
						else if (goalColumns == j / 2 && goalRow == i / 2)
							cout << " G ";
						else
							cout << " " << maze[i / 2][j / 2].value << " ";
					}
				}
				else {
					if (j % 2 == 1) {
						if (maze[i / 2][j / 2].northWall == true)
							cout << "---";
						else
							cout << "   ";
					}
					else
					{
						cout << "*";
					}
				}
			}
			cout << endl;
		}
	}
};

int main()
{
	
	Maze2 <4,6> maze;
	maze.CreateMaze();
	maze.addGoal(2,4);
	maze.addRobotPosition(1,1, NORTH);
	cout << maze.FindBestNeighbor() << endl;
	maze.print();
	
	return 0;
}