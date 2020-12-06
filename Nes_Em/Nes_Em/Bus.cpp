#include "Bus.h"

Bus::Bus() {
	//Initialize the RAM with 0 values
	for (auto& i : ram) {
		i = 0x0000;
	}

	//Connecting the Cpu to the bus;
	cpu.connectBus(this);
}

uint8_t Bus::read(uint16_t addr) {
	//Guarding the Ram to its address limit
	if (addr >= 0x0000 && addr <= 0xFFFF) {
		return ram[addr;
	}
	else {
		//When the Bus write gets addr out of range for the RAM
		return 0x00;
	}
}

void Bus::write(uint16_t addr, uint8_t data) {
	//Guarding the Ram to its address limit
	if (addr >= 0x0000 && addr <= 0xFFFF) {
		ram[addr] = data;
	}

	return;
}