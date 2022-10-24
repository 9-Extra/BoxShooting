#include "CallBacks.h"

void MouseCallBackEmpty(float x, float y, bool lb, bool rb) {
	debug_log("Empty mouse callback called with: (%f, %f)\n", x, y);
}

void* MouseCallBackArg = nullptr;
void (*MouseCallBackFunc) (float x, float y, bool lb, bool rb) = MouseCallBackEmpty;