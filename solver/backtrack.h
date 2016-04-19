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

#pragma once

#include "iqlink.h"
#include <vector>
#include <set>

class CIqLinkBackTrack
{
public:
	// Empty board constructor
	CIqLinkBackTrack(unsigned char positions) : _positions(positions) {}
	~CIqLinkBackTrack() {}
	// Main solvinf function
	bool Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize);
	// Getter of the computed solutions
	std::set<std::vector<unsigned long long> > GetSolutions() const { return _solutions; }
	// Demo introduction to pieces
	void ShowPieces(std::vector<unsigned long> pieces);
private:
	unsigned char								_positions;
	std::set<std::vector<unsigned long long> >	_solutions;
};