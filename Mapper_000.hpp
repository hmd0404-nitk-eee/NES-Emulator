#pragma once
#include <iostream>
#include "Mapper.hpp"
using namespace std;

class Mapper_000 : public Mapper
{
public:
	Mapper_000(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper_000();
    

public:
	bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
	bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data=0) override;
	bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
	bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
    
};

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{
}


Mapper_000::~Mapper_000()
{
}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr=addr&(nPRGBanks>1 ? 0x7FFF: 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data){
    
    if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr){
    
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        mapped_addr =addr;
        return true;
    }
    return false;    
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr){
    if (addr <= 0x1FFF)
	{
		if (!nCHRBanks)
		{
			mapped_addr = addr;
            
			return true;
		}
	}
	return false;
}
