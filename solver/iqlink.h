#pragma once
#include <vector>
#include <map>
#include <algorithm>

// IQ LINK game representation - place 12 pieces with 36 PIN openings on a board with only 24 PINs
//    A    B    C    D    E    F    
// G    H    I    J    K    L
//    M    N    O    P    Q    R
// S    T    U    V    W    X

// Each Pin has its name A - X
enum class PinId : unsigned char { A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,_ };
const unsigned char IqLinkPinCount = (unsigned char)PinId::_;

// Direction & position definition
//       2       1
//        \     /
//         \   /
//   3 ---   6  --- 0
//         /   \ 
//        /	    \
//       4       5
enum class Direction : unsigned char { East, NorthEast, NorthWest, West, SouthWest, SouthEast, Center };
	
// PIN Neighbourhood map
//    A    B    C    D    E    F    
// G    H    I    J    K    L
//    M    N    O    P    Q    R
// S    T    U    V    W    X
const std::map< unsigned char, std::vector<unsigned char> > s_neighbourhood =
{	// PIN Name 			----- NEIGHBOUR IN EACH DIRECTION ----
	// PinId		East    NorthEast NorthWest   West    SouthWest SouthEast 
	{ PinId::A, { PinId::B, PinId::_, PinId::_, PinId::_, PinId::G, PinId::H } },
	{ PinId::B, { PinId::C, PinId::_, PinId::_, PinId::A, PinId::H, PinId::I } },
	{ PinId::C, { PinId::D, PinId::_, PinId::_, PinId::B, PinId::I, PinId::J } },
	{ PinId::D, { PinId::E, PinId::_, PinId::_, PinId::C, PinId::J, PinId::K } },
	{ PinId::E, { PinId::F, PinId::_, PinId::_, PinId::D, PinId::K, PinId::L } },
	{ PinId::F, { PinId::_, PinId::_, PinId::_, PinId::E, PinId::L, PinId::_ } },
	// ID        0      1      2      3      4      5      
	{ PinId::G, { PinId::H, PinId::A, PinId::_, PinId::_, PinId::_, PinId::M } },
	{ PinId::H, { PinId::I, PinId::B, PinId::A, PinId::G, PinId::M, PinId::N } },
	{ PinId::I, { PinId::J, PinId::C, PinId::B, PinId::H, PinId::N, PinId::O } },
	{ PinId::J, { PinId::K, PinId::D, PinId::C, PinId::I, PinId::O, PinId::P } },
	{ PinId::K, { PinId::L, PinId::E, PinId::D, PinId::J, PinId::P, PinId::Q } },
	{ PinId::L, { PinId::_, PinId::F, PinId::E, PinId::K, PinId::Q, PinId::R } },
	// ID        0      1      2      3      4      5      
	{ PinId::M, { PinId::N, PinId::H, PinId::G, PinId::_, PinId::S, PinId::T } },
	{ PinId::N, { PinId::O, PinId::I, PinId::H, PinId::M, PinId::T, PinId::U } },
	{ PinId::O, { PinId::P, PinId::J, PinId::I, PinId::N, PinId::U, PinId::V } },
	{ PinId::P, { PinId::Q, PinId::K, PinId::J, PinId::O, PinId::V, PinId::W } },
	{ PinId::Q, { PinId::R, PinId::L, PinId::K, PinId::P, PinId::W, PinId::X } },
	{ PinId::R, { PinId::_, PinId::_, PinId::L, PinId::Q, PinId::X, PinId::_ } },
	// ID        0      1      2      3      4      5      
	{ PinId::S, { PinId::T, PinId::M, PinId::_, PinId::_, PinId::_, PinId::_ } },
	{ PinId::T, { PinId::U, PinId::N, PinId::M, PinId::S, PinId::_, PinId::_ } },
	{ PinId::U, { PinId::V, PinId::O, PinId::N, PinId::T, PinId::_, PinId::_ } },
	{ PinId::V, { PinId::W, PinId::P, PinId::O, PinId::U, PinId::_, PinId::_ } },
	{ PinId::W, { PinId::X, PinId::Q, PinId::P, PinId::V, PinId::_, PinId::_ } },
	{ PinId::X, { PinId::_, PinId::R, PinId::Q, PinId::W, PinId::_, PinId::_ } },
};

// 12 Pieces colors 
enum class PieceColor : unsigned char
{
	NoColor = 0,
	LightBlue = 1,
	DarkBlue,
	Purple,
	Brown,
	DarkGreen,
	LightGreen,
	Pink,
	DarkRed,
	LightRed,
	Orange,
	Yellow
};

