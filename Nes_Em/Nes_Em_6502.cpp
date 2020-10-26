#include "Nes_Em_6502.h"
#include "Bus.h"

Nes_Em_6502::Nes_Em_6502() {
	bus = nullptr;

	//Initializing the opCodeLookUpTable
	using a = Nes_Em_6502;

	opCodeLookUpTable = {
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },
		{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },
		{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },
		{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },
		{ "???", &a::XXX, &a::IMP, 6 },{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },
		{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },
		{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },
		{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },{ "JSR", &a::JSR, &a::ABS, 6 },
		{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },
		{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },
		{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },
		{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },
		{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },
		{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },
		{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },
		{ "???", &a::XXX, &a::IMP, 7 },{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },
		{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },
		{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },
		{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "BVC", &a::BVC, &a::REL, 2 },
		{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },
		{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },
		{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },
		{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },
		{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },
		{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },
		{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },
		{ "???", &a::XXX, &a::IMP, 6 },{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },
		{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },
		{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },
		{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 2 },
		{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },
		{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },
		{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },
		{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },
		{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },
		{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },
		{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },
		{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },
		{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },
		{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "BCS", &a::BCS, &a::REL, 2 },
		{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },
		{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },
		{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },
		{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },
		{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },
		{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },
		{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },
		{ "???", &a::XXX, &a::IMP, 6 },{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },
		{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },
		{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },
		{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },{ "CPX", &a::CPX, &a::IMM, 2 },
		{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },
		{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },
		{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },
		{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },
		{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },
		{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },
		{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },
		{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },
		{ "???", &a::XXX, &a::IMP, 7 },
	};
}

Nes_Em_6502::~Nes_Em_6502() {
	//Nothing Yet
}

void Nes_Em_6502::connectBus(Bus* busAddr) {
	bus = busAddr;
	return;
}

uint8_t Nes_Em_6502::read(uint16_t addr) {
	return bus->read(addr);
}

void Nes_Em_6502::write(uint16_t addr, uint8_t data) {
	bus->write(addr, data);
	return;
}

void Nes_Em_6502::clock() {
	if (cycles == 0) {
		opcode = read(progcount_reg);
		progcount_reg++;

		//Get the cycles for the OP-Code read
		cycles = opCodeLookUpTable[opcode].cycles;

		uint8_t additional_cycles1 = (this->*opCodeLookUpTable[opcode].addrmode)();		//As some conditions may lead to the 
		uint8_t additional_cycles2 =  (this->*opCodeLookUpTable[opcode].operate)();		//Instruction needing more cycles than usual

		cycles += (additional_cycles1 + additional_cycles2);
	}

	//Everytime the fucntion is called the no of available cycle reduces
	cycles--;
}

// ADDRESSING MODES //

//Implied Address Mode
uint8_t Nes_Em_6502::IMP() {
	//May operate on the accumulator
	fetched = accum_reg;
	return 0;
}

//Immediate Adressing Mode
uint8_t Nes_Em_6502::IMM() {
	//Requires the immediate next address for data
	addr_abs = progcount_reg++;
	return 0;
}

//Zero Page Indexing 
//The memory is divided as 256 Pages, each page has 256 bytes.
//Thus the address 0xFFFF means that we are accessing 256th page (0-255 actually) and the last byte of it
// First two bytes (High byte) denote the pages and last two (low byte) denote the offset within the page.

//----> Zero Page with no Offset
uint8_t Nes_Em_6502::ZP0()
{
	addr_abs = read(progcount_reg);
	progcount_reg++;
	//Gets the last two bytes of the address. as we are already on 0th page
	addr_abs &= 0x00FF;
	return 0;
}

//-----> Zero Page Indexing with X reg offset
uint8_t Nes_Em_6502::ZPX() {
	addr_abs = (read(progcount_reg) + x_reg);		//Somewhat like array indexing
	progcount_reg++;
	addr_abs &= 0x00FF;
	return 0;
}

//-----> Zero Page Indexing with Y reg offset
uint8_t Nes_Em_6502::ZPX() {
	addr_abs = (read(progcount_reg) + y_reg);		//Somewhat like array indexing
	progcount_reg++;
	addr_abs &= 0x00FF;
	return 0;
}

