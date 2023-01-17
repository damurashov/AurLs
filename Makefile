CC := gcc
EXEC_NAME := dmls
C_SOURCES := $(wildcard src/*.c)
C_OBJECTS := $(addprefix build/,$(C_SOURCES:.c=.o))
C_FLAGS := -Wall
LD_FLAGS := -Wall

build: build/$(EXEC_NAME)

build/$(EXEC_NAME): $(C_OBJECTS)
	$(CC) -o $@ $^ $(LD_FLAGS)

build/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(C_FLAGS) $< -o build/$*.o

clean:
	rm -rf build/$(EXEC_NAME)

distclean:
	rm -rf build/
