#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <windows.h>

struct direction
{
	int x;
	int y;
};

void SetColor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

void resizeGrid(std::vector<std::vector <int>>& tileGrid, int size)
{
	tileGrid.resize(size);
	for (int i = 0; i < size; i++)
	{
		tileGrid[i].resize(size);
	}
}

void printGrid(std::vector<std::vector <int>>& tileGrid)
{
	for (const auto& row : tileGrid)
	{
		for (const auto& elem : row)
		{
			std::cout << elem << '\t';
		}
		std::cout << std::endl;
	}
}

void setStartFinish(std::vector<std::vector <int>>& tileGrid, direction start, direction finish)
{
	tileGrid[start.x][start.y] = 1;
	tileGrid[finish.x][finish.y] = 2;
}

void fillGridRandom(std::vector<std::vector <int>>& tileGrid, int maxValue)
{
	for (int i = 0; i < tileGrid.size(); i++)
	{
		for (int j = 0; j < tileGrid[i].size(); j++)
		{
			tileGrid[i][j] = rand() % maxValue + 1;
		}
	}
}

bool isValidTile(direction position, int size)
{
	return ((position.x >= 0 && position.x < size) &&
			(position.y >= 0 && position.y < size));
}

bool isTileRoad(const direction& position, const std::vector<direction>& currentRoad)
{
	for (int i = 0; i < currentRoad.size(); i++)
	{
		if ((currentRoad[i].x == position.x) && (currentRoad[i].y == position.y))
			return true;
	}
	return false;
}

int compareWeights(direction& newTilePosition, const std::vector<std::vector <int>>& tileGrid, const std::vector<direction>& currentRoad, int minWeight, direction& resultTilePosition, bool& success)
{
	if (isValidTile(newTilePosition, tileGrid.size()))
	{
		if (!isTileRoad(newTilePosition, currentRoad))
		{
			if (tileGrid[newTilePosition.x][newTilePosition.y] < minWeight)
			{
				minWeight = tileGrid[newTilePosition.x][newTilePosition.y];
				resultTilePosition = newTilePosition;
				success = true;
			}
		}
	}
	return minWeight;
}

direction getMinRoadPosition(const std::vector<std::vector <int>>& tileGrid, const std::vector<direction>& currentRoad, direction currentTilePosition, bool &success)
{
	int minWeight = 9999999;
	direction newTilePosition = currentTilePosition;
	direction resultTilePosition;
	// left
	newTilePosition = { currentTilePosition.x, currentTilePosition.y - 1 };
	minWeight = compareWeights(newTilePosition, tileGrid, currentRoad, minWeight, resultTilePosition, success);
	// up
	newTilePosition = { currentTilePosition.x + 1, currentTilePosition.y };
	minWeight = compareWeights(newTilePosition, tileGrid, currentRoad, minWeight, resultTilePosition, success);
	// right
	newTilePosition = { currentTilePosition.x, currentTilePosition.y + 1 };
	minWeight = compareWeights(newTilePosition, tileGrid, currentRoad, minWeight, resultTilePosition, success);
	// down
	newTilePosition = { currentTilePosition.x - 1, currentTilePosition.y };
	minWeight = compareWeights(newTilePosition, tileGrid, currentRoad, minWeight, resultTilePosition, success);
	
	return resultTilePosition;
}

bool isPositionEqual(direction positon, direction otherposition)
{
	return ((positon.x == otherposition.x) && (positon.y == otherposition.y));
}

void structNewRoad(std::vector<std::vector <int>>& tileGrid, std::vector<direction>& currentRoad, direction start, direction finish, bool& success)
{
	bool canStructRoad = true;
	direction currentTilePosition = start;
	direction tempPosition;
	currentRoad.clear();
	currentRoad.push_back(start);
	while (true)
	{
		bool findSuccess = false;
		tempPosition = getMinRoadPosition(tileGrid, currentRoad, currentTilePosition, findSuccess);
		if (findSuccess)
		{
			currentRoad.push_back(tempPosition);
			currentTilePosition = tempPosition;
			//std::cout << "x = " << currentTilePosition.x << " y = " << currentTilePosition.y << std::endl;
			if (isPositionEqual(currentTilePosition, finish))
			{
				success = true;
				return;
			}
		}
		else
		{
			success = false;
			return;
		}
	}
}
/*
void printPositions(const std::vector<direction>& currentRoad)
{
	for (const auto& elem : currentRoad)
	{
		std::cout << "x = "<< elem.x << " y = " << elem.y << std::endl;
	}
	std::cout << std::endl;
}*/

void printRoad(std::vector<std::vector <int>>& tileGrid, std::vector<direction>& currentRoad)
{
	for (int i = 0; i < tileGrid.size(); i++)
	{
		for (int j = 0; j < tileGrid[i].size(); j++)
		{
			if (isTileRoad({ i,j }, currentRoad))
				SetColor(4, 0);
			std::cout << tileGrid[i][j] << '\t';
			SetColor(15, 0);
		}
		std::cout << std::endl;
	}
}

void testStructRoad(std::vector<std::vector <int>>& tileGrid, std::vector<direction>& currentRoad, direction start, direction finish)
{
	int countPassed = 0;
	int countFailed = 0;
	for (int i = 0; i < 10000; i++)
	{
		int attempts = 100;
		bool success = false;
		while (attempts <= 0 || !success)
		{
			structNewRoad(tileGrid, currentRoad, start, finish, success);
			if (success)
			{
			}
			else
			{
				attempts--;
				fillGridRandom(tileGrid, 20);
				setStartFinish(tileGrid, start, finish);
			}
		}
		if (success)
			countPassed++;
		else
			countFailed++;
		
	}
	std::cout << "Passed = " << countPassed << std::endl;
	std::cout << "Failed = " << countFailed << std::endl;
}

int main()
{
	int size = 5;
	std::vector<std::vector <int>> tileGrid;
	std::vector<direction> currentRoad;
	srand(time(NULL));
	direction start = { 0, size - 2 };
	direction finish = { size - 1, 2 };
	

	resizeGrid(tileGrid, size);
	fillGridRandom(tileGrid, 19);
	setStartFinish(tileGrid, start, finish);

	std::cout << "Our tile grid" << std::endl;
	printGrid(tileGrid);


	int attempts = 100;
	bool success = false;
	while (attempts <= 0 || !success)
	{
		structNewRoad(tileGrid, currentRoad, start, finish, success);
		if (success)
		{
			std::cout << std::endl << std::endl;
			std::cout << "Calculated road" << std::endl;
			printRoad(tileGrid, currentRoad);
			std::cout << "attempts have: " << attempts;
			break;
		}
		else
		{
			attempts--;
			fillGridRandom(tileGrid, 20);
			setStartFinish(tileGrid, start, finish);
		}
	}

	return 0;
}