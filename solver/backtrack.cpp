#include "stdafx.h"
#include "backtrack.h"

void CIqLinkBackTrack::Solve(std::vector<unsigned long> occupance, unsigned long availability)
{
	if (availability == 0)
	{
		// Found solution - record it
		_solutions.push_back(occupance);
	}

	// For all available pieces
	for (unsigned char piece = 0; piece < _pieces; piece++)
	{
		if (IsPieceAvailable(availability, piece))
		{
			// Try to place them in every possible position (rotation/flip)
			for (unsigned char pos = 0; pos < _positions; pos++)
			{
				// To any available PIN
				for (unsigned char pin = A_PIN; pin < _pins; pin++)
				{
					if (IsPinAvailable(occupance, pin))
					{
						std::vector<unsigned long> new_occupance;
						if (IsPlaceable(occupance, new_occupance, pin, piece, pos))
						{
							// And if placeable recursively iterate until we consume all pieces or hit the non-placeable situation
							unsigned short new_availability = ConsumePiece(availability, piece);
							return Solve(new_occupance, new_availability);
						}
					}
				}
			}
		}
	}
}