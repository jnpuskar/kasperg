// solver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iqlink.h"
#include "backtrack.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// Create backtracker instance
	CIqLinkBackTrack solver(PIN_CNT, PIECES_CNT, POSITIONS_CNT);
	
	// All Pieces available
	unsigned long availability = 0xFFFFFFFF;

	// All PINs are empty
	std::vector<unsigned long> occupance(PIN_CNT, 0xFFFFFFFF);

	// Find all solutions of the problem
	solver.Solve(occupance, availability);

	// Get computed solutions and visualize them
	std::vector<std::vector<unsigned long> > solutions = solver.GetSolutions();

	return 0;
}

