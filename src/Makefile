NO_MAIN_FILES=resource.c
SOLUTION_FILES=$(NO_MAIN_FILES) solution.c 

HEADERS=$(wildcard *.h)

EXE_DIR=../release/bin/

CFLAGS=-D _GNU_SOURCE -g -std=c99 `pkg-config --cflags --libs gtk+-2.0 gthread-2.0`

all: $(EXE_DIR)/solution \

$(EXE_DIR)/solution: $(patsubst %.c,%.o,$(SOLUTION_FILES))
	@echo Building $@
	@gcc -o $@ $^ $(CFLAGS)

%.o: %.c
	@echo Building $@
	@gcc -o $@ -c $^ $(CFLAGS)
