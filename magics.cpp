#include "defs.h"
#include <iostream>
#include <fstream>

using namespace std;
bool GenMagics(int k, BITMAP magicnum, BITMAP magics[], int shifts[], vector<BITMAP> occupancies[], vector<BITMAP> attackset[], vector<BITMAP> moves[])
{
	moves[k].resize(1 << shifts[k]);
	for(int y = 0; y < (1 << shifts[k]); y++)
	{
		moves[k][y] = BITMAP(-1);
	}
	int i;
	for(i = 0; i < (1 << shifts[k]); i++)
	{
		int index = (int)((occupancies[k][i] * magicnum) >> (64 - shifts[k]));
		if(moves[k][index] == BITMAP(-1))
		{
			moves[k][index] = attackset[k][i];
		}
		else 
		{
			if (moves[k][index] != attackset[k][i])
				return false;
		}
	}
	if (i == (1 << shifts[k]))
	{
		magics[k] = magicnum;
		return true;
	}
}

void PrintMagics(BITMAP magics[], ofstream& myfile)
{
	for(int i = 0; i < 64; i++)
	{
		myfile << "0x" << hex << magics[i] << ", " << endl;
	}
}