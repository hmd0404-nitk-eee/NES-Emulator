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

// Fetch method: To get data and update the fetched value.
//Also return the same value for ease in handling in few functions.
uint8_t Nes_Em_6502::fetch() {
	//we don't fetch data if the mode is IMP (implied) as it doesn't need any extra data
	if (opCodeLookUpTable[opcode].addrmode != &Nes_Em_6502::IMP) {
		//Read from the absolute address
		fetched = read(addr_abs);
		return fetched;
	}

	return 0;
}

//Reset method for the 6502.
//The chip reads data directly from 0xFFFC to get the location for the program
//The address 0xFFFC can be set directly by the programmer in compile time for the chip,
//to handle reset and proper arrive at the program.
//Also The Unused flag is kept 'on' (IDK why?)
void Nes_Em_6502::reset() {
	//Getting the location for reseted prog_counter
	addr_abs = 0xFFFC;
	uint16_t low_byte = read(addr_abs);
	uint16_t high_byte = read(addr_abs + 1);
	progcount_reg = (high_byte << 8) | low_byte;		//As the prog_counter is 2 bytes

	//Resetting the registers
	accum_reg = 0x00;
	x_reg = 0x00;
	y_reg = 0x00;
	stkptr_reg = 0xFD;		//Some static chip inbuilt address for it
	status_reg = 0x00 | U;	//U flag must be on always

	//Clearing the internal helper variables
	addr_abs = 0x00;
	addr_rel = 0x00;
	fetched = 0x00;

	//Reset function takes time, but why 8? To check!
	cycles = 8;
}

//Similar to reset function but doesn't change the program state, it just stops the current actions
//And performs a specific interupt handling action programmed by the programmer.
void Nes_Em_6502::interupt_req() {
	//Getting if the Interupts are allowed
	if (getFlag(I) == 0) {
		//Via standard procedure, the program must push the program counter and the status to the stack
		
		//Program counter is 16bit thus two pushes required!
		//The stack starts at 0x0100 (system)
		write(0x0100 + stkptr_reg, (progcount_reg >> 8) & 0x00FF);		//High 8 bits
		stkptr_reg--;
		write(0x0100 + stkptr_reg, progcount_reg & 0x00FF);				//Low 8 bits
		stkptr_reg--;

		//Pushing the status to the stack
		setFlag(B, 0);
		setFlag(U, 1);
		setFlag(I, 1);
		write(0x0100 + stkptr_reg, status_reg);
		stkptr_reg--;

		//Reading the location of program that handles the interupts
		//The program is always written to a fixe address
		addr_abs = 0xFFFE;							//Absolute address that has the interupt handler program
		uint16_t low_bits = read(addr_abs);
		uint16_t high_bits = read(addr_abs + 1);
		progcount_reg = (high_bits << 8) | low_bits;

		//Again interupts takes time but why 7 cycles? To check!
		cycles = 7;
	}
}

//Non maskable interupts are same as interupt_req but nothing can stop it from executing
void Nes_Em_6502::non_mask_interupt_req() {
	//Via standard procedure, the program must push the program counter and the status to the stack

	//Program counter is 16bit thus two pushes required!
	//The stack starts at 0x0100 (system)
	write(0x0100 + stkptr_reg, (progcount_reg >> 8) & 0x00FF);		//High 8 bits
	stkptr_reg--;
	write(0x0100 + stkptr_reg, progcount_reg & 0x00FF);				//Low 8 bits
	stkptr_reg--;

	//Pushing the status to the stack
	setFlag(B, 0);
	setFlag(U, 1);
	setFlag(I, 1);
	write(0x0100 + stkptr_reg, status_reg);
	stkptr_reg--;

	//Reading the location of program that handles the interupts
	//The program is always written to a fixe address
	addr_abs = 0xFFFA;							//Absolute address that has the interupt handler program
	uint16_t low_bits = read(addr_abs);
	uint16_t high_bits = read(addr_abs + 1);
	progcount_reg = (high_bits << 8) | low_bits;

	//Again interupts takes time but why 8 cycles? To check!
	cycles = 8;
}

