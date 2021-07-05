//
// Created by hwf on 2021/6/4.
//

#include "ppu.h"
#include "../util.h"

namespace th {

    uint8_t PPU::cpuRead(uint16_t addr, bool readOnly) {
        return this->bridge.read(addr, readOnly);
    }

/**
 * Cpu Write is strange. because it's seems like that cpu write
 * to some it's self. address.
 * @param addr
 * @param data
 */
    void PPU::cpuWrite(uint16_t addr, uint8_t data) {
        this->bridge.write(addr, data);
    }

    uint8_t PPU::ppuRead(uint16_t addr, bool readOnly) {
        uint8_t data = 0x00;
        if (cart->ppuRead(addr, data)) {
        } else if (addr >= nameTables.addr_min && addr <= nameTables.addr_max) {
            data = nameTables.read(addr);
        } else if (addr >= palette.addr_min && addr <= palette.addr_max) {
            data = palette.read(addr, mask.grayscale);
        }
        // should throw here?
        return data;
    }

    static int count = 0;

    void PPU::ppuWrite(uint16_t addr, uint8_t data) {
        // why &= 0x3FFF
        addr &= 0x3FFF;
        if (cart->ppuWrite(addr, data)) {

        } else if (addr >= nameTables.addr_min && addr <= nameTables.addr_max) {
            nameTables.write(addr, data);
        } else if (addr >= palette.mirror_min && addr <= palette.mirror_max) {
            // why some strange value??
            palette.write(addr, data);
        }
        // should throw here?
    }

    void PPU::connectCartridge(const std::shared_ptr<Cartridge> &cartridge) {
        this->cart = cartridge;
    }

    olc::Sprite *PPU::GetScreen() {
        return &sprScreen;
    }

