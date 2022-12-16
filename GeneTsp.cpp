#include "GeneTsp.h"

GeneTSP::GeneTSP(int population_size, int epoch, float mutation_rate, float elite_rate)
{
	this->population_size = population_size;
	this->epoch = epoch;
	this->mutation_rate = mutation_rate;
	this->elite_rate = elite_rate;
	srand(time(0));
}

GeneTSP::~GeneTSP()
{
	for(int i=0; i<population.size(); i++)
	{
		free(population[i].path);
	}
}

Route GeneTSP::getRoute(FileInfo info, fstream& out)
{
	city_size=info.dimension;
	weight=info.weight;
	generatePopulation();
	for(int i=0; i<epoch; i++)
	{
		// wheelSelectionPrepare();
		vector<Route> children;
		for(int k=0; k<population_size && k+1<population_size; k+=2)
		{
			int parent1 = selection();
			int parent2 = selection();
			// while(parent1 == parent2)
			// 	parent2 = selection();

			int* path1 = (int*)malloc(city_size * sizeof(int));
			int* path2 = (int*)malloc(city_size * sizeof(int));
			crossover(population[parent1].path, population[parent2].path, path1, path2);
			mutation(path1);
			mutation(path2);
			Route child1(path1);
			Route child2(path2);
			child1.weight = calcFitness(child1);
			child2.weight = calcFitness(child2);
			children.push_back(child1);
			children.push_back(child2);
		}
		sort(population.begin(), population.end());
		sort(children.begin(), children.end());
		int parent_size = population_size * elite_rate;
		int index = 0;
		for(int k=parent_size; k<population_size; k++)
		{
			// free(population[k].path);
			population[k].path = children[index].path;
			population[k].weight = children[index].weight;
			index++;
		}
		// for(int k=index; k<children.size(); k++)
		// {
		// 	free(children[k].path);
		// }
		out << "epoch: " << i << "\t" << "best weight: " << fixed << setprecision(3) << (population[0].weight < children[0].weight ? population[0].weight : children[0].weight) << endl;
	}

	float best = 1000000000;
	int best_index = 0;
	for(int i=0; i<population_size; i++)
	{
		if(population[i].weight < best)
		{
			best = population[i].weight;
			best_index = i;
		}
	}
	return population[best_index];
}

void GeneTSP::generatePopulation()
{
	if(population.size() != 0)
	{
		for(int i=0; i<population.size(); i++)
		{
			free(population[i].path);
		}
	}
	population.clear();
	for(int i=0; i<population_size; i++)
	{
		population.push_back(generateRoute());
	}
}

Route GeneTSP::generateRoute()
{
	Route route;
	int* path = (int*)malloc(city_size * sizeof(int));;
	for(int i=0; i<city_size; i++)
	{
		path[i] = i;
	}
	random_shuffle(path, path+city_size);

	route.path = path;
	route.weight = calcFitness(route);
	return route;
}

float GeneTSP::calcFitness(Route route)
{
	float res = weight[route.path[city_size-1]][0];
	for(int i=1; i<city_size; i++)
	{
		res += weight[route.path[i-1]][route.path[i]];
	}
	return res;
}
int GeneTSP::selection()
{
	vector<CompareStruct> mating;
	for(int i=0; i<population_size; i++)
	{
		float mutation_select = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
		if(mutation_select > 0.5)
		{
			CompareStruct ele;
			ele.weight = population[i].weight;
			ele.index = i;
			mating.push_back(ele);
		}
	}
	sort(mating.begin(), mating.end());
	return mating[0].index;
}

int GeneTSP::locationIn(int* path, int search, int current_size)
{
	for(int i=0; i<current_size; i++)
	{
		if(path[i] == search)
			return i;
	}
	return -1;
}

void GeneTSP::crossover(int* parent1, int* parent2, int* path1, int* path2)
{
	for(int i=0; i<city_size; i++)
	{
		path1[i] = parent2[i];
		path2[i] = parent1[i];
	}
	int current = 0;
	int next_index = 0;
	path1[current] = parent1[current];
	path2[current] = parent2[current];
	bool map[city_size];
	for(int i=0; i<city_size; i++)
	{
		map[i] = false;
	}

	while(next_index != -1 && !map[next_index])
	{
		map[next_index] = true;
		next_index = locationIn(parent1, parent2[current], city_size);
		path1[next_index] = parent1[next_index];
		path2[next_index] = parent2[next_index];
		current = next_index;
	}

}

void GeneTSP::mutation(int* path)
{
	float mutation_select = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	if(mutation_select > mutation_rate)
		return;
	int index1 = rand() % city_size;
	int index2 = rand() % city_size;
	while(index1 == index2)
		index2 = rand() % city_size;
	int tmp = path[index1];
	path[index1] = path[index2];
	path[index2] = tmp;
}