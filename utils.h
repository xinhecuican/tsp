#ifndef __UTILS_H__
#define __UTILS_H__
#include <iostream>
#include <regex>
#include <fstream>
#include <cmath>
#include <random>
#include <ctime>
#include <cstdlib>
using namespace std;

enum TSP_TYPE{TYPE_TSP, TYPE_ATSP};
enum EDGE_WEIGHT_TYPE{EXPLICIT, EUC_2D, EUC_3D, MAX_2D, MAX_3D, MAN_2D, MAN_3D, CEIL_2D, GEO, ATT};

struct FileInfo
{
	string name;
	string comment;
	TSP_TYPE type;
	int dimension;
	EDGE_WEIGHT_TYPE weight_type;
	float** coord;
	float** weight;
	void release()
	{
		for(int i=0; i<dimension; i++)
		{
			free(coord[i]);
			free(weight[i]);
		}
		free(coord);
		free(weight);
	}
};

struct Route
{
	float weight;
	int* path;
	Route(){weight=0;path=NULL;}
	Route(int* path)
	{
		this->path = path;
	}

	Route(float weight, int* path)
	{
		this->weight = weight;
		this->path = path;
	}

	bool operator<(const Route& compare)const
	{
		return weight < compare.weight;
	}
};

static float calDistance(EDGE_WEIGHT_TYPE type, float* point1, float* point2)
{
	float ans;
	switch(type)
	{
		case EUC_2D:
			float xd = point1[0] - point2[0];
			float yd = point1[1] - point2[1];
			ans = sqrt( xd*xd + yd*yd );
			break;
	}
	return ans;
}

static void shuffle(int* a, int len)
{
	for(int i=0; i<len; i++)
	{
		int index1 = rand() % len;
		int index2 = rand() % len;
		int tmp = a[index1];
		a[index1] = a[index2];
		a[index2] = tmp;
	}
}

static FileInfo parseFile(string path)
{
	FileInfo info;
	fstream file;
	file.open(path, ios::in);
	while(!file.eof())
	{
		string line;
		getline(file, line);
		if(line.compare("NODE_COORD_SECTION") == 0)
			break;
		bool key_pass = false;
		string key;
		string value;
		for(int i=0; i<line.size(); i++)
		{
			if(line[i] == ' ' || line[i] == ':')
			{
				key_pass = true;
				continue;
			}
			if(!key_pass)
				key.push_back(line[i]);
			else
				value.push_back(line[i]);
		}
		if(key == "NAME")
		{
			info.name = value;
		}
		else if(key == "COMMENT")
		{
			info.comment = value;
		}
		else if(key == "EDGE_WEIGHT_TYPE")
		{
			if(value == "EXPLICIT")
				info.weight_type = EXPLICIT;
			else if(value == "EUC_2D")
				info.weight_type = EUC_2D;
			else if(value == "EUC_3D")
				info.weight_type = EUC_3D;
			else if(value == "MAX_2D")
				info.weight_type = MAX_2D;
			else if(value == "MAX_3D")
				info.weight_type = MAX_3D;
			else if(value == "MAN_2D")
				info.weight_type = MAN_2D;
			else if(value == "MAN_3D")
				info.weight_type = MAN_3D;
			else if(value == "CEIL_2D")
				info.weight_type = CEIL_2D;
			else if(value == "GEO")
				info.weight_type = GEO;
			else if(value == "ATT")
				info.weight_type = ATT;
		}
		else if(key == "TYPE")
		{
			if(value == "TSP")
				info.type = TYPE_TSP;
			else
				info.type = TYPE_ATSP;
		}
		else if(key == "DIMENSION")
			info.dimension = stoi(value);
	}

	info.coord = (float**)malloc(info.dimension * sizeof(float*));
	for(int i=0; i<info.dimension; i++)
		info.coord[i] = (float*)malloc(2 * sizeof(float)); // 只考虑二维情况
	int index = 0;
	while(!file.eof())
	{
		string line;
		getline(file, line);
		if(line == "EOF")
			break;
		int s = line.find_first_not_of(" ");
		line = line.substr(s, line.size() - s + 1);
		int l1 = line.find(" ");
		int l2 = line.rfind(" ");
		info.coord[index][0] = stoi(line.substr(l1, l2-l1+1));
		info.coord[index][1] = stoi(line.substr(l2, line.size()-l2+1));
		index++;
	}

	info.weight = (float**)malloc(info.dimension * sizeof(float*));
	for(int i=0; i<info.dimension; i++)
		info.weight[i] = (float*)malloc(info.dimension * sizeof(float));
	for(int i=0; i<info.dimension; i++)
	{
		for(int k=0; k<info.dimension; k++)
		{
			info.weight[i][k] = calDistance(info.weight_type, info.coord[i], info.coord[k]);
		}
	}
	file.close();
	return info;
}

static Route parseOpt(string path, FileInfo info)
{
	Route route;
	route.path = (int*)malloc(info.dimension * sizeof(int));
	fstream file;
	file.open(path, ios::in);
	while(!file.eof())
	{
		string line;
		getline(file, line);
		if(line == "TOUR_SECTION")
			break;
	}
	int index = 0;
	route.weight = 0;
	while(!file.eof())
	{
		string line;
		getline(file, line);
		if(line == "-1" || line == "EOF")
			break;
		route.path[index++] = stoi(line);
	}
	for(int i=1; i<info.dimension; i++)
	{
		route.weight += info.weight[route.path[i-1]-1][route.path[i]-1];	
	}
	file.close();
	return route;
}
#endif