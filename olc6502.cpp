#include "olc6502.h"


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

olc6502::~olc6502()
{

}

uint8_t olc6502::read(uint16_t a){
  return bus->read(a,false);
}

uint8_t olc6502::write(uint16_t a, uint8_t d){
  return bus->write(a, d);
}

void olc6502::clock()
{
  if (cycles == 0)
  {
    opcode = read(pc);
    pc++;

    cycles = lookup[opcode].cycles;
    uint8_t additional_cycles1 = (this->*lookup[opcode].addrmode)();

    uint8_t additional_cycles2 = (this->*lookup[opcode].operate)();

    cycles += (additional_cycles1 & additional_cycles2);

  }
  cycles--;
}


void olc6502::reset(){
  a=0;
  x=0;
  y=0;
  stkp=0xFD;
  status = 0x00 | U;
  addr_abs = 0xFFFC;
  uint16_t low = read(addr_abs+0);
  uint16_t high = read(addr_abs+1);

  pc = (high << 8)| low;
  addr_rel = 0x0000;
  addr_abs = 0x0000;
  fetched = 0x00;
  cycles =8;
}


void olc6502::irq()
{
  if (GetFlag(I) == 0){
    write(0x0100 + stkp, (pc>>8)& 0x00FF);
    stkp--;
    write(0x0100 + stkp,  pc & 0x00FF);
    stkp--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    write(0x0100 + stkp, status);
    stkp--;

    addr_abs = 0xFFFE;
    uint16_t low = read(addr_abs+0);
    uint16_t high = read(addr_abs+1);
    pc = (high << 8)| low;

    cycles = 7;
  }
}

void olc6502::nmi()
{

    write(0x0100 + stkp, (pc>>8)& 0x00FF);
    stkp--;
    write(0x0100 + stkp,  pc & 0x00FF);
    stkp--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    write(0x0100 + stkp, status);
    stkp--;

    addr_abs = 0xFFFA;
    uint16_t low = read(addr_abs+0);
    uint16_t high = read(addr_abs+1);
    pc = (high << 8)| low;

    cycles = 8;

}

uint8_t olc6502::GetFlag(FLAGS6502 f)
{
	return ((status & f) > 0) ? 1 : 0;
}

// Sets or clears a specific bit of the status register
void olc6502::SetFlag(FLAGS6502 f, bool v)
{
	if (v)
		status |= f;
	else
		status &= ~f;
}

// Addressing modes

uint8_t olc6502::IMP(){
  fetched = a;
  return 0;
}

uint8_t olc6502::IMM(){
  addr_abs = pc;
  pc++;
  return 0;
}

uint8_t olc6502::ZP0(){
  addr_abs = read(pc);
  pc++;
  addr_abs = addr_abs & 0x00FF;
  return 0;
}

uint8_t olc6502::ZPX(){
  addr_abs = (read(pc) + x);
  pc++;
  addr_abs = addr_abs & 0x00FF;
  return 0;
}

uint8_t olc6502::ZPY(){
  addr_abs = (read(pc) + y);
  pc++;
  addr_abs = addr_abs & 0x00FF;
  return 0;
}

uint8_t olc6502::ABS()
{
  uint16_t low =read(pc);
  pc++;
  uint16_t high = read(pc);
  pc++;

  addr_abs = (high<<8)| low;
  return 0;
}

uint8_t olc6502::ABX()
{
  uint16_t low =read(pc);
  pc++;
  uint16_t high = read(pc);//Why is high reading from a different pc?
  pc++;

  addr_abs = (high<<8)| low;
  addr_abs +=x;
  if ((addr_abs & 0xFF00) ! = (high << 8 ))
    return 1;
  else
    return 0;
}

uint8_t olc6502::ABY()
{
  uint16_t low =read(pc);
  pc++;
  uint16_t high = read(pc);
  pc++;

  addr_abs = (high<<8)| low;
  addr_abs +=y;
  if ((addr_abs & 0xFF00) ! = (high << 8 ))
    return 1;
  else
    return 0;
}

