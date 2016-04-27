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
#include <vector>
#include <set>
#include <windows.h>
#include "iqlink.h"

class IqLinkPresenter
{
public:
	
	void Visualize(const std::set<std::vector<unsigned long long>>& solutions, bool fWait);
	void Visualize(const std::vector<unsigned long long>& solution);
	void Visualize(const CIqLinkOcc& occ, PieceColor c);
	void DisplayPin(HANDLE h, unsigned long long pin);
private:
	const short _width = 110;
	const short _height = 36;
};

class IqLinkPin
{
public:
	IqLinkPin(PinId id, SHORT x, SHORT y) : _id(id), _x(x), _y(y) { }
	static SHORT GetX(PinId id);
	static SHORT GetY(PinId id);
	void Display(HANDLE h, PieceColor c6, PieceColor c5, PieceColor c4, PieceColor c3, PieceColor c2, PieceColor c1, PieceColor c0);
	void DisplayEdge(HANDLE h, COORD xy, PieceColor color, wchar_t ch);
	void DisplayCenter(HANDLE h, COORD xy, PieceColor color, wchar_t ch);
private:
	PinId _id;
	SHORT _x;
	SHORT _y;
};