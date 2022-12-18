#ifndef __AACTSP_H__
#define __AACTSP_H__
#include "TSP.h"

// Annealing Ant Colony Optimization with Mutation Operator for Solving TSP
class AACTSP : public TSP
{
public:
	struct Ant
	{
		float weight;
		int* path;
		short* map;
	};
	/// @brief 
	/// @param ant_count 
	/// @param epoch 
	/// @param alpha 
	/// @param beta 
	/// @param q 信息素强度系数
	/// @param rho 信息素衰减系数
	/// @param init_temperature 模拟退火算法初始温度
	/// @param final_temperature 模拟退火算法结束温度
	/// @param mutation_rate 突变概率
	/// @param cool_rate 模拟退火算法冷却系数
	AACTSP(int ant_count, int epoch, float alpha, float beta, float q, float rho,
		float init_temperature, float final_temperature, float mutation_rate, float cool_rate);
	~AACTSP();
	Route getRoute(FileInfo info, fstream& out)override;
	void generateAnts();
	void journey();
	int update();
private:
	void clearAnts();
	void initAnts();
	vector<Ant> ants;
	int ant_count;
	int epoch;
	float alpha;
	float beta;
	float init_temperature;
	float final_temperature;
	float mutation_rate;
	float cool_rate;
	float q;
	float rho;
	int city_size;
	float** weight;
	float** eta; // 距离倒数
	float** pheromone;
};
#endif