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

// write_tile inspiration from https://github.com/Chipxiang/Jump-Guy/blob/master/asset_converter.cpp
PPU466::Tile write_tile(const char* tile0_name, const char* tile1_name, const PPU466::Palette palette, std::ostream* to, const std::vector<glm::u8vec4>& pixel_data) {
	assert(pixel_data.size() == TILE_WIDTH * TILE_HEIGHT);
	PPU466::Tile tile{};

	for (int i = 0; i < TILE_HEIGHT; i++) {
		for (int j = 0; j < TILE_WIDTH; j++) {
			// pixel at (j, i)
			glm::u8vec4 color = pixel_data[i * TILE_WIDTH + j];
			auto idx = (size_t)(std::find(palette.begin(), palette.end(), color) - palette.begin());

			assert(idx < palette.size());
			tile.bit0[i] |= ((idx & 1) << j);
			tile.bit1[i] |= (((idx & 2) >> 1) << j);
		}
	}
	
	write_chunk< glm::uint8_t >(tile0_name, tile.bit0, to);
	write_chunk< glm::uint8_t >(tile1_name, tile.bit1, to);
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
