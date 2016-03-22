#pragma once
#include <vector>
#include <map>

#include "iqlink.h"

class CIqLinkBackTrack
{
public:
	// Empty board constructor
	CIqLinkBackTrack(unsigned char positions) : _positions(positions) {}
	~CIqLinkBackTrack() {}
	void Solve(std::vector<unsigned long> occupance, std::vector<unsigned long> pieces);
	std::vector<std::vector<unsigned long> > GetSolutions() const { return _solutions; }
private:
	unsigned char								_positions;
	std::vector<std::vector<unsigned long> >	_solutions;
};