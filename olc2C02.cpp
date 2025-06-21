#include "olc2C02.h"

olc2C02::olc2C02() {
    palScreen[0x00] = olc::Pixel(84, 84, 84);
    palScreen[0x01] = olc::Pixel(0, 30, 116);
    palScreen[0x02] = olc::Pixel(8, 16, 144);
    palScreen[0x03] = olc::Pixel(48, 0, 136);
    palScreen[0x04] = olc::Pixel(68, 0, 100);
    palScreen[0x05] = olc::Pixel(92, 0, 48);
    palScreen[0x06] = olc::Pixel(84, 4, 0);
    palScreen[0x07] = olc::Pixel(60, 24, 0);
    palScreen[0x08] = olc::Pixel(32, 42, 0);
    palScreen[0x09] = olc::Pixel(8, 58, 0);
    palScreen[0x0A] = olc::Pixel(0, 64, 0);
    palScreen[0x0B] = olc::Pixel(0, 60, 0);
    palScreen[0x0C] = olc::Pixel(0, 50, 60);
    palScreen[0x0D] = olc::Pixel(0, 0, 0);
    palScreen[0x0E] = olc::Pixel(0, 0, 0);
    palScreen[0x0F] = olc::Pixel(0, 0, 0);

    palScreen[0x10] = olc::Pixel(152, 150, 152);
    palScreen[0x11] = olc::Pixel(8, 76, 196);
    palScreen[0x12] = olc::Pixel(48, 50, 236);
    palScreen[0x13] = olc::Pixel(92, 30, 228);
    palScreen[0x14] = olc::Pixel(136, 20, 176);
    palScreen[0x15] = olc::Pixel(160, 20, 100);
    palScreen[0x16] = olc::Pixel(152, 34, 32);
    palScreen[0x17] = olc::Pixel(120, 60, 0);
    palScreen[0x18] = olc::Pixel(84, 90, 0);
    palScreen[0x19] = olc::Pixel(40, 114, 0);
    palScreen[0x1A] = olc::Pixel(8, 124, 0);
    palScreen[0x1B] = olc::Pixel(0, 118, 40);
    palScreen[0x1C] = olc::Pixel(0, 102, 120);
    palScreen[0x1D] = olc::Pixel(0, 0, 0);
    palScreen[0x1E] = olc::Pixel(0, 0, 0);
    palScreen[0x1F] = olc::Pixel(0, 0, 0);

    palScreen[0x20] = olc::Pixel(236, 238, 236);
    palScreen[0x21] = olc::Pixel(76, 154, 236);
    palScreen[0x22] = olc::Pixel(120, 124, 236);
    palScreen[0x23] = olc::Pixel(176, 98, 236);
    palScreen[0x24] = olc::Pixel(228, 84, 236);
    palScreen[0x25] = olc::Pixel(236, 88, 180);
    palScreen[0x26] = olc::Pixel(236, 106, 100);
    palScreen[0x27] = olc::Pixel(212, 136, 32);
    palScreen[0x28] = olc::Pixel(160, 170, 0);
    palScreen[0x29] = olc::Pixel(116, 196, 0);
    palScreen[0x2A] = olc::Pixel(76, 208, 32);
    palScreen[0x2B] = olc::Pixel(56, 204, 108);
    palScreen[0x2C] = olc::Pixel(56, 180, 204);
    palScreen[0x2D] = olc::Pixel(60, 60, 60);
    palScreen[0x2E] = olc::Pixel(0, 0, 0);
    palScreen[0x2F] = olc::Pixel(0, 0, 0);

    palScreen[0x30] = olc::Pixel(236, 238, 236);
    palScreen[0x31] = olc::Pixel(168, 204, 236);
    palScreen[0x32] = olc::Pixel(188, 188, 236);
    palScreen[0x33] = olc::Pixel(212, 178, 236);
    palScreen[0x34] = olc::Pixel(236, 174, 236);
    palScreen[0x35] = olc::Pixel(236, 174, 212);
    palScreen[0x36] = olc::Pixel(236, 180, 176);
    palScreen[0x37] = olc::Pixel(228, 196, 144);
    palScreen[0x38] = olc::Pixel(204, 210, 120);
    palScreen[0x39] = olc::Pixel(180, 222, 120);
    palScreen[0x3A] = olc::Pixel(168, 226, 144);
    palScreen[0x3B] = olc::Pixel(152, 226, 180);
    palScreen[0x3C] = olc::Pixel(160, 214, 228);
    palScreen[0x3D] = olc::Pixel(160, 162, 160);
    palScreen[0x3E] = olc::Pixel(0, 0, 0);
    palScreen[0x3F] = olc::Pixel(0, 0, 0);

    sprScreen = new olc::Sprite(256, 240);
    sprNameTable[0] = new olc::Sprite(256, 240);
    sprNameTable[1] = new olc::Sprite(256, 240);
    sprPatternTable[0] = new olc::Sprite(128, 128);
    sprPatternTable[1] = new olc::Sprite(128, 128);
}

