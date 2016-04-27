/*
*
* Copyright (C) 2016,  Jan Puskar <jan.puskar@gmail.com>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "stdafx.h"
#include "iqlink.h"
#include "backtrack.h"
#include "presenter.h"

void Intro(bool fShow)
{
	// Convenience flag
	if (!fShow) return;

	// Create backtracker instance
	CIqLinkBackTrack solver;
	
	// All 12 Pieces available
	std::vector<unsigned long> pieces = { LightBluePiece,	DarkBluePiece, DarkPurplePiece,	LightPurplePiece, DarkGreenPiece,LightGreenPiece,
		GreenPiece,	LightPinkPiece,DarkPinkPiece,RedPiece,OrangePiece,YellowPiece };

	solver.ShowPieces(pieces);
}

int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	// Make an introduction to all used pieces
	Intro(false);

	// Pieces and occupance vector are defined by the game number --> 51 in this case
	std::vector<unsigned long> pieces;
	std::vector<unsigned long long> occupance;
	if (!SetupGame(occupance, pieces, 120))
	{
		// Bad things have happened
		return -1;
	}

	// Solve the game, visualize partial progress flag and stop at 1st solution flag
	CIqLinkBackTrackHeuristic2 solver;
	solver.Solve(occupance, pieces, true, true);
	
	// Get computed solution(s) 
	std::set<std::vector<unsigned long long> > solutions = solver.GetSolutions();

	// Visualize the results. Wait for ENTER after each one.
	IqLinkPresenter presenter;
	presenter.Visualize(solutions, true);

	// Bye 
	return 0;
}
