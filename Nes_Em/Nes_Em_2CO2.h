#pragma once
#include <cstdint>
#include <memory>
#include "Cartridge.h"
#include "olcPixelGameEngine.h"

class Nes_Em_2CO2
{
public:
	//Devices connected to the PPU Bus
	uint8_t NameTableVRAM[2][1024];	//2KB: Splitted into two 1kB as Full Table is only 1kB and NES has two such tables
	uint8_t PatternTable[2][4096];
	uint8_t PaletteTable[32];

	//Connect PPU with Main CPU Bus
	uint8_t cpu_read(uint16_t addr);
	void    cpu_write(uint16_t addr, uint8_t  data);

	// Connect PPU with PPU Bus
	uint8_t ppu_read(uint16_t addr, bool rdonly = false);
	void    ppu_write(uint16_t addr, uint8_t data);

	//Constructors
	Nes_Em_2CO2();

public:
	// Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette);

	olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);

	bool frame_complete = false;

private:
	olc::Pixel  palScreen[0x40];
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };

private:
	// The Cartridge connected to the PPU as well.
	std::shared_ptr<Cartridge> cart;

public:
	// Interface
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
};

