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

	// 12 lines + center	
	//			     * --- *   
	//			    / \   / \	
	//			   /   \ /   \	
	//			  * --- A --- *
	//			   \   / \   /	
	//			    \ /	  \ /	
	//			     * --- *	
								   
	std::vector<COORD> s_vCenters =
		//     A       B        C       D       E       F				
		{ {20, 4}, {36, 4},{52, 4},{68, 4},{84, 4},{100, 4},
		//     G       H        I       J       K       L				
		   { 12, 12 },{ 28, 12 },{ 44, 12 },{ 60, 12 },{ 76, 12},{ 92, 12 },
		//    M      N        O       P       Q      R			
		  { { 20, 20 },{ 36, 20},{ 52, 20 },{ 68, 20 },{ 84, 20 },{ 100, 20 },
		//     S       T        U       V       W       X				
		  { 12, 28 },{ 28, 28 },{ 44, 28 },{ 60, 28 },{ 76, 28 },{ 92, 28 }};


	SetConsoleTextAttribute(h, (WORD)c6);
}