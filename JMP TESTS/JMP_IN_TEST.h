mem.memory_block[obj.program_counter] = 0x6C;
mem.memory_block[obj.program_counter + 1] = 0x34;
mem.memory_block[obj.program_counter + 2] = 0x43;

mem.memory_block[0x4334] = 0x23;
mem.memory_block[0x4335] = 0x32;

//and - PC should become 0x3223