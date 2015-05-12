#include "defs.h"
#include "board.h"
#include <iostream>
using namespace std;
bool IsSquareAttacked(int, int, Board* board);
bool IsKingAttacked(int, Board* board);
int MoveGen(int, Board* board,  unsigned int* moves);
void MakeMove(int, Board* board);
void UnmakeMove(int, Board* board);
#define displaymove(a)  cout << squaresarray[GetFrom(a)] << squaresarray[GetTo(a)] << endl;


int castlemoves = 0;
int switcht = 1;

unsigned long long Perft(int depth, int ply, Board* board, int startdepth)
{
	unsigned long long perftreturn = 0;

	if(depth == 0) 
		return 1; 
	
	int currply = ply;			
	int nummoves =  MoveGen(board->movebuflen[ply], board, board->movebuffer);
	board->movebuflen[ply+1] = nummoves;
	for(int i = board->movebuflen[currply]; i < board->movebuflen[currply+1]; i++)
	{	
		BITMAP key = board->hashkey;
		BITMAP before = board->allpieces;
		int mat = board->material;
		int currentmove = board->movebuffer[i];
		//if((GetFrom(currentmove) == B2) && (GetTo(currentmove) == A1) && (GetProm(currentmove) == BLACK_ROOK)) cout << "move found from perft is " << currentmove << endl;
		if(currentmove != 0)
		{
			BITMAP currboard  = board -> allpieces;
			MakeMove(currentmove, board);
			int currentperft;
			if(!IsKingAttacked(1^(board->side), board))
			{
				currentperft = Perft(depth-1, currply+1, board, startdepth);
				perftreturn += currentperft;
				if(depth == startdepth)
					cout << squaresarray[GetFrom(board->movebuffer[i])] << squaresarray[GetTo(board->movebuffer[i])] << " " << currentperft << endl;
			}
			UnmakeMove(currentmove, board);

			/*if(!IsKingAttacked(board->side, board) && depth == startdepth)
			{
				displaymove(currentmove);
				cout << endl;
				DisplayBitmap(board->allpieces);
				cout << endl;
			}
			/if(currboard != board->allpieces)
			{
				displaymove(currentmove);
				cout << endl;
				DebugMove(currentmove);
				cout << endl;
			} */
		}
	}

	return perftreturn;


}
