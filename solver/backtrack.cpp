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

// Brute force approach
bool CIqLinkBackTrack::Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize)
{
	UNREFERENCED_PARAMETER(occupance);
	UNREFERENCED_PARAMETER(pieces);
	UNREFERENCED_PARAMETER(fStopAt1st);
	UNREFERENCED_PARAMETER(fVisualize);
	return false;
}
bool CIqLinkBackTrackBrute::Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize)
{
	// Trivial case ends the recursion successfully
	if (pieces.empty())
	{
		// Found solution - record it. std::set makes sure we have all members unique
		AddSolution(occupance);
		
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
			// some pins to avoid repetitions. Any reduction results in massive time saving. But for that we have another class...
			if (PinIsAdjacent(pin, occupance, pieces.size()))
			{
				// Place a piece in every possible position (rotation/flip)
				for (unsigned char pos = 0; pos < IqLinkPiecePositions; pos++)
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
		for (unsigned char pos = 0; pos < IqLinkPiecePositions; pos++)
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
bool CIqLinkBackTrackHeuristic::Futile(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces)
{
	// Get full pins count in pieces
	size_t fpc = 0, cpc = 0, apc = pieces.size() * 3;
	std::for_each(pieces.begin(), pieces.end(), [&](auto piece) { fpc += GetPieceFullPins(piece); cpc += GetPieceCenters(piece); });

	std::vector<unsigned long long> occf, occc, occa;
	std::for_each(occupance.begin(), occupance.end(), [&](auto pin) 
	{ 
		if (IsPinEmpty(pin)) occf.push_back(pin); 
		if (PinCenterFree(pin)) occc.push_back(pin); 
		if (!PinFull(pin)) occa.push_back(pin); 
	});

	// Subtract unreachable pins in all 3 vectors
	auto lambda = [](auto pin) { return PinUnreachable(pin); };
	std::remove_if(occf.begin(), occf.end(), lambda);
	std::remove_if(occc.begin(), occc.end(), lambda);
	std::remove_if(occa.begin(), occa.end(), lambda);
	
	// Check we have enough pins to use
	/*if (apc > occa.size())
	{
		return true;
	}*/
	if (fpc > occf.size())
	{
		return true;
	}
	if (cpc > occc.size())
	{
		return true;
	}
	return false;
}
bool CIqLinkBackTrackHeuristic::Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize)
{
	// Trivial case ends the recursion successfully
	if (pieces.empty())
	{
		// Found solution - record it. std::set makes sure we have all members unique
		AddSolution(occupance);

		// Success. Go upwards with positive news!
		return true;
	}

	// Detect futile regress - there is not enough pins to accomodate remaining pieces
	if (Futile(occupance, pieces))
	{
		return false;
	}

	// Compute all possible moves and their cost. Sort them by cost. The smaller the better. We will try the moves from the best ones to worst.
	std::vector<CIqLinkMove> statespace;
	if (!GenerateStateSpace(occupance, pieces, statespace))
	{
		return false;
	}

	// Sort by cost
	std::sort(statespace.begin(), statespace.end());

	// Cycle through all possible moves in this round. Most promising ones are at the beginning.
	for (auto move : statespace)
	{
		// Remove used piece from pieces
		std::vector<unsigned long> next_pieces;
		std::copy_if(pieces.begin(), pieces.end(), std::back_inserter(next_pieces), [=](auto p) { return !(p == move.piece); });
		
		// Update new occupance
		std::vector<unsigned long long> next_occupance = occupance;
		UpdatePin(move.pin1, next_occupance);
		UpdatePin(move.pin2, next_occupance);
		UpdatePin(move.pin3, next_occupance);

		if (fVisualize)
		{
				// Tracing
				std::wstringstream str;
				str << " Piece " << PieceName(move.piece).c_str() << ", Pin " << PinIdName(move.pin1) << ", Position " << (int)(move.position) << ", Level " << pieces.size() << ", Cost " << move.cost << std::endl;
				OutputDebugString(str.str().c_str());

				// Display the move
				IqLinkPresenter pr;
				pr.Visualize(next_occupance);
		}
		
		// Recursively iterate with new occupance and pieces until we consume all pieces or hit the non-placeable situation or end
		bool fResult = Solve(next_occupance, next_pieces, fStopAt1st, fVisualize);

		// Exiting after the 1st solution?
		if (fStopAt1st &&  fResult)
		{
			return true;
		}		
	}

	// All possible moves were exhausted. Return.
	return false;
}
unsigned long CIqLinkBackTrackHeuristic::GetCost(unsigned long long pin_old)
{
	// Occupying a new empty pin is penalized
	return IsPinEmpty(pin_old) ? 1 : 0;
}
bool CIqLinkBackTrackHeuristic::GenerateStateSpace(const std::vector<unsigned long long>& occupance, const std::vector<unsigned long>& pieces, std::vector<CIqLinkMove>& statespace)
{
	for (auto piece : pieces)
	{
		for (auto pin : occupance)
		{
			for (unsigned char position = 0; position < IqLinkPiecePositions; ++position)
			{
				// Rotate the pice to a given position. Get the possibly occupied pins with occupance masks
				unsigned long long pin1, pin2, pin3;
				if (RotatePiece(pin, piece, position, pin1, pin2, pin3))
				{
					unsigned long long pin1ex, pin2ex, pin3ex;
					if (GetExistingPin(pin1, occupance, pin1ex) && GetExistingPin(pin2, occupance, pin2ex) && GetExistingPin(pin3, occupance, pin3ex))
					{
						unsigned long cost = GetCost(pin1ex) + GetCost(pin2ex) + GetCost(pin3ex);
						// Check possible placement and get the new pins
						unsigned long long newpin1, newpin2, newpin3;
						if (IsAvailable(pin1, occupance, newpin1) && IsAvailable(pin2, occupance, newpin2) && IsAvailable(pin3, occupance, newpin3))
						{
							CIqLinkMove move = { cost, 	piece,	position, newpin1, newpin2, newpin3 };
							statespace.push_back(move);
						}
					}
				}				
			}
		}
	}
	return true;
}
