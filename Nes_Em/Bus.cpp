#include "Bus.h"

Bus::Bus() {
	//Initialize the RAM with 0 values
	for (auto& i : cpu_ram) {
		i = 0x0000;
	}

	//Connecting the Cpu to the bus;
	cpu.connectBus(this);
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge) {
	this->cart = cartridge;
	ppu.ConnectCartridge(cartridge);
}

void Bus::reset() {
	//Performs a hard reset on the NES
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock()
{
}

uint8_t Bus::cpu_read(uint16_t addr) {
	uint8_t data = 0x00;

	//Guarding the CPU_RAM to its address limit <8KB> 
	//But mirroring so we ignore the first bit and consider only the last three bits
	if (cart->cpuRead(addr, data)) {
		// Cartridge Address Range
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		data = cpu_ram[addr & 0x07FF];
	}
	else if(addr >= 0x2000 && addr <= 0x3FFF) {
		data = ppu.cpuRead(addr & 0x0007);
	}

	return data;
}

void Bus::cpu_write(uint16_t addr, uint8_t data) {
	//Guarding the CPU_RAM to its address limit <8KB>
	//But mirroring so we ignore the first bit and consider only the last three bits
	if (cart->cpuWrite(addr, data)) {
		//Cartridge can veto
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		cpu_ram[addr & 0x07FF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF) {
		//PPU has only 8 places to store value and a larger addressable range
		//Hence again we using Mirroring
		ppu.cpuWrite(addr & 0x0007, data);
	}
	return;
}