#pragma once
#include <vector>
#include <map>

class CIqLinkBackTrack
{
public:
	// Empty board constructor
	CIqLinkBackTrack(	unsigned char pins, 
						unsigned char pieces,
						unsigned char positions) : _pins(pins), _pieces(pieces), _piecespan(piecespan), _positions(positions) {}
	~CIqLinkBackTrack() {}
	void Solve(std::vector<unsigned long> occupance, unsigned long avalability)
	{
		if (avalability == 0)
		{
			// Found solution. Print occupance
		}
		
		unsigned short piece = TakePiece(avalability);
		for (int )
	}

private:
	unsigned char								_pins;
	unsigned char								_pieces;
	unsigned char								_positions;
	std::vector<std::vector<unsigned long> >	_solutions;
};