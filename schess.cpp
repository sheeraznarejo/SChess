#include "defs.h"
#include "board.h"
#include <iostream>
#include <string>
#include <fstream>
#include <string.h>
using namespace std;

int MoveGen(int, Board* board);
void MakeMove(int, Board* board);
void UnmakeMove(int, Board* board);
void ParseMove(std::string move, Board *board);
void InitMoves();
unsigned long long Perft(int depth, int ply, Board* board, int startdepth);
bool IsSquareAttacked(int side, int square, Board* board);
void InitSearch(Board* board, SearchConstraints* con);
void Search(Board* board, int depth);
int StorePvLine(int, Board* board);
int AlphaBeta(int alpha, int beta, int ply, int depth, Board* board, SearchConstraints* con, bool nullmove);
int RetrievePvMove(Board* board);
int Evaluate(Board* board);
bool MoveExists(Board* board, int move);

int main()
{
	InitMoves();
	Board board;
	SearchConstraints con;
	con.depth = 6;
	con.nodes = 0;
	string matein5 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BqP1P3/5N2/Pp1P2PP/R2Q1R1K w kq - 0 1";
	string matein3 = "1nkr1q1r/pb1p4/1p1Qp1p1/4Pp1p/5P2/2N3P1/PPP4P/2KR1BR1 w - 0 1";
	string startfen = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
	string c4c5 = "r3k2r/Pppp1ppp/1b3nbN/nPP5/BB2P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1";
	string startboard = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	string rookpromotion = "r3k2r/Pppp1ppp/1b3nbN/nPP5/BB2P3/q4N2/P2P2PP/r2Q1RK1 w kq - 0 1";
	string newfen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	board.InitFromFen(matein3);
	Search(&board, 7);
	/*unsigned int move1 = 0;
	SetPiece(WHITE_KING, &move1);
	SetFrom(G1, &move1);
	SetTo(H1, &move1);
	MakeMove(move1, &board);
	unsigned int move2 = 0;
	SetPiece(BLACK_PAWN, &move2);
	SetFrom(B2, &move2);
	SetTo(A1, &move2);
	SetProm(BLACK_QUEEN, &move2);
	SetCapt(WHITE_ROOK,&move2);
	MakeMove(move2, &board);
	cout << "evaluation is " << Evaluate(&board) << endl;
	cout << "hashkey after h1g1 b2a1 : ";
	cout << board.hashkey << endl;
	//displaymove(RetrievePvMove(&board));
	cout << MoveExists(&board,741379);*/

	
	
	
}




