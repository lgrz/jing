TARGET = jing2indigo

LEX = flex
YACC = bison -d
CC = gcc
CFLAGS += -Wall -Wextra -pedantic -O0 -DYYDEBUG=1 -g -Isrc
LDFLAGS +=

SRC = src/main.c src/node.c src/symtab.c src/util.c src/array.c \
	  src/emitter.c src/semcheck.c
OBJ := $(SRC:.c=.o) src/parse.o
DEP := $(patsubst %.c,%.d,$(SRC)) src/parse.d

TESTDIR = test
TPASS_TESTS = $(wildcard test/trans-pass/*.jing)
TFAIL_TESTS = $(wildcard test/trans-fail/*.jing)

.PHONY: all
all: $(TARGET)

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
check: check-jing check-trans-pass check-trans-fail

.PHONY: check-jing
check-jing: test-jing
	./test/all

.PHONY: test-jing
test-jing: $(TARGET) | local.mk
	$(MAKE) -C $(TESTDIR) $@

local.mk:
	@echo "# For customisation of CPPUTEST_PATH" > local.mk
	@echo "CPPUTEST_PATH=/path/to/cpputest" >> local.mk

.PHONY: check-trans-pass
check-trans-pass: $(TPASS_TESTS) $(TARGET)
	./test/tpass ./$(TARGET) $(TPASS_TESTS)

.PHONY: check-trans-fail
check-trans-fail: $(TFAIL_TESTS) $(TARGET)
	./test/tfail ./$(TARGET) $(TFAIL_TESTS)

.PHONY: clean
clean:
	$(MAKE) -C $(TESTDIR) $@
	$(RM) $(TARGET) $(OBJ) $(DEP) \
		src/lex.yy.c src/y.tab.{c,h}

-include $(DEP)
