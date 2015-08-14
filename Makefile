TARGET = jing2indigo

LEX = flex
YACC = bison -d
CC = gcc
CFLAGS += -Wall -Wextra -pedantic -O0 -DYYDEBUG=1 -g -Isrc
LDFLAGS +=

SRC = src/main.c
OBJ := $(SRC:.c=.o) src/parse.o
DEP := $(patsubst %.c,%.d,$(SRC)) src/parse.d

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

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJ) $(DEP) \
		src/lex.yy.c src/y.tab.{c,h}

-include $(DEP)
