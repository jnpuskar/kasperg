// solver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iqlink.h"
#include "backtrack.h"
#include "presenter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// Create backtracker instance
	CIqLinkBackTrack solver(IqLinkPiecePositions);

	// All Pieces available
	std::vector<unsigned long> pieces = {	LightBluePiece,	DarkBluePiece, DarkPurplePiece,	LightPurplePiece, DarkGreenPiece,LightGreenPiece,
											GreenPiece,	LightPinkPiece,DarkPinkPiece,RedPiece,OrangePiece,YellowPiece };

	// All PINs are empty
	std::vector<unsigned long long> occupance =	{ 	MakeEmptyPin(PinId::A), MakeEmptyPin(PinId::B), MakeEmptyPin(PinId::C), MakeEmptyPin(PinId::D), MakeEmptyPin(PinId::E), MakeEmptyPin(PinId::F),
													MakeEmptyPin(PinId::G),	MakeEmptyPin(PinId::H),	MakeEmptyPin(PinId::I),	MakeEmptyPin(PinId::J),	MakeEmptyPin(PinId::K),	MakeEmptyPin(PinId::L),
													MakeEmptyPin(PinId::M), MakeEmptyPin(PinId::N), MakeEmptyPin(PinId::O), MakeEmptyPin(PinId::P), MakeEmptyPin(PinId::Q), MakeEmptyPin(PinId::R),
													MakeEmptyPin(PinId::S), MakeEmptyPin(PinId::T), MakeEmptyPin(PinId::U), MakeEmptyPin(PinId::V), MakeEmptyPin(PinId::W), MakeEmptyPin(PinId::X) };

	// Find all solutions of the problem
	solver.Solve(occupance, pieces);
	
	// Get computed solutions 
	std::vector<std::vector<unsigned long long> > solutions = solver.GetSolutions();

	// Visualize the results
	IqLinkPresenter presenter;
	presenter.Visualize(solutions);

	return 0;
}

