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