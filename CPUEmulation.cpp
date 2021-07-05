#include<iostream>
#include "type_definitions.h"
using namespace std;

//program counter - will hold the memory address of the next instruction to be executed
//instruction register - will hold the current instruction (opcode) to be executed 

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

	//Flags
	Byte CF;
	Byte ZF;
	Byte IDF;
	Byte DMF;
	Byte BCF;
	Byte OF;
	Byte NF;


	//initializing register and flag values
	void ResetCPU()
	{
		stack_pointer = 0xFF;
		program_counter = 0xFFFC;

		CF = 0;
		ZF = 0;
		IDF = 0;
		DMF = 0;
		BCF = 0;
		OF = 0;
		NF = 0;
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
		LDY_ABX = 0xBC;


	void LDA_SET_FLAGS()
	{
		if (A == 0)
		{
			ZF = 1;
		}

		NF = (A & 0b01000000) > 0;
	}

	void ADC_SET_FLAGS()
	{
		if (A > 0xffff)
		{
			CF = 1;
		}

		if (A == 0)
		{
			ZF = 1;
		}

		NF = (A & 0b01000000) > 0;
	}

	void CMP_SET_FLAGS(Byte A, Byte cmp_byte)
	{
		if (A >= cmp_byte)
		{
			CF = 1;
		}

		if (A == cmp_byte)
		{
			ZF = 1;
		}

		NF = (A & 0b01000000) > 0;
	}

	void DEC_INC_SET_FLAGS(Byte result)
	{
		if (result == 0)
		{
			ZF = 1;
		}

		NF = (A & 0b01000000) > 0;
	}

	//fetch byte from memory
	Byte fetch_byte(Memory mem)
	{
		Byte byte= mem.memory_block[program_counter];
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
	void explicit_write_back(T value, Memory &mem, T addr)
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
				
				cin.get();

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

};

int main()
{
	cpu obj;
	obj.ResetCPU();
	Memory mem;
	
	//hardcoding assembly
	
	//initializing program counter to read 2 bytes, join them and form one address to fetch the initial value 
	mem.memory_block[0xfffc] = 0x00;
	mem.memory_block[0xfffd] = 0x10;
	obj.program_counter = mem.memory_block[0xfffd] * 0x100 + mem.memory_block[0xfffc];
	//program counter initialization complete
}