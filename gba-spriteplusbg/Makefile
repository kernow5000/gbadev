# GBA project Makefile
# 
# kernow x/5/05


GBA_SRC = spriteplusbg.c
GBA_ELF = spriteplusbg.elf
GBA_BIN = spriteplusbg.gba


CC = arm-elf-gcc
OBJCOPY = arm-elf-objcopy

CFLAGS = -Wall -I./include $(GBA_SRC) -o $(GBA_ELF) -mthumb -specs=gba.specs



ARCHIVE = gba-spriteplusbg


all: clean gba_elf gba_bin

gba_elf:      
	      $(CC) $(CFLAGS)
	      
gba_bin:
	      $(OBJCOPY) -v -O binary $(GBA_ELF) $(GBA_BIN)
	      
run:		
		VisualBoyAdvance $(GBA_BIN)	
	
clean:	      
	      rm -Rf *~ $(GBA_ELF) $(GBA_BIN)

backup:
	      tar cf $(ARCHIVE).tar *
	      gzip $(ARCHIVE).tar
	      mv $(ARCHIVE).tar.gz $(HOME)/dev/GBA/old/
