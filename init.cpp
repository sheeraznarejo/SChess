#include "defs.h"
#include "board.h"
#include <sys/time.h>
#include <iostream>
using namespace std;
int MoveGen(int, Board* b, unsigned int* moves);
void MakeMove( int, Board* board);
void UnmakeMove(int, Board* board);
unsigned char KINGSIDECASTLE = 1;
unsigned char QUEENSIDECASTLE = 2;
bool IsSquareAttacked(int side, int square, Board* board);
bool IsKingAttacked(int side, Board* board);
int Evaluate(Board* board);
int mate = -30000;
int bestwhite = -INFINITY;
int bestblack = -INFINITY;
int RANKS[64] = 
{ 	
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8
};


int FILES[64] = 
{
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8,
	1, 2, 3, 4, 5, 6, 7, 8
};

string squaresarray[64] = 
{
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

BITMAP BMAGICS[64] =  {
0x1208011020810100, 0x9100228010c2000, 0x4010012441020000, 0x208208030008004, 0x6121020010011, 0x4211100804000204, 0x2800410820100200, 0xa0100608a201020, 0x41048c048200, 0x1000062206040113, 0x403108112410018, 0x400080485000080, 0x420210004a44, 0x120184210000, 0x410a0400940c2102, 0x204050400840404, 0x4020041005050800, 0x8007012008c00, 0x8004801000220140, 0x200804440c200900, 0x4002200a22800, 0x706480010180c002, 0x100883200908810, 0xa21040c034040c00, 0x210a100088101008, 0x40041006020a0800, 0xc04084004014c00, 0x2001828008020002, 0x301000050c000, 0x2004200d1018200, 0x2240002051324, 0x8004102402410410, 0x24044100a00300, 0x1489198810200804, 0x804050040408, 0x108020082280080, 0x840008020020120, 0x180a8100080900, 0x8848124050140900, 0xc401011206011040, 0x2104022090010600, 0x252081148020400, 0x8202030050800, 0x800022033000800, 0x200844300c000081, 0x1104100400a00, 0x8004180200400400, 0x8048880100402023, 0x1881510100400, 0x4086021202122002, 0x810240a080004, 0x40000020a0881264, 0xd1000910461a0000, 0x20010204802c604, 0x8008080888004220, 0x4484302401002044, 0x5140208028800, 0x12006004e321120, 0x86040012d080800, 0x100002205040a, 0x1c0000290020208, 0x24240c010020980, 0x8008108401082208, 0x40082109002d00};

BITMAP RMAGICS[64] = {
0x4280008050400020, 0x2cc0004020025000, 0x2100089040a00100, 0x2080245000880080, 0x85000300040800d0, 
0x480250600040080, 0x100040081000600, 0x580010008402080, 0x810a800060804000, 0x408680400080e000, 
0x1202002280120140, 0x1009003000200d00, 0x800400880080, 0x4302001416004830, 0x53000200090004,
0x8841000c45000992, 0x922248000804002, 0x100640002005c4, 0x1020004040100800, 0x40808008001000, 
0x2001010010349800, 0x8044008002004480, 0xc050100020004, 0x200a0000950144, 0x12208a80014000, 
0x1400220200410080, 0x100280200080, 0x2023001000a2100, 0xc8080080040080, 0x4a80520080040080, 0x640080400013a10,
0x200080218003c100, 0x40048420800448, 0x800c00084802008, 0x8480d00880802000, 0x1800800803000, 0x8002014040040,
0x1006813000c00, 0x10114864000e10, 0x2000084402000081, 0x8060219040008008, 0xa400400900850022, 
0x800406001010010, 0x14001001009b0020, 0x4000180100910014, 0xa00040002008080, 0x2408010002008080, 
0x940000c104820024, 0x200660885004200, 0x640200440108080, 0x10090c901200100, 0x248000810008080,
0x802400080080, 0x20800400020080, 0x8000100211080400, 0x4008008401044200, 0x1820410880120022, 
0x10021080420101a2, 0xc000a2000410113, 0x4001001001200489, 0x8002000420110802, 0x1403001208a40001, 
0x8605080a089004, 0x420200a401438112};


BITMAP BITSET[64];
BITMAP ROOKMASK[64];
BITMAP BISHOPMASK[64];
BITMAP RANKMASKS[8];
BITMAP FILEMASKS[8];
BITMAP ROOKMOVES[64][4096];
BITMAP BISHOPMOVES[64][4096];
BITMAP KNIGHTMOVES[64];
BITMAP KINGMOVES[64];
BITMAP WHITEPAWNMOVES[64];
BITMAP BLACKPAWNMOVES[64];
BITMAP WHITEPAWNATTACKS[64];
BITMAP BLACKPAWNATTACKS[64];
// castling keys 0 = 
int RMASKSIZES[64];
int BMASKSIZES[64];
unsigned int moves[5000];
int bestmoveoverall;



int rftosq(int rank, int file)
{
	return rank * 8 + file - 9;
}



void FillRankMasks()
{
	BITMAP a = 1;
	for(int i = 0; i<7; i++)
	{
		a = a | (a << 1);
	}
	for(int i =0; i<8; i++)
	{
		RANKMASKS[i] = a << (8 * i);
	}
}

void FillFileMasks()
{
	for(int j = 0; j <8; j++)
	{
		BITMAP a = 1;
		a = a << j;
		for(int i =0; i < 7; i++)
		{
			a |= (a << 8);
		}
		FILEMASKS[j] = a;
	}
}


void FillBitSets()
{
	BITMAP a = 1;
	for(int i =0; i<64; i++)
	{
		BITSET[i] = a;
		a = a << 1;
	}
}

void FillBishopMasks()
{

	for(int i = 0; i< 64; i++)
	{
		BITMAP mask = 0;
		int r = RANKS[i];
		int c = FILES[i];
		int s = 0;
		while(r <=6 && c <= 6)
		{
			mask |= BITSET[rftosq(r+1, c+1)];
			r++; c++; s++;
		}
		r = RANKS[i];
		c = FILES[i];
		while(r <= 6 && c >= 3)
		{
			mask |= BITSET[rftosq(r+1, c-1)];
			r++; c--; s++;
		}
		r = RANKS[i];
		c = FILES[i];
		while(r >= 3 && c <= 6)
		{
			mask |= BITSET[rftosq(r-1, c+1)];
			r--;
			c++;
			s++;
		}
		r = RANKS[i];
		c = FILES[i];
		while(r >= 3 && c >=3)
		{
			mask|= BITSET[rftosq(r-1, c-1)];
			r--;
			c--;
			s++;
		}
		BISHOPMASK[i] = mask;
	}
}


void FillRookMasks()
{
	for(int i = 0; i < 64; i++)
	{
		int s = 0;
		int rank = RANKS[i];
		int file = FILES[i];
		BITMAP a = BITSET[i];
		while(file >= 3)
		{
			a = a|(a >> 1);
			file--;
			s++;
		}
		BITMAP b = BITSET[i];
		file = FILES[i];
		while(file <=6)
		{
			b = b | (b << 1);
			file++;
			s++;
		}
		BITMAP c = BITSET[i];
		while(rank >= 3)
		{
			c = c|(c >> 8);
			rank--;
			s++;
		}
		rank = RANKS[i];
		BITMAP d = BITSET[i];
		while(rank <=6)
		{
			d = d|(d << 8);
			rank++;
			s++;
		}
		ROOKMASK[i] = (a | b |c |d) ^ BITSET[i];
	}
}

void FillBROccs(vector<BITMAP> occs[], bool rook )
{
	for(int i =0; i<64; i++)
	{
		int s = (rook) ? RMASKSIZES[i] : BMASKSIZES[i];
		vector<int> onebitsloc;
		BITMAP a = (rook) ? ROOKMASK[i] : BISHOPMASK[i];
		while(a != 0)
		{
			int b = PopBit(&a);
			onebitsloc.push_back(b);
		}

		int k = onebitsloc.size();
		//occs[i].resize(1 << (rook ? RMASKSIZES[i] : BMASKSIZES[i]));
		int p = 0;
		for(; p < (1 << s); p++)
		{

			BITMAP occup = 0;
			for(int x =0; x <k; x++)
			{
				if((p >> x) & 1) occup |= ((BITMAP(1) << onebitsloc[x]));
			}
			occs[i].push_back(occup);
		}
	}

}

void FillRAttackSets(vector<BITMAP> occs[], vector<BITMAP> attks[])
{
	for(int i =0; i < 64; i++)
	{
		for(int j = 0; j <  (1 << RMASKSIZES[i]); j++)
		{
			BITMAP attackset = 0;
			BITMAP a = occs[i][j];
			int r = RANKS[i];
			int f = FILES[i];
			while((f+1 <= 8) && (BITSET[rftosq(r, f+1)] & a) == 0)
			{
				attackset |= BITSET[rftosq(r,f+1)];
				f++;
			}
			if(f+1 <= 8) attackset |= BITSET[rftosq(r,f+1)];
			f = FILES[i];
			while((f-1 >= 1) && (BITSET[rftosq(r,f-1)] & a) == 0)
			{
				attackset |= BITSET[rftosq(r, f-1)];
				f--;
			}
			if(f-1 >=1) attackset |= BITSET[rftosq(r, f-1)];
			f = FILES[i];
			while((r+1 <= 8) && (BITSET[rftosq(r+1, f)] & a) == 0)
			{
				attackset |= BITSET[rftosq(r+1,f)];
				r++;
			}
			if(r+1 <=8) attackset |= BITSET[rftosq(r+1,f)];
			r = RANKS[i];
			while((r-1 >= 1) && (BITSET[rftosq(r-1,f)] & a) == 0)
			{
				attackset |= BITSET[rftosq(r-1, f)];
				r--;
			}
			if(r-1 >=1) attackset |= BITSET[rftosq(r-1, f)];
			attks[i].push_back(attackset);
		}
	}
}

void FillBAttackSets(vector<BITMAP> occs[], vector<BITMAP> attks[])
{
	for(int i=0; i< 64; i++)
	{
		
		for(int j = 0; j < (1 << BMASKSIZES[i]); j++)
		{
			BITMAP attackset = 0;
			BITMAP a = occs[i][j];
			int r = RANKS[i];
			int f = FILES[i];
			while(r+1 <= 8 && f+1 <= 8 && (BITSET[rftosq(r+1, f+1)] & a) == 0)
			{
				attackset |= BITSET[rftosq(r+1,f+1)];
				f++;
				r++;
			}
			if(r+1 <=8 && f+1 <= 8) attackset|= BITSET[rftosq(r+1,f+1)];
			f = FILES[i];
			r = RANKS[i];
			while(r-1 >=1 && f-1 >=1 && (BITSET[rftosq(r-1,f-1)] & a) ==  0)
			{
				attackset |= BITSET[rftosq(r-1, f-1)];
				f--;
				r--;
			}
			if(r-1 >=1 && f-1 >=1) attackset |= BITSET[rftosq(r-1,f-1)];
			f = FILES[i];
			r = RANKS[i];
			while(r+1 <= 8 && f-1>=1 && (BITSET[rftosq(r+1, f-1)] & a) == 0)
			{
				attackset |= BITSET[rftosq(r+1,f-1)];
				r++;
				f--;
			}
			if(r+1 <=8 && f-1 >=1) attackset |= BITSET[rftosq(r+1,f-1)];
			r = RANKS[i];
			f = FILES[i];
			while(r-1 >=1 && f+1 <= 8 && (BITSET[rftosq(r-1,f+1)] & a) == 0)
			{
				attackset |= BITSET[rftosq(r-1, f+1)];
				r--;
				f++;
			}
			if(r-1 >= 1 && f+1 <= 8) attackset |= BITSET[rftosq(r-1, f+1)];
			r = RANKS[i];
			f = FILES[i];
			attks[i].push_back(attackset);
		}
		
	}
}

void FillRookBishopMoves()
{
	vector<BITMAP> roccs[64];
	vector<BITMAP> boccs[64];
	vector<BITMAP> rattks[64];
	vector<BITMAP> battks[64];
	FillBROccs(roccs,1);
	FillBROccs(boccs,0);
	FillRAttackSets(roccs,rattks);
	FillBAttackSets(boccs,battks);
	for(int i = 0; i < 64; i++)
	{
		for(int j = 0; j < (1 << RMASKSIZES[i]); j++)
		{
			ROOKMOVES[i][(roccs[i][j] * RMAGICS[i]) >> (64 - RMASKSIZES[i])] = rattks[i][j];
		}
		for(int j = 0; j < (1 << BMASKSIZES[i]); j++)
		{
			BISHOPMOVES[i][(boccs[i][j] * BMAGICS[i]) >> (64 - BMASKSIZES[i])] = battks[i][j];
		}
	}
}

void FillKnightMoves()
{
	for(int i = 0; i < 64; i++)
	{
		BITMAP a = BITMAP(0);
		int rank = RANKS[i];
		int file = FILES[i];
		int deltafile[] = {1, 1, 2, 2,-1,-1,-2,-2};
		int deltarank[] = {2,-2,-1, 1, 2,-2, 1, -1};
		for(int j = 0; j < 8; j++)
		{
			int r2 = rank + deltarank[j];
			int f2 = file + deltafile[j];
			if(r2 >= 1 && r2 <=8 && f2 >=1 && f2 <= 8)
			{
				 a |= BITSET[rftosq(r2,f2)];
			}
		}
		KNIGHTMOVES[i] = a;

	}
}

void FillWhitePawnMoves()
{
	for(int i = 0; i < 64; i++)
	{
		BITMAP attacks = 0;
		int rank = RANKS[i];
		int file = FILES[i];
		if(rank + 1 <= 8) attacks |= BITSET[rftosq(rank+1, file)];
		WHITEPAWNMOVES[i] = attacks;
	}
}


void FillBlackPawnMoves()
{
	for(int i = 0; i < 64; i++)
	{
		BITMAP attacks = 0;
		int rank = RANKS[i];
		int file = FILES[i];
		if(rank -1 >= 1) attacks |= BITSET[rftosq(rank-1, file)];
		BLACKPAWNMOVES[i] = attacks;
	}
}

void FillWhitePawnAttacks()
{
	int filedelta[] = {-1,1};
	int rankdelta[] = {1, 1};
	for(int i =0; i < 64; i++)
	{
		BITMAP attacks = 0;
		int rank = RANKS[i];
		int file = FILES[i];
		for(int j = 0; j < 2; j++)
		{
			int r2 = rank + rankdelta[j];
			int f2 = file + filedelta[j];
			if(f2 >=1 && f2 <= 8 && r2 <= 8)
			{
				attacks |= BITSET[rftosq(r2,f2)];
			}
		}

		WHITEPAWNATTACKS[i] = attacks;
	}	
}


void FillBlackPawnAttacks()
{
	int filedelta[] = {-1,1};
	int rankdelta[] = {-1, -1};
	for(int i =0; i < 64; i++)
	{
		BITMAP attacks = 0;
		int rank = RANKS[i];
		int file = FILES[i];
		for(int j = 0; j < 2; j++)
		{
			int r2 = rank + rankdelta[j];
			int f2 = file + filedelta[j];
			if(f2 >=1 && f2 <= 8 && r2 <= 8)
			{
				attacks |= BITSET[rftosq(r2,f2)];
			}
			BLACKPAWNATTACKS[i] = attacks;
		}
	}	
}

void FillKingMoves()
{
	int filedelta[] = {1, 1,1,0, 0,-1,-1,-1};
	int rankdelta[] = {1,-1,0,1,-1, 0, 1, -1};
	for(int i = 0; i<64; i++)
	{
		BITMAP kmove = 0;
		int rank = RANKS[i];
		int file = FILES[i];
		for(int j = 0; j<8;j++)
		{
			int r2 = rank + rankdelta[j];
			int f2 = file + filedelta[j];
			if(r2 >=1 && r2<=8 && f2 >=1 && f2<=8)
			{
				kmove |= BITSET[rftosq(r2,f2)];
			}
		}
		KINGMOVES[i] = kmove;
	}
} 
void InitMoves()
{
	// Initialize all the different move arrays for pawns, bishops, rooks, queens, kings
	FillBitSets();
	FillRankMasks();
	FillFileMasks();
	FillBishopMasks();
	FillRookMasks();
	for(int i=0; i<64; i++)
	{
		BMASKSIZES[i] = CountBits(BISHOPMASK[i]);
		RMASKSIZES[i] = CountBits(ROOKMASK[i]);
	}
	FillWhitePawnMoves();
	FillBlackPawnMoves();
	FillWhitePawnAttacks();
	FillBlackPawnAttacks();
	FillKingMoves();
	FillRookBishopMoves();
	FillKnightMoves();

}


bool MoveExists(Board* board,  int move)
{
	unsigned int moves[100];
	if(move == 0) return false;
	int index = MoveGen(0,board, moves);
	for(int i =0; i < index; i++)
	{
		if(moves[i] == move)
		{
			MakeMove(moves[i], board);
			if(!IsKingAttacked(1^board->side, board))
			{
				UnmakeMove(moves[i], board);
				return true;
			}
			else{
				UnmakeMove(moves[i], board);
				return false;
			}
		}
	}
	return false;
}



int GetTime()
{
	struct timeval t;
	gettimeofday(&t,NULL);
	return t.tv_sec*1000 + t.tv_usec/1000; 
}


int AlphaBetaSearch(Board* board, int ply, int depth, int alpha, int beta)
{

 
   int i, j, movesfound, val;
   board->principalvariationlength[ply] = ply;
   if (depth == 0) return Evaluate(board);
   movesfound = 0;
   board->movebuflen[ply+1] = MoveGen(board->movebuflen[ply], board, board->movebuffer);
   for (i = board->movebuflen[ply]; i < board->movebuflen[ply+1]; i++)
   {
          MakeMove(board->movebuffer[i], board);
          {
                 if (!IsKingAttacked(1^(board->side), board))
                 {
                       board->inodes++;
                       if (movesfound)
                       {
                            val = -AlphaBetaSearch(board,ply+1, depth-1, -alpha-1, -alpha);
                      		if ((val > alpha) && (val < beta))
                            {
                                     val = -AlphaBetaSearch(board,ply+1, depth - 1, -beta, -alpha);   // In case of failure, proceed with normal alphabeta       
                            }
                       }

                       else val = -AlphaBetaSearch(board,ply+1, depth-1, -beta, -alpha);          // Normal alphabeta
                       UnmakeMove(board->movebuffer[i], board);
                       if (val >= beta)
                       {
                              return beta;
                       }
                       if (val > alpha)
                       {
                              alpha = val;                                                      // both sides want to maximize from *their* perspective
                              movesfound++;
                              board->principalvariationarray[ply][ply] = board->movebuffer[i];                                  // save this move
                              for (j = ply + 1; j < board->principalvariationlength[ply + 1]; j++)
                              {
                                    board->principalvariationarray[ply][j] = board->principalvariationarray[ply+1][j];   // and append the latest best PV from deeper plies
                              }
                              board->principalvariationlength[ply] = board->principalvariationlength[ply + 1];
                       }
                 }
                 else UnmakeMove(board->movebuffer[i], board);
          }
   }
   if(movesfound == 0)//IsKingAttacked(board->side, board))
   {
   	   	//cout << "checkmate found " << endl;
   		return mate - ply;
   }
   return alpha;
}







void Search(Board* board, int depth)
{
	 
	unsigned int bestmove = 0;
	int bestscore = -INFINITY;
	int currentdepth = 0;
	int pvmoves = 0;
	int pvnum = 0;
	for(int currentdepth = 1; currentdepth <= depth; currentdepth++)
	{
		bestscore = AlphaBetaSearch(board,0,currentdepth, -INFINITY, INFINITY);		
		//pvmoves = StorePvLine(currentdepth, board);
		bestmove = board->principalvariationarray[0][0];
		cout << "Depth " << currentdepth << " score " << bestscore << " move " << squaresarray[GetFrom(bestmove)] << squaresarray[GetTo(bestmove)] << " Principal Variation : ";
		for(int i=0; i < board->principalvariationlength[0]; i++)
		{
			cout << squaresarray[GetFrom(board->principalvariationarray[0][i])] << squaresarray[GetTo(board->principalvariationarray[0][i])] << " ";		
		}
		cout << endl;
	}	
}


int Quiescense(int alpha, int beta, Board* board, SearchConstraints* con)
{

}