//Absolute Address Mode (Unlike Zero Page, this takes input as full address [page and offset])
//------> No Offset Absolute Address
uint8_t Nes_Em_6502::ABS() {
	uint16_t high_byte = read(progcount_reg);
	progcount_reg++;
	uint16_t low_byte = read(progcount_reg);
	progcount_reg++;

	//Forming the complete 2Bytes Address
	addr_abs = (high_byte << 8) | low_byte;		// << by 8 cause Hexadecimal

	return 0;
}

//------> X Offset Absolute Address
uint8_t Nes_Em_6502::ABX() {
	uint16_t high_byte = read(progcount_reg);
	progcount_reg++;
	uint16_t low_byte = read(progcount_reg);
	progcount_reg++;

	//Forming the complete 2Bytes Address
	addr_abs = (high_byte << 8) | low_byte;		// << by 8 cause Hexadecimal
	addr_abs += x_reg;							//Offset by x_reg

	//Checking for Page change by getting the high byte and checking if it has changed after the offset
	if ((addr_abs & 0xFF00) != (high_byte << 8)) {
		//Means the page has changed so we need one more clock cycle for the getting the actual address on the next page
		return 1;
	}
	else {
		return 0;
	}
}

//------> Y Offset Absolute Address
uint8_t Nes_Em_6502::ABY() {
	uint16_t high_byte = read(progcount_reg);
	progcount_reg++;
	uint16_t low_byte = read(progcount_reg);
	progcount_reg++;

	//Forming the complete 2Bytes Address
	addr_abs = (high_byte << 8) | low_byte;		// << by 8 cause Hexadecimal
	addr_abs += y_reg;							//Offset by y_reg

	//Checking for Page change by getting the high byte and checking if it has changed after the offset
	if ((addr_abs & 0xFF00) != (high_byte << 8)) {
		//Means the page has changed so we need one more clock cycle for the getting the actual address on the next page
		return 1;
	}
	else {
		return 0;
	}
}


//Indirect Addressing : Similar to Pointers
//The data read is acutally the address to the acutal data intended

//-------> Zero Offset Ptr
//Emulated Bug: If the low byte is 0xFF then we must add 1 to the high byte to move to the next
//page but the hardware failed to do so. Programs have work-arounds for this bug
//and to make the programs valid with those work-arounds we need emulate it. 
uint8_t Nes_Em_6502::IND()
{
	uint16_t ptr_low = read(progcount_reg);
	progcount_reg++;
	uint16_t ptr_high = read(progcount_reg);
	progcount_reg++;

	//Constructing the pointer
	uint16_t ptr = (ptr_high << 8) | ptr_low;

	if (ptr_low == 0x00FF) // Simulate page boundary hardware bug
	{
		//Getting the faulty address by ignoring the +1 to index to next page
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else // Behave normally
	{
		//Add 1 to read the page number.
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}

//-----> Indirect Addressing of 0th Page with X offset
uint8_t Nes_Em_6502::IZX()
{
	uint16_t addr = read(progcount_reg);
	progcount_reg++;

	uint16_t low_byte = read((uint16_t)(addr + (uint16_t)x_reg) & 0x00FF);
	uint16_t high_byte = read((uint16_t)(addr + (uint16_t)x_reg + 1) & 0x00FF);

	addr_abs = (high_byte << 8) | low_byte;

	return 0;
}

//-----> Indirect Addressing of 0th Page with Y offset
//But this one behaves differently as we can move to next page with this one.
uint8_t Nes_Em_6502::IZY()
{
	uint16_t addr = read(progcount_reg);
	progcount_reg++;

	//Constructing the actual address to read from
	uint16_t low_byte = read(addr & 0x00FF);
	uint16_t high_byte = read((addr + 1) & 0x00FF);
	addr_abs = (high_byte << 8) | low_byte;
	addr_abs += y;

	//If indexing into the following page then extra clock cycle required
	if ((addr_abs & 0xFF00) != (high_byte << 8))
		return 1;
	else
		return 0;
}

//Relative Address Mode
//This is used for branch instructions like for & while loops
//or switch and if statements or break or continue etc.
//Thus cannot jump anywhere in the memory . Only from -128 to 127 of the
//branch instruction address.
uint8_t Nes_Em_6502::REL()
{
	addr_rel = read(progcount_reg);
	progcount_reg++;
	//Typically signed numbers have the 7th bit set to 1
	if (addr_rel & 0x80)			//Checking for sign (+ve is forward jump and -ve is backward)
		addr_rel |= 0xFF00;			//Setting the High Bits to 1 so as to ensure proper binary arithmetic later when we 
	return 0;						//add this relative address to the program counter
}