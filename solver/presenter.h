#pragma once
#include <vector>
#include <windows.h>
#include "iqlink.h"

class IqLinkPresenter
{
public:
	
	void Visualize(const std::vector<std::vector<unsigned long long>>& solutions);
	void Visualize(std::vector<unsigned long long>& solution);
	void DisplayPin(HANDLE h, unsigned long long pin);
private:
	const short _width = 110;
	const short _height = 36;
	const 
};

class IqLinkPin
{
public:
	IqLinkPin(PinId id, SHORT x, SHORT y) : _id(id), _x(x), _y(y) { }
	static SHORT GetX(PinId id);
	static SHORT GetY(PinId id);
	void Display(HANDLE h, unsigned char c6, unsigned char c5, unsigned char c4, unsigned char c3, unsigned char c2, unsigned char c1, unsigned char c0);
private:
	PinId _id;
	SHORT _x;
	SHORT _y;
};