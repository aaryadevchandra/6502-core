obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0xD5;
mem.memory_block[obj.program_counter + 1] = 0x31;
obj.X = 0x32;
mem.memory_block[0x31 + 0x32] = 0x14;

//ans = 0x14