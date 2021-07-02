//LDX ZPY test
mem.memory_block[obj.program_counter] = 0xB6;
mem.memory_block[obj.program_counter + 1] = 0x43;
obj.Y = 0x34;
mem.memory_block[0x77] = 69;