#include <iostream>
using namespace std;
#include "utils.h"
#include "MCCOTSP.h"
#include "PMTSP.h"

string datasets[11] = {"pr76", "a280"};

int main(int argc, char** argv)
{
	if(argc < 2)
		return -1;
	TSP* tsp;
	if(strcmp(argv[1], "MCCO") == 0)
	{
		tsp = new MCCOTSP(76, 500, 0.5, 0.1);
	}
	else if(strcmp(argv[1], "PM") == 0)
	{
		tsp = new PMTSP(500, 0.5, 0.1);
	}
	else
	{
		tsp = new GeneTSP(76, 500, 0.5, 0.1);
	}

	fstream file;
	file.open("print.txt", ios::out | ios::trunc);
	for(int i=0; i<2; i++)
	{
		cout << "dataset:\t" << datasets[i] << endl;
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