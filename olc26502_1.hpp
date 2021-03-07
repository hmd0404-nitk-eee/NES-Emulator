#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <string>

#include "olc2C02.hpp"
#include "Cartridge.hpp"

// FORWARD DECLARATION
class olc6502;

#define Bus_olc6502_RAM_SIZE 2048 // 2KB

class Bus
{
	public: // VARIABLES

	Cartridge* Cartridge;
	olc6502* olc6502;	
	olc2C02 olc2C02;

	uint8_t olc6502_ram[Bus_olc6502_RAM_SIZE];

	uint8_t controller[2];

	public: // FUNCTIONS

	Bus();
	~Bus();

	void clock();
	void reset();

	void connect_Rom(Cartridge*);

	uint8_t olc6502_read(uint16_t);
	void olc6502_write(uint16_t, uint8_t);

//-------------------------------------------------------

	private: // VARIABLES

	uint32_t clock_total = 0;

	uint8_t controller_state[2];

	// DMA for interacting with the olc2C02's OAM
	bool dma = false;
	bool dma_wait = true;
	uint8_t dma_page = 0;
	uint8_t dma_address = 0;
	uint8_t dma_data = 0;
};

//==================================================================================================================================

#define olc6502_FLAG uint8_t
#define	olc6502_FLAG_C (1 << 0) // Carry Bit
#define	olc6502_FLAG_Z (1 << 1) // Zero
#define	olc6502_FLAG_I (1 << 2) // Disable Interrupts
#define	olc6502_FLAG_D (1 << 3) // Decimal Mode - Unused
#define	olc6502_FLAG_B (1 << 4) // Break
#define	olc6502_FLAG_U (1 << 5) // Unused
#define	olc6502_FLAG_V (1 << 6) // Overflow
#define	olc6502_FLAG_N (1 << 7) // Negative

struct olc6502_INSTRUCTION
{
	std::string name;		
	uint8_t (olc6502::*operate )(void) = nullptr;
	uint8_t (olc6502::*addrmode)(void) = nullptr;
	uint8_t cycles = 0;
};

class olc6502
{
	public: // VARIABLES

	uint8_t a = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t PSW = 0;

	uint16_t PC = 0;
	uint8_t stack = 0;

	public: // FUNCTIONS

	olc6502();

	void connect_Bus(Bus*);

	void clock();
	void reset();
	void irq();
	void nmi();

//-------------------------------------------------------

	private: // VARIABLES

	Bus* Bus = nullptr;

	uint8_t fetched = 0;
	uint16_t addr_abs = 0;
	uint16_t addr_rel = 0;

	uint8_t opcode = 0;

	uint8_t cycles = 0;

	std::vector<olc6502_INSTRUCTION> lookup;

	uint16_t temp = 0;

	private: // FUNCTIONS

	uint8_t get_flag(olc6502_FLAG);
	void set_flag(olc6502_FLAG, bool);

	uint8_t read(uint16_t);
	void write(uint16_t, uint8_t);
	
	uint8_t fetch();

	// 12 Addressing Modes
	uint8_t IMP();
	uint8_t IMM();	
	uint8_t ZP0();
	uint8_t ZPX();	
	uint8_t ZPY();
	uint8_t REL();
	uint8_t ABS();
	uint8_t ABX();	
	uint8_t ABY();
	uint8_t IND();	
	uint8_t IZX();
	uint8_t IZY();

	// 56 Opcodes + 1 Opcode for invalid operations
	uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();
	uint8_t XXX();
};

//==================================================================================================================================
//==================================================================================================================================
// FUNCTION DEFINITIONS
//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

Bus::Bus()
{
    memset(olc6502_ram, 0, Bus_olc6502_RAM_SIZE);

    olc6502 = new olc6502();
	olc6502->connect_Bus(this);

	controller[0] = 0;
	controller[1] = 0;
}

Bus::~Bus()
{
    delete olc6502;
}

