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
#include "presenter.h"
#include "iqlink.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <algorithm>

using namespace std;
void IqLinkPresenter::Visualize(const std::set<std::vector<unsigned long long>>& solutions, bool fWait)
{
	for (auto solution : solutions)
	{
		Visualize(solution);
		if (fWait)
		{
			// Press ENTER to continue
			std::cin.ignore();
		}
	}
}
void IqLinkPresenter::Visualize(const std::vector<unsigned long long>& solution)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Clear screen
	system("cls");

	// Each pin will be displayed on given console
	for_each(solution.begin(), solution.end(), [&](auto ull) { DisplayPin(hStdout, ull); });	
}
void IqLinkPresenter::Visualize(const CIqLinkOcc& occ, PieceColor c)
{

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	std::vector<unsigned long long> occupance;
	DecompressOcc(occ, c, occupance);
	
	// Each pin will be displayed on given console
	for_each(occupance.begin(), occupance.end(), [&](auto ull) { DisplayPin(hStdout, ull); });
}
void IqLinkPresenter::DisplayPin(HANDLE h, unsigned long long pin)
{
	// -Pin-ID-    -Dir6-Color- -Dir5-Color- -Dir4-Color- -Dir3-Color- -Dir2-Color- -Dir1-Color- -Dir0-Color- 
	// b32 ... b28   b27 ... b24   b23 ... b20  b19 ... b16  b15 ... b12  b11 ... b8   b7 ... b4    b3 ... b0
	unsigned long long id = (pin & (0b11111 << 28)) >> 28;
	unsigned long long c6 = (pin & (0b1111 << 24)) >> 24;
	unsigned long long c5 = (pin & (0b1111 << 20)) >> 20;
	unsigned long long c4 = (pin & (0b1111 << 16)) >> 16;
	unsigned long long c3 = (pin & (0b1111 << 12)) >> 12;
	unsigned long long c2 = (pin & (0b1111 << 8)) >> 8;
	unsigned long long c1 = (pin & (0b1111 << 4)) >> 4;
	unsigned long long c0 = (pin & 0b1111);

	SHORT x = IqLinkPin::GetX((PinId)id);
	SHORT y = IqLinkPin::GetY((PinId)id);

	IqLinkPin iqpin((PinId)id, x, y);
	iqpin.Display(h, (PieceColor)c6, (PieceColor)c5, (PieceColor)c4, (PieceColor)c3, (PieceColor)c2, (PieceColor)c1, (PieceColor)c0);
}