uint8_t olc6502::IND()
{
  uint16_t ptr_low =read(pc);
  pc++;
  uint16_t ptr_high = read(pc);
  pc++;
  uint16_t ptr = (ptr_high<<8)| ptr_low;
  if(ptr_low == 0x00FF){
    addr_abs = (read(ptr & 0xFF00) << 8) | read (ptr + 0);
  }
  else {
    addr_abs = (read(ptr + 1 ) << 8) | read (ptr + 0);//Why is ptr being incremented?

  }
  return 0;
}

uint8_t olc6502::IZX(){
  uint16_t t = read(pc);
  pc++;

  uint16_t low = read(uint16_t( t + (uint16_t x)) & 0x00FF);
  uint16_t high = read(uint16_t( t + (uint16_t x +1)) & 0x00FF);
  addr_abs = (high << 8) | low;
  return 0;
}


uint8_t olc6502::IZY(){
  uint16_t t = read(pc);
  pc++;
  uint16_t low = read(t & 0x00FF);
  uint16_t high = read((t+1) & 0x00FF);
  pc++;
  addr_abs = (high << 8) | low;
  addr_abs += y;
  if ((addr_abs & 0xFF00) != (high << 8))
    return 1;
  else
    return 0;
}

 uint8t olc6502::REL()
 {
   addr_rel = read (pc);
   pc++;
   if (addr_rel & 0x80){
     addr_rel |= 0xFF00;
   }

 }


 // fetches the address
 uint8_t olc6502::fetch(){
   if(lookup[opcode].addrmode!= &olc6502::IMP){
     fetched = read(addr_abs);
   }
   return fetched;
 }

 // Instruction codes
 //1.
// perform and operation
 uint8t olc6502::ADD()
 {
   fetch();
   a = a & fetched;
   SetFlag(Z, a==0x00);
   SetFlag(N, a & 0x80);
   return 1;
 }
 //2.
// Add the relative offset to the absolute address if carry flag is set
 uint8_t olc6502:BCS(){
   if(GetFlag(C)==1){
     cycles++;
     addr_abs = pc + addr_rel;
     if((addr_abs & 0xFF00) != (pc & 0xFF00))
        cycles++;
     pc = addr_abs;
   }
   return 0;
 }
 //3.
// Branch if zero bit is set
 uint8_t olc6502:BEQ(){
   if(GetFlag(Z)==1){
     cycles++;
     addr_abs = pc + addr_rel;
     if((addr_abs & 0xFF00) != (pc & 0xFF00))
        cycles++;
     pc = addr_abs;
   }
   return 0;
 }
 //4.
 // Branch if zero bit is not set
 uint8_t olc6502:BNE(){
    if(GetFlag(Z)==0){
      cycles++;
      addr_abs = pc + addr_rel;
      if((addr_abs & 0xFF00) != (pc & 0xFF00))
         cycles++;
      pc = addr_abs;
    }
    return 0;
  }
 //5.
  // Branch if minus bit is set
   uint8_t olc6502:BMI(){
     if(GetFlag(N)==1){
       cycles++;
       addr_abs = pc + addr_rel;
       if((addr_abs & 0xFF00) != (pc & 0xFF00))
          cycles++;
       pc = addr_abs;
     }
     return 0;
   }
 //6.
   // Branch if carry bit is clear
  uint8_t olc6502:BCC(){
    if(GetFlag(C)==0){
      cycles++;
      addr_abs = pc + addr_rel;
      if((addr_abs & 0xFF00) != (pc & 0xFF00))
         cycles++;
      pc = addr_abs;
    }
    return 0;
    }
 //7.
    // Branch if minus bit is clear
 uint8_t olc6502:BPL(){
    if(GetFlag(N)==0){
       cycles++;
     addr_abs = pc + addr_rel;
     if((addr_abs & 0xFF00) != (pc & 0xFF00))
        cycles++;
     pc = addr_abs;
   }
   return 0;
  }
 //8.
