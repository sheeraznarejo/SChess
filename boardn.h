#include "defs.h"
typedef unsigned long long BITMAP;
class Board
{
	public:

		//below are the bitmaps for the individual pieces and the sides and allpieces
		int gameindex;
		BITMAP whitepawns;
		BITMAP whitebishops;
		BITMAP whiteknights;
		BITMAP whiterooks;
		BITMAP whitequeen;
		BITMAP whiteking;
		BITMAP blackpawns;
		BITMAP blackbishops;
		BITMAP blackknights;
		BITMAP blackrooks;
		BITMAP blackqueen;
		BITMAP blackking;
		BITMAP whitepieces;
		BITMAP blackpieces;
		BITMAP allpieces;

		//below are the 64bit keys for each piece-square, the enpassant keys, the castling keys, the keys for the sides
		BITMAP piecekeys[15][64]; // 12 different types of pieces, these are their hash keys
		BITMAP enpassantkeys[64]; // one enpassant key for each square on the board, even though most of the squares on the board can't be enpassant squares
		BITMAP wksckey;
		BITMAP wqsckey;
		BITMAP bksckey;
		BITMAP bqsckey;
		BITMAP whitesidekey;
		BITMAP blacksidekey;
		int squares[64];
		int enpassantsquare;
		int side;
		int material;
		int fiftymove;
		unsigned int whitecastlingpermissions; 
		unsigned int blackcastlingpermissions;
		BITMAP hashkey;
		unsigned int principalvariationarray[MAXPLY][MAXPLY];
		unsigned int principalvariationlength[MAXPLY];
		unsigned int movebuffer[MAXMOVEBUFFER];
		int movebuflen[MAXPLY];
		Record gameline[MAXGAMELINE];
		void InitBoard();
		void InitFromFen(const string& s);
		void DebugBoard();
		int FillBoardRank(int, int, const string& s);
		int SquaresFill(const char, int);
		int FillRank(int,int,const string& s);
		void InitHashKey();
		bool IsRepetition();
};




