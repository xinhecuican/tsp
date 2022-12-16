#ifndef __MCCOTSP_H__
#define __MCCOTSP_H__
#include "GeneTSP.h"
#include <vector>

// Genetic Algorithm for Traveling Salesman Problem with Modified Cycle Crossover Operator
class MCCOTSP : public GeneTSP
{
public:
	MCCOTSP(int population_size, int epoch, float mutation_rate, float elite_rate);
	~MCCOTSP();
	void crossover(int* parent1, int* parent2, int* path1, int* path2)override;
private:
	int extractUnselect(int* path, int* extract, int path_size, int extract_begin, int extract_end);
};

#endif