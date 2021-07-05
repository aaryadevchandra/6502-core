obj.X = 0x32;
mem.memory_block[obj.program_counter] = 0xD6;
mem.memory_block[obj.program_counter + 1] = 0x43;

mem.memory_block[0x75] = 0x56;

//ans - 0x57 at 0x75