    void PPU::clock() {
        // As we progress through scanlines and cycles, the PPU is effectively
        // a state machine going through the motions of fetching background
        // information and sprite information, compositing them into a pixel
        // to be output.

        // The lambda functions (functions inside functions) contain the various
        // actions to be performed depending upon the output of the state machine
        // for a given scanline/cycle combination

        // ==============================================================================
        // Increment the background tile "pointer" one tile/column horizontally
        auto IncrementScrollX = [&]()
        {
            // Note: pixel perfect scrolling horizontally is handled by the
            // data shifters. Here we are operating in the spatial domain of
            // tiles, 8x8 pixel blocks.

            // Ony if rendering is enabled
            if (mask.render_background || mask.render_sprites)
            {
                // A single name table is 32x30 tiles. As we increment horizontally
                // we may cross into a neighbouring nametable, or wrap around to
                // a neighbouring nametable
                if (vram_addr.coarse_x == 31)
                {
                    // Leaving nametable so wrap address round
                    vram_addr.coarse_x = 0;
                    // Flip target nametable bit
                    vram_addr.nametable_x = ~vram_addr.nametable_x;
                }
                else
                {
                    // Staying in current nametable, so just increment
                    vram_addr.coarse_x++;
                }
            }
        };

        // ==============================================================================
        // Increment the background tile "pointer" one scanline vertically
        auto IncrementScrollY = [&]()
        {
            // Incrementing vertically is more complicated. The visible nametable
            // is 32x30 tiles, but in memory there is enough room for 32x32 tiles.
            // The bottom two rows of tiles are in fact not tiles at all, they
            // contain the "attribute" information for the entire table. This is
            // information that describes which palettes are used for different
            // regions of the nametable.

            // In addition, the NES doesnt scroll vertically in chunks of 8 pixels
            // i.e. the height of a tile, it can perform fine scrolling by using
            // the fine_y component of the register. This means an increment in Y
            // first adjusts the fine offset, but may need to adjust the whole
            // row offset, since fine_y is a value 0 to 7, and a row is 8 pixels high

            // Ony if rendering is enabled
            if (mask.render_background || mask.render_sprites)
            {
                // If possible, just increment the fine y offset
                if (vram_addr.fine_y < 7)
                {
                    vram_addr.fine_y++;
                }
                else
                {
                    // If we have gone beyond the height of a row, we need to
                    // increment the row, potentially wrapping into neighbouring
                    // vertical nametables. Dont forget however, the bottom two rows
                    // do not contain tile information. The coarse y offset is used
                    // to identify which row of the nametable we want, and the fine
                    // y offset is the specific "scanline"

                    // Reset fine y offset
                    vram_addr.fine_y = 0;

                    // Check if we need to swap vertical nametable targets
                    if (vram_addr.coarse_y == 29)
                    {
                        // We do, so reset coarse y offset
                        vram_addr.coarse_y = 0;
                        // And flip the target nametable bit
                        vram_addr.nametable_y = ~vram_addr.nametable_y;
                    }
                    else if (vram_addr.coarse_y == 31)
                    {
                        // In case the pointer is in the attribute memory, we
                        // just wrap around the current nametable
                        vram_addr.coarse_y = 0;
                    }
                    else
                    {
                        // None of the above boundary/wrapping conditions apply
                        // so just increment the coarse y offset
                        vram_addr.coarse_y++;
                    }
                }
            }
        };

        // ==============================================================================
        // Transfer the temporarily stored horizontal nametable access information
        // into the "pointer". Note that fine x scrolling is not part of the "pointer"
        // addressing mechanism
        auto TransferAddressX = [&]()
        {
            // Ony if rendering is enabled
            if (mask.render_background || mask.render_sprites)
            {
                vram_addr.nametable_x = tram_addr.nametable_x;
                vram_addr.coarse_x    = tram_addr.coarse_x;
            }
        };

        // ==============================================================================
        // Transfer the temporarily stored vertical nametable access information
        // into the "pointer". Note that fine y scrolling is part of the "pointer"
        // addressing mechanism
        auto TransferAddressY = [&]()
        {
            // Ony if rendering is enabled
            if (mask.render_background || mask.render_sprites)
            {
                vram_addr.fine_y      = tram_addr.fine_y;
                vram_addr.nametable_y = tram_addr.nametable_y;
                vram_addr.coarse_y    = tram_addr.coarse_y;
            }
        };


        // ==============================================================================
        // Prime the "in-effect" background tile shifters ready for outputting next
        // 8 pixels in scanline.
        auto LoadBackgroundShifters = [&]()
        {
            // Each PPU update we calculate one pixel. These shifters shift 1 bit along
            // feeding the pixel compositor with the binary information it needs. Its
            // 16 bits wide, because the top 8 bits are the current 8 pixels being drawn
            // and the bottom 8 bits are the next 8 pixels to be drawn. Naturally this means
            // the required bit is always the MSB of the shifter. However, "fine x" scrolling
            // plays a part in this too, whcih is seen later, so in fact we can choose
            // any one of the top 8 bits.
            bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
            bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

            // Attribute bits do not change per pixel, rather they change every 8 pixels
            // but are synchronised with the pattern shifters for convenience, so here
            // we take the bottom 2 bits of the attribute word which represent which
            // palette is being used for the current 8 pixels and the next 8 pixels, and
            // "inflate" them to 8 bit words.
            bg_shifter_attrib_lo  = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
            bg_shifter_attrib_hi  = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
        };


        // ==============================================================================
        // Every cycle the shifters storing pattern and attribute information shift
        // their contents by 1 bit. This is because every cycle, the output progresses
        // by 1 pixel. This means relatively, the state of the shifter is in sync
        // with the pixels being drawn for that 8 pixel section of the scanline.
        auto UpdateShifters = [&]()
        {
            if (mask.render_background)
            {
                // Shifting background tile pattern row
                bg_shifter_pattern_lo <<= 1;
                bg_shifter_pattern_hi <<= 1;

                // Shifting palette attributes by 1
                bg_shifter_attrib_lo <<= 1;
                bg_shifter_attrib_hi <<= 1;
            }
        };

        // All but 1 of the secanlines is visible to the user. The pre-render scanline
        // at -1, is used to configure the "shifters" for the first visible scanline, 0.
        if (scanline >= -1 && scanline < 240)
        {
            if (scanline == 0 && cycle == 0)
            {
                // "Odd Frame" cycle skip
                cycle = 1;
            }

            if (scanline == -1 && cycle == 1)
            {
                // Effectively start of new frame, so clear vertical blank flag
                status.vertical_blank = 0;
            }


            if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
            {
                UpdateShifters();


                // In these cycles we are collecting and working with visible data
                // The "shifters" have been preloaded by the end of the previous
                // scanline with the data for the start of this scanline. Once we
                // leave the visible region, we go dormant until the shifters are
                // preloaded for the next scanline.

                // Fortunately, for background rendering, we go through a fairly
                // repeatable sequence of events, every 2 clock cycles.
                switch ((cycle - 1) % 8)
                {
                    case 0:
                        // Load the current background tile pattern and attributes into the "shifter"
                        LoadBackgroundShifters();

                        // Fetch the next background tile ID
                        // "(vram_addr.reg & 0x0FFF)" : Mask to 12 bits that are relevant
                        // "| 0x2000"                 : Offset into nametable space on PPU address bus
                        bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));

                        // Explanation:
                        // The bottom 12 bits of the loopy register provide an index into
                        // the 4 nametables, regardless of nametable mirroring configuration.
                        // nametable_y(1) nametable_x(1) coarse_y(5) coarse_x(5)
                        //
                        // Consider a single nametable is a 32x32 array, and we have four of them
                        //   0                1
                        // 0 +----------------+----------------+
                        //   |                |                |
                        //   |                |                |
                        //   |    (32x32)     |    (32x32)     |
                        //   |                |                |
                        //   |                |                |
                        // 1 +----------------+----------------+
                        //   |                |                |
                        //   |                |                |
                        //   |    (32x32)     |    (32x32)     |
                        //   |                |                |
                        //   |                |                |
                        //   +----------------+----------------+
                        //
                        // This means there are 4096 potential locations in this array, which
                        // just so happens to be 2^12!
                        break;
                    case 2:
                        // Fetch the next background tile attribute. OK, so this one is a bit
                        // more involved :P

                        // Recall that each nametable has two rows of cells that are not tile
                        // information, instead they represent the attribute information that
                        // indicates which palettes are applied to which area on the screen.
                        // Importantly (and frustratingly) there is not a 1 to 1 correspondance
                        // between background tile and palette. Two rows of tile data holds
                        // 64 attributes. Therfore we can assume that the attributes affect
                        // 8x8 zones on the screen for that nametable. Given a working resolution
                        // of 256x240, we can further assume that each zone is 32x32 pixels
                        // in screen space, or 4x4 tiles. Four system palettes are allocated
                        // to background rendering, so a palette can be specified using just
                        // 2 bits. The attribute byte therefore can specify 4 distinct palettes.
                        // Therefore we can even further assume that a single palette is
                        // applied to a 2x2 tile combination of the 4x4 tile zone. The very fact
                        // that background tiles "share" a palette locally is the reason why
                        // in some games you see distortion in the colours at screen edges.

                        // As before when choosing the tile ID, we can use the bottom 12 bits of
                        // the loopy register, but we need to make the implementation "coarser"
                        // because instead of a specific tile, we want the attribute byte for a
                        // group of 4x4 tiles, or in other words, we divide our 32x32 address
                        // by 4 to give us an equivalent 8x8 address, and we offset this address
                        // into the attribute section of the target nametable.

                        // Reconstruct the 12 bit loopy address into an offset into the
                        // attribute memory

                        // "(vram_addr.coarse_x >> 2)"        : integer divide coarse x by 4,
                        //                                      from 5 bits to 3 bits
                        // "((vram_addr.coarse_y >> 2) << 3)" : integer divide coarse y by 4,
                        //                                      from 5 bits to 3 bits,
                        //                                      shift to make room for coarse x

                        // Result so far: YX00 00yy yxxx

                        // All attribute memory begins at 0x03C0 within a nametable, so OR with
                        // result to select target nametable, and attribute byte offset. Finally
                        // OR with 0x2000 to offset into nametable address space on PPU bus.
                        bg_next_tile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11)
                                                      | (vram_addr.nametable_x << 10)
                                                      | ((vram_addr.coarse_y >> 2) << 3)
                                                      | (vram_addr.coarse_x >> 2));

