//ADC ZPX TEST
obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0x75;
mem.memory_block[obj.program_counter + 1] = 0x31;
obj.X = 0x32;
mem.memory_block[0x31 + 0x32] = 0x33;