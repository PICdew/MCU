# Compiling variables.
CC		:= ../../../tools/cc5x/CC5X.EXE
CCFLAGS	:= -a -I"..\..\lib" -L -V -Q -B

SRC  	:= main.c
HEX		:= out.hex

all:chk_cc5x $(HEX)

chk_cc5x:
	@if [ ! -f $(CC) ];then									\
		cd ../../../tools && unzip cc5xfree.zip -d cc5x; 	\
	fi

$(HEX): $(SRC)
	wine $(CC) -o$(HEX) $(CCFLAGS) $(SRC)

clean:
	@echo "delete file"
	rm -f $(HEX)
	rm -f *.cpr *.fcs *.lst *.occ *.var *.asm
