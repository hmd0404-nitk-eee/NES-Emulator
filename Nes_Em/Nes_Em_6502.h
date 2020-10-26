#pragma once
#include <cstdint>
#include <string>
#include <vector>

//Forward Declaration of the Bus class for the connection
class Bus;

class Nes_Em_6502
{
private:
	Bus* bus;

	//CPU's read write functions internally call the bus' r/w functions.
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

public:
	//Registers in the CPU
	uint8_t status_reg = 0x00;				//Status Register
	uint8_t accum_reg = 0x00;				// Accumulator Register
	uint8_t x_reg = 0x00;					// X Register
	uint8_t y_reg = 0x00;					// Y Register
	uint8_t stkptr_reg = 0x00;				// Stack Pointer (points to location on bus)
	uint16_t progcount_reg = 0x0000;		// Program Counter

	//The enums for handling the status of the cpu stored in Status register
	enum FLAGS_6502
	{
		C = (1 << 0),	// Carry Bit
		Z = (1 << 1),	// Zero
		I = (1 << 2),	// Disable Interrupts
		D = (1 << 3),	// Decimal Mode (N/A)
		B = (1 << 4),	// Break
		U = (1 << 5),	// Unused
		V = (1 << 6),	// Overflow
		N = (1 << 7),	// Negative
	};

	//Main CPU functions
	void connectBus(Bus* busAddr);	//Called in Constructor of the Bus to connect the Cpu to the Bus


	//----> External event Functions
	void clock();					//Updates or performs one clock cycle long task
	//-----------> Status Change Functions (Interupts)
	void reset();					//The reset request function
	void interupt_req();			//Interupts that can be ignored
	void non_mask_interupt_req();	//Interupts that must be handled and cannot be ignored

	//Construstor and Destructors for Nes_Em_6502 class
	Nes_Em_6502();
	~Nes_Em_6502();

private:
	uint8_t getFlag(FLAGS_6502 flag);				//Functions for 
	void setFlag(FLAGS_6502 flag, bool value);		//Flag handling

	uint8_t  fetched = 0x00;			// Represents the working input value to the ALU
	uint16_t addr_abs = 0x0000;			// All used memory addresses end up in here
	uint16_t addr_rel = 0x00;			// Represents absolute address following a branch
	uint8_t  opcode = 0x00;				// the opcode itslef
	uint8_t  cycles = 0;				// Counts how many cycles the instruction has remaining
	uint32_t clock_count = 0;			//A counter for number of clocks

	//To fetch the data from two sources, a memory address, or
	//its immediately available as part of the instruction. This function decides
	//depending on address mode of instruction byte
	uint8_t fetch();

	//The struct for the instructions 
	struct INSTRUCTION
	{
		std::string name;											//Stores the OP code as Text
		uint8_t(Nes_Em_6502:: *operate)(void) = nullptr;			//The actual fuction which is to called to perform the OP
		uint8_t(Nes_Em_6502:: *addrmode)(void) = nullptr;			//The address mode the OP-Code uses
		uint8_t cycles = 0;											//The number of cycles required for the Instruction
	};

	//The lookup Table (16x16) which contains the OP-Code
	std::vector<INSTRUCTION> opCodeLookUpTable;

private:
	//The 12 Addressing modes used in the CPU 6502
	//Determines the actual length of instruction in bytes and
	//also the amount of clock cycles required for that instruction
	
	uint8_t IMP();	uint8_t IMM(); 
	uint8_t ZP0();	uint8_t ZPX();
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();
	uint8_t ABY();	uint8_t IND();
	uint8_t IZX();	uint8_t IZY();

private:
	//The 56 legal OP-Codes provided by the 6502 CPU
	//
	// <To CHECK> The functions return 0 under normal conditions but 
	// ----------> may return 1 under certain combined conditions which may require
	// ----------> the function to take more clock cycles

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

	//All illegal OP-Codes and the NOP (No Operation) Code are handled by
	//the function below

	uint8_t XXX();  //i.e. ILL or NOP
};

