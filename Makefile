ps2	?= 0
binary	:= larkfx

debug	?= 0
paths	:= src
source	:= $(foreach path,$(paths),$(wildcard $(path)/*.c))
objects	:= $(patsubst %.c,build/%.o,$(source))
incs	:= -Iinclude 
ldflags	:= -lc -lSDL2main -lSDL2 -lSDL2_ttf 

ifeq ($(debug), 1)
	flags += -DINTERACTIVE
endif

prefix		:=
incs		+= -I/usr/include -Iinclude -I/usr/include/SDL2
libs		+= -L/usr/lib
linker		:= $(prefix)ld
compiler	:= $(prefix)gcc

ifeq ($(ps2),1)
	objects		:= $(patsubst %.c,build/%-ps2.o,$(source))
	binary		:= larkfx-ps2.elf
	prefix		:= mips64r5900el-ps2-elf-
	linker		:= $(prefix)ld
	compiler	:= $(prefix)gcc
	
	linkfile	:= -T$(PS2SDK)/ee/startup/linkfile
	incs		:= -Iinclude \
			   -I$(PS2SDK)/common/include \
			   -I$(PS2SDK)/ee/include \
			   -I$(PS2SDK)/ports/include \
			   -I$(PS2SDK)/ports/include/SDL2 \
			   -I$(GSKIT)/include \

	libs		:= -L$(PS2SDK)/ee/lib \
			   -L$(PS2SDK)/ports/lib \
			   -L$(GSKIT)/lib

	flags		+= -D_PS2=1 -D_EE 
	
	ldflags		:= -lSDL2 -lSDL2_ttf -lgskit_toolkit -lgskit \
			   -ldmakit -lpad -lpatches -laudsrv -lps2_drivers \
			   -lmtap -lfreetype -lz -lpng -ldebug -lkernel

	# I     H A T E  linkers absolutely fuck you even if you arent even real
	# ldflags		:= -lpad -lpatches -ldmakit -laudsrv -lps2_drivers -lgskit_toolkit -lmtap -lfreetype -lz -lpng -lSDL2 -lSDL2_ttf

endif

# A clean is necessary due to header files

all: clean $(binary)

clean:
	rm -rf $(binary) $(objects)

$(binary): $(objects)
	@test -d $(@D) || mkdir -p $(@D)
	$(compiler) $(flags) $(incs) $^ $(libs) $(ldflags) -o $@ -g
	
build/%-ps2.o: %.c
	@test -d $(@D) || mkdir -p $(@D)
	$(compiler) $(incs) $(flags) -c $< -o $@ -g

build/%.o: %.c
#@echo Building $@...
	
	@test -d $(@D) || mkdir -p $(@D)
	$(compiler) $(flags) $(libs) $(ldflags) $(incs) -c $< -o $@ -g
