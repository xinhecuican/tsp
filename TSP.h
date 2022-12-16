#ifndef __TSP_H__
#define __TSP_H__
#include "utils.h"

class TSP
{
public:
	virtual Route getRoute(FileInfo info, fstream& out)=0;
};

#endif