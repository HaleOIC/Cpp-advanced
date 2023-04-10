src = $(wildcard ./src/*.cpp)
obj = $(patsubst ./src/%.cpp, ./obj/%.o, $(src))

myArgs = -Wall -g
exe = a.out

ALL:$(exe)

$(exe): $(obj)
	g++ $^ -o $(exe)

$(obj): ./obj/%.o: ./src/%.cpp
	g++ -c $< -o $@ $(myArgs)

.PHONY: ALL

