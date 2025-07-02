ps2	?= 0
binary	:= larkfx

debug	?= 0
paths	:= src
source	:= $(foreach path,$(paths),$(wildcard $(path)/*.c))
objects	:= $(patsubst %.c,%.o,$(source))
incs	:= -Iinclude 
flags	:= -Wno-unused-variable -lc -lSDL2main -lSDL2 -lSDL2_ttf 

ifeq ($(debug), 1)
	flags += -DDEBUG
endif

ifeq ($(ps2),1)
	objects		:= $(patsubst %.c,%-ps2.o,$(source))
	binary		:= larkfx-ps2.elf
	prefix		:= mips64r5900el-ps2-elf-
	linkfile	:= -L$(PS2SDK)/ee/startup/linkfile
	incs		+= -I$(PS2SDK)/common/include \
			   -I$(PS2SDK)/ee/include \
			   -I$(PS2SDK)/ports/include/SDL2

	libs		:= -L$(PS2SDK)/ee/lib \
			   -L$(PS2SDK)/ports/lib

else
	prefix		:=
	incs		+= -I/usr/include -Iinclude -I/usr/include/SDL2
	libs		+= -L/usr/lib
endif

linker		:= $(prefix)ld
compiler	:= $(prefix)gcc

all: clean $(binary)

clean:
	rm -rf $(binary) $(objects)

$(binary): $(objects)
	$(compiler) $(flags) $(incs) $? -o $@

%-ps2.o: %.c
	$(compiler) $(flags) $(incs) -c $< -o $@

%.o: %.c
	$(compiler) $(flags) $(incs) -c $< -o $@
