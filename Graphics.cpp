#include "Graphics.h"

void Graphcis::clear(Color color) {
	std::fill(panel.begin(), panel.end(), color);
}
void Graphcis::draw_point(Vector2f p, Color color) {
	int center_x = (int)(p.x / MAP_RATIO);
	int center_y = (int)(p.y / MAP_RATIO);
	if (center_x >= 0 && center_x < WINDOW_WIDTH && center_y >= 0 && center_y < WINDOW_HEIGHT) {
		panel[center_y * WINDOW_WIDTH + center_x] = color;
	}
}
void Graphcis::draw_box(Vector2f p, int width, int height, Color color) {
	//debug_log("Drawing box at (%f, %f)\n", x, y);
	int center_x = (int)(p.x / MAP_RATIO);
	int center_y = (int)(p.y / MAP_RATIO);
	int x_start = std::max<int>(0, center_x - width / 2);
	int y_start = std::max<int>(0, center_y - height / 2);
	int x_end = std::min<int>(WINDOW_WIDTH - 1, center_x + width / 2);
	int y_end = std::min<int>(WINDOW_HEIGHT - 1, center_y + height / 2);
	for (int y = y_start; y < y_end; y++) {
		for (int x = x_start; x < x_end; x++) {
			panel[y * WINDOW_WIDTH + x] = color;
		}
	}

}

void Graphcis::draw_texture_upleft(Vector2f p, const Texture& texture) {
	int left = (int)(p.x / MAP_RATIO);
	int up = (int)(p.y / MAP_RATIO);

	//todo
}

void Graphcis::draw_text_upleft(int up, int left, Color color,const wchar_t* str) {
	int pen_x = up;
	int pen_y = left;
	for (const wchar_t* c = str; *c != L'\0'; c++) {
		FT_GlyphSlot slot = font_engine.load_char(*c);
		unsigned int width = slot->bitmap.width;
		unsigned int height = slot->bitmap.rows;
		
		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				int tar_x = pen_x + x;
				int tar_y = pen_y + y;
				if (tar_x >= 0 && tar_x < WINDOW_WIDTH && tar_y >= 0 && tar_y < WINDOW_HEIGHT) {
					unsigned char gray = slot->bitmap.buffer[y * width + x];
					if (gray > 0) {
						Color& ori = panel[tar_y * WINDOW_WIDTH + tar_x];
						color.a = gray;
						ori = ori.blend(color);
					}
				}

			}
		}
		
		pen_x += slot->advance.x / 64;
		pen_y += slot->advance.y;

	}
}

void Graphcis::draw_texture_center(Vector2f p, const Texture& texture) {
#ifndef NDEBUG
	if (texture.pixels.size() == 0) {
		GameError(L"Use unloaded texture");
	}
#endif // !NDEBUG
	unsigned int width = texture.width;
	unsigned int height = texture.height;
	int center_x = (int)(p.x / MAP_RATIO);
	int center_y = (int)(p.y / MAP_RATIO);
	
	int tar_x_start = center_x - width / 2;
	int tar_y_start = center_y - height / 2;

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			int tar_x = tar_x_start + x;
			int tar_y = tar_y_start + y;
			if (tar_x >= 0 && tar_x < WINDOW_WIDTH && tar_y >= 0 && tar_y < WINDOW_HEIGHT) {
				Color pixel = texture.pixels[y * width + x];
				if (pixel.a != 0) {
					panel[tar_y * WINDOW_WIDTH + tar_x] = pixel;
				}
			}
	
		}
	}


}