// 12 Piece definitions - each spans 3 PINS
// Take 1st PIN and place the piece in such a way that the 2nd PIN is in direction 0. 
// Mark direction of the piece between 1st and 2nd(3 bits) and 2nd and 3rd PIN (3bits) 
// and also mark all occupated positions at each PIN (3 x 7 bits) => 32 bits is fine 

//  -Piece-Color- -Pin1-positions-  -Pin2-positions-  -Pin3-positions- -Pin1-Pin2-Direction-  -Pin2-Pin3-Direction- 
//   b30 ... b27    b26 ... b20      b19 ...  b13        b12 ...  b6         b5 ... b3             b2 ... b0
inline unsigned long MakePiece(PieceColor color, unsigned char pin1, unsigned char pin2, unsigned char pin3, Direction dir12, Direction dir23)
{
	return	(((unsigned long)color) << 27)	| 
			(((unsigned long)pin1) << 20)	| 
			(((unsigned long)pin2) << 13)	| 
			(((unsigned long)pin3) << 6)	| 
			(((unsigned long)dir12) << 3)	| 
			(((unsigned long)dir23));
}

const unsigned long LightBluePiece = MakePiece(PieceColor::LightBlue, 0, 0, 0, Direction::East, Direction::East);
const unsigned long DarkBluePiece = MakePiece(PieceColor::DarkBlue, 0, 0, 0, Direction::East, Direction::East);
const unsigned long PurplePiece = MakePiece(PieceColor::Purple, 0, 0, 0, Direction::East, Direction::East);
const unsigned long BrownPiece = MakePiece(PieceColor::Brown, 0, 0, 0, Direction::East, Direction::East);
const unsigned long DarkGreenPiece = MakePiece(PieceColor::DarkGreen, 0, 0, 0, Direction::East, Direction::East);
const unsigned long LightGreenPiece = MakePiece(PieceColor::LightGreen, 0, 0, 0, Direction::East, Direction::East);
const unsigned long PinkPiece = MakePiece(PieceColor::Pink, 0, 0, 0, Direction::East, Direction::East);
const unsigned long DarkRedPiece = MakePiece(PieceColor::DarkRed, 0, 0, 0, Direction::East, Direction::East);
const unsigned long LightRedPiece = MakePiece(PieceColor::LightRed, 0, 0, 0, Direction::East, Direction::East);
const unsigned long OrangePiece = MakePiece(PieceColor::Orange, 0, 0, 0, Direction::East, Direction::East);
const unsigned long YellowPiece = MakePiece(PieceColor::Yellow, 0, 0, 0, Direction::East, Direction::East);

// Piece rotation is done via modular arithmetics mod 6. EVery piece at given PIN can be 
// placed at most in 6 different direction for every PIN and since it is 2D object it can be flipped.
// Totalling to 3 * 6 * 2 = 36 different positions at given PIN
const unsigned char IqLinkPiecePositions = 36;

// Occupancy vector - keeps state of empty/full positions for all PINs + piece id that occupies the PIN
// For every PIN we need to store info about each direction (0 .. 5) and also about the center of the PIN (6).
// These 7 values must keep id of the piece(12 = 4 bits) occupying it. We have 24 PINS => 24 * 7 * 4 bits.
// After all a vector of 24 unsigned longs(32 bits) will be fine.
//     -Pin-ID-    -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
//   b30 ... b27   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
inline unsigned long MakePin(PinId id, PieceColor colcenter, PieceColor colse, PieceColor colsw, PieceColor colw, PieceColor colnw, PieceColor colne, PieceColor cole )
{
	return	(((unsigned long)id) << 27)			|
			(((unsigned long)colcenter) << 24)	|
			(((unsigned long)colse) << 20)		|
			(((unsigned long)colsw) << 16)		|
			(((unsigned long)colw) << 12)		|
			(((unsigned long)colnw) << 8)		|
			(((unsigned long)colne) << 4)		|
			(((unsigned long)cole));
}
inline unsigned long MakeEmptyPin(PinId id)
{
	return MakePin(id, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor);
}

bool IsPinAvailable(unsigned long pin);
// Tests if the piece can be placed in given position and outputs new occupance if so
bool IsPlaceable(std::vector<unsigned long> occupance, std::vector<unsigned long>& new_occupance, unsigned char pin, unsigned short piece, unsigned char rotation);

