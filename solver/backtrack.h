#pragma once
#include <vector>
#include <map>

#include "iqlink.h"

class CIqLinkBackTrack
{
public:
	// Empty board constructor
	CIqLinkBackTrack(	unsigned char pins, 
						unsigned char pieces,
						unsigned char positions) : _pins(pins), _pieces(pieces), _positions(positions) {}
	~CIqLinkBackTrack() {}
	void Solve(std::vector<unsigned long> occupance, unsigned long availability);
	std::vector<std::vector<unsigned long> > GetSolutions() const { return _solutions; }
private:
	unsigned char								_pins;
	unsigned char								_pieces;
	unsigned char								_positions;
	std::vector<std::vector<unsigned long> >	_solutions;
};