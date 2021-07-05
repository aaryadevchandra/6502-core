mem.memory_block[obj.program_counter] = 0x46;
mem.memory_block[obj.program_counter + 1] = 0x32;
mem.memory_block[0x32] = 0x96;
//ans - value at 0x32 should become 4B