// Branch if overflow bit is clear
uint8_t olc6502:BVC(){
  if(GetFlag(V)==0){
     cycles++;
   addr_abs = pc + addr_rel;
   if((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;
   pc = addr_abs;
 }
 return 0;
}
 //9.
// Branch if overflow bit is set
uint8_t olc6502:BVS(){
  if(GetFlag(V)==1){
     cycles++;
   addr_abs = pc + addr_rel;
   if((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;
   pc = addr_abs;
 }
 return 0;
}

 //10.
// Clear the carry bit
 uint8t olc6502::CLC()
 {
   SetFlag(C, false);
   return 0;
 }


//11.
 // Clear the interrupt bit
  uint8t olc6502::CLI()
  {
    SetFlag(I, false);
    return 0;
  }

//12.
  // Clear the decimal bit
   uint8t olc6502::CLD()
   {
     SetFlag(D, false);
     return 0;
   }

//13.
   // Clear the overflow flag
    uint8t olc6502::CLV()
    {
      SetFlag(V, false);
      return 0;
    }
//14.
    // Set the carry flag
    uint8t olc6502::SEC()
    {
      SetFlag(C, true);
      return 0;
    }

//15.
    // Set the decimal flag
    uint8t olc6502::SED()
    {
      SetFlag(D, true);
      return 0;
    }
//16.
    // Set the interrupt flag
    uint8t olc6502::SEI()
    {
      SetFlag(I, true);
      return 0;
    }

//17.
    //Addition with Carry
    uint8t olc6502::ADC(){
      fetch();
      uint16_t sum = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);
      SetFlag(C, sum>255);
      SetFlag(Z, (sum & 0x00FF) == 0);
      SetFlag(N, sum&0x80);
      SetFlag(V, (((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)sum)) & 0x0080);
      a = temp & 0x00FF;
      return 1;
    }
//18.
    //Subtraction with Carry
    uint8t olc6502::SBC(){
      fetch();
      uint8_t value = ((uint16_t)fetched ^ 0x00FF)
      uint16_t difference = (uint16_t)a + value + (uint16_t)GetFlag(C);
      SetFlag(C, difference>255);
      SetFlag(Z, (difference & 0x00FF) == 0);
      SetFlag(N, difference&0x0080);
      SetFlag(V, (((uint16_t)a ^ difference) & ((uint16_t)a ^ value)) & 0x0080);
      a = temp & 0x00FF;
      return 1;
    }

//19.
    // Push the accumulator to the top of the Stack
    uint8_t olc6502::PHA()
    {
      write(0x0100 + stkp, a);
      stkp--;
      return 0;
    }

//20.
    // Pops the accumulator to the top of the Stack
    uint8_t olc6502::PLA()
    {
      stkp++;
      a = read(0x0100 + stkp);
      SetFlag(Z, a == 0x00);
      SetFlag(N, a & 0x80);
      return 0;
    }

//21.
    // Return from the interrupt state
    uint8_t olc6502::RTI()
    {
      stkp++;
      status = read(0x0100 + stkp);
      status = status & ~B;
      status = status & ~U;
      stkp++;
      pc = (uint16_t)read(0x0100 + stkp);
      stkp++;
      pc = pc | (uint16_t)read(0x0100 + stkp)<<8;
      return 0;
    }
//22.
    // Program sourced Interrupt
    uint8_t olc6502::BRK(){
      pc++;
      write(0x0100 + stkp, pc>>8 & 0x00FF);
      stkp--;
      write(0x0100 + stkp, pc & 0x00FF);
      stkp--;
      SetFlag(B, 1);
      write(0x0100 + stkp, status);
      stkp--;
      SetFlag(B, 0);
      pc = (uint16_t)read(0xFFFE)|((uint16_t) read(0xFFFF) << 8);
      return 0;

    }

    //23.
// Arithmetic Left shift
    uint8_t olc6502::ASL(){
      fetch();
      SetFlag(C, fetched & 0x80);
      uint8_t temp = fetched << 1;
      SetFlag(Z, temp==0x00);
      SetFlag(N, temp & 0x80);
      if (lookup[opcode].addrmode == &olc6502::IMP)
		     a = (uint16_t) temp;
	    else
		    write(addr_abs, (uint16_t)temp);
	    return 0;

    }

    //24.
 // And bitwise operation
    uint8_t olc6502::AND(){
      fetch();
      a = fetched & a;
      SetFlag(Z, a==0x00);
      SetFlag(N, a & 0x80);
      return 1;
    }

  //25.
// Test bits
    uint8_t olc6502::BIT(){
      fetch();
      temp = a & fetched;
      SetFlag(Z, (temp & 0x00FF)==0x00);
      SetFlag(N, fetched & (1<<7) );
      SetFlag(V, fetched & (1<<6) );
      return 0;
    }

  //26.
// Exclusive OR
    uint8_t olc6502::EOR(){
      fetch();
      a = a ^ fetched;
      SetFlag(Z, a==0x00);
      SetFlag(N, a & 0x80);
      return 1;
    }

  //27.
// Decrement y
    uint8_t olc6502::DEY(){
      y--;
      SetFlag(Z, y == 0x00);
      SetFlag(N, y & 0x80);
      return 0;
    }

  //28.
// Decrement x
    uint8_t olc6502::DEX()){
      x--;
      SetFlag(Z, x == 0x00);
      SetFlag(N, x & 0x80);
      return 0;
    }
  //29.
// Increment y
    uint8_t olc6502::INY(){
      y++;
      SetFlag(Z, y == 0x00);
      SetFlag(N, y & 0x80);
      return 0;
    }

    //30. 
  // Increment x
   uint8_t olc6502::INX()){
      x++;
      SetFlag(Z, x == 0x00);
      SetFlag(N, x & 0x80);
      return 0;
    }
    //31
    //Compare functions, compare the value fetched with the value at Accumulator/X/Y reg
    //Sets flags as follows:
    //N - if Accum/X/Y < fetched		C - if Accum/X/Y > fetched		C,Z - if Accum/X/Y = fetched
    uint8_t olc6502::CMP() {
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
// 32
  uint8_t olc6502::CPX() {
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

// 33
uint8_t olc6502::CPY() {
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
// 34
//Decrementing Functions, decrement the value stored at the memory location/X by 1 and re-write it to the
//same location/X.
//Flags set as follows:
//N - if new value <0		Z - if new value == 0
uint8_t olc6502::DEC() {
	fetch();
	uint8_t temp = fetched - 1;
	write(addr_abs, temp);
	setFlag(N, temp & 0x80);
	setFlag(Z, temp == 0x00);
	return 0;
}

// 35
//Incrementing Functions, increment the value stored at the memory location/X by 1 and re-write to the
//same location/X.
//Flags set as follows:
//N - if new value <0		Z - if new value == 0

uint8_t olc6502::INC() {
	fetch();
	uint8_t temp = fetched + 1;
	write(addr_abs, temp);
	setFlag(N, temp & 0x80);
	setFlag(Z, temp == 0x00);
	return 0;
}
// 36
// Instruction: Jump To Location
// Function:    pc = address
uint8_t olc6502::JMP() {
	progcount_reg = addr_abs;
	return 0;
}
// 37 
// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t olc6502::JSR() {
	progcount_reg--;
	write(0x0100 + stkptr_reg, (progcount_reg >> 8) & 0x00FF);
	stkptr_reg--;
	write(0x0100 + stkptr_reg, progcount_reg & 0x00FF);
	stkptr_reg--;

	progcount_reg = addr_abs;
	return 0;
}
// 38
// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t olc6502::LDA() {
	fetch();

	accum_reg = fetched;
	setFlag(N, accum_reg & 0x80);
	setFlag(Z, accum_reg == 0x00);
	return 0;
}
// 39
// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t olc6502::LDX() {
	fetch();
	x_reg = fetched;
	setFlag(N, x_reg & 0x80);
	setFlag(Z, x_reg == 0x00);
	return 0;
}
// 40
// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t olc6502::LDY() {
	fetch();
	y_reg = fetched;
	setFlag(N, y_reg & 0x80);
	setFlag(Z, y_reg == 0x00);
	return 0;
}
// 41
// instruction: logical shift right
// LSR shifts all bits right one position. 0 is shifted into bit 7 and the original bit 0 is shifted into the Carry.
// flages out : Z,N ,C
uint8_t olc6502::LSR() {
	fetch();
	setFlag(C, fetched & 0x0001);
	uint16_t temp = fetched >> 1;
	setFlag(Z, (temp && 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	if (opCodeLookUpTable[opcode].addrmode == &olc6502::IMP) {
		accum_reg = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF);
	}

	return 0;


}
// 42
//NOP is a mnemonic that stands for No Operation. This instruction does nothing during execution
uint8_t olc6502::NOP() {
	switch (opcode) {
		case 0x1C:
		case 0x3C:
		case 0x5C:
		case 0x7C:
		case 0xDC:
		case 0xFC:
			return 1;
	}
	return 0;

}
// 43
// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t olc6502::ORA() {
	fetch();
	accum_reg = accum_reg | fetched;
	setFlag(Z, accum_reg == 0x00);
	setFlag(N, accum_reg & 0x80);
	return 0;
}
// 44
// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t olc6502::PHP() {
	write(0x0100 + stkptr_reg, status_reg | B | U);
	stkptr_reg--;
	setFlag(B, 0);
	setFlag(U, 0);
	return 0;
}
// 45
// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t olc6502::PLP() {
	stkptr_reg++;
	status_reg = read(0x0100 + stkptr_reg);
	setFlag(U, 1);
	return 0;
}