//-------------> Flag Functions <---------------//

uint8_t Nes_Em_6502::getFlag(FLAGS_6502 flag) {
	//returns 1 if status register has the flag active
	//Checks by performing bitwise & with the flag.
	return (status_reg & flag) ? 1 : 0;
}

void Nes_Em_6502::setFlag(FLAGS_6502 flag, bool value) {
	//sets the status_reg to the flag as per value
	if (value) {
		//value -> true then we set the flag 'on' by 'or-ing' it with existing value
		status_reg |= flag;
	}
	else {
		//value -> false then we set the flag 'off' by 'and-ing' it's complement with exiting value
		status_reg &= ~flag;
	}
}

//----------------------------------------------//


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
	addr_abs += y_reg;

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


//----------------> INSTRUCTIONS <----------------------//

//Branch if Carry Set: All branching fucntions require 1 extra cycle by default (so as to go to next instruction)
//Also if the branch occurs are at a page change then one more additional cycle
uint8_t Nes_Em_6502::BCS() {
	//Check if Carry flag is set ('on')
	if (getFlag(C) == 1) {
		cycles++;											//Default extra cycle
		addr_abs = progcount_reg + addr_rel;				//Getting the location of the next instruction 
															//(rel as mentioned in the datasheet)

		//Checking if the current instruction and next instruction are on the same page. If not then extra cycle time
		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {			
			cycles++;
		}

		//setting the location of the new instruction in the prog_counter
		progcount_reg = addr_abs;
	}

	//To Check: Should return 1 I guess as this fuction may take extra time.
	return 0;
}

//Similarly BEQ: Branch if Equal i.e. Zero flag is set
uint8_t Nes_Em_6502::BEQ() {
	//Check if Zero flag is set ('on')
	if (getFlag(Z) == 1) {
		cycles++;											//Default extra cycle
		addr_abs = progcount_reg + addr_rel;				//Getting the location of the next instruction 
															//(rel as mentioned in the datasheet)

		//Checking if the current instruction and next instruction are on the same page. If not then extra cycle time
		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		//setting the location of the new instruction in the prog_counter
		progcount_reg = addr_abs;
	}

	//To Check: Should return 1 I guess as this fuction may take extra time.
	return 0;
}

//Similarly Branch if Not Equal i.e. Zero flag not set ('off')
uint8_t Nes_Em_6502::BNE() {
	//Check if Carry flag is set ('on')
	if (getFlag(Z) == 0) {
		cycles++;											//Default extra cycle
		addr_abs = progcount_reg + addr_rel;				//Getting the location of the next instruction 
															//(rel as mentioned in the datasheet)

		//Checking if the current instruction and next instruction are on the same page. If not then extra cycle time
		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		//setting the location of the new instruction in the prog_counter
		progcount_reg = addr_abs;
	}

	//To Check: Should return 1 I guess as this fuction may take extra time.
	return 0;
}

//Similarly BEQ: Branch if Overflow Set i.e. Overflow flag is set
uint8_t Nes_Em_6502::BVS() {
	//Check if Carry flag is set ('on')
	if (getFlag(V) == 1) {
		cycles++;											//Default extra cycle
		addr_abs = progcount_reg + addr_rel;				//Getting the location of the next instruction 
															//(rel as mentioned in the datasheet)

		//Checking if the current instruction and next instruction are on the same page. If not then extra cycle time
		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		//setting the location of the new instruction in the prog_counter
		progcount_reg = addr_abs;
	}

	//To Check: Should return 1 I guess as this fuction may take extra time.
	return 0;
}

//Redundant Function actually, cause NES here doesn't use this mode (Decimal Mode)
//Clear the Decimal Flag
uint8_t Nes_Em_6502::CLD() {
	setFlag(D, false);
	return 0;
}

//Clear the Overflow flag
uint8_t Nes_Em_6502::CLV() {
	setFlag(V, false);
	return 0;
}

