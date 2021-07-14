# 6502-emulator
_____________________________________

Currently the project is in progress. The 6502 has 56 instructions out of which I have implemented 37 (LDA, LDX, LDY, ADC, AND, CMP, DEC, DEX, DEY, INC, INX, INY, JMP, JSR, LSR, CPY, CPX, EOR, PHA, PLA, PHP, NOP, ROL, ROR, ORA, SBC, SEC, SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA, with all addressing modes). The project is clock cycle accurate and ... well yeah that's all for now. 

For each addressing mode of every instruction, I'll be uploading some codes that you can use to test the instruction out. I will be trying to make it a bit modular so as to avoid adding all tests in the same file as well as make custom headers to make it look a bit cleaner. 


# How To Use
__________________________________

- The assembly according to which the instructions will be called is hardcoded in the code itself. 
- The assembler would usually arrange your assembly code in memory, however I have done that job by manually entering it into memory i.e. opcodes for specific instructions and their operands have been manually assigned to certain indices of the memory byte array. 

- These are the "tests" I was talking about. To use the program, just call the `execute_instruction()` function which takes the number of clock cycles and then an instance of the memory structure as parameters. 

- The clock cycles _have_ to be accurate according to the instruction and its specific addressing mode you wish to run or the program will fail.


# Resources
_________________________________________________
- The base instruction reference with information related to [opcodes and number of cycles](http://www.obelisk.me.uk/6502/reference.html)

- Information related to [addressing modes in the 6502](http://www.emulator101.com/6502-addressing-modes.html) 

- And at last, the absolute goldmine of a website is [this](https://nesdev.com/6502_cpu.txt), with detailed information on what each cycle in the 6502 does for _every instruction_ and _for every addressing mode of an instruction_. This site is hands-down the best resource for 6502 development.

- Another possible resource which would _have_ to be mentioned is the [official 6502 manual](https://archive.org/details/mos_microcomputers_programming_manual/mode/2up) from MOS Technologies. 