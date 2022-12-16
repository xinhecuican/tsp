#include "MCCOTSP.h"

MCCOTSP::MCCOTSP(int population_size, int epoch, float mutation_rate, float elite_rate)
	:GeneTSP(population_size, epoch, mutation_rate, elite_rate)
{
	srand(time(0));
}

MCCOTSP::~MCCOTSP()
{

}

void MCCOTSP::crossover(int* parent1, int* parent2, int* path1, int* path2)
{
	int count1 = 0;
	int count2 = 0;

	path1[count1++] = parent2[0];
	bool map[city_size];
	bool consider2 = true;
	int parent_path1[city_size+1];
	int parent_path2[city_size+1];
	int current_size = city_size;
	int select_begin = 0;
	for(int i=0; i<city_size; i++)
	{
		parent_path1[i] = parent1[i];
		parent_path2[i] = parent2[i];
		map[i] = false;
	}
	while(count1 < city_size || count2 < city_size)
	{
		if(consider2) // step3
		{
			int tmp1 = locationIn(parent_path1, path1[count1-1], current_size);
			int tmp2 = locationIn(parent_path1, parent_path2[tmp1], current_size);
			if(tmp1 == -1 || tmp2 == -1)
			{
				cout << "index error" << endl;
			}
			path2[count2++] = parent_path2[tmp2];
			map[tmp2] = true;
			if(parent_path1[0] == parent_path2[tmp2]) // step5,6
			{
				int index = 0;
				for(int i=0; i<current_size; i++)
				{
					if(!map[i])
						parent_path2[index++] = parent_path2[i];
					else
						map[i] = false;
				}
				current_size = extractUnselect(parent_path1, path1, current_size, select_begin, count1);
				// extractUnselect(parent_path2, path2, current_size, select_begin, count2);
				for(int i=0; i<current_size; i++)
				{
					bool success = false;
					for(int k=0; k<current_size; k++)
					{
						if(parent_path1[i] == parent_path2[k])
						{
							success = true;
							break;
						}
					}
					if(!success)
					{
						for(int j=0; j<current_size; j++)
						{
							path1[count1++] = parent_path1[j];
							path2[count2++] = parent_path2[j];
						}
						goto OUT;
					}
				}
				select_begin = count1;
				path1[count1++] = parent_path2[0];
				consider2 = true;
				continue;
			}

			consider2 = false;
		}
		else // step4
		{
			int tmp1 = locationIn(parent_path1, path2[count2-1], current_size);
			path1[count1++] = parent_path2[tmp1];
			consider2 = true;
		}
	}
OUT:;
}

int MCCOTSP::extractUnselect(int* path, int* extract, int path_size, int extract_begin, int extract_end)
{
	int index = 0;
	for(int i=0; i<path_size; i++)
	{
		bool success = false;
		for(int k=extract_begin; k<extract_end; k++)
		{
			if(path[i] == extract[k])
			{
				success = true;
				break;
			}
		}
		if(!success)
			path[index++] = path[i];
	}
	return index;
}