mem.memory_block[obj.program_counter] = 0xC6;
mem.memory_block[obj.program_counter + 1] = 0x43;

mem.memory_block[0x43] = 0x69;

//ans - 0x70 at 0x43