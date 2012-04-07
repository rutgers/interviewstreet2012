all::

CXX = clang++
LD  = clang++
RM  = rm -f
CXXFLAGS = -MMD -Wall -g
LDFLAGS  = $(CXXFLAGS)
#LIB_OBJ+=

test_in : input.cc.o test_in.cc.o
TARGETS=test_in

.PHONY: all test clean
.SECONDARY:


all:: $(TARGETS)

test: $(TEST_TARGET)

clean:
	$(RM) $(TARGETS) $(LIB_OBJ) $(LIB_OBJ:.o=.d) $(TEST_TARGET) $(TEST_OBJECTS)

$(TARGETS):
	$(LD) $(LDFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJECTS)
	$(LD) $(LDFLAGS) -lgmock -lgtest -lgtest_main -o $@ $^

%.cc.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<


-include $(wildcard *.d)
