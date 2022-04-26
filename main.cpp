#include<iostream>


using namespace std;

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

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
	}

	void floodFill(int row, int column)
	{
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
int main()
{
	Maze <4, 5> maze;
	maze.addRobotPosition(3, 4, WEST);
	maze.addWall(WEST);
	maze.addWall(EAST);
	maze.addGoal(1, 2);
	maze.print();

	return 0;
}