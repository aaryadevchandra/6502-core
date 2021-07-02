//LDA_ABX test
mem.memory_block[0x1000] = 0xBD;
mem.memory_block[0x1000 + 1] = 0xff;
mem.memory_block[0x1000 + 2] = 0xff;
obj.X = 0x1;
mem.memory_block[0xffff + obj.X] = 69;