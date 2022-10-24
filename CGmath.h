#pragma once

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
	Color() : r(0), g(0), b(0), a(255) {}
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
};
#pragma pack(pop)