// 46
//The rotate left (ROL) and rotate through carry left (RCL) instructions shift all the bits toward more-significant bit positions,
//except for the most-significant bit, which is rotated to the least significant bit location
// set flage : N,Z,C;
uint8_t olc6502::ROL() {
	fetch();
	uint16_t temp = (uint16_t)(fetched << 1) | getFlag(C);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, (temp & 0x00FF) == 0x0000);
	setFlag(N, temp & 0x0080);
	if (opCodeLookUpTable[opcode].addrmode == &olc6502::IMP) {
		accum_reg = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF);
	}
		
	return 0;
}
// 47
//The rotate right (ROR) and rotate through carry right (RCR) instructions shift all the bits toward less significant bit positions
// except for the least-significant bit, which is rotated to the most-significant bit location
//set flage : C,N,Z
uint8_t olc6502::ROR() {
	fetch();
	uint16_t temp = (uint16_t)(fetched >> 1) | (getFlag(C) << 7);
	setFlag(C, temp & 0xFF00);
	setFlag(Z, temp & 0x00FF == 0x0000);
	setFlag(N, temp & 0x0080);
	if (opCodeLookUpTable[opcode].addrmode == &olc6502::IMP) {
		accum_reg = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF);
	}
		
	return 0;
}
// 48
//RTS    ---   RTS stands for Return from Subroutine
//RTS is one of the 6502 Subroutine Operations of 6502 instruction-set.
//The function of RTS is to pulls the top two bytes off the stack (low byte first)
// and transfers program control to that address+1 i.e.return (RTS)  from the calling subroutine
uint8_t olc6502::RTS() {
	stkptr_reg++;
	progcount_reg = (uint16_t)read(0x0100 + stkptr_reg);
	stkptr_reg++;
	progcount_reg |= (uint16_t)read(0x0100 + stkptr_reg) << 8;
	progcount_reg++;
	return 0;
}


