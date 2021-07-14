#include<iostream>
#include "type_definitions.h"
using namespace std;

Byte twoscomplement(Byte var)
{
	var = ~var;
	var++;
	return var;
}

struct Memory
{
	static constexpr DWord MAX_MEM = 65 * 1024; //65Kb of memory

	Byte memory_block[MAX_MEM];

};


struct cpu
{
	Byte stack_pointer;
	Word program_counter;
	Word instruction_register;

	//clock cycles
	short int cycles;

	//General purpose registers
	Byte A;
	Byte X;
	Byte Y;

	//Status register
	Byte status_byte = 0b00100000;


	//initializing register and flag values
	void ResetCPU()
	{
		stack_pointer = 0xFF;
		program_counter = 0xFFFC;
	}

	//mapping opcode to instructions
	static const Byte
		ADC_IM = 0x69,
		ADC_ZP = 0x65,
		ADC_ZPX = 0x75,
		ADC_AB = 0x6D,
		ADC_ABX = 0x7D,
		ADC_ABY = 0x79,
		ADC_INX = 0x61,
		ADC_INY = 0x71,

		AND_IM = 0x29,
		AND_ZP = 0x25,
		AND_ZPX = 0x35,
		AND_AB = 0x2D,
		AND_ABX = 0x3D,
		AND_ABY = 0x39,
		AND_INX = 0x21,
		AND_INY = 0x31,

		CMP_IM = 0xC9,
		CMP_ZP = 0xC5,
		CMP_ZPX = 0xD5,
		CMP_AB = 0xCD,
		CMP_ABX = 0xDD,
		CMP_ABY = 0xD9,
		CMP_INX = 0xC1,
		CMP_INY = 0xD1,

		DEC_ZP = 0xC6,
		DEC_ZPX = 0xD6,
		DEC_AB = 0xCE,
		DEC_ABX = 0xDE,

		DEX_IMPLIED = 0xCA,
		DEY_IMPLIED = 0x88,

		INC_ZP = 0xE6,
		INC_ZPX = 0xF6,
		INC_AB = 0xEE,
		INC_ABX = 0xFE,

		INX_IMPLIED = 0xE8,
		INY_IMPLIED = 0xC8,

		JMP_AB = 0x4C,
		JMP_IN = 0x6C,

		JSR_AB = 0x20,

		CPX_IM = 0xE0,
CPX_ZP = 0xE4,
CPX_AB = 0xEC,

CPY_IM = 0xC0,
CPY_ZP = 0xC4,
CPY_AB = 0xCC,

EOR_IM = 0x49,
EOR_ZP = 0x45,
EOR_ZPX = 0x55,
EOR_AB = 0x4D,
EOR_ABX = 0x5D,
EOR_ABY = 0x59,
EOR_INX = 0x41,
EOR_INY = 0x51,

PHA_IMPLIED = 0x48,
PHP_IMPLIED = 0x08,
PLA_IMPLIED = 0x68,

LDA_IM = 0xA9,
LDA_ZP = 0xA5,
LDA_ZPX = 0xB5,
LDA_AB = 0xAD,
LDA_ABX = 0xBD,
LDA_ABY = 0xB9,
LDA_INX = 0xA1,
LDA_INY = 0xB1,

LDX_IM = 0xA2,
LDX_ZP = 0xA6,
LDX_ZPY = 0xB6,
LDX_AB = 0xAE,
LDX_ABY = 0xBE,

LDY_IM = 0xA0,
LDY_ZP = 0xA4,
LDY_ZPX = 0xB4,
LDY_AB = 0xAC,
LDY_ABX = 0xBC,

LSR_A = 0x4A,
LSR_ZP = 0x46,
LSR_ZPX = 0x56,
LSR_AB = 0x4E,
LSR_ABX = 0x5E,

ROL_A = 0x2A,
ROL_ZP = 0x26,
ROL_ZPX = 0x36,
ROL_AB = 0x2E,
ROL_ABX = 0x3E,

ROR_A = 0x6A,
ROR_ZP = 0x66,
ROR_ZPX = 0x76,
ROR_AB = 0x6E,
ROR_ABX = 0x7E,

NOP_IMPLIED = 0xEA,

ORA_IM = 0x09,
ORA_ZP = 0x05,
ORA_ZPX = 0x15,
ORA_AB = 0x0D,
ORA_ABX = 0x1D,
ORA_ABY = 0x19,
ORA_INX = 0x01,
ORA_INY = 0x11,

SBC_IM = 0xE9,
SBC_ZP = 0xE5,
SBC_ZPX = 0xF5,
SBC_AB = 0xED,
SBC_ABX = 0xFD,
SBC_ABY = 0xF9,
SBC_INX = 0xE1,
SBC_INY = 0xF1,

SEC_IMPLIED = 0x38,
SED_IMPLIED = 0xF8,
SEI_IMPLIED = 0x78,

STA_ZP = 0x85,
STA_ZPX = 0x95,
STA_AB = 0x8D,
STA_ABX = 0x9D,
STA_ABY = 0x99,
STA_INX = 0x81,
STA_INY = 0x91,

STX_ZP = 0x86,
STX_ZPY = 0x96,
STX_AB = 0x8E,

STY_ZP = 0x84,
STY_ZPX = 0x94,
STY_AB = 0x8C,

TAX_IMPLIED = 0xAA,
TAY_IMPLIED = 0xA8,

TSX_IMPLIED = 0xBA,
TXA_IMPLIED = 0x8A,
TXS_IMPLIED = 0x9A,
TYA_IMPLIED = 0X98;
		
		


	void LDA_SET_FLAGS()
	{
		if (A == 0)
		{
			set_zero_flag();
		}

		if (A >> 7 == 1)
		{
			status_byte |= 0b10000000;
		}
	}

	void ADC_SET_FLAGS()
	{
		if (A > 0xffff)
		{
			set_carry_flag();
		}

		if (A == 0)
		{
			set_zero_flag();
		}

		if (A >> 7 == 1)
		{
			status_byte |= 0b10000000;
		}
	}

	void CMP_SET_FLAGS(Byte A, Byte cmp_byte)
	{
		if (A >= cmp_byte)
		{
			set_carry_flag();
		}

		if (A == cmp_byte)
		{
			set_zero_flag();
		}

		Byte result = A - cmp_byte;

		if (result >> 7 == 1)
		{
			status_byte |= 0b10000000;
		}
	}

	void DEC_INC_SET_FLAGS(Byte result)
	{
		if (result == 0)
		{
			set_zero_flag();
		}

		if (result >> 7 == 1)
		{
			status_byte |= 0b10000000;
		}
	}

	void CPI(Byte register_val, Byte cmp_val)
	{
		if (register_val >= cmp_val)
		{
			set_carry_flag();
		}

		if (register_val == cmp_val)
		{
			set_zero_flag();

		}
	}

