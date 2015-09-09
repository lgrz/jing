
include ../local.mk

CXX = g++
CXXFLAGS += --std=c++11 -Wall -Wextra -pedantic -I../src \
			-I$(CPPUTEST_PATH)/include
LDFLAGS += -L$(CPPUTEST_PATH)/lib -lCppUTest
DEBUG_CXXFLAGS = -g -O0 -DDEBUG

TARGET = all
SRC = jing/main.cpp jing/symtab_test.cpp
TEST_OBJ := $(SRC:.cpp=.o)
DEP := $(SRC:.cpp=.d)

# object files from ../src
OBJDIR = ../src
OBJ = $(OBJDIR)/symtab.o $(OBJDIR)/util.o

.PHONY: test-jing
test-jing: $(TARGET)

$(TARGET): CXXFLAGS := $(CXXFLAGS) $(DEBUG_CXXFLAGS)
$(TARGET): $(OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(TARGET) $(TEST_OBJ) $(DEP)

-include $(DEP)
