mem.memory_block[obj.program_counter] = 0x4C;
mem.memory_block[obj.program_counter + 1] = 0x34;
mem.memory_block[obj.program_counter + 2] = 0x43;

mem.memory_block[0x4334] = 0x69;

//0x4334 should hold 0x69