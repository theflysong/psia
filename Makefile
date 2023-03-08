TOOL_PREFIX :=
AR := $(TOOL_PREFIX)ar

GCC := $(TOOL_PREFIX)gcc

PLATFORM := windows

SOURCES := psia \
	arch/$(PLATFORM)/$(PLATFORM)_api

TESTBENCH_A_SOURCES := A
TESTBENCH_B_SOURCES := B

OBJECTS := $(foreach src, $(SOURCES), src/$(src).o)

TESTBENCH_A_OBJECTS := $(foreach src, $(TESTBENCH_A_SOURCES), src/testbench/$(src).o)
TESTBENCH_B_OBJECTS := $(foreach src, $(TESTBENCH_B_SOURCES), src/testbench/$(src).o)

OUTPUT := libpsia.a
TESTBENCH_A := a
TESTBENCH_B := b

FLAGS := -I"./src/" -std=c99

AR_FLAGS := r

src/%.o : src/%.c
	$(GCC) $(FLAGS) $^ -c -o $@

$(OUTPUT) : $(OBJECTS)
	$(AR) $(AR_FLAGS) $@ $^

.PHONY: testbench
testbench: $(TESTBENCH_A_OBJECTS) $(TESTBENCH_B_OBJECTS)
	$(GCC) $(TESTBENCH_A_OBJECTS) -o $(TESTBENCH_A)
	$(GCC) $(TESTBENCH_B_OBJECTS) -L"./" -lpsia -o $(TESTBENCH_B)