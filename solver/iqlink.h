/*
*
* Copyright (C) 2016,  Jan Puskar <jan.puskar@gmail.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#pragma once
#include <vector>
#include <map>
#include <string>
#include <algorithm>

// IQ LINK game representation - place 12 pieces with 36 PIN openings on a board with only 24 PINs
//    A    B    C    D    E    F    
//      G    H    I    J    K    L
//    M    N    O    P    Q    R
//      S    T    U    V    W    X

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
enum class Direction : unsigned char { East, NorthEast, NorthWest, West, SouthWest, SouthEast };
const std::vector<Direction> s_directions = {	Direction::East, 
												Direction::NorthEast, 
												Direction::NorthWest, 
												Direction::West, 
												Direction::SouthWest, 
												Direction::SouthEast
											};
// PIN Neighbourhood map
//    A    B    C    D    E    F    
//      G    H    I    J    K    L
//    M    N    O    P    Q    R
//      S    T    U    V    W    X
const std::map< PinId, std::vector<PinId> > s_neighbourhood =
{	// PIN Name 			----- NEIGHBOUR IN EACH DIRECTION ----
	// PinId		East    NorthEast NorthWest   West    SouthWest SouthEast 
	{ PinId::A,{ PinId::B, PinId::_, PinId::_, PinId::_, PinId::_, PinId::G } },
	{ PinId::B,{ PinId::C, PinId::_, PinId::_, PinId::A, PinId::G, PinId::H } },
	{ PinId::C,{ PinId::D, PinId::_, PinId::_, PinId::B, PinId::H, PinId::I } },
	{ PinId::D,{ PinId::E, PinId::_, PinId::_, PinId::C, PinId::I, PinId::J } },
	{ PinId::E,{ PinId::F, PinId::_, PinId::_, PinId::D, PinId::J, PinId::K } },
	{ PinId::F,{ PinId::_, PinId::_, PinId::_, PinId::E, PinId::K, PinId::L } },
	// PinId		East    NorthEast NorthWest   West    SouthWest SouthEast 
	{ PinId::G,{ PinId::H, PinId::B, PinId::A, PinId::_, PinId::M, PinId::N } },
	{ PinId::H,{ PinId::I, PinId::C, PinId::B, PinId::G, PinId::N, PinId::O } },
	{ PinId::I,{ PinId::J, PinId::D, PinId::C, PinId::H, PinId::O, PinId::P } },
	{ PinId::J,{ PinId::K, PinId::E, PinId::D, PinId::I, PinId::P, PinId::Q } },
	{ PinId::K,{ PinId::L, PinId::F, PinId::E, PinId::J, PinId::Q, PinId::R } },
	{ PinId::L,{ PinId::_, PinId::_, PinId::F, PinId::K, PinId::R, PinId::_ } },
	// PinId		East    NorthEast NorthWest   West    SouthWest SouthEast       
	{ PinId::M,{ PinId::N, PinId::G, PinId::_, PinId::_, PinId::_, PinId::S } },
	{ PinId::N,{ PinId::O, PinId::H, PinId::G, PinId::M, PinId::S, PinId::T } },
	{ PinId::O,{ PinId::P, PinId::I, PinId::H, PinId::N, PinId::T, PinId::U } },
	{ PinId::P,{ PinId::Q, PinId::J, PinId::I, PinId::O, PinId::U, PinId::V } },
	{ PinId::Q,{ PinId::R, PinId::K, PinId::J, PinId::P, PinId::V, PinId::W } },
	{ PinId::R,{ PinId::_, PinId::L, PinId::K, PinId::Q, PinId::W, PinId::X } },
	// PinId		East    NorthEast NorthWest   West    SouthWest SouthEast 
	{ PinId::S,{ PinId::T, PinId::N, PinId::M, PinId::_, PinId::_, PinId::_ } },
	{ PinId::T,{ PinId::U, PinId::O, PinId::N, PinId::S, PinId::_, PinId::_ } },
	{ PinId::U,{ PinId::V, PinId::P, PinId::O, PinId::T, PinId::_, PinId::_ } },
	{ PinId::V,{ PinId::W, PinId::Q, PinId::P, PinId::U, PinId::_, PinId::_ } },
	{ PinId::W,{ PinId::X, PinId::R, PinId::Q, PinId::V, PinId::_, PinId::_ } },
	{ PinId::X,{ PinId::_, PinId::_, PinId::R, PinId::W, PinId::_, PinId::_ } },
};

inline bool FindPin(PinId pinfrom, Direction dir, PinId& pinto)
{
	if(s_neighbourhood.count(pinfrom) > 0)
	{
		size_t offset = (size_t)dir % 6;
		if (offset < s_neighbourhood.find(pinfrom)->second.size())
		{
			pinto = s_neighbourhood.find(pinfrom)->second.at(offset);
			if (pinto != PinId::_)
			{
				return true;
			}
		}		
	}
	
	return false;	
}

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

const std::vector<std::wstring> s_colornames = { L"NoColor",
											L"LightBlue",
											L"DarkBlue",
											L"DarkPurple",
											L"LightPurple",
											L"DarkGreen",
											L"LightGreen",
											L"Green",
											L"LightPink",
											L"DarkPink",
											L"Red",
											L"Orange",
											L"Yellow" };

// COLORS
//0 = Black       8 = Gray
//1 = Blue        9 = Light Blue
//2 = Green       A = Light Green
//3 = Aqua        B = Light Aqua
//4 = Red         C = Light Red
//5 = Purple      D = Light Purple
//6 = Yellow      E = Light Yellow
//7 = White       F = Bright White
const std::map<PieceColor, unsigned char > s_piececolors = { { PieceColor::NoColor, 0},
{ PieceColor::LightBlue, 0x09 },
{ PieceColor::DarkBlue, 0x01},
{ PieceColor::DarkPurple, 0x05},
{ PieceColor::LightPurple, 0x0D},
{ PieceColor::DarkGreen, 0x08},
{ PieceColor::LightGreen, 0x0A },
{ PieceColor::Green, 0x02},
{ PieceColor::LightPink, 0x0B},
{ PieceColor::DarkPink, 0x03},
{ PieceColor::Red, 0x04},
{ PieceColor::Orange, 0x0C},
{ PieceColor::Yellow, 0x06} };

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

inline size_t GetPieceFullPins(const unsigned long& piece)
{
	size_t cnt = 0;
	unsigned long mask = 0b00111111;
	if ((piece & (mask << 20)) == (mask << 20)) cnt++;
	if ((piece & (mask << 13)) == (mask << 13)) cnt++;
	if ((piece & (mask << 6)) == (mask << 6)) cnt++;
	return cnt;
}
inline size_t GetPieceCenters(const unsigned long& piece)
{
	size_t cnt = 0;
	unsigned long mask = 0b01000000;
	if ((piece & (mask << 20)) == (mask << 20)) cnt++;
	if ((piece & (mask << 13)) == (mask << 13)) cnt++;
	if ((piece & (mask << 6)) == (mask << 6)) cnt++;
	return cnt;
}

const unsigned long LightBluePiece = MakePiece(PieceColor::LightBlue, 0b00101111, 0b00111111, 0b01000100, Direction::East, Direction::SouthEast);
const unsigned long DarkBluePiece = MakePiece(PieceColor::DarkBlue, 0b01000001, 0b01001100, 0b00110111, Direction::East, Direction::NorthWest);
const unsigned long DarkPurplePiece = MakePiece(PieceColor::DarkPurple, 0b01000001, 0b00111111, 0b00011111, Direction::East, Direction::NorthEast);
const unsigned long LightPurplePiece = MakePiece(PieceColor::LightPurple, 0b01000001, 0b00111001, 0b01001000, Direction::East, Direction::East);
const unsigned long DarkGreenPiece = MakePiece(PieceColor::DarkGreen, 0b01000001, 0b01001100, 0b00111011, Direction::East, Direction::NorthWest);
const unsigned long LightGreenPiece = MakePiece(PieceColor::LightGreen, 0b01000001, 0b00111111, 0b00111011, Direction::East, Direction::East);
const unsigned long GreenPiece = MakePiece(PieceColor::Green, 0b01000001, 0b00111111, 0b00111101, Direction::East, Direction::East);
const unsigned long LightPinkPiece = MakePiece(PieceColor::LightPink, 0b00011111, 0b00111111, 0b01000100, Direction::East, Direction::SouthEast);
const unsigned long DarkPinkPiece = MakePiece(PieceColor::DarkPink, 0b01000001, 0b00111110, 0b01000100, Direction::East, Direction::SouthEast);
const unsigned long RedPiece = MakePiece(PieceColor::Red, 0b01000001, 0b00111101, 0b00111110, Direction::East, Direction::SouthWest);
const unsigned long OrangePiece = MakePiece(PieceColor::Orange, 0b01000001, 0b00111110, 0b01100000, Direction::East, Direction::NorthWest);
const unsigned long YellowPiece = MakePiece(PieceColor::Yellow, 0b00110111, 0b00111111, 0b00111011, Direction::East, Direction::NorthEast);

inline std::wstring PieceName(unsigned long piece) {
	return s_colornames.at((piece >> 27));
}
// At the given Pin Rotate piece and morph its pin flags and directions
// Values 0 - 5   = rotate around 1st piece-pin
//        6 - 11  = flip & rotate around 1st piece-pin
// Other possibilities not used:			
//		rotate around 2nd piece-pin --> not used
//		rotate around 3rd piece-pin --> not used	
//		flip & rotate around 2nd piece-pin --> not used
//		flip & rotate around 3rd piece-pin  --> not used
bool RotatePiece(unsigned long long pin, unsigned long piece, unsigned char position, unsigned long long& pin1, unsigned long long& pin2, unsigned long long& pin3);

// Piece rotation is done via modular arithmetics mod 6. EVery piece at given PIN can be 
// placed at most in 6 different direction for every PIN and since it is 2D object it can be flipped.
const unsigned char IqLinkPiecePositions = 12;

// Occupancy vector - keeps state of empty/full positions for all PINs
// For every PIN we need to store info about each direction (0 .. 5) and also about the center of the PIN (6).
// These 7 values must keep id of the piece(24 = 5 bits) occupying it. We have 24 PINS each 33 bits : 5 + 7 * 4.
// After all a vector of 24 unsigned long longs(64 bits) will be fine.
//     -Pin-ID-    -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
//   b32 ... b28   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
inline unsigned long long MakePin(PinId id, PieceColor colcenter, PieceColor colse, PieceColor colsw, PieceColor colw, PieceColor colnw, PieceColor colne, PieceColor cole )
{
	return	(((unsigned long long)id) << 28)		|
			(((unsigned long long)colcenter) << 24) |
			(((unsigned long long)colse) << 20)		|
			(((unsigned long long)colsw) << 16)		|
			(((unsigned long long)colw) << 12)		|
			(((unsigned long long)colnw) << 8)		|
			(((unsigned long long)colne) << 4)		|
			(((unsigned long long)cole));
}
inline unsigned long long MakeEmptyPin(PinId id)
{
	return MakePin(id, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor, PieceColor::NoColor);
}
inline wchar_t PinIdName(unsigned long long pin) {
	return L'A' + (wchar_t)(pin >> 28);
}
inline unsigned long long MakePinWithPiece(PinId id, PieceColor color, unsigned char PiecePin)
{
	return MakePin(id, 
		((PiecePin & 0b01000000) == 0) ? PieceColor::NoColor : color, // center
		((PiecePin & 0b00100000) == 0) ? PieceColor::NoColor : color, // SE
		((PiecePin & 0b00010000) == 0) ? PieceColor::NoColor : color, // SW
		((PiecePin & 0b00001000) == 0) ? PieceColor::NoColor : color, // W
		((PiecePin & 0b00000100) == 0) ? PieceColor::NoColor : color, // NW
		((PiecePin & 0b00000010) == 0) ? PieceColor::NoColor : color, // NE
		((PiecePin & 0b00000001) == 0) ? PieceColor::NoColor : color);// E
}

// Checks whether we can insert a piece on this pin
bool IsAvailable(unsigned long long pin2, const std::vector<unsigned long long>& occupance, unsigned long long& pin12);
void UpdatePin(unsigned long long pin, std::vector<unsigned long long>& occupance);
bool PinIsAdjacent(unsigned long long pin, const std::vector<unsigned long long>& occupance, const size_t& level);
bool IsPinEmpty(unsigned long long pin);
bool PinFull(unsigned long long pin);
bool PinCenterFree(unsigned long long pin);
bool GetExistingPin(unsigned long long pin, const std::vector<unsigned long long>& occupance, unsigned long long& pin_existing);
// Tests if the piece can be placed in given position and outputs new occupance if so
bool IsPlaceable(const std::vector<unsigned long long>& occupance, std::vector<unsigned long long>& new_occupance, unsigned long long pin, unsigned long piece, unsigned char rotation);

// Create initial setup of the game
bool SetupGame(std::vector<unsigned long long>& occupance, std::vector<unsigned long>& pieces, unsigned long index);


// Compressed occupance - 3 64 bit values to keep 7 bits per PIN
struct CIqLinkOcc
{
	CIqLinkOcc() : ah(0), ip(0), qx(0){}
	unsigned long long ah;
	unsigned long long ip;
	unsigned long long qx;
	const bool operator < (const CIqLinkOcc &rhs) const 
	{ 
		if (ah < rhs.ah)
			return true;
		if (ip < rhs.ip)
			return true;
		if (qx < rhs.qx)
			return true;
		return false;
	}
	const bool operator == (const CIqLinkOcc &rhs) const { return  ah == rhs.ah && ip == rhs.ip && qx == rhs.qx; }
	CIqLinkOcc operator ^ (const CIqLinkOcc& rhs) { CIqLinkOcc res; res.ah = ah ^ rhs.ah; res.ip = ip ^ rhs.ip; res.qx = qx ^ rhs.qx; return res; }
	CIqLinkOcc operator & (const CIqLinkOcc& rhs) { CIqLinkOcc res; res.ah = ah & rhs.ah; res.ip = ip & rhs.ip; res.qx = qx & rhs.qx; return res; }
	CIqLinkOcc operator | (const CIqLinkOcc& rhs) { CIqLinkOcc res; res.ah = ah | rhs.ah; res.ip = ip | rhs.ip; res.qx = qx | rhs.qx; return res; }
	CIqLinkOcc operator ~ () { CIqLinkOcc res; res.ah = ~ah; res.ip = ~ip; res.qx = ~qx; return res; }

	bool IsNull() { return ah == 0 && ip == 0 && qx == 0; }
	bool Discrete(const CIqLinkOcc& o)
	{
		if (((ah & o.ah) == 0) && ((ip & o.ip) == 0) && ((qx & o.qx) == 0))
		{
			return true;
		}
		return false;
	}
};

CIqLinkOcc CompressOcc(const std::vector<unsigned long long>& occupance);
void DecompressOcc(const CIqLinkOcc& o, PieceColor c, std::vector<unsigned long long>& occupance);