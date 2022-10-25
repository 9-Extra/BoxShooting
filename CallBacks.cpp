#include "CallBacks.h"

void MouseCallBackEmpty(float x, float y, bool lb, bool rb) {
	debug_log("Empty mouse callback called with: (%f, %f)\n", x, y);
}

void* MouseCallBackArg = nullptr;
void (*MouseCallBackFunc) (float x, float y, bool lb, bool rb) = MouseCallBackEmpty;

void KeyDownCallBackEmpty(unsigned long long vk) {
	debug_log("Empty key down callback\n");
}
void (*KeyDownCallBackFunc) (unsigned long long vk) = KeyDownCallBackEmpty;
void* KeyDownCallBackArg = nullptr;

void KeyUpCallBackEmpty(unsigned long long vk) {
	debug_log("Empty key up callback\n");
}
void (*KeyUpCallBackFunc) (unsigned long long vk) = KeyUpCallBackEmpty;
void* KeyUpCallBackArg = nullptr;

void KillFocusCallBackEmpty() {
	debug_log("Empty lose focus callback\n");
};
void (*KillFocusCallBackFunc) (void) = KillFocusCallBackEmpty;
void* KillFocusArg = nullptr;