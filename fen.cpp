//Function that will take in a fen string input and use that to set up the board
#include <string>

void InitFromFen(const string& s, int i)
{


	int stringstart = 0;
	int k = 56;
	while(k >= 7)
	{
		stringstart = FillRank(k,stringstart, s);
		k = k - 8;
	}

	switch (s[stringstart])
	{
		case w : 
			side = WHITE;
			hashkey ^= whitesidekey;
			break;
		case b :
			side = BLACK;
			hashkey ^= blacksidekey;
			break;
		default : 
	}
	stringstart = stringstart + 2;
	if(s[stringstart] == '-') stringstart += 2;
	else
	{
		while(s[stringstart] == 'k' || s[stringstart] == 'K' || s[stringstart] == 'Q' || s[stringstart] = 'q')
		{
			if(s[stringstart] == 'k')
			{
				hashkey ^= bksckey;
				baord.blackcastlingpermissions |= KINGSIDECASTLE;
			}
			else if(s[stringstart] == 'q')
			{
				hashkey ^= bqsckey;
				blackcastlingpermissions |= QUEENSIDECASTLE;
			}
			else if (s[stringstart] == 'K' )
			{
				hashkey ^= wksckey;
				blackcastlingpermissions |= KINGSIDECASTLE;
			}
			else
			{
				hashkey ^= wqsckey;
				blackcastlingpermissions |= QUEENSIDECASTLE;
			}
			stringstart++;
		}
		stringstart++;		
	}

	// enpassant key
	if(s[stringstart] == '-') stringgstart += 2;
	else
	{
		int file = s[stringstart] - 'a' + 1;
		int rank = s[stringstart+1] - '0';
		int square = rank * 8 + file - 9;
		enpassantsquare = square;
		hashkey ^= enpassantkeys[square];
		stringstart += 3;
	}

	// fill in fifty moves 
	int fiftymove = s[stringstart++] - '0';
	if(s[stringstart] != ' ')
	{
		fiftymove = fiftymove * 10 + (s[stringstart] - '0');
		stringstart++;
	}
	fiftymove = fiftymove;
	stringstart++;
	// fill in full move clock // why ? 

	

}


int SquaresFill(const char s, int pos)
{
	switch(s)
	{
		case p :
			squares[pos] = BLACK_PAWN:
			blackpawns ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_PAWN][pos];
			material -= PAWN_VALUE;
			break;
		case n :
			squares[pos] = BLACK_KNIGHT:
			blackknights ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_KNIGHT][pos];
			material -= KNIGHT_VALUE;
			break;
		case b :
			squares[pos] = BLACK_BISHOP:
			blackbishops ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_BISHOP][pos];
			material -= BISHOP_VALUE;
			break;

		case q :
			squares[pos] = BLACK_QUEEN:
			blackqueen ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_QUEEN][pos];
			material -= QUEEN_VALUE;
			break;

		case r : 
			squares[pos] = BLACK_ROOK;
			blackrooks ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_ROOK][pos];
			allpieces ^= BITSET[pos];
			material -= ROOK_VALUE;
			break;

		case k :
			squares[pos] = BLACK_KING:
			blackking ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_KING][pos];
			material -= KING_VALUE;
			break;	


		case P :
			squares[pos] = WHITE_PAWN:
			whitepawns ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_PAWN][pos];
			material += PAWN_VALUE;
			break;
		case N :
			squares[pos] = WHITE_KNIGHT:
			whiteknights ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_KNIGHTS][pos];
			material += KNIGHT_VALUE;
			break;
		case B :
			squares[pos] = WHITE_BISHOP:
			whitebishops ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_BISHOP][pos];
			material += BISHOP_VALUE;
			break;

		case Q :
			squares[pos] = WHITE_QUEEN:
			whitequeen ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_QUEEN][pos];
			material += QUEEN_VALUE;
			break;

		case R : 
			squares[pos] = WHITE_ROOK;
			whiterooks ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_ROOK][pos];
			allpieces ^= BITSET[pos];
			material += ROOK_VALUE;
			break;

		case K :
			squares[pos] = WHITE_KING:
			whiteking ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_KING][pos];
			material += KING_VALUE;
			break;	 
	}
}

int FillRank(int rankstart,  int stringstart, const string& s)
{

	int numwritten = 0;
	int i = 0;
	for(; i < 8; i++)
	{
		char c = s[stringstart +i];
		if (c >= '0' && c <= '9')
		{
			int k = c - '0';
			for(int j = 0; j < k; j++)
			{
				squares[rankstart++] = EMPTY;
			}
			numwritten = numwritten + k;
		}
		else 
		{
			SquaresFill(c, rankstart);
			numwritten++;
		}
		if(numwritten = 8) break;	
	}
	return stringstart + i + 1;
}