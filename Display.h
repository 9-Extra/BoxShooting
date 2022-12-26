#pragma once

#include "GameDesc.h"
#include "CGmath.h"
#include "Error.h"

class Display {
private:
	friend class Graphcis;

	HWND hwnd;
	unsigned int width;
	unsigned int height;

	void init_window();

public:

	Display(unsigned int width, unsigned int height);
	~Display() {
	}
};
