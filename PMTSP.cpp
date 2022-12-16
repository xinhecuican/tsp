#include "PMTSP.h"

PMTSP::PMTSP(int epoch, float mutation_rate, float elite_rate) : 
	GeneTSP(0, epoch, mutation_rate, elite_rate)
{

}

Route PMTSP::getRoute(FileInfo info, fstream& out)
{
	population_size = info.dimension;
	return GeneTSP::getRoute(info, out);
}

void PMTSP::generatePopulation()
{
	if(population.size() != 0)
	{
		for(int i=0; i<population.size(); i++)
		{
			free(population[i].path);
		}
	}
	population.clear();

	for(int i=0; i<city_size; i++)
	{
		bool map[city_size];
		memset(map, 0, city_size);
		map[i] = true;
		Route route;
		route.path = (int*)malloc(city_size * sizeof(int));
		int current_index = i;
		route.path[0] = i;

		int perm1_index1 = 0;
		int perm1_index2 = 0;
		int perm1_index3 = 0;
		int perm1_index4 = 0;
		float perm1_weight1 = 0;
		float perm1_weight2 = 0;

		for(int k=1; k<city_size; k++)
		{
			float min_weight = 1000000000;
			int select_index = 0;
			for(int j=0; j<city_size; j++)
			{
				if(map[j])
					continue;
				if(weight[current_index][j] < min_weight)
				{
					min_weight = weight[current_index][j];
					select_index = j;
				}
			}
			if(perm1_weight1 < min_weight)
			{
				perm1_weight2 = perm1_weight1;
				perm1_weight1 = min_weight;
				perm1_index3 = perm1_index1;
				perm1_index4 = perm1_index2;
				perm1_index1 = current_index;
				perm1_index2 = select_index;
			}
			else if(perm1_weight2 < min_weight)
			{
				perm1_weight2 = min_weight;
				perm1_index3 = current_index;
				perm1_index4 = select_index;
			}
			current_index = select_index;
			route.weight += min_weight;
			route.path[k] = select_index;
			map[select_index] = true;
		}

		for(int i=1; i<city_size; i++)
		{
			if(route.path[i-1] == perm1_index1 && route.path[i] == perm1_index2)
			{
				route.path[i] = perm1_index4;
			}
			if(route.path[i-1] == perm1_index3 && route.path[i] == perm1_index4)
			{
				route.path[i] = perm1_index2;
			}
		}
		route.weight += weight[perm1_index1][perm1_index4] + weight[perm1_index3][perm1_index2] - perm1_weight1 - perm1_weight2;
		population.push_back(route);
	}

}