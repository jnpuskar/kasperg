#include "stdafx.h"
#include "backtrack.h"
#include <iterator>
#include "presenter.h"
#include <iostream>
#include <sstream>

void CIqLinkBackTrack::Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces)
{
	// Trivial case ends the recursion successfully
	if (pieces.empty())
	{
		// Found solution - record it
		_solutions.push_back(occupance);

		// Display the move
		IqLinkPresenter pr;
		pr.Visualize(occupance);

		// Stop
		return;
	}

	// For all available pieces
	for (auto piece: pieces)
	{
		// For all pins
		for (auto pin : occupance)
		{
			// Reduce to pins next to full pins or partially empty pins - this tremendously reduces state space. Without it will run forever...
			if (PinIsAdjacent(pin, occupance))
			{
				// Place a piece in every possible position (rotation/flip)
				for (unsigned char pos = 0; pos < _positions; pos++)
				{
					// And if successfull provide new occupance 
					std::vector<unsigned long long> next_occupance;
					if (IsPlaceable(occupance, next_occupance, pin, piece, pos))
					{
						// Tracing
						std::wstringstream str;
						str << L" Piece " << PieceName(piece).c_str() << L", Pin " << PinIdName(pin) << L", Position " << (int)pos << ", Level " << pieces.size() << std::endl;
						OutputDebugString(str.str().c_str());

						// Display the move
						IqLinkPresenter pr;
						pr.Visualize(next_occupance);

						// Remove used piece from pieces
						std::vector<unsigned long> next_pieces;
						std::copy_if(pieces.begin(), pieces.end(), std::back_inserter(next_pieces), [=](auto p) { return !(p == piece); });

						// Recursively iterate with new occupance and pieces until we consume all pieces or hit the non-placeable situation
						Solve(next_occupance, next_pieces);
					}
				}
			}
		}
	}
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

				system("pause");
			}
			
		}
	}
}