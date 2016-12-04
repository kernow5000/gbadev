# GBA project Makefile
# 
# kernow x/5/05

# problems/errata
# *.o compiling support needed
# tar doesn't get dir/ but just * inside it
# nothing much else really

ARCHIVE = gba-kern1

GBA_SRC = main.c
GBA_SRC += lz77.c
GBA_SRC += include/gfx/titlescreen_lz77.o
GBA_SRC += include/gfx/titlescreen_pal.o
GBA_ELF = main.elf
GBA_BIN = main.gba

CC = arm-elf-gcc
OBJCOPY = arm-elf-objcopy


CFLAGS = -Wall -I./include $(GBA_SRC) -o $(GBA_ELF) -mthumb-interwork --specs=gba.specs


all: clean gba_elf gba_bin

gba_elf:      
	      $(CC) $(CFLAGS)
	      
gba_bin:
	      $(OBJCOPY) -v -O binary $(GBA_ELF) $(GBA_BIN)

clean:	      
	      rm -Rf *~ $(GBA_ELF) $(GBA_BIN)

backup:
	      tar cf $(ARCHIVE).tar *
	      gzip $(ARCHIVE).tar
	      mv $(ARCHIVE).tar.gz $(HOME)/dev/GBA/old/