//Compare functions, compare the value fetched with the value at Accumulator/X/Y reg
//Sets flags as follows:
//N - if Accum/X/Y < fetched		C - if Accum/X/Y > fetched		C,Z - if Accum/X/Y = fetched
uint8_t Nes_Em_6502::CMP() {
	fetch();
	if (accum_reg >= fetched) {
		setFlag(C, true);
		if (fetched == accum_reg) {
			setFlag(Z, true);
		}
	}
	else {
		setFlag(N, true);
	}
}

uint8_t Nes_Em_6502::CPX() {
	fetch();
	if (x_reg >= fetched) {
		setFlag(C, true);
		if (fetched == x_reg) {
			setFlag(Z, true);
		}
	}
	else {
		setFlag(N, true);
	}
}

uint8_t Nes_Em_6502::CPY() {
	fetch();
	if (y_reg >= fetched) {
		setFlag(C, true);
		if (fetched == y_reg) {
			setFlag(Z, true);
		}
	}
	else {
		setFlag(N, true);
	}
}

//Decrementing Functions, decrement the value stored at the memory location/X by 1 and re-write it to the
//same location/X.
//Flags set as follows:
//N - if new value <0		Z - if new value == 0
uint8_t Nes_Em_6502::DEC() {
	fetch();
	uint8_t temp = fetched - 1;
	write(addr_abs, temp);
	setFlag(N, temp & 0x80);
	setFlag(Z, temp == 0x00);
	return 0;
}

uint8_t Nes_Em_6502::DEX() {
	x_reg--;
	setFlag(N, x_reg & 0x80);
	setFlag(Z, x_reg == 0x00);
	return 0;
}

//Incrementing Functions, increment the value stored at the memory location/X by 1 and re-write to the
//same location/X.
//Flags set as follows:
//N - if new value <0		Z - if new value == 0

uint8_t Nes_Em_6502::INC() {
	fetch();
	uint8_t temp = fetched + 1;
	write(addr_abs, temp);
	setFlag(N, temp & 0x80);
	setFlag(Z, temp == 0x00);
	return 0;
}

uint8_t Nes_Em_6502::INX() {
	x_reg++;
	setFlag(N, x_reg & 0x80);
	setFlag(Z, x_reg == 0x00);
	return 0;
}


//Instead of subtracting the numbers, we are adding the negative of the number
//Also for the borrow we add the inverse of carry i.e. 1-C
uint8_t Nes_Em_6502::SBC() {
	fetch();
	
	//Inverting the fetched value i.e. getting the negative of that number using 
	//Two's complement
	uint8_t value = (fetched ^ 0xFF) + 1;

	//The negative addition with borrow.
	//If Carry is 1 then borrow 0 and vice-versa
	uint16_t temp;
	if (getFlag(C) == 1) {
		temp = accum_reg + value;
	}
	else {
		temp = accum_reg + value + 1;
	}

	//Setting the cary flag for subsequent operations 
	//By checking if any bit has overflowed into higher word
	setFlag(C, (temp & 0xFF00));

	//Setting the Zero flag to indicate that the current operation resulted
	//in 0 as a result
	setFlag(Z, (temp & 0x00FF) == 0);

	//Setting the overflow flag as per the following table: 
	//Note: Using the MSB (0 - Positive, 1 - Negative)
	//Note2: Setting the Negative with the overflow flag itself
	//Accum			Data			Result			Overflow
	//	0			 0				  1				  Yes
	//	1			 1				  0				  Yes
	//Else no overflow
	if ((accum_reg & 0x80) && (value & 0x80)) {
		if (temp & 0x0080) {
			setFlag(V, true);
			//Also setting the negative flag 
			setFlag(N, true);
		}
	} else if(!(accum_reg & 0x80) && !(value & 0x80)) {
		if (!(temp & 0x0080)) {
			setFlag(V, true);
			setFlag(N, false);
		}
	} else {
		setFlag(V, false);
		setFlag(N, (temp & 0x0080));
	}

	//As the function may require more time
	return 1;
}

