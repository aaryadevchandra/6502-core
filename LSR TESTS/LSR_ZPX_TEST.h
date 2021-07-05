obj.X = 0x32;
mem.memory_block[obj.program_counter] = 0x56;
mem.memory_block[obj.program_counter + 1] = 0x32;
mem.memory_block[0x64] = 0x96;

//ans - 4B at 0x96