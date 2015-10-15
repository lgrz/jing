TARGET = jing2indigo

LEX = flex
YACC = bison -d
CC = gcc
CFLAGS += -std=c99 -Wall -Wextra -pedantic -O2 -D_XOPEN_SOURCE=700 -Isrc
LDFLAGS +=
DEBUG_CFLAGS = -g -O0 -DYYDEBUG=1 -DDEBUG

SRC = src/main.c src/node.c src/symtab.c src/util.c src/array.c \
	  src/emitter.c src/semcheck.c src/strbuf.c
OBJ := src/parse.o $(SRC:.c=.o)
DEP := $(patsubst %.c,%.d,$(SRC)) src/parse.d

TESTDIR = test
TPASS_TESTS = $(wildcard test/trans-pass/*.jing)
TFAIL_TESTS = $(wildcard test/trans-fail/*.jing)
CHECK_QUICK = check-jing check-tpass check-tfail
CHECK_FULL = $(CHECK_QUICK) check-tvalgrind

.PHONY: all
all: $(TARGET)

debug: YACC := $(YACC) -v
debug: CFLAGS := $(CFLAGS:-O%=)
debug: CC := $(CC) $(DEBUG_CFLAGS)
debug: all

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ src/lex.yy.c $(LDFLAGS)

src/y.tab.c: src/parse.y src/lex.yy.c
	$(YACC) -o $@ $<

src/lex.yy.c: src/lex.l
	$(LEX) -o $@ $^

src/parse.o: src/y.tab.c src/lex.yy.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ $(LDFLAGS)

.PHONY: check
check: $(CHECK_QUICK)

.PHONY: check-full
check-full: $(CHECK_FULL)

.PHONY: check-jing
check-jing: test-jing
	./test/all -c

.PHONY: test-jing
test-jing: $(TARGET) | local.mk
	$(MAKE) -C $(TESTDIR) $@

local.mk:
	@echo "# For customisation of CPPUTEST_PATH" > local.mk
	@echo "CPPUTEST_PATH=/path/to/cpputest" >> local.mk

.PHONY: check-tpass
check-tpass: $(TPASS_TESTS) $(TARGET)
	./test/tpass ./$(TARGET) $(TPASS_TESTS)

.PHONY: check-tfail
check-tfail: $(TFAIL_TESTS) $(TARGET)
	./test/tfail ./$(TARGET) $(TFAIL_TESTS)

.PHONY: check-tvalgrind
check-tvalgrind: $(TPASS_TESTS) $(TARGET)
	./test/tvalgrind ./$(TARGET) $(TPASS_TESTS)

.PHONY: clean
clean: | local.mk
	$(MAKE) -C $(TESTDIR) $@
	$(RM) $(TARGET) $(OBJ) $(DEP) \
		src/lex.yy.c src/y.tab.{c,h} src/y.output

-include $(DEP)