//Addition with Carry
uint8_t Nes_Em_6502::ADC() {
	fetch();

	//Adding the accum and fetched alongwith the carry flag
	uint16_t sum = (uint16_t)accum_reg + (uint16_t)fetched + (uint16_t)getFlag(C);

	//Setting the Carry flag if the value of sum is greater than 8 bits
	setFlag(C, sum > 255);

	//Setting the Zero flag if sum is zero
	setFlag(Z, (sum & 0x00FF) == 0);

	//Setting the Negative flag by checking the MSB 
	setFlag(N, sum & 0x80);

	//Setting the overflow flag as per the following table: 
	//Note: Using the MSB (0 - Positive, 1 - Negative)
	//Note2: Setting the Negative with the overflow flag itself
	//Accum			Data			Result			Overflow
	//	0			 0				  1				  Yes
	//	1			 1				  0				  Yes
	//Else no overflow
	if ((accum_reg & 0x80) && (fetched & 0x80)) {
		if (!(sum & 0x0080)) {
			setFlag(V, true);
		}
	}
	else if (!(accum_reg & 0x80) && !(fetched & 0x80)) {
		if (sum & 0x0080) {
			setFlag(V, true);
		}
	}
	else {
		setFlag(V, false);
	}

	accum_reg = sum & 0x00FF;
	return 1;
}

//Performs logical Bitwise And operation of accum and memory read and stores it back in accum.
//Flags set: N, Z
uint8_t Nes_Em_6502::AND() {
	fetch();
	accum_reg = accum_reg & fetched;

	setFlag(N, accum_reg & 0x80);
	setFlag(Z, accum_reg == 0x00);

	return 1;
}

// Add the relative offset to the absolute address if carry flag is set
uint8_t Nes_Em_6502::BCS() {
	if (getFlag(C) == 1) {
		cycles++;
		addr_abs = progcount_reg + addr_rel;

		//If the addr_abs and progcount aren't on the same page then extra cycle needed
		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		progcount_reg = addr_abs;
	}
	return 0;
}

// Branch if zero bit is set
uint8_t Nes_Em_6502::BEQ() {
	if (getFlag(Z) == 1) {
		cycles++;
		addr_abs = progcount_reg + addr_rel;

		//If the addr_abs and progcount aren't on the same page then extra cycle needed
		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		progcount_reg = addr_abs;
	}
	return 0;
}

// Branch if zero bit is not set
uint8_t Nes_Em_6502::BNE() {
	if (getFlag(Z) == 0) {
		cycles++;
		addr_abs = progcount_reg + addr_rel;

		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		progcount_reg = addr_abs;
	}
	return 0;
}

// Branch if minus bit is set
uint8_t Nes_Em_6502::BMI() {
	if (getFlag(N) == 1) {
		cycles++;
		addr_abs = progcount_reg + addr_rel;

		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		progcount_reg = addr_abs;
	}
	return 0;
}

// Branch if carry bit is clear
uint8_t Nes_Em_6502::BCC() {
	if (getFlag(C) == 0) {
		cycles++;
		addr_abs = progcount_reg + addr_rel;

		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		progcount_reg = addr_abs;
	}
	return 0;
}

// Branch if overflow bit is clear
uint8_t Nes_Em_6502::BVC() {
	if (getFlag(V) == 0) {
		cycles++;
		addr_abs = progcount_reg + addr_rel;

		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}

		progcount_reg = addr_abs;
	}
	return 0;
}

// Branch if overflow bit is set
uint8_t Nes_Em_6502::BVS() {
	if (getFlag(V) == 1) {
		cycles++;
		addr_abs = progcount_reg + addr_rel;

		if ((addr_abs & 0xFF00) != (progcount_reg & 0xFF00)) {
			cycles++;
		}
			
		progcount_reg = addr_abs;
	}
	return 0;
}

// Clear the carry bit
uint8_t Nes_Em_6502::CLC()
{
	setFlag(C, false);
	return 0;
}

// Clear the interrupt bit
uint8_t Nes_Em_6502::CLI()
{
	setFlag(I, false);
	return 0;
}

// Clear the decimal bit
uint8_t Nes_Em_6502::CLD()
{
	setFlag(D, false);
	return 0;
}

