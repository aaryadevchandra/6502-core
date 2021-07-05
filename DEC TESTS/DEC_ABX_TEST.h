obj.X = 0x32;
mem.memory_block[obj.program_counter] = 0xDE;
mem.memory_block[obj.program_counter + 1] = 0x80;
mem.memory_block[obj.program_counter + 2] = 0x43;

mem.memory_block[0x43B2] = 0x68;

//ans - 0x67  at 0x43b2