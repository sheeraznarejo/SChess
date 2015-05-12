#ifndef DEFS_H
#define DEFS_H
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
typedef unsigned long long BITMAP;
using namespace std;

#define NAME "SCHESS 1.0"
#define MAXMOVEBUFFER 4096
#define MAXPLY 64
#define MAXGAMELINE 1024
#define INFINITY 0x01000000
#define PVTABLESIZE  0x100000 * 2 // 2 MB PV TABLE;
#define R64 (BITMAP(rand())| (BITMAP(rand()) <<  32))
#define displaymove(a)  cout << squaresarray[GetFrom(a)] << squaresarray[GetTo(a)] << endl;

enum BoardSquares
{
	A1 = 0, B1, C1, D1, E1, F1, G1, H1,
	A2, 	B2, C2, D2, E2, F2, G2, H2,
	A3, 	B3, C3, D3, E3, F3, G3, H3,
	A4,		B4, C4, D4, E4, F4, G4, H4,
	A5, 	B5, C5, D5, E5, F5, G5, H5,
	A6, 	B6, C6, D6, E6, F6, G6, H6,
	A7, 	B7, C7, D7, E7, F7, G7, H7,
	A8, 	B8, C8, D8, E8, F8, G8, H8
};


extern string squaresarray[64];
typedef struct 
{
	int move;
	unsigned char whitecastlingpermissions;
	unsigned char blackcastlingpermissions;
	unsigned char enpassantsquare;
	int fiftymove;
	BITMAP hashkey;
	int material;
} Record;


typedef struct 
{
	int starttime;
	int endtime;
	int depth;
	int depthset;
	int timeset;
	int quit;
	int stopped;
	int infinite;
	long nodes;
	float fh;
	float fhf;
} SearchConstraints;

enum PieceTypes {EMPTY, WHITE_PAWN, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, BLACK_PAWN, BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK, BLACK_QUEEN, BLACK_KING};
enum PieceValues { PAWN_VALUE = 100, BISHOP_VALUE = 325, KNIGHT_VALUE = 325, ROOK_VALUE = 550, QUEEN_VALUE = 1000, KING_VALUE = 50000};
enum FileTypes {F0, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum RankTypes {R0, RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
enum SIDE {WHITE, BLACK};


//Precomputed integers to represent queen and kingside castling. KINGSIDECASTLE = 1, QUEENSIDECASTLE = 2;
extern unsigned char KINGSIDECASTLE;
extern unsigned char QUEENSIDECASTLE;

extern int mate;
extern int bestwhite;
extern int bestblack;
extern ofstream myfile;
//BITSET[i] is a 64 bit number in which only the ith bit is set
extern BITMAP BITSET[64];

// Mapping of squares (0 to 63) to Ranks(1 to 8) and files(1 to 8)
extern int RANKS[64];
extern int FILES[64];

// BITMASKS for Ranks, Files, Rooks, Bishops
extern BITMAP RANKMASKS[8];
extern BITMAP FILEMASKS[8];
extern BITMAP ROOKMASK[64];
extern BITMAP BISHOPMASK[64];

//Sizes for the Rook/Bishop Mask for square i, where the size is the number of bits set in the mask
extern int BMASKSIZES[64];
extern int RMASKSIZES[64];

// Sizes for necessary shifts when calculating magic numbers. ROOKMAGICSHIFTS[i] = 64 - RMASKSIZES[i]
extern int ROOKMAGICSHIFTS[64];
extern int BISHOPMAGICSHIFTS[64];

//Array for all the Bishop Moves from square i
extern BITMAP ROOKMOVES[64][4096];

// array for all the Rook moves from square i
extern BITMAP BISHOPMOVES[64][4096];

// array for all knight moves from square i
extern BITMAP KNIGHTMOVES[64];

// array for all king moves from square i 
extern BITMAP KINGMOVES[64];

//The Rook and Bishop Magics. These are not computed at runtime but are rather precomputed. Check magics.cpp
// for details on how the magics are generated.
extern BITMAP RMAGICS[64];
extern BITMAP BMAGICS[64];

// Bitmaps for white and black pawn non capture moves.
extern BITMAP WHITEPAWNMOVES[64];
extern BITMAP BLACKPAWNMOVES[64];
extern BITMAP BLACKPAWNATTACKS[64];
extern BITMAP WHITEPAWNATTACKS[64];

int PopBit(BITMAP* b);
int CountBits(BITMAP b);
int rftosq(int,int);
BITMAP genrandom();
void FillRankMasks();
void FillFileMasks();
void FillRookMasks();
void FillBishopMasks();
void FillBitSet();
void FillBAttackSets(vector<BITMAP> occs[], vector<BITMAP> att[]);
void FillRAttackSets(vector<BITMAP> occs[], vector<BITMAP> att[]);
void FillRookMagics();
void FillBishopMagics();
void FillBROccs(vector<BITMAP> att[], bool rb);
void FillRookMoves();
void FillBishopMoves();
void FillKnightMoves();
void DisplayBitmap(BITMAP b, ofstream& f);
void DebugMove(unsigned int);
void DebugMove(unsigned int, ofstream& f);
void DisplayBitmap(BITMAP b);
void DisplayBitmap(BITMAP a, BITMAP b, ofstream& f);
void PrintMagics(BITMAP magics[], ofstream& f);
void InitMoves();
void InitBoard();
bool GenMagics(int k, BITMAP magicnum, BITMAP magics[], int shifts[], vector<BITMAP> occupancies[], vector<BITMAP> attackset[], vector<BITMAP> moves[]);
void SetFrom(unsigned int a, unsigned int* move);
unsigned GetFrom(unsigned int);
void SetTo(unsigned int a, unsigned int* move);
unsigned GetTo(unsigned int move);
void SetPiece(unsigned int, unsigned int*);
unsigned GetPiece(unsigned int);
void SetCapt(unsigned int, unsigned int*);
unsigned GetCapt(unsigned int);
void SetEnpassant(unsigned int*);
bool IsEnpassant(unsigned int);
unsigned GetCastle(unsigned int);

void SetCastle(unsigned int, unsigned int*);
void SetProm(unsigned int, unsigned int*);
unsigned GetProm(unsigned int);

#endif