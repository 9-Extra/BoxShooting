#pragma once
#include "CGmath.h"
#include "CallBacks.h"

static void HandleMouseCallback(float x, float y, bool lb, bool rb);

class InputSystem {
private:
	Vector2f mouse_position;
	bool mouse_lb = false;
	bool mouse_rb = false;

	friend void HandleMouseCallback(float x, float y, bool lb, bool rb);

	void handle_mouse_move(float x, float y, bool lb, bool rb) {
		//debug_log("Real mouse handle called!\n");
		mouse_position = Vector2f(x, y);
		mouse_lb = lb;
		mouse_rb = rb;
	}
	


public:
	InputSystem() {
		MouseCallBackArg = this;
		MouseCallBackFunc = HandleMouseCallback;

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


};

static void HandleMouseCallback(float x, float y, bool lb, bool rb) {
	((InputSystem*)MouseCallBackArg)->handle_mouse_move(x, y, lb, rb);
}
