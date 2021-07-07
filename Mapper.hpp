#pragma once
#include <iostream>
#include <cstdint>

class Mapper
{
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper();
    

public:
    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr)	 = 0;
	virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data=0)	 = 0;
	virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr)	 = 0;
	virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr)	 = 0;
    void reset(){};
protected:
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;
};

Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks)
{
    this->nPRGBanks = prgBanks;
    this->nCHRBanks= chrBanks;
    reset();
}

Mapper::~Mapper()
{   
}
