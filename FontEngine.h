#pragma once

#include <freetype/freetype.h>
#include "Error.h"

class FontEngine {
private:
	FT_Library library;
	FT_Face face;

public:
	FontEngine();

	FT_GlyphSlot load_char(wchar_t c) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			debug_log("Failed to load char");
		}
		return face->glyph;
	}
};
