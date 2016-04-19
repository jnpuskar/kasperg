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
#include "backtrack.h"
#include <iterator>
#include "presenter.h"
#include <iostream>
#include <sstream>

bool CIqLinkBackTrack::Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize)
{
	// Trivial case ends the recursion successfully
	if (pieces.empty())
	{
		// Found solution - record it. std::set makes sure we have all members unique
		_solutions.insert(occupance);
		
		// Success. Go upwards with positive news!
		return true;
	}

	// Cycle through available non-taken pieces. By proper heuristics we can put the more promising pieces at front. TBD...
	for (auto piece: pieces)
	{
		// For all pins 
		for (auto pin : occupance)
		{
			// Here we try to reduce the used pins. By proper and more robust pin checking we could exclude 
			// some pins to avoid repetitions. Any reduction results in massive time saving. 
			// This function should be trying harder than it is as of today...
			if (PinIsAdjacent(pin, occupance, pieces.size()))
			{
				// Place a piece in every possible position (rotation/flip)
				for (unsigned char pos = 0; pos < _positions; pos++)
				{
					// And if successfull provide new occupance 
					std::vector<unsigned long long> next_occupance;
					if (IsPlaceable(occupance, next_occupance, pin, piece, pos))
					{
						if (fVisualize)
						{
							// Tracing
							std::wstringstream str;
							str << " Piece " << PieceName(piece).c_str() << ", Pin " << PinIdName(pin) << ", Position " << (int)pos << ", Level " << pieces.size() << std::endl;
							OutputDebugString(str.str().c_str());

							// Display the move
							IqLinkPresenter pr;
							pr.Visualize(next_occupance);
						}

						// Remove used piece from pieces
						std::vector<unsigned long> next_pieces;
						std::copy_if(pieces.begin(), pieces.end(), std::back_inserter(next_pieces), [=](auto p) { return !(p == piece); });

						// Recursively iterate with new occupance and pieces until we consume all pieces or hit the non-placeable situation or end
						bool fResult = Solve(next_occupance, next_pieces, fStopAt1st, fVisualize);
						
						// Exiting after the 1st solution?
						if (fStopAt1st &&  fResult) 
						{	
							return true;
						}
					}
				}
			}
		}
	}

	// All possible moves were exhausted. Return.
	return false;
}

void CIqLinkBackTrack::ShowPieces(std::vector<unsigned long> pieces)
{
	// Empty occupance
	std::vector<unsigned long long> occupance = { MakeEmptyPin(PinId::A), MakeEmptyPin(PinId::B), MakeEmptyPin(PinId::C), MakeEmptyPin(PinId::D), MakeEmptyPin(PinId::E), MakeEmptyPin(PinId::F),
		MakeEmptyPin(PinId::G),	MakeEmptyPin(PinId::H),	MakeEmptyPin(PinId::I),	MakeEmptyPin(PinId::J),	MakeEmptyPin(PinId::K),	MakeEmptyPin(PinId::L),
		MakeEmptyPin(PinId::M), MakeEmptyPin(PinId::N), MakeEmptyPin(PinId::O), MakeEmptyPin(PinId::P), MakeEmptyPin(PinId::Q), MakeEmptyPin(PinId::R),
		MakeEmptyPin(PinId::S), MakeEmptyPin(PinId::T), MakeEmptyPin(PinId::U), MakeEmptyPin(PinId::V), MakeEmptyPin(PinId::W), MakeEmptyPin(PinId::X) };

	// Show pices at PinId::I
	unsigned long long pin = MakeEmptyPin(PinId::I);

	// For all available pieces
	for (auto piece : pieces)
	{		
		// Place a piece in every possible position (rotation/flip)
		for (unsigned char pos = 0; pos < _positions; pos++)
		{
			// And if successfull provide new occupance 
			std::vector<unsigned long long> next_occupance;
			if (IsPlaceable(occupance, next_occupance, pin, piece, pos))
			{	
				// Display the move
				IqLinkPresenter pr;
				pr.Visualize(next_occupance);	

				// Tracing
				std::wstringstream str;
				str << L" Piece " << PieceName(piece).c_str() << L", Pin " << PinIdName(pin) << L", Position " << (int)pos << ", Level " << pieces.size() << std::endl;
				OutputDebugString(str.str().c_str());

				// Press any key
				std::cin.ignore();
			}
			
		}
	}
}