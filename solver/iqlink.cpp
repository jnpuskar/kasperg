#include "stdafx.h"
#include "iqlink.h"
#include <set>
#include <vector>
#include <algorithm>

// Rotate character val by n left-shift this is mod 6!!!
unsigned long long RotateCounterClockWise(unsigned long long val, unsigned char shift)
{
	// Detect center occupance bit 7 and do not rotate in such case
	if ((val & 0b01000000) != 0)
	{
		// No rotation for center
		return val;
	}
	unsigned char shiftmod = shift % 6;
	return (val << shiftmod) | (val >> ( 6 - shiftmod));
}

// Rotate character val by n right-shift this is mod 6!!!
unsigned long long RotateClockWise(unsigned long long val, unsigned char shift)
{
	// Detect center occupance bit 7 and do not rotate in such case
	if ((val & 0b01000000) != 0)
	{
		// No rotation for center
		return val;
	}
	unsigned char shiftmod = shift % 6;
	return (val >> shiftmod) | (val << (6 - shiftmod));
}
unsigned long long Flip(unsigned long long val)
{
	// Detect center occupance bit 7 and do not rotate in such case
	if ((val & 0b01000000) != 0)
	{
		// No rotation for center
		return val;
	}

	// Bits 0 and 3 stay, bits 2 flips with 4, bit 1 flips with 5
	unsigned long long bit2 = val & 0b00000100;
	unsigned long long bit4 = val & 0b00010000;
	
	unsigned long long bit1 = val & 0b00000010;
	unsigned long long bit5 = val & 0b00100000;

	unsigned long long bit0 = val & 0b00000001;
	unsigned long long bit3 = val & 0b00001000;

	return (bit1 << 4) | (bit2 << 2) | bit3 | (bit4 >> 2) | (bit5 >> 4) | bit0;
}
Direction RotateCounterClockWise(Direction dir, unsigned char shift)
{
	unsigned char cdir = (unsigned char)dir;
	unsigned char index = (cdir + shift) % 6;

	return s_directions.at(index);
}
Direction RotateClockWise(Direction dir, unsigned char shift)
{
	unsigned char cdir = (unsigned char)dir;
	unsigned char modshift = shift % 6;
	unsigned char index = (cdir + (6 - modshift)) % 6;

	return s_directions.at(index);
}
Direction Inverse(Direction dir)
{
	unsigned char d = (unsigned char)dir;
	return (Direction)(d % 6);
}
Direction Flip(Direction dir)
{
	switch (dir)
	{
	case Direction::East:
	case Direction::West:
		return dir;
	case Direction::NorthEast:
		return Direction::SouthEast;
	case Direction::NorthWest:
		return Direction::SouthWest;
	case Direction::SouthEast:
		return Direction::NorthEast;
	case Direction::SouthWest:
		return Direction::NorthWest;
	}
	return Direction::East;
}

