include ../micropython/py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE = ee-
include $(PS2SDK)/samples/Makefile.eeglobal

EE_AR = ee-ar

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

EE_BIN = $(BUILD)/firmware.elf
EE_LIB = libmicropython.a
CFLAGS = $(INC) $(EE_INCS) -Wall -std=c99 $(EE_CFLAGS) $(EE_NO_CRT) $(COPT)
LDFLAGS = $(EE_LDFLAGS) -T$(PS2SDK)/ee/startup/linkfile --gc-sections

# Tune for Debugging or Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -D__DEBUG
else
CFLAGS += -DNDEBUG
CFLAGS += -fdata-sections -ffunction-sections
endif

LIBS = -lc -lkernel -lahx

SRC_C = \
	main.c \
	uart_core.c \
	lib/utils/printf.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/libc/string0.c \
	lib/mp-readline/readline.c

OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

PY_BYTECODE = frozentest.mpy
PY_SCRIPT = frozentest.py

all: $(EE_LIB)

remake: clean all

fullclean: clean
	rm -f *.mpy *.a *.elf

test: install $(PY_BYTECODE)
	$(MAKE) -f Makefile.pyfreeze test

retest: fullclean test
	
install: $(EE_LIB)
	cp -f $< $(PS2SDK)/ports/lib
	mkdir -p $(PS2SDK)/ports/include/micropython
	mkdir -p $(PS2SDK)/ports/include/micropython/genhdr
	cp -f $(BUILD)/genhdr/qstrdefs.preprocessed.h $(PS2SDK)/ports/include/micropython/genhdr
	cp -f $(BUILD)/genhdr/qstrdefs.generated.h $(PS2SDK)/ports/include/micropython/genhdr
	mkdir -p $(PS2DEV)/share/micropython
	cp -f ../micropython/tools/mpy-tool.py $(PS2DEV)/share/micropython
	cp -f ../micropython/py/makeqstrdata.py $(PS2DEV)/share/micropython
	cp -f Makefile.pyfreeze $(PS2DEV)/share/micropython/Makefile
	cp -f ps2-upython $(PS2DEV)/bin
	mkdir -p $(PS2SDK)/ports/include/micropython/py
	cp -f ../micropython/py/*.h $(PS2SDK)/ports/include/micropython/py
	cp *.h $(PS2SDK)/ports/include/micropython

$(PY_BYTECODE): $(PY_SCRIPT)
	mpy-cross $<
	
$(EE_LIB): $(OBJ)
	$(EE_AR) cru $(EE_LIB) $(OBJ)

#$(BUILD)/_frozen_mpy.c: $(PY_BYTECODE) $(BUILD)/genhdr/qstrdefs.generated.h
#	$(ECHO) "MISC freezing bytecode"
#	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

include $(TOP)/py/mkrules.mk
