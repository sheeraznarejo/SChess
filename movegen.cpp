#include "defs.h"
#include "board.h"
#include <iostream>
#include <fstream>
using namespace std;
bool IsSquareAttacked(int side, int square, Board* board);

void SetFrom(unsigned int a, unsigned int* move)
{ // need 6 bits
	*move &= 0xffffffc0;
	*move |= (a & 0x0000003f);
}

unsigned int GetFrom(unsigned int move)
{
	return (int)(move & 0x0000003f);
}

unsigned int GetTo(unsigned int move)
{
	return (int)((move >> 6) & 0x3f);
}
void SetTo(unsigned int a, unsigned int* move)
{  // need 6 bits
	*move &= 0xfffff03f;
	*move |= (a & 0x0000003f) << 6;
}

void SetPiece(unsigned int piece, unsigned int* move)
{ // need 4 bits
	*move &= 0xffff0fff;
	*move |= (piece & 0x0000000f) << 12;
}

unsigned int GetPiece(unsigned  int move)
{
	return (int)((move >> 12) & 0xf);
}
void SetCapt(unsigned int piece, unsigned int* move)
{ // need 4 bits
	*move &= 0xfff0ffff;
	*move |= (piece & 0x0000000f) << 16;
}

unsigned int GetCapt( unsigned int move)
{
	return (int)((move >> 16) & 0xf);
}

void SetProm(unsigned int piece, unsigned int* move)
{ // need 4 bits
	*move &= 0xff0fffff;
	*move |= (piece & 0x0000000f) << 20;
}

unsigned int GetProm(unsigned int move)
{
	return (int)((move >> 20) & 0x0000000f);
}

void SetCastle(unsigned int type, unsigned int* move)
{ 	//KINGSIDECASTLE == 1, QUEENSIDECASTLE == 2;
	
	if (type == KINGSIDECASTLE) // BQSCA
	{
		*move |= (1 << 24);
	}
	if (type == QUEENSIDECASTLE) // BKSCA 
	{
		*move |= (1 << 25);
	}
}

void SetEnpassant(unsigned int* move) // sets the 29th bit to 1 if the move is an enpassant move
{
	*move|= (1 << 28);
}


bool IsEnpassant( unsigned int move)
{
	return (move >> 28) & 1;
}

unsigned int GetCastle( unsigned int move)
{
	return ((move >> 24) & 0x00000003);
}

void DebugMove(unsigned int move)
{
	std::cout << dec << "Piece " << GetPiece(move) << endl;
	std::cout << "From " << GetFrom(move) << endl;
	std::cout << "To " << GetTo(move) << endl;
	std::cout << "Captured " << GetCapt(move) << endl;
	std::cout << "Castling " << GetCastle(move) << endl;
	std::cout << "Is Enpassant " << IsEnpassant(move) << endl;
	std::cout << "Promotion Piece " << GetProm(move) << endl;
	std::cout << endl;
}



void DebugMove(unsigned int move, ofstream& f)
{
	f << dec << "Piece " << GetPiece(move) << endl;
	f << "From " << GetFrom(move) << endl;
	f << "To " << GetTo(move) << endl;
	f << "Captured " << GetCapt(move) << endl;
	f << "Castling " << GetCastle(move) << endl;
	f << "Is Enpassant " << IsEnpassant(move) << endl;
	f << endl;
}

