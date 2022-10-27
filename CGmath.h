#pragma once

#include <cmath>

static float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long*)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);               // what the fuck?
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	//y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

struct Vector3f {
	union {
		struct
		{
			float x, y, z;
		};
		float v[3];
	};
	Vector3f() : x(0), y(0), z(0) {}
	Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vector2f {
	union {
		struct
		{
			float x, y;
		};
		float v[2];
	};
	Vector2f() : x(0), y(0) {}
	Vector2f(float x, float y) : x(x), y(y){}

	Vector2f operator+(const Vector2f b) {
		return Vector2f(x + b.x, y + b.y);
	}

	Vector2f operator-(const Vector2f b) {
		return Vector2f(x - b.x, y - b.y);
	}

	Vector2f operator*(const float s) {
		return Vector2f(x * s, y * s);
	}

	float squared() {
		return x * x + y * y;
	}

	float length() {
		return sqrtf(squared());
	}

	Vector2f normalized() {
		float s = Q_rsqrt(squared());
		return Vector2f(x * s, y * s);
	}

	Vector2f rotate(float radiam) {
		return Vector2f(x * cosf(radiam) + y * sinf(radiam), x * -sinf(radiam) + y * cosf(radiam));
	}
};

#pragma pack(push, 1)
struct Color {
	union {
		struct
		{
			unsigned char r, g, b, a;
		};
		unsigned char v[4];
	};
	Color() {}
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
};
#pragma pack(pop)