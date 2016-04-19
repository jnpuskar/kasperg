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

#include "stdafx.h"
#include "iqlink.h"
#include "presenter.h"

#include <vector>
#include <algorithm>

// Rotate character val by n left-shift this is mod 6!!!
unsigned long long RotateCounterClockWise(unsigned long long val, unsigned char shift)
{
	unsigned char shiftmod = shift % 6;
	unsigned long long temp = val & 0b00111111;
	return (val & 0b01000000) | (0b00111111 & (temp << shiftmod) | (temp >> ( 6 - shiftmod)));
}

// Rotate character val by n right-shift this is mod 6!!!
unsigned long long RotateClockWise(unsigned long long val, unsigned char shift)
{
	unsigned char shiftmod = shift % 6;
	unsigned long long temp = val & 0b00111111;
	return (val & 0b01000000) | (0b00111111 & (temp >> shiftmod) | (temp << (6 - shiftmod)));
}
unsigned long long Flip(unsigned long long val)
{
	// Detect center occupance 
	unsigned long long bit6 = (val & 0b01000000);
	
	// Bits 0 and 3 stay, bits 2 flips with 4, bit 1 flips with 5
	unsigned long long bit2 = val & 0b00000100;
	unsigned long long bit4 = val & 0b00010000;
	
	unsigned long long bit1 = val & 0b00000010;
	unsigned long long bit5 = val & 0b00100000;

	unsigned long long bit0 = val & 0b00000001;
	unsigned long long bit3 = val & 0b00001000;

	return bit6 | (bit1 << 4) | (bit2 << 2) | bit3 | (bit4 >> 2) | (bit5 >> 4) | bit0;
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
	return (Direction)((d + 3) % 6);
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
bool RotatePiece(unsigned long long pin, unsigned long piece, unsigned char position, unsigned long long& pin1, unsigned long long& pin2, unsigned long long& pin3)
{
	// Piece
	// -Piece-Color- -Pin1-positions-  -Pin2-positions-  -Pin3-positions- -Pin1-Pin2-Direction-  -Pin2-Pin3-Direction- 
	// b30 ... b27    b26 ... b20      b19 ...  b13        b12 ...  b6         b5 ... b3             b2 ... b0
	PieceColor color = (PieceColor)((piece & 0b01111000000000000000000000000000) >> 27);
	// Pin1  
	unsigned long long ullpiecepin1	= (piece & 0b00000111111100000000000000000000) >> 20;
	// Pin2  
	unsigned long long ullpiecepin2	= (piece & 0b00000000000011111110000000000000) >> 13;
	// Pin3  
	unsigned long long ullpiecepin3	= (piece & 0b00000000000000000001111111000000) >> 6;
	// Dir12  
	Direction dir12 = (Direction)((piece & 0b00000000000000000000000000111000) >> 3);
	// Dir23
	Direction dir23 = (Direction)(piece & 0b00000000000000000000000000000111);
	
	// Pin
	// -Pin-ID-    -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
	// b32 ... b28   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
	
	PinId pin1Id = (PinId)(pin >> 28);
	PinId pin2Id, pin3Id;

	// Rotation is only done for each direction on bits 0 ... 5, bit 6 means center and does not rotate
	unsigned char rotation = position % 6;

	// Flip the piece for position 18 and up
	if (position >= 6)
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

	dir12 = RotateCounterClockWise(dir12, rotation);
	dir23 = RotateCounterClockWise(dir23, rotation);
	// Find new pins
	if (!FindPin(pin1Id, dir12, pin2Id))
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

	return true;
}

bool IsAvailable(unsigned long long pin2, const std::vector<unsigned long long>& occupance, unsigned long long& pin12)
{
	// Find existing pin1 in occupance that has the same PinId as pin2
	unsigned long long id2 = (pin2 & (0b11111 << 28)) >> 28;
	// Find the pin with same id and make its value pin
	if ((PinId)id2 >= PinId::_)
	{
		return false;
	}

	unsigned long long pin1 = occupance.at(id2);

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
	unsigned long long id = (pin & (0b11111 << 28)) >> 28;
	// Find the pin with same id and make its value pin
	if ((PinId)id < PinId::_)
	{
		occupance.at(id) = pin;
	}	
}

bool IsPinEmpty(unsigned long long pin)
{
	//   -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
	//   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
	unsigned long long c6 = (pin & (0b1111 << 24)) >> 24;
	unsigned long long c5 = (pin & (0b1111 << 20)) >> 20;
	unsigned long long c4 = (pin & (0b1111 << 16)) >> 16;
	unsigned long long c3 = (pin & (0b1111 << 12)) >> 12;
	unsigned long long c2 = (pin & (0b1111 << 8)) >> 8;
	unsigned long long c1 = (pin & (0b1111 << 4)) >> 4;
	unsigned long long c0 = (pin & 0b1111);
	return c6 == 0 && c5 == 0 && c4 == 0 && c3 == 0 && c2 == 0 && c1 == 0 && c0 == 0;
}

bool PinFull(unsigned long long pin)
{
	//   -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
	//   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
	unsigned long long c6 = (pin & (0b1111 << 24)) >> 24;
	unsigned long long c5 = (pin & (0b1111 << 20)) >> 20;
	unsigned long long c4 = (pin & (0b1111 << 16)) >> 16;
	unsigned long long c3 = (pin & (0b1111 << 12)) >> 12;
	unsigned long long c2 = (pin & (0b1111 << 8)) >> 8;
	unsigned long long c1 = (pin & (0b1111 << 4)) >> 4;
	unsigned long long c0 = (pin & 0b1111);
	
	// We have to be fast. So no STL container creation. No algorithms. Test if we have 2 non-empty colors present
	return (c6 != 0 && ((c5 != 0 && c5 != c6) || (c4 != 0 && c4 != c6) || (c3 != 0 && c3 != c6) || (c2 != 0 && c2 != c6) || (c1 != 0 && c1 != c6) || (c0 != 0 && c0 != c6))) ||
		(c5 != 0 && ((c4 != 0 && c4 != c5) || (c3 != 0 && c3 != c5) || (c2 != 0 && c2 != c5) || (c1 != 0 && c1 != c5) || (c0 != 0 && c0 != c5))) ||
		(c4 != 0 && ((c3 != 0 && c3 != c4) || (c2 != 0 && c2 != c4) || (c1 != 0 && c1 != c4) || (c0 != 0 && c0 != c4))) ||
		(c3 != 0 && ((c2 != 0 && c2 != c3) || (c1 != 0 && c1 != c3) || (c0 != 0 && c0 != c3))) ||
		(c2 != 0 && ((c1 != 0 && c1 != c2) || (c0 != 0 && c0 != c2))) ||
		(c1 != 0 && (c0 != 0 && c0 != c1));	
}

unsigned long long GetPin(PinId id, const std::vector<unsigned long long>& occupance)
{
	return occupance.at((size_t)id);	
}

bool PinIsAdjacent(unsigned long long pin, const std::vector<unsigned long long>& occupance, const size_t& level)
{
	UNREFERENCED_PARAMETER(occupance);

	// NOTE:
	// This function is too simple. It should check the occupance vector and decide whether it makes sense 
	// to consider this pin for placemnt of the piece.

	// This is the begining of the run. Only place pieces around 1st pin A
	if (level == 12)
	{
		return pin == MakeEmptyPin(PinId::A) || 
			pin == MakeEmptyPin(PinId::B) || 
			pin == MakeEmptyPin(PinId::C) || 
			pin == MakeEmptyPin(PinId::G) || 
			pin == MakeEmptyPin(PinId::H) || 
			pin == MakeEmptyPin(PinId::N) || 
			pin == MakeEmptyPin(PinId::S) || 
			pin == MakeEmptyPin(PinId::M);
	}
	
	// Pin is full.
	if (PinFull(pin))
	{
		return false;
	}

	return true;
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
bool PlacePiece(std::vector<unsigned long long>& occupance, unsigned long long pin, unsigned long piece, unsigned char rotation)
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

	// Update the new occupance
	UpdatePin(newpin1, occupance);
	UpdatePin(newpin2, occupance);
	UpdatePin(newpin3, occupance);

	return true;
}
bool SetupGame(std::vector<unsigned long long>& occupance, std::vector<unsigned long>& pieces, unsigned long index)
{
	IqLinkPresenter pr;

	std::vector<unsigned long> all_pieces = { LightBluePiece,	DarkBluePiece, DarkPurplePiece,	LightPurplePiece, DarkGreenPiece,LightGreenPiece,
		GreenPiece,	LightPinkPiece,DarkPinkPiece,RedPiece,OrangePiece,YellowPiece };

	std::vector<unsigned long long> temp_occupance = { MakeEmptyPin(PinId::A), MakeEmptyPin(PinId::B), MakeEmptyPin(PinId::C), MakeEmptyPin(PinId::D), MakeEmptyPin(PinId::E), MakeEmptyPin(PinId::F),
		MakeEmptyPin(PinId::G),	MakeEmptyPin(PinId::H),	MakeEmptyPin(PinId::I),	MakeEmptyPin(PinId::J),	MakeEmptyPin(PinId::K),	MakeEmptyPin(PinId::L),
		MakeEmptyPin(PinId::M), MakeEmptyPin(PinId::N), MakeEmptyPin(PinId::O), MakeEmptyPin(PinId::P), MakeEmptyPin(PinId::Q), MakeEmptyPin(PinId::R),
		MakeEmptyPin(PinId::S), MakeEmptyPin(PinId::T), MakeEmptyPin(PinId::U), MakeEmptyPin(PinId::V), MakeEmptyPin(PinId::W), MakeEmptyPin(PinId::X) };

	occupance = temp_occupance;

	switch (index)
	{
		case 51:
		{
			unsigned long long pinB = GetPin(PinId::B, occupance);
			if (PlacePiece(occupance, pinB, GreenPiece, 18)) // Flip GreenPiece and do not rotate
			{
				unsigned long long pinD = GetPin(PinId::D, occupance);
				if (PlacePiece(occupance, pinD, LightPurplePiece, 5)) // Rotate by 5 LightPurplePiece around 1st pin
				{
					unsigned long long pinJ = GetPin(PinId::J, occupance);
					if (PlacePiece(occupance, pinJ, RedPiece, 18)) // Flip RedPiece and do not rotate
					{

						unsigned long long pinL = GetPin(PinId::L, occupance);
						if (PlacePiece(occupance, pinL, LightBluePiece, 20)) // Flip LightBluePiece and do not rotate by 2 around 1st
						{
							unsigned long long pinP = GetPin(PinId::P, occupance);
							if (PlacePiece(occupance, pinP, LightPinkPiece, 20)) // Flip LightPinkPiece and rotate by 2 around 1st
							{
								// Resuse pinP
								if (PlacePiece(occupance, pinP, LightGreenPiece, 3)) // Rotate LightGreenPiece by 3 around 1st
								{
									// Reuse pinL
									if (PlacePiece(occupance, pinL, OrangePiece, 22)) // Flip OrangePiece and rotate by 4 around 1st
									{
										// Display the move
										pr.Visualize(occupance);

										// Pieces left
										std::vector<unsigned long> pieces_51 = { DarkBluePiece, DarkPurplePiece, DarkGreenPiece, DarkPinkPiece, YellowPiece };
										pieces = pieces_51;

										return true;
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	
		case 120:
		{
			// const unsigned long LightGreenPiece = MakePiece(PieceColor::LightGreen, 0b01000001, 0b00111111, 0b00111011, Direction::East, Direction::East);
			// const unsigned long DarkGreenPiece = MakePiece(PieceColor::DarkGreen, 0b01000001, 0b01001100, 0b00111011, Direction::East, Direction::NorthWest);
			unsigned long long pinT = GetPin(PinId::T, occupance);
			if (PlacePiece(occupance, pinT, DarkGreenPiece, 2)) // DarkGreenPiece rotated by 2 no flip 
			{
				// Display the move
				pr.Visualize(occupance);

				unsigned long long pinX = GetPin(PinId::X, occupance);
				if (PlacePiece(occupance, pinX, LightGreenPiece, 9)) // Flip LightGreenPiece and rotate by 3
				{
					// Display the move
					pr.Visualize(occupance);
					
					// Pieces left
					std::vector<unsigned long> pieces_120 = { LightBluePiece,	DarkBluePiece, DarkPurplePiece,	LightPurplePiece,
						GreenPiece,	LightPinkPiece,DarkPinkPiece,RedPiece,OrangePiece,YellowPiece };

					pieces = pieces_120;

					return true;					
				}
			}
		}
		break;

		// Empty board		
		case 0:
		default:
			pieces = all_pieces;
			return true;
		break;
	}
	return false;
}