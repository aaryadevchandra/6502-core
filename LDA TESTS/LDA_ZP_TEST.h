//LDA zero page instruction test
mem.memory_block[obj.program_counter] = 0xa5;
mem.memory_block[obj.program_counter + 1] = 0x67;
mem.memory_block[0x67] = 78;