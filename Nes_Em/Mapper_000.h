#pragma once
#include <cstdint>

class Mapper_000
{
public:
	Mapper_000(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper_000();

public:
	bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr);
	bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr);
	bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr);
	bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr);

protected:
	uint8_t num_of_PRG_banks = 0;
	uint8_t num_of_CHR_banks = 0;
};

