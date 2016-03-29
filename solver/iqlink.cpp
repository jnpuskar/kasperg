#include "stdafx.h"
#include "iqlink.h"
#include <set>
#include <vector>
#include <algorithm>

unsigned long RotatePiece(unsigned long piece, unsigned char position)
{
	return 0;
}

bool IsPinAvailable(unsigned long pin)
{
	//     -Pin-ID-    -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
	//   b31 ... b28   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
	unsigned long c6 = (pin & (0b1111 << 24)) >> 24;
	unsigned long c5 = (pin & (0b1111 << 20)) >> 20;
	unsigned long c4 = (pin & (0b1111 << 16)) >> 16;
	unsigned long c3 = (pin & (0b1111 << 12)) >> 12;
	unsigned long c2 = (pin & (0b1111 << 8))  >> 8;
	unsigned long c1 = (pin & (0b1111 << 4))  >> 4;
	unsigned long c0 = (pin & 0b1111);
	
	std::set<unsigned char> scolors;
	std::vector<unsigned char> vcolors;
	vcolors.push_back((unsigned char)c6);
	vcolors.push_back((unsigned char)c5);
	vcolors.push_back((unsigned char)c4);
	vcolors.push_back((unsigned char)c3);
	vcolors.push_back((unsigned char)c2);
	vcolors.push_back((unsigned char)c1);
	vcolors.push_back((unsigned char)c0);
	
	for( auto c : vcolors) 
	{
		if (c != (unsigned char)PieceColor::NoColor)
		{
			scolors.insert(c);
		}
	}

	// Max 2 pieces at 1 Pin
	if (scolors.size() > 1)
	{
		return false;
	}

	// In all other cases (empty Pin or only 1 Piece) we can use the Pin
	return true;
}
// Tests if the piece can be placed in given position and outputs new occupance if so
bool IsPlaceable(std::vector<unsigned long> occupance, std::vector<unsigned long>& new_occupance, unsigned long pin, unsigned long piece, unsigned char rotation)
{
	return false;
}