// At the given Pin Rotate piece and morph its pin flags and directions
// Values 0 - 5   = rotate around 1st piece-pin
//        6 - 11  = rotate around 2nd piece-pin 
//        12 - 17 = rotate around 3rd piece-pin
//		  18 - 23 = flip & rotate around 1st piece-pin
//        24 - 29 = flip & rotate around 2nd piece-pin
//	      30 - 35 = flip & rotate around 3rd piece-pin  
bool RotatePiece(unsigned long long pin, unsigned long piece, unsigned char position, unsigned long long& pin1, unsigned long long& pin2, unsigned long long& pin3)
{
	// Piece
	// -Piece-Color- -Pin1-positions-  -Pin2-positions-  -Pin3-positions- -Pin1-Pin2-Direction-  -Pin2-Pin3-Direction- 
	// b30 ... b27    b26 ... b20      b19 ...  b13        b12 ...  b6         b5 ... b3             b2 ... b0
	PieceColor color				= (PieceColor)((piece & 0b01111000000000000000000000000000) >> 27);
	// Pin1  
	unsigned long long ullpiecepin1	= (piece & 0b00000111111100000000000000000000) >> 20;
	// Pin2  
	unsigned long long ullpiecepin2	= (piece & 0b00000000000011111110000000000000) >> 13;
	// Pin3  
	unsigned long long ullpiecepin3	= (piece & 0b00000000000000000001111111000000) >> 6;
	// Dir12  
	Direction dir12					= (Direction)((piece & 0b00000000000000000000000000111000) >> 3);
	// Dir23
	Direction dir23					= (Direction)(piece & 0b00000000000000000000000000000111);
	
	// Pin
	// -Pin-ID-    -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
	// b32 ... b28   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
	
	PinId pin1Id = (PinId)(pin >> 28);
	PinId pin2Id, pin3Id;

	// Rotation is only done for each direction on bits 0 ... 5, bit 6 means center and does not rotate
	unsigned char rotation = position % 6;

	// Flip the piece for position 18 and up
	if (position >= 18)
	{
		ullpiecepin1 = Flip(ullpiecepin1);
		ullpiecepin2 = Flip(ullpiecepin2);
		ullpiecepin3 = Flip(ullpiecepin3);
		dir12 = Flip(dir12);
		dir23 = Flip(dir23);
	}

	// Rotate the piece accordingly
	ullpiecepin1 = RotateCounterClockWise(ullpiecepin1, rotation);
	ullpiecepin2 = RotateCounterClockWise(ullpiecepin2, rotation);
	ullpiecepin3 = RotateCounterClockWise(ullpiecepin3, rotation);

	// Pin1 with Id is 
	switch (position)
	{	
	case 0: // 0 - 5  rotate around 1st piece-pin
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 18: // 18 - 23 = flip & rotate around 1st piece-pin
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		dir12 = RotateCounterClockWise(dir12, rotation);
		dir23 = RotateCounterClockWise(dir23, rotation);
		// Find new pins
		if(!FindPin(pin1Id, dir12, pin2Id))
		{
			return false;
		}
		if (!FindPin(pin2Id, dir23, pin3Id))
		{
			return false;
		}
		// Costruct new Pins occupied by this piece in the given position
		pin1 = MakePinWithPiece(pin1Id, color, (unsigned char)ullpiecepin1);
		pin2 = MakePinWithPiece(pin2Id, color, (unsigned char)ullpiecepin2);
		pin3 = MakePinWithPiece(pin3Id, color, (unsigned char)ullpiecepin3);
		break;

	case 6:// 6 - 11  = rotate around 2nd piece-pin 
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:		
	case 24:// 24 - 29 = flip & rotate around 2nd piece-pin
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:		
		dir12 = RotateCounterClockWise(Inverse(dir12), rotation); // pin1Id is the middle pin
		dir23 = RotateCounterClockWise(dir23, rotation);
		// Find new pins - pin1Id is in the middle
		if (!FindPin(pin1Id, dir12, pin2Id))
		{
			return false;
		}
		if (!FindPin(pin1Id, dir23, pin3Id))
		{
			return false;
		}
		// Costruct new Pins occupied by this piece in the given position
		pin1 = MakePinWithPiece(pin1Id, color, (unsigned char)ullpiecepin2);
		pin2 = MakePinWithPiece(pin2Id, color, (unsigned char)ullpiecepin1);
		pin3 = MakePinWithPiece(pin3Id, color, (unsigned char)ullpiecepin3);
		break;
	case 12:// 12 - 17 = rotate around 3rd piece - pin
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:		
	case 30:// 30 - 35 = flip & rotate around 3rd piece-pin  
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:		
		dir12 = RotateCounterClockWise(Inverse(dir12), rotation); // pin1Id is the most right one
		dir23 = RotateCounterClockWise(Inverse(dir23), rotation);
		// Find new pins - pin1Id is the most right one
		if (!FindPin(pin1Id, dir23, pin2Id))
		{
			return false;
		}
		if (!FindPin(pin2Id, dir12, pin3Id))
		{
			return false;
		}
		// Costruct new Pins occupied by this piece in the given position
		pin1 = MakePinWithPiece(pin1Id, color, (unsigned char)ullpiecepin3);
		pin2 = MakePinWithPiece(pin2Id, color, (unsigned char)ullpiecepin2);
		pin3 = MakePinWithPiece(pin3Id, color, (unsigned char)ullpiecepin1);		
		break;

	default:
		return false;
	}
	return true;
}