void Bus::clock()
{
	olc2C02.clock();
	if (!(clock_total % 3))
	{
		if (dma)
		{
			if (dma_wait)
			{
				if (clock_total % 2)
				{
					dma_wait = false;
				}
			}
			else
			{
				if (!(clock_total % 2))
				{
					dma_data = olc6502_read((dma_page << 8) | dma_address);
				}
				else
				{
					olc2C02.oam[dma_address++] = dma_data;

					if (!dma_address)
					{
						dma = false;
						dma_wait = true;
					}
				}
			}
		}
		else
		{
			olc6502->clock();
		}
	}

	if (olc2C02.nmi)
	{
		olc2C02.nmi = false;
		olc6502->nmi();
	}

	clock_total++;
}

void Bus::reset()
{
	Cartridge->reset();
	olc6502->reset();
	olc2C02.reset();
	clock_total = 0;
}

void Bus::connect_Cartridge(Cartridge* Cartridge)
{
	this->Cartridge = Cartridge;
	olc2C02.connect_Cartridge(Cartridge);
}

uint8_t Bus::olc6502_read(uint16_t address)
{
	uint8_t data = 0;

	if (Cartridge->olc6502_read(address, data))
	{}
	else if (address <= 0x1FFF)
	{
		data = olc6502_ram[address & 0x7FF];
	}
	else if (0x2000 <= address && address <= 0x3FFF)
	{
		data = olc2C02.olc6502_read(address & 7);
	}
	else if (0x4016 <= address && address <= 0x4017)
	{
		data = (controller_state[address & 1] & 0x80) > 0;
		controller_state[address & 1] <<= 1;
	}

	return data;
}

void Bus::olc6502_write(uint16_t address, uint8_t data)
{	

	if (Cartridge->olc6502_write(address, data))
	{}
	else if (address <= 0x1FFF)
	{
		olc6502_ram[address & 0x7FF] = data;
	}
	else if (0x2000 <= address && address <= 0x3FFF)
	{
	
		olc2C02.olc6502_write(address & 7, data);
	}
	else if (address == 0x4014)
	{
		dma_page = data;
		dma_address = 0;
		dma = true;
	}
	else if (0x4016 <= address && address <= 0x4017)
	{
		controller_state[address & 1] = controller[address & 1];
	}
}

//==================================================================================================================================

olc6502::olc6502()
{
	using a = olc6502;
	lookup = 
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

void olc6502::connect_Bus(Bus* Bus)
{
	this->Bus = Bus;
}

uint8_t olc6502::read(uint16_t address)
{
	return Bus->olc6502_read(address);
}

void olc6502::write(uint16_t address, uint8_t data)
{
	Bus->olc6502_write(address, data);
}

void olc6502::reset()
{
	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	PC = (hi << 8) | lo;

	a = 0;
	x = 0;
	y = 0;
	PSW = 0 | olc6502_FLAG_U;

	stack = 0xFD;

	addr_rel = 0;
	addr_abs = 0;
	fetched = 0;

	cycles = 8;
}

void olc6502::irq()
{
	if (!get_flag(olc6502_FLAG_I))
	{
		write(0x100 + stack, (PC >> 8) & 0xFF);
		stack--;
		write(0x100 + stack, PC & 0xFF);
		stack--;

		set_flag(olc6502_FLAG_B, false);
		set_flag(olc6502_FLAG_U, true);
		set_flag(olc6502_FLAG_I, true);
		write(0x100 + stack, PSW);
		stack--;

		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs);
		uint16_t hi = read(addr_abs + 1);
		PC = (hi << 8) | lo;

		cycles = 7;
	}
}

void olc6502::nmi()
{
	write(0x100 + stack, (PC >> 8) & 0xFF);
	stack--;
	write(0x100 + stack, PC & 0xFF);
	stack--;

	set_flag(olc6502_FLAG_B, false);
	set_flag(olc6502_FLAG_U, true);
	set_flag(olc6502_FLAG_I, true);
	write(0x100 + stack, PSW);
	stack--;

	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs);
	uint16_t hi = read(addr_abs + 1);
	PC = (hi << 8) | lo;

	cycles = 8;
}

void olc6502::clock()
{
	if (!cycles)
	{
	
		opcode = read(PC);
		set_flag(olc6502_FLAG_U, true);
		PC++;

		cycles = lookup[opcode].cycles;
		uint8_t tmp1 = (this->*lookup[opcode].addrmode)();
		uint8_t tmp2 = (this->*lookup[opcode].operate)();
		cycles += (tmp1 & tmp2);

		set_flag(olc6502_FLAG_U, true);
	}
	cycles--;
}