                        // Right we've read the correct attribute byte for a specified address,
                        // but the byte itself is broken down further into the 2x2 tile groups
                        // in the 4x4 attribute zone.

                        // The attribute byte is assembled thus: BR(76) BL(54) TR(32) TL(10)
                        //
                        // +----+----+			    +----+----+
                        // | TL | TR |			    | ID | ID |
                        // +----+----+ where TL =   +----+----+
                        // | BL | BR |			    | ID | ID |
                        // +----+----+			    +----+----+
                        //
                        // Since we know we can access a tile directly from the 12 bit address, we
                        // can analyse the bottom bits of the coarse coordinates to provide us with
                        // the correct offset into the 8-bit word, to yield the 2 bits we are
                        // actually interested in which specifies the palette for the 2x2 group of
                        // tiles. We know if "coarse y % 4" < 2 we are in the top half else bottom half.
                        // Likewise if "coarse x % 4" < 2 we are in the left half else right half.
                        // Ultimately we want the bottom two bits of our attribute word to be the
                        // palette selected. So shift as required...
                        if (vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
                        if (vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
                        bg_next_tile_attrib &= 0x03;
                        break;

                        // Compared to the last two, the next two are the easy ones... :P

                    case 4:
                        // Fetch the next background tile LSB bit plane from the pattern memory
                        // The Tile ID has been read from the nametable. We will use this id to
                        // index into the pattern memory to find the correct sprite (assuming
                        // the sprites lie on 8x8 pixel boundaries in that memory, which they do
                        // even though 8x16 sprites exist, as background tiles are always 8x8).
                        //
                        // Since the sprites are effectively 1 bit deep, but 8 pixels wide, we
                        // can represent a whole sprite row as a single byte, so offsetting
                        // into the pattern memory is easy. In total there is 8KB so we need a
                        // 13 bit address.

                        // "(control.pattern_background << 12)"  : the pattern memory selector
                        //                                         from control register, either 0K
                        //                                         or 4K offset
                        // "((uint16_t)bg_next_tile_id << 4)"    : the tile id multiplied by 16, as
                        //                                         2 lots of 8 rows of 8 bit pixels
                        // "(vram_addr.fine_y)"                  : Offset into which row based on
                        //                                         vertical scroll offset
                        // "+ 0"                                 : Mental clarity for plane offset
                        // Note: No PPU address bus offset required as it starts at 0x0000
                        bg_next_tile_lsb = ppuRead((control.pattern_background << 12)
                                                   + ((uint16_t)bg_next_tile_id << 4)
                                                   + (vram_addr.fine_y) + 0);

                        break;
                    case 6:
                        // Fetch the next background tile MSB bit plane from the pattern memory
                        // This is the same as above, but has a +8 offset to select the next bit plane
                        bg_next_tile_msb = ppuRead((control.pattern_background << 12)
                                                   + ((uint16_t)bg_next_tile_id << 4)
                                                   + (vram_addr.fine_y) + 8);
                        break;
                    case 7:
                        // Increment the background tile "pointer" to the next tile horizontally
                        // in the nametable memory. Note this may cross nametable boundaries which
                        // is a little complex, but essential to implement scrolling
                        IncrementScrollX();
                        break;
                }
            }

