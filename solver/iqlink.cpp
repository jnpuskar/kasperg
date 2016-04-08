#include "stdafx.h"
#include "iqlink.h"
#include <set>
#include <vector>
#include <algorithm>

// Rotate character val by n left-shift this is mod 6!!!
unsigned char RotateCounterClockWise(unsigned char val, unsigned char shift)
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
unsigned char RotateClockWise(unsigned char val, unsigned char shift)
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
unsigned char Flip(unsigned char val)
{
	// Detect center occupance bit 7 and do not rotate in such case
	if ((val & 0b01000000) != 0)
	{
		// No rotation for center
		return val;
	}

	// Bits 0 and 3 stay, bits 2 flips with 4, bit 1 flips with 5
	unsigned char bit2 = val & 0b00000100;
	unsigned char bit4 = val & 0b00010000;
	
	unsigned char bit1 = val & 0b00000010;
	unsigned char bit5 = val & 0b00100000;

	unsigned char bit0 = val & 0b00000001;
	unsigned char bit3 = val & 0b00001000;

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
	//  -Piece-Color- -Pin1-positions-  -Pin2-positions-  -Pin3-positions- -Pin1-Pin2-Direction-  -Pin2-Pin3-Direction- 
	//   b30 ... b27    b26 ... b20      b19 ...  b13        b12 ...  b6         b5 ... b3             b2 ... b0
	
	// Rotation is only done for each direction on bits 0 ... 5, bit 6 means center and does not rotate
	
	unsigned char rotation = position % 6;
	switch (position)
	{

	// 0 - 5   = rotate around 1st piece-pin
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:

		break;
	// 6 - 11  = rotate around 2nd piece-pin 
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		break;
	// 12 - 17 = rotate around 3rd piece - pin
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	// 18 - 23 = flip & rotate around 1st piece-pin
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		break;
	// 24 - 29 = flip & rotate around 2nd piece-pin
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
		break;
	// 30 - 35 = flip & rotate around 3rd piece-pin  
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
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
	//   Direction bits ignore the PinId
	const unsigned long long cDirectionBits = 0b1111111111111111111111111111; // bits 0 ... 27

	// Availability is determined by the bitwise NAND operation
	unsigned long long nand = ~((pin1 & cDirectionBits) & (pin2 & cDirectionBits));
		
	// Directions of the 2 pins must not overlap. When no overlap happens the NAND gives all 1s 
	if (nand != cDirectionBits)
	{
		return false;
	}
	
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