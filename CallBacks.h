#pragma once

#include "Error.h"

extern void (*MouseCallBackFunc) (float x, float y, bool lb, bool rb);
extern void* MouseCallBackArg;