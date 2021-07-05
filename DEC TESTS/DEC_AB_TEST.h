mem.memory_block[obj.program_counter] = 0xCE;
mem.memory_block[obj.program_counter + 1] = 0x80;
mem.memory_block[obj.program_counter + 2] = 0x43;

mem.memory_block[0x4380] = 0x68;

//ans - at addr 0x4380 value 0x67 present