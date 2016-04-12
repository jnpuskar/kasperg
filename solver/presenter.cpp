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

const std::vector<unsigned char> s_lineoff = {  16, 15, 14, 13, 14, 15, 16, // A - F
                                                15, 
	                                            8, 7, 6, 5, 6, 7, 8, // G - L
                                                15,
											    16, 15, 14, 13, 14, 15, 16, // M - R
												15,
												8, 7, 6, 5, 6, 7, 8 // S - X
};

void IqLinkPresenter::Visualize(const std::vector<std::vector<unsigned long long>>& solutions)
{
	for (auto solution : solutions)
	{
		Visualize(solution);
	}
}
void IqLinkPresenter::Visualize(std::vector<unsigned long long>& solution)
{
#ifdef WIN32
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// Set buffer size
	COORD c = { _width, _height };
	if (!SetConsoleScreenBufferSize(hStdout, c))
	{
		cout << "SetConsoleScreenBufferSize: " << GetLastError() << endl;
		return;
	}

	// Set window size
	SMALL_RECT r = {0, 0, _width - 1, _height  - 1};
	if (!SetConsoleWindowInfo(hStdout, TRUE, &r))
	{
		cout << "SetConsoleWindowInfo: " << GetLastError() << endl;
		return;
	}
	
	// Clear screen
	system("cls");

	// Each pin will be displayed on given console
	for_each(solution.begin(), solution.end(), [&](auto ull) { DisplayPin(hStdout, ull); });
	
	// Wait till key is pressed
	system("pause");
#endif
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
	iqpin.Display(h, c6, c5, c4, c3, c2, c1, c0);
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
void IqLinkPin::Display(HANDLE h, unsigned char c6, unsigned char c5, unsigned char c4, unsigned char c3, unsigned char c2, unsigned char c1, unsigned char c0)
{

}

	// 12 lines + center	
	//			     2 --- 1   
	//			    / \   / \	
	//			   /   \ /   \	
	//			  3 --- A --- 0
	//			   \   / \   /	
	//			    \ /	  \ /	
	//			     4 --- 5	
								   
	std::vector<COORD> s_vCenters =
	
	SetConsoleTextAttribute(h, (WORD)c6);
}