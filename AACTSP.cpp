#include "AACTSP.h"
#include <iomanip>

AACTSP::AACTSP(int ant_count, int epoch, float alpha, float beta, float q, float rho,
	float init_temperature, float final_temperature, float mutation_rate, float cool_rate)
{
	this->ant_count = ant_count;
	this->epoch = epoch;
	this->alpha = alpha;
	this->beta = beta;
	this->q = q;
	this->rho = rho;
	this->init_temperature = init_temperature;
	this->final_temperature = final_temperature;
	this->mutation_rate = mutation_rate;
	this->cool_rate = cool_rate;
	pheromone = NULL;
	srand(time(0));
}

AACTSP::~AACTSP()
{
	if(pheromone != NULL)
	{
		for(int i=0; i<city_size; i++)
		{
			delete [] pheromone[i];
			delete [] eta[i];
		}
		delete [] pheromone;
		delete [] eta;
	}
	clearAnts();
}

void AACTSP::generateAnts()
{
	clearAnts();
	for(int i=0; i<ant_count; i++)
	{
		Ant ant;
		ant.weight = 0;
		ant.path = new int[city_size];
		ant.map = new short[city_size];
		ants.push_back(ant);
	}

	pheromone = new float*[city_size];
	eta = new float*[city_size];
	for(int i=0; i<city_size; i++)
	{
		pheromone[i] = new float[city_size];
		eta[i] = new float[city_size];
	}
	for(int i=0; i<city_size; i++)
	{
		for(int k=0; k<city_size; k++)
		{
			pheromone[i][k] = 1;
			eta[i][k] = 1 / weight[i][k];
		}
	}
}

Route AACTSP::getRoute(FileInfo info, fstream& out)
{
	if(pheromone != NULL)
	{
		for(int i=0; i<city_size; i++)
		{
			delete [] pheromone[i];
			delete [] eta[i];
		}
		delete [] pheromone;
		delete [] eta;
		pheromone = NULL;
		eta = NULL;
	}
	city_size = info.dimension;
	weight = info.weight;
	generateAnts();
	float best_weight = 1000000000;
	for(int i=0; i<epoch; i++)
	{
		initAnts();
		journey();
		int best_index = update();
		out << "epoch: " << i << "\t" << "best weight: " << fixed << setprecision(3) << ants[best_index].weight << endl;
		if(ants[best_index].weight < best_weight)
			best_weight = ants[best_index].weight;
	}
	
	float min_weight = best_weight;
	float min_index = 0;
	for(int i=0; i<ant_count; i++)
	{
		if(ants[i].weight < min_weight)
		{
			min_weight = ants[i].weight;
			min_index = i;
		}
	}
	Route route;
	route.path = ants[min_index].path;
	route.weight = min_weight;
	return route;
}

void AACTSP::clearAnts()
{
	for(int i=0; i<ants.size(); i++)
	{
		delete [] ants[i].path;
		delete [] ants[i].map;
	}
	ants.clear();
}

void AACTSP::initAnts()
{

	for(int i=0; i<ant_count; i++)
	{
		for(int k=0; k<city_size; k++)
			ants[i].map[k] = -1;
		int random_index = rand() % city_size;
		ants[i].path[0] = random_index;
		ants[i].map[random_index] = 0;
		ants[i].weight = 0;
	}
}