	//fetch byte from memory
	Byte fetch_byte(Memory mem)
	{
		Byte byte = mem.memory_block[program_counter];
		program_counter++;
		cycles--;
		return byte;
	}

	template <typename T>
	T readValue_on_address(Memory mem, T address)
	{
		T valueOnAddress = mem.memory_block[address];
		cycles--;

		return valueOnAddress;
	}

	template<typename T>
	void explicit_write_back(T value, Memory& mem, T addr)
	{
		mem.memory_block[addr] = value;
		cycles--;
	}

	//fetching byte from memory and executing it
	void execute_instruction(int cyc, Memory mem)
	{
		this->cycles = cyc;
		while (this->cycles != 0)
		{
			//fetching instruction opcode from memory - takes 1 cycle
			Byte opcode = fetch_byte(mem);

			switch (opcode)
			{
				//all cycle values mentioned henceforth would be including the 1 clock cycle for fetching the opcode

			//2 cycles
			case LDA_IM:
			{
				//fetching the immediate value to be stored and assigning it to A register
				A = fetch_byte(mem);
				LDA_SET_FLAGS();
			}
			break;

			//3 cycles
			case LDA_ZP:
			{
				//fetch_byte() fetches the zero page address
				Word effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				A = readValue_on_address<Byte>(mem, effective_addr);
				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case LDA_ZPX:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + X;
				cycles--; //fetching from X register

				A = readValue_on_address<Word>(mem, effective_addr);

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case LDA_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				A = readValue_on_address<Word>(mem, absolute_address);

				LDA_SET_FLAGS();
			}
			break;

			case LDA_ABX:
			{

				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + X > 0xffff)
				{
					cycles--;
				}

				A = readValue_on_address<DWord>(mem, absolute_address + X);

				LDA_SET_FLAGS();

			}
			break;

			case LDA_ABY:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + Y > 0xffff)
				{
					cycles--;
				}

				A = readValue_on_address<DWord>(mem, absolute_address + Y);

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case LDA_INX:
			{
				Byte BAL = fetch_byte(mem);

				Word effective_addr = BAL + X;
				cycles--;

				Byte effective_addr_L = readValue_on_address<Byte>(mem, effective_addr);
				Byte effective_addr_H = readValue_on_address<Byte>(mem, effective_addr + 1);

				Word fetch_addr;
				if (effective_addr_L > 0xff)
				{
					fetch_addr = effective_addr_H * 0x100 + effective_addr_L;
				}

				else
				{
					fetch_addr = effective_addr_L;
				}

				A = readValue_on_address<Word>(mem, fetch_addr);

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case LDA_INY:
			{

				Byte IAL = fetch_byte(mem);

				Byte BAL = readValue_on_address<Byte>(mem, IAL);
				Byte BAH = readValue_on_address<Byte>(mem, IAL + 1);

				DWord fetch_addr = BAH * 0x100 + BAL + Y;

				if (fetch_addr > 0xffff)
				{
					cycles--;
				}

				A = readValue_on_address<DWord>(mem, fetch_addr);


				LDA_SET_FLAGS();
			}
			break;



			//LDX



			//2 cycles
			case LDX_IM:
			{
				//fetching the immediate value to be stored and assigning it to A register
				X = fetch_byte(mem);
				LDA_SET_FLAGS();
			}
			break;

			//3 cycles
			case LDX_ZP:
			{
				//fetch_byte() fetches the zero page address
				Word effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				X = readValue_on_address<Byte>(mem, effective_addr);
				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case LDX_ZPY:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + Y;
				cycles--; //fetching from Y register

				X = readValue_on_address<Word>(mem, effective_addr);

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case LDX_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				X = readValue_on_address<Word>(mem, absolute_address);

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles (+1 if page crossed)
			case LDX_ABY:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + Y > 0xffff)
				{
					cycles--;
				}

				X = readValue_on_address<DWord>(mem, absolute_address + Y);

				LDA_SET_FLAGS();
			}
			break;

			//LDY

			//2 cycles
			case LDY_IM:
			{
				//fetching the immediate value to be stored and assigning it to A register
				Y = fetch_byte(mem);
				LDA_SET_FLAGS();
			}
			break;

			//3 cycles
			case LDY_ZP:
			{
				//fetch_byte() fetches the zero page address
				Word effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				Y = readValue_on_address<Byte>(mem, effective_addr);
				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case LDY_ZPX:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + X;
				cycles--; //fetching from X register

				Y = readValue_on_address<Word>(mem, effective_addr);

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case LDY_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				Y = readValue_on_address<Word>(mem, absolute_address);

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles (+1 if page crossed)
			case LDY_ABX:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + X > 0xffff)
				{
					cycles--;
				}

				Y = readValue_on_address<DWord>(mem, absolute_address + Y);

				LDA_SET_FLAGS();
			}
			break;

			//ADC

			case ADC_IM:
			{
				A += fetch_byte(mem);

				ADC_SET_FLAGS();

			}
			break;

			case ADC_ZP:
			{

				//fetch_byte() fetches the zero page address
				Word effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				A += readValue_on_address<Byte>(mem, effective_addr);
				ADC_SET_FLAGS();

			}
			break;

			case ADC_ZPX:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + X;
				cycles--; //fetching from X register

				A += readValue_on_address<Word>(mem, effective_addr);

