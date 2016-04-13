#include "stdafx.h"
#include "presenter.h"
#include "iqlink.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <algorithm>
using namespace std;
// Direction & position definition
//			     2 --- 1         2 --- 1         2 --- 1         2 --- 1         2 --- 1         2 --- 1
//			    / \   / \	    / \   / \	    / \   / \	    / \   / \	    / \   / \	    / \   / \
//			   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \
//			  3 --- A --- 0 - 3 --- B --- 0 - 3 --- C --- 0 - 3 --- D --- 0 - 3 --- E --- 0 - 3 --- F --- 0
//			   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /
//			    \ /	  \ /	    \ /   \ /	    \ /	  \ /	    \ /   \ /	    \ /	  \ /	    \ /   \ /
//			     4 --- 5		 4 --- 5	     4 --- 5		 4 --- 5	     4 --- 5		 4 --- 5
//              /       \       /       \       /       \       /       \       /       \       /
//	     2 --- 1         2 --- 1         2 --- 1         2 --- 1         2 --- 1         2 --- 1
//	    / \   / \	    / \   / \	    / \   / \	    / \   / \	    / \   / \	    / \   / \
//	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \
//	  3 --- G --- 0 - 3 --- H --- 0 - 3 --- I --- 0 - 3 --- J --- 0 - 3 --- K --- 0 - 3 --- L --- 0
//	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /
//	    \ /	  \ /	    \ /   \ /	    \ /	  \ /	    \ /   \ /	    \ /	  \ /	    \ /   \ /
//	     4 --- 5		 4 --- 5	     4 --- 5		 4 --- 5	     4 --- 5		 4 --- 5
//              \       /       \       /       \       /       \       /       \       /       \ 
//			     2 --- 1         2 --- 1         2 --- 1         2 --- 1         2 --- 1         2 --- 1
//			    / \   / \	    / \   / \	    / \   / \	    / \   / \	    / \   / \	    / \   / \
//			   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \
//			  3 --- M --- 0 - 3 --- N --- 0 - 3 --- O --- 0 - 3 --- P --- 0 - 3 --- Q --- 0 - 3 --- R --- 0
//			   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /
//			    \ /	  \ /	    \ /   \ /	    \ /	  \ /	    \ /   \ /	    \ /	  \ /	    \ /   \ /
//			     4 --- 5		 4 --- 5	     4 --- 5		 4 --- 5	     4 --- 5		 4 --- 5
//              /       \       /       \       /       \       /       \       /       \       /
//	     2 --- 1         2 --- 1         2 --- 1         2 --- 1         2 --- 1         2 --- 1
//	    / \   / \	    / \   / \	    / \   / \	    / \   / \	    / \   / \	    / \   / \
//	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \	   /   \ /   \
//	  3 --- S --- 0 - 3 --- T --- 0 - 3 --- U --- 0 - 3 --- V --- 0 - 3 --- W --- 0 - 3 --- X --- 0
//	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /	   \   / \   /
//	    \ /	  \ /	    \ /   \ /	    \ /	  \ /	    \ /   \ /	    \ /	  \ /	    \ /   \ /
//	     4 --- 5		 4 --- 5	     4 --- 5		 4 --- 5	     4 --- 5		 4 --- 5

void IqLinkPresenter::Visualize(const std::vector<std::vector<unsigned long long>>& solutions)
{
	for (auto solution : solutions)
	{
		Visualize(solution);
	}
}
void IqLinkPresenter::Visualize(const std::vector<unsigned long long>& solution)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	
	//// Set buffer size
	//COORD c = { _width, _height };
	//if (!SetConsoleScreenBufferSize(hStdout, c))
	//{
	//	cout << "SetConsoleScreenBufferSize: " << GetLastError() << endl;
	//	return;
	//}

	//// Set window size
	//SMALL_RECT r = {0, 0, _width - 1, _height  - 1};
	//if (!SetConsoleWindowInfo(hStdout, TRUE, &r))
	//{
	//	cout << "SetConsoleWindowInfo: " << GetLastError() << endl;
	//	return;
	//}
	
	// Clear screen
	system("cls");

	// Each pin will be displayed on given console
	for_each(solution.begin(), solution.end(), [&](auto ull) { DisplayPin(hStdout, ull); });
	
	// Wait till key is pressed
	//Sleep(500);
}
void IqLinkPresenter::DisplayPin(HANDLE h, unsigned long long pin)
{
	// Pin
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
		return 20;
	case PinId::B:
	case PinId::N:
		return 36;
	case PinId::C:
	case PinId::O:
		return 52;
	case PinId::D:
	case PinId::P:
		return 68;
	case PinId::E:
	case PinId::Q:
		return 84;
	case PinId::F:
	case PinId::R:
		return 100;

	case PinId::G:
	case PinId::S:
		return 12;
	case PinId::H:
	case PinId::T:
		return 28;
	case PinId::I:
	case PinId::U:
		return 44;
	case PinId::J:
	case PinId::V:
		return 60;
	case PinId::K:
	case PinId::W:
		return 76;
	case PinId::L:
	case PinId::X:
		return 92;
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
		return 4;
	case PinId::G:
	case PinId::H:
	case PinId::I:
	case PinId::J:
	case PinId::K:
	case PinId::L:
		return 12;
	case PinId::M:
	case PinId::N:
	case PinId::O:
	case PinId::P:
	case PinId::Q:
	case PinId::R:
		return 20;
	case PinId::S:
	case PinId::T:
	case PinId::U:
	case PinId::V:
	case PinId::W:
	case PinId::X:
		return 28;
	default:
		return 0;
	}
}
void IqLinkPin::Display(HANDLE h, PieceColor c6, PieceColor c5, PieceColor c4, PieceColor c3, PieceColor c2, PieceColor c1, PieceColor c0)
{	
	//			     2 --- 1   
	//			    / \   / \	
	//			   /   \ /   \	
	//			  3 --- A --- 0
	//			   \   / \   /	
	//			    \ /	  \ /	
	//			     4 --- 5	
	
	COORD center = { _x, _y }, dir0 = { _x + 6 , _y }, dir1 = { _x + 3 , _y - 3 }, dir2 = { _x - 3 , _y - 3 }, dir3 = { _x - 6 , _y }, dir4 = { _x - 3 , _y + 3 }, dir5 = { _x + 3 , _y + 3 };
	DisplayEdge(h, center, c6, L'a' + (wchar_t)c6);
	DisplayEdge(h, dir0, c0, L'a' + (wchar_t)c0);
	DisplayEdge(h, dir1, c1, L'a' + (wchar_t)c1);
	DisplayEdge(h, dir2, c2, L'a' + (wchar_t)c2);
	DisplayEdge(h, dir3, c3, L'a' + (wchar_t)c3);
	DisplayEdge(h, dir4, c4, L'a' + (wchar_t)c4);
	DisplayEdge(h, dir5, c5, L'a' + (wchar_t)c5);
}
void IqLinkPin::DisplayEdge(HANDLE h, COORD xy, PieceColor color, wchar_t ch)
{
	if (color != PieceColor::NoColor)
	{
		if(SetConsoleCursorPosition(h, xy))
		{
			if (SetConsoleTextAttribute(h, (WORD)15))
			{
				std::wcout << ch;
			}
		}
	}
}
