ps2	?= 0
binary	:= larkfx

debug	?= 0
paths	:= src
source	:= $(foreach path,$(paths),$(wildcard $(path)/*.c))
objects	:= $(patsubst %.c,%.o,$(source))
incs	:= -Iinclude 
flags	:= -lc 

ifeq ($(ps2),1)
	objects		:= $(patsubst %.c,%-ps2.o,$(source))
	binary		:= larkfx-ps2.elf
	prefix		:= mips64r5900el-ps2-elf-
	linkfile	:= -L$(PS2SDK)/ee/startup/linkfile
	incs		+= -I$(PS2SDK)/common/include \
			   -I$(PS2SDK)/ee/include
	libs		:= -L$(PS2SDK)/ee/lib
else
	prefix		:=
	incs		+= -I/usr/local/include -Iinclude
	libs		+= -L/usr/local/lib
endif

linker		:= $(prefix)ld
compiler	:= $(prefix)gcc

all: clean $(binary)

clean:
	rm -rf $(binary) $(objects)

$(binary): $(objects)
	$(compiler) $? -o $@

%-ps2.o: %.c
	$(compiler) $(flags) $(incs) -c $< -o $@

%.o: %.c
	$(compiler) $(flags) $(incs) -c $< -o $@
