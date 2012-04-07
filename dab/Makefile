all::

CXX = clang++
LD  = clang++
RM  = rm -f
CXXFLAGS = -MMD -Wall -g -O2 -march=native
LDFLAGS  = $(CXXFLAGS)
#LIB_OBJ+=

test_in : input.cc.o test_in.cc.o
submit  : all.cc.o
TARGETS=test_in submit

.PHONY: all test clean
.SECONDARY:


all:: $(TARGETS)

all.cc : input.hh input.cc test_in.cc
	cat input.hh input.cc test_in.cc | sed 's/#include ".*"//g' > all.cc

test: $(TEST_TARGET)

clean:
	$(RM) $(TARGETS) *.o $(LIB_OBJ) $(LIB_OBJ:.o=.d) $(TEST_TARGET) $(TEST_OBJECTS) all.cc

$(TARGETS):
	$(LD) $(LDFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJECTS)
	$(LD) $(LDFLAGS) -lgmock -lgtest -lgtest_main -o $@ $^

%.cc.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<


-include $(wildcard *.d)
