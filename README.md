# 6502-emulator

Currently the project is in progress. The 6502 has 56 instructions out of which I have implemented 4 (LDA, LDX, LDY, ADC with all addressing modes). The project is clock cycle accurate and ... well yeah that's all for now. 

For each addressing mode of every instruction, I'll be uploading some codes that you can use to test the instruction out. I will be trying to make it a bit modular so as to avoid adding all tests in the same file as well as make custom headers to make it look a bit cleaner. 

This is first push so the tests are in the file itself, feel free to play around.

I have used [this](http://www.obelisk.me.uk/6502/reference.html) site as reference for the emulator. See you soon, adios!


# How To Use
_______________________________

- The assembly according to which the instructions will be called is hardcoded in the code itself. 
- The assembler would usually arrange your assembly code in memory, however I have done that job by manually entering it into memory i.e. opcodes for specific instructions and their operands have been manually assigned to certain indices of the memory byte array. 

- These are the "tests" I was talking about. To use the program, just call the `execute_instruction()` function which takes the number of clock cycles and then an instance of the memory structure as parameters. 

- The clock cycles _have_ to be accurate according to the instruction and its specific addressing mode you wish to run or the program will fail.
- Use this [reference manual](http://www.obelisk.me.uk/6502/reference.html) given above to find the number cycles for a particular instruction belonging to a specific addressing mode. 

- Another resource to help with addresing modes in the 6502 would be [this](http://www.emulator101.com/6502-addressing-modes.html).
