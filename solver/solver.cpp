// solver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iqlink.h"
#include "backtrack.h"
#include "presenter.h"

void Demo()
{
	// Create backtracker instance
	CIqLinkBackTrack solver(IqLinkPiecePositions);
	// All 12 Pieces available
	std::vector<unsigned long> pieces = { LightBluePiece,	DarkBluePiece, DarkPurplePiece,	LightPurplePiece, DarkGreenPiece,LightGreenPiece,
		GreenPiece,	LightPinkPiece,DarkPinkPiece,RedPiece,OrangePiece,YellowPiece };

	solver.ShowPieces(pieces);
}

int _tmain(int argc, _TCHAR* argv[])
{
	Demo();

	// Create backtracker instance
	CIqLinkBackTrack solver(IqLinkPiecePositions);

	// Pieces and occupance are defined by the game number
	std::vector<unsigned long> pieces;
	std::vector<unsigned long long> occupance;
	if (!SetupGame(occupance, pieces, 0))
	{
		return -1;
	}

	// Find all solutions of the problem
	solver.Solve(occupance, pieces);
	
	// Get computed solutions 
	std::vector<std::vector<unsigned long long> > solutions = solver.GetSolutions();

	// Visualize the results
	IqLinkPresenter presenter;
	presenter.Visualize(solutions);

	return 0;
}
