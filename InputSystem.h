#pragma once
#include "CGmath.h"
#include "CallBacks.h"

#define KEYCODE_MAX 0xff

class InputSystem {
private:
	Vector2f mouse_position;
	bool mouse_lb = false;
	bool mouse_rb = false;
	bool keyboard[KEYCODE_MAX];

	void clear_keyboard_state() {
		for (unsigned int i = 0; i < KEYCODE_MAX; i++) {
			keyboard[i] = false;
		}
	}

	void clear_mouse_state() {
		mouse_lb = false;
		mouse_rb = false;
		mouse_position = Vector2f(0.5, 0.5);
	}

	friend void HandleMouseCallback(float x, float y, bool lb, bool rb);

	void handle_mouse_move(float x, float y, bool lb, bool rb) {
		//debug_log("Real mouse handle called!\n");
		mouse_position = Vector2f(x, y);
		mouse_lb = lb;
		mouse_rb = rb;
	}

	void key_down(unsigned long long vk) {
		if (vk < KEYCODE_MAX) {
			keyboard[vk] = true;
		}
	}

	void key_up(unsigned long long vk) {
		if (vk < KEYCODE_MAX) {
			keyboard[vk] = false;
		}
	}

	static void HandleMouseCallback(float x, float y, bool lb, bool rb) {
		((InputSystem*)MouseCallBackArg)->handle_mouse_move(x, y, lb, rb);
	}

	static void HandleKeyDownCallBack(unsigned long long vk) {
		((InputSystem*)KeyDownCallBackArg)->key_down(vk);
	}
	static void HandleKeyUpCallBack(unsigned long long vk) {
		((InputSystem*)KeyUpCallBackArg)->key_up(vk);
	}
	static void HandleKillFocusCallBack() {
		((InputSystem*)KillFocusArg)->clear_keyboard_state();
		((InputSystem*)KillFocusArg)->clear_mouse_state();
	}
	


public:
	InputSystem() {
		clear_keyboard_state();
		clear_mouse_state();

		MouseCallBackArg = this;
		MouseCallBackFunc = HandleMouseCallback;
		KeyDownCallBackArg = this;
		KeyDownCallBackFunc = HandleKeyDownCallBack;
		KeyUpCallBackArg = this;
		KeyUpCallBackFunc = HandleKeyUpCallBack;
		KillFocusArg = this;
		KillFocusCallBackFunc = HandleKillFocusCallBack;

	}

	inline Vector2f get_mouse_position() {
		return mouse_position;
	}

	inline bool is_left_button_down() {
		return mouse_lb;
	}

	inline bool is_right_button_down() {
		return mouse_rb;
	}

	inline bool is_keydown(unsigned int key) {
		assert(key < KEYCODE_MAX);
		return keyboard[key];
	}


};
