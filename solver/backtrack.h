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
	void Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces);
	std::vector<std::vector<unsigned long long> > GetSolutions() const { return _solutions; }
	void ShowPieces(std::vector<unsigned long> pieces);
private:
	unsigned char									_positions;
	std::vector<std::vector<unsigned long long> >	_solutions;
};