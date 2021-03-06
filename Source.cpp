#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <windows.h>

enum class TileType
{
	Road,
	Start,
	Finish,
	Blocking,
	Field
};

enum class RoadType
{
	None,
	Straight,
	Turn,
	T_junction,
	Crossroad
};

enum class EventType
{
	None,
	Enemy,
	Start,
	Finish,
	Event,
	Equipment
};

enum SideType
{
	Up = 1,
	Down = 2,
	Left = 4,
	Right = 8
};

struct direction
{
	int x;
	int y;
};

struct TileInfo
{
	TileType tileType = TileType::Field;
	RoadType roadType = RoadType::None;
	EventType eventType = EventType::None;
	float rotateAngle = 0.0;
};

void SetColor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

////////////////////PRITNS//////////////////////
bool isTileRoad(const direction& position, const std::vector<direction>& currentRoad);

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

void printPositions(const std::vector<direction>& currentRoad)
{
	for (const auto& elem : currentRoad)
	{
		std::cout << "x = " << elem.x << " y = " << elem.y << std::endl;
	}
	std::cout << std::endl;
}

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

void printRoadSides(direction roadPosition, int roadSides)
{

	std::cout << "road " << roadPosition.x << " " << roadPosition.y << " have ";
	if (roadSides & SideType::Right)
		std::cout << "right ";
	if (roadSides & SideType::Left)
		std::cout << "left ";
	if (roadSides & SideType::Up)
		std::cout << "up ";
	if (roadSides & SideType::Down)
		std::cout << "down ";
	std::cout << std::endl;
}

void printTilesType(std::vector<std::vector<TileInfo>> tileGrid)
{
	std::cout << std::endl;
	for (const auto& row : tileGrid)
	{
		for (const auto& elem : row)
		{
			switch (elem.tileType)
			{
			case TileType::Start:
				std::cout << "start" << '\t';
				break;
			case TileType::Finish:
				std::cout << "finish" << '\t';
				break;
			case TileType::Road:
				std::cout << "road" << '\t';
				break;
			case TileType::Blocking:
				std::cout << "block" << '\t';
				break;
			case TileType::Field:
				std::cout << "field" << '\t';
				break;
			default:
				break;
			}

		}
		std::cout << std::endl;
	}
}

void printTilesRoadType(std::vector<std::vector<TileInfo>> tileGrid)
{
	std::cout << std::endl;
	for (const auto& row : tileGrid)
	{
		for (const auto& elem : row)
		{
			switch (elem.roadType)
			{
			case RoadType::None:
				std::cout << "None" << "\t";
				break;
			case RoadType::Straight:
				std::cout << "Straight" << "\t";
				break;
			case RoadType::Turn:
				std::cout << "Turn" << "\t";
				break;
			case RoadType::T_junction:
				std::cout << "T_junction" << "\t";
				break;
			case RoadType::Crossroad:
				std::cout << "Crossroad" << "\t";
				break;
			}

		}
		std::cout << std::endl;
	}
}
////////////////////PRITNS//////////////////////

void resizeGrid(std::vector<std::vector <int>>& tileGrid, int size)
{
	tileGrid.resize(size);
	for (int i = 0; i < size; i++)
	{
		tileGrid[i].resize(size);
	}
}

void resizeGrid(std::vector<std::vector <TileInfo>>& tileGrid, int size)
{
	tileGrid.resize(size);
	for (int i = 0; i < size; i++)
	{
		tileGrid[i].resize(size);
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



void clearRoadFromStartFinish(std::vector<direction>& currentRoad)
{
	currentRoad.pop_back(); // erase finish
	currentRoad.erase(currentRoad.begin());
}

std::vector<direction> generateRoad(std::vector<std::vector <int>>& tileGrid, direction start, direction finish)
{
	std::vector<direction> currentRoad;
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
			//std::cout << "attempts have: " << attempts;
			break;
		}
		else
		{
			attempts--;
			fillGridRandom(tileGrid, 20);
			setStartFinish(tileGrid, start, finish);
		}
	}
	return currentRoad;
}

void fillTileTypesGrid(std::vector<std::vector <TileInfo>>& tileGrid, std::vector<direction>& currentRoad, direction start, direction finish)
{
	for (int i = 0; i < tileGrid.size(); i++)
	{
		for (int j = 0; j < tileGrid[i].size(); j++)
		{
			if (isTileRoad({ i,j }, currentRoad))
			{
				if ((isPositionEqual({ i,j }, start)))
				{
					tileGrid[i][j].tileType = TileType::Start;
				}
				else if (isPositionEqual({ i,j }, finish))
				{
					tileGrid[i][j].tileType = TileType::Finish;
				}
				else
				{
					tileGrid[i][j].tileType = TileType::Road;
				}
			}
			else
			{
				tileGrid[i][j].tileType = TileType::Field;
			}
		}
	}
}

