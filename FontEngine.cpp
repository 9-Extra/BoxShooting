#include "FontEngine.h"

FontEngine::FontEngine() {
	if (FT_Init_FreeType(&library)) {
		SysError(L"Failed to init freetype library");
	}

	if (FT_New_Face(library, "resources\\font\\LLtype.ttf", 0, &face)) {
		SysError(L"Failed to load font");
	}

	debug_log("Loaded font \"%s\"\n", face->family_name);

	if (FT_Set_Pixel_Sizes(face, 0, 128)) {
		SysError(L"Failed to set font size");
	}

	if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
		SysError(L"Failed to select charset");
	}
#ifndef NDEBUG
	load_char(L'A');
	assert(face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY);
#endif // !NDEBUG
}
