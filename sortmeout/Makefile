# GBA project Makefile
# 
# kernow 7/10/03 
#
# tweakored to strip elf by GG 11/03/2004


GBA_SRC = crt0.o main.c sprites.c interrupts.c dma.c lz77.c 2d.c rand.c quicksort.c collision.c
IMAGES = include/gfx/score_lz77.o include/gfx/nums_lz77.o include/gfx/shot_lz77.o include/gfx/alien1-1_lz77.o include/gfx/sprite_pal.o include/gfx/titlescreen_lz77.o include/gfx/titlescreen_pal.o include/gfx/levels/level1_lz77.o include/gfx/levels/level1_pal.o
GBA_ELF = main.elf
GBA_BIN = main.gba


CC = arm-elf-gcc
STRIP = arm-elf-strip
OBJCOPY = arm-elf-objcopy
CFLAGS = -nostartfiles -O0 -mthumb-interwork -T lnkscript -I./include $(GBA_SRC) $(IMAGES) -o $(GBA_ELF)

ARCHIVE = gba-shooter

EMU_NAME = VisualBoyAdvance



all: clean gba_elf gba_bin

gba_elf:      
	      $(CC) $(CFLAGS)
	      $(STRIP) $(GBA_ELF)	      


gba_bin: gba_elf
	      $(OBJCOPY) -v -O binary $(GBA_ELF) $(GBA_BIN)
	      
	      
clean:	      
	      rm -Rf *~ include/*~ $(GBA_ELF) $(GBA_BIN)

run:
	      $(EMU_NAME) $(GBA_BIN)

backup:
	      tar cf $(ARCHIVE).tar *
	      gzip $(ARCHIVE).tar
	      mv $(ARCHIVE).tar.gz $(HOME)
