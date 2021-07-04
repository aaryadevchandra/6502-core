obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0xD9;
mem.memory_block[0x1000 + 1] = 0xff;
mem.memory_block[0x1000 + 2] = 0xff;
obj.Y = 0x1;
mem.memory_block[0xffff + obj.Y] = 0x14;

//ans = 0x10