mem.memory_block[obj.program_counter] = 0x5E;
mem.memory_block[obj.program_counter + 1] = 0x32;
mem.memory_block[obj.program_counter + 2] = 0x23;
obj.X = 0x1;
mem.memory_block[0x2332 + obj.X] = 0x14;

//ans - 0x0A at (0x2332 + obj.X)