uint8_t olc6502::get_flag(olc6502_FLAG flag)
{
	return ((PSW & flag) > 0) ? 1 : 0;
}

void olc6502::set_flag(olc6502_FLAG flag, bool value)
{
	if (value)
		PSW |= flag;
	else
		PSW &= ~flag;
}

uint8_t olc6502::IMP()
{
	fetched = a;
	return 0;
}

uint8_t olc6502::IMM()
{
	addr_abs = PC++;	
	return 0;
}

uint8_t olc6502::ZP0()
{
	addr_abs = read(PC);	
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::ZPX()
{
	addr_abs = (read(PC) + x);
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::ZPY()
{
	addr_abs = (read(PC) + y);
	PC++;
	addr_abs &= 0x00FF;
	return 0;
}

uint8_t olc6502::REL()
{
	addr_rel = read(PC);
	PC++;
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00;
	return 0;
}

uint8_t olc6502::ABS()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;

	addr_abs = (hi << 8) | lo;

	return 0;
}

uint8_t olc6502::ABX()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;

	addr_abs = (hi << 8) | lo;
	addr_abs += x;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;	
}

uint8_t olc6502::ABY()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

uint8_t olc6502::IND()
{
	uint16_t ptr_lo = read(PC);
	PC++;
	uint16_t ptr_hi = read(PC);
	PC++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug
	{
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else // Behave normally
	{
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}
	
	return 0;
}

uint8_t olc6502::IZX()
{
	uint16_t t = read(PC);
	PC++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	
	return 0;
}

uint8_t olc6502::IZY()
{
	uint16_t t = read(PC);
	PC++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;
	
	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

uint8_t olc6502::fetch()
{
	if (!(lookup[opcode].addrmode == &olc6502::IMP))
		fetched = read(addr_abs);
	return fetched;
}

uint8_t olc6502::ADC()
{

	fetch();
	temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)get_flag(olc6502_FLAG_C);
	set_flag(olc6502_FLAG_C, temp > 255);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0);
	set_flag(olc6502_FLAG_V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);
	set_flag(olc6502_FLAG_N, temp & 0x80);
	a = temp & 0x00FF;
	return 1;
}

