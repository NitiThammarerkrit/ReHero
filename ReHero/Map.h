#pragma once

#include <fstream>
#include <vector>

enum class mapPointType
{
	POINT_START = 0,
	POINT_WEAK_MONSTER = 1,
	POINT_STRONG_MONSTER = 2,
	POINT_HEALING_WELL = 3,
	POINT_TREASURE_CHEST = 4,
	POINT_BOSS_MONSTER = 5
};

class DungeonMap
{
	static DungeonMap* instance;

	struct mapPoint {
		int id;
		mapPointType type;
		int usefulValueA = 0;
		int usefulValueB = 0;
	};

	int numberOfPoints;
	int ** mapLinkData;
	mapPoint * mapPointData;

public:
	DungeonMap();
	~DungeonMap();

	static DungeonMap* getInstance();
	void printDebugMapLinkData();
	void loadMapData();
	void clearMapData();
	std::vector<int> getConnectedID(int index);
	mapPointType getPointType(int index);

};

