obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0x31;
mem.memory_block[obj.program_counter + 1] = 0x20;
mem.memory_block[0x20] = 0xff;
mem.memory_block[0x21] = 0xff;
obj.Y = 0x4;
mem.memory_block[0x10003] = 45;

//ans = 0x20