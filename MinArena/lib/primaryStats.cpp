#include "primaryStats.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

PrimaryStats **primaryTable;

/**
* This function is extremely important as it starts the stats table. If this function is not
* called, the game will probably crash.
*
* @author Pedro Araujo Rosa
*/
void loadPrimaryStats(const char* fileName)
{
	ifstream file;
	string line, token;
	int pos, tPos, a;

	primaryTable = new PrimaryStats*[512];

	tPos = 0;
	while(tPos < 512)
	{
		primaryTable[tPos] = 0;
		tPos++;
	}

	file.open(fileName);

	getline(file, line);
	while(line != "")
	{
		if(line[0] != '#')
		{
			pos = line.find(":");
			token = line.substr(0, pos);
			stringstream(token) >> tPos;
			line = line.substr(pos + 1);

			if(tPos < 512)
			{
				primaryTable[tPos] = new PrimaryStats;
				pos = line.find(",");
				token = line.substr(0, pos);
				stringstream(token) >> primaryTable[tPos]->box.w;
				line = line.substr(pos + 1);

				pos = line.find(",");
				token = line.substr(0, pos);
				stringstream(token) >> primaryTable[tPos]->box.h;
				line = line.substr(pos + 1);

				pos = line.find(",");
				token = line.substr(0, pos);
				stringstream(token) >> a;
				primaryTable[tPos]->type = a;
				line = line.substr(pos + 1);

				pos = line.find(",");
				token = line.substr(0, pos);
				stringstream(token) >> a;
				primaryTable[tPos]->kind = a;
				line = line.substr(pos + 1);

				pos = line.find(";");
				token = line.substr(0, pos);
				stringstream(token) >> primaryTable[tPos]->maxLife;

				primaryTable[tPos]->box.x = 0;
				primaryTable[tPos]->box.y = 0;
				primaryTable[tPos]->dx = 0;
				primaryTable[tPos]->dy = 0;
				primaryTable[tPos]->state = 0;
				primaryTable[tPos]->life = primaryTable[tPos]->maxLife;
			}
		}
	
		getline(file, line);
	}
}

/**
* Function to copy primary stats from *src to *dst
*/
void copyPrimaryStats(PrimaryStats *src, PrimaryStats *dst)
{
	dst->box.x = src->box.x;
	dst->box.y = src->box.y;
	dst->box.w = src->box.w;
	dst->box.h = src->box.h;
	dst->dx = src->dx;
	dst->dy = src->dy;
	dst->state = src->state;
	dst->type = src->type;
	dst->kind = src->kind;
	dst->maxLife = src->maxLife;
	dst->life = src->life;
}

PrimaryStats craftPrimaryStats(int id)
{
	PrimaryStats stats;

	copyPrimaryStats(*(primaryTable + id), &stats);

	return stats;
}
