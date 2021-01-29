#pragma once
#include <cstdint>
#include "olc6502.h"
#include <array>
#include "olc2C02.h"
#include "Cartridge.h"
class Bus
{
public:
  Bus();
  ~Bus();

public:
  olc6502 cpu;
  olc2C02 ppu;
  array<uint8_t, 2*1024> cpuRam;
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
