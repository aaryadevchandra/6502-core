//ADC AB TEST
obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0x6D;
mem.memory_block[obj.program_counter + 1] = 0xca;
mem.memory_block[obj.program_counter + 2] = 0xac;
mem.memory_block[0xacca] = 0x31;