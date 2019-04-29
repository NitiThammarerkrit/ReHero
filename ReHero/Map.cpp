#include "stdafx.h"
#include <iostream>
#include "Map.h"

using namespace std;

DungeonMap* DungeonMap::instance = nullptr;


DungeonMap* DungeonMap::getInstance()
{
	if (instance == nullptr) {
		instance = new DungeonMap();
	}
	return instance;
}

DungeonMap::DungeonMap() {
	loadMapData();
	
	//test print for debugging
	/*cout << endl << "MAP DATA PRINT TEST" << endl;
	printDebugMapLinkData();
	
	{
		cout << "0 connect with";
		vector<int> printTest = getConnectedID(0);
		for (int i = 0; i < printTest.size(); i++) cout << printTest[i] << " ";
		cout << endl;
	}
	
	{
		cout << "14 connect with";
		vector<int> printTest = getConnectedID(14);
		for (int i = 0; i < printTest.size(); i++) cout << printTest[i] << " ";
		cout << endl;
	}

	{
		cout << "5 connect with";
		vector<int> printTest = getConnectedID(5);
		for (int i = 0; i < printTest.size(); i++) cout << printTest[i] << " ";
		cout << endl;
	}

	for (int i = 0; i < numberOfPoints; i++)
	{
		cout << i << " is ";
		mapPointType temp = getPointType(i);
		if (temp == mapPointType::POINT_START) cout << "start ";
		else if (temp == mapPointType::POINT_WEAK_MONSTER) cout << "weak monster ";
		else if (temp == mapPointType::POINT_STRONG_MONSTER) cout << "strong monster ";
		else if (temp == mapPointType::POINT_HEALING_WELL) cout << "healing ";
		else if (temp == mapPointType::POINT_TREASURE_CHEST) cout << "treasure ";
		else if (temp == mapPointType::POINT_BOSS_MONSTER) cout << "boss ";

		cout << "point." << endl;
	}*/
}

DungeonMap::~DungeonMap() {
	clearMapData();
}

void DungeonMap::loadMapData() {
	ifstream mapLinkFile("mapLink.txt");
	ifstream mapPointFile("mapPoint.txt");
	int num;

	if (!mapLinkFile)
	{
		cout << "ERROR: cannot open a mapLink file" << endl;
		return;
	}
	else if (!mapPointFile)
	{
		cout << "ERROR: cannot open a mapPoint file" << endl;
		return;
	}

	clearMapData();
	
	//get number of data
	mapLinkFile >> num;
	numberOfPoints = num;

	//generate containers
	mapLinkData = new int*[numberOfPoints];
	for (int i = 0; i < num; i++) mapLinkData[i] = new int[numberOfPoints];
	mapPointData = new mapPoint[numberOfPoints];

	//get point link
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			mapLinkFile >> mapLinkData[i][j];
		}
	}

	//get point type
	int value1, value2;
	for (int i = 0; i < numberOfPoints; i++)
	{
		mapPointFile >> value1 >> value2;

		mapPointData[i].id = value1;
		switch (value2)
		{
			case 0: mapPointData[i].type = mapPointType::POINT_START; break;
			case 1: mapPointData[i].type = mapPointType::POINT_WEAK_MONSTER; break;
			case 2: mapPointData[i].type = mapPointType::POINT_STRONG_MONSTER; break;
			case 3: mapPointData[i].type = mapPointType::POINT_HEALING_WELL; break;
			case 4: mapPointData[i].type = mapPointType::POINT_TREASURE_CHEST; break;
			case 5: mapPointData[i].type = mapPointType::POINT_BOSS_MONSTER; break;
		}
	}

	//close files
	mapLinkFile.close();
	mapPointFile.close();
}

void DungeonMap::clearMapData() {
	for (int i = 0; i < numberOfPoints; i++)
	{
		delete[] mapLinkData[i];
	}
	delete[] mapLinkData;

	for (int i = 0; i < numberOfPoints; i++)
	{
		delete[] mapPointData;
	}
}

void DungeonMap::printDebugMapLinkData() {
	for (int i = 0; i < numberOfPoints; i++)
	{
		for (int j = 0; j < numberOfPoints; j++)
		{
			cout << mapLinkData[i][j] << " ";
		}
		cout << endl;
	}
}

vector<int> DungeonMap::getConnectedID(int index) {
	vector<int> temp;
	for (int i = 0; i < numberOfPoints; i++)
	{
		if (mapLinkData[index][i] == 1) temp.push_back(i);
	}

	return temp;
}

mapPointType DungeonMap::getPointType(int index) {
	return mapPointData[index].type;
}