bool IsAvailable(unsigned long long pin2, const std::vector<unsigned long long>& occupance, unsigned long long& pin12)
{
	// Find existing pin1 in occupance that has the same PinId as pin2
	auto it = std::find_if(occupance.begin(), occupance.end(), [=](auto occpin) { return  ((pin2 & (0b11111 << 28)) == (occpin & (0b11111 << 28))); });
	if (it != occupance.end())
	{
		return false;
	}
	unsigned long long pin1 = *it;

	//   -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
	//   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
	unsigned long long c16 = (pin1 & (0b1111 << 24)) >> 24;
	unsigned long long c15 = (pin1 & (0b1111 << 20)) >> 20;
	unsigned long long c14 = (pin1 & (0b1111 << 16)) >> 16;
	unsigned long long c13 = (pin1 & (0b1111 << 12)) >> 12;
	unsigned long long c12 = (pin1 & (0b1111 << 8)) >> 8;
	unsigned long long c11 = (pin1 & (0b1111 << 4)) >> 4;
	unsigned long long c10 = (pin1 & 0b1111);
				 
	unsigned long long c26 = (pin2 & (0b1111 << 24)) >> 24;
	unsigned long long c25 = (pin2 & (0b1111 << 20)) >> 20;
	unsigned long long c24 = (pin2 & (0b1111 << 16)) >> 16;
	unsigned long long c23 = (pin2 & (0b1111 << 12)) >> 12;
	unsigned long long c22 = (pin2 & (0b1111 << 8)) >> 8;
	unsigned long long c21 = (pin2 & (0b1111 << 4)) >> 4;
	unsigned long long c20 = (pin2 & 0b1111);

	// Positions of the 2 pins must not overlap 
	if (c20 != 0 && c10 != 0) return false;
	if (c21 != 0 && c11 != 0) return false;
	if (c22 != 0 && c12 != 0) return false;
	if (c23 != 0 && c13 != 0) return false;
	if (c24 != 0 && c14 != 0) return false;
	if (c25 != 0 && c15 != 0) return false;
	if (c26 != 0 && c16 != 0) return false;
	
	// Construct the new resulting pin by bitwise OR
	pin12 = pin1 | pin2;

	return true;
}
void UpdatePin(unsigned long long pin, std::vector<unsigned long long>& occupance)
{
	// Find the pin with same id and make its value pin
	std::transform(occupance.begin(), occupance.end(), occupance.begin(), [=](auto ull)	{ return ((pin & (0b11111 << 28)) == (ull & (0b11111 << 28))) ? pin : ull; });
}
// Tests if the piece can be placed in given position and outputs new occupance if so
bool IsPlaceable(const std::vector<unsigned long long>& occupance, std::vector<unsigned long long>& new_occupance, unsigned long long pin, unsigned long piece, unsigned char rotation)
{
	// Rotate the pice to a given position. Get the possibly occupied pins with occupance masks
	unsigned long long pin1, pin2, pin3;
	if (!RotatePiece(pin, piece, rotation, pin1, pin2, pin3))
	{
		return false;
	}

	// Check possible placement and get the new pins
	unsigned long long newpin1, newpin2, newpin3;
	if (!(IsAvailable(pin1, occupance, newpin1) && IsAvailable(pin2, occupance, newpin2) && IsAvailable(pin3, occupance, newpin3)))
	{
		return false;
	}

	// Initialize new occupance with the previous one
	new_occupance = occupance;

	// Update the new occupance
	UpdatePin(newpin1, new_occupance);
	UpdatePin(newpin2, new_occupance);
	UpdatePin(newpin3, new_occupance);
		
	return true;
}