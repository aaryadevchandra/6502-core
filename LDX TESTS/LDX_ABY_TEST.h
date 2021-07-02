//LDX_ABY test
mem.memory_block[0x1000] = 0xBE;
mem.memory_block[0x1000 + 1] = 0xff;
mem.memory_block[0x1000 + 2] = 0xff;
obj.Y = 0x1;
mem.memory_block[0xffff + obj.Y] = 69;
