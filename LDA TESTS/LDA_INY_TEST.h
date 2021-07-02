//LDA indirect indexed Y test ( crosses page )
mem.memory_block[obj.program_counter + 2] = 0xB1;
mem.memory_block[obj.program_counter + 3] = 0x20;
mem.memory_block[0x20] = 0xff;
mem.memory_block[0x21] = 0xff;
obj.Y = 0x4;
mem.memory_block[0x10003] = 45;