				ADC_SET_FLAGS();
			}
			break;

			case ADC_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				A += readValue_on_address<Word>(mem, absolute_address);

				ADC_SET_FLAGS();
			}
			break;

			case ADC_ABX:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + X > 0xffff)
				{
					cycles--;
				}

				A += readValue_on_address<DWord>(mem, absolute_address + X);

				ADC_SET_FLAGS();
			}
			break;


			case ADC_ABY:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + Y > 0xffff)
				{
					cycles--;
				}

				A += readValue_on_address<DWord>(mem, absolute_address + Y);

				ADC_SET_FLAGS();
			}
			break;

			case ADC_INX:
			{
				Byte BAL = fetch_byte(mem);

				Word effective_addr = BAL + X;
				cycles--;

				Byte effective_addr_L = readValue_on_address<Byte>(mem, effective_addr);
				Byte effective_addr_H = readValue_on_address<Byte>(mem, effective_addr + 1);

				Word fetch_addr;
				if (effective_addr_L > 0xff)
				{
					fetch_addr = effective_addr_H * 0x100 + effective_addr_L;
				}

				else
				{
					fetch_addr = effective_addr_L;
				}

				A += readValue_on_address<Word>(mem, fetch_addr);

				ADC_SET_FLAGS();
			}
			break;


			case ADC_INY:
			{
				Byte IAL = fetch_byte(mem);

				Byte BAL = readValue_on_address<Byte>(mem, IAL);
				Byte BAH = readValue_on_address<Byte>(mem, IAL + 1);

				DWord fetch_addr = BAH * 0x100 + BAL + Y;

				if (fetch_addr > 0xffff)
				{
					cycles--;
				}

				A += readValue_on_address<DWord>(mem, fetch_addr);
			}
			break;

			//AND 


			case AND_IM:
			{
				A &= fetch_byte(mem);

				LDA_SET_FLAGS();

			}
			break;

			case AND_ZP:
			{

				//fetch_byte() fetches the zero page address
				Word effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				A &= readValue_on_address<Byte>(mem, effective_addr);
				LDA_SET_FLAGS();

			}
			break;

			case AND_ZPX:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + X;
				cycles--; //fetching from X register

				A &= readValue_on_address<Word>(mem, effective_addr);

				LDA_SET_FLAGS();
			}
			break;

			case AND_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				A &= readValue_on_address<Word>(mem, absolute_address);

				LDA_SET_FLAGS();
			}
			break;

			case AND_ABX:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + X > 0xffff)
				{
					cycles--;
				}

				A &= readValue_on_address<DWord>(mem, absolute_address + X);

				LDA_SET_FLAGS();
			}
			break;


			case AND_ABY:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + Y > 0xffff)
				{
					cycles--;
				}

				A &= readValue_on_address<DWord>(mem, absolute_address + Y);

				LDA_SET_FLAGS();
			}
			break;

			case AND_INX:
			{
				Byte BAL = fetch_byte(mem);

				Word effective_addr = BAL + X;
				cycles--;

				Byte effective_addr_L = readValue_on_address<Byte>(mem, effective_addr);
				Byte effective_addr_H = readValue_on_address<Byte>(mem, effective_addr + 1);

				Word fetch_addr;
				if (effective_addr_L > 0xff)
				{
					fetch_addr = effective_addr_H * 0x100 + effective_addr_L;
				}

				else
				{
					fetch_addr = effective_addr_L;
				}

				A &= readValue_on_address<Word>(mem, fetch_addr);

				LDA_SET_FLAGS();
			}
			break;


			case AND_INY:
			{
				Byte IAL = fetch_byte(mem);

				Byte BAL = readValue_on_address<Byte>(mem, IAL);
				Byte BAH = readValue_on_address<Byte>(mem, IAL + 1);

				DWord fetch_addr = BAH * 0x100 + BAL + Y;

				if (fetch_addr > 0xffff)
				{
					cycles--;
				}

				A &= readValue_on_address<DWord>(mem, fetch_addr);

				LDA_SET_FLAGS();
			}
			break;


			//CMP

			case CMP_IM:
			{
				Byte CMP_byte = fetch_byte(mem);

				CMP_SET_FLAGS(A, CMP_byte);

			}
			break;

			case CMP_ZP:
			{

				//fetch_byte() fetches the zero page address
				Byte effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				Byte CMP_byte = readValue_on_address<Byte>(mem, effective_addr);

				CMP_SET_FLAGS(A, CMP_byte);


			}
			break;

			case CMP_ZPX:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + X;
				cycles--; //fetching from X register

				Byte CMP_byte = readValue_on_address<Word>(mem, effective_addr);

				CMP_SET_FLAGS(A, CMP_byte);


			}
			break;

			case CMP_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				Byte CMP_byte = readValue_on_address<Word>(mem, absolute_address);

				CMP_SET_FLAGS(A, CMP_byte);


			}
			break;

			case CMP_ABX:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + X > 0xffff)
				{
					cycles--;
				}

				Byte CMP_byte = readValue_on_address<DWord>(mem, absolute_address + X);

				CMP_SET_FLAGS(A, CMP_byte);
			}
			break;


			case CMP_ABY:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + Y > 0xffff)
				{
					cycles--;
				}

				Byte CMP_byte = readValue_on_address<DWord>(mem, absolute_address + Y);

				CMP_SET_FLAGS(A, CMP_byte);
			}
			break;

			case CMP_INX:
			{
				Byte BAL = fetch_byte(mem);

				Word effective_addr = BAL + X;
				cycles--;

				Byte effective_addr_L = readValue_on_address<Byte>(mem, effective_addr);
				Byte effective_addr_H = readValue_on_address<Byte>(mem, effective_addr + 1);

				Word fetch_addr;
				if (effective_addr_L > 0xff)
				{
					fetch_addr = effective_addr_H * 0x100 + effective_addr_L;
				}

				else
				{
					fetch_addr = effective_addr_L;
				}

				Byte CMP_byte = readValue_on_address<Word>(mem, fetch_addr);

				CMP_SET_FLAGS(A, CMP_byte);

			}
			break;


			case CMP_INY:
			{
				Byte IAL = fetch_byte(mem);

				Byte BAL = readValue_on_address<Byte>(mem, IAL);
				Byte BAH = readValue_on_address<Byte>(mem, IAL + 1);

				DWord fetch_addr = BAH * 0x100 + BAL + Y;

				if (fetch_addr > 0xffff)
				{
					cycles--;
				}

				Byte CMP_byte = readValue_on_address<DWord>(mem, fetch_addr);

				CMP_SET_FLAGS(A, CMP_byte);



			}
			break;


			//DEC

			case DEC_ZP:
			{
				Byte zp_address = fetch_byte(mem);

				Byte value_at_zp_addr = readValue_on_address<Byte>(mem, zp_address);

				explicit_write_back<Byte>(value_at_zp_addr, mem, zp_address);
				value_at_zp_addr -= 1;

				explicit_write_back<Byte>(value_at_zp_addr, mem, zp_address);

				DEC_INC_SET_FLAGS(value_at_zp_addr);

			}

			break;

			case DEC_ZPX:
			{
				Byte zp_addr = fetch_byte(mem);

				Byte zp_val = readValue_on_address<Byte>(mem, zp_addr);

				Byte effective_addr = zp_addr + X;

				Byte effective_val = readValue_on_address<Byte>(mem, effective_addr);

				explicit_write_back(effective_val, mem, effective_addr);
				effective_val -= 1;

				explicit_write_back<Byte>(effective_val, mem, effective_addr);


				DEC_INC_SET_FLAGS(effective_val);


			}
			break;

			case DEC_AB:
			{
				Byte BAL = fetch_byte(mem);
				Byte BAH = fetch_byte(mem);

				Word effective_addr = BAH * 0x100 + BAL;

				Byte effective_val = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_val, mem, effective_addr);
				effective_val -= 1;

				explicit_write_back<Word>(effective_val, mem, effective_addr);

				DEC_INC_SET_FLAGS(effective_val);

			}
			break;

			case DEC_ABX:
			{
				Byte BAL = fetch_byte(mem);
				Byte BAH = fetch_byte(mem);

				Word effective_addr = BAH * 0x100 + BAL + X;

				Byte effective_val = readValue_on_address<Word>(mem, effective_addr);

				effective_val = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_val, mem, effective_addr);
				effective_val -= 1;

				explicit_write_back<Word>(effective_val, mem, effective_addr);


				DEC_INC_SET_FLAGS(effective_val);

			}
			break;

			case DEX_IMPLIED:
			{
				fetch_byte(mem);
				X--;

				LDA_SET_FLAGS();
			}
			break;

			case DEY_IMPLIED:
			{
				fetch_byte(mem);
				Y--;

				LDA_SET_FLAGS();
			}
			break;


			//INC

			case INC_ZP:
			{
				Byte zp_address = fetch_byte(mem);

				Byte value_at_zp_addr = readValue_on_address<Byte>(mem, zp_address);

				explicit_write_back<Byte>(value_at_zp_addr, mem, zp_address);
				value_at_zp_addr += 1;

				explicit_write_back<Byte>(value_at_zp_addr, mem, zp_address);

				DEC_INC_SET_FLAGS(value_at_zp_addr);

			}

			break;

			case INC_ZPX:
			{
				Byte zp_addr = fetch_byte(mem);

				Byte zp_val = readValue_on_address<Byte>(mem, zp_addr);

				Byte effective_addr = zp_addr + X;

				Byte effective_val = readValue_on_address<Byte>(mem, effective_addr);

				explicit_write_back(effective_val, mem, effective_addr);
				effective_val += 1;

				explicit_write_back<Byte>(effective_val, mem, effective_addr);


				DEC_INC_SET_FLAGS(effective_val);


			}
			break;

			case INC_AB:
			{
				Byte BAL = fetch_byte(mem);
				Byte BAH = fetch_byte(mem);

				Word effective_addr = BAH * 0x100 + BAL;

				Byte effective_val = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_val, mem, effective_addr);
				effective_val += 1;

				explicit_write_back<Word>(effective_val, mem, effective_addr);

				DEC_INC_SET_FLAGS(effective_val);

			}
			break;

			case INC_ABX:
			{
				Byte BAL = fetch_byte(mem);
				Byte BAH = fetch_byte(mem);

				Word effective_addr = BAH * 0x100 + BAL + X;

				Byte effective_val = readValue_on_address<Word>(mem, effective_addr);

				effective_val = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_val, mem, effective_addr);
				effective_val += 1;

				explicit_write_back<Word>(effective_val, mem, effective_addr);


				DEC_INC_SET_FLAGS(effective_val);

			}
			break;

			case INX_IMPLIED:
			{
				fetch_byte(mem);
				X++;

				LDA_SET_FLAGS();
			}
			break;

			case INY_IMPLIED:
			{
				fetch_byte(mem);
				Y++;

				LDA_SET_FLAGS();
			}
			break;

			case JMP_AB:
			{
				Byte PCL = fetch_byte(mem);
				Byte PCH = fetch_byte(mem);

				Word PC_JMP_ADDR = PCH * 0x100 + PCL;

				program_counter = PC_JMP_ADDR;

			}
			break;

			case JMP_IN:
			{
				Byte indirect_addr_low = fetch_byte(mem);
				Byte indirect_addr_high = fetch_byte(mem);

				Word pointer_addr = indirect_addr_high * 0x100 + indirect_addr_low;

				Byte PCL = readValue_on_address<Word>(mem, pointer_addr);
				Byte PCH = readValue_on_address<Word>(mem, pointer_addr + 1);

				Word PC_JMP_ADDR = PCH * 0x100 + PCL;

				program_counter = PC_JMP_ADDR;
			}

			case JSR_AB:
			{
				Byte PCL = fetch_byte(mem);
				Byte PCH = fetch_byte(mem);

				explicit_write_back(PCH, mem, stack_pointer);
				stack_pointer--;

				explicit_write_back(PCL, mem, stack_pointer);
				stack_pointer--;

				Word effective_addr = PCL * 0x100 + PCH;

				explicit_write_back(effective_addr, mem, program_counter);
			}
			break;

			//LSR

			case LSR_A:
			{
				//shifting bit 0 to carry flag
				Byte bitwise_temp = A << 7;
				bitwise_temp >>= 7;
				status_byte |= bitwise_temp;

				//one right shift
				A = A >> 1;

				A = A & 0b01111111;

				LSR_SET_FLAGS(A);
			}
			break;

			case LSR_ZP:
			{
				Byte effective_addr = fetch_byte(mem);

				Byte effective_value = readValue_on_address<Byte>(mem, effective_addr);

				explicit_write_back(effective_value, mem, effective_addr);

				Byte bitwise_temp = effective_value << 7;
				bitwise_temp >>= 7;
				status_byte |= bitwise_temp;

				effective_value = effective_value >> 1;

				effective_value = effective_value & 0b01111111;

				explicit_write_back(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}

			case LSR_ZPX:
			{
				Byte addr = fetch_byte(mem);

				readValue_on_address<Byte>(mem, addr);

				Byte effective_addr = addr + X;

				Byte effective_value = readValue_on_address<Byte>(mem, effective_addr);

				explicit_write_back(effective_value, mem, effective_addr);

				Byte bitwise_temp = effective_value << 7;
				bitwise_temp >>= 7;
				status_byte |= bitwise_temp;

				effective_value = effective_value >> 1;

				effective_value = effective_value & 0b01111111;

				explicit_write_back(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);

			}
			break;

			case LSR_AB:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr = high_byte * 0x100 + low_byte;

				Byte effective_value = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_value, mem, effective_addr);

				Byte bitwise_temp = effective_value << 7;
				bitwise_temp >>= 7;
				status_byte |= bitwise_temp;

				effective_value = effective_value >> 1;

				effective_value = effective_value & 0b01111111;

				explicit_write_back<Word>(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}
			break;

			case LSR_ABX:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr_asterisk = low_byte + X;

				readValue_on_address<Word>(mem, effective_addr_asterisk);


				Word effective_addr = high_byte * 0x100 + effective_addr_asterisk;

				Byte effective_value = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_value, mem, effective_addr);

				Byte bitwise_temp = effective_value << 7;
				bitwise_temp >>= 7;
				status_byte |= bitwise_temp;

				effective_value = effective_value >> 1;

				effective_value = effective_value & 0b01111111;

				explicit_write_back<Word>(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}
			break;

			//CPX

			case CPX_IM:
			{
				Byte cmp_byte = fetch_byte(mem);

				CPI(X, cmp_byte);
			}
			break;

			case CPX_ZP:
			{
				Byte zp_addr = fetch_byte(mem);

				Byte zp_addr_val = readValue_on_address<Byte>(mem, zp_addr);

				CPI(X, zp_addr_val);
			}
			break;


			case CPX_AB:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr = high_byte * 0x100 + low_byte;

				Byte effective_value = readValue_on_address<Word>(mem, effective_addr);

				CPI(X, effective_value);
			}
			break;


			//CPY

			case CPY_IM:
			{
				Byte cmp_byte = fetch_byte(mem);

				CPI(X, cmp_byte);
			}
			break;

			case CPY_ZP:
			{
				Byte zp_addr = fetch_byte(mem);

				Byte zp_addr_val = readValue_on_address<Byte>(mem, zp_addr);

				CPI(X, zp_addr_val);
			}
			break;

			case CPY_AB:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr = high_byte * 0x100 + low_byte;

				Byte effective_value = readValue_on_address<Word>(mem, effective_addr);

				CPI(X, effective_value);
			}
			break;

			//EOR

			//2 cycles
			case EOR_IM:
			{
				//fetching the immediate value to be stored and assigning it to A register
				A ^= fetch_byte(mem);
				LDA_SET_FLAGS();
			}
			break;

			//3 cycles
			case EOR_ZP:
			{
				//fetch_byte() fetches the zero page address
				Word effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				A ^= readValue_on_address<Byte>(mem, effective_addr);
				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case EOR_ZPX:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + X;
				cycles--; //fetching from X register

				A ^= readValue_on_address<Word>(mem, effective_addr);

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case EOR_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				A ^= readValue_on_address<Word>(mem, absolute_address);

				LDA_SET_FLAGS();
			}
			break;

			case EOR_ABX:
			{

				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + X > 0xffff)
				{
					cycles--;
				}

				A ^= readValue_on_address<DWord>(mem, absolute_address + X);

				LDA_SET_FLAGS();

			}
			break;

			case EOR_ABY:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + Y > 0xffff)
				{
					cycles--;
				}

				A ^= readValue_on_address<DWord>(mem, absolute_address + Y);

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case EOR_INX:
			{
				Byte BAL = fetch_byte(mem);

				Word effective_addr = BAL + X;
				cycles--;

				Byte effective_addr_L = readValue_on_address<Byte>(mem, effective_addr);
				Byte effective_addr_H = readValue_on_address<Byte>(mem, effective_addr + 1);

				Word fetch_addr;
				if (effective_addr_L > 0xff)
				{
					fetch_addr = effective_addr_H * 0x100 + effective_addr_L;
				}

				else
				{
					fetch_addr = effective_addr_L;
				}

				A ^= readValue_on_address<Word>(mem, fetch_addr);

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case EOR_INY:
			{

				Byte IAL = fetch_byte(mem);

				Byte BAL = readValue_on_address<Byte>(mem, IAL);
				Byte BAH = readValue_on_address<Byte>(mem, IAL + 1);

				DWord fetch_addr = BAH * 0x100 + BAL + Y;

				if (fetch_addr > 0xffff)
				{
					cycles--;
				}

				A ^= readValue_on_address<DWord>(mem, fetch_addr);


				LDA_SET_FLAGS();
			}
			break;

			case PHA_IMPLIED:
			{
				explicit_write_back<Word>(A, mem, stack_pointer);
				stack_pointer--;
			}
			break;

			case PHP_IMPLIED:
			{
				explicit_write_back<Word>(status_byte, mem, stack_pointer);
				stack_pointer--;
			}
			break;

			case PLA_IMPLIED:
			{
				A = readValue_on_address<Byte>(mem, stack_pointer);

				LDA_SET_FLAGS();
			}
			break;

			case ROL_A:
			{
				Byte A_bit_7 = A >> 7;

				A = A << 1;

				Byte temp = status_byte << 7;
				temp = temp >> 7;

				if (temp == 1)
				{
					A |= temp;
				}

				else if (temp == 0)
				{
					A = A >> 1;
					A = A << 1;
				}

				if (A_bit_7 == 1)
				{
					status_byte |= A_bit_7;
				}
				else if (A_bit_7 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}

				LSR_SET_FLAGS(A);
			}
			break;

			case ROL_ZP:
			{
				Byte effective_addr = fetch_byte(mem);

				Byte effective_value = readValue_on_address<Byte>(mem, effective_addr);

				explicit_write_back(effective_value, mem, effective_addr);

				Byte bit_7 = effective_value >> 7;

				effective_value = effective_value << 1;

				Byte temp = status_byte << 7;
				temp = temp >> 7;

				if (temp == 1)
				{
					effective_value |= temp;
				}

				else if (temp == 0)
				{
					effective_value = effective_value >> 1;
					effective_value = effective_value << 1;
				}

				if (bit_7 == 1)
				{
					status_byte |= bit_7;
				}
				else if (bit_7 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}

				explicit_write_back(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}

			case ROL_ZPX:
			{
				Byte addr = fetch_byte(mem);

				readValue_on_address<Byte>(mem, addr);

				Byte effective_addr = addr + X;

				Byte effective_value = readValue_on_address<Byte>(mem, effective_addr);

				explicit_write_back(effective_value, mem, effective_addr);
				Byte bit_7 = effective_value >> 7;

				effective_value = effective_value << 1;

				Byte temp = status_byte << 7;
				temp = temp >> 7;

				if (temp == 1)
				{
					effective_value |= temp;
				}

				else if (temp == 0)
				{
					effective_value = effective_value >> 1;
					effective_value = effective_value << 1;
				}

				if (bit_7 == 1)
				{
					status_byte |= bit_7;
				}
				else if (bit_7 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}

				explicit_write_back(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);

			}
			break;

			case ROL_AB:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr = high_byte * 0x100 + low_byte;

				Byte effective_value = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_value, mem, effective_addr);
				Byte bit_7 = effective_value >> 7;

				effective_value = effective_value << 1;

				Byte temp = status_byte << 7;
				temp = temp >> 7;

				if (temp == 1)
				{
					effective_value |= temp;
				}

				else if (temp == 0)
				{
					effective_value = effective_value >> 1;
					effective_value = effective_value << 1;
				}

				if (bit_7 == 1)
				{
					status_byte |= bit_7;
				}
				else if (bit_7 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}

				explicit_write_back<Word>(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}
			break;

			case ROL_ABX:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr_asterisk = low_byte + X;

				readValue_on_address<Word>(mem, effective_addr_asterisk);


				Word effective_addr = high_byte * 0x100 + effective_addr_asterisk;

				Byte effective_value = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_value, mem, effective_addr);
				Byte bit_7 = effective_value >> 7;

				effective_value = effective_value << 1;

				Byte temp = status_byte << 7;
				temp = temp >> 7;

				if (temp == 1)
				{
					effective_value |= temp;
				}

				else if (temp == 0)
				{
					effective_value = effective_value >> 1;
					effective_value = effective_value << 1;
				}

				if (bit_7 == 1)
				{
					status_byte |= bit_7;
				}
				else if (bit_7 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}

				explicit_write_back<Word>(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}
			break;

			case ROR_A:
			{
				Byte A_bit_0 = A << 7;
				A_bit_0 = A_bit_0 >> 7;

				A = A >> 1;

				Byte temp = status_byte << 7;

				if (temp >> 7 == 1)
				{
					A |= temp;
				}

				else if (temp >> 7 == 0)
				{
					A = A << 1;
					A = A >> 1;
				}

				if (A_bit_0 == 1)
				{
					status_byte |= A_bit_0;
				}
				else if (A_bit_0 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}		

				LSR_SET_FLAGS(A);
			}
			break;

			case ROR_ZP:
			{
				Byte effective_addr = fetch_byte(mem);

				Byte effective_value = readValue_on_address<Byte>(mem, effective_addr);

				explicit_write_back(effective_value, mem, effective_addr);

				Byte effective_value_bit_0 = effective_value << 7;
				effective_value_bit_0 = effective_value_bit_0 >> 7;

				effective_value = effective_value >> 1;

				Byte temp = status_byte << 7;

				if (temp >> 7 == 1)
				{
					effective_value |= temp;
				}

				else if (temp >> 7 == 0)
				{
					effective_value = effective_value << 1;
					effective_value = effective_value >> 1;
				}

				if (effective_value_bit_0 == 1)
				{
					status_byte |= effective_value_bit_0;
				}
				else if (effective_value_bit_0 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}

				explicit_write_back(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}

			case ROR_ZPX:
			{
				Byte addr = fetch_byte(mem);

				readValue_on_address<Byte>(mem, addr);

				Byte effective_addr = addr + X;

				Byte effective_value = readValue_on_address<Byte>(mem, effective_addr);

				explicit_write_back(effective_value, mem, effective_addr);
				
				Byte effective_value_bit_0 = effective_value << 7;
				effective_value_bit_0 = effective_value_bit_0 >> 7;

				effective_value = effective_value >> 1;

				Byte temp = status_byte << 7;

				if (temp >> 7 == 1)
				{
					effective_value |= temp;
				}

				else if (temp >> 7 == 0)
				{
					effective_value = effective_value << 1;
					effective_value = effective_value >> 1;
				}

				if (effective_value_bit_0 == 1)
				{
					status_byte |= effective_value_bit_0;
				}
				else if (effective_value_bit_0 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}

				explicit_write_back(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);

			}
			break;

			case ROR_AB:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr = high_byte * 0x100 + low_byte;

				Byte effective_value = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_value, mem, effective_addr);
				Byte effective_value_bit_0 = effective_value << 7;
				effective_value_bit_0 = effective_value_bit_0 >> 7;

				effective_value = effective_value >> 1;

				Byte temp = status_byte << 7;

				if (temp >> 7 == 1)
				{
					effective_value |= temp;
				}

				else if (temp >> 7 == 0)
				{
					effective_value = effective_value << 1;
					effective_value = effective_value >> 1;
				}

				if (effective_value_bit_0 == 1)
				{
					status_byte |= effective_value_bit_0;
				}
				else if (effective_value_bit_0 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}
				explicit_write_back<Word>(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}
			break;

			case ROR_ABX:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr_asterisk = low_byte + X;

				readValue_on_address<Word>(mem, effective_addr_asterisk);


				Word effective_addr = high_byte * 0x100 + effective_addr_asterisk;

				Byte effective_value = readValue_on_address<Word>(mem, effective_addr);

				explicit_write_back<Word>(effective_value, mem, effective_addr);
				
				Byte effective_value_bit_0 = effective_value << 7;
				effective_value_bit_0 = effective_value_bit_0 >> 7;

				effective_value = effective_value >> 1;

				Byte temp = status_byte << 7;

				if (temp >> 7 == 1)
				{
					effective_value |= temp;
				}

				else if (temp >> 7 == 0)
				{
					effective_value = effective_value << 1;
					effective_value = effective_value >> 1;
				}

				if (effective_value_bit_0 == 1)
				{
					status_byte |= effective_value_bit_0;
				}
				else if (effective_value_bit_0 == 0)
				{
					status_byte >>= 1;
					status_byte <<= 1;
				}

				explicit_write_back<Word>(effective_value, mem, effective_addr);

				LSR_SET_FLAGS(effective_value);
			}
			break;

			//NOP

			case NOP_IMPLIED:
			{
				fetch_byte(mem);
			}
			break;

			//ORA

			//2 cycles
			case ORA_IM:
			{
				//fetching the immediate value to be stored and assigning it to A register
				A |= fetch_byte(mem);
				LDA_SET_FLAGS();
			}
			break;

			//3 cycles
			case ORA_ZP:
			{
				//fetch_byte() fetches the zero page address
				Word effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				A |= readValue_on_address<Byte>(mem, effective_addr);
				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case ORA_ZPX:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + X;
				cycles--; //fetching from X register

				A |= readValue_on_address<Word>(mem, effective_addr);

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case ORA_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				A |= readValue_on_address<Word>(mem, absolute_address);

				LDA_SET_FLAGS();
			}
			break;

			case ORA_ABX:
			{

				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + X > 0xffff)
				{
					cycles--;
				}

				A |= readValue_on_address<DWord>(mem, absolute_address + X);

				LDA_SET_FLAGS();

			}
			break;

			case ORA_ABY:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + Y > 0xffff)
				{
					cycles--;
				}

				A |= readValue_on_address<DWord>(mem, absolute_address + Y);

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case ORA_INX:
			{
				Byte BAL = fetch_byte(mem);

				Word effective_addr = BAL + X;
				cycles--;

				Byte effective_addr_L = readValue_on_address<Byte>(mem, effective_addr);
				Byte effective_addr_H = readValue_on_address<Byte>(mem, effective_addr + 1);

				Word fetch_addr;
				if (effective_addr_L > 0xff)
				{
					fetch_addr = effective_addr_H * 0x100 + effective_addr_L;
				}

				else
				{
					fetch_addr = effective_addr_L;
				}

				A |= readValue_on_address<Word>(mem, fetch_addr);

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case ORA_INY:
			{

				Byte IAL = fetch_byte(mem);

				Byte BAL = readValue_on_address<Byte>(mem, IAL);
				Byte BAH = readValue_on_address<Byte>(mem, IAL + 1);

				DWord fetch_addr = BAH * 0x100 + BAL + Y;

				if (fetch_addr > 0xffff)
				{
					cycles--;
				}

				A |= readValue_on_address<DWord>(mem, fetch_addr);


				LDA_SET_FLAGS();
			}
			break;

			//SBC

			//2 cycles
			case SBC_IM:
			{
				//fetching the immediate value to be stored and assigning it to A register
				
				Byte M = fetch_byte(mem);

				M = twoscomplement(M);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				Byte temp = A + M;

				A = A + M - ~bitwise_temp;

				if (temp > 0xff)
				{
					if (bitwise_temp == 1)
					{
						bitwise_temp = bitwise_temp | status_byte;

						bitwise_temp = bitwise_temp >> 1;
						bitwise_temp = bitwise_temp << 1;

						status_byte &= bitwise_temp;
					}
				}

				LDA_SET_FLAGS();
			}
			break;

			//3 cycles
			case SBC_ZP:
			{
				//fetch_byte() fetches the zero page address
				Word effective_addr = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				Byte M= readValue_on_address<Byte>(mem, effective_addr);
				
				M = twoscomplement(M);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				Byte temp = A + M;

				A = A + M - ~bitwise_temp;

				if (temp > 0xff)
				{
					if (bitwise_temp == 1)
					{
						bitwise_temp = bitwise_temp | status_byte;

						bitwise_temp = bitwise_temp >> 1;
						bitwise_temp = bitwise_temp << 1;

						status_byte &= bitwise_temp;
					}
				}
				
				
				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case SBC_ZPX:
			{
				//fetching zero page base address 
				Byte BAL = fetch_byte(mem);

				Byte effective_addr = BAL + X;
				cycles--; //fetching from X register

				Byte M = readValue_on_address<Word>(mem, effective_addr);

				M = twoscomplement(M);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				Byte temp = A + M;

				A = A + M - ~bitwise_temp;

				if (temp > 0xff)
				{
					if (bitwise_temp == 1)
					{
						bitwise_temp = bitwise_temp | status_byte;

						bitwise_temp = bitwise_temp >> 1;
						bitwise_temp = bitwise_temp << 1;

						status_byte &= bitwise_temp;
					}
				}

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case SBC_AB:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;


				//reading value stored at the previously formed 16-bit address
				Byte M = readValue_on_address<Word>(mem, absolute_address);
				M = twoscomplement(M);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				Byte temp = A + M;

				A = A + M - ~bitwise_temp;

				if (temp > 0xff)
				{
					if (bitwise_temp == 1)
					{
						bitwise_temp = bitwise_temp | status_byte;

						bitwise_temp = bitwise_temp >> 1;
						bitwise_temp = bitwise_temp << 1;

						status_byte &= bitwise_temp;
					}
				}

				LDA_SET_FLAGS();
			}
			break;

			case SBC_ABX:
			{

				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + X > 0xffff)
				{
					cycles--;
				}

				Byte M = readValue_on_address<DWord>(mem, absolute_address + X);

				M = twoscomplement(M);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				Byte temp = A + M;

				A = A + M - ~bitwise_temp;

				if (temp > 0xff)
				{
					if (bitwise_temp == 1)
					{
						bitwise_temp = bitwise_temp | status_byte;

						bitwise_temp = bitwise_temp >> 1;
						bitwise_temp = bitwise_temp << 1;

						status_byte &= bitwise_temp;
					}
				}

				LDA_SET_FLAGS();

			}
			break;

			case SBC_ABY:
			{
				//getting first 8 bits (1 byte) of 16 bits address
				Byte address_byte1 = fetch_byte(mem);

				//getting second 8 bits of 16 bits address
				Byte address_byte2 = fetch_byte(mem);

				//forming a 16-bit address by adding the 2 individual 8-bit addresses
				Word absolute_address = address_byte2 * 0x100 + address_byte1;

				if (absolute_address + Y > 0xffff)
				{
					cycles--;
				}

				Byte M = readValue_on_address<DWord>(mem, absolute_address + Y);
				M = twoscomplement(M);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				Byte temp = A + M;

				A = A + M - ~bitwise_temp;

				if (temp > 0xff)
				{
					if (bitwise_temp == 1)
					{
						bitwise_temp = bitwise_temp | status_byte;

						bitwise_temp = bitwise_temp >> 1;
						bitwise_temp = bitwise_temp << 1;

						status_byte &= bitwise_temp;
					}
				}

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case SBC_INX:
			{
				Byte BAL = fetch_byte(mem);

				Word effective_addr = BAL + X;
				cycles--;

				Byte effective_addr_L = readValue_on_address<Byte>(mem, effective_addr);
				Byte effective_addr_H = readValue_on_address<Byte>(mem, effective_addr + 1);

				Word fetch_addr;
				if (effective_addr_L > 0xff)
				{
					fetch_addr = effective_addr_H * 0x100 + effective_addr_L;
				}

				else
				{
					fetch_addr = effective_addr_L;
				}

				Byte M = readValue_on_address<Word>(mem, fetch_addr);
				M = twoscomplement(M);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				Byte temp = A + M;

				A = A + M - ~bitwise_temp;

				if (temp > 0xff)
				{
					if (bitwise_temp == 1)
					{
						bitwise_temp = bitwise_temp | status_byte;

						bitwise_temp = bitwise_temp >> 1;
						bitwise_temp = bitwise_temp << 1;

						status_byte &= bitwise_temp;
					}
				}

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case SBC_INY:
			{

				Byte IAL = fetch_byte(mem);

				Byte BAL = readValue_on_address<Byte>(mem, IAL);
				Byte BAH = readValue_on_address<Byte>(mem, IAL + 1);

				DWord fetch_addr = BAH * 0x100 + BAL + Y;

				if (fetch_addr > 0xffff)
				{
					cycles--;
				}

				Byte M = readValue_on_address<DWord>(mem, fetch_addr);
				M = twoscomplement(M);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				Byte temp = A + M;

				A = A + M - ~bitwise_temp;
				
				if (temp > 0xff)
				{
					if (bitwise_temp == 1)
					{
						bitwise_temp = bitwise_temp | status_byte;

						bitwise_temp = bitwise_temp >> 1;
						bitwise_temp = bitwise_temp << 1;

						status_byte &= bitwise_temp;
					}
				}				

				LDA_SET_FLAGS();
			}
			break;

			case SEC_IMPLIED:
			{
				fetch_byte(mem);

				Byte bitwise_temp = status_byte << 7;
				bitwise_temp = bitwise_temp >> 7;

				if (bitwise_temp == 1)
				{
					//do nothing
				}
				else if (bitwise_temp == 0)
				{
					status_byte |= 0b00000001;
				}
				
			}
			break;

			//0010 0000

			case SED_IMPLIED:
			{
				fetch_byte(mem);

				Byte bitwise_temp = status_byte << 4;
				bitwise_temp = bitwise_temp >> 7;

				if (bitwise_temp == 1)
				{
					//do nothing, flag already 1
				}
				else if (bitwise_temp == 0)
				{
					status_byte |= 0b00101000;
				}
			}
			break;

			case SEI_IMPLIED:
			{
				fetch_byte(mem);

				Byte bitwise_temp = status_byte << 3;
				bitwise_temp = bitwise_temp >> 7;
				

				if (bitwise_temp == 1)
				{
					//do nothing 
				}
				else if (bitwise_temp == 0)
				{
					status_byte |= 0b00100100;
				}
			}
			break;

			case STA_ZP:
			{
				Byte zp_addr = fetch_byte(mem);

				explicit_write_back<Byte>(A, mem, zp_addr);

			}
			break;

			case STA_ZPX:
			{
				Byte zp_addr = fetch_byte(mem);

				Byte fetch_addr = readValue_on_address<Byte>(mem, zp_addr);

				fetch_addr += X;

				explicit_write_back<Byte>(A, mem, fetch_addr);
			}
			break;

			case STA_AB:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word effective_addr = high_byte * 0x100 + low_byte;

				explicit_write_back<Word>(A, mem, effective_addr);
			}
			break;

			case STA_ABX:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word lowX_sum = low_byte + X;

				low_byte += X;

				Word effective_addr = high_byte * 0x100 + low_byte;


				readValue_on_address<Word>(mem, effective_addr);

				effective_addr = high_byte * 0x100 + lowX_sum;

				explicit_write_back<Word>(A, mem, effective_addr);

			}
			break;

			case STA_ABY:
			{
				Byte low_byte = fetch_byte(mem);
				Byte high_byte = fetch_byte(mem);

				Word lowX_sum = low_byte + Y;

				low_byte += Y;

				Word effective_addr = high_byte * 0x100 + low_byte;


				readValue_on_address<Word>(mem, effective_addr);

				effective_addr = high_byte * 0x100 + lowX_sum;

				explicit_write_back<Word>(A, mem, effective_addr);
			}
			break;

			case STA_INX:
			{
				Byte pointer = fetch_byte(mem);

				readValue_on_address<Byte>(mem, pointer);

				pointer += X;

				Byte low_byte = readValue_on_address<Byte>(mem, pointer);

				pointer += 1;

				Byte high_byte = readValue_on_address<Byte>(mem, pointer);

				Word effective_addr = high_byte * 0x100 + low_byte;

				explicit_write_back<Word>(A, mem, effective_addr);
			}
			break;
			
			case STA_INY:
			{
				Byte pointer = fetch_byte(mem);

				Byte low_byte = readValue_on_address<Byte>(mem, pointer);

				pointer++;

				Byte high_byte = readValue_on_address<Byte>(mem, pointer);

				Word low_byte_no_loss = low_byte + Y;

				low_byte += Y;

				readValue_on_address<Byte>(mem, low_byte);

				Word effective_addr = high_byte * 0x100 + low_byte_no_loss;

				explicit_write_back<Word>(A, mem, effective_addr);
			}
			break;

			//STX

			case STX_ZP:
			{
				Byte addr = fetch_byte(mem);
				explicit_write_back<Byte>(X, mem, addr);
			}
			break;

			case STX_ZPY:
			{
				Byte addr = fetch_byte(mem);
				readValue_on_address<Byte>(mem, addr);
				addr += Y;
				explicit_write_back<Word>(X, mem, addr);
			}
			break;

			case STX_AB:
			{
				Byte BAL = fetch_byte(mem);
				Byte BAH = fetch_byte(mem);

				Word effective_addr = BAH * 0x100 + BAL;
				explicit_write_back<Word>(X, mem, effective_addr);
			}
			break;

			//STY

			case STY_ZP:
			{
				Byte addr = fetch_byte(mem);
				explicit_write_back<Byte>(Y, mem, addr);
			}
			break;

			case STY_ZPX:
			{
				Byte addr = fetch_byte(mem);
				readValue_on_address<Byte>(mem, addr);
				addr += X;
				explicit_write_back<Word>(Y, mem, addr);
			}
			break;

			case STY_AB:
			{
				Byte BAL = fetch_byte(mem);
				Byte BAH = fetch_byte(mem);

				Word effective_addr = BAH * 0x100 + BAL;
				explicit_write_back<Word>(Y, mem, effective_addr);
			}
			break;


			case TAX_IMPLIED:
			{
				fetch_byte(mem);
				X = A;

				if (X == 0)
				{
					status_byte |= 0b00100010;
				}

				Byte bitwise_temp = X >> 7;
				if (bitwise_temp == 1)
				{
					status_byte |= 0b10100000;
				}
			}
			break;

			case TAY_IMPLIED:
			{
				fetch_byte(mem);
				Y = A;

				if (Y == 0)
				{
					status_byte |= 0b00100010;
				}

				Byte bitwise_temp = X >> 7;
				if (bitwise_temp == 1)
				{
					status_byte |= 0b10100000;
				}
				
			}
			break;

			case TSX_IMPLIED:
			{
				fetch_byte(mem);

				X = stack_pointer;

				LDA_SET_FLAGS();
			}
			break;

			case TXA_IMPLIED:
			{
				fetch_byte(mem);

				A = X;

				LDA_SET_FLAGS();
			}
			break;

			case TXS_IMPLIED:
			{
				fetch_byte(mem);

				stack_pointer = X;

				LDA_SET_FLAGS();
			}
			break;

			case TYA_IMPLIED:
			{
				fetch_byte(mem);
				
				A = Y;

				LDA_SET_FLAGS();
			}
			break;

			

			

			default:
				cout << "\n\nIllegal opcode received! exiting..." << endl;
				exit(0);
				break;
			}
		}

	}

	Word getActualStackAddr(Byte stack_ptr)
	{
		Word Bit16_addr = 0x100 + stack_pointer;
		return Bit16_addr;
	}

	void LSR_SET_FLAGS(Byte value)
	{
		status_byte |= (((value << 7) ^ 0b00000000) >> 7);

		if (value == 0)
		{
			set_zero_flag();
		}

		if (value >> 7 == 1)
		{
			status_byte |= 0b10000000;
		}
	}

	//set flag functions
	void set_carry_flag()
	{
		status_byte |= 0b00000001;
	}

	void set_zero_flag()
	{
		status_byte |= 0b00000010;
	}

	void set_interruptdisable_flag()
	{
		status_byte |= 0b00000100;
	}

	void set_decimal_flag()
	{
		status_byte |= 0b00001000;
	}

	void set_B_flag()
	{
		status_byte |= 0b00010000;
	}

	void set_overflow_flag()
	{
		status_byte |= 0b01000000;
	}

	void set_negative_flag()
	{
		status_byte |= 0b10000000;
	}

};



int main()
{
	cpu obj;
	obj.ResetCPU();
	Memory mem;

	//hardcoding assembly

	//initializing program counter to read 2 bytes, join them and form one address to fetch the initial value 
	mem.memory_block[0xfffc] = 0x00;
	mem.memory_block[0xfffd] = 0x00;
	obj.program_counter = mem.memory_block[0xfffd] * 0x100 + mem.memory_block[0xfffc];
	//program counter initialization complete
}