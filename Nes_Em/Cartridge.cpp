#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge(const std::string& sFileName) {
	// iNES Format Header
	struct sHeader
	{
		char name[4];
		uint8_t prg_rom_chunks;
		uint8_t chr_rom_chunks;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prg_ram_size;
		uint8_t tv_system1;
		uint8_t tv_system2;
		char unused[5];
	} header;

	std::ifstream inpFile;
	inpFile.open(sFileName, std::ifstream::binary);
	if (inpFile.is_open()) {
		// Read file header
		inpFile.read((char*)&header, sizeof(sHeader));

		//Skipping the trainer part in the file
		if (header.mapper1 & 0x04) {
			inpFile.seekg(512, std::ios_base::cur);
		}

		// Determine Mapper ID or Getting the entire chunk of headers in the struct
		nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

		uint8_t nFileType = 1;

		if (nFileType == 1) {
			num_of_PRG_banks = header.prg_rom_chunks;
			vramPRG_Memory.resize(num_of_PRG_banks * 16384);						//16KB bank each
			inpFile.read((char*)vramPRG_Memory.data(), vramPRG_Memory.size());

			num_of_CHR_banks = header.chr_rom_chunks;
			vramCHR_Memory.resize(num_of_CHR_banks * 8192);							//8Kb bank each
			inpFile.read((char*)vramCHR_Memory.data(), vramCHR_Memory.size());
		}

		// Load appropriate mapper
		switch (nMapperID) {
			case 0: 
				pMapper = std::make_shared<Mapper_000>(num_of_PRG_banks, num_of_CHR_banks);
				break;
		}

		inpFile.close();
	}

}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data) {
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapRead(addr, mapped_addr)) {
		data = vramPRG_Memory[mapped_addr];
		return true;
	}
	else {
		return false;
	}	
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapWrite(addr, mapped_addr)) {
		vramPRG_Memory[mapped_addr] = data;
		return true;
	}
	else {
		return false;
	}
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data) {
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr))
	{
		data = vramCHR_Memory[mapped_addr];
		return true;
	}
	else {
		return false;
	}
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr))
	{
		vramCHR_Memory[mapped_addr] = data;
		return true;
	}
	else { 
		return false; 
	}
}

bool Cartridge::ImageValid()
{
	return bImageValid;
}