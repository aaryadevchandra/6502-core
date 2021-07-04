obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0x3D;
mem.memory_block[obj.program_counter + 1] = 0xff;
mem.memory_block[obj.program_counter + 2] = 0xff;
obj.X = 0x1;
mem.memory_block[0xffff + obj.X] = 0x14;

//ans = 0x10