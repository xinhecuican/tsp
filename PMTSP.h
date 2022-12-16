#ifndef __PMTSP_H__
#define __PMTSP_H__
#include "GeneTSP.h"

// Permutation rules and genetic algorithm to solve the traveling salesman problem
class PMTSP : public GeneTSP
{
public:
	PMTSP(int epoch, float mutation_rate, float elite_rate);
	Route getRoute(FileInfo info, fstream& out)override;
	void generatePopulation()override;
private:
};
#endif