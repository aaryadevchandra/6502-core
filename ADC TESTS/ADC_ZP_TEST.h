//ADC ZP TEST
obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0x65;
mem.memory_block[obj.program_counter + 1] = 0x31;
mem.memory_block[0x31] = 0x32;