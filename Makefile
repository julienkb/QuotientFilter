CXXFLAGS = -std=c++11 -Wall -Werror -g -O0
CXX = g++

OBJECTS = main.o quotient_filter.o

default: run-tests

run-tests: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cc quotient_filter.h

quotient_filter.o: quotient_filter.cc quotient_filter.h

clean:
	rm -f run-tests *.o *~~