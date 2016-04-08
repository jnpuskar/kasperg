#include "stdafx.h"
#include "backtrack.h"

void CIqLinkBackTrack::Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces)
{
	// Trivial case ends the recursion successfully
	if (pieces.empty())
	{
		// Found solution - record it
		_solutions.push_back(occupance);
	}

	// For all available pieces
	for (auto piece : pieces)
	{
		// On all available PINs
		for (auto pin : occupance)
		{
			// Place a piece in every possible position (rotation/flip)
			for ( unsigned char pos = 0; pos < _positions; pos++)
			{
				// And if successfull provide new occupance 
				std::vector<unsigned long long> next_occupance;
				if (IsPlaceable(occupance, next_occupance, pin, piece, pos))
				{
					// Remove used piece from pieces
					std::vector<unsigned long> next_pieces;
					std::copy_if(pieces.begin(), pieces.end(), next_pieces.begin(), [=](auto p) { return !(p == piece); });
						
					// Recursively iterate with new occupance and pieces until we consume all pieces or hit the non-placeable situation
					return Solve(next_occupance, next_pieces);
				}
			}			
		}
	}
}