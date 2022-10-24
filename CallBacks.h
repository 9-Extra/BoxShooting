#pragma once

#include "Error.h"

extern void (*MouseCallBackFunc) (float x, float y, bool lb, bool rb);
extern void* MouseCallBackArg;

extern void (*KeyDownCallBackFunc) (unsigned long long vk);
extern void* KeyDownCallBackArg;

extern void (*KeyUpCallBackFunc) (unsigned long long vk);
extern void* KeyUpCallBackArg;

extern void (*KillFocusCallBackFunc) (void);
extern void* KillFocusArg;