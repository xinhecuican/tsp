#ifndef __GENETSP_H__
#define __GENETSP_H__
#include "TSP.h"
#include <algorithm>
#include <iomanip>

class GeneTSP : public TSP
{
public:
	GeneTSP(){}
	~GeneTSP();
	GeneTSP(int population_size, int epoch, float mutation_rate, float elite_rate);
	virtual Route getRoute(FileInfo info, fstream& out);
	virtual void generatePopulation();
	virtual float calcFitness(Route route);
	virtual int selection();
	virtual void crossover(int* parent1, int* parent2, int* path1, int* path2);
	virtual void mutation(int* path);
protected:
	struct CompareStruct{
		float weight;
		int index;
		bool operator<(const CompareStruct& compare)const
		{
			return weight < compare.weight;
		}
	};
	int locationIn(int* path, int search, int size);
	int city_size;
	float** weight;
	vector<Route> population;
	int population_size;
	int epoch;
	float mutation_rate;
	float elite_rate;

private:
	Route generateRoute();
};
#endif