// 49
// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t olc6502::STA() {
	write(addr_abs, accum_reg);
	return 0;
}
// 50
// Instruction: Store X Register at Address
// Function:    M = X
uint8_t olc6502::STX() {
	write(addr_abs, x_reg);
	return 0;
}
// 51
// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t olc6502::STY() {
	write(addr_abs, y_reg);
	return 0;
}
// 52
// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t olc6502::TAX() {
	x_reg = accum_reg;
	setFlag(N, x_reg & 0x80);
	setFlag(Z, x_reg == 0x00);
	return 0;
}
// 53
// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t olc6502::TAY() {
	y_reg = accum_reg;
	setFlag(N, y_reg & 0x80);
	setFlag(Z, y_reg == 0x00);
	return 0;
}
// 54
// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t olc6502::TSX() {
	x_reg = stkptr_reg;
	setFlag(N, x_reg & 0x80);
	setFlag(Z, x_reg == 0x00);
	return 0;
}
// 55
// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t olc6502::TXA() {
	accum_reg = x_reg;
	setFlag(N, accum_reg & 0x80);
	setFlag(Z, accum_reg == 0x00);
	return 0;
}
// 56
// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t olc6502::TXS() {
	stkptr_reg = x_reg;
	return 0;
}
// 57
// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t olc6502::TYA() {
	accum_reg = y_reg;
	setFlag(Z, accum_reg == 0x00);
	setFlag(N, accum_reg & 0x80);
	return 0;
}

// 58
// all illegal opcodes capturing
uint8_t olc6502::XXX() {
	return 0;
}