olc2C02::~olc2C02() {
    delete sprScreen;
	delete sprNameTable[0];
	delete sprNameTable[1];
	delete sprPatternTable[0];
	delete sprPatternTable[1];
}

olc::Sprite& olc2C02::GetScreen()
{
	return *sprScreen;
}

olc::Sprite & olc2C02::GetNameTable(uint8_t i)
{
	return *sprNameTable[i];
}

olc::Sprite & olc2C02::GetPatternTable(uint8_t i, uint8_t palette)
{
	// create 16 * 16 pattern tiles
	for (uint16_t nTileY{} ; nTileY < 16; nTileY++)
	{
		for (uint16_t nTileX{} ; nTileX < 16; nTileX++)
		{
			// converting the 2D coordinates to 1D coordinates
			// to find the color coordinate

			uint16_t nOffset = nTileY * 256 + nTileX * 16;

			for (uint16_t row{} ; row < 8; row++)
			{
				uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000);
				uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 0x0008);

				for (uint16_t col{} ; col < 8; col++)
				{
					uint8_t pixel = (tile_lsb & 0x01) << 1 | (tile_msb & 0x01);
					tile_lsb >>= 1;
					tile_msb >>= 1;

					// to draw this pixel value into the sprite that represents this
					// section of the pattern memory

					sprPatternTable[i]->SetPixel
					(
						nTileX * 8 + (7 - col), 
						nTileY * 8 + row, 
						GetColourFromPaletteRam(palette, pixel)
					);
				}
			}
		}
	}

	// Finally return the updated sprite representing the pattern table
	return *sprPatternTable[i];
}

olc::Pixel& olc2C02::GetColourFromPaletteRam(uint8_t palette, uint8_t pixel) {
	return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
	// we multiply the palette id by 4, add it with the pixel id and offset this value to the address range
}

uint8_t olc2C02::cpuRead (uint16_t addr, bool rdonly) {
    // the cpu can only address 8 locations on the ppu
    uint8_t data = 0x00;

	if (rdonly) {
		switch (addr) {
			case 0x0000: //control
				data = control.reg;
				break;
			case 0x0001: //mask
				data = mask.reg;
				break;
			case 0x0002: //status
				data = status.reg;
				break;
			case 0x0003: // oam address
				break;
			case 0x0004: //oam data
				break;
			case 0x0005: //scroll
				break;
			case 0x0006: //ppu address
				break;
			case 0x0007: //ppu data
				break;
		}
	}
    else {
		switch (addr) {
			case 0x0000:
				break;
			case 0x0001:
				break;
			case 0x0002:
				data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
				status.vertical_blank = 0;
				address_latch = 0;
				break;
			case 0x0003:
				break;
			case 0x0004:
				data = pOAM[oam_addr];
				break;
			case 0x0005:
				break;
			case 0x0006:
				break;
			case 0x0007:
				data = ppu_data_buffer;
				ppu_data_buffer = ppuRead(vram_addr.reg);
	
				if (vram_addr.reg >= 0x3F00) data = ppu_data_buffer;
				vram_addr.reg += (control.increment_mode ? 32 : 1);
				break;
		}
	}

    return data;
}

