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
	std::vector< glm::u8vec4 > pal;
	for (int i = 0; i < pixel_data.size(); i++) {
		auto current_pixel = pixel_data[i];
		bool curr_pixel_found = false;
		for (unsigned int col = 0; col < pal.size(); col++) {
			if (current_pixel == pal[col]) {
				curr_pixel_found = true;
				break;
			}
		}

		if (!curr_pixel_found) {
			pal.push_back(current_pixel);
		}
	}

	write_chunk< glm::u8vec4 >(palette_name, pal, to);
	return pal;
}

// write_tile inspiration from https://github.com/Chipxiang/Jump-Guy/blob/master/asset_converter.cpp
void write_tile(const char* tile0_name, const char* tile1_name, std::vector< glm::u8vec4 > palette, std::ostream* to, const std::vector<glm::u8vec4>& pixel_data) {
	assert(pixel_data.size() == 8 * 8);
	PPU466::Tile tile{};

	std::vector< uint8_t > bit0;
	std::vector< uint8_t > bit1;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// pixel at (j, i)
			glm::u8vec4 color = pixel_data[i * 8 + j];
			auto idx = (size_t)(std::find(palette.begin(), palette.end(), color) - palette.begin());

			assert(idx < palette.size());
			bit0[i] |= ((idx & 1) << j);
			bit1[i] |= (((idx & 2) >> 1) << j);
		}
	}
	
	write_chunk< glm::uint8_t >(tile0_name, bit0, to);
	write_chunk< glm::uint8_t >(tile1_name, bit1, to);
}

int main(int argc, char** argv) {
	try {
		std::filebuf fb;
		fb.open("assets.bin", std::ios::out | std::ios::binary);
		std::ostream os(&fb);
		
		std::string asset1 = "player.png";
		glm::uvec2 size(0, 0);
		std::vector< glm::u8vec4 > pixel_data;
		load_png(asset1, &size, &pixel_data, LowerLeftOrigin);

		std::vector< glm::u8vec4 > player_palette = write_palette("ppal", &os, pixel_data);
		write_tile("pla1", "pla2", player_palette, &os, pixel_data);

		fb.close();

		return 0;
	} catch (std::exception const& e) {
		std::cerr << "Unhandled exception:\n" << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unhandled exception (unknown type)." << std::endl;
		throw;
	}
}
