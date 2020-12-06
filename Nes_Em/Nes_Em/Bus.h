#pragma once
#include<cstdint>
#include "Nes_Em_6502.h"
#include <array>

class Bus
{
public:
	//Devices attached
	Nes_Em_6502 cpu;
	
	//Fake array
	std::array<uint8_t, 64*1024> ram;

	//Functions for the Bus
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);

	//--> Constructor for the Bus Class
	Bus();
};

