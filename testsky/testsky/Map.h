#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "objloader.h"
using namespace std;
class Map
{
private:
	//vector<char>  g_HeightMap;    // 保存高度数据
	ObjLoader loader;
	double MiddleX;
	double MiddleY;
	double MiddleZ;
public:
	//int step_size=16;
	//int Height_ratio=1.6;
	Map() {};
	void LoadFile(string objfile, string mtlfile);
	int getHeight(int x, int y);
	void Render();
};