void AACTSP::journey()
{
	struct DoubleCompareStruct
	{
		double weight;
		int index;
		DoubleCompareStruct(){weight=0;index=0;}
		DoubleCompareStruct(int index, double weight)
		{
			this->index = index;
			this->weight = weight;
		}
		bool operator<(const DoubleCompareStruct& compare)const
		{
			return weight > compare.weight;
		}
	};
	for(int i=1; i<city_size; i++)
	{
		int next_index = (i + 1) % city_size;
		for(int k=0; k<ant_count; k++)
		{
			vector<DoubleCompareStruct> unvisit;
			for(int j=0; j<city_size; j++)
			{
				if(ants[k].map[j] == -1)
				{
					double value = (double)pow((double)pheromone[ants[k].path[i-1]][j], (double)alpha) * (double)pow((double)eta[ants[k].path[i-1]][j], (double)beta);
					unvisit.push_back(DoubleCompareStruct(j, value));
				}
			}
			vector<DoubleCompareStruct> select_unvisit;
			for(int i=0; i<unvisit.size(); i++)
			{
				float rate = (rand() / (double)RAND_MAX);
				if(rate > 0.5)
				{
					select_unvisit.push_back(unvisit[i]);
				}
			}
			sort(select_unvisit.begin(), select_unvisit.end());
			if(select_unvisit.size() != 0)
			{
				ants[k].path[i] = select_unvisit[0].index;
				ants[k].map[select_unvisit[0].index] = i;
				ants[k].weight += weight[ants[k].path[i-1]][select_unvisit[0].index];
			}
			else
			{
				sort(unvisit.begin(), unvisit.end());
				ants[k].path[i] = unvisit[0].index;
				ants[k].map[unvisit[0].index] = i;
				ants[k].weight += weight[ants[k].path[i-1]][unvisit[0].index];
			}
		}
	}
	for(int k=0; k<ant_count; k++)
	{
		ants[k].weight += weight[ants[k].path[city_size-1]][ants[k].path[0]];
	}
}

int AACTSP::update()
{
	// 算法改进点
	float max_weight=0, min_weight=10000000, average_weight=0;
	float min_index=0;
	for(int i=0; i<ant_count; i++)
	{
		average_weight += ants[i].weight;
		if(ants[i].weight > max_weight)
		{
			max_weight = ants[i].weight;
		}
		if(ants[i].weight < min_weight)
		{
			min_weight = ants[i].weight;
			min_index = i;
		}
	}
	average_weight = average_weight / ant_count;

	float ed = (average_weight - min_weight) / (max_weight - min_weight);
	for(int i=0; i<ant_count; i++)
	{
		float rate = (rand() / (double)RAND_MAX);
		int index1 = rand() % city_size;
		int index2 = rand() % city_size;
		while(index1 == index2)
			index2 = rand() % city_size;
		int next1 = (index1 + 1) % city_size;
		int next2 = (index2 + 1) % city_size;
		float cost = weight[ants[i].path[index2]][ants[i].path[next1]] + 
					weight[ants[i].path[index1]][ants[i].path[next2]] - 
					weight[ants[i].path[index1]][ants[i].path[next1]] - 
					weight[ants[i].path[index2]][ants[i].path[next2]];
		if(rate < mutation_rate && ed > 0.5)
		{
			float current = init_temperature;
			while(current > final_temperature)
			{
				float rand_float = (rand() / (double)RAND_MAX);
				if(cost < 0 || pow(2.71828, -cost/current) > rand_float)
				{
					int tmp = ants[i].path[index1];
					ants[i].path[index1] = ants[i].path[index2];
					ants[i].path[index2] = tmp;
					ants[i].weight += cost;
				}
				current = current * cool_rate;
			}
		}
		else if(rate < mutation_rate && ed <= 0.5)
		{
			int tmp = ants[i].path[index1];
			ants[i].path[index1] = ants[i].path[index2];
			ants[i].path[index2] = tmp;
			ants[i].weight += cost;
		}
	}

	float delta_pheromone[city_size][city_size];
	for(int i=0; i<city_size; i++)
	{
		for(int k=0; k<city_size; k++)
			delta_pheromone[i][k] = 0;
	}
	for(int i=0; i<ant_count; i++) // 更新信息素
	{
		for(int k=1; k<city_size; k++)
		{
			delta_pheromone[ants[i].path[k-1]][ants[i].path[k]] += q / ants[i].weight; 
		}
		delta_pheromone[ants[i].path[city_size-1]][ants[i].path[0]] += q / ants[i].weight;
	}

	for(int i=0; i<city_size; i++)
	{
		for(int k=0; k<city_size; k++)
		{
			pheromone[i][k] = (1 - rho) * pheromone[i][k] + delta_pheromone[i][k];
		}
	}
	return min_index;
}