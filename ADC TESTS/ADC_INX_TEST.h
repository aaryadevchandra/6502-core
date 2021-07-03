//ADC INX TEST
obj.A = 0x30;
mem.memory_block[obj.program_counter] = 0x61;
mem.memory_block[obj.program_counter + 1] = 0x20;
obj.X = 0x04;
mem.memory_block[0x24] = 0x0070;
mem.memory_block[0x0070] = 231;