uint8_t olc6502::SBC()
{
	fetch();
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
	temp = (uint16_t)a + value + (uint16_t)get_flag(olc6502_FLAG_C);
	set_flag(olc6502_FLAG_C, temp & 0xFF00);
	set_flag(olc6502_FLAG_Z, ((temp & 0x00FF) == 0));
	set_flag(olc6502_FLAG_V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	a = temp & 0x00FF;
	return 1;
}

uint8_t olc6502::AND()
{
	fetch();
	a = a & fetched;
	set_flag(olc6502_FLAG_Z, a == 0x00);
	set_flag(olc6502_FLAG_N, a & 0x80);
	return 1;
}

uint8_t olc6502::ASL()
{
	fetch();
	temp = (uint16_t)fetched << 1;
	set_flag(olc6502_FLAG_C, (temp & 0xFF00) > 0);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x00);
	set_flag(olc6502_FLAG_N, temp & 0x80);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::BCC()
{
	if (get_flag(olc6502_FLAG_C) == 0)
	{
		cycles++;
		addr_abs = PC + addr_rel;
		
		if((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;
		
		PC = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BCS()
{
	if (get_flag(olc6502_FLAG_C) == 1)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BEQ()
{
	if (get_flag(olc6502_FLAG_Z) == 1)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BIT()
{
	fetch();
	temp = a & fetched;
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x00);
	set_flag(olc6502_FLAG_N, fetched & (1 << 7));
	set_flag(olc6502_FLAG_V, fetched & (1 << 6));
	return 0;
}

uint8_t olc6502::BMI()
{
	if (get_flag(olc6502_FLAG_N) == 1)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BNE()
{
	if (get_flag(olc6502_FLAG_Z) == 0)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BPL()
{
	if (get_flag(olc6502_FLAG_N) == 0)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BRK()
{
	PC++;
	
	set_flag(olc6502_FLAG_I, 1);
	write(0x0100 + stack, (PC >> 8) & 0x00FF);
	stack--;
	write(0x0100 + stack, PC & 0x00FF);
	stack--;

	set_flag(olc6502_FLAG_B, 1);
	write(0x0100 + stack, PSW);
	stack--;
	set_flag(olc6502_FLAG_B, 0);

	PC = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

uint8_t olc6502::BVC()
{
	if (get_flag(olc6502_FLAG_V) == 0)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

uint8_t olc6502::BVS()
{
	if (get_flag(olc6502_FLAG_V) == 1)
	{
		cycles++;
		addr_abs = PC + addr_rel;

		if ((addr_abs & 0xFF00) != (PC & 0xFF00))
			cycles++;

		PC = addr_abs;
	}
	return 0;
}

uint8_t olc6502::CLC()
{
	set_flag(olc6502_FLAG_C, false);
	return 0;
}

uint8_t olc6502::CLD()
{
	set_flag(olc6502_FLAG_D, false);
	return 0;
}

uint8_t olc6502::CLI()
{
	set_flag(olc6502_FLAG_I, false);
	return 0;
}

uint8_t olc6502::CLV()
{
	set_flag(olc6502_FLAG_V, false);
	return 0;
}

uint8_t olc6502::CMP()
{
	fetch();
	temp = (uint16_t)a - (uint16_t)fetched;
	set_flag(olc6502_FLAG_C, a >= fetched);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x0000);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	return 1;
}

uint8_t olc6502::CPX()
{
	fetch();
	temp = (uint16_t)x - (uint16_t)fetched;
	set_flag(olc6502_FLAG_C, x >= fetched);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x0000);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	return 0;
}

uint8_t olc6502::CPY()
{
	fetch();
	temp = (uint16_t)y - (uint16_t)fetched;
	set_flag(olc6502_FLAG_C, y >= fetched);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x0000);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	return 0;
}

uint8_t olc6502::DEC()
{
	fetch();
	temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x0000);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	return 0;
}

uint8_t olc6502::DEX()
{
	x--;
	set_flag(olc6502_FLAG_Z, x == 0x00);
	set_flag(olc6502_FLAG_N, x & 0x80);
	return 0;
}

uint8_t olc6502::DEY()
{
	y--;
	set_flag(olc6502_FLAG_Z, y == 0x00);
	set_flag(olc6502_FLAG_N, y & 0x80);
	return 0;
}

uint8_t olc6502::EOR()
{
	fetch();
	a = a ^ fetched;	
	set_flag(olc6502_FLAG_Z, a == 0x00);
	set_flag(olc6502_FLAG_N, a & 0x80);
	return 1;
}

uint8_t olc6502::INC()
{
	fetch();
	temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x0000);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	return 0;
}

uint8_t olc6502::INX()
{
	x++;
	set_flag(olc6502_FLAG_Z, x == 0x00);
	set_flag(olc6502_FLAG_N, x & 0x80);
	return 0;
}

uint8_t olc6502::INY()
{
	y++;
	set_flag(olc6502_FLAG_Z, y == 0x00);
	set_flag(olc6502_FLAG_N, y & 0x80);
	return 0;
}

uint8_t olc6502::JMP()
{
	PC = addr_abs;
	return 0;
}

uint8_t olc6502::JSR()
{
	PC--;

	write(0x0100 + stack, (PC >> 8) & 0x00FF);
	stack--;
	write(0x0100 + stack, PC & 0x00FF);
	stack--;

	PC = addr_abs;
	return 0;
}

uint8_t olc6502::LDA()
{
	fetch();
	a = fetched;
	set_flag(olc6502_FLAG_Z, a == 0x00);
	set_flag(olc6502_FLAG_N, a & 0x80);
	return 1;
}

uint8_t olc6502::LDX()
{
	fetch();
	x = fetched;
	set_flag(olc6502_FLAG_Z, x == 0x00);
	set_flag(olc6502_FLAG_N, x & 0x80);
	return 1;
}

uint8_t olc6502::LDY()
{
	fetch();
	y = fetched;
	set_flag(olc6502_FLAG_Z, y == 0x00);
	set_flag(olc6502_FLAG_N, y & 0x80);
	return 1;
}

uint8_t olc6502::LSR()
{
	
	fetch();
	set_flag(olc6502_FLAG_C, fetched & 0x0001);
	temp = fetched >> 1;	
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x0000);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::NOP()
{
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}

uint8_t olc6502::ORA()
{
	fetch();
	a = a | fetched;
	set_flag(olc6502_FLAG_Z, a == 0x00);
	set_flag(olc6502_FLAG_N, a & 0x80);
	return 1;
}

uint8_t olc6502::PHA()
{
	write(0x0100 + stack, a);
	stack--;
	return 0;
}

uint8_t olc6502::PHP()
{
	write(0x0100 + stack, PSW | olc6502_FLAG_B | olc6502_FLAG_U);
	set_flag(olc6502_FLAG_B, 0);
	set_flag(olc6502_FLAG_U, 0);
	stack--;
	return 0;
}

uint8_t olc6502::PLA()
{
	stack++;
	a = read(0x0100 + stack);
	set_flag(olc6502_FLAG_Z, a == 0x00);
	set_flag(olc6502_FLAG_N, a & 0x80);
	return 0;
}

uint8_t olc6502::PLP()
{
	stack++;
	PSW = read(0x0100 + stack);
	set_flag(olc6502_FLAG_U, 1);
	return 0;
}

uint8_t olc6502::ROL()
{
	fetch();
	temp = (uint16_t)(fetched << 1) | get_flag(olc6502_FLAG_C);
	set_flag(olc6502_FLAG_C, temp & 0xFF00);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x0000);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::ROR()
{
	
	fetch();
	temp = (uint16_t)(get_flag(olc6502_FLAG_C) << 7) | (fetched >> 1);
	set_flag(olc6502_FLAG_C, fetched & 0x01);
	set_flag(olc6502_FLAG_Z, (temp & 0x00FF) == 0x00);
	set_flag(olc6502_FLAG_N, temp & 0x0080);
	if (lookup[opcode].addrmode == &olc6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t olc6502::RTI()
{
	stack++;
	PSW = read(0x0100 + stack);
	PSW &= ~olc6502_FLAG_B;
	PSW &= ~olc6502_FLAG_U;

	stack++;
	PC = (uint16_t)read(0x0100 + stack);
	stack++;
	PC |= (uint16_t)read(0x0100 + stack) << 8;
	return 0;
}

uint8_t olc6502::RTS()
{
	stack++;
	PC = (uint16_t)read(0x0100 + stack);
	stack++;
	PC |= (uint16_t)read(0x0100 + stack) << 8;
	
	PC++;
	return 0;
}

uint8_t olc6502::SEC()
{
	set_flag(olc6502_FLAG_C, true);
	return 0;
}

uint8_t olc6502::SED()
{
	set_flag(olc6502_FLAG_D, true);
	return 0;
}

uint8_t olc6502::SEI()
{
	set_flag(olc6502_FLAG_I, true);
	return 0;
}

uint8_t olc6502::STA()
{
	write(addr_abs, a);
	return 0;
}

uint8_t olc6502::STX()
{
	write(addr_abs, x);
	return 0;
}

uint8_t olc6502::STY()
{
	write(addr_abs, y);
	return 0;
}

uint8_t olc6502::TAX()
{
	x = a;
	set_flag(olc6502_FLAG_Z, x == 0x00);
	set_flag(olc6502_FLAG_N, x & 0x80);
	return 0;
}

uint8_t olc6502::TAY()
{
	y = a;
	set_flag(olc6502_FLAG_Z, y == 0x00);
	set_flag(olc6502_FLAG_N, y & 0x80);
	return 0;
}

uint8_t olc6502::TSX()
{
	x = stack;
	set_flag(olc6502_FLAG_Z, x == 0x00);
	set_flag(olc6502_FLAG_N, x & 0x80);
	return 0;
}

uint8_t olc6502::TXA()
{
	a = x;
	set_flag(olc6502_FLAG_Z, a == 0x00);
	set_flag(olc6502_FLAG_N, a & 0x80);
	return 0;
}

uint8_t olc6502::TXS()
{
	stack = x;
	return 0;
}

uint8_t olc6502::TYA()
{
	a = y;
	set_flag(olc6502_FLAG_Z, a == 0x00);
	set_flag(olc6502_FLAG_N, a & 0x80);
	return 0;
}

uint8_t olc6502::XXX()
{
	return 0;
}