# GBA project Makefile
# 
# kernow x/5/05


GBA_SRC = mode4.c
GBA_SRC += bloke.o
GBA_SRC += bloke_pal.o
GBA_ELF = mode4.elf
GBA_BIN = mode4.gba


CC = arm-elf-gcc
OBJCOPY = arm-elf-objcopy


CFLAGS = -Wall -I./include $(GBA_SRC) -o $(GBA_ELF) -mthumb-interwork --specs=gba.specs



ARCHIVE = gba-mode4


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

	      