// Clear the overflow flag
uint8_t Nes_Em_6502::CLV()
{
	setFlag(V, false);
	return 0;
}

// Set the carry flag
uint8_t Nes_Em_6502::SEC()
{
	setFlag(C, true);
	return 0;
}

// Set the decimal flag
uint8_t Nes_Em_6502::SED()
{
	setFlag(D, true);
	return 0;
}

// Set the interrupt flag
uint8_t Nes_Em_6502::SEI()
{
	setFlag(I, true);
	return 0;
}

// Push the accumulator to the top of the Stack
uint8_t Nes_Em_6502::PHA()
{
	write(0x0100 + stkptr_reg, accum_reg);
	stkptr_reg--;
	return 0;
}

// Pops the accumulator to the top of the Stack
uint8_t Nes_Em_6502::PLA()
{
	stkptr_reg++;
	accum_reg = read(0x0100 + stkptr_reg);
	setFlag(Z, accum_reg == 0x00);
	setFlag(N, accum_reg & 0x80);
	return 0;
}

// Return from the interrupt state
uint8_t Nes_Em_6502::RTI()
{
	stkptr_reg++;
	status_reg = read(0x0100 + stkptr_reg);
	status_reg = status_reg & ~B;
	status_reg = status_reg & ~U;
	stkptr_reg++;
	progcount_reg = (uint16_t)read(0x0100 + stkptr_reg);
	stkptr_reg++;
	progcount_reg = progcount_reg | (uint16_t)read(0x0100 + stkptr_reg) << 8;
	return 0;
}

// Program sourced Interrupt
uint8_t Nes_Em_6502::BRK() {
	progcount_reg++;
	write(0x0100 + stkptr_reg, progcount_reg >> 8 & 0x00FF);
	stkptr_reg--;
	write(0x0100 + stkptr_reg, progcount_reg & 0x00FF);
	stkptr_reg--;
	setFlag(B, 1);
	write(0x0100 + stkptr_reg, status_reg);
	stkptr_reg--;
	setFlag(B, 0);
	progcount_reg = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;

}

// Arithmetic Left shift
uint8_t Nes_Em_6502::ASL() {
	fetch();
	setFlag(C, fetched & 0x80);
	uint8_t temp = fetched << 1;
	setFlag(Z, temp == 0x00);
	setFlag(N, temp & 0x80);
	if (opCodeLookUpTable[opcode].addrmode == &Nes_Em_6502::IMP)
		accum_reg = (uint16_t)temp;
	else
		write(addr_abs, (uint16_t)temp);
	return 0;

}

// Test bits
uint8_t Nes_Em_6502::BIT() {
	fetch();
	uint16_t temp = accum_reg & fetched;
	setFlag(Z, (temp & 0x00FF) == 0x00);
	setFlag(N, fetched & (1 << 7));
	setFlag(V, fetched & (1 << 6));
	return 0;
}

// Exclusive OR
uint8_t Nes_Em_6502::EOR() {
	fetch();
	accum_reg = accum_reg ^ fetched;
	setFlag(Z, accum_reg == 0x00);
	setFlag(N, accum_reg & 0x80);
	return 1;
}

// Decrement y
uint8_t Nes_Em_6502::DEY() {
	y_reg--;
	setFlag(Z, y_reg == 0x00);
	setFlag(N, y_reg & 0x80);
	return 0;
}

// Decrement x
uint8_t Nes_Em_6502::DEX(){
	x_reg--;
	setFlag(Z, x_reg == 0x00);
	setFlag(N, x_reg & 0x80);
	return 0;
}

// Increment y
uint8_t Nes_Em_6502::INY() {
	  y_reg++;
	  setFlag(Z, y_reg == 0x00);
	  setFlag(N, y_reg & 0x80);
	  return 0;
}

// Increment x
uint8_t Nes_Em_6502::INX(){
	x_reg++;
	setFlag(Z, x_reg == 0x00);
	setFlag(N, x_reg & 0x80);
	return 0;
}

//------------------------------------------------------//