unsigned int MoveGen(int index, Board* board, unsigned int* movesarray)
{
	int starter = index;
	if((*board).side == WHITE)
	{
		
		// White Pawns : 
		// First generate white pawn single moves and white pawn non capture promotion moves
		BITMAP currentpawns =  (*board).whitepawns;
		while(currentpawns != BITMAP(0)) 
		{
			unsigned int FROM = PopBit(&currentpawns);
			BITMAP tobitmap = WHITEPAWNMOVES[FROM] & (~board->allpieces);
			while(tobitmap != 0)
			{

				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(WHITE_PAWN, &move);
				SetCapt(0, &move);				
				if(RANKS[TO] == 8)
				{
		
					SetProm(WHITE_KNIGHT, &move);
					movesarray[index++] = move;
					SetProm(WHITE_BISHOP, &move);;
					movesarray[index++] = move;
					SetProm(WHITE_ROOK, &move);
					movesarray[index++] = move;
					SetProm(WHITE_QUEEN, &move);	
					movesarray[index++] = move;
				}
				else
				{
					movesarray[index++] = move;
				}
			}
		}

		currentpawns = (*board).whitepawns;
		// Generate white pawn double moves 
		while(currentpawns != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentpawns);
			if(RANKS[FROM] == 2)
			{
				if(((*board).squares[FROM+8] == EMPTY) && ((*board).squares[FROM+16] == EMPTY))
				{
					unsigned int TO = FROM + 16;
					unsigned int move = 0;
					SetPiece(WHITE_PAWN, &move);
					SetFrom(FROM, &move);
					SetTo(TO, &move);
					SetCapt(0, &move);
					movesarray[index++] = move;
				}
			}
		}

		// generate pawn captures
		currentpawns  = board->whitepawns;
		while(currentpawns != BITMAP(0)) 
		{
			unsigned int FROM = PopBit(&currentpawns);
			{
				BITMAP tobitmap = (WHITEPAWNATTACKS[FROM] & board->blackpieces);
				if(board->enpassantsquare != -1)
				{

					if(RANKS[board->enpassantsquare] == 6)
					{
						if(WHITEPAWNATTACKS[FROM] & BITSET[board->enpassantsquare])
						{
							unsigned int move = 0;
							SetPiece(WHITE_PAWN, &move);
							SetFrom(FROM, &move);
							SetTo(board->enpassantsquare, &move);
							SetCapt(BLACK_PAWN, &move);
							SetEnpassant(&move);
							movesarray[index++] = move;
						}
					}
				}


				// generate normal pawn captures
				while(tobitmap != BITMAP(0))
				{
					unsigned int TO = PopBit(&tobitmap);
					unsigned int move = 0;
					SetFrom(FROM, &move);
					SetTo(TO, &move);
					SetPiece(WHITE_PAWN, &move);
					SetProm(0, &move);
					SetCapt(board->squares[TO], &move);
					if(RANKS[TO] == 8)
					{
						// generate promotion pawn captures
						SetProm(WHITE_KNIGHT, &move);
						movesarray[index++] = move;
						SetProm(WHITE_BISHOP, &move);;
						movesarray[index++] = move;
						SetProm(WHITE_ROOK, &move);
						movesarray[index++] = move;
						SetProm(WHITE_QUEEN, &move);	
						movesarray[index++] = move;

						
					}
					else					
						movesarray[index++] = move;
					}
			}
		}


		BITMAP currentknights = board->whiteknights;
		while(currentknights != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentknights);
			{
				BITMAP tobitmap = KNIGHTMOVES[FROM] & (~board->whitepieces);
				while(tobitmap != BITMAP(0))
				{
					unsigned int TO = PopBit(&tobitmap);
					unsigned int move = 0;
					SetFrom(FROM, &move);
					SetTo(TO, &move);
					SetPiece(WHITE_KNIGHT, &move);
					SetProm(0, &move);
					SetCapt(board->squares[TO], &move);
					movesarray[index++]  = move;
				}
			}
		}
		// white bishop moves : 
		BITMAP currentbishops = board->whitebishops;
		while(currentbishops != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentbishops);
			BITMAP BMOVES =  (BISHOPMOVES[FROM][((board->allpieces & BISHOPMASK[FROM]) * BMAGICS[FROM]) >> (64 - BMASKSIZES[FROM])]);
			BITMAP tobitmap = BMOVES & (~(board->whitepieces));
			while(tobitmap != BITMAP(0))
			{
				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(WHITE_BISHOP, &move);
				SetProm(0, &move);
				SetCapt(board->squares[TO], &move);
				movesarray[index++]  = move;
			}
		}
		// white rook normal moves : 
		BITMAP currentrooks = board->whiterooks;
		while(currentrooks != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentrooks);
			BITMAP RMOVES = (ROOKMOVES[FROM][((board->allpieces & ROOKMASK[FROM]) * RMAGICS[FROM]) >> (64 - RMASKSIZES[FROM])]);
			BITMAP tobitmap = RMOVES & (~(board->whitepieces));
			while(tobitmap != BITMAP(0))
			{
				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(WHITE_ROOK, &move);
				SetProm(0, &move);
				SetCapt(board->squares[TO], &move);
				movesarray[index++]  = move;
			}		
		}
		// generate castling moves  : 
		// castling moves are only generated if castling is still permissible (rook and king have not moved)
		if((board->whitecastlingpermissions & KINGSIDECASTLE) && (board->squares[F1] == EMPTY) && (board->squares[G1] == EMPTY) && (!IsSquareAttacked(WHITE,E1, board)) && (!IsSquareAttacked(WHITE,F1, board)) && (!IsSquareAttacked(WHITE,G1, board))) // white can castle on kingside
		{
			unsigned int move = 0;
			SetCastle(KINGSIDECASTLE, &move);
			SetPiece(WHITE_ROOK, &move);
			movesarray[index++] = move;
		}
		if((board->whitecastlingpermissions  & QUEENSIDECASTLE) && (board->squares[B1] == EMPTY) && (board->squares[C1] == EMPTY) && (board->squares[D1] == EMPTY) &&  (!IsSquareAttacked(WHITE, C1, board)) && (!IsSquareAttacked(WHITE,D1, board)) && (!IsSquareAttacked(WHITE,E1, board)))
		{
			unsigned int move = 0;
			SetCastle(QUEENSIDECASTLE, &move);
			SetPiece(WHITE_ROOK, &move);
			movesarray[index++] = move;
		}

		// white king moves 
		BITMAP currentking = board->whiteking;
		while(currentking != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentking);
			BITMAP tobitmap = KINGMOVES[FROM] & (~board->whitepieces);
			while(tobitmap != BITMAP(0))
			{
				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(WHITE_KING, &move);
				SetProm(0, &move);
				SetCastle(0, &move);
				SetCapt(board->squares[TO], &move);
				movesarray[index++]  = move;
			}
		}
		// white queen moves
		BITMAP currentqueen = board->whitequeen;
		while(currentqueen != BITMAP(0))
		{
			unsigned int move = 0;
			unsigned int FROM = PopBit(&currentqueen);
			BITMAP RMOVES = (ROOKMOVES[FROM][((board->allpieces & ROOKMASK[FROM]) * RMAGICS[FROM]) >> (64 - RMASKSIZES[FROM])]);
			BITMAP BMOVES =  (BISHOPMOVES[FROM][((board->allpieces & BISHOPMASK[FROM]) * BMAGICS[FROM]) >> (64 - BMASKSIZES[FROM])]);
			BITMAP bishops = BMOVES & (~(board -> whitepieces));
			BITMAP rooks = 	 RMOVES  & (~(board -> whitepieces));
			BITMAP tobitmap = bishops | rooks;
			while(tobitmap != BITMAP(0))
			{
				unsigned int TO = PopBit(&tobitmap);
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(WHITE_QUEEN, &move);
				SetProm(0, &move);
				SetCastle(0, &move);
				SetCapt(board->squares[TO], &move);
				movesarray[index++] = move;
			}
		}

 	}


	else // generate black moves
	{
		//generate pawn moves : 
		// Black Pawns : 
		BITMAP currentpawns =  board->blackpawns;
		// generate normal pawn moves and white pawn promotion moves
		while(currentpawns != BITMAP(0)) 
		{
			unsigned int FROM = PopBit(&currentpawns);
			BITMAP tobitmap = BLACKPAWNMOVES[FROM] & (~board->allpieces);
			while(tobitmap != 0)
			{
				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(BLACK_PAWN, &move);
				SetCastle(0, &move);
				SetProm(0, &move);
				SetCapt(0, &move);
				if(RANKS[TO] == 1)
				{
				
					SetProm(BLACK_KNIGHT, &move);
					movesarray[index++] = move;
					SetProm(BLACK_BISHOP, &move);
					movesarray[index++] = move;
					SetProm(BLACK_ROOK, &move);
					movesarray[index++] = move;
					SetProm(BLACK_QUEEN, &move);	
					movesarray[index++] = move;
				}
				else
				{
					movesarray[index++] = move;
				}
			}
		}
		currentpawns = board->blackpawns;
		// Generate black pawn double moves 
		while(currentpawns != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentpawns);
			if(RANKS[FROM] == 7)
			{
				if(board->squares[FROM-8] == EMPTY && board->squares[FROM-16] == EMPTY)
				{
					unsigned int TO = FROM - 16;
					unsigned int move = 0;
					SetPiece(BLACK_PAWN, &move);
					SetFrom(FROM, &move);
					SetTo(TO, &move);
					SetCapt(0, &move);
					movesarray[index++] = move;

				}
			}
		}

		// generate pawn captures
		currentpawns  = board->blackpawns;
		while(currentpawns != BITMAP(0)) 
		{
			unsigned int FROM = PopBit(&currentpawns);
			{
				BITMAP tobitmap = BLACKPAWNATTACKS[FROM] & board->whitepieces;
				// generate enpassant captures
				if(board->enpassantsquare != -1)
				{
					unsigned int move = 0;
					if(RANKS[board->enpassantsquare] == 3)
					{
						if(BLACKPAWNATTACKS[FROM] & BITSET[board->enpassantsquare])
						{
							SetFrom(FROM, &move);
							SetTo(board->enpassantsquare, &move);
							SetPiece(BLACK_PAWN, &move);
							SetCapt(WHITE_PAWN, &move);
							SetEnpassant(&move);
							movesarray[index++] = move;
						}
					}
				}
				while(tobitmap != BITMAP(0))
				{
					unsigned int TO = PopBit(&tobitmap);
					unsigned int move = 0;
					SetFrom(FROM, &move);
					SetTo(TO, &move);
					SetPiece(BLACK_PAWN, &move);
					SetProm(0, &move);
					SetCastle(0, &move);
					SetCapt(board->squares[TO], &move);
					if(RANKS[TO] == 1)
					{
						
						SetProm(BLACK_KNIGHT, &move);
						movesarray[index++] = move;
						SetProm(BLACK_BISHOP, &move);
						movesarray[index++] = move;						
						SetProm(BLACK_ROOK, &move);
						movesarray[index++] = move;
						SetProm(BLACK_QUEEN, &move);	
						movesarray[index++] = move;
				}
					else
					{					
						movesarray[index++] = move;
					}
					
				}
			}
		}
		// black knight moves : 
		BITMAP currentknights = board->blackknights;
		while(currentknights != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentknights);
			{
				BITMAP tobitmap = (KNIGHTMOVES[FROM] & (~board->blackpieces));
				while(tobitmap != BITMAP(0))
				{
					unsigned int TO = PopBit(&tobitmap);
					unsigned int move = 0;
					SetFrom(FROM, &move);
					SetTo(TO, &move);
					SetPiece(BLACK_KNIGHT, &move);
					SetProm(0, &move);
					SetCastle(0, &move);
					SetCapt(board->squares[TO], &move);
					movesarray[index++]  = move;
				}
			}
		}
		BITMAP currentbishops = board->blackbishops;
		while(currentbishops != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentbishops);
			BITMAP BMOVES =  (BISHOPMOVES[FROM][((board->allpieces & BISHOPMASK[FROM]) * BMAGICS[FROM]) >> (64 - BMASKSIZES[FROM])]);
			BITMAP tobitmap = BMOVES & (~(board->blackpieces));
			while(tobitmap != BITMAP(0))
			{
				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(BLACK_BISHOP, &move);
				SetProm(0, &move);
				SetCastle(0, &move);
				SetCapt(board->squares[TO], &move);
				movesarray[index++]  = move;
			}
		}

		// black rook moves : 
		BITMAP currentrooks = board->blackrooks;
		while(currentrooks != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentrooks);
			BITMAP RMOVES = (ROOKMOVES[FROM][((board->allpieces & ROOKMASK[FROM]) * RMAGICS[FROM]) >> (64 - RMASKSIZES[FROM])]);
			BITMAP tobitmap = RMOVES & (~(board->blackpieces));
			while(tobitmap != BITMAP(0))
			{
				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(BLACK_ROOK, &move);
				SetProm(0, &move);
				SetCastle(0, &move);
				SetCapt(board->squares[TO], &move);
				movesarray[index++]  = move;
			}		
		}
		if((board->blackcastlingpermissions & KINGSIDECASTLE) && (board->squares[F8] == EMPTY) && (board->squares[G8] == EMPTY) && (board->blackcastlingpermissions & KINGSIDECASTLE) && (!IsSquareAttacked(BLACK, E8, board)) && (!IsSquareAttacked(BLACK,F8, board)) && (!IsSquareAttacked(BLACK,G8, board))) //black can castle on kingside
		{
			unsigned int move = 0;
			SetCastle(KINGSIDECASTLE, &move);
			SetPiece(BLACK_ROOK, &move);
			movesarray[index++] = move;
		}
		if((board->blackcastlingpermissions & QUEENSIDECASTLE) && (board->squares[B8] == EMPTY) && (board->squares[C8] == EMPTY) && (board->squares[D8] == EMPTY) && (!IsSquareAttacked(BLACK,C8, board)) && (!IsSquareAttacked(BLACK,D8, board)) && (!IsSquareAttacked(BLACK,E8, board)))
		{
			unsigned int move = 0;
			SetCastle(QUEENSIDECASTLE, &move);
			SetPiece(BLACK_ROOK, &move);
			movesarray[index++] = move;
		}

		// black king moves 
		BITMAP currentking = board->blackking;
		while(currentking != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentking);
			BITMAP tobitmap = (KINGMOVES[FROM] & (~board->blackpieces));
			while(tobitmap != BITMAP(0))
			{
				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(BLACK_KING, &move);
				SetProm(0, &move);
				SetCastle(0, &move);
				SetCapt(board->squares[TO], &move);
				movesarray[index++]  = move;

			}
		}
		// black queen moves
		BITMAP currentqueen = board->blackqueen;
		while(currentqueen != BITMAP(0))
		{
			unsigned int FROM = PopBit(&currentqueen);
			BITMAP RMOVES = (ROOKMOVES[FROM][((board->allpieces & ROOKMASK[FROM]) * RMAGICS[FROM]) >> (64 - RMASKSIZES[FROM])]);
			BITMAP BMOVES =  (BISHOPMOVES[FROM][((board->allpieces & BISHOPMASK[FROM]) * BMAGICS[FROM]) >> (64 - BMASKSIZES[FROM])]);
			BITMAP tobitmap = (RMOVES | BMOVES) & (~(board ->blackpieces));
			while(tobitmap != BITMAP(0))
			{
				unsigned int TO = PopBit(&tobitmap);
				unsigned int move = 0;
				SetFrom(FROM, &move);
				SetTo(TO, &move);
				SetPiece(BLACK_QUEEN, &move);
				SetProm(0, &move);
				SetCastle(0, &move);
				SetCapt(board->squares[TO], &move);
				movesarray[index++] = move;
			}
		}
	}
	return index;


}	



