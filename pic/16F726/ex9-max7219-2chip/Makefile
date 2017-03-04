# Compiling variables.
CC		:= ../../../tools/cc5x.exe
CCFLAGS	:= -a -I"..\..\lib" -L -V -Q -B

SRC  	:= main.c
HEX		:= out.hex

all:$(HEX)

$(HEX): $(SRC)
	wine $(CC) -o$(HEX) $(CCFLAGS) $(SRC)

clean:
	@echo "delete file"
	rm -f $(HEX)
	rm -f *.cpr *.fcs *.lst *.occ *.var *.asm
