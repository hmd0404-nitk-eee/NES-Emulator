#pragma once
#include <cstdint>
#include <vector>
#include <string>

#include "Mapper_000.h"

class Cartridge
{
public:
	//Constructors
	Cartridge(const std::string& sFileName);

	bool ImageValid();

	// Communication with Main Bus
	bool cpuRead(uint16_t addr, uint8_t& data);
	bool cpuWrite(uint16_t addr, uint8_t data);

	// Communication with PPU Bus
	bool ppuRead(uint16_t addr, uint8_t& data);
	bool ppuWrite(uint16_t addr, uint8_t data);

private:
	std::vector<uint8_t> vramPRG_Memory;
	std::vector<uint8_t> vramCHR_Memory;

	std::shared_ptr<Mapper_000> pMapper;

	bool bImageValid = false;

	//Basic Variables
	uint8_t nMapperID = 0;
	uint8_t num_of_PRG_banks = 0;
	uint8_t num_of_CHR_banks = 0;

public:
	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} mirror = HORIZONTAL;
};

