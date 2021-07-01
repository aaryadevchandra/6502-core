#include<iostream>
using namespace std;


using Byte = unsigned char;
using Word = unsigned short int;
using DWord = unsigned int;



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

		LDA_IM = 0xA9,
		LDA_ZP = 0xA5,
		LDA_ZPX = 0xB5,
		LDA_AB = 0xAD,
		LDA_ABX = 0xBD,
		LDA_ABY = 0xB9,
		LDA_INX = 0xA1,
		LDA_INY = 0xB1;


	void LDA_SET_FLAGS()
	{
		if (A == 0)
		{
			ZF = 0;
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
				//fetching the immediate value to be stored
				Byte immediate_value = fetch_byte(mem);
				A = immediate_value;

				LDA_SET_FLAGS();
			}
			break;

			//3 cycles
			case LDA_ZP:
			{
				//fetching the zero page address where the value to be stored will be present
				Byte ZP_address = fetch_byte(mem);

				//fetching the byte from the zero page address we just fetched from memory
				A = readValue_on_address<Byte>(mem, ZP_address);

				LDA_SET_FLAGS();
			}
			break;

			//4 cycles
			case LDA_ZPX:
			{
				//fetching zero page address
				Byte ZP_address = fetch_byte(mem);

				Word ZPX_addr = ZP_address * 0x100 + X;
				cycles--;//for fetching X

				//reading value stored at ZPX_address
				A = readValue_on_address<Word>(mem, ZPX_addr);

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

				DWord ABX_addr = absolute_address + X;

				if (ABX_addr > 0xffff)
				{
					cycles--;
				}

				A = readValue_on_address<DWord>(mem, ABX_addr);

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

				DWord ABY_addr = absolute_address + Y;

				if (ABY_addr > 0xffff)
				{
					cycles--;
				}

				A = readValue_on_address<DWord>(mem, ABY_addr);

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case LDA_INX:
			{
				A = readValue_on_address<Word>(mem, readValue_on_address<Word>(mem, (fetch_byte(mem) + X)));
				cycles--;//for reading X reg
				cycles--;//for writing to A reg

				LDA_SET_FLAGS();
			}
			break;

			//6 cycles
			case LDA_INY:
			{
				A = readValue_on_address<Word>(mem, readValue_on_address<Word>(mem, (fetch_byte(mem) + Y)));
				cycles--;//for reading Y reg
				cycles--;//for writing to A reg

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

	//LDA immediate test
	/*mem.memory_block[obj.program_counter] = 0xA9;
	mem.memory_block[obj.program_counter + 1] = 95;*/



	//LDA zero page instruction test
	/*mem.memory_block[obj.program_counter] = 0xa5;
	mem.memory_block[obj.program_counter + 1] = 0x67;
	mem.memory_block[0x67] = 78;*/

	//LDA ZPX test
	/*mem.memory_block[obj.program_counter] = 0xB5;
	mem.memory_block[obj.program_counter + 1] = 0x43;
	obj.X = 0x34;
	mem.memory_block[0x4334] = 69;*/
	
	//LDA_AB test
	/*mem.memory_block[0x1000] = 0xAD;
	mem.memory_block[0x1000 + 1] = 0xca;
	mem.memory_block[0x1000 + 2] = 0xac;
	mem.memory_block[0xacca] = 69;*/

	//LDA_ABX test
	/*mem.memory_block[0x1000] = 0xBD;
	mem.memory_block[0x1000 + 1] = 0xff;
	mem.memory_block[0x1000 + 2] = 0xff;
	obj.X = 0x1;
	mem.memory_block[0xffff + obj.X] = 69;*/

	//LDA_ABY test
	/*mem.memory_block[0x1000] = 0xB9;
	mem.memory_block[0x1000 + 1] = 0xff;
	mem.memory_block[0x1000 + 2] = 0xff;
	obj.Y = 0x1;
	mem.memory_block[0xffff + obj.Y] = 69;*/

	//LDA indirect indexed X test
	/*mem.memory_block[0x1000] = 0xA1;
	mem.memory_block[0x1000 + 1] = 0x20;
	obj.X = 0x04;
	mem.memory_block[0x24] = 0x70;
	mem.memory_block[0x70] = 231;*/

	//LDA indirect indexed Y test
	/*mem.memory_block[0x1000] = 0xB1;
	mem.memory_block[0x1000 + 1] = 0x20;
	obj.X = 0x04;
	mem.memory_block[0x24] = 0x70;
	mem.memory_block[0x70] = 231;*/

}