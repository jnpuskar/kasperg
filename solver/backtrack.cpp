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

	std::map<unsigned long, std::vector<CIqLinkMove>> statespace;
	GenerateStateSpace(occupance, pieces, statespace);

	// Cycle through all possible moves in this round. Most promising ones are at the beginning.
	for (auto piece : pieces)
	{
		std::vector<CIqLinkMove>& moves = statespace[piece];
		for (auto move : moves)
		{
				// Remove used piece from pieces
				std::vector<unsigned long> next_pieces;
				std::copy_if(pieces.begin(), pieces.end(), std::back_inserter(next_pieces), [=](auto p) { return !(p == piece); });

				// Update new occupance
				std::vector<unsigned long long> next_occupance = occupance;
				UpdatePin(move.pin1, next_occupance);
				UpdatePin(move.pin2, next_occupance);
				UpdatePin(move.pin3, next_occupance);

				if (fVisualize)
				{
					// Tracing
					std::wstringstream str;
					str << " Piece " << PieceName(piece).c_str() << ", Pin " << PinIdName(move.pin1) << ", Position " << (int)(move.position) << ", Level " << pieces.size() << ", Cost " << move.cost << std::endl;
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
	}
	

	// All possible moves were exhausted. Return.
	return false;
}
unsigned long CIqLinkBackTrackHeuristic::Cost(const std::vector<unsigned long long>& occupance, const std::vector<unsigned long> pieces)
{
	// Cost - we value "condense" states. For each pin count 
	// 1 - pin is fully occupied
	// 2 - pin is halfoccupied (!full and !empty) and can be used for placement
	// 4 - pin is halfoccupied (!full and !empty), but cannot be used for placement
	// 8 - pin is empty, but cannot be used for placement
	unsigned long cost = 0;
	std::for_each(occupance.begin(), occupance.end(), [&](auto pin) {
		if (PinFull(pin))
		{
			cost += 1;
		}
		else
		{
			if (!IsPinEmpty(pin))
			{
				cost += 4;
			}
		}
	});
	return cost;
}
bool CIqLinkBackTrackHeuristic::PinUnreachable(const std::vector<unsigned long long>& occupance, const std::vector<unsigned long>& pieces, unsigned long long pin)
{
	for (auto piece : pieces)
	{
		for (auto pint : occupance)
		{
			for (unsigned char position = 0; position < IqLinkPiecePositions; ++position)
			{
				// Rotate the pice to a given position. Get the possibly occupied pins with occupance masks
				unsigned long long pin1, pin2, pin3;
				if (RotatePiece(pint, piece, position, pin1, pin2, pin3))
				{
					// Check possible placement and get the new pins
					unsigned long long newpin1, newpin2, newpin3;
					if (IsAvailable(pin1, occupance, newpin1) && IsAvailable(pin2, occupance, newpin2) && IsAvailable(pin3, occupance, newpin3))
					{
						if (pin == newpin1 || pin == newpin2 || pin == newpin3)
						{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

bool CIqLinkBackTrackHeuristic::GenerateStateSpace(const std::vector<unsigned long long>& occupance, const std::vector<unsigned long>& pieces, std::map<unsigned long, std::vector<CIqLinkMove>>& statespace)
{
	for (auto piece : pieces)
	{
		std::vector<CIqLinkMove> m;
		statespace[piece] = m;

		for (auto pin : occupance)
		{
			for (unsigned char position = 0; position < IqLinkPiecePositions; ++position)
			{
				// Rotate the piece to a given position. Get the possibly occupied pins with occupance masks
				unsigned long long pin1, pin2, pin3;
				if (RotatePiece(pin, piece, position, pin1, pin2, pin3))
				{
					// Check possible placement and get the new pins
					unsigned long long newpin1, newpin2, newpin3;
					if (IsAvailable(pin1, occupance, newpin1) && IsAvailable(pin2, occupance, newpin2) && IsAvailable(pin3, occupance, newpin3))
					{
						CIqLinkMove move = { 0xFFFFFFFF,	piece,	position, newpin1, newpin2, newpin3 };

						std::vector<unsigned long long> next_occupance = occupance;
						UpdatePin(move.pin1, next_occupance);
						UpdatePin(move.pin2, next_occupance);
						UpdatePin(move.pin3, next_occupance);

						move.cost = EvaluateMove(next_occupance, pieces, 2);
						statespace[piece].push_back(move);

						if (false)
						{
							// Tracing
							std::wstringstream str;
							str << " Piece " << PieceName(piece).c_str() << ", Pin " << PinIdName(move.pin1) << ", Position " << (int)(move.position) << ", Level " << pieces.size() << ", Cost " << move.cost << std::endl;
							OutputDebugString(str.str().c_str());

							// Display the move
							IqLinkPresenter pr;
							pr.Visualize(next_occupance);

							// Press any key
							//std::cin.ignore();
						}

					}					
				}				
			}

			// Sort the vector and make it unique
			std::sort(statespace[piece].begin(), statespace[piece].end());
			statespace[piece].erase(std::unique(statespace[piece].begin(), statespace[piece].end()), statespace[piece].end());
		}
	}


	return true;
}

// Find the smallest cost in all branches. This is used to sort the most promising moves at the begining of statespace set.
unsigned long CIqLinkBackTrackHeuristic::EvaluateMove(
	std::vector<unsigned long long> occupance,
	std::vector<unsigned long> pieces,
	unsigned char level)
{
	return Cost(occupance, pieces);
}