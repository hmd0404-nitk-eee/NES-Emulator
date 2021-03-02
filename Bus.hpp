#pragma once
#include <iostream>
#include <cstdint>
#include "olc6502.hpp"
#include <array>
#include "olc2C02.hpp"
#include "Cartridge.hpp"
class Bus
{
public:
  Bus();
  ~Bus();

public:
  olc6502 cpu;
  olc2C02 ppu;
  array<uint8_t, 2*1024> ram;
  shared_ptr<Cartridge> cart;

public:
  void cpuWrite(uint16_t addr, uint8_t data);
  uint8_t cpuRead(uint16_t addr, bool bReadOnly=false);

public:
  void insertCartridge(const shared_ptr<Cartridge>& cartridge);
  void reset();
  void clock();

private:
  uint32_t nSystemClockCounter = 0;

};

Bus::Bus()
{
  for(auto &i :ram) i = 0x00;
  cpu.ConnectBus(this);
}

Bus::~Bus()
{

}

void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
  if (cart->cpuWrite(addr, data)){

  }
  else if(addr>= 0x0000 && addr <= 0x1FFF)
    ram[addr & 0x07FF] = data;
  else if(addr >= 0x2000 && addr<= 0x3FFF){
    ppu.cpuWrite(addr & 0x0007, data);
  }
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
  uint8_t data=0x00;
  if(cart->cpuRead(addr,data))
  {

  }
  else if(addr>= 0x0000 && addr <= 0x1FFF)
      data = ram[addr & 0x07FF];
    else if(addr>=0x2000 && addr <=0x3FFF){
      data=ppu.cpuRead(addr & 0x0007, bReadOnly);
    }
    return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
  this->cart=cartridge;
  ppu.ConnectCartridge(cartridge);
}

void Bus::reset(){
  cpu.reset();
  nSystemClockCounter=0;
}

void Bus::clock(){
  ppu.clock();
  if(nSystemClockCounter % 3 ==0)
  {
    cpu.clock();
  }
  nSystemClockCounter++;
}

