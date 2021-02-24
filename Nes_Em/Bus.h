#pragma once
#include<cstdint>
#include "Nes_Em_6502.h"
#include <array>
#include <memory>
#include "Nes_Em_2CO2.h"
#include "Cartridge.h"

class Bus
{
public:
	//Devices attached
	Nes_Em_6502 cpu;

	//--->The PPU: 2CO2
	Nes_Em_2CO2 ppu;
	
	//The cartridge 
	std::shared_ptr<Cartridge> cart;

	//CPU RAM (2KB)
	std::array<uint8_t, 2048> cpu_ram;

	//Functions for the Bus 
	void cpu_write(uint16_t addr, uint8_t data);
	uint8_t cpu_read(uint16_t addr);

	//--> Constructor for the Bus Class
	Bus();

public:
	//The bus will ultimately become the main class so it requires the following
	//functions
	// System Interface
	// Connects a cartridge object to the internal buses
	void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
	// Resets the system
	void reset();
	// Clocks the system - a single whole systme tick
	void clock();

private:
	// A count of how many clocks have passed
	uint32_t nSystemClockCounter = 0;
};

