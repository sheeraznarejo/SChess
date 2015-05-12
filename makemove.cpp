#include "defs.h"
#include "board.h"
#include <iostream>
void MovePiece(int,int,int, Board* board);
void DoCapture(int capt, int square, Board* board);
void UndoCapture(int capt, int square, Board* board);
void ParseMove(std::string move);
void StorePvMove(Board*board, unsigned int move);
void Search(Board* board, SearchConstraints* con);
void MakeMove(int move, Board* board)
{
	int piece = GetPiece(move);
	int from = GetFrom(move);
	int to = GetTo(move);
	BITMAP wk1 = board->whiteking;
	BITMAP bk1  = board->blackking;
	if(to == PopBit(&wk1) || to == PopBit(&bk1)) return;
	int capt = GetCapt(move);
	int prom = GetProm(move);
	int castling = GetCastle(move);

	//store the move, the castling permissions, the enpassant square, the fifty move number, the board hashkey
	//before making the move. 
	(board->gameline[board->gameindex]).move = move;
	(board->gameline[board->gameindex]).whitecastlingpermissions = board->whitecastlingpermissions;
	(board->gameline[board->gameindex]).blackcastlingpermissions = board->blackcastlingpermissions;
	(board->gameline[board->gameindex]).enpassantsquare = board->enpassantsquare;
	(board->gameline[board->gameindex]).fiftymove = board->fiftymove;
	(board->gameline[board->gameindex]).hashkey = board->hashkey;
	(board->gameline[board->gameindex]).material = board->material;


	// hash out the enpassant square key before continuing
	switch (piece)
	{
		case WHITE_PAWN : // Move is a white pawn
			board->fiftymove = 0; // fifty move is set to 0 for either pawn move or a capture move
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			if(RANKS[from] == 2 && RANKS[to] == 4) 
			{
				if(board->enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
				board->enpassantsquare = to-8;
				board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			}
			MovePiece(WHITE_PAWN, to, from, board);
			if(capt != 0)
			{
				switch (capt)
				{
					case BLACK_PAWN :
						if(IsEnpassant(move))
						{
							DoCapture(BLACK_PAWN, to-8, board);
							board->squares[to-8] = EMPTY;
						}
						else
							DoCapture(BLACK_PAWN,to,board);
						break;
					case BLACK_BISHOP : 
						DoCapture(BLACK_BISHOP, to, board);
						break;
					case BLACK_KNIGHT :
						DoCapture(BLACK_KNIGHT, to, board);
						break;
					case BLACK_ROOK : 
						DoCapture(BLACK_ROOK,to, board);
						break;
					case BLACK_QUEEN :
						DoCapture(BLACK_QUEEN, to, board);
						break;
					case BLACK_KING :  // what do we really need to do here ? 
						DoCapture(BLACK_KING, to,  board);
						break;
					default :
						break;
				}
				// if the capture is a pawn promotional capture addiditional changes have to be made
				// if it is just a normal pawn capture nothing additional is done after calling movepiece and docapture
				if(prom != 0) // pawn promotion capture
				{
					switch(prom)
					{
						case WHITE_BISHOP : 
							board->squares[to] = WHITE_BISHOP;
							board->whitebishops ^= BITSET[to];
							board->whitepawns ^= BITSET[to];
							board->hashkey ^= board->piecekeys[WHITE_PAWN][to];
							board->hashkey ^= board->piecekeys[WHITE_BISHOP][to];
							board->material += BISHOP_VALUE;
							board->material -= PAWN_VALUE;
							break;
						case WHITE_ROOK : 
							board->squares[to] = WHITE_ROOK;
							board->whiterooks ^= BITSET[to];
							board->whitepawns ^= BITSET[to];
							board->hashkey ^= board->piecekeys[WHITE_ROOK][to];
							board->hashkey ^= board->piecekeys[WHITE_PAWN][to];
							board->material += ROOK_VALUE;
							board->material -= PAWN_VALUE;
							break;
						case WHITE_KNIGHT : 
							board->squares[to] = WHITE_KNIGHT;
							board->whiteknights ^= BITSET[to];
							board->whitepawns ^= BITSET[to];
							board->hashkey ^= board->piecekeys[WHITE_KNIGHT][to];
							board->hashkey ^= board->piecekeys[WHITE_PAWN][to];
							board->material += KNIGHT_VALUE;
							board->material -= PAWN_VALUE;
							break;
						case WHITE_QUEEN : 
							board->squares[to] = WHITE_QUEEN;
							board->whitequeen ^= BITSET[to];
							board->whitepawns ^= BITSET[to];
							board->hashkey ^= board->piecekeys[WHITE_QUEEN][to];
							board->hashkey ^= board->piecekeys[WHITE_PAWN][to];
							board->material += QUEEN_VALUE;
							board->material -= PAWN_VALUE;
							break;
						default : 
							break;
					}								
				}			
			}	
			else if (prom != 0) // normal pawn promotion moves without capture
			{
				switch(prom)
				{
					case WHITE_BISHOP : 
						board->squares[to] = WHITE_BISHOP;
						board->whitebishops ^= BITSET[to];
						board->whitepawns ^= BITSET[to];
						board->hashkey ^= board->piecekeys[WHITE_PAWN][to];
						board->hashkey ^= board->piecekeys[WHITE_BISHOP][to];
						board->material += BISHOP_VALUE;
						board->material -= PAWN_VALUE;
						break;
					case WHITE_ROOK : 
						board->squares[to] = WHITE_ROOK;
						board->whiterooks ^= BITSET[to];
						board->whitepawns ^= BITSET[to];
						board->hashkey ^= board->piecekeys[WHITE_ROOK][to];
						board->hashkey ^= board->piecekeys[WHITE_PAWN][to];
						board->material += ROOK_VALUE;
						board->material -= PAWN_VALUE;
						break;
					case WHITE_KNIGHT : 
						board->squares[to] = WHITE_KNIGHT;
						board->whiteknights ^= BITSET[to];
						board->whitepawns ^= BITSET[to];
						board->hashkey ^= board->piecekeys[WHITE_KNIGHT][to];
						board->hashkey ^= board->piecekeys[WHITE_PAWN][to];
						board->material += KNIGHT_VALUE;
						board->material -= PAWN_VALUE;
						break;
					case WHITE_QUEEN : 
						board->squares[to] = WHITE_QUEEN;
						board->whitequeen ^= BITSET[to];
						board->whitepawns ^= BITSET[to];
						board->hashkey ^= board->piecekeys[WHITE_QUEEN][to];
						board->hashkey ^= board->piecekeys[WHITE_PAWN][to];
						board->material += QUEEN_VALUE;
						board->material -= PAWN_VALUE;
						break;
					default : 
						break;
				}
			}
			break;
		case WHITE_BISHOP : // Move is a white bishop : 
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			MovePiece(WHITE_BISHOP, to, from, board);
			if(capt != 0)
			{
				board->fiftymove = 0;
				switch (capt)
				{
					case BLACK_PAWN : 
						DoCapture(BLACK_PAWN, to, board);
						break;
					case BLACK_ROOK : 
						DoCapture(BLACK_ROOK,to, board);
						break;
					case BLACK_QUEEN : 
						DoCapture(BLACK_QUEEN, to, board);
						break;
					case BLACK_KNIGHT : 
						DoCapture(BLACK_KNIGHT, to, board);
						break;
					case BLACK_BISHOP : 
						DoCapture(BLACK_BISHOP, to, board);
						break;
					case BLACK_KING :
						DoCapture(BLACK_KING, to, board);
						break;
					default : 
						break;
				}
			}
			break;
		case WHITE_KNIGHT : // Piece is a white knight : 
			MovePiece(WHITE_KNIGHT, to, from, board);	
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;	
			if(capt != 0)
			{
				switch (capt)
				{
					case BLACK_PAWN : 
						DoCapture(BLACK_PAWN, to, board);
						break;
					case BLACK_ROOK : 
						DoCapture(BLACK_ROOK, to, board);
						break;
					case BLACK_QUEEN : 
						DoCapture(BLACK_QUEEN, to, board);
						break;
					case BLACK_KNIGHT : 
						DoCapture(BLACK_KNIGHT, to, board);
						break;
					case BLACK_BISHOP : 
						DoCapture(BLACK_BISHOP, to, board);
						break;
					case BLACK_KING:
						DoCapture(BLACK_KING, to, board);
						break;
					default : 
						break;
				}
			}
			break;
		case WHITE_QUEEN : // Move is a white queen : 
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			MovePiece(WHITE_QUEEN, to, from, board);
			if(capt != 0)
			{
				switch (capt)
				{
					case BLACK_PAWN : 
						DoCapture(BLACK_PAWN, to, board);
						break;
					case BLACK_ROOK : 
						DoCapture(BLACK_ROOK,to, board);
						break;
					case BLACK_QUEEN : 
						DoCapture(BLACK_QUEEN,to, board);
						break;
					case BLACK_KNIGHT : 
						DoCapture(BLACK_KNIGHT, to, board);
						break;
					case BLACK_BISHOP : 
						DoCapture(BLACK_BISHOP, to, board);
						break;
					case BLACK_KING:
						DoCapture(BLACK_KING, to,  board);
						break;
					default :
						break;
				}
			}
			break;
		case WHITE_ROOK : // Move is a white rook : 
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			if(castling != 0)
			{
				
				if(castling == KINGSIDECASTLE) //  this is a white king side castle
				{   // setcastle only sets castling to 1 (KINGSIDECASTLE) or 2 (QUEENSIDECASTLE). It is not called
					// if we are not setting the castling bit, in which case it will be 0
					if(board->whitecastlingpermissions & QUEENSIDECASTLE) board->hashkey = board->hashkey ^ board->wqsckey;
					if(board->whitecastlingpermissions & KINGSIDECASTLE) board->hashkey = board->hashkey ^ board->wksckey;
					board->whitecastlingpermissions = 0;
					MovePiece(WHITE_ROOK, F1, H1, board);
					MovePiece(WHITE_KING, G1, E1, board );
				}
				if(castling == QUEENSIDECASTLE) // this is a white king side castle 
				{
					if(board->whitecastlingpermissions & QUEENSIDECASTLE) board->hashkey = board->hashkey ^ board->wqsckey;
					if(board->whitecastlingpermissions & KINGSIDECASTLE) board->hashkey = board->hashkey ^ board->wksckey;
					board->whitecastlingpermissions = 0;
					MovePiece(WHITE_ROOK, D1, A1, board);
					MovePiece(WHITE_KING, C1, E1, board);					
				}
			}
			else
			{
				MovePiece(WHITE_ROOK, to,from, board);
				if((from == A1) ) 
				{
					if(board->whitecastlingpermissions & QUEENSIDECASTLE)
						board->hashkey = board->hashkey ^ board->wqsckey;
					board->whitecastlingpermissions = board->whitecastlingpermissions & ((~QUEENSIDECASTLE) & 0xf);
				
				}
				if (from == H1)
				{
					if(board->whitecastlingpermissions & KINGSIDECASTLE)
						board->hashkey = board->hashkey ^ board->wksckey;
					board->whitecastlingpermissions = board->whitecastlingpermissions & ((~KINGSIDECASTLE) & 0xf);	
				}

				if(capt != 0)
				{
		
					switch (capt)
					{
						case BLACK_PAWN : 
							DoCapture(BLACK_PAWN, to, board);
							break;
						case BLACK_ROOK : 
							DoCapture(BLACK_ROOK, to, board);
							break;
						case BLACK_QUEEN : 
							DoCapture(BLACK_QUEEN, to, board);
							break;
						case BLACK_KNIGHT : 
							DoCapture(BLACK_KNIGHT, to, board);
							break;
						case BLACK_BISHOP : 
							DoCapture(BLACK_BISHOP, to, board);
							break;
						case BLACK_KING :
							DoCapture(BLACK_KING,to, board);
							break;
						default : 
							break;
					}
				}
			}
			break;
		case WHITE_KING : // Move is a white king non castle move: 
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			if(board->whitecastlingpermissions & KINGSIDECASTLE) board->hashkey ^= board->wksckey;
			if(board->whitecastlingpermissions & QUEENSIDECASTLE) board->hashkey ^= board->wqsckey;
			board->whitecastlingpermissions = 0;
			MovePiece(WHITE_KING, to, from, board);
			if(capt != 0)
			{
				switch (capt)
					{
						case BLACK_PAWN : 
							DoCapture(BLACK_PAWN, to, board);
							break;
						case BLACK_ROOK : 
							DoCapture(BLACK_ROOK, to, board);
							break;
						case BLACK_QUEEN : 
							DoCapture(BLACK_QUEEN, to, board);
							break;
						case BLACK_KNIGHT : 
							DoCapture(BLACK_KNIGHT, to, board);
							break;
						case BLACK_BISHOP : 
							DoCapture(BLACK_BISHOP, to, board);
							break;
						case BLACK_KING : 
							DoCapture(BLACK_KING,to, board);
							break;
						default :
							break;
					}
			}
			break;
		case BLACK_PAWN : // Move is a black pawn
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			board->fiftymove = 0;			
			if(RANKS[from] == 7 && RANKS[to] == 5)
			{	

				board->enpassantsquare = to+8; 
				board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			}
			MovePiece(BLACK_PAWN, to, from, board);
			if(capt != 0)
			{
				switch (capt)
				{
					case WHITE_PAWN : 
						if(IsEnpassant(move))
						{
							DoCapture(WHITE_PAWN, to+8, board);
							board->squares[to+8] = EMPTY;
						}
						else
							DoCapture(WHITE_PAWN, to,board);
						break;	
					case WHITE_BISHOP : 
						DoCapture(WHITE_BISHOP,to,board);
						break;
					case WHITE_KNIGHT :
						DoCapture(WHITE_KNIGHT, to,board);
						break;
					case WHITE_ROOK : 
						DoCapture(WHITE_ROOK,to,board);
						break;
					case WHITE_QUEEN :
						DoCapture(WHITE_QUEEN,to,board);
						break;
					case WHITE_KING : 
						DoCapture(WHITE_KING,to, board);
						break;
					default : 
						break;
				}

				if(prom != 0) // pawn promotion capture
				{
					switch(prom)
					{
						case BLACK_BISHOP : 
							board->squares[to] = BLACK_BISHOP;
							board->blackbishops ^= BITSET[to];
							board->blackpawns ^= BITSET[to];
							board->hashkey ^= board->piecekeys[BLACK_PAWN][to];
							board->hashkey ^= board->piecekeys[BLACK_BISHOP][to];
							board->material -= BISHOP_VALUE;
							board->material += PAWN_VALUE;
							break;
						case BLACK_ROOK : 
							board->squares[to] = BLACK_ROOK;
							board->blackrooks ^= BITSET[to];
							board->blackpawns ^= BITSET[to];
							board->hashkey ^= board->piecekeys[BLACK_ROOK][to];
							board->hashkey ^= board->piecekeys[BLACK_PAWN][to];
							board->material -= ROOK_VALUE;
							board->material += PAWN_VALUE;
							break;
						case BLACK_KNIGHT : 
							board->squares[to] = BLACK_KNIGHT;
							board->blackknights ^= BITSET[to];
							board->blackpawns ^= BITSET[to];
							board->hashkey ^= board->piecekeys[BLACK_KNIGHT][to];
							board->hashkey ^= board->piecekeys[BLACK_PAWN][to];
							board->material -= KNIGHT_VALUE;
							board->material += PAWN_VALUE;
							break;
						case BLACK_QUEEN : 
							board->squares[to] = BLACK_QUEEN;
							board->blackqueen ^= BITSET[to];
							board->blackpawns ^= BITSET[to];
							board->hashkey ^= board->piecekeys[BLACK_QUEEN][to];
							board->hashkey ^= board->piecekeys[BLACK_PAWN][to];
							board->material -= QUEEN_VALUE;
							board->material += PAWN_VALUE;
							break;
						default : 
							break;
					}								
				}					
			}
			else if (prom != 0)
			{
				switch(prom)
				{
					case BLACK_BISHOP : 
						board->squares[to] = BLACK_BISHOP;
						board->blackbishops ^= BITSET[to];
						board->blackpawns ^= BITSET[to];
						board->material -= BISHOP_VALUE;
						board->material += PAWN_VALUE;
						board->hashkey ^= board->piecekeys[BLACK_BISHOP][to];
						board->hashkey ^= board->piecekeys[BLACK_PAWN][to];
						break;
					case BLACK_ROOK : 
						board->squares[to] = BLACK_ROOK;
						board->blackrooks ^= BITSET[to];
						board->blackpawns ^= BITSET[to];
						board->material -= ROOK_VALUE;
						board->material += PAWN_VALUE;
						board->hashkey ^= board->piecekeys[BLACK_ROOK][to];
						board->hashkey ^= board->piecekeys[BLACK_PAWN][to];
						break;
					case BLACK_KNIGHT : 
						board->squares[to] = BLACK_KNIGHT;
						board->blackknights ^= BITSET[to];
						board->blackpawns ^= BITSET[to];
						board->material -= KNIGHT_VALUE;
						board->material += PAWN_VALUE;
						board->hashkey ^= board->piecekeys[BLACK_KNIGHT][to];
						board->hashkey ^= board->piecekeys[BLACK_PAWN][to];
						break;
					case BLACK_QUEEN : 
						board->squares[to] = BLACK_QUEEN;
						board->blackqueen ^= BITSET[to];
						board->blackpawns ^= BITSET[to];
						board->material -= QUEEN_VALUE;
						board->material += PAWN_VALUE;
						board->hashkey ^= board->piecekeys[BLACK_QUEEN][to];
						board->hashkey ^= board->piecekeys[BLACK_PAWN][to];			
						break;	
					default : 
						break;		

				}
			}
			break;
		case BLACK_BISHOP : // Move is a black bishop : 
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			MovePiece(BLACK_BISHOP, to, from,board);
			if(capt != 0)
			{
				switch (capt)
				{
					case WHITE_PAWN : 
						DoCapture(WHITE_PAWN, to, board);
						break;
					case WHITE_ROOK : 
						DoCapture(WHITE_ROOK, to, board);
						break;
					case WHITE_QUEEN : 
						DoCapture(WHITE_QUEEN, to, board);
						break;
					case WHITE_KNIGHT : 
						DoCapture(WHITE_KNIGHT, to, board);
						break;
					case WHITE_BISHOP : 
						DoCapture(WHITE_BISHOP, to, board);
						break;
					case WHITE_KING : 
						DoCapture(WHITE_KING,to, board);
						break;
					default : 
						break;
				}
			}
			break;
		case BLACK_KNIGHT : // Piece is a black knight
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			MovePiece(BLACK_KNIGHT, to, from, board);
				if(capt != 0)
				{
					board->enpassantsquare = -1;
					switch (capt)
					{
						case WHITE_PAWN : 
							DoCapture(WHITE_PAWN, to, board);
							break;
						case WHITE_ROOK : 
							DoCapture(WHITE_ROOK, to, board);
							break;
						case WHITE_QUEEN : 
							DoCapture(WHITE_QUEEN, to, board);
							break;
						case WHITE_KNIGHT : 
							DoCapture(WHITE_KNIGHT, to, board);
							break;
						case WHITE_BISHOP : 
							DoCapture(WHITE_BISHOP, to, board);
							break;
						case WHITE_KING:
							DoCapture(WHITE_KING,to, board);
							break;
						default : 
							break;
					}
				}
			break;
		case BLACK_ROOK: // Move is a black rook : 
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			if(castling != 0)
			{
				if(castling == QUEENSIDECASTLE) //  this is a black queen side castle // 2nd bit
				{
					if(board->blackcastlingpermissions & KINGSIDECASTLE) board->hashkey = board->hashkey ^ board->bksckey;
					if(board->blackcastlingpermissions & QUEENSIDECASTLE) board->hashkey = board->hashkey ^ board->bqsckey;
					board->blackcastlingpermissions = 0;
					MovePiece(BLACK_KING, C8, E8, board);
					MovePiece(BLACK_ROOK, D8, A8, board);
				}
				if(castling == KINGSIDECASTLE) // this is a black king side castle // 1st bit
				{
					if(board->blackcastlingpermissions & KINGSIDECASTLE) board->hashkey = board->hashkey ^ board->bksckey;
					if(board->blackcastlingpermissions & QUEENSIDECASTLE) board->hashkey = board->hashkey ^ board->bqsckey;
					board->blackcastlingpermissions = 0;
					MovePiece(BLACK_KING, G8, E8, board);
					MovePiece(BLACK_ROOK, F8, H8, board);				
				}
			}
			else
			{
				MovePiece(BLACK_ROOK, to, from, board);
				if(from == A8)
				{
					if(board->blackcastlingpermissions & QUEENSIDECASTLE) board->hashkey = board->hashkey ^ board->bqsckey;
					board->blackcastlingpermissions = board->blackcastlingpermissions & ((~QUEENSIDECASTLE) & 0xf);
				}
				if(from == H8)
				{
					if(board->blackcastlingpermissions & KINGSIDECASTLE) board->hashkey = board->hashkey ^ board->bksckey;
					board->blackcastlingpermissions = board->blackcastlingpermissions & ((~KINGSIDECASTLE) & 0xf);
				}
				if(capt != 0)
				{
					switch (capt)
					{
						case WHITE_PAWN : 
							DoCapture(WHITE_PAWN, to, board);
							break;
						case WHITE_ROOK : 
							DoCapture(WHITE_ROOK, to, board);
							break;
						case WHITE_QUEEN : 
							DoCapture(WHITE_QUEEN, to, board);
							break;
						case WHITE_KNIGHT : 
							DoCapture(WHITE_KNIGHT, to, board);
							break;
						case WHITE_BISHOP : 
							DoCapture(WHITE_BISHOP, to, board);
							break;
						case WHITE_KING :
							DoCapture(WHITE_KING,to, board);
							break;
						default : 
							break;
					}	
				}
			}
			break;
		case BLACK_QUEEN : // Move is a black queen : 
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			MovePiece(BLACK_QUEEN, to, from, board);
			if(capt != 0)
			{
				switch (capt)
				{
					case WHITE_PAWN : 
						DoCapture(WHITE_PAWN, to, board);
						break;
					case WHITE_ROOK : 
						DoCapture(WHITE_ROOK, to, board);
						break;
					case WHITE_QUEEN : 
						DoCapture(WHITE_QUEEN, to, board);
						break;
					case WHITE_KNIGHT : 
						DoCapture(WHITE_KNIGHT, to, board);
						break;
					case WHITE_BISHOP : 
						DoCapture(WHITE_BISHOP, to, board);
						break;
					case WHITE_KING :
						DoCapture(WHITE_KING,to, board);
						break;
					default : 
						break;
				}	
			}
			break;		
		case BLACK_KING: // Move is a black king non castle move:
			if(board -> enpassantsquare != -1) board->hashkey ^= board->enpassantkeys[board->enpassantsquare];
			board->enpassantsquare = -1;
			MovePiece(BLACK_KING, to, from, board);
			if(board->blackcastlingpermissions & KINGSIDECASTLE) board->hashkey ^= board->bksckey;
			if(board->blackcastlingpermissions & QUEENSIDECASTLE) board->hashkey ^= board->bqsckey;
			board->blackcastlingpermissions = 0;
			if(capt != 0)
			{
				switch (capt)
				{
					case WHITE_PAWN : 
						DoCapture(WHITE_PAWN, to, board);
						break;
					case WHITE_ROOK : 
						DoCapture(WHITE_ROOK, to, board);
						break;
					case WHITE_QUEEN : 
						DoCapture(WHITE_QUEEN, to, board);
						break;
					case WHITE_KNIGHT : 
						DoCapture(WHITE_KNIGHT, to, board);
						break;
					case WHITE_BISHOP : 
						DoCapture(WHITE_BISHOP, to, board);
						break;
					case WHITE_KING : 
						DoCapture(WHITE_KING,to, board);
						break;
					default : 
						break;
				}	
			}
			break;

		default : 
			break;
	}			
	 
	board->gameindex++;
	board->side = 1 ^ board->side;
	board->hashkey ^= board->whitesidekey;
	board->hashkey ^= board->blacksidekey;
}

void UnmakeMove(int move, Board *board)
{
	
	int piece = GetPiece(move);
	int from = GetFrom(move);
	int to = GetTo(move);
	int capt = GetCapt(move);
	int prom = GetProm(move);
	int castling = GetCastle(move);
	switch (piece)
	{
		case WHITE_PAWN : // Move is a white pawn
			MovePiece(WHITE_PAWN, from, to, board); //reset attacking piece, attacking side, square bit(*board)s and hash keys
				// difference between undoing a pawn promotion capture vs regular capture ? 
			if (capt != 0)
			{
				switch (capt)
				{
					case BLACK_PAWN : 
						if(IsEnpassant(move))
						{
							UndoCapture(BLACK_PAWN, to-8, board);
						}
						else
						{
							UndoCapture(BLACK_PAWN, to, board);
						}	
						break;				
					case BLACK_BISHOP : 
						UndoCapture(BLACK_BISHOP, to, board);
						break;
					case BLACK_KNIGHT : 
						UndoCapture(BLACK_KNIGHT, to, board);
						break;
					case BLACK_QUEEN : 
						UndoCapture(BLACK_QUEEN, to, board);
						break;
					case BLACK_ROOK : 
						UndoCapture(BLACK_ROOK, to, board);
						break;
					case BLACK_KING : 
						UndoCapture(BLACK_KING, to, board);
						break;
					default : 
						break;
				}

				if(prom != 0) // undo promotion captures
				{
					switch(prom)
					{
						case WHITE_BISHOP : 
							board->whitebishops ^= BITSET[to];
							board->whitepawns ^= BITSET[to];
							board->material -= BISHOP_VALUE;
							board->material += PAWN_VALUE;
							break;
						case WHITE_ROOK : 
							board->whiterooks ^= BITSET[to];
							board->whitepawns ^= BITSET[to];
							board->material -= ROOK_VALUE;
							board->material += PAWN_VALUE;
							break;
						case WHITE_KNIGHT : 
							board->whiteknights ^= BITSET[to];
							board->whitepawns ^= BITSET[to];
							board->material -= KNIGHT_VALUE;
							board->material += PAWN_VALUE;
							break;
						case WHITE_QUEEN : 
							board->whitequeen ^= BITSET[to];
							board->whitepawns ^= BITSET[to];
							board->material -= QUEEN_VALUE;
							board->material += PAWN_VALUE;
							break;
						default : 
							break;
					}
				}
			}
			// undo normal pawn promotions

			else if (prom != 0) // undo white pawn promotion
			{
				switch(prom)
				{
					//whitepieces and all pieces bit(*board)s do not have to be adjusted
					case WHITE_BISHOP : 
						board->whitebishops ^= BITSET[to];
						board->whitepawns ^= BITSET[to]; // since this bit will be set in MovePiece
						board->material += PAWN_VALUE;
						board->material -= BISHOP_VALUE;
						break;
					case WHITE_ROOK : 
						board->whiterooks ^= BITSET[to];
						board->whitepawns ^= BITSET[to];
						board->material += PAWN_VALUE;
						board->material -= ROOK_VALUE;
						break;
					case WHITE_KNIGHT : 
						board->whiteknights ^= BITSET[to];
						board->whitepawns ^= BITSET[to];
						board->material += PAWN_VALUE;
						board->material -= KNIGHT_VALUE;
						break;
					case WHITE_QUEEN : 
						board->whitequeen ^= BITSET[to];
						board->whitepawns ^= BITSET[to];
						board->material += PAWN_VALUE;
						board->material -= QUEEN_VALUE;
						break;
					default:
						break;
				}
			}
			break;
		case WHITE_BISHOP : // Move is a white bishop : 
			MovePiece(WHITE_BISHOP, from, to, board);	
			if(capt != 0)
			{
				switch (capt)
				{
					case BLACK_PAWN : 
						UndoCapture(BLACK_PAWN, to, board);
						break;
					case BLACK_ROOK : 
						UndoCapture(BLACK_ROOK,to, board);
						break;
					case BLACK_QUEEN : 
						UndoCapture(BLACK_QUEEN, to, board);
						break;
					case BLACK_KNIGHT : 
						UndoCapture(BLACK_KNIGHT, to, board);
						break;
					case BLACK_BISHOP : 
						UndoCapture(BLACK_BISHOP, to, board);
						break;
					case BLACK_KING : 
						UndoCapture(BLACK_KING, to, board);
						break;
					default : 
						break;
				}
			}
			break;
		case WHITE_KNIGHT : // Piece is a white knight : 
			MovePiece(WHITE_KNIGHT, from, to, board);		
			if(capt != 0)
			{
				switch (capt)
				{
					case BLACK_PAWN : 
						UndoCapture(BLACK_PAWN, to, board);
						break;
					case BLACK_ROOK : 
						UndoCapture(BLACK_ROOK, to, board);
						break;
					case BLACK_QUEEN : 
						UndoCapture(BLACK_QUEEN, to, board);
						break;
					case BLACK_KNIGHT : 
						UndoCapture(BLACK_KNIGHT, to, board);
						break;
					case BLACK_BISHOP : 
						UndoCapture(BLACK_BISHOP, to, board);
						break;
					case BLACK_KING :
						UndoCapture(BLACK_KING, to, board);
						break;
					default : 
						break;
				}
			}
			break;
		case WHITE_QUEEN : // Move is a white queen : 
			MovePiece(WHITE_QUEEN, from, to, board);
			if(capt != 0)
			{
				switch (capt)
				{
					case BLACK_PAWN : 
						UndoCapture(BLACK_PAWN, to, board);
						break;
					case BLACK_ROOK : 
						UndoCapture(BLACK_ROOK,to, board);
						break;
					case BLACK_QUEEN : 
						UndoCapture(BLACK_QUEEN,to, board);
						break;
					case BLACK_KNIGHT : 
						UndoCapture(BLACK_KNIGHT, to, board);
						break;
					case BLACK_BISHOP : 
						UndoCapture(BLACK_BISHOP, to, board);
						break;
					case BLACK_KING :
						UndoCapture(BLACK_KING, to, board);
						break;
					default : 
						break;
				}
			}
			break;
		case WHITE_ROOK : // Move is a white rook : 
			if(castling != 0)
			{
				if(castling == KINGSIDECASTLE) //  this is a white king side castle, hence reset third bit to 1
				{
					MovePiece(WHITE_ROOK, H1, F1, board);
					MovePiece(WHITE_KING, E1, G1, board);
				}
				if(castling == QUEENSIDECASTLE) // this is a white castle queen side
				{
					MovePiece(WHITE_ROOK, A1, D1, board);
					MovePiece(WHITE_KING, E1, C1, board);					
				}
			}
			else
			{
				MovePiece(WHITE_ROOK, from,to, board);
				if(capt != 0)
				{
		
					switch (capt)
					{
						case BLACK_PAWN : 
							UndoCapture(BLACK_PAWN, to,board);
							break;
						case BLACK_ROOK : 
							UndoCapture(BLACK_ROOK, to,board);
							break;
						case BLACK_QUEEN : 
							UndoCapture(BLACK_QUEEN, to, board);
							break;
						case BLACK_KNIGHT : 
							UndoCapture(BLACK_KNIGHT, to, board);
							break;
						case BLACK_BISHOP : 
							UndoCapture(BLACK_BISHOP, to, board);
							break;
						case BLACK_KING :
							UndoCapture(BLACK_KING,to, board);
							break;
						default : 
							break;
					}
				}
			}
			break;
		case WHITE_KING : // Move is a white king non castle move: 
			MovePiece(WHITE_KING, from, to, board);
			if(capt != 0)
			{
				switch (capt)
					{
						case BLACK_PAWN : 
							UndoCapture(BLACK_PAWN, to, board);
							break;
						case BLACK_ROOK : 
							UndoCapture(BLACK_ROOK, to, board);
							break;
						case BLACK_QUEEN : 
							UndoCapture(BLACK_QUEEN, to, board);
							break;
						case BLACK_KNIGHT : 
							UndoCapture(BLACK_KNIGHT, to, board);
							break;
						case BLACK_BISHOP : 
							UndoCapture(BLACK_BISHOP, to, board);
							break;
						case BLACK_KING :
							UndoCapture(BLACK_KING,to, board);
							break;
						default :
							break;
					}
			}
			break;
		case BLACK_PAWN : // Move is a black pawn
			MovePiece(BLACK_PAWN, from, to, board); //reset attacking piece, attacking side, square bit(*board)s and hash keys
			if(capt != 0)
			{
				switch (capt)
				{
					case WHITE_PAWN : 
						if(IsEnpassant(move))
						{
							UndoCapture(WHITE_PAWN, to+8, board);
						}
						else
						{
							UndoCapture(WHITE_PAWN, to, board);
						}
						break;
					case WHITE_BISHOP : 
						UndoCapture(WHITE_BISHOP, to, board);
						break;
					case WHITE_KNIGHT :
						UndoCapture(WHITE_KNIGHT, to, board);
						break;
					case WHITE_ROOK : 
						UndoCapture(WHITE_ROOK,to,  board);
						break;
					case WHITE_QUEEN :
						UndoCapture(WHITE_QUEEN, to, board);
						break;
					case WHITE_KING : 
						board->whiteking ^= BITSET[to];
						board->whitepieces ^= BITSET[to];
						board->allpieces ^= BITSET[to];
						board->squares[to] = WHITE_KING;
						break;
					default :
						break;
				}
				if (prom != 0)
				{
					switch(prom)
					{
						case BLACK_BISHOP : 
							board->blackbishops ^= BITSET[to];
							board->blackpawns ^= BITSET[to];
							board->material += BISHOP_VALUE;
							board->material -= PAWN_VALUE;
							break;
						case BLACK_ROOK : 
							board->blackrooks ^= BITSET[to];
							board->blackpawns ^= BITSET[to];
							board->material += ROOK_VALUE;
							board->material -= PAWN_VALUE;
							break;
						case BLACK_KNIGHT : 
							board->blackknights ^= BITSET[to];
							board->blackpawns ^= BITSET[to];
							board->material += KNIGHT_VALUE;
							board->material -= PAWN_VALUE;
							break;
						case BLACK_QUEEN : 
							board->blackqueen ^= BITSET[to];
							board->blackpawns ^= BITSET[to];
							board->material += QUEEN_VALUE;
							board->material -= PAWN_VALUE;
							break;
						default :
							break;

					}
				}
			}
			else if (prom != 0)
			{
				switch(prom)
				{
					//blackpieces and all pieces bit(*board)s do not have to be adjusted
					case BLACK_BISHOP : 
						board->blackbishops ^= BITSET[to];
						board->blackpawns ^= BITSET[to]; // since this bit will be set in MovePiece
						board->material += BISHOP_VALUE;
						board->material -= PAWN_VALUE;
						break;
					case BLACK_ROOK : 
						board->blackrooks ^= BITSET[to];
						board->blackpawns ^= BITSET[to];
						board->material += ROOK_VALUE;
						board->material -= PAWN_VALUE;
						break;
					case BLACK_KNIGHT : 
						board->blackknights ^= BITSET[to];
						board->blackpawns ^= BITSET[to];
						board->material += KNIGHT_VALUE;
						board->material -= PAWN_VALUE;
						break;
					case BLACK_QUEEN : 
						board->blackqueen ^= BITSET[to];
						board->blackpawns ^= BITSET[to];
						board->material += QUEEN_VALUE;
						board->material -= PAWN_VALUE;
						break;
					default : 
						break;

				}
			}
			break;
		case BLACK_BISHOP : // Move is a black bishop : 
			MovePiece(BLACK_BISHOP, from, to, board);
			if(capt != 0)
			{
				switch (capt)
				{
					case WHITE_PAWN : 
						UndoCapture(WHITE_PAWN, to, board);
						break;
					case WHITE_ROOK : 
						UndoCapture(WHITE_ROOK,to, board);
						break;
					case WHITE_QUEEN : 
						UndoCapture(WHITE_QUEEN, to, board);
						break;
					case WHITE_KNIGHT : 
						UndoCapture(WHITE_KNIGHT, to, board);
						break;
					case WHITE_BISHOP : 
						UndoCapture(WHITE_BISHOP, to, board);
						break;
					case WHITE_KING :
						UndoCapture(WHITE_KING, to, board);
						break;
					default : 
						break;
				}
			}
			break;
		case BLACK_KNIGHT : // Piece is a black knight : 
			MovePiece(BLACK_KNIGHT, from, to, board);		
			if(capt != 0)
			{
				switch (capt)
				{
					case WHITE_PAWN : 
						UndoCapture(WHITE_PAWN, to, board);
						break;
					case WHITE_ROOK : 
						UndoCapture(WHITE_ROOK, to, board);
						break;
					case WHITE_QUEEN : 
						UndoCapture(WHITE_QUEEN, to, board);
						break;
					case WHITE_KNIGHT : 
						UndoCapture(WHITE_KNIGHT, to, board);
						break;
					case WHITE_BISHOP : 
						UndoCapture(WHITE_BISHOP, to, board);
						break;
					case WHITE_KING :
						UndoCapture(WHITE_KING, to, board);
						break;
					default :
						break;
				}
			}
			break;
		case BLACK_QUEEN : // Move is a black queen : 
			MovePiece(BLACK_QUEEN, from, to, board);
			if(capt != 0)
			{
				switch (capt)
				{
					case WHITE_PAWN : 
						UndoCapture(WHITE_PAWN, to, board);
						break;
					case WHITE_ROOK : 
						UndoCapture(WHITE_ROOK,to,board);
						break;
					case WHITE_QUEEN : 
						UndoCapture(WHITE_QUEEN,to, board);
						break;
					case WHITE_KNIGHT : 
						UndoCapture(WHITE_KNIGHT, to, board);
						break;
					case WHITE_BISHOP : 
						UndoCapture(WHITE_BISHOP, to, board);
						break;
					case WHITE_KING :
						UndoCapture(WHITE_KING, to, board);
						break;
					default : 
						break;
				}
			}
			break;
		case BLACK_ROOK : // Move is a black rook : 
			if(castling != 0)
			{
				if(castling == KINGSIDECASTLE) //  this is a black king side castle
				{
					MovePiece(BLACK_ROOK, H8, F8, board);
					MovePiece(BLACK_KING,E8, G8, board);
				}
				if(castling == QUEENSIDECASTLE)
				{
					MovePiece(BLACK_ROOK, A8, D8, board);
					MovePiece(BLACK_KING, E8, C8, board);					
				}
			}
			else
			{
				MovePiece(BLACK_ROOK, from,to, board);
				if(capt != 0)
				{
		
					switch (capt)
					{
						case WHITE_PAWN : 
							UndoCapture(WHITE_PAWN, to, board);
							break;
						case WHITE_ROOK : 
							UndoCapture(WHITE_ROOK, to, board);
							break;
						case WHITE_QUEEN : 
							UndoCapture(WHITE_QUEEN, to, board);
							break;
						case WHITE_KNIGHT : 
							UndoCapture(WHITE_KNIGHT, to, board);
							break;
						case WHITE_BISHOP : 
							UndoCapture(WHITE_BISHOP, to, board);
							break;
						case WHITE_KING :
							UndoCapture(WHITE_KING,to, board);
							break;
					}
				}
			}
			break;
		case BLACK_KING : // Move is a black king non castle move: 
			MovePiece(BLACK_KING, from, to, board);
			if(capt != 0)
			{
				switch (capt)
					{
						case WHITE_PAWN : 
							UndoCapture(WHITE_PAWN, to, board);
							break;
						case WHITE_ROOK : 
							UndoCapture(WHITE_ROOK, to, board);
							break;
						case WHITE_QUEEN : 
							UndoCapture(WHITE_QUEEN, to, board);
							break;
						case WHITE_KNIGHT : 
							UndoCapture(WHITE_KNIGHT, to, board);
							break;
						case WHITE_BISHOP :
							UndoCapture(WHITE_BISHOP, to, board);
							break;
						case WHITE_KING :
							UndoCapture(WHITE_KING,to, board);
							break;
						default :
							break;
					}
			}
			break;
		default : 
			break;
	}
		board->gameindex--;
		board->whitecastlingpermissions = board->gameline[board->gameindex].whitecastlingpermissions;
		board->blackcastlingpermissions = board->gameline[board->gameindex].blackcastlingpermissions;
		board->enpassantsquare = board->gameline[board->gameindex].enpassantsquare;
		board->fiftymove = board->gameline[board->gameindex].fiftymove;
		board->hashkey = board->gameline[board->gameindex].hashkey;
		board->side = 1 ^ board->side; 
		board->material = board->gameline[board->gameindex].material;
}







void MovePiece(int piece, int to, int from, Board* board)
{ // will adjust the specific piece bitboard, the side bitboard, the allpieces bitboard, and the board.squares array
	// to undo MovePiece just call move piece again 
	if(piece != WHITE_PAWN && (piece != BLACK_PAWN)) board->enpassantsquare = -1;
	switch (piece)
	{
		case WHITE_PAWN : 
			board->whitepawns ^= BITSET[to]; board->whitepawns ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->whitepieces ^= BITSET[to]; board->whitepieces ^= BITSET[from];
			board->squares[to] = WHITE_PAWN; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[WHITE_PAWN][to]; 
			board->hashkey ^= board->piecekeys[WHITE_PAWN][from];
			break;
		case WHITE_BISHOP : 
			board->whitebishops ^= BITSET[to]; board->whitebishops ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->whitepieces ^= BITSET[to]; board->whitepieces ^= BITSET[from];
			board->squares[to] = WHITE_BISHOP; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[WHITE_BISHOP][to];
			board->hashkey ^= board->piecekeys[WHITE_BISHOP][from];
			break;
		case WHITE_KNIGHT : 
			board->whiteknights ^= BITSET[to]; board->whiteknights ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->whitepieces ^= BITSET[to]; board->whitepieces ^= BITSET[from];
			board->squares[to] = WHITE_KNIGHT; board->squares[from] = EMPTY;  board->hashkey ^= board->piecekeys[WHITE_KNIGHT][to];
			board->hashkey ^= board->piecekeys[WHITE_KNIGHT][from];
			break;
		case WHITE_ROOK : 
			board->whiterooks ^= BITSET[to]; board->whiterooks ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->whitepieces ^= BITSET[to]; board->whitepieces ^= BITSET[from];
			board->squares[to] = WHITE_ROOK; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[WHITE_ROOK][to];
			board->hashkey ^= board->piecekeys[WHITE_ROOK][from];
			break;
		case WHITE_QUEEN : 
			board->whitequeen ^= BITSET[to]; board->whitequeen ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->whitepieces ^= BITSET[to]; board->whitepieces ^= BITSET[from];
			board->squares[to] = WHITE_QUEEN; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[WHITE_QUEEN][to];
			board->hashkey ^= board->piecekeys[WHITE_QUEEN][from];
			break;
		case WHITE_KING : 
			board->whiteking ^= BITSET[to]; board->whiteking ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->whitepieces ^= BITSET[to]; board->whitepieces ^= BITSET[from];
			board->squares[to] = WHITE_KING; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[WHITE_KING][to];
			board->hashkey ^= board->piecekeys[WHITE_KING][from];	
			break;
		case BLACK_PAWN : 
			board->blackpawns ^= BITSET[to]; board->blackpawns ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->blackpieces ^= BITSET[to]; board->blackpieces ^= BITSET[from];
			board->squares[to] = BLACK_PAWN; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[BLACK_PAWN][to];
			board->hashkey ^= board->piecekeys[BLACK_PAWN][from];
			break;
		case BLACK_BISHOP : 
			board->blackbishops ^= BITSET[to]; board->blackbishops ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->blackpieces ^= BITSET[to]; board->blackpieces ^= BITSET[from];
			board->squares[to] = BLACK_BISHOP; board->squares[from] = EMPTY;  board->hashkey ^= board->piecekeys[BLACK_BISHOP][to];
			board->hashkey ^= board->piecekeys[BLACK_BISHOP][from];
			break;
		case BLACK_KNIGHT : 
			board->blackknights ^= BITSET[to]; board->blackknights ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->blackpieces ^= BITSET[to]; board->blackpieces ^= BITSET[from];
			board->squares[to] = BLACK_KNIGHT; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[BLACK_KNIGHT][to];
			board->hashkey ^= board->piecekeys[BLACK_KNIGHT][from];
			break;
		case BLACK_ROOK : 
			board->blackrooks ^= BITSET[to]; board->blackrooks ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->blackpieces ^= BITSET[to]; board->blackpieces ^= BITSET[from];
			board->squares[to] = BLACK_ROOK; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[BLACK_ROOK][to];
			board->hashkey ^= board->piecekeys[BLACK_ROOK][from];	
			break;
		case BLACK_QUEEN : 
			board->blackqueen ^= BITSET[to]; board->blackqueen ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->blackpieces ^= BITSET[to]; board->blackpieces ^= BITSET[from];
			board->squares[to] = BLACK_QUEEN; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[BLACK_QUEEN][to];
			board->hashkey ^= board->piecekeys[BLACK_QUEEN][from];
			break;
		case BLACK_KING : 
			board->blackking ^= BITSET[to]; board->blackking ^= BITSET[from]; board->allpieces ^= BITSET[to];
			board->allpieces ^= BITSET[from]; board->blackpieces ^= BITSET[to]; board->blackpieces ^= BITSET[from];
			board->squares[to] = BLACK_KING; board->squares[from] = EMPTY; board->hashkey ^= board->piecekeys[BLACK_KING][to];
			board->hashkey ^= board->piecekeys[BLACK_KING][from];
			break;
		default : 
			break;
	}

}


void DoCapture(int capt, int square, Board* board)
{ // adjusts the captured piece bitboard, then the side captured bitboard, then the allpieces bitboard and the board material
	board->fiftymove = 0;
	switch (capt)
	{
		case WHITE_PAWN : 
			board->whitepawns ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material -= PAWN_VALUE;
			board->hashkey ^= board->piecekeys[WHITE_PAWN][square];
			break;
		case WHITE_ROOK : 
			if((square == H1)  && (board->whitecastlingpermissions & KINGSIDECASTLE))
			{
				board->whitecastlingpermissions &= (~KINGSIDECASTLE);
				board->hashkey ^= board->wksckey;
			}
			if((square == A1)  && (board->whitecastlingpermissions & QUEENSIDECASTLE))
			{
				board->whitecastlingpermissions &= (~QUEENSIDECASTLE);
				board->hashkey ^= board->wqsckey;
			}
			board->whiterooks ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material -= ROOK_VALUE;
			board->hashkey ^= board->piecekeys[WHITE_ROOK][square];
			break;
		case WHITE_QUEEN : 
			board->whitequeen ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material -= QUEEN_VALUE;
			board->hashkey ^= board->piecekeys[WHITE_QUEEN][square];
			break;
		case WHITE_KNIGHT : 
			board->whiteknights ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material -= KNIGHT_VALUE;
			board->hashkey ^= board->piecekeys[WHITE_KNIGHT][square];
			break;
		case WHITE_BISHOP : 
			board->whitebishops ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material -= BISHOP_VALUE;
			board->hashkey ^= board->piecekeys[WHITE_BISHOP][square];
			break;
		/*case WHITE_KING :
			board->whiteking ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material -= KING_VALUE;
			board->hashkey ^= board->piecekeys[WHITE_KING][square];
			break;*/
		case BLACK_PAWN : 
			board->blackpawns ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material += PAWN_VALUE;
			board->hashkey ^= board->piecekeys[BLACK_PAWN][square];
			break;
		case BLACK_ROOK : 
			if((square == H8)  && (board->blackcastlingpermissions & KINGSIDECASTLE))
			{
				board->blackcastlingpermissions &= (~KINGSIDECASTLE);
				board->hashkey ^= board->bksckey;
			}
			if((square == A8)  && (board->blackcastlingpermissions & QUEENSIDECASTLE))
			{
				board->blackcastlingpermissions &= (~QUEENSIDECASTLE);
				board->hashkey ^= board->wqsckey;
			}
			board->blackrooks ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material += ROOK_VALUE;
			board->hashkey ^= board->piecekeys[BLACK_ROOK][square];
			break;
		case BLACK_QUEEN : 
			board->blackqueen ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material += QUEEN_VALUE;
			board->hashkey ^= board->piecekeys[BLACK_QUEEN][square];
			break;
		case BLACK_KNIGHT : 
			board->blackknights ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material += KNIGHT_VALUE;
			board->hashkey ^= board->piecekeys[BLACK_KNIGHT][square];
			break;
		case BLACK_BISHOP : 
			board->blackbishops ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material += BISHOP_VALUE;
			board->hashkey ^= board->piecekeys[BLACK_BISHOP][square];
			break;
		/*case BLACK_KING :
			board->blackking ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material += KING_VALUE;
			board->hashkey ^= board->piecekeys[BLACK_KING][square];
			break;*/
		default : 
			break;
	}
}
	


void UndoCapture(int capt, int square, Board* board)
{ // adjusts the captured piece bitboard, then the side captured bitboard, then the allpieces bitboard and the board material
	board->fiftymove = 0;
	switch (capt)
	{
		case WHITE_PAWN : 
			board->whitepawns ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = WHITE_PAWN;
			break;
		case WHITE_ROOK : 
			board->whiterooks ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = WHITE_ROOK;
			break;
		case WHITE_QUEEN : 
			board->whitequeen ^= BITSET[square];
			board->whitepieces^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = WHITE_QUEEN;
			break;
		case WHITE_KNIGHT : 
			board->whiteknights ^= BITSET[square];
			board->whitepieces^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = WHITE_KNIGHT;
			break;
		case WHITE_BISHOP : 
			board->whitebishops ^= BITSET[square];
			board->whitepieces^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = WHITE_BISHOP;
			break;
		case WHITE_KING :
			board->whiteking ^= BITSET[square];
			board->whitepieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = WHITE_KING;
			break;

		case BLACK_PAWN : 
			board->blackpawns ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = BLACK_PAWN;
			break;
		case BLACK_ROOK : 
			board->blackrooks ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = BLACK_ROOK;
			break;
		case BLACK_QUEEN : 
			board->blackqueen ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->material -= QUEEN_VALUE;
			board->squares[square] = BLACK_QUEEN;
			break;		
		case BLACK_KNIGHT : 
			board->blackknights ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = BLACK_KNIGHT;
			break;
		case BLACK_BISHOP : 
			board->blackbishops ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = BLACK_BISHOP;
			break;
		case BLACK_KING :
			board->blackking ^= BITSET[square];
			board->blackpieces ^= BITSET[square];
			board->allpieces ^= BITSET[square];
			board->squares[square] = BLACK_KING;
			break;
		default : 
			break;
	}
}

void ParseMove(std::string moveinput, Board* board)
{
	unsigned int move = 0;
	if(moveinput[0] == 'u') 
	{
		UnmakeMove(board->gameline[board->gameindex-1].move, board);
	}
	else if(moveinput[0] == 'k' || moveinput[0] == 'Q')
	{
		if(board->side == WHITE)
		{
			SetPiece(WHITE_ROOK, &move);
			if(moveinput[0] == 'k')
				SetCastle(KINGSIDECASTLE, &move);
			else
				SetCastle(QUEENSIDECASTLE, &move);

		}
		else
		{
			SetPiece(BLACK_ROOK, &move);
			if(moveinput[0] == 'k')
				SetCastle(KINGSIDECASTLE, &move);
			else
				SetCastle(QUEENSIDECASTLE, &move);
		}
	}

	else
	{
		int filefrom = moveinput[0] - 'a' + 1;
		int rankfrom = moveinput[1] - '0';
		int fileto = moveinput[2] - 'a' + 1;
		int rankto = moveinput[3] - '0';
		int squarefrom = rftosq(rankfrom, filefrom);
		int squareto = rftosq(rankto, fileto);
		int piece = board->squares[squarefrom];
		int capt = board->squares[squareto];
		
		unsigned int move = 0;
		SetPiece(piece, &move);
		SetFrom(squarefrom, &move);
		SetTo(squareto, &move);
		if((piece == BLACK_PAWN || piece == WHITE_PAWN) && squareto == board->enpassantsquare)
			SetEnpassant(&move);
		SetCapt(capt,&move);
		//StorePvMove(board,move);
		MakeMove(move, board);
	}
	
	


}