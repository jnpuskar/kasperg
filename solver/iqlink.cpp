#include "stdafx.h"
#include "iqlink.h"
#include <set>
#include <vector>
#include <algorithm>

// At the given Pin Rotate piece and morph its pin flags and directions
// Values 0 - 5   = rotate around 1st piece-pin
//        6 - 11  = rotate around 2nd piece-pin 
//        12 - 17 = rotate around 3rd piece-pin
//		  18 - 23 = flip & rotate around 1st piece-pin
//        24 - 29 = flip & rotate around 2nd piece-pin
//	      30 - 35 = flip & rotate around 3rd piece-pin  
bool RotatePiece(unsigned long pin, unsigned long piece, unsigned char position, unsigned long& pin1, unsigned long& pin2, unsigned long& pin3)
{
	return false;
}

bool IsAvailable(unsigned long pin1, unsigned long pin2, unsigned long& pin12)
{
	//     -Pin-ID-    -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
	//   b31 ... b28   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
	unsigned long c16 = (pin1 & (0b1111 << 24)) >> 24;
	unsigned long c15 = (pin1 & (0b1111 << 20)) >> 20;
	unsigned long c14 = (pin1 & (0b1111 << 16)) >> 16;
	unsigned long c13 = (pin1 & (0b1111 << 12)) >> 12;
	unsigned long c12 = (pin1 & (0b1111 << 8)) >> 8;
	unsigned long c11 = (pin1 & (0b1111 << 4)) >> 4;
	unsigned long c10 = (pin1 & 0b1111);

	unsigned long c26 = (pin2 & (0b1111 << 24)) >> 24;
	unsigned long c25 = (pin2 & (0b1111 << 20)) >> 20;
	unsigned long c24 = (pin2 & (0b1111 << 16)) >> 16;
	unsigned long c23 = (pin2 & (0b1111 << 12)) >> 12;
	unsigned long c22 = (pin2 & (0b1111 << 8)) >> 8;
	unsigned long c21 = (pin2 & (0b1111 << 4)) >> 4;
	unsigned long c20 = (pin2 & 0b1111);

	// Positions of the 2 pins must not overlap 
	if (c20 != 0 && c10 != 0) return false;
	if (c21 != 0 && c11 != 0) return false;
	if (c22 != 0 && c12 != 0) return false;
	if (c23 != 0 && c13 != 0) return false;
	if (c24 != 0 && c14 != 0) return false;
	if (c25 != 0 && c15 != 0) return false;
	if (c26 != 0 && c16 != 0) return false;

	// Construct the new resulting pin occupied by the 2 
	unsigned long c126 = (c16 | c26) << 24;
	unsigned long c125 = (c15 | c25) << 20;
	unsigned long c124 = (c14 | c24) << 16;
	unsigned long c123 = (c13 | c23) << 12;
	unsigned long c122 = (c12 | c22) << 8;
	unsigned long c121 = (c11 | c21) << 4;
	unsigned long c120 = (c10 | c20);

	pin12 = c126 | c125 | c124 | c123 | c122 | c121 | c120;

	return true;
}
// Tests if the piece can be placed in given position and outputs new occupance if so
bool IsPlaceable(std::vector<unsigned long> occupance, std::vector<unsigned long>& new_occupance, unsigned long pin, unsigned long piece, unsigned char rotation)
{
	unsigned long pin1, pin2, pin3;
	if (!RotatePiece(pin, piece, rotation, pin1, pin2, pin3))
	{
		return false;
	}
	unsigned long newpin1, existingpin1 = FindExistingPin(pin1, occupance);
	unsigned long newpin2, existingpin2 = FindExistingPin(pin2, occupance);
	unsigned long newpin3, existingpin3 = FindExistingPin(pin3, occupance);

	if (!IsAvailable(existingpin1, pin1, newpin1) ||
		!IsAvailable(existingpin2, pin2, newpin2) ||
		!IsAvailable(existingpin3, pin3, newpin3))
	{
		return false;
	}

	new_occupance = occupance;

	if (!UpdatePins(newpin1, newpin2, newpin3, new_occupance))
	{
		return false;
	}
	
	return true;
}