SHORT IqLinkPin::GetX(PinId id)
{
	switch (id)
	{
	case PinId::A:
	case PinId::M:
		return 4;
	case PinId::B:
	case PinId::N:
		return 14;
	case PinId::C:
	case PinId::O:
		return 24;
	case PinId::D:
	case PinId::P:
		return 34;
	case PinId::E:
	case PinId::Q:
		return 44;
	case PinId::F:
	case PinId::R:
		return 54;

	case PinId::G:
	case PinId::S:
		return 10;
	case PinId::H:
	case PinId::T:
		return 20;
	case PinId::I:
	case PinId::U:
		return 30;
	case PinId::J:
	case PinId::V:
		return 40;
	case PinId::K:
	case PinId::W:
		return 50;
	case PinId::L:
	case PinId::X:
		return 60;
	default:
		return 0;
	}
}
SHORT IqLinkPin::GetY(PinId id)
{
	switch (id)
	{
	case PinId::A:
	case PinId::B:
	case PinId::C:
	case PinId::D:
	case PinId::E:
	case PinId::F:
		return 3;
	case PinId::G:
	case PinId::H:
	case PinId::I:
	case PinId::J:
	case PinId::K:
	case PinId::L:
		return 8;
	case PinId::M:
	case PinId::N:
	case PinId::O:
	case PinId::P:
	case PinId::Q:
	case PinId::R:
		return 13;
	case PinId::S:
	case PinId::T:
	case PinId::U:
	case PinId::V:
	case PinId::W:
	case PinId::X:
		return 18;
	default:
		return 0;
	}
}
void IqLinkPin::Display(HANDLE h, PieceColor c6, PieceColor c5, PieceColor c4, PieceColor c3, PieceColor c2, PieceColor c1, PieceColor c0)
{	
	COORD 
		center = { _x, _y }, 
		dir0 = { _x + 4  , _y }, dir1 = { _x + 2 , _y - 2 }, dir2 = { _x - 2 , _y - 2 }, 
		dir3 = { _x - 4 , _y }, dir4 = { _x - 2 , _y + 2 }, dir5 = { _x + 2 , _y + 2 },
		dir01 = { _x + 3  , _y - 1 }, dir12 = { _x , _y - 2 }, dir23 = { _x - 3  , _y - 1 }, 
		dir34 = { _x - 3  , _y + 1 }, dir45 = { _x  , _y + 2 }, dir50 = { _x + 3, _y + 1 },
		dirc0 = { _x + 2  , _y }, dirc1 = { _x + 1  , _y - 1 }, dirc2 = { _x - 1 , _y - 1 }, 
		dirc3 = { _x - 2  , _y }, dirc4 = { _x - 1  , _y + 1 }, dirc5 = { _x + 1  , _y + 1 };

	// Display main points + center
	DisplayCenter(h, center, c6, L'*');
	DisplayEdge(h, dir0, c0, L'*');
	DisplayEdge(h, dir1, c1, L'*');
	DisplayEdge(h, dir2, c2, L'*');
	DisplayEdge(h, dir3, c3, L'*');
	DisplayEdge(h, dir4, c4, L'*');
	DisplayEdge(h, dir5, c5, L'*');

	// Display contours
	if (c0 == c1 && (c0 != c6))	DisplayEdge(h, dir01, c0, L'\\');
	if (c3 == c4 && (c3 != c6)) DisplayEdge(h, dir34, c3, L'\\');
	if (c1 == c2 && (c1 != c6))DisplayEdge(h, dir12, c2, L'-');
	if (c4 == c5 && (c4 != c6))DisplayEdge(h, dir45, c4, L'-');
	if (c2 == c3 && (c2 != c6))DisplayEdge(h, dir23, c2, L'/');
	if (c5 == c0 && (c5 != c6))DisplayEdge(h, dir50, c5, L'/');
	
	// Display center lines
	if (c6 == c0)DisplayEdge(h, dirc0, c6, L'-');
	if (c6 == c1)DisplayEdge(h, dirc1, c6, L'/');
	if (c6 == c2)DisplayEdge(h, dirc2, c6, L'\\');
	if (c6 == c3)DisplayEdge(h, dirc3, c6, L'-');
	if (c6 == c4)DisplayEdge(h, dirc4, c6, L'/');
	if (c6 == c5)DisplayEdge(h, dirc5, c6, L'\\');
}
void IqLinkPin::DisplayEdge(HANDLE h, COORD xy, PieceColor color, wchar_t ch)
{	
	if (color != PieceColor::NoColor)
	{
		if (s_piececolors.find(color) == s_piececolors.end())
		{
			return;
		}
		WORD c = (WORD)(s_piececolors.find(color)->second);
		if(SetConsoleCursorPosition(h, xy))
		{
			if (SetConsoleTextAttribute(h, c))
			{
				std::wcout << ch;
			}
		}
	}
}
void IqLinkPin::DisplayCenter(HANDLE h, COORD xy, PieceColor color, wchar_t ch)
{
	WORD c;
	wchar_t cht;
	if (color == PieceColor::NoColor)
	{
		c = (WORD)7;
		cht = 248;
	}
	else
	{
		if (s_piececolors.find(color) == s_piececolors.end())
		{
			return;
		}
		c = (WORD)(s_piececolors.find(color)->second);
		cht = ch;
	}
	if (SetConsoleCursorPosition(h, xy))
	{
		if (SetConsoleTextAttribute(h, c))
		{
			std::wcout << cht;
		}
	}	
}