            // End of a visible scanline, so increment downwards...
            if (cycle == 256)
            {
                IncrementScrollY();
            }

            //...and reset the x position
            if (cycle == 257)
            {
                LoadBackgroundShifters();
                TransferAddressX();
            }

            // Superfluous reads of tile id at end of scanline
            if (cycle == 338 || cycle == 340)
            {
                bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
            }

            if (scanline == -1 && cycle >= 280 && cycle < 305)
            {
                // End of vertical blank period so reset the Y address ready for rendering
                TransferAddressY();
            }
        }

        if (scanline == 240)
        {
            // Post Render Scanline - Do Nothing!
        }

        if (scanline >= 241 && scanline < 261)
        {
            if (scanline == 241 && cycle == 1)
            {
                // Effectively end of frame, so set vertical blank flag
                status.vertical_blank = 1;

                // If the control register tells us to emit a NMI when
                // entering vertical blanking period, do it! The CPU
                // will be informed that rendering is complete so it can
                // perform operations with the PPU knowing it wont
                // produce visible artefacts
                if (control.enable_nmi)
                    nmi = true;
            }
        }



        // Composition - We now have background pixel information for this cycle
        // At this point we are only interested in background

        uint8_t bg_pixel = 0x00;   // The 2-bit pixel to be rendered
        uint8_t bg_palette = 0x00; // The 3-bit index of the palette the pixel indexes

        // We only render backgrounds if the PPU is enabled to do so. Note if
        // background rendering is disabled, the pixel and palette combine
        // to form 0x00. This will fall through the colour tables to yield
        // the current background colour in effect
        if (mask.render_background)
        {
            // Handle Pixel Selection by selecting the relevant bit
            // depending upon fine x scolling. This has the effect of
            // offsetting ALL background rendering by a set number
            // of pixels, permitting smooth scrolling
            uint16_t bit_mux = 0x8000 >> fine_x;

            // Select Plane pixels by extracting from the shifter
            // at the required location.
            uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
            uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

            // Combine to form pixel index
            bg_pixel = (p1_pixel << 1) | p0_pixel;

            // Get palette
            uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
            uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
            bg_palette = (bg_pal1 << 1) | bg_pal0;
        }


        // Now we have a final pixel colour, and a palette for this cycle
        // of the current scanline. Let's at long last, draw that ^&%*er :P

        sprScreen.SetPixel(cycle - 1, scanline, getColorInPalette(bg_palette, bg_pixel));

        // Advance renderer - it never stops, it's relentless
        cycle++;
        if (cycle >= 341)
        {
            cycle = 0;
            scanline++;
            if (scanline >= 261)
            {
                scanline = -1;
                frame_complete = true;
            }
        }
    }

    PPU::PPU() {
        this->bridge.setPPU(this);
        this->pattern.setPPU(this);
        this->palette.setPPU(this);
        this->nameTables.setPPU(this);
        this->nameTables.setChildPPU(this);
    }

    olc::Pixel PPU::getColorInPalette(int which_palette, int index) {
        if (index < 0 || index > 3) {
            throw std::runtime_error("index range is [" + std::to_string(0) + " - " + std::to_string(3) + "]");
        }

//        auto c = palette.getColor(which_palette, index);
//        return c;
        auto i = ppuRead(palette.addr_min + which_palette * 4 + index);
        return palette.getColor(i);
    }

    olc::Sprite *PPU::getPattern(int which, int nPalette) {
        return this->pattern.getSprite(which, nPalette);
    }

    void PPU::reset() {
        this->status.reg = 0;
        this->control.reg = 0;
        this->mask.reg = 0;
    }

}