void olc2C02::cpuWrite(uint16_t addr, uint8_t data) {

    switch (addr) {
        case 0x0000: //control
			control.reg = data;
			tram_addr.nametable_x = control.nametable_x;
			tram_addr.nametable_y = control.nametable_y;
            break;
        case 0x0001: //mask
			mask.reg = data;
            break;
        case 0x0002: //status
            break;
        case 0x0003: // oam address
			oam_addr = data;
            break;
        case 0x0004: //oam data
			pOAM[oam_addr] = data;
            break;
        case 0x0005: //scroll
			if (address_latch == 0) {
				fine_x = data & 0x07;
				tram_addr.coarse_x = data >> 3;
				address_latch = 1;
			}
			else {
				tram_addr.fine_y = data & 0x07;
				tram_addr.coarse_y = data >> 3;
				address_latch = 0;
			}
            break;
        case 0x0006: //ppu address
			if (address_latch == 0) {
				tram_addr.reg = (uint16_t)(((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF));
				address_latch = 1;
			}
			else {
				tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
				vram_addr = tram_addr;
				address_latch = 0;
			}
            break;
        case 0x0007: //ppu data
			ppuWrite(vram_addr.reg, data);
			vram_addr.reg += (control.increment_mode ? 32 : 1);
            break;
    }
}

uint8_t olc2C02::ppuRead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    if (cart->ppuRead(addr, data)) {

    }
	else if (addr >= 0x0000 && addr <= 0x1FFF) { // pattern memory
		data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF) { // name table memory
		addr &= 0x0FFF;
		if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
			// vertical mirroring mode
			if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[1][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
		}
		else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
			// horizontal mirroring
			if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[0][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[1][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF) { // palette memory
		addr &= 0x001F; // masking the bottom five bits

		// hard coding the mirroring
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;

		// read directly from that memory location
		data = tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
	}

    return data;
}

void olc2C02::ppuWrite(uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;

    if (cart->ppuWrite(addr, data)) {

    }
	else if (addr >= 0x0000 && addr <= 0x1FFF) { // pattern memory
		tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data; // (addr & 0x1000) choose the left or right array
																// the offset into that memory if calculated by offsetting the remaining bits of the ppu address 
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF) { // name table memory
		addr &= 0x0FFF;
		if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
			// vertical mirroring mode
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
		else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
			// horizontal mirroring
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF) { // palette memory
		addr &= 0x001F; // masking the bottom five bits
		
		// hard coding the mirroring
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;

		tblPalette[addr] = data;
	}

}

void olc2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
}

void olc2C02::reset()
{
	fine_x = 0x00;
	address_latch = 0x00;
	ppu_data_buffer = 0x00;
	scan_line = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	vram_addr.reg = 0x0000;
	tram_addr.reg = 0x0000;
}

void olc2C02::clock() {

	auto IncrementScrollX = [&]() {
		if (mask.render_background || mask.render_sprites) {
			// A single name table is 32x30 tiles. As we increment horizontally
			// we may cross into a neighboring nametable, or wrap around to
			// a neighboring nametable
			if (vram_addr.coarse_x == 31) {
				// Leaving nametable so wrap address round
				vram_addr.coarse_x = 0;
				// Flip target nametable bit
				vram_addr.nametable_x = ~vram_addr.nametable_x;
			}
			else {
				// Staying in current nametable, so just increment
				vram_addr.coarse_x++;
			}
		}
	};

	auto IncrementScrollY = [&]() {
		if (mask.render_background || mask.render_sprites) {
			// If possible, just increment the fine y offset
			if (vram_addr.fine_y < 7) {
				vram_addr.fine_y++;
			}
			else {
				// If we have gone beyond the height of a row, we need to
				// increment the row, potentially wrapping into neighbouring
				// vertical nametable. Don't forget however, the bottom two rows
				// do not contain tile information. The coarse y offset is used
				// to identify which row of the nametable we want, and the fine
				// y offset is the specific 'scanline'

				// Reset fine y offset
				vram_addr.fine_y = 0;

				// Check if we need to swap vertical nametable targets
				if (vram_addr.coarse_y == 29) {
					// reset coarse y offset
					vram_addr.coarse_y = 0;
					// flip the target nametable bit
					vram_addr.nametable_y = ~vram_addr.nametable_y;
				}
				else if (vram_addr.coarse_y == 31) {
					// if pointer is in the attribute memory, we just wrap around the current nametable
					vram_addr.coarse_y = 0;
				}
				else {
					// increment the coarse y offset
					vram_addr.coarse_y++;
				}
			}
		}
	};

	auto TransferAddressX = [&]() {
		// only if rendering is enabled
		if (mask.render_background || mask.render_sprites) {
			vram_addr.nametable_x = tram_addr.nametable_x;
			vram_addr.coarse_x = tram_addr.coarse_x;
		}
	};

	auto TransferAddressY = [&]() {
		if (mask.render_background || mask.render_sprites) {
			vram_addr.fine_y = tram_addr.fine_y;
			vram_addr.nametable_y = tram_addr.nametable_y;
			vram_addr.coarse_y = tram_addr.coarse_y;
 		}
	};

	auto LoadBackgroundShifters = [&]() {
		bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
		bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

		bg_shifter_attrib_lo  = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
		bg_shifter_attrib_hi  = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
	};

	auto UpdateShifters = [&]() {
		if (mask.render_background) {
			bg_shifter_pattern_lo <<= 1;
			bg_shifter_pattern_hi <<= 1;
			bg_shifter_attrib_lo <<= 1;
			bg_shifter_attrib_hi <<= 1;
		}

		if (mask.render_sprites && cycle >= 1 && cycle < 258) {
			for (int i = 0 ; i < sprite_count ; i++) {
				if (spriteScanLine[i].x > 0) {
					spriteScanLine[i].x--;
				}
				else {
					sprite_shifter_pattern_lo[i] <<= 1;
					sprite_shifter_pattern_hi[i] <<= 1;
				}
			}
		}
	};

	if (scan_line >= -1 && scan_line < 240) {
		if (scan_line == 0 && cycle == 0) {
			cycle = 1;
		}

		if (scan_line == -1 && cycle == 1) {
			status.vertical_blank = 0; // effectively start of a new frame so clear vertical blank flag
			status.sprite_overflow = 0; // clear sprite overflow flag
			status.sprite_zero_hit = 0; // clear the sprite zero hit flag
			// clear shifters
			for (int i = 0 ; i < 8 ; i++) {
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
		}

		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {
			UpdateShifters();
			switch ((cycle - 1) % 8) {
				case 0:
					LoadBackgroundShifters();
					bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
					break;
				case 2:
					bg_next_tile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11)
						| (vram_addr.nametable_x << 10)
						| ((vram_addr.coarse_y >> 2) << 3)
						| (vram_addr.coarse_x >> 2));
					if (vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
					if (vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
					bg_next_tile_attrib &= 0x03;
					break;
				case 4:
					bg_next_tile_lsb = ppuRead((control.pattern_background << 12)
						+ ((uint16_t)bg_next_tile_id << 4)
						+ (vram_addr.fine_y) + 0);
					break;
				case 6:
					bg_next_tile_msb = ppuRead((control.pattern_background << 12)
						+ ((uint16_t)bg_next_tile_id << 4)
						+ (vram_addr.fine_y) + 8);
					break;
				case 7:
					IncrementScrollX();
					break;
			}
		}

		if (cycle == 256) {
			IncrementScrollY();
		}

		if (cycle == 257) {
			LoadBackgroundShifters();
			TransferAddressX();
		}

		if (cycle == 338 || cycle == 340) {
			bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
		}

		if (scan_line == -1 && cycle >= 280 && cycle < 305) {
			TransferAddressY();
		}

		// Foreground rendering ---------------------------------

		if (cycle == 257 && scan_line >= 0) {
			std::memset(spriteScanLine, 0xFF, 8 * sizeof(sObjectAttributeEntry));
			sprite_count = 0;

			// for (uint8_t i = 0 ; i < 8 ; i++) {
			// 	sprite_shifter_pattern_lo[i] = 0;
			// 	sprite_shifter_pattern_hi[i] = 0;
			// }

			uint8_t nOAMEntry = 0;
			bSpriteZeroHitPossible = false;
			while (nOAMEntry < 64 && sprite_count < 9) {
				int16_t diff = ((int16_t)scan_line - (int16_t)OAM[nOAMEntry].y);
				if (diff >= 0 && diff < (control.sprite_size ? 16 : 8)) {
					if (sprite_count < 8) {

						// to check if this sprite is sprite zero
						if (nOAMEntry == 0) {
							// it is, so it can trigger a sprite zero hit when drawn
							bSpriteZeroHitPossible = true;
						}

						memcpy(&spriteScanLine[sprite_count], &OAM[nOAMEntry], sizeof(sObjectAttributeEntry));
						sprite_count++;
					}
				}
				nOAMEntry++;
			}
			status.sprite_overflow = (sprite_count > 8);
		}

		// to find relevant data from pattern memory
		if (cycle == 340) {
			for (uint8_t i = 0 ; i < sprite_count ; i++) {
				uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
				uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;

				if (!control.sprite_size) {
					// 8x8 sprite mode - the control register determined the pattern table
					if (!(spriteScanLine[i].attribute & 0x80)) {
						// sprite is not flipped vertically i.e. normal
						sprite_pattern_addr_lo = 
							(control.pattern_sprite << 12)
							| (spriteScanLine[i].id << 4)
							| (scan_line - spriteScanLine[i].y);
					}
					else {
						// sprite is flipped vertically i.e. upside down
						sprite_pattern_addr_lo = 
							(control.pattern_sprite << 12)
							| (spriteScanLine[i].id << 4)
							| (7 - (scan_line - spriteScanLine[i].y));
					}
				}
				else {
					// 8x16 sprite mode - the sprite attribute determines the patter table
					if (!(spriteScanLine[i].attribute & 0x80)) {
						// sprite is not flipped vertically i.e. normal
						if (scan_line - spriteScanLine[i].y < 8) {
							// to read the top half tile
							sprite_pattern_addr_lo = 
								((spriteScanLine[i].id & 0x01) << 12)
								| ((spriteScanLine[i].id & 0xFE) << 4)
								| ((scan_line - spriteScanLine[i].y) & 0x07);
						}
						else {
							// to read the bottom half tile
							sprite_pattern_addr_lo = 
								((spriteScanLine[i].id & 0x01) << 12)
								| (((spriteScanLine[i].id & 0xFE) + 1) << 4)
								| ((scan_line - spriteScanLine[i].y) & 0x07);
						}
					}
					else {
						// sprite is flipped vertically i.e. upside down
						if (scan_line - spriteScanLine[i].y < 8) {
							// reading top half file
							sprite_pattern_addr_lo = 
								((spriteScanLine[i].id & 0x01) << 12)
								| (((spriteScanLine[i].id & 0xFE) + 1) << 4)
								| (7 - (scan_line - spriteScanLine[i].y) & 0x07);
						}
						else {
							// Reading Bottom Half Tile
							sprite_pattern_addr_lo = 
							  ((spriteScanLine[i].id & 0x01) << 12)    // Which Pattern Table? 0KB or 4KB offset
							| ((spriteScanLine[i].id & 0xFE) << 4 )    // Which Cell? Tile ID * 16 (16 bytes per tile)
							| (7 - (scan_line - spriteScanLine[i].y) & 0x07); // Which Row in cell? (0->7)
						}
					}
				}

				sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
				sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
				sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);

				// in case sprite is flipped horizontally
				if (spriteScanLine[i].attribute & 0x40) {
					auto flipbyte = [](uint8_t b) {
						b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
						b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
						b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
						return b;
					};

					// flip patterns horizontally
					sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
					sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);

				}

				sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
				sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;

			}
		}

	}

	if (scan_line == 240) {
		// post render scanline
	}

	if (scan_line >= 241 && scan_line < 261) {
		if (scan_line == 241 && cycle == 1) {
			status.vertical_blank = 1;
			if (control.enable_nmi) {
				nmi = true;
			}
		}
	}
	
	// to render background pixels
	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;

	if (mask.render_background) {
		uint16_t bit_mux = 0x8000 >> fine_x;

		uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
		uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;
		bg_pixel = (p1_pixel << 1) | p0_pixel;

		uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
		uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}

	uint8_t fg_pixel = 0x00;
	uint8_t fg_palette = 0x00;
	uint8_t fg_priority = 0x00;

	if (mask.render_sprites) {

		bSpriteZeroBeingRendered = false;

		for (uint8_t i = 0 ; i < sprite_count ; i++) {
			if (spriteScanLine[i].x == 0) {
				uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
				uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

				fg_palette = (spriteScanLine[i].attribute & 0x03) + 0x04;
				fg_priority = (spriteScanLine[i].attribute & 0x20) == 0;

				if (fg_pixel != 0) {
					if (i == 0) {
						bSpriteZeroBeingRendered = true;
					}
					break;
				}
			}
		}
	}

	uint8_t pixel = 0x00; // the final pixel
	uint8_t palette = 0x00; // the final palette

	if (bg_pixel == 0 && fg_pixel == 0) {
		// the background and foreground pixel is transparent
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bg_pixel == 0 && fg_pixel > 0) {
		// background is transparent and foreground is visible
		pixel = fg_pixel;
		palette = fg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel == 0) {
		// background is visible and foreground is transparent
		pixel = bg_pixel;
		palette = bg_palette;
	}
	else if (bg_pixel > 0 && fg_pixel > 0) {

		// bg and fg are visible
		if (fg_priority) {
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else {
			pixel = bg_pixel;
			palette = bg_palette;
		}

		if (bSpriteZeroHitPossible && bSpriteZeroBeingRendered) {
			if (mask.render_background & mask.render_sprites) {
				if (~(mask.render_background_left | mask.render_sprites_left)) {
					if (cycle >= 9 || cycle < 258) {
						status.sprite_zero_hit = 1;
					}
				}
				// else {
				// 	if (cycle >= 1 && cycle < 258) {
				// 		status.sprite_zero_hit = 1;
				// 	}
				// }
			}
		}
	}

    // Fake some noise for now
    sprScreen->SetPixel(cycle - 1, scan_line, GetColourFromPaletteRam(palette, pixel));

    // Advance rendered - it never stops
    cycle++;

    if (cycle >= 341) {
        cycle = 0;
        scan_line++;
        if (scan_line >= 261) {
            scan_line = -1;
            frame_complete = true;
        }
    }
}


