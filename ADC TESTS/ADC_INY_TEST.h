//ADC INY TEST
obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0x71;
mem.memory_block[obj.program_counter + 3] = 0x20;
mem.memory_block[0x20] = 0xff;
mem.memory_block[0x21] = 0xff;
obj.Y = 0x4;
mem.memory_block[0x10003] = 45;