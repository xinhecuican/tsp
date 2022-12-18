#include <iostream>
using namespace std;
#include "utils.h"
#include "MCCOTSP.h"
#include "PMTSP.h"
#include "AACTSP.h"

string datasets[11] = {"eil51", "berlin52", "eil76", "st70", "pr76", "kroA100", "kroC100", "kroD100", "eil101", "lin105"};

int main(int argc, char** argv)
{
	if(argc < 2)
		return -1;
	TSP* tsp;
	int mcco_population = 100;
	int mcco_epoch = 500;
	float mcco_mutate = 0.5;
	float mcco_elite = 0.1;
	int pm_epoch = 500;
	float pm_mutate = 0.5;
	float pm_elite = 0.1;
	int aac_population = 25;
	int aac_epoch = 500;
	float aac_alpha = 1;
	float aac_beta = 2;
	float aac_q = 500000;
	float aac_rho = 0.1;
	float aac_init_tempera = 1000;
	float aac_end_tempera = 100;
	float aac_mutate = 0.5;
	float aac_cool = 0.7;
	if(argc  >= 3)
	{
		mcco_population = atoi(argv[2]);
		pm_epoch = atoi(argv[2]);
		aac_population = atoi(argv[2]);
	}
	if(argc >= 4)
	{
		mcco_epoch = atoi(argv[3]);
		pm_mutate = atof(argv[3]);
		aac_epoch = atoi(argv[3]);
	}
	if(argc >= 5)
	{
		mcco_mutate = atof(argv[4]);
		pm_elite = atof(argv[4]);
		aac_alpha = atof(argv[4]);
	}
	if(argc >= 6)
	{
		mcco_elite = atof(argv[5]);
		aac_beta = atof(argv[5]);
	}
	if(argc >= 7) aac_q = atoi(argv[6]);
	if(argc >= 8) aac_rho = atof(argv[7]);
	if(argc >= 9) aac_init_tempera = atoi(argv[8]);
	if(argc >= 10) aac_end_tempera = atoi(argv[9]);
	if(argc >= 11) aac_mutate = atof(argv[10]);
	if(argc >= 12) aac_cool = atof(argv[11]);

	if(strcmp(argv[1], "MCCO") == 0)
	{
		tsp = new MCCOTSP(mcco_population, mcco_epoch, mcco_mutate, mcco_elite);
	}
	else if(strcmp(argv[1], "PM") == 0)
	{
		tsp = new PMTSP(pm_epoch, pm_mutate, pm_elite);
	}
	else if(strcmp(argv[1], "AAC") == 0)
	{
		tsp = new AACTSP(aac_population, aac_epoch, aac_alpha, aac_beta, aac_q, aac_rho, aac_init_tempera
		, aac_end_tempera, aac_mutate, aac_cool);
	}
	else
	{
		tsp = new GeneTSP(100, 500, 0.5, 0.1);
	}

	fstream file;
	string method(argv[1]);
	file.open("print_" + method + ".txt", ios::out | ios::trunc);
	for(int i=0; i<10; i++)
	{
		file << "dataset:\t" << datasets[i] << endl;
		string path = "ALL_tsp/" + datasets[i] + ".tsp";
		string opt_path = "ALL_tsp/" + datasets[i] + ".opt.tour";
		FileInfo info = parseFile(path);
		Route opt_route = parseOpt(opt_path, info);
		clock_t start, end;
		start = clock();
		Route best_route = tsp->getRoute(info, file);
		end = clock();
		file << (end - start) / 1000.0 << endl;
		file << (float)best_route.weight << " " << (float)opt_route.weight << endl;
		for(int i=0; i<info.dimension; i++)
		{
			file << best_route.path[i] << " ";
		}
		file << endl << endl;
	}
	file.close();
	return 0;
}