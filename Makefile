SRC := utest/src/test_mock.cc utest/src/utest.cc 
# SRC += $(wildcard UCOSII/port-win32/*.c)

# KERNEL_SRC:=os_core.c  os_flag.c  os_mem.c    os_q.c    os_task.c  os_tmr.c\
# 	os_dbg.c   os_mbox.c  os_mutex.c  os_sem.c  os_time.c

# SRC += $(KERNEL_SRC:%.c=UCOSII/src/%.c)
# SRC += $(KERNEL_SRC:%.c=Source/%.c)

SRC += main_deprecated.cc test2.cc

OBJ := $(SRC:%=build/%.o)

CC := gcc
CPP := g++
CPPFLAG :=
APP_CFLAGS := -g -O0
# APP_CFLAGS += -ISource
APP_CFLAGS += -Iutest/include
LINK := g++
LINK_FLAG := -g
ECHO := echo
MKDIR := mkdir -p

# MINGW_LIB:=/mingw64/x86_64-w64-mingw32/lib
# LIB:=$(MINGW_LIB)/libwinmm.a 

define compile_c
@$(ECHO) Info: Compiling $< to $@
@$(MKDIR) $(@D)
$(CC) -MP -MMD -c $(APP_CFLAGS) -o $@ $<
$(CC_POST_PROCESS)
endef

define compile_cpp
@$(ECHO) Info: Compiling $< to $@
@$(MKDIR) $(@D)
$(CPP) -MP -MMD -c $(APP_CFLAGS) $(CPPFLAG) -o $@ $<
$(CPP_POST_PROCESS)
endef

build/%.c.o: %.c
	$(compile_c)

build/%.cc.o: %.cc
	$(compile_cpp)

build/%.cpp.o: %.cpp
	$(compile_cpp)

all: ${OBJ}
	$(LINK) $(LINK_FLAG) -o main.exe ${OBJ}

clean:
	@rm -rf build
	@rm main.exe
