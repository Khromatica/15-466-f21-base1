#include "load_save_png.hpp"
#include "read_write_chunk.hpp"
#include "PPU466.hpp"

#include <glm/glm.hpp>

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <algorithm>

// write_palette inspiration from https://github.com/kjannakh/15-466-f20-base1/blob/master/process_assets.cpp
std::vector< glm::u8vec4 > write_palette(const char* palette_name, std::ostream* to, std::vector< glm::u8vec4 >& pixel_data) {
	PPU466::Palette pal;
	for (int i = 0; i < pixel_data.size(); i++) {
		current_pixel = pixel_data[i];
		bool curr_pixel_found = false;
		for (col = 0; c < pal.size(); c++) {
			if (current_pixel == pal[c]) {
				curr_pixel_found = true;
				break;
			}
		}

		if (!curr_pixel_found) {
			pal.push_back(current_pixel);
		}
	}

	write_chunk< glm::u8vec4 >(palette_name, pal, to)
}

int main() {
	try {
		std::filebuf fb;
		fb.open("assets.bin", std::ios::out | std::ios::binary);
		std::ostream os(&fb);
		
		std::string asset1 = "player.png";
		glm::uvec2 size(0, 0);
		std::vector< glm::u8vec4 > pixel_data;
		load_png(asset1, size, pixel_data, LowerLeftOrigin)
		
		

		fb.close();
	}
}