bool isRoadThisSide(std::vector<std::vector<TileInfo>>& tileGrid, direction currentPosition, SideType SelectedSide)
{
	bool success = false;
	int x_offset = 0;
	int y_offset = 0;
	switch (SelectedSide)
	{
	case SideType::Up:
		x_offset = 1;
		y_offset = 0;
		break;
	case SideType::Down:
		x_offset = -1;
		y_offset = 0;
		break;
	case SideType::Left:
		x_offset = 0;
		y_offset = -1;
		break;
	case SideType::Right:
		x_offset = 0;
		y_offset = 1;
		break;
	default:
		break;
	}

	direction newPositionOffset = { currentPosition.x + x_offset, currentPosition.y + y_offset };

	if (isValidTile(newPositionOffset, tileGrid.size()))
	{
		if (tileGrid[newPositionOffset.x][newPositionOffset.y].tileType == TileType::Road ||
			tileGrid[newPositionOffset.x][newPositionOffset.y].tileType == TileType::Start ||
			tileGrid[newPositionOffset.x][newPositionOffset.y].tileType == TileType::Finish)
		{
			success = true;
		}
		else
		{
			success = false;
		}
	}
	else
	{
		success = false;
	}
	return success;
}

RoadType recognizeRoadTypeWithRotateAngle(int roadSides, float& rotateAngle, int countRoads)
{
	RoadType roadType = RoadType::None;

	switch (countRoads)
	{
	default:
	case 2:
		if (roadSides & SideType::Up && roadSides & SideType::Down) // |
		{
			roadType = RoadType::Straight;
			rotateAngle = 0.0;
		}
		else if (roadSides & SideType::Left && roadSides & SideType::Right) // ─
		{
			roadType = RoadType::Straight;
			rotateAngle = 90.0;
		}
		else if (roadSides & SideType::Right)
		{
			roadType = RoadType::Turn;
			if (roadSides & SideType::Up) // └
			{
				rotateAngle = 180.0;
			}
			else						 // ╔
			{
				rotateAngle = 90.0;
			}
		}
		else
		{
			roadType = RoadType::Turn;
			if (roadSides & SideType::Up) // ╝
			{
				rotateAngle = -90.0;
			}
			else						 // ┐
			{
				rotateAngle = 0.0;
			}
		}
		break;
	case 3:
		roadType = RoadType::T_junction;
		if (!(roadSides & SideType::Right)) // ┤
		{
			rotateAngle = -90.0;
		}
		else if (!(roadSides & SideType::Up)) // ┬
		{
			rotateAngle = 0.0;
		}
		else if (!(roadSides & SideType::Left)) // ├
		{
			rotateAngle = 90.0;
		}
		else									// ┴
		{
			rotateAngle = 180.0;
		}
		break;
	case 4:
		roadType = RoadType::Crossroad;      // ┼
		rotateAngle = 0.0;
		break;
	}
	return roadType;
}

void recognizeRoadTypes(std::vector<direction>& currentRoad, std::vector<std::vector<TileInfo>>& tileGrid, direction start, direction finish, bool& success)
{
	for (int i = 0; i < currentRoad.size(); i++)
	{
		if ((isPositionEqual(currentRoad[i], start) || (isPositionEqual(currentRoad[i], finish)))) // first and last roads - start and finish
		{
			tileGrid[currentRoad[i].x][currentRoad[i].y].roadType = RoadType::None;
			tileGrid[currentRoad[i].x][currentRoad[i].y].rotateAngle = 0.0;
			continue;
		}

		int roadSides = 0;

		int countRoads = 0;
		float rotateAngle = 0.0;
		
		if (isRoadThisSide(tileGrid, currentRoad[i], SideType::Right))
		{
			roadSides |= SideType::Right;
			countRoads++;
		}
		if (isRoadThisSide(tileGrid, currentRoad[i], SideType::Left))
		{
			roadSides |= SideType::Left;
			countRoads++;
		}
		if (isRoadThisSide(tileGrid, currentRoad[i], SideType::Up))
		{
			roadSides |= SideType::Up;
			countRoads++;
		}
		if (isRoadThisSide(tileGrid, currentRoad[i], SideType::Down))
		{
			roadSides |= SideType::Down;
			countRoads++;
		}

		tileGrid[currentRoad[i].x][currentRoad[i].y].roadType = recognizeRoadTypeWithRotateAngle(roadSides, rotateAngle, countRoads);
		tileGrid[currentRoad[i].x][currentRoad[i].y].rotateAngle = rotateAngle;
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
	std::vector<std::vector <int>> numericGrid;
	std::vector<direction> currentRoad;
	std::vector<RoadType> RoadTypes;
	std::vector<std::vector<TileInfo>> tileGrid;
	srand(time(NULL));
	direction start = { 0, size - 2 };
	direction finish = { size - 1, 2 };
	
	//init
	resizeGrid(numericGrid, size);
	resizeGrid(tileGrid, size);


	// tile type
	fillGridRandom(numericGrid, 19);
	setStartFinish(numericGrid, start, finish);
	currentRoad = generateRoad(numericGrid, start, finish); // start and finish inside road (need to recognize angle rotate first road
	fillTileTypesGrid(tileGrid, currentRoad, start, finish);

	// road type
	//RoadTypes.clear();
	//RoadTypes.resize(currentRoad.size());
	bool success = false;
	recognizeRoadTypes(currentRoad, tileGrid, start, finish, success);
	printTilesRoadType(tileGrid);
	return 0;
}