SOURCES=$(wildcard ../src/*.cpp)
INC=-I../include/
LIB=-lboost_regex -lboost_program_options -std=c++0x

all:
	g++ $(SOURCES) $(LIB) $(INC) -Wall -o test

doc:
	doxygen suboxTestDoxy

clean_all: clean clean_doc

clean:
	rm test

clean_doc:
	rm -rf ../doc/html/
