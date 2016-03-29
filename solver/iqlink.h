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
const std::vector<Direction> s_directions = {	Direction::East, 
												Direction::NorthEast, 
												Direction::NorthWest, 
												Direction::West, 
												Direction::SouthWest, 
												Direction::SouthEast, 
												Direction::Center };
// PIN Neighbourhood map
//    A    B    C    D    E    F    
// G    H    I    J    K    L
//    M    N    O    P    Q    R
// S    T    U    V    W    X
const std::map< PinId, std::vector<PinId> > s_neighbourhood =
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
	LightBlue,
	DarkBlue,
	DarkPurple,
	LightPurple,
	DarkGreen,
	LightGreen,
	Green,
	LightPink,
	DarkPink,
	Red,
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

const unsigned long LightBluePiece = MakePiece(PieceColor::LightBlue, 0b00101111, 0b00111111, 0b01000100, Direction::East, Direction::SouthEast);
const unsigned long DarkBluePiece = MakePiece(PieceColor::DarkBlue, 0b01000001, 0b01001100, 0b00110111, Direction::East, Direction::NorthWest);
const unsigned long DarkPurplePiece = MakePiece(PieceColor::DarkPurple, 0b01000001, 0b00111111, 0b00111110, Direction::East, Direction::NorthEast);
const unsigned long LightPurplePiece = MakePiece(PieceColor::LightPurple, 0b01000001, 0b00111001, 0b01001000, Direction::East, Direction::East);
const unsigned long DarkGreenPiece = MakePiece(PieceColor::DarkGreen, 0b01000001, 0b01001100, 0b00111011, Direction::East, Direction::NorthWest);
const unsigned long LightGreenPiece = MakePiece(PieceColor::LightGreen, 0b01000001, 0b00111111, 0b00111011, Direction::East, Direction::East);
const unsigned long GreenPiece = MakePiece(PieceColor::Green, 0b01000001, 0b00111111, 0b00111101, Direction::East, Direction::East);
const unsigned long LightPinkPiece = MakePiece(PieceColor::LightPink, 0b00011111, 0b00111111, 0b01000100, Direction::East, Direction::SouthEast);
const unsigned long DarkPinkPiece = MakePiece(PieceColor::DarkPink, 0b01000001, 0b00111110, 0b01000100, Direction::East, Direction::SouthEast);
const unsigned long RedPiece = MakePiece(PieceColor::Red, 0b01000001, 0b00111101, 0b00111110, Direction::East, Direction::SouthWest);
const unsigned long OrangePiece = MakePiece(PieceColor::Orange, 0b01000001, 0b00111110, 0b01100000, Direction::East, Direction::NorthWest);
const unsigned long YellowPiece = MakePiece(PieceColor::Yellow, 0b00110111, 0b00111111, 0b00111011, Direction::East, Direction::NorthEast);

// At the given Pin Rotate piece and morph its pin flags and directions
// Values 0 - 5   = rotate piece around 1st piece-pin
//        6 - 11  = rotate around 2nd piece-pin 
//        12 - 17 = rotate around 3rd piece-pin
//		  18 - 23 = flip & rotate around 1st piece-pin
//        24 - 29 = flip & rotate around 2nd piece-pin
//	      30 - 35 = flip & rotate around 3rd piece-pin  
unsigned long RotatePiece(unsigned long piece, unsigned char position);

// Piece rotation is done via modular arithmetics mod 6. EVery piece at given PIN can be 
// placed at most in 6 different direction for every PIN and since it is 2D object it can be flipped.
// Totalling to 3 * 6 * 2 = 36 different positions at given PIN
const unsigned char IqLinkPiecePositions = 36;

// Occupancy vector - keeps state of empty/full positions for all PINs + piece id that occupies the PIN
// For every PIN we need to store info about each direction (0 .. 5) and also about the center of the PIN (6).
// These 7 values must keep id of the piece(12 = 4 bits) occupying it. We have 24 PINS => 24 * 7 * 4 bits.
// After all a vector of 24 unsigned longs(32 bits) will be fine.
//     -Pin-ID-    -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
//   b31 ... b28   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
inline unsigned long MakePin(PinId id, PieceColor colcenter, PieceColor colse, PieceColor colsw, PieceColor colw, PieceColor colnw, PieceColor colne, PieceColor cole )
{
	return	(((unsigned long)id) << 28)			|
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

// Checks whether we can insert a piece on this pin
bool IsPinAvailable(unsigned long pin);

// Tests if the piece can be placed in given position and outputs new occupance if so
bool IsPlaceable(std::vector<unsigned long> occupance, std::vector<unsigned long>& new_occupance, unsigned long pin, unsigned long piece, unsigned char rotation);

