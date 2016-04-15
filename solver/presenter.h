#pragma once
#include <vector>
#include <windows.h>
#include "iqlink.h"

class IqLinkPresenter
{
public:
	
	void Visualize(const std::vector<std::vector<unsigned long long>>& solutions);
	void Visualize(const std::vector<unsigned long long>& solution);
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