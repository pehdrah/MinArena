#include "objectStats.h"
#include<fstream>
#include<string>
#include<sstream>

using namespace std;

ObjectStats* statsTable;

void startStatsTable()
{
	if(statsTable == 0)
	{
		statsTable = new ObjectStats[512];
	}
}

void loadPrimaryBasics(const char* fileName)
{
	startStatsTable();
	loadPrimaryStats(fileName);
	primaryFinish();
}

void primaryFinish()
{
	int i;
	while(i < 512)
	{
		if(primaryTable[i] != 0)
		{
			statsTable[i].ps = primaryTable[i];	
		}
		i++;
	}
}

/**
* This function is extremely important as it starts the stats table. If this function is not
* called, the game will probably crash.
*
* @author Pedro Araujo Rosa
*/
void loadObjectsBasics(const char* primaryFile, const char* secondFile)
{
	loadPrimaryBasics(primaryFile);
	loadSecondaryStats(secondFile);
}

void loadSecondaryStats(const char* fileName)
{
	ifstream file;
	string line, token;
	int pos, tPos, a;

	startStatsTable();

	file.open(fileName);

	getline(file, line);
	while(!file.eof())
	{
		if(line != "")
		{
			if(line[0] != '#')
			{
				pos = line.find(":");
				token = line.substr(0, pos);
				stringstream(token) >> tPos;
				line = line.substr(pos + 1);

				if(tPos < 512)
				{
					pos = line.find(",");
					token = line.substr(0, pos);
					stringstream(token) >> a;
					statsTable[tPos].ss.melee = a;
					line = line.substr(pos + 1);

					pos = line.find(",");
					token = line.substr(0, pos);
					stringstream(token) >> a;
					statsTable[tPos].ss.defense = a;
					line = line.substr(pos + 1);

					pos = line.find(",");
					token = line.substr(0, pos);
					stringstream(token) >> a;
					statsTable[tPos].ss.topSpeed = a;
					line = line.substr(pos + 1);

					pos = line.find(",");
					token = line.substr(0, pos);
					stringstream(token) >> a;
					statsTable[tPos].ss.startMelee = a;
					line = line.substr(pos + 1);

					pos = line.find(",");
					token = line.substr(0, pos);
					stringstream(token) >> a;
					statsTable[tPos].ss.meleeTicks = a;
					line = line.substr(pos + 1);

					pos = line.find(";");
					token = line.substr(0, pos);
					stringstream(token) >> a;
					statsTable[tPos].ss.hittedTicks = a;

					statsTable[tPos].ss.ticks = 0;
				}
			}
		}
		getline(file, line);
	}
}

/**
* Function to copy objectStats from *src to *dst
*/
void copyObjectStats(ObjectStats *src, ObjectStats *dst)
{
	if(dst->ps == 0)
	{
		dst->ps = new PrimaryStats();
	}

	copyPrimaryStats(src->ps, dst->ps);
	copySecondaryStats(&(src->ss), &(dst->ss));
}

/**
* Function to copy secondary stats from *src to *dst
*/
void copySecondaryStats(SecondaryStats *src, SecondaryStats *dst)
{
	dst->melee = src->melee;
	dst->defense = src->defense;
	dst->topSpeed = src->topSpeed;
	dst->meleeTicks = src->meleeTicks;
	dst->hittedTicks = src->hittedTicks;
	dst->ticks = src->ticks;
	dst->startMelee = src->startMelee;
}

/**
* This function is suppose to get a number and find an object associated with it to
* produce a new instance of an object with the correct parameters.
*/
ObjectStats craftObjectStats(int id)
{
	ObjectStats os;
	os.ps = new PrimaryStats();
	copyObjectStats((statsTable + id), &os);
	return os;
}