bool IsSquareAttacked(int side, int square, Board* board)
{
	if(side == WHITE)
	{

		BITMAP RMOVES = (ROOKMOVES[square][((board->allpieces & ROOKMASK[square]) * RMAGICS[square]) >> (64 - RMASKSIZES[square])]);
		BITMAP BMOVES =  (BISHOPMOVES[square][((board->allpieces & BISHOPMASK[square]) * BMAGICS[square]) >> (64 - BMASKSIZES[square])]);
		BITMAP rook  = RMOVES  & (~board->whitepieces);
		BITMAP bishops = BMOVES & (~board->whitepieces);
		BITMAP knights = KNIGHTMOVES[square];
		BITMAP king = KINGMOVES[square];
		// check if attacked by black rooks or queen-rook : 
		if((rook & (board->blackrooks | board->blackqueen)) != BITMAP(0))
		{
			return true;
		}
		// check if attacked by black bishops or queen bishop;
		else if((bishops & (board->blackbishops|board->blackqueen)) != BITMAP(0))
		{
			return true;
		}
		//check if square attacked by black knights
		else if((knights & board->blackknights) != BITMAP(0))
		{
			return true;
		}
		//check if attacked by black pawns
		else if((RANKS[square] + 1 <= 8) && (FILES[square]+1 <= 8) && (board->squares[rftosq(RANKS[square]+1, FILES[square]+1)] == BLACK_PAWN))
		{
			return true;
		}
		else if((RANKS[square]+ 1 <=8) && (FILES[square]-1 >= 1) && (board->squares[rftosq(RANKS[square]+1, FILES[square]-1)] == BLACK_PAWN))
			return true;
		else if((king & board->blackking) != BITMAP(0))
			return true;
		return false;

	}
	else // (*board).side == BLACK
	{


		BITMAP RMOVES = (ROOKMOVES[square][((board->allpieces & ROOKMASK[square]) * RMAGICS[square]) >> (64 - RMASKSIZES[square])]);
		BITMAP BMOVES = (BISHOPMOVES[square][((board->allpieces & BISHOPMASK[square]) * BMAGICS[square]) >> (64 - BMASKSIZES[square])]);
		BITMAP rook  = RMOVES & (~(board -> blackpieces));
		BITMAP bishops = BMOVES & (~(board -> blackpieces));
		BITMAP knights = KNIGHTMOVES[square];
		BITMAP king = KINGMOVES[square];
		// check if attacked by white rooks or queen-rook : 
		if ((rook & (board->whiterooks | board->whitequeen)) != BITMAP(0))
			return true;
		// check if attacked by white bishops or queen bishop;
		else if ((bishops & (board->whitebishops|board->whitequeen)) != BITMAP(0))
			return true;
		//check if square attacked by white knights
		else if ((knights & board->whiteknights) != BITMAP(0))
			return true;
		//check if attacked by white pawns
		else if ((RANKS[square] - 1 >= 1) && (FILES[square]+1 <= 8) && (board->squares[rftosq(RANKS[square]-1, FILES[square]+1)] == WHITE_PAWN))
			return true;
		else if ((RANKS[square]- 1 >= 1) && (FILES[square]-1 >= 1) && (board->squares[rftosq(RANKS[square]-1, FILES[square]-1)] == WHITE_PAWN))
			return true;
		else if ((king & board->whiteking) != BITMAP(0))
			return true;
		return false;
	}
}


bool IsKingAttacked(int side, Board* board)
{

	if(side == WHITE)
	{
		BITMAP wk = board->whiteking;
		int kingsq = PopBit(&wk);
		if(IsSquareAttacked(WHITE, kingsq,board))
			return true;
		
		else
			return false;
	}
	if(side == BLACK)
	{
		BITMAP bk = board->blackking;
		int kingsq = PopBit(&bk);
		if(IsSquareAttacked(BLACK, kingsq, board))
			return true;
		else
			return false;
	}
}