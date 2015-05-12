#include "board.h"
#include "defs.h"
#include <iostream>


const int PawnTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int KnightTable[64] = {
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0		
};

const int BishopTable[64] = {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const int RookTable[64] = {
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0		
};

const int Mirror64[64] = {
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};


void Board::InitHashKey() // function will initialize the board hash key at the start position.
// There is no enpassant key here, and castling permissions are full and the side is white
{
	for(int i = 0; i < 64; i++)
	{
		switch (squares[i])
		{
			case WHITE_PAWN : 
				hashkey ^= piecekeys[WHITE_PAWN][i];
				break;
			case WHITE_BISHOP :
				hashkey ^= piecekeys[WHITE_BISHOP][i];
				break;
			case WHITE_KNIGHT :
				hashkey ^= piecekeys[WHITE_KNIGHT][i];
				break;
			case WHITE_QUEEN :
				hashkey ^= piecekeys[WHITE_QUEEN][i];
				break;
			case WHITE_KING : 
				hashkey ^= piecekeys[WHITE_KING][i];
				break;
			case WHITE_ROOK : 
				hashkey ^= piecekeys[WHITE_ROOK][i];
				break;
			case BLACK_PAWN :
				hashkey ^= piecekeys[BLACK_PAWN][i];
				break;
			case BLACK_BISHOP :
				hashkey ^= piecekeys[BLACK_BISHOP][i];
				break;
			case BLACK_KNIGHT : 
				hashkey ^= piecekeys[BLACK_KNIGHT][i];
				break;
			case BLACK_QUEEN : 
				hashkey ^= piecekeys[BLACK_QUEEN][i];
				break;
			case BLACK_KING : 
				hashkey ^= piecekeys[BLACK_KING][i];
				break;
			case BLACK_ROOK : 
				hashkey ^= piecekeys[BLACK_ROOK][i];
				break;
			case EMPTY :
				break; 		
		}	
	}
	hashkey ^= wksckey;
	hashkey ^= wqsckey;
	hashkey ^= bqsckey;
	hashkey ^= bksckey;
	hashkey ^= whitesidekey; // initially the position hashkey is going to have whitesidekey; // 
	// for each move hashkey ^= whitesidekey ^= blacksidekey // this will switch off one of the hash side keys and switch in the
	// the other one
}

void Board::InitBoard()
{
	inodes = 0;
	gameindex = 0;
	side = WHITE;
	hashkey = BITMAP(0);
	enpassantsquare = -1;
	material = 0;
	fiftymove = 0;
	blackking  =   0x1000000000000000;
	blackqueen =   0x0800000000000000;
	blackbishops = 0x2400000000000000;
	blackknights = 0x4200000000000000;
	blackrooks =   0x8100000000000000;
	blackpawns =   0x00ff000000000000;
	whitepawns =   0x000000000000ff00;
	whiteking =    0x0000000000000010;
	whitequeen =   0x0000000000000008;
	whitebishops = 0x0000000000000024;
	whiteknights = 0x0000000000000042;
	whiterooks =   0x0000000000000081;
	blackpieces = blackking | blackbishops | blackrooks | blackqueen | blackpawns | blackknights;
	whitepieces = whiteking | whitebishops | whiterooks | whitequeen | whitepawns | whiteknights;
	allpieces = whitepieces | blackpieces;
	// material = total white material - total black material which are both equal here
	whitecastlingpermissions = KINGSIDECASTLE | QUEENSIDECASTLE;
	blackcastlingpermissions = KINGSIDECASTLE | QUEENSIDECASTLE;
	squares[0] = squares[7] = WHITE_ROOK;
	squares[1] = squares[6] = WHITE_KNIGHT;
	squares[2] = squares[5] = WHITE_BISHOP;
	squares[3] = WHITE_QUEEN; squares[4] = WHITE_KING;
	for(int i = 8; i < 16; i++)
		squares[i] = WHITE_PAWN;
	squares[56] = squares[63] = BLACK_ROOK;
	squares[57] = squares[62] = BLACK_KNIGHT;
	squares[58] = squares[61] = BLACK_BISHOP;
	squares[59] = BLACK_QUEEN;
	squares[60] = BLACK_KING;
	for(int i = 48; i < 56; i++) squares[i] = BLACK_PAWN;
 	for(int i = 0; i < 32; i++) squares[16+i] = EMPTY;
 	for(int i = 0; i < 15; i++)
 	{
 		for(int j = 0; j<64; j++)
 		{
 			piecekeys[i][j] = R64;
 		}
 	}
 	for(int i =0; i<64; i++)
 		enpassantkeys[i] = R64;
 	for(int i=0; i<MAXPLY; i++)
 		movebuflen[i] = 0;
 	
 	wksckey = R64;
 	wqsckey = R64;
 	bksckey = R64;
 	bqsckey = R64;
 	whitesidekey = R64;
 	blacksidekey = R64;
 	InitHashKey();

}

void Board::DebugBoard()
{	
	std::cout << "White Pawns " << hex << whitepawns << endl;
	std::cout << "White Knights " << hex << whiteknights << endl;
	std::cout << "White Bishops " << hex << whitebishops << endl;
	std::cout << "White Rooks " << hex << whiterooks << endl;
	std::cout << "White Queen " << hex << whitequeen << endl;
	std::cout << "White King " << hex << whiteking << endl;
	std::cout << "Black Pawns " << hex << blackpawns << endl;
	std::cout << "Black Knights " << hex << blackknights << endl;
	std::cout << "Black Bishops " << hex << blackbishops << endl;
	std::cout << "Black Rooks " << hex << blackrooks << endl;
	std::cout << "Black Queen " << hex << blackqueen << endl;
	std::cout << "Black King " << hex << blackking << endl;
	std::cout << "White Pieces " << hex << whitepieces  << endl; 
	std::cout << "Black Pieces " << hex << blackpieces << endl;
	std::cout << "All Pieces " << hex << allpieces << endl;
	std::cout << "Hash Key " << hex << hashkey << endl;

}



void Board::InitFromFen(const string& s)
{
	inodes = 0;
	whitecastlingpermissions = 0;
	blackcastlingpermissions = 0;
	gameindex = 0;
	material = 0;
	hashkey = 0;
	for(int i =0; i < MAXPLY; i++) movebuflen[i] = 0;
	for(int i = 0; i < 15; i++)
 	{
 		for(int j = 0; j<64; j++)
 		{
 			piecekeys[i][j] = R64;
 		}
 	}
 	for(int i =0; i<64; i++)
 		enpassantkeys[i] = R64;

 	for(int i=0; i<MAXPLY; i++)
 		movebuflen[i] = 0;
 	
 	wksckey = R64;
 	wqsckey = R64;
 	bksckey = R64;
 	bqsckey = R64;
 	whitesidekey = R64;
 	blacksidekey = R64;

	int stringstart = 0;
	int k = 56;
	while(k >= 0)
	{
		stringstart = FillRank(k,stringstart, s);
		k = k - 8;
	}

	switch (s[stringstart])
	{
		case 'w' : 
			side = WHITE;
			hashkey ^= whitesidekey;
			break;
		case 'b' :
			side = BLACK;
			hashkey ^= blacksidekey;
			break;
		default : 
			break;
	}
	stringstart = stringstart + 2;
	if(s[stringstart] == '-') stringstart += 2;
	else
	{
		while(s[stringstart] == 'k' || s[stringstart] == 'K' || s[stringstart] == 'Q' || s[stringstart] == 'q')
		{
			if(s[stringstart] == 'k')
			{
				hashkey ^= bksckey;
				blackcastlingpermissions |= KINGSIDECASTLE;
			}
			else if(s[stringstart] == 'q')
			{
				hashkey ^= bqsckey;
				blackcastlingpermissions |= QUEENSIDECASTLE;
			}
			else if (s[stringstart] == 'K' )
			{
				hashkey ^= wksckey;
				whitecastlingpermissions |= KINGSIDECASTLE;
			}
			else
			{
				hashkey ^= wqsckey;
				whitecastlingpermissions |= QUEENSIDECASTLE;
			}
			stringstart++;
		}
		stringstart++;		
	}

	// enpassant key
	if(s[stringstart] == '-') stringstart += 2;
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


int Board::SquaresFill(const char s, int pos)
{
	switch(s)
	{
		case 'p' :
			squares[pos] = BLACK_PAWN;
			blackpawns ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_PAWN][pos];
			material -= PAWN_VALUE;
			break;
		case 'n' :
			squares[pos] = BLACK_KNIGHT;
			blackknights ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_KNIGHT][pos];
			material -= KNIGHT_VALUE;
			break;
		case 'b' :
			squares[pos] = BLACK_BISHOP;
			blackbishops ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_BISHOP][pos];
			material -= BISHOP_VALUE;
			break;

		case 'q' :
			squares[pos] = BLACK_QUEEN;
			blackqueen ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_QUEEN][pos];
			material -= QUEEN_VALUE;
			break;

		case 'r' : 
			squares[pos] = BLACK_ROOK;
			blackrooks ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_ROOK][pos];
			allpieces ^= BITSET[pos];
			material -= ROOK_VALUE;
			break;

		case 'k' :
			squares[pos] = BLACK_KING;
			blackking ^= BITSET[pos];
			blackpieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[BLACK_KING][pos];
			material -= KING_VALUE;
			break;	


		case 'P' :
			squares[pos] = WHITE_PAWN;
			whitepawns ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_PAWN][pos];
			material += PAWN_VALUE;
			break;
		case 'N' :
			squares[pos] = WHITE_KNIGHT;
			whiteknights ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_KNIGHT][pos];
			material += KNIGHT_VALUE;
			break;
		case 'B' :
			squares[pos] = WHITE_BISHOP;
			whitebishops ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_BISHOP][pos];
			material += BISHOP_VALUE;
			break;

		case 'Q' :
			squares[pos] = WHITE_QUEEN;
			whitequeen ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_QUEEN][pos];
			material += QUEEN_VALUE;
			break;

		case 'R' : 
			squares[pos] = WHITE_ROOK;
			whiterooks ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_ROOK][pos];
			allpieces ^= BITSET[pos];
			material += ROOK_VALUE;
			break;

		case 'K' :
			squares[pos] = WHITE_KING;
			whiteking ^= BITSET[pos];
			whitepieces ^= BITSET[pos];
			allpieces ^= BITSET[pos];
			hashkey ^= piecekeys[WHITE_KING][pos];
			material += KING_VALUE;
			break;	 
		default : 
			break;
	}
}

int Board::FillRank(int rankstart,  int stringstart, const string& s)
{

	int numwritten = 0;
	int i = 0;
	for(; i < 8, numwritten < 8; i++)
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
			SquaresFill(c, rankstart++);
			numwritten++;
		}
	}
	return stringstart + i + 1;

}



bool Board::IsRepetition()
{
	int count = 0;
	for(int i=0; i < gameindex; i++)
	{
		if(gameline[i].hashkey == hashkey) 
		{
			count++;
			if(count == 3)
				return true;
		}
	}
	return false;
}
