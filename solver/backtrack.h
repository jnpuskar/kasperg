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
#include <atomic>
#include <mutex>

struct CIqLinkMove
{	
	unsigned long		cost;			
	unsigned long		piece;	 // Piece itself
	unsigned char		position;// Rotation & flip
	unsigned long long	pin1;	 // Pin1 after Piece was placed on existing occupance	
	unsigned long long	pin2;	 // Pin1 after Piece was placed on existing occupance	
	unsigned long long	pin3;	 // Pin1 after Piece was placed on existing occupance		

	// We need to sort moves based on the price
	const bool operator < (const CIqLinkMove &rhs) const { return  cost < rhs.cost; }
	const bool operator == (const CIqLinkMove &rhs) const 
	{ 
		return  cost == rhs.cost && piece == rhs.piece && position == rhs.position &&
			pin1 == rhs.pin1 && pin2 == rhs.pin2 && pin3 == rhs.pin3;
	}
};

class CIqLinkBackTrack
{
public:
	// Empty board constructor
	CIqLinkBackTrack() {}
	virtual ~CIqLinkBackTrack() {}
	// Brute force approach
	virtual bool Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize);
	// Getter of the computed solutions
	std::set<std::vector<unsigned long long> > GetSolutions() const { return _solutions; }
	void AddSolution(const std::vector<unsigned long long>& v) { _solutions.insert(v); }
	// Demo introduction to pieces
	void ShowPieces(std::vector<unsigned long> pieces);
private:
	std::set<std::vector<unsigned long long> >	_solutions;
};

class CIqLinkBackTrackBrute : public CIqLinkBackTrack
{
public:
	// Empty board constructor
	CIqLinkBackTrackBrute() : CIqLinkBackTrack() {}
	~CIqLinkBackTrackBrute() {}
	virtual bool Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize);	
};

class CIqLinkBackTrackHeuristic : public CIqLinkBackTrack
{
public:
	// Empty board constructor
	CIqLinkBackTrackHeuristic() : CIqLinkBackTrack() {}
	~CIqLinkBackTrackHeuristic() {}
	// Solve using a cost function associated with each move
	virtual bool Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize);	
	bool GenerateStateSpace(const std::vector<unsigned long long>& occupance, const std::vector<unsigned long>& pieces, std::map<unsigned long, std::vector<CIqLinkMove>>& statespace);

private:
	unsigned long Cost(const std::vector<unsigned long long>& occupance, const std::vector<unsigned long> pieces);
	unsigned long EvaluateMove(
		std::vector<unsigned long long> occupance,
		std::vector<unsigned long> pieces,
		unsigned char level);
	bool PinUnreachable(const std::vector<unsigned long long>& occupance, const std::vector<unsigned long>& pieces, unsigned long long pin);
	
};

class CIqLinkBackTrackHeuristic2 : public CIqLinkBackTrack
{
public:
	// Empty board constructor
	CIqLinkBackTrackHeuristic2() : CIqLinkBackTrack() {}
	~CIqLinkBackTrackHeuristic2() {}
	// Solve using a cost function associated with each move
	virtual bool Solve(std::vector<unsigned long long> occupance, std::vector<unsigned long> pieces, bool fStopAt1st, bool fVisualize);
	bool GenerateStateSpace(const std::vector<unsigned long long>& occupance, const std::vector<unsigned long>& pieces, std::map<unsigned long, std::vector<CIqLinkOcc>>& statespace);
};
	bool Solve_120(std::vector<unsigned long long> occupance, std::map<unsigned long, std::vector<CIqLinkOcc>> statespace, bool fStopAt1st, bool fVisualize, unsigned long id, unsigned long tnum);
	bool Solve_51(std::vector<unsigned long long> occupance, std::map<unsigned long, std::vector<CIqLinkOcc>> statespace, bool fStopAt1st, bool fVisualize, unsigned long id, unsigned long tnum);
	bool Solve_0(std::vector<unsigned long long> occupance, std::map<unsigned long, std::vector<CIqLinkOcc>> statespace, bool fStopAt1st, bool fVisualize, unsigned long id, unsigned long tnum);

	extern std::atomic<unsigned long> g_cnt;
	extern std::mutex g_mutex;