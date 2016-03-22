#include "stdafx.h"
#include "backtrack.h"

void CIqLinkBackTrack::Solve(std::vector<unsigned long> occupance, std::vector<unsigned long> pieces)
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
			if (IsPinAvailable(pin))
			{
				// Place it in every possible position (rotation/flip)
				for (unsigned char pos = 0; pos < _positions; pos++)
				{
					std::vector<unsigned long> next_occupance;
					
					if (IsPlaceable(occupance, next_occupance, pin, piece, pos))
					{
						// And if placeable recursively iterate until we consume all pieces or hit the non-placeable situation
						auto next_pieces = RemovePiece(pieces, piece);
						return Solve(next_occupance, next_pieces);
					